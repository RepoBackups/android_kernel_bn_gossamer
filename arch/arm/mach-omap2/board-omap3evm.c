/*
 * linux/arch/arm/mach-omap2/board-omap3evm.c
 *
 * Copyright (C) 2008 Texas Instruments
 *
 * Modified from mach-omap2/board-3430sdp.c
 *
 * Initial code: Syed Mohammed Khasim
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>

#include <asm/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <asm/arch/gpio.h>
#include <asm/arch/board.h>
#include <asm/arch/hsmmc.h>
#include <asm/arch/usb-musb.h>
#include <asm/arch/usb-ehci.h>
#include <asm/arch/common.h>

static struct omap_uart_config omap3_evm_uart_config __initdata = {
	.enabled_uarts	= ((1 << 0) | (1 << 1) | (1 << 2)),
};

static int __init omap3_evm_i2c_init(void)
{
	omap_register_i2c_bus(1, 2600, NULL, 0);
	omap_register_i2c_bus(2, 400, NULL, 0);
	omap_register_i2c_bus(3, 400, NULL, 0);
	return 0;
}

static struct omap_mmc_config omap3_evm_mmc_config __initdata = {
	.mmc [0] = {
		.enabled        = 1,
		.wire4          = 1,
	},
};

static struct platform_device omap3_evm_lcd_device = {
	.name		= "omap3evm_lcd",
	.id		= -1,
};

static struct omap_lcd_config omap3_evm_lcd_config __initdata = {
	.ctrl_name	= "internal",
};

static struct platform_device omap3_evm_twl4030rtc_device = {
	.name		= "twl4030_rtc",
	.id		= -1,
};

static void __init omap3_evm_init_irq(void)
{
	omap2_init_common_hw();
	omap_init_irq();
	omap_gpio_init();
}

static struct omap_board_config_kernel omap3_evm_config[] __initdata = {
	{ OMAP_TAG_UART,	&omap3_evm_uart_config },
	{ OMAP_TAG_MMC,		&omap3_evm_mmc_config },
	{ OMAP_TAG_LCD,		&omap3_evm_lcd_config },
};

static struct platform_device *omap3_evm_devices[] __initdata = {
	&omap3_evm_lcd_device,
#ifdef CONFIG_RTC_DRV_TWL4030
	&omap3_evm_twl4030rtc_device,
#endif
};

static void __init omap3_evm_init(void)
{
	platform_add_devices(omap3_evm_devices, ARRAY_SIZE(omap3_evm_devices));
	omap_board_config = omap3_evm_config;
	omap_board_config_size = ARRAY_SIZE(omap3_evm_config);
	omap_serial_init();
	hsmmc_init();
	usb_musb_init();
	usb_ehci_init();
	omap3evm_flash_init();
}

arch_initcall(omap3_evm_i2c_init);

static void __init omap3_evm_map_io(void)
{
	omap2_set_globals_343x();
	omap2_map_common_io();
}

MACHINE_START(OMAP3EVM, "OMAP3 EVM")
	/* Maintainer: Syed Mohammed Khasim - Texas Instruments */
	.phys_io	= 0x48000000,
	.io_pg_offst	= ((0xd8000000) >> 18) & 0xfffc,
	.boot_params	= 0x80000100,
	.map_io		= omap3_evm_map_io,
	.init_irq	= omap3_evm_init_irq,
	.init_machine	= omap3_evm_init,
	.timer		= &omap_timer,
MACHINE_END