/* linux/arch/arm/mach-s3c2410/mach-h1940.c
 *
 * Copyright (c) 2003-2005 Simtec Electronics
 *   Ben Dooks <ben@simtec.co.uk>
 *
 * http://www.handhelds.org/projects/h1940.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *     16-May-2003 BJD  Created initial version
 *     16-Aug-2003 BJD  Fixed header files and copyright, added URL
 *     05-Sep-2003 BJD  Moved to v2.6 kernel
 *     06-Jan-2003 BJD  Updates for <arch/map.h>
 *     18-Jan-2003 BJD  Added serial port configuration
 *     17-Feb-2003 BJD  Copied to mach-ipaq.c
 *     21-Aug-2004 BJD  Added struct s3c2410_board
 *     04-Sep-2004 BJD  Changed uart init, renamed ipaq_ -> h1940_
 *     18-Oct-2004 BJD  Updated new board structure name
 *     04-Nov-2004 BJD  Change for new serial clock
 *     04-Jan-2005 BJD  Updated uart init call
 *     10-Jan-2005 BJD  Removed include of s3c2410.h
 *     14-Jan-2005 BJD  Added clock init
 *     12-Dec-2006 lili Add Nand part for yc2440 board
 *     13-Dec-2006 lili Add NEC LCD driver parameter
 *     14-Dec-2006 lili Add SUMSANG LCD driver parameter
*/



#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/device.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <asm/hardware.h>
#include <asm/hardware/iomd.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mach-types.h>

//#include <asm/debug-ll.h>
#include <asm/arch/regs-serial.h>
#include <asm/arch/regs-gpio.h>
#include <asm/arch/regs-lcd.h>
#include <asm/arch/regs-timer.h>

#include <asm/arch/fb.h>
#include <asm/arch/udc.h>
#include <asm/arch/ts.h>
#include <asm/arch/lcd.h>

#include <linux/serial_core.h>

#include <asm/arch/map.h> 
#include <asm/arch/bit-map.h>

#include <asm/arch/nand.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>


#include "clock.h"
#include "devs.h"
#include "cpu.h"

#include "usb-simtec.h"
#include <asm/arch/usb-control.h>
#include <asm/arch/bast-map.h>
#include <asm/arch/bast-irq.h>

static struct map_desc bit2440_iodesc[] __initdata = {
	{vSMDK2410_ETH_IO, pSMDK2410_ETH_IO, SZ_1M, MT_DEVICE},
  { (unsigned long)S3C24XX_VA_IIS, S3C2410_PA_IIS, S3C24XX_SZ_IIS, MT_DEVICE },   
};

#define UCON S3C2410_UCON_DEFAULT | S3C2410_UCON_UCLK
#define ULCON S3C2410_LCON_CS8 | S3C2410_LCON_PNONE | S3C2410_LCON_STOPB
#define UFCON S3C2410_UFCON_RXTRIG8 | S3C2410_UFCON_FIFOMODE



static struct s3c2410_uartcfg bit2440_uartcfgs[] = {
	[0] = {
		.hwport	     = 0,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	[1] = {
		.hwport	     = 1,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	/* IR port */
	[2] = {
		.hwport	     = 2,
		.flags	     = 0,
		//.uart_flags  = UPF_CONS_FLOW,//yu
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,//yu
		.ufcon	     = 0x51,
	}
};


static void pullup(unsigned char cmd)
{
	printk(KERN_DEBUG "udc: pullup(%d)\n",cmd);
	switch (cmd)
	{
		case S3C2410_UDC_P_ENABLE :
			break;
		case S3C2410_UDC_P_DISABLE :
			break;
		case S3C2410_UDC_P_RESET :
			break;
		default: break;
	}
}

static struct s3c2410_udc_mach_info bit2440_udc_cfg __initdata = {
		.udc_command = pullup,
};


static struct s3c2410_ts_mach_info bit2440_ts_cfg __initdata = {
		.delay = 10000,
		.presc = 49,
		.oversampling_shift = 2,
};

/* LCD driver info */
//===================================================================
// NEC LCD parameter: add by lili <bit.lili@gmail>
//===================================================================

#if defined(CONFIG_FB_S3C2410_240X320_NEC)
static struct s3c2410fb_mach_info bit2440_lcdcfg __initdata = {
	.regs	= {
		.lcdcon1 =	S3C2410_LCDCON1_TFT16BPP |S3C2410_LCDCON1_TFT |S3C2410_LCDCON1_CLKVAL(0x04),
		
		.lcdcon2 =	S3C2410_LCDCON2_VBPD(1) |S3C2410_LCDCON2_VFPD(5) |S3C2410_LCDCON2_VSPW(1),
		.lcdcon3 =	S3C2410_LCDCON3_HBPD(13) |S3C2410_LCDCON3_HFPD(15),
		.lcdcon4 =	S3C2410_LCDCON4_MVAL(13) |S3C2410_LCDCON4_HSPW(5),
		
		.lcdcon5 =	S3C2410_LCDCON5_FRM565 |S3C2410_LCDCON5_INVVLINE |S3C2410_LCDCON5_INVVFRAME |S3C2410_LCDCON5_PWREN |S3C2410_LCDCON5_HWSWP,
	},

	.lpcsel =	0xf82,

	.gpccon =	0xaa955699,
	.gpccon_mask =	0xffc003cc,
	.gpcup =	0x0000ffff,
	.gpcup_mask =	0xffffffff,

	.gpdcon =	0xaa95aaa1,
	.gpdcon_mask =	0xffc0fff0,
	.gpdup =	0x0000faff,
	.gpdup_mask =	0xffffffff,

	.fixed_syncs =	1,
	.width  =	240,
	.height =	320,

	.xres	= {
		.min =		240,
		.max =		240,
		.defval =	240,
	},

	.yres	= {
		.max =		320,
		.min =		320,
		.defval	=	320,
	},

	.bpp	= {
		.min =		16,
		.max =		16,
		.defval =	16,
	},
};
#endif
//===================================================================
// SUMSANG LCD Parameter: add by lili <bit.lili@gmail.com>
//===================================================================

#if	defined(CONFIG_FB_S3C2410_320X240_SAMSUNG)
static struct s3c2410fb_mach_info bit2440_lcdcfg __initdata = {
	.regs	= {
		.lcdcon1 =	S3C2410_LCDCON1_TFT16BPP | \
				S3C2410_LCDCON1_TFT | \
				S3C2410_LCDCON1_CLKVAL(0x03),

		.lcdcon2 =	S3C2410_LCDCON2_VBPD(3) | \
				S3C2410_LCDCON2_LINEVAL(239) | \
				S3C2410_LCDCON2_VFPD(5) | \
				S3C2410_LCDCON2_VSPW(15),

		.lcdcon3 =	S3C2410_LCDCON3_HBPD(5) | \
				S3C2410_LCDCON3_HOZVAL(319) | \
				S3C2410_LCDCON3_HFPD(15),

		.lcdcon4 =	S3C2410_LCDCON4_MVAL(13) | \
				S3C2410_LCDCON4_HSPW(8),

		.lcdcon5 =	S3C2410_LCDCON5_FRM565 |
				S3C2410_LCDCON5_INVVLINE |
				S3C2410_LCDCON5_INVVFRAME |
				S3C2410_LCDCON5_PWREN |
				S3C2410_LCDCON5_HWSWP,
	},

	.lpcsel =	0xf82,

	.gpccon =	0xaa955699,
	.gpccon_mask =	0xffc003cc,
	.gpcup =	0x0000ffff,
	.gpcup_mask =	0xffffffff,

	.gpdcon =	0xaa95aaa1,
	.gpdcon_mask =	0xffc0fff0,
	.gpdup =	0x0000faff,
	.gpdup_mask =	0xffffffff,

	.fixed_syncs =	1,
	.width  =	320,
	.height =	240,

	.xres	= {
		.min =		320,
		.max =		320,
		.defval =	320,
	},

	.yres	= {
		.max =		240,
		.min =		240,
		.defval	=	240,
	},

	.bpp	= {
		.min =		16,
		.max =		16,
		.defval =	16,
	},
};
#endif


#ifdef  GDEBUG
#    define gprintk( x... )  printk( x )
#else
#    define gprintk( x... )
#endif


//===================================================================
// nand flash: add by lili
//===================================================================

static int chip0_map[] = { 0 }; 	//add by lili
struct mtd_partition bit_default_nand_part[] = {
[0] = { 
.name = "bootloader", 
.offset = 0x00000000, 
.size = 0x00030000, 
},
[1] = { 
.name = "kernel", 
.offset = 0x00030000, 
.size = 0x00200000, 
},
[2] = { 
.name = "root", 
.offset = 0x00230000, 
.size = 0x03dcc000, 
}
};

/* the bit has 1 selectable slots for nand-flash, the three
 * on-board chip areas, as well as the external SmartMedia
 * slot.
 *
 * Note, there is no current hot-plug support for the SmartMedia
 * socket.
*/

static struct s3c2410_nand_set bit_nand_sets[] = {
	[0] = {
		.name		= "chip0",
		.nr_chips	= 1,
		.nr_map		= chip0_map,
		.nr_partitions	= ARRAY_SIZE(bit_default_nand_part),
		.partitions	= bit_default_nand_part
	},
};


static struct s3c2410_platform_nand bit_nand_info = {
	.tacls		= 0,
	.twrph0		= 30,
	.twrph1		= 0,
	.nr_sets	= ARRAY_SIZE(bit_nand_sets),
	.sets		= bit_nand_sets,
};


/*	Add by lili for USB setup
 *
 */
static unsigned int power_state[2];

static void
usb_simtec_powercontrol(int port, int to)
{
	pr_debug("usb_simtec_powercontrol(%d,%d)\n", port, to);

	power_state[port] = to;

	if (power_state[0] && power_state[1])
		s3c2410_gpio_setpin(S3C2410_GPB4, 0);
	else
		s3c2410_gpio_setpin(S3C2410_GPB4, 1);
}

static irqreturn_t
usb_simtec_ocirq(int irq, void *pw, struct pt_regs *regs)
{
	struct s3c2410_hcd_info *info = (struct s3c2410_hcd_info *)pw;

	if (s3c2410_gpio_getpin(S3C2410_GPG10) == 0) {
		pr_debug("usb_simtec: over-current irq (oc detected)\n");
		s3c2410_usb_report_oc(info, 3);
	} else {
		pr_debug("usb_simtec: over-current irq (oc cleared)\n");
		s3c2410_usb_report_oc(info, 0);
	}

	return IRQ_HANDLED;
}

static void usb_simtec_enableoc(struct s3c2410_hcd_info *info, int on)
{
	int ret;

	if (on) {
		ret = request_irq(IRQ_USBOC, usb_simtec_ocirq, SA_INTERRUPT,
				  "USB Over-current", info);
		if (ret != 0) {
			printk(KERN_ERR "failed to request usb oc irq\n");
		}

		set_irq_type(IRQ_USBOC, IRQT_BOTHEDGE);
	} else {
		free_irq(IRQ_USBOC, info);
	}
}

static struct s3c2410_hcd_info usb_simtec_info = {
	.port[0]	= {
		.flags	= S3C_HCDFLG_USED
	},
	.port[1]	= {
		.flags	= S3C_HCDFLG_USED
	},

	.power_control	= usb_simtec_powercontrol,
	.enable_oc	= usb_simtec_enableoc,
};


static struct platform_device *bit2440_devices[] __initdata = {
	&s3c_device_usb,
	&s3c_device_lcd,
	&s3c_device_wdt,
	&s3c_device_i2c,
	&s3c_device_iis,
	&s3c_device_sdi,
	&s3c_device_usbgadget,
	&s3c_device_ts,
	&s3c_device_nand,
	&s3c_device_sound,
	&s3c_device_buttons,
	&s3c_device_rtc,
};

static struct s3c24xx_board bit2440_board __initdata = {
	.devices       = bit2440_devices,
	.devices_count = ARRAY_SIZE(bit2440_devices)
};

void __init bit2440_map_io(void)
{
	u32 val;//yu
	
	s3c24xx_init_io(bit2440_iodesc, ARRAY_SIZE(bit2440_iodesc));
	
#ifdef CONFIG_S3C2440_INCLK12
	s3c24xx_init_clocks(12000000);
#else
	s3c24xx_init_clocks(16934400);
#endif

//Add by yu
	val = readl(S3C2410_GPHCON);
	//printk("old gphcon = %x\n\n\n", val);
	val &= ~(0xfff << 4);
	val |= (0xaaa << 4);
	writel(val, S3C2410_GPHCON);//inialize RxD2 and TxD2
	
	val = readl(S3C2410_GPHUP);
	val |= 0x3f << 2;
	writel(val, S3C2410_GPHUP);
	val = readl(S3C2410_GPHUP);
	//printk("new gphup = %x\n", val);
	///////////////////////////
	
	s3c24xx_init_uarts(bit2440_uartcfgs, ARRAY_SIZE(bit2440_uartcfgs));
	s3c24xx_set_board(&bit2440_board);
	
	s3c_device_nand.dev.platform_data = &bit_nand_info;	
}

void __init bit2440_init_irq(void)
{
	s3c24xx_init_irq();

}

void __init bit2440_init(void)
{
	set_s3c2410ts_info(&bit2440_ts_cfg);
	set_s3c2410udc_info(&bit2440_udc_cfg);
	set_s3c2410fb_info(&bit2440_lcdcfg); 
	
	s3c2410_gpio_cfgpin(S3C2410_GPH0, S3C2410_GPH0_OUTP);
	s3c2410_gpio_pullup(S3C2410_GPH0, 0); 
	s3c2410_gpio_setpin(S3C2410_GPH0, 0); 
	/*	set LED start status by lili */
	s3c2410_gpio_cfgpin(S3C2410_GPF4, S3C2410_GPF4_OUTP);
	s3c2410_gpio_cfgpin(S3C2410_GPF5, S3C2410_GPF5_OUTP);
	s3c2410_gpio_cfgpin(S3C2410_GPF6, S3C2410_GPF6_OUTP);
	s3c2410_gpio_cfgpin(S3C2410_GPF7, S3C2410_GPF7_OUTP);

	s3c2410_gpio_setpin(S3C2410_GPF4, 0);
	s3c2410_gpio_setpin(S3C2410_GPF5, 0);
	s3c2410_gpio_setpin(S3C2410_GPF6, 0);
	s3c2410_gpio_setpin(S3C2410_GPF7, 0);

	/* Add by lili for usb setup*/
	printk("USB Power Control, (c) 2004 Simtec Electronics\n");
	s3c_device_usb.dev.platform_data = &usb_simtec_info;

	s3c2410_gpio_cfgpin(S3C2410_GPB4, S3C2410_GPB4_OUTP);
	s3c2410_gpio_setpin(S3C2410_GPB4, 1);

}
		
MACHINE_START(BIT2440, "QT-2440")
	.phys_ram	= S3C2410_SDRAM_PA,
	.phys_io	= S3C2410_PA_UART,
	.io_pg_offst	= (((u32)S3C24XX_VA_UART) >> 18) & 0xfffc,
	.boot_params	= S3C2410_SDRAM_PA + 0x100,
	
	.init_irq	= bit2440_init_irq,
	.map_io		= bit2440_map_io,
	.init_machine	= bit2440_init,
	.timer		= &s3c24xx_timer,	
MACHINE_END
