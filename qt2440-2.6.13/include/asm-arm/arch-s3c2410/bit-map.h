/* linux/include/asm-arm/arch-s3c2410/bit-map.h */

/* Date: 2005.05.03
 * This source file is written by DaeHo Jung<aplus43@hanmail.net>
 * -jdh-
 */  

#ifndef __ASM_ARCH_BITMAP_H
#define __ASM_ARCH_BITMAP_H

#define BIT_VA_IOBASE	0xF8000000

#define BIT_VA_CS8900A    BIT_VA_IOBASE	 	/* 0xF8000000 */
#define BIT_PA_CS8900A    (S3C2410_CS3 + 0x1000000)	/* 0x19000000 */
#define BIT_SZ_CS8900A    SZ_1M

#define pSMDK2410_ETH_IO        0x19000000
#define vSMDK2410_ETH_IO        0xE0000000
#define SMDK2410_ETH_IRQ        IRQ_EINT9


#endif /* __ASM_ARCH_BITMAP_H */
