/*-
 * Copyright (c) 2012 Damjan Marion <dmarion@Freebsd.org>
 * All rights reserved.
 *
 * Based on OMAP3 INTC code by Ben Gray
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
#include <sys/ktr.h>
#include <sys/module.h>
#include <sys/rman.h>
#include <machine/bus.h>
#include <machine/intr.h>

#include <dev/fdt/fdt_common.h>
#include <dev/ofw/openfirm.h>
#include <dev/ofw/ofw_bus.h>
#include <dev/ofw/ofw_bus_subr.h>

struct brcm_intc_softc {
	device_t		sc_dev;
	struct resource *	intc_res;
	bus_space_tag_t		intc_bst;
	bus_space_handle_t	intc_bsh;
};

static struct resource_spec brcm_intc_spec[] = {
	{ SYS_RES_MEMORY,	0,	RF_ACTIVE },
	{ -1, 0 }
};


static struct brcm_intc_softc *brcm_intc_sc = NULL;

#define	intc_read_4(reg)		\
    bus_space_read_4(brcm_intc_sc->intc_bst, brcm_intc_sc->intc_bsh, reg)
#define	intc_write_4(reg, val)		\
    bus_space_write_4(brcm_intc_sc->intc_bst, brcm_intc_sc->intc_bsh, reg, val)

static int
brcm_intc_probe(device_t dev)
{
	if (!ofw_bus_is_compatible(dev, "roadcom,bcm2835-armctrl-ic"))
		return (ENXIO);
	device_set_desc(dev, "BCM2835 Interrupt Controller");
	return (BUS_PROBE_DEFAULT);
}

static int
brcm_intc_attach(device_t dev)
{
	struct		brcm_intc_softc *sc = device_get_softc(dev);

	sc->sc_dev = dev;

	if (brcm_intc_sc)
		return (ENXIO);

	if (bus_alloc_resources(dev, brcm_intc_spec, &sc->intc_res)) {
		device_printf(dev, "could not allocate resources\n");
		return (ENXIO);
	}

	sc->intc_bst = rman_get_bustag(sc->intc_res);
	sc->intc_bsh = rman_get_bushandle(sc->intc_res);

	brcm_intc_sc = sc;

	return (0);
}

static device_method_t brcm_intc_methods[] = {
	DEVMETHOD(device_probe,		brcm_intc_probe),
	DEVMETHOD(device_attach,	brcm_intc_attach),
	{ 0, 0 }
};

static driver_t brcm_intc_driver = {
	"intc",
	brcm_intc_methods,
	sizeof(struct brcm_intc_softc),
};

static devclass_t brcm_intc_devclass;

DRIVER_MODULE(intc, simplebus, brcm_intc_driver, brcm_intc_devclass, 0, 0);

int
arm_get_next_irq(int last_irq)
{
	return -1;
}

void
arm_mask_irq(uintptr_t nb)
{
	// TODO: implement me
}

void
arm_unmask_irq(uintptr_t nb)
{
	// TODO: implement me
}
