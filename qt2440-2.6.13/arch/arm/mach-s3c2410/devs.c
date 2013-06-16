/* linux/arch/arm/mach-s3c2410/devs.c
 *
 * Copyright (c) 2004 Simtec Electronics
 * Ben Dooks <ben@simtec.co.uk>
 *
 * Base S3C2410 platform device definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *     10-Mar-2005 LCVR Changed S3C2410_{VA,SZ} to S3C24XX_{VA,SZ}
 *     10-Feb-2005 BJD  Added camera from guillaume.gourat@nexvision.tv
 *     29-Aug-2004 BJD  Added timers 0 through 3
 *     29-Aug-2004 BJD  Changed index of devices we only have one of to -1
 *     21-Aug-2004 BJD  Added IRQ_TICK to RTC resources
 *     18-Aug-2004 BJD  Created initial version
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
#include <asm/arch/fb.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>

#include <asm/arch/regs-serial.h>
#include <asm/arch/udc.h>
#include <asm/arch/ts.h>
#include <asm/arch/lcd.h>

#include "devs.h"
/*Head use for NAND_FALSH*/
#include <linux/mtd/mtd.h> 
#include <linux/mtd/nand.h> 
#include <linux/mtd/nand_ecc.h> 
#include <linux/mtd/partitions.h> 
#include <asm/arch/nand.h>
/* Serial port registrations */

struct platform_device *s3c24xx_uart_devs[3];

/* USB Host Controller */

static struct resource s3c_usb_resource[] = {
	[0] = {
		.start = S3C2410_PA_USBHOST,
		.end   = S3C2410_PA_USBHOST + S3C24XX_SZ_USBHOST,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_USBH,
		.end   = IRQ_USBH,
		.flags = IORESOURCE_IRQ,
	}
};

static u64 s3c_device_usb_dmamask = 0xffffffffUL;

struct platform_device s3c_device_usb = {
	.name		  = "s3c2410-ohci",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_usb_resource),
	.resource	  = s3c_usb_resource,
	.dev              = {
		.dma_mask = &s3c_device_usb_dmamask,
		.coherent_dma_mask = 0xffffffffUL
	}
};

EXPORT_SYMBOL(s3c_device_usb);

/* LCD Controller */

static struct resource s3c_lcd_resource[] = {
	[0] = {
		.start = S3C2410_PA_LCD,
		.end   = S3C2410_PA_LCD + S3C24XX_SZ_LCD,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_LCD,
		.end   = IRQ_LCD,
		.flags = IORESOURCE_IRQ,
	}

};

static u64 s3c_device_lcd_dmamask = 0xffffffffUL;

struct platform_device s3c_device_lcd = {
	.name		  = "s3c2410-lcd",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_lcd_resource),
	.resource	  = s3c_lcd_resource,
	.dev              = {
		.dma_mask		= &s3c_device_lcd_dmamask,
		.coherent_dma_mask	= 0xffffffffUL
	}
};

EXPORT_SYMBOL(s3c_device_lcd);


static struct s3c2410fb_mach_info s3c2410fb_info;

void __init set_s3c2410fb_info(struct s3c2410fb_mach_info *hard_s3c2410fb_info)
{
	memcpy(&s3c2410fb_info, hard_s3c2410fb_info, sizeof(struct s3c2410fb_mach_info));
	s3c_device_lcd.dev.platform_data = &s3c2410fb_info;
}
EXPORT_SYMBOL(set_s3c2410fb_info);

struct platform_device s3c_device_bl = {
	.name		  = "s3c2410-bl",
	.id		  = -1,
};

EXPORT_SYMBOL(s3c_device_bl);

static struct s3c2410_bl_mach_info s3c2410bl_info;

void __init set_s3c2410bl_info(struct s3c2410_bl_mach_info *hard_s3c2410bl_info)
{
	memcpy(&s3c2410bl_info,hard_s3c2410bl_info,sizeof(struct s3c2410_bl_mach_info));
	s3c_device_bl.dev.platform_data = &s3c2410bl_info;
}
EXPORT_SYMBOL(set_s3c2410bl_info);

/* NAND Controller */

static struct resource s3c_nand_resource[] = {
	[0] = {
		.start = S3C2410_PA_NAND,
		.end   = S3C2410_PA_NAND + S3C24XX_SZ_NAND,
		.flags = IORESOURCE_MEM,
	}
};

struct platform_device s3c_device_nand = {
	.name		  = "s3c2410-nand",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_nand_resource),
	.resource	  = s3c_nand_resource,
};

EXPORT_SYMBOL(s3c_device_nand);

/* Touchscreen */
struct platform_device s3c_device_ts = {
	.name		  = "s3c2410-ts",
	.id		  = -1,
};

EXPORT_SYMBOL(s3c_device_ts);

static struct s3c2410_ts_mach_info s3c2410ts_info;

void __init set_s3c2410ts_info(struct s3c2410_ts_mach_info *hard_s3c2410ts_info)
{
	memcpy(&s3c2410ts_info,hard_s3c2410ts_info,sizeof(struct s3c2410_ts_mach_info));
	s3c_device_ts.dev.platform_data = &s3c2410ts_info;
}
EXPORT_SYMBOL(set_s3c2410ts_info);

/* 'Keyboard' */
struct platform_device s3c_device_buttons = {
	.name             = "s3c2410-buttons",
	.id               = -1,
};

EXPORT_SYMBOL(s3c_device_buttons);

/* USB Device (Gadget)*/

static struct resource s3c_usbgadget_resource[] = {
	[0] = {
		.start = S3C2410_PA_USBDEV,
		.end   = S3C2410_PA_USBDEV + S3C24XX_SZ_USBDEV,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_USBD,
		.end   = IRQ_USBD,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device s3c_device_usbgadget = {
	.name		  = "s3c2410-usbgadget",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_usbgadget_resource),
	.resource	  = s3c_usbgadget_resource,
};

EXPORT_SYMBOL(s3c_device_usbgadget);

static struct s3c2410_udc_mach_info s3c2410udc_info;

void __init set_s3c2410udc_info(struct s3c2410_udc_mach_info *hard_s3c2410udc_info)
{
	memcpy(&s3c2410udc_info,hard_s3c2410udc_info,sizeof(struct s3c2410_udc_mach_info));
	s3c_device_usbgadget.dev.platform_data = &s3c2410udc_info;
}
EXPORT_SYMBOL(set_s3c2410udc_info);


/* Watchdog */

static struct resource s3c_wdt_resource[] = {
	[0] = {
		.start = S3C2410_PA_WATCHDOG,
		.end   = S3C2410_PA_WATCHDOG + S3C24XX_SZ_WATCHDOG,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_WDT,
		.end   = IRQ_WDT,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device s3c_device_wdt = {
	.name		  = "s3c2410-wdt",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_wdt_resource),
	.resource	  = s3c_wdt_resource,
};

EXPORT_SYMBOL(s3c_device_wdt);

/* I2C */

static struct resource s3c_i2c_resource[] = {
	[0] = {
		.start = S3C2410_PA_IIC,
		.end   = S3C2410_PA_IIC + S3C24XX_SZ_IIC,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_IIC,
		.end   = IRQ_IIC,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device s3c_device_i2c = {
	.name		  = "s3c2410-i2c",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_i2c_resource),
	.resource	  = s3c_i2c_resource,
};

EXPORT_SYMBOL(s3c_device_i2c);

/* IIS */

static struct resource s3c_iis_resource[] = {
	[0] = {
		.start = S3C2410_PA_IIS,
		.end   = S3C2410_PA_IIS + S3C24XX_SZ_IIS,
		.flags = IORESOURCE_MEM,
	}
};

static u64 s3c_device_iis_dmamask = 0xffffffffUL;

struct platform_device s3c_device_iis = {
	.name		  = "s3c2410-iis",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_iis_resource),
	.resource	  = s3c_iis_resource,
	.dev              = {
		.dma_mask = &s3c_device_iis_dmamask,
		.coherent_dma_mask = 0xffffffffUL
	}
};

EXPORT_SYMBOL(s3c_device_iis);

/* RTC */

static struct resource s3c_rtc_resource[] = {
	[0] = {
		.start = S3C2410_PA_RTC,
		.end   = S3C2410_PA_RTC + 0xff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_RTC,
		.end   = IRQ_RTC,
		.flags = IORESOURCE_IRQ,
	},
	[2] = {
		.start = IRQ_TICK,
		.end   = IRQ_TICK,
		.flags = IORESOURCE_IRQ
	}
};

struct platform_device s3c_device_rtc = {
	.name		  = "s3c2410-rtc",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_rtc_resource),
	.resource	  = s3c_rtc_resource,
};

EXPORT_SYMBOL(s3c_device_rtc);

/* ADC */

static struct resource s3c_adc_resource[] = {
	[0] = {
		.start = S3C2410_PA_ADC,
		.end   = S3C2410_PA_ADC + S3C24XX_SZ_ADC,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_TC,
		.end   = IRQ_ADC,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device s3c_device_adc = {
	.name		  = "s3c2410-adc",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_adc_resource),
	.resource	  = s3c_adc_resource,
};

/* SDI */

static struct resource s3c_sdi_resource[] = {
	[0] = {
		.start = S3C2410_PA_SDI,
		.end   = S3C2410_PA_SDI + S3C24XX_SZ_SDI,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_SDI,
		.end   = IRQ_SDI,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device s3c_device_sdi = {
	.name		  = "s3c2410-sdi",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_sdi_resource),
	.resource	  = s3c_sdi_resource,
};

EXPORT_SYMBOL(s3c_device_sdi);

/* SPI (0) */

static struct resource s3c_spi0_resource[] = {
	[0] = {
		.start = S3C2410_PA_SPI,
		.end   = S3C2410_PA_SPI + 0x1f,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_SPI0,
		.end   = IRQ_SPI0,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device s3c_device_spi0 = {
	.name		  = "s3c2410-spi",
	.id		  = 0,
	.num_resources	  = ARRAY_SIZE(s3c_spi0_resource),
	.resource	  = s3c_spi0_resource,
};

EXPORT_SYMBOL(s3c_device_spi0);

/* SPI (1) */

static struct resource s3c_spi1_resource[] = {
	[0] = {
		.start = S3C2410_PA_SPI + 0x20,
		.end   = S3C2410_PA_SPI + 0x20 + 0x1f,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_SPI1,
		.end   = IRQ_SPI1,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device s3c_device_spi1 = {
	.name		  = "s3c2410-spi",
	.id		  = 1,
	.num_resources	  = ARRAY_SIZE(s3c_spi1_resource),
	.resource	  = s3c_spi1_resource,
};

EXPORT_SYMBOL(s3c_device_spi1);

/* pwm timer blocks */

static struct resource s3c_timer0_resource[] = {
	[0] = {
		.start = S3C2410_PA_TIMER + 0x0C,
		.end   = S3C2410_PA_TIMER + 0x0C + 0xB,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_TIMER0,
		.end   = IRQ_TIMER0,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device s3c_device_timer0 = {
	.name		  = "s3c2410-timer",
	.id		  = 0,
	.num_resources	  = ARRAY_SIZE(s3c_timer0_resource),
	.resource	  = s3c_timer0_resource,
};

EXPORT_SYMBOL(s3c_device_timer0);

/* timer 1 */

static struct resource s3c_timer1_resource[] = {
	[0] = {
		.start = S3C2410_PA_TIMER + 0x18,
		.end   = S3C2410_PA_TIMER + 0x23,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_TIMER1,
		.end   = IRQ_TIMER1,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device s3c_device_timer1 = {
	.name		  = "s3c2410-timer",
	.id		  = 1,
	.num_resources	  = ARRAY_SIZE(s3c_timer1_resource),
	.resource	  = s3c_timer1_resource,
};

EXPORT_SYMBOL(s3c_device_timer1);

/* timer 2 */

static struct resource s3c_timer2_resource[] = {
	[0] = {
		.start = S3C2410_PA_TIMER + 0x24,
		.end   = S3C2410_PA_TIMER + 0x2F,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_TIMER2,
		.end   = IRQ_TIMER2,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device s3c_device_timer2 = {
	.name		  = "s3c2410-timer",
	.id		  = 2,
	.num_resources	  = ARRAY_SIZE(s3c_timer2_resource),
	.resource	  = s3c_timer2_resource,
};

EXPORT_SYMBOL(s3c_device_timer2);

/* timer 3 */

static struct resource s3c_timer3_resource[] = {
	[0] = {
		.start = S3C2410_PA_TIMER + 0x30,
		.end   = S3C2410_PA_TIMER + 0x3B,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_TIMER3,
		.end   = IRQ_TIMER3,
		.flags = IORESOURCE_IRQ,
	}

};

struct platform_device s3c_device_timer3 = {
	.name		  = "s3c2410-timer",
	.id		  = 3,
	.num_resources	  = ARRAY_SIZE(s3c_timer3_resource),
	.resource	  = s3c_timer3_resource,
};

EXPORT_SYMBOL(s3c_device_timer3);
#if 0
/* LED devices */

static struct s3c24xx_led_platdata smdk_pdata_led4 = {
	.gpio		= S3C2410_GPF4,
	.flags		= S3C24XX_LEDF_ACTLOW | S3C24XX_LEDF_TRISTATE,
	.name		= "led4",
	.def_trigger	= "timer",
};

static struct s3c24xx_led_platdata smdk_pdata_led5 = {
	.gpio		= S3C2410_GPF5,
	.flags		= S3C24XX_LEDF_ACTLOW | S3C24XX_LEDF_TRISTATE,
	.name		= "led5",
	.def_trigger	= "nand-disk",
};

static struct s3c24xx_led_platdata smdk_pdata_led6 = {
	.gpio		= S3C2410_GPF6,
	.flags		= S3C24XX_LEDF_ACTLOW | S3C24XX_LEDF_TRISTATE,
	.name		= "led6",
};

static struct s3c24xx_led_platdata smdk_pdata_led7 = {
	.gpio		= S3C2410_GPF7,
	.flags		= S3C24XX_LEDF_ACTLOW | S3C24XX_LEDF_TRISTATE,
	.name		= "led7",
};

static struct platform_device smdk_led4 = {
	.name		= "s3c24xx_led",
	.id		= 0,
	.source		= smdk_pdata_led4,
};

static struct platform_device smdk_led5 = {
	.name		= "s3c24xx_led",
	.id		= 1,
	.source		= smdk_pdata_led5,
};

static struct platform_device smdk_led6 = {
	.name		= "s3c24xx_led",
	.id		= 2,
	.source		= &smdk_pdata_led6,
};

static struct platform_device smdk_led7 = {
	.name		= "s3c24xx_led",
	.id		= 3,
	.source		= &smdk_pdata_led7,
};
EXPORT_SYMBOL(smdk_led4);
EXPORT_SYMBOL(smdk_led5);
EXPORT_SYMBOL(smdk_led6);
EXPORT_SYMBOL(smdk_led7);
#endif
#ifdef CONFIG_CPU_S3C2440

/* Camif Controller */

static struct resource s3c_camif_resource[] = {
	[0] = {
		.start = S3C2440_PA_CAMIF,
		.end   = S3C2440_PA_CAMIF + S3C2440_SZ_CAMIF,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_CAM,
		.end   = IRQ_CAM,
		.flags = IORESOURCE_IRQ,
	}

};

static u64 s3c_device_camif_dmamask = 0xffffffffUL;

struct platform_device s3c_device_camif = {
	.name		  = "s3c2440-camif",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s3c_camif_resource),
	.resource	  = s3c_camif_resource,
	.dev              = {
		.dma_mask = &s3c_device_camif_dmamask,
		.coherent_dma_mask = 0xffffffffUL
	}
};

EXPORT_SYMBOL(s3c_device_camif);

struct platform_device s3c_device_sound = {
	.name		= "s3c2440-sound",
	.id		= -1,
};
EXPORT_SYMBOL(s3c_device_sound);



#endif // CONFIG_CPU_S32440
