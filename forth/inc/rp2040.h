#pragma once
// ROM
#define ROM_BASE					0x00000000
// FLASH
#define XIP_BASE					0x10000000
#define XIP_NOALLOC_BASE			0x11000000
#define XIP_NOCACHE_BASE			0x12000000
#define XIP_NOCACHE_NOALLOC_BASE	0x13000000
#define XIP_CTRL_BASE				0x14000000
#define XIP_SRAM_BASE				0x15000000
#define XIP_SRAM_END				0x15004000
#define XIP_SSI_BASE				0x18000000
// SRAM
#define SRAM_BASE					0x20000000
#define SRAM_STRIPED_BASE			0x20000000
#define SRAM_STRIPED_END			0x20040000
#define SRAM4_BASE					0x20040000
#define SRAM5_BASE					0x20041000
#define SRAM_END					0x20042000
#define SRAM0_BASE					0x21000000
#define SRAM1_BASE					0x21010000
#define SRAM2_BASE					0x21020000
#define SRAM3_BASE					0x21030000
// APB
#define SYSINFO_BASE				0x40000000
#define SYSCFG_BASE					0x40004000
#define CLOCKS_BASE					0x40008000
#define RESETS_BASE					0x4000C000
#define PSM_BASE					0x40010000
#define IO_BANK0_BASE				0x40014000
#define IO_QSPI_BASE				0x40018000
#define PADS_BANK0_BASE				0x4001C000
#define PADS_QSPI_BASE				0x40020000
#define XOSC_BASE					0x40024000
#define PLL_SYS_BASE				0x40028000
#define PLL_USB_BASE				0x4002C000
#define BUSCTRL_BASE				0x40030000
#define UART0_BASE					0x40034000
#define UART1_BASE					0x40038000
#define SPI0_BASE					0x4003C000
#define SPI1_BASE					0x40040000
#define I2C0_BASE					0x40044000
#define I2C1_BASE					0x40048000
#define ADC_BASE					0x4004C000
#define PWM_BASE					0x40050000
#define TIMER_BASE					0x40054000
#define WATCHDOG_BASE				0x40058000
#define RTC_BASE					0x4005C000
#define ROSC_BASE					0x40060000
#define VREG_AND_CHIP_RESET_BASE	0x40064000
#define TBMAN_BASE					0x4006C000
// AHB
#define DMA_BASE					0x50000000
#define USBCTRL_BASE				0x50100000
#define USBCTRL_DPRAM_BASE			0x50100000
#define USBCTRL_REGS_BASE			0x50110000
#define PIO0_BASE					0x50200000
#define PIO1_BASE					0x50300000
#define XIP_AUX_BASE				0x50400000
// SIO
#define SIO_BASE					0xD0000000
// CM0+ internal peripherals
#define PPB_BASE					0xE0000000


typedef unsigned int 	uint;
typedef unsigned short	ushort;
typedef unsigned char	byte;

#define IO_RD(addr)            *((volatile uint*)(addr))
#define IO_WR(addr, data)      *((volatile uint*)(addr)) = (data)
