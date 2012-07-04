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

#include <dev/fdt/fdt_common.h>
#include <dev/ofw/ofw_bus.h>
#include <dev/ofw/ofw_bus_subr.h>

#include <arm/broadcom/bcm2835/mbox.h>
#include <arm/broadcom/bcm2835/vcbus.h>

struct brcm_fb_config {
	uint32_t	xres;
	uint32_t	yres;
	uint32_t	vxres;
	uint32_t	vyres;
	uint32_t	pitch;
	uint32_t	bpp;
	uint32_t	xoffset;
	uint32_t	yoffset;
	/* Filled by videocore */
	uint32_t	base;
	uint32_t	screen_size;
};

struct brcm_fb_softc {
	device_t		dev;
	struct cdev *		cdev;
	struct mtx		mtx;
	bus_dma_tag_t		dma_tag;
	bus_dmamap_t		dma_map;
	struct brcm_fb_config*	fb_config;
	bus_addr_t		fb_config_phys;
	struct intr_config_hook	init_hook;
};

#define	brcm_fb_lock(_sc)	mtx_lock(&(_sc)->mtx)
#define	brcm_fb_unlock(_sc)	mtx_unlock(&(_sc)->mtx)
#define	brcm_fb_lock_assert(sc)	mtx_assert(&(_sc)->mtx, MA_OWNED)

static int brcm_fb_probe(device_t);
static int brcm_fb_attach(device_t);
static void brcm_fb_dmamap_cb(void *arg, bus_dma_segment_t *segs, int nseg, int err);

static void
brcm_fb_init(void *arg)
{
	struct brcm_fb_softc *sc = arg;
	int err;
	volatile struct brcm_fb_config*	fb_config = sc->fb_config;

	/* TODO: replace it with FDT stuff */
	fb_config->xres = 1184;
	fb_config->yres = 928;
	fb_config->vxres = 0;
	fb_config->vyres = 0;
	fb_config->xoffset = 0;
	fb_config->yoffset = 0;
	fb_config->bpp = 24;
	fb_config->base = 0;
	fb_config->pitch = 0;
	fb_config->screen_size = 0;

	bus_dmamap_sync(sc->dma_tag, sc->dma_map,
		BUS_DMASYNC_PREWRITE | BUS_DMASYNC_PREREAD);
	brcm_mbox_write(BCM2835_MBOX_CHAN_FB, sc->fb_config_phys);
	brcm_mbox_read(BCM2835_MBOX_CHAN_FB, &err);
	bus_dmamap_sync(sc->dma_tag, sc->dma_map,
		BUS_DMASYNC_POSTREAD);

	if (err == 0) {
		device_printf(sc->dev, "%dx%d(%dx%d@%d,%d) %dbpp\n", 
			fb_config->xres, fb_config->yres,
			fb_config->vxres, fb_config->vyres,
			fb_config->xoffset, fb_config->yoffset,
			fb_config->bpp);


		device_printf(sc->dev, "pitch %d, base 0x%08x, screen_size %d\n", 
			fb_config->pitch, fb_config->base,
			fb_config->screen_size);
	}
	else
		device_printf(sc->dev, "Failed to set framebuffer info\n");

	config_intrhook_disestablish(&sc->init_hook);
}

static int
brcm_fb_probe(device_t dev)
{
	if (!ofw_bus_is_compatible(dev, "broadcom,bcm2835-fb"))
		return (ENXIO);

	device_set_desc(dev, "BCM2835 framebuffer device");
	return (BUS_PROBE_DEFAULT);
}

static int
brcm_fb_attach(device_t dev)
{
	struct brcm_fb_softc *sc = device_get_softc(dev);
	int dma_size = sizeof(struct brcm_fb_config);
	int err;

	sc->dev = dev;
	mtx_init(&sc->mtx, "bcm2835fb", "fb", MTX_DEF);

	err = bus_dma_tag_create(
	    bus_get_dma_tag(sc->dev),
	    PAGE_SIZE, 0,		/* alignment, boundary */
	    BUS_SPACE_MAXADDR_32BIT,	/* lowaddr */
	    BUS_SPACE_MAXADDR,		/* highaddr */
	    NULL, NULL,			/* filter, filterarg */
	    dma_size, 1,		/* maxsize, nsegments */
	    dma_size, 0,		/* maxsegsize, flags */
	    NULL, NULL,			/* lockfunc, lockarg */
	    &sc->dma_tag);

	err = bus_dmamem_alloc(sc->dma_tag, (void **)&sc->fb_config,
	    0, &sc->dma_map);
	if (err) {
		device_printf(dev, "cannot allocate framebuffer\n");
		goto fail;
	}

	err = bus_dmamap_load(sc->dma_tag, sc->dma_map, sc->fb_config,
	    dma_size, brcm_fb_dmamap_cb, &sc->fb_config_phys, BUS_DMA_NOWAIT);

	if (err) {
		device_printf(dev, "cannot load DMA map\n");
		goto fail;
	}

	/* 
	 * We have to wait until interrupts are enabled. 
	 * Mailbox relies on it to get data from VideoCore
	 */
        sc->init_hook.ich_func = brcm_fb_init;
        sc->init_hook.ich_arg = sc;

        if (config_intrhook_establish(&sc->init_hook) != 0)
                return (ENOMEM);

	return (0);

fail:
	return (ENXIO);
}


static void
brcm_fb_dmamap_cb(void *arg, bus_dma_segment_t *segs, int nseg, int err)
{
	bus_addr_t *addr;

	if (err)
		return;

	addr = (bus_addr_t*)arg;
	*addr = PHYS_TO_VCBUS(segs[0].ds_addr);
}

static device_method_t brcm_fb_methods[] = {
	/* Device interface */
	DEVMETHOD(device_probe,		brcm_fb_probe),
	DEVMETHOD(device_attach,	brcm_fb_attach),

	{ 0, 0 }
};

static devclass_t brcm_fb_devclass;

static driver_t brcm_fb_driver = {
	"fb",
	brcm_fb_methods,
	sizeof(struct brcm_fb_softc),
};

DRIVER_MODULE(bcm2835fb, simplebus, brcm_fb_driver, brcm_fb_devclass, 0, 0);
