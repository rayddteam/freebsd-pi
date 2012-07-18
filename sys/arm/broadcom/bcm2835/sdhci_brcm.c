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
 *
 */
#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/bio.h>
#include <sys/bus.h>
#include <sys/conf.h>
#include <sys/endian.h>
#include <sys/kernel.h>
#include <sys/kthread.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/module.h>
#include <sys/mutex.h>
#include <sys/queue.h>
#include <sys/resource.h>
#include <sys/rman.h>
#include <sys/taskqueue.h>
#include <sys/time.h>
#include <sys/timetc.h>
#include <sys/watchdog.h>

#include <sys/kdb.h>

#include <machine/bus.h>
#include <machine/cpu.h>
#include <machine/cpufunc.h>
#include <machine/resource.h>
#include <machine/frame.h>
#include <machine/intr.h>

#include <dev/ofw/ofw_bus.h>
#include <dev/ofw/ofw_bus_subr.h>

#include <dev/mmc/bridge.h>
#include <dev/mmc/mmcreg.h>
#include <dev/mmc/mmcbrvar.h>

#include <dev/sdhci/sdhci.h>
#include "sdhci_if.h"

#define	DEBUG

#ifdef DEBUG
#define dprintf(fmt, args...) do { printf("%s(): ", __func__);   \
    printf(fmt,##args); } while (0)
#else
#define dprintf(fmt, args...)
#endif

struct bcm_sdhci_dmamap_arg {
	bus_addr_t		sc_dma_busaddr;
};

struct bcm_sdhci_softc {
	device_t		sc_dev;
	struct mtx		sc_mtx;
	struct resource *	sc_mem_res;
	struct resource *	sc_irq_res;
	bus_space_tag_t		sc_bst;
	bus_space_handle_t	sc_bsh;
	void *			sc_intrhand;
	struct mmc_request *	sc_req;
	struct mmc_data *	sc_data;
	uint32_t		sc_flags;
#define	LPC_SD_FLAGS_IGNORECRC		(1 << 0)
	int			sc_xfer_direction;
#define	DIRECTION_READ		0
#define	DIRECTION_WRITE		1
	int			sc_xfer_done;
	int			sc_bus_busy;
	bus_dma_tag_t		sc_dma_tag;
	bus_dmamap_t		sc_dma_map;
	bus_addr_t		sc_buffer_phys;
	void *			sc_buffer;
	struct sdhci_slot	sc_slot;
};

#define	SD_MAX_BLOCKSIZE	1024
/* XXX */

static int bcm_sdhci_probe(device_t);
static int bcm_sdhci_attach(device_t);
static int bcm_sdhci_detach(device_t);
static void bcm_sdhci_intr(void *);

static int bcm_sdhci_get_ro(device_t, device_t);

static void bcm_sdhci_dmamap_cb(void *, bus_dma_segment_t *, int, int);

#define	bcm_sdhci_lock(_sc)						\
    mtx_lock(&_sc->sc_mtx);
#define	bcm_sdhci_unlock(_sc)						\
    mtx_unlock(&_sc->sc_mtx);

static int
bcm_sdhci_probe(device_t dev)
{
	if (!ofw_bus_is_compatible(dev, "broadcom,bcm2835-sdhci"))
		return (ENXIO);

	device_set_desc(dev, "Broadcom 2708 SDHCI controller");
	return (BUS_PROBE_DEFAULT);
}

static int
bcm_sdhci_attach(device_t dev)
{
	struct bcm_sdhci_softc *sc = device_get_softc(dev);
	struct bcm_sdhci_dmamap_arg ctx;
	int rid, err;

	sc->sc_dev = dev;
	sc->sc_req = NULL;

	mtx_init(&sc->sc_mtx, "bcm sdhci", "sdhci", MTX_DEF);

	rid = 0;
	sc->sc_mem_res = bus_alloc_resource_any(dev, SYS_RES_MEMORY, &rid,
	    RF_ACTIVE);
	if (!sc->sc_mem_res) {
		device_printf(dev, "cannot allocate memory window\n");
		return (ENXIO);
	}

	sc->sc_bst = rman_get_bustag(sc->sc_mem_res);
	sc->sc_bsh = rman_get_bushandle(sc->sc_mem_res);

	rid = 0;
	sc->sc_irq_res = bus_alloc_resource_any(dev, SYS_RES_IRQ, &rid,
	    RF_ACTIVE);
	if (!sc->sc_irq_res) {
		device_printf(dev, "cannot allocate interrupt\n");
		bus_release_resource(dev, SYS_RES_MEMORY, 0, sc->sc_mem_res);
		return (ENXIO);
	}

	if (bus_setup_intr(dev, sc->sc_irq_res, INTR_TYPE_MISC | INTR_MPSAFE,
	    NULL, bcm_sdhci_intr, sc, &sc->sc_intrhand))
	{
		bus_release_resource(dev, SYS_RES_MEMORY, 0, sc->sc_mem_res);
		bus_release_resource(dev, SYS_RES_IRQ, 0, sc->sc_irq_res);
		device_printf(dev, "cannot setup interrupt handler\n");
		return (ENXIO);
	}

#if 0
	sc->sc_host.f_min = 312500;
	sc->sc_host.f_max = 2500000;
	sc->sc_host.host_ocr = MMC_OCR_300_310 | MMC_OCR_310_320 |
	    MMC_OCR_320_330 | MMC_OCR_330_340;
	sc->sc_host.caps = MMC_CAP_4_BIT_DATA;
#endif

	/* Alloc DMA memory */
	err = bus_dma_tag_create(
	    bus_get_dma_tag(sc->sc_dev),
	    4, 0,			/* alignment, boundary */
	    BUS_SPACE_MAXADDR_32BIT,	/* lowaddr */
	    BUS_SPACE_MAXADDR,		/* highaddr */
	    NULL, NULL,			/* filter, filterarg */
	    SD_MAX_BLOCKSIZE, 1,	/* maxsize, nsegments */
	    SD_MAX_BLOCKSIZE, 0,	/* maxsegsize, flags */
	    NULL, NULL,			/* lockfunc, lockarg */
	    &sc->sc_dma_tag);

	err = bus_dmamem_alloc(sc->sc_dma_tag, (void **)&sc->sc_buffer,
	    0, &sc->sc_dma_map);
	if (err) {
		device_printf(dev, "cannot allocate DMA memory\n");
		goto fail;
	}

	err = bus_dmamap_load(sc->sc_dma_tag, sc->sc_dma_map, sc->sc_buffer,
	    SD_MAX_BLOCKSIZE, bcm_sdhci_dmamap_cb, &ctx, BUS_DMA_NOWAIT);
	if (err) {
		device_printf(dev, "cannot load DMA map\n");
		goto fail;
	}

	sc->sc_buffer_phys = ctx.sc_dma_busaddr;

	/* TODO: allocate DMA here */
	sdhci_init_slot(dev, &sc->sc_slot);
	sc->sc_slot.mem_res = sc->sc_mem_res;

	bus_generic_probe(dev);
	bus_generic_attach(dev);

	return (0);

fail:
	if (sc->sc_intrhand)
		bus_teardown_intr(dev, sc->sc_irq_res, sc->sc_intrhand);
	if (sc->sc_irq_res)
		bus_release_resource(dev, SYS_RES_IRQ, 0, sc->sc_irq_res);
	if (sc->sc_mem_res)
		bus_release_resource(dev, SYS_RES_MEMORY, 0, sc->sc_mem_res);
	return (err);
}

static int
bcm_sdhci_detach(device_t dev)
{

	return (EBUSY);
}

static void
bcm_sdhci_intr(void *arg)
{
	struct bcm_sdhci_softc *sc = arg;

	sdhci_generic_intr(&sc->sc_slot);
}

static int
bcm_sdhci_get_ro(device_t bus, device_t child)
{

	return (0);
}

static void
bcm_sdhci_dmamap_cb(void *arg, bus_dma_segment_t *segs, int nseg, int err)
{
	struct bcm_sdhci_dmamap_arg *ctx;

	if (err)
		return;

	ctx = (struct bcm_sdhci_dmamap_arg *)arg;
	ctx->sc_dma_busaddr = segs[0].ds_addr;
}

static inline uint32_t
RD4(struct bcm_sdhci_softc *sc, bus_size_t off)
{
	uint32_t val = bus_space_read_4(sc->sc_bst, sc->sc_bsh, off);
	// printf("RD4 [%02x] == %08x\n", (unsigned int)off, val);
	return val;
}

static inline void
WR4(struct bcm_sdhci_softc *sc, bus_size_t off, uint32_t val)
{
	// printf("WR4 [%02x] := %08x\n", (unsigned int)off, val);
	bus_space_write_4(sc->sc_bst, sc->sc_bsh, off, val);

	if ((off != SDHCI_BUFFER && off != SDHCI_INT_STATUS && off != SDHCI_CLOCK_CONTROL))
	{
		int timeout = 100000;
		while (val != bus_space_read_4(sc->sc_bst, sc->sc_bsh, off) 
		    && --timeout > 0)
			continue;

		if (timeout <= 0)
			printf("sdhci_brcm: writing 0x%X to reg 0x%X "
				"always gives 0x%X\n",
				val, (uint32_t)off, 
				bus_space_read_4(sc->sc_bst, sc->sc_bsh, off));
	}
}

static uint8_t
bcm_sdhci_read_1(device_t dev, struct sdhci_slot *slot, bus_size_t off)
{
	struct bcm_sdhci_softc *sc = device_get_softc(dev);
	uint32_t val = RD4(sc, off & ~3);

	return ((val >> (off & 3)*8) & 0xff);
}

static uint16_t
bcm_sdhci_read_2(device_t dev, struct sdhci_slot *slot, bus_size_t off)
{
	struct bcm_sdhci_softc *sc = device_get_softc(dev);
	uint32_t val = RD4(sc, off & ~3);

	return ((val >> (off & 3)*8) & 0xffff);
}

static uint32_t
bcm_sdhci_read_4(device_t dev, struct sdhci_slot *slot, bus_size_t off)
{
	struct bcm_sdhci_softc *sc = device_get_softc(dev);

	return RD4(sc, off);
}

static void
bcm_sdhci_write_1(device_t dev, struct sdhci_slot *slot, bus_size_t off, uint8_t val)
{
	struct bcm_sdhci_softc *sc = device_get_softc(dev);
	uint32_t val32 = RD4(sc, off & ~3);
	val32 &= ~(0xff << (off & 3)*8);
	val32 |= (val << (off & 3)*8);
	WR4(sc, off & ~3, val32);
}

static void
bcm_sdhci_write_2(device_t dev, struct sdhci_slot *slot, bus_size_t off, uint16_t val)
{
	struct bcm_sdhci_softc *sc = device_get_softc(dev);
	static uint32_t cmd_and_trandfer_mode;
	uint32_t val32;
	if (off == SDHCI_COMMAND_FLAGS)
		val32 = cmd_and_trandfer_mode;
	else
		val32 = RD4(sc, off & ~3);
	val32 &= ~(0xffff << (off & 3)*8);
	val32 |= (val << (off & 3)*8);
	if (off == SDHCI_TRANSFER_MODE)
		cmd_and_trandfer_mode = val32;
	else
		WR4(sc, off & ~3, val32);
}

static void
bcm_sdhci_write_4(device_t dev, struct sdhci_slot *slot, bus_size_t off, uint32_t val)
{
	struct bcm_sdhci_softc *sc = device_get_softc(dev);
	WR4(sc, off, val);
}

static device_method_t bcm_sdhci_methods[] = {
	/* Device interface */
	DEVMETHOD(device_probe,		bcm_sdhci_probe),
	DEVMETHOD(device_attach,	bcm_sdhci_attach),
	DEVMETHOD(device_detach,	bcm_sdhci_detach),

	/* Bus interface */
	DEVMETHOD(bus_read_ivar,	sdhci_generic_read_ivar),
	DEVMETHOD(bus_write_ivar,	sdhci_generic_write_ivar),
	DEVMETHOD(bus_print_child,	bus_generic_print_child),

	/* MMC bridge interface */
	DEVMETHOD(mmcbr_update_ios,	sdhci_generic_update_ios),
	DEVMETHOD(mmcbr_request,	sdhci_generic_request),
	DEVMETHOD(mmcbr_get_ro,		bcm_sdhci_get_ro),
	DEVMETHOD(mmcbr_acquire_host,	sdhci_generic_acquire_host),
	DEVMETHOD(mmcbr_release_host,	sdhci_generic_release_host),


	/* SDHCI registers accessors */
	DEVMETHOD(sdhci_read_1,		bcm_sdhci_read_1),
	DEVMETHOD(sdhci_read_2,		bcm_sdhci_read_2),
	DEVMETHOD(sdhci_read_4,		bcm_sdhci_read_4),
	DEVMETHOD(sdhci_write_1,	bcm_sdhci_write_1),
	DEVMETHOD(sdhci_write_2,	bcm_sdhci_write_2),
	DEVMETHOD(sdhci_write_4,	bcm_sdhci_write_4),

	{ 0, 0 }
};

static devclass_t bcm_sdhci_devclass;

static driver_t bcm_sdhci_driver = {
	"sdhci",
	bcm_sdhci_methods,
	sizeof(struct bcm_sdhci_softc),
};

DRIVER_MODULE(sdhci, simplebus, bcm_sdhci_driver, bcm_sdhci_devclass, 0, 0);
