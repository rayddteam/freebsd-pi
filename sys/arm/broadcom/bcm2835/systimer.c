/*-
 * Copyright (c) 2012 Oleksandr Tymoshenko <gonzo@freebsd.org>
 * Copyright (c) 2012 Damjan Marion <dmarion@freebsd.org>
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

#define	BCM2835_NUM_TIMERS	4

#define	BCM2835_SYSTIMER_CS	0x00
#define	BCM2835_SYSTIMER_CLO	0x04
#define	BCM2835_SYSTIMER_CHI	0x08
#define	BCM2835_SYSTIMER_C0	0x0C
#define	BCM2835_SYSTIMER_C1	0x10
#define	BCM2835_SYSTIMER_C2	0x14
#define	BCM2835_SYSTIMER_C3	0x18

struct brcm_systimer_softc {
	struct resource *	tmr_mem_res;
	struct resource *	tmr_irq_res;
	uint32_t		sysclk_freq;
	bus_space_tag_t		bst;
	bus_space_handle_t	bsh;
	struct eventtimer       et[BCM2835_NUM_TIMERS];
};

#if 0
static struct resource_spec brcm_systimer_mem_spec[] = {
	{ SYS_RES_MEMORY,   0,  RF_ACTIVE },
	{ -1,               0,  0 }
};
static struct resource_spec brcm_systimer_irq_spec[] = {
	{ SYS_RES_IRQ,      0,  RF_ACTIVE },
	{ -1,               0,  0 }
};
#endif

static struct brcm_systimer_softc *brcm_systimer_sc = NULL;

/* Read/Write macros for Timer used as timecounter */
#define brcm_systimer_tc_read_4(reg)		\
	bus_space_read_4(brcm_systimer_sc->bst, \
		brcm_systimer_sc->bsh, reg)

#define brcm_systimer_tc_write_4(reg, val)	\
	bus_space_write_4(brcm_systimer_sc->bst, \
		brcm_systimer_sc->bsh, reg, val)

static unsigned brcm_systimer_tc_get_timecount(struct timecounter *);

static struct timecounter brcm_systimer_tc = {
	.tc_name           = "BCM2835 Timecouter",
	.tc_get_timecount  = brcm_systimer_tc_get_timecount,
	.tc_poll_pps       = NULL,
	.tc_counter_mask   = ~0u,
	.tc_frequency      = 0,
	.tc_quality        = 1000,
};

static unsigned
brcm_systimer_tc_get_timecount(struct timecounter *tc)
{
	return brcm_systimer_tc_read_4(BCM2835_SYSTIMER_CLO);
}

#if 0
static int
brcm_systimer_start(struct eventtimer *et, struct bintime *first,
              struct bintime *period)
{
	printf("Implement me\n");
	return (0);
}

static int
brcm_systimer_stop(struct eventtimer *et)
{
	printf("Implement me\n");
	return (0);
}

static int
brcm_systimer_intr(void *arg)
{
	printf("Implement me\n");

	return (FILTER_HANDLED);
}
#endif

static int
brcm_systimer_probe(device_t dev)
{
	struct	brcm_systimer_softc *sc;
	sc = (struct brcm_systimer_softc *)device_get_softc(dev);

	if (ofw_bus_is_compatible(dev, "broadcom,bcm2835-system-timer")) {
		device_set_desc(dev, "BCM2835 System Timer");
		return(BUS_PROBE_DEFAULT);
	}

	return (ENXIO);
}

static int
brcm_systimer_attach(device_t dev)
{
	// struct brcm_systimer_softc *sc = device_get_softc(dev);

	if (brcm_systimer_sc != NULL)
		return (EINVAL);

	printf("Implement me\n");

	return (ENXIO);
}

static device_method_t brcm_systimer_methods[] = {
	DEVMETHOD(device_probe,		brcm_systimer_probe),
	DEVMETHOD(device_attach,	brcm_systimer_attach),
	{ 0, 0 }
};

static driver_t brcm_systimer_driver = {
	"bcm2835_systimer",
	brcm_systimer_methods,
	sizeof(struct brcm_systimer_softc),
};

static devclass_t brcm_systimer_devclass;

DRIVER_MODULE(brcm_systimer, simplebus, brcm_systimer_driver, brcm_systimer_devclass, 0, 0);

void
cpu_initclocks(void)
{
}

void
DELAY(int usec)
{
	int32_t counts;
	uint32_t first, last;

	if (brcm_systimer_sc == NULL) {
		for (; usec > 0; usec--)
			for (counts = 200; counts > 0; counts--)
				/* Prevent gcc from optimizing  out the loop */
				cpufunc_nullop();
		return;
	}

	/* Get the number of times to count */
	counts = usec * ((brcm_systimer_tc.tc_frequency / 1000000) + 1);;

	first = brcm_systimer_tc_read_4(BCM2835_SYSTIMER_CLO);

	while (counts > 0) {
		last = brcm_systimer_tc_read_4(BCM2835_SYSTIMER_CLO);
		if (last>first) {
			counts -= (int32_t)(last - first);
		} else {
			counts -= (int32_t)((0xFFFFFFFF - first) + last);
		}
		first = last;
	}
}

