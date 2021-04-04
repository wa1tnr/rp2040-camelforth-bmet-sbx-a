#include "rp2040.h"

#define SYST_CSR    (PPB_BASE + 0xE010)
#define SYST_RVR    (PPB_BASE + 0xE014)
#define SYST_CVR    (PPB_BASE + 0xE018)
#define SYST_CALIB  (PPB_BASE + 0xE01C)

#define NVIC_ISER   (PPB_BASE + 0xE100)
#define NVIC_ICER   (PPB_BASE + 0xE180)
#define NVIC_ISPR   (PPB_BASE + 0xE200)
#define NVIC_ICPR   (PPB_BASE + 0xE280)
#define NVIC_IPR0   (PPB_BASE + 0xE400)
#define NVIC_IPR1   (PPB_BASE + 0xE404)
#define NVIC_IPR2   (PPB_BASE + 0xE408)
#define NVIC_IPR3   (PPB_BASE + 0xE40C)
#define NVIC_IPR4   (PPB_BASE + 0xE410)
#define NVIC_IPR5   (PPB_BASE + 0xE414)
#define NVIC_IPR6   (PPB_BASE + 0xE418)
#define NVIC_IPR7   (PPB_BASE + 0xE41C)

#define CPUID       (PPB_BASE + 0xED00)
#define ICSR        (PPB_BASE + 0xED04)
#define VTOR        (PPB_BASE + 0xED08)
#define AIRCR       (PPB_BASE + 0xED0C)
#define SCR         (PPB_BASE + 0xED10)
#define CCR         (PPB_BASE + 0xED14)
#define SHPR2       (PPB_BASE + 0xED1C)
#define SHPR3       (PPB_BASE + 0xED20)
#define SHCSR       (PPB_BASE + 0xED24)

#define MPU_TYPE    (PPB_BASE + 0xED90)
#define MPU_CTRL    (PPB_BASE + 0xED94)
#define MPU_RNR     (PPB_BASE + 0xED98)
#define MPU_RBAR    (PPB_BASE + 0xED9C)
#define MPU_RASR    (PPB_BASE + 0xEDA0)

// Flags
#define SYS_CSR_COUNTFLAG   (1 << 16)
#define SYS_CSR_CLKSOURCE   (1 << 2)
#define SYS_CSR_TICKINT     (1 << 1)
#define SYS_CSR_ENABLE      (1 << 0)
#define SYS_RVR_RELOAD      (0x00ffffff)
#define SYS_CVR_CURRENT     (0x00ffffff)
#define SYS_CALIB_NOREF     (1 << 31)
#define SYS_CALIB_SKEW      (1 << 30)
#define SYS_CALIB_TENMS     (0x00ffffff)

#define AIRCR_SYSRESETREQ   (1 << 2)

#define SCR_SEVONPEND       (1 << 4)
#define SCR_SLEEPDEEP       (1 << 2)
#define SCR_SLEEPONEXIT     (1 << 1)

#define SHCSR_SVCALLPENDED  (1 << 15)