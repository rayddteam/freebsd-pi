/*-
 * Copyright (c) 2012 Oleksandr Tymoshenko <gonzo@freebsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/bus.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/malloc.h>
#include <sys/rman.h>
#include <sys/timeet.h>
#include <sys/timetc.h>
#include <sys/watchdog.h>
#include <machine/bus.h>
#include <machine/cpu.h>
#include <machine/frame.h>
#include <machine/intr.h>

#include <dev/fdt/fdt_common.h>
#include <dev/ofw/openfirm.h>
#include <dev/ofw/ofw_bus.h>
#include <dev/ofw/ofw_bus_subr.h>

#include <machine/bus.h>
#include <machine/fdt.h>
#include <arm/broadcom/bcm2835/dma.h>

#define	DMA_LOCK	do {			\
	mtx_lock(&brcm_dma_sc->lock);	\
} while(0)

#define	DMA_UNLOCK	do {			\
	mtx_unlock(&brcm_dma_sc->lock);	\
} while(0)

#define  DEBUG
#ifdef  DEBUG
#define dprintf(fmt, args...) printf(fmt, ##args)
#else
#define dprintf(fmt, args...)
#endif

#define	DMA_REG_CS(chan)		(0x100*(chan) + 0x00)
#define		REG_CS_RESET			(1 << 31)
#define		REG_CS_ABORT			(1 << 30)
#define		REG_CS_DISDEBUG			(1 << 29)
#define		REG_CS_WAIT_WRITES		(1 << 28)
#define		REG_CS_PANIC_PRI_SHIFT	20
#define		REG_CS_PANIC_PRI_MASK	0xf
#define		REG_CS_PRI_SHIFT		16
#define		REG_CS_PRI_MASK			0xf
#define		REG_CS_ERROR			(1 <<  8)
#define		REG_CS_WAITING_WRITES	(1 <<  6)
#define		REG_CS_DREQ_STOPS_DMA	(1 <<  5)
#define		REG_CS_PAUSED			(1 <<  4)
#define		REG_CS_DREQ				(1 <<  3)
#define		REG_CS_INT				(1 <<  2)
#define		REG_CS_END				(1 <<  1)
#define		REG_CS_ACTIVE			(1 <<  0)
#define	DMA_REG_CB_ADDR(chan)	(0x100*(chan) + 0x04)
#define	DMA_REG_TI(chan)	(0x100*(chan) + 0x08)
#define		REG_TI_NO_WIDE_BURSTS	(1 << 26)
#define		REG_TI_WAITS_SHIFT		21
#define		REG_TI_WAITS_MASK		0x1f
#define		REG_TI_PERMAP_SHIFT		16
#define		REG_TI_PERMAP_MASK		0x1f
#define		REG_TI_BURST_LEN_SHIFT	12
#define		REG_TI_BURST_LEN_MASK	7
#define		REG_TI_SRC_IGNORE		(1 << 11)
#define		REG_TI_SRC_DREQ			(1 << 10)
#define		REG_TI_SRC_WIDTH		(1 <<  9)
#define		REG_TI_SRC_INC			(1 <<  8)
#define		REG_TI_DST_IGNORE		(1 <<  7)
#define		REG_TI_DST_DREQ			(1 <<  6)
#define		REG_TI_DST_WIDTH		(1 <<  5)
#define		REG_TI_DST_INC			(1 <<  4)
#define		REG_TI_WAIT_RESP		(1 <<  3)
#define		REG_TI_TD_MODE			(1 <<  1)
#define		REG_TI_INTEN			(1 <<  0)
#define	DMA_REG_SRC_ADDR(chan)	(0x100*(chan) + 0x10)
#define	DMA_REG_DST_ADDR(chan)	(0x100*(chan) + 0x14)
#define	DMA_REG_STRIDE(chan)	(0x100*(chan) + 0x18)
#define	DMA_REG_NEXT_CB(chan)	(0x100*(chan) + 0x1C)
#define	DMA_REG_DEBUG(chan)		(0x100*(chan) + 0x20)
#define	DMA_REG_INT_STATUS		0xfe0
#define	DMA_REG_ENABLE			0xff0

#define	DMA_MAX_CHANNEL	14
#define	DMA_IRQS	12

struct dma_cb
{
	uint32_t		cb_ti;		/* transfer information */
	uint32_t		cb_src_pa;	/* source address */
	uint32_t		cb_dst_pa;	/* destination address */
	uint32_t		cb_length;	/* transfer length */
	uint32_t		cb_stride;	/* sride for 2D mode */
	uint32_t		cb_next;	/* next CB address */
	uint32_t		cb_reserved[2];	/* set to zero */
};

struct brcm_dma_softc {
	struct mtx		lock;
	struct resource *	mem_res;
	struct resource *	irq_res[DMA_IRQS];
	void*			intr_hl[DMA_IRQS];
	bus_space_tag_t		bst;
	bus_space_handle_t	bsh;
};

static struct brcm_dma_softc *brcm_dma_sc = NULL;

#define	dma_read_4(reg)		\
    bus_space_read_4(brcm_dma_sc->bst, brcm_dma_sc->bsh, reg)
#define	dma_write_4(reg, val)		\
    bus_space_write_4(brcm_dma_sc->bst, brcm_dma_sc->bsh, reg, val)

/*
 * We're interested only in channels 2 and 3
 */
static struct resource_spec bcm_dma_irq_spec[] = {
	{ SYS_RES_IRQ,	 0,	RF_ACTIVE },
	{ SYS_RES_IRQ,	 1,	RF_ACTIVE },
	{ -1,		 0,	0 }
};

static void
brcm_dma_intr(void *arg)
{

	DMA_LOCK;
	// TODO: implement 
	DMA_UNLOCK;
}

static int
brcm_dma_probe(device_t dev)
{

	if (ofw_bus_is_compatible(dev, "broadcom,bcm2835-dma")) {
		device_set_desc(dev, "BCM2835 DMA Controller");
		return(BUS_PROBE_DEFAULT);
	}

	return (ENXIO);
}

static int
brcm_dma_attach(device_t dev)
{
	struct brcm_dma_softc *sc = device_get_softc(dev);
	int i;
	int rid = 0;
	int err;

	if (brcm_dma_sc != NULL)
		return (EINVAL);

	sc->mem_res = bus_alloc_resource_any(dev, SYS_RES_MEMORY, &rid, RF_ACTIVE);
	if (sc->mem_res == NULL) {
		device_printf(dev, "could not allocate memory resource\n");
		return (ENXIO);
	}

	sc->bst = rman_get_bustag(sc->mem_res);
	sc->bsh = rman_get_bushandle(sc->mem_res);

	rid = 0;
	err = bus_alloc_resources(dev, bcm_dma_irq_spec, sc->irq_res);
	if (err) {
		device_printf(dev, "could not allocate interrupt resource\n");
		return (ENXIO);
	}

	/* Setup and enable the timer */
	for (i = 0; bcm_dma_irq_spec[i].type != -1; i++) {
		if (bus_setup_intr(dev, sc->irq_res[i], INTR_TYPE_MISC | INTR_MPSAFE,
				NULL, brcm_dma_intr, sc,
				&sc->intr_hl[i]) != 0) {
			device_printf(dev, "Unable to setup the clock irq handler.\n");
			goto fail;
		}
	}

	mtx_init(&sc->lock, "vcio dma", MTX_DEF, 0);

	brcm_dma_sc = sc;

	return (0);
fail:
	/* TODO: teardown */
	bus_release_resources(dev, bcm_dma_irq_spec, sc->irq_res);

	return (ENXIO);
}

static device_method_t brcm_dma_methods[] = {
	DEVMETHOD(device_probe,		brcm_dma_probe),
	DEVMETHOD(device_attach,	brcm_dma_attach),
	{ 0, 0 }
};

static driver_t brcm_dma_driver = {
	"dma",
	brcm_dma_methods,
	sizeof(struct brcm_dma_softc),
};

static devclass_t brcm_dma_devclass;

DRIVER_MODULE(dma, simplebus, brcm_dma_driver, brcm_dma_devclass, 0, 0);
