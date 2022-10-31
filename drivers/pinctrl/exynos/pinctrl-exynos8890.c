// SPDX-License-Identifier: GPL-2.0+
/*
 * Exynos8890 pinctrl driver.
 *
 * Copyright (c) 2022 Suraaj Vashisht (suraajvashisht@gmail.com)
 *
 * based on drivers/pinctrl/exynos/pinctrl-exynos7420.c :
 * Copyright (C) 2016 Samsung Electronics
 * Thomas Abraham <thomas.ab@samsung.com>
 */

#include <command.h>
#include <dm.h>
#include <errno.h>
#include <asm/io.h>
#include <dm/pinctrl.h>
#include <dm/root.h>
#include <fdtdec.h>
#include <asm/arch/pinmux.h>
#include "pinctrl-exynos.h"

static const struct pinctrl_ops exynos8890_pinctrl_ops = {
	.set_state = exynos_pinctrl_set_state
};

/* pin banks of exynos8890 pin-controller 0 (ALIVE) */
static const struct samsung_pin_bank_data exynos8890_pin_banks0[] = {
	EXYNOS_PIN_BANK(8, 0x000, "gpa0"),
	EXYNOS_PIN_BANK(8, 0x020, "gpa1"),
	EXYNOS_PIN_BANK(8, 0x040, "gpa2"),
	EXYNOS_PIN_BANK(8, 0x060, "gpa3"),
};

/* pin banks of exynos8890 pin-controller 1 (AUD) */
static const struct samsung_pin_bank_data exynos8890_pin_banks1[] = {
	EXYNOS_PIN_BANK(7, 0x000, "gph0"),
};

/* pin banks of exynos8890 pin-controller 2 (CCORE) */
static const struct samsung_pin_bank_data exynos8890_pin_banks2[] = {
	EXYNOS_PIN_BANK(2, 0x000, "etc0"),
};

/* pin banks of exynos8890 pin-controller 4 (FP) */
static const struct samsung_pin_bank_data exynos8890_pin_banks4[] = {
	EXYNOS_PIN_BANK(4, 0x000, "gpf2"),
};

/* pin banks of exynos8890 pin-controller 5 (FSYS0) */
static const struct samsung_pin_bank_data exynos8890_pin_banks5[] = {
	EXYNOS_PIN_BANK(4, 0x000, "gpi1"),
	EXYNOS_PIN_BANK(8, 0x020, "gpi2"),
};

/* pin banks of exynos8890 pin-controller 6 (FSYS1) */
static const struct samsung_pin_bank_data exynos8890_pin_banks6[] = {
	EXYNOS_PIN_BANK(7, 0x000, "gpj0"),
};

/* pin banks of exynos8890 pin-controller 8 (PERIC0) */
static const struct samsung_pin_bank_data exynos8890_pin_banks8[] = {
	EXYNOS_PIN_BANK(6, 0x000, "gpi0"),
	EXYNOS_PIN_BANK(8, 0x020, "gpd0"),
	EXYNOS_PIN_BANK(6, 0x040, "gpd1"),
	EXYNOS_PIN_BANK(4, 0x060, "gpd2"),
	EXYNOS_PIN_BANK(4, 0x080, "gpd3"),
	EXYNOS_PIN_BANK(2, 0x0A0, "gpb1"),
	EXYNOS_PIN_BANK(2, 0x0C0, "gpb2"),
	EXYNOS_PIN_BANK(3, 0x0E0, "gpb0"),
	EXYNOS_PIN_BANK(5, 0x100, "gpc0"),
	EXYNOS_PIN_BANK(5, 0x120, "gpc1"),
	EXYNOS_PIN_BANK(6, 0x140, "gpc2"),
	EXYNOS_PIN_BANK(8, 0x160, "gpc3"),
	EXYNOS_PIN_BANK(4, 0x180, "gpk0"),
	EXYNOS_PIN_BANK(7, 0x1A0, "etc1"),
};

/* pin banks of exynos8890 pin-controller 9 (PERIC1) */
static const struct samsung_pin_bank_data exynos8890_pin_banks9[] = {
	EXYNOS_PIN_BANK(8, 0x000, "gpe0"),
	EXYNOS_PIN_BANK(8, 0x020, "gpe5"),
	EXYNOS_PIN_BANK(8, 0x040, "gpe6"),
	EXYNOS_PIN_BANK(8, 0x060, "gpj1"),
	EXYNOS_PIN_BANK(2, 0x080, "gpj2"),
	EXYNOS_PIN_BANK(8, 0x0A0, "gpe2"),
	EXYNOS_PIN_BANK(8, 0x0C0, "gpe3"),
	EXYNOS_PIN_BANK(8, 0x0E0, "gpe4"),
	EXYNOS_PIN_BANK(8, 0x100, "gpe1"),
	EXYNOS_PIN_BANK(4, 0x120, "gpe7"),
	EXYNOS_PIN_BANK(3, 0x140, "gpg0"),
};

const struct samsung_pin_ctrl exynos8890_pin_ctrl[] = {
	{
		/* pin-controller instance 0 ALIVE data */
		.pin_banks  = exynos8890_pin_banks0,
		.nr_banks   = ARRAY_SIZE(exynos8890_pin_banks0),
	}, {
		/* pin-controller instance 1 AUD data */
		.pin_banks  = exynos8890_pin_banks1,
		.nr_banks   = ARRAY_SIZE(exynos8890_pin_banks1),
	}, {
		/* pin-controller instance 2 CCORE data */
		.pin_banks  = exynos8890_pin_banks2,
		.nr_banks   = ARRAY_SIZE(exynos8890_pin_banks2),
	}, {
		/* pin-controller instance 4 FP data */
		.pin_banks  = exynos8890_pin_banks4,
		.nr_banks   = ARRAY_SIZE(exynos8890_pin_banks4),
	}, {
		/* pin-controller instance 5 FSYS0 data */
		.pin_banks  = exynos8890_pin_banks5,
		.nr_banks   = ARRAY_SIZE(exynos8890_pin_banks5),
	}, {
		/* pin-controller instance 6 FSYS1 data */
		.pin_banks  = exynos8890_pin_banks6,
		.nr_banks   = ARRAY_SIZE(exynos8890_pin_banks6),
	}, {
		/* pin-controller instance 8 PERIC0 data */
		.pin_banks  = exynos8890_pin_banks8,
		.nr_banks   = ARRAY_SIZE(exynos8890_pin_banks8),
	}, {
		/* pin-controller instance 9 PERIC1 data */
		.pin_banks  = exynos8890_pin_banks9,
		.nr_banks   = ARRAY_SIZE(exynos8890_pin_banks9),
	},
	{/* list termiantor */}
};

static const struct udevice_id exynos8890_pinctrl_ids[] = {
	{ .compatible = "samsung,exynos8890-pinctrl",
		.data = (ulong)exynos8890_pin_ctrl },
	{ }
};

U_BOOT_DRIVER(pinctrl_exynos8890) = {
	.name		= "pinctrl_exynos8890",
	.id		= UCLASS_PINCTRL,
	.of_match	= exynos8890_pinctrl_ids,
	.priv_auto = sizeof(struct exynos_pinctrl_priv),
	.ops		= &exynos8890_pinctrl_ops,
	.probe		= exynos_pinctrl_probe,
};
