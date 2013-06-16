/***********************************************************************
 *
 * linux/arch/arm/mach-s3c2410/mach-smdk2410.c
 *
 * Copyright (C) 2004 by FS Forth-Systeme GmbH
 * All rights reserved.
 *
 * $Id: mach-smdk2410.c,v 1.1 2004/05/11 14:15:38 mpietrek Exp $
 * @Author: Jonas Dietsche
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * @History:
 * derived from linux/arch/arm/mach-s3c2410/mach-bast.c, written by
 * Ben Dooks <ben@simtec.co.uk>
 ***********************************************************************/

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
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mach-types.h>

#include <asm/arch/regs-serial.h>
#include <asm/arch/smdk2410.h>
#include <asm/arch/regs-mem.h>
#include <asm/arch/nand.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>


#include "devs.h"
#include "cpu.h"

static struct map_desc smdk2410_iodesc[] __initdata = {
  	/* nothing here yet */
	/* Map the ethernet controller CS8900A */
	{vSMDK2410_ETH_IO, pSMDK2410_ETH_IO, SZ_1M, MT_DEVICE},
//	{S3C2410_VA_NAND,S3C2410_PA_NAND,S3C2410_SZ_NAND,MT_DEVICE},

	//{SMDK2410_ETH_IO, 0x19000000, SZ_1M, MT_DEVICE}

};

#define UCON S3C2410_UCON_DEFAULT
#define ULCON S3C2410_LCON_CS8 | S3C2410_LCON_PNONE | S3C2410_LCON_STOPB
#define UFCON S3C2410_UFCON_RXTRIG8 | S3C2410_UFCON_FIFOMODE

static struct s3c2410_uartcfg smdk2410_uartcfgs[] = {
	[0] = {
		.hwport	     = 0,
		.flags	     = 0,
		.ucon	     = UCON,
		.ulcon	     = ULCON,
		.ufcon	     = UFCON,
	},
	[1] = {
		.hwport	     = 1,
		.flags	     = 0,
		.ucon	     = UCON,
		.ulcon	     = ULCON,
		.ufcon	     = UFCON,
	},
	[2] = {
		.hwport	     = 2,
		.flags	     = 0,
		.ucon	     = UCON,
		.ulcon	     = ULCON,
		.ufcon	     = UFCON,
	}
};
/* NAND Flash on SMDK  board */


//static int smartmedia_map[] = { 0 };
static int chip0_map[] = { 0 };
//static int chip1_map[] = { 2 };
//static int chip2_map[] = { 3 };

struct mtd_partition smdk_default_nand_part[] = {
	[0] = {
		.name	= "flb0",
		.size	= SZ_8M,
		.offset	= 0
	},
	[1] = {
		.name	= "flb1",
		.size	= SZ_16M,
		.offset	= MTDPART_OFS_APPEND,
	},
	[2] = {
		.name	= "flb2",
		.size	= MTDPART_SIZ_FULL,
		.offset	= MTDPART_OFS_APPEND,
	}
};
/* the bast has 4 selectable slots for nand-flash, the three
 * on-board chip areas, as well as the external SmartMedia
 * slot.
 *
 * Note, there is no current hot-plug support for the SmartMedia
 * socket.
*/

static struct s3c2410_nand_set smdk_nand_sets[] = {
/*	[0] = {
		.name		= "SmartMedia",
		.nr_chips	= 1,
		.nr_map		= smartmedia_map,
		.nr_partitions	= ARRAY_SIZE(bast_default_nand_part),
		.partitions	= bast_default_nand_part
	}*/
	[0] = {
		.name		= "chip0",
		.nr_chips	= 1,
		.nr_map		= chip0_map,
		.nr_partitions	= ARRAY_SIZE(smdk_default_nand_part),
		.partitions	= smdk_default_nand_part
	},
/*	[2] = {
		.name		= "chip1",
		.nr_chips	= 1,
		.nr_map		= chip1_map,
		.nr_partitions	= ARRAY_SIZE(bast_default_nand_part),
		.partitions	= bast_default_nand_part
	},
	[3] = {
		.name		= "chip2",
		.nr_chips	= 1,
		.nr_map		= chip2_map,
		.nr_partitions	= ARRAY_SIZE(bast_default_nand_part),
		.partitions	= bast_default_nand_part
	}*/
};

static void smdk_nand_select(struct s3c2410_nand_set *set, int slot)
{
	//unsigned int tmp;

	slot = set->nr_map[slot] & 3;

/*	pr_debug("bast_nand: selecting slot %d (set %p,%p)\n",
		 slot, set, set->nr_map);

	tmp = __raw_readb(BAST_VA_CTRL2);
	tmp &= BAST_CPLD_CTLR2_IDERST;
	tmp |= slot;
	tmp |= BAST_CPLD_CTRL2_WNAND;

	pr_debug("bast_nand: ctrl2 now %02x\n", tmp);

	__raw_writeb(tmp, BAST_VA_CTRL2);*/
}

static struct s3c2410_platform_nand smdk_nand_info = {
	.tacls		= 80,
	.twrph0		= 80,
	.twrph1		= 80,
	.nr_sets	= ARRAY_SIZE(smdk_nand_sets),
	.sets		= smdk_nand_sets,
	.select_chip	= smdk_nand_select,
};

static struct platform_device *smdk2410_devices[] __initdata = {
	&s3c_device_usb,
	&s3c_device_lcd,
	&s3c_device_wdt,
	&s3c_device_i2c,
	&s3c_device_iis,
	&s3c_device_nand,
};

static struct s3c24xx_board smdk2410_board __initdata = {
	.devices       = smdk2410_devices,
	.devices_count = ARRAY_SIZE(smdk2410_devices)
};

void __init smdk2410_map_io(void)
{
	s3c_device_nand.dev.platform_data = &smdk_nand_info;
	s3c24xx_init_io(smdk2410_iodesc, ARRAY_SIZE(smdk2410_iodesc));
	s3c24xx_init_clocks(0);
	s3c24xx_init_uarts(smdk2410_uartcfgs, ARRAY_SIZE(smdk2410_uartcfgs));
	s3c24xx_set_board(&smdk2410_board);
}

void __init smdk2410_init_irq(void)
{
	s3c24xx_init_irq();
}

MACHINE_START(SMDK2410, "SMDK2410") /* @TODO: request a new identifier and switch
				    * to SMDK2410 */
     MAINTAINER("Jonas Dietsche")
     BOOT_MEM(S3C2410_SDRAM_PA, S3C2410_PA_UART, S3C2410_VA_UART)
     BOOT_PARAMS(S3C2410_SDRAM_PA + 0x100)
     MAPIO(smdk2410_map_io)
     INITIRQ(smdk2410_init_irq)
	.timer		= &s3c24xx_timer,
MACHINE_END


