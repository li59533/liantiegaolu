/**
 **************************************************************************************************
 * @file        bsp_clock.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */

#include "bsp_clock.h"
#include "clog.h"
/**
 * @addtogroup    XXX 
 * @{  
 */
#include "bsp_led.h"
/**
 * @addtogroup    bsp_clock_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_clock_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_clock_Macros_Defines 
 * @brief         
 * @{  
 */
#define MCG_PLL_DISABLE                                   0U  /*!< MCGPLLCLK disabled */
#define OSC_CAP0P                                         0U  /*!< Oscillator 0pF capacitor load */
#define OSC_ER_CLK_DISABLE                                0U  /*!< Disable external reference clock */
#define SIM_OSC32KSEL_LPO_CLK                             3U  /*!< OSC32KSEL select: LPO clock */
#define SIM_PLLFLLSEL_MCGFLLCLK_CLK                       0U  /*!< PLLFLL select: MCGFLLCLK clock */
#define SIM_PLLFLLSEL_MCGPLLCLK_CLK                       1U  /*!< PLLFLL select: MCGPLLCLK clock */

extern uint32_t SystemCoreClock;

/**
 * @}
 */

/**
 * @defgroup      bsp_clock_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_clock_Private_Types
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_clock_Private_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_clock_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_clock_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */
static void CLOCK_CONFIG_SetFllExtRefDiv(uint8_t frdiv)
{
    MCG->C1 = ((MCG->C1 & ~MCG_C1_FRDIV_MASK) | MCG_C1_FRDIV(frdiv));
}
/**
 * @}
 */

/**
 * @defgroup      bsp_clock_Functions 
 * @brief         
 * @{  
 */
/*******************************************************************************
 ********************** Configuration BOARD_BootClockRUN ***********************
 ******************************************************************************/
/* TEXT BELOW IS USED AS SETTING FOR THE CLOCKS TOOL *****************************
!!Configuration
name: BOARD_BootClockRUN
outputs:
- {id: Bus_clock.outFreq, value: 24 MHz}
- {id: Core_clock.outFreq, value: 48 MHz, locked: true, accuracy: '0.001'}
- {id: ERCLK32K.outFreq, value: 1 kHz}
- {id: Flash_clock.outFreq, value: 24 MHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: MCGIRCLK.outFreq, value: 32.768 kHz}
- {id: OSCERCLK.outFreq, value: 8 MHz}
- {id: PLLFLLCLK.outFreq, value: 48 MHz}
- {id: System_clock.outFreq, value: 48 MHz}
settings:
- {id: MCGMode, value: PEE}
- {id: MCG.FCRDIV.scale, value: '1', locked: true}
- {id: MCG.FRDIV.scale, value: '32'}
- {id: MCG.IREFS.sel, value: MCG.FRDIV}
- {id: MCG.PLLS.sel, value: MCG.PLL}
- {id: MCG.PRDIV.scale, value: '2'}
- {id: MCG_C1_IRCLKEN_CFG, value: Enabled}
- {id: MCG_C2_OSC_MODE_CFG, value: ModeOscLowPower}
- {id: MCG_C2_RANGE0_CFG, value: High}
- {id: MCG_C2_RANGE0_FRDIV_CFG, value: High}
- {id: OSC0_CR_ERCLKEN_CFG, value: Enabled}
- {id: OSC_CR_ERCLKEN_CFG, value: Enabled}
- {id: SIM.CLKOUTSEL.sel, value: SIM.OUTDIV4}
- {id: SIM.OSC32KSEL.sel, value: PMC.LPOCLK}
- {id: SIM.OUTDIV1.scale, value: '2'}
- {id: SIM.PLLFLLSEL.sel, value: SIM.MCGPLLCLK_DIV2}
- {id: SIM.TPMSRCSEL.sel, value: OSC.OSCERCLK}
- {id: SIM.UART0SRCSEL.sel, value: OSC.OSCERCLK}
- {id: SIM.USBSRCSEL.sel, value: SIM.PLLFLLSEL}
sources:
- {id: OSC.OSC.outFreq, value: 8 MHz, enabled: true}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE CLOCKS TOOL **/

/*******************************************************************************
 * Variables for BOARD_BootClockRUN configuration
 ******************************************************************************/
const mcg_config_t mcgConfig_BOARD_BootClockRUN =
    {
        .mcgMode = kMCG_ModePEE,                  /* PEE - PLL Engaged External */
        .irclkEnableMode = kMCG_IrclkEnable,      /* MCGIRCLK enabled, MCGIRCLK disabled in STOP mode */
        .ircs = kMCG_IrcSlow,                     /* Slow internal reference clock selected */
        .fcrdiv = 0x0U,                           /* Fast IRC divider: divided by 1 */
        .frdiv = 0x0U,                            /* FLL reference clock divider: divided by 32 */
        .drs = kMCG_DrsLow,                       /* Low frequency range */
        .dmx32 = kMCG_Dmx32Default,               /* DCO has a default range of 25% */
        .pll0Config =
            {
                .enableMode = MCG_PLL_DISABLE,    /* MCGPLLCLK disabled */
                .prdiv = 0x1U,                    /* PLL Reference divider: divided by 2 */
                .vdiv = 0x0U,                     /* VCO divider: multiplied by 24 */
            },
    };
const sim_clock_config_t simConfig_BOARD_BootClockRUN =
    {
        .pllFllSel = SIM_PLLFLLSEL_MCGPLLCLK_CLK, /* PLLFLL select: MCGPLLCLK clock */
        .er32kSrc = SIM_OSC32KSEL_LPO_CLK,        /* OSC32KSEL select: LPO clock */
        .clkdiv1 = 0x10010000U,                   /* SIM_CLKDIV1 - OUTDIV1: /2, OUTDIV4: /2 */
    };
const osc_config_t oscConfig_BOARD_BootClockRUN =
    {
        .freq = 8000000U,                         /* Oscillator frequency: 8000000Hz */
        .capLoad = (OSC_CAP0P),                   /* Oscillator capacity load: 0pF */
        .workMode = kOSC_ModeOscLowPower,         /* Oscillator low power */
        .oscerConfig =
            {
                .enableMode = kOSC_ErClkEnable,   /* Enable external reference clock, disable external reference clock in STOP mode */
            }
    };

void BSP_SystemClockInit(void)
{
	/* Set the system clock dividers in SIM to safe value. */
	CLOCK_SetSimSafeDivs();
	/* Initializes OSC0 according to board configuration. */
	CLOCK_InitOsc0(&oscConfig_BOARD_BootClockRUN);
	CLOCK_SetXtal0Freq(oscConfig_BOARD_BootClockRUN.freq);
	/* Configure FLL external reference divider (FRDIV). */
	CLOCK_CONFIG_SetFllExtRefDiv(mcgConfig_BOARD_BootClockRUN.frdiv);
	/* Set MCG to PEE mode. */
	CLOCK_BootToPeeMode(kMCG_OscselOsc,
						kMCG_PllClkSelPll0,
						&mcgConfig_BOARD_BootClockRUN.pll0Config);
	/* Configure the Internal Reference clock (MCGIRCLK). */
	CLOCK_SetInternalRefClkConfig(mcgConfig_BOARD_BootClockRUN.irclkEnableMode,
								  mcgConfig_BOARD_BootClockRUN.ircs, 
								  mcgConfig_BOARD_BootClockRUN.fcrdiv);
	/* Set the clock configuration in SIM module. */
	CLOCK_SetSimConfig(&simConfig_BOARD_BootClockRUN);
	/* Set SystemCoreClock variable. */
	SystemCoreClock = BOARD_BOOTCLOCKRUN_CORE_CLOCK;
}


/*******************************************************************************
 ********************* Configuration BOARD_BootClockVLPR ***********************
 ******************************************************************************/
/* TEXT BELOW IS USED AS SETTING FOR THE CLOCKS TOOL *****************************
!!Configuration
name: BOARD_BootClockVLPR
outputs:
- {id: Bus_clock.outFreq, value: 800 kHz}
- {id: Core_clock.outFreq, value: 4 MHz, locked: true, accuracy: '0.001'}
- {id: ERCLK32K.outFreq, value: 1 kHz}
- {id: Flash_clock.outFreq, value: 800 kHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: MCGIRCLK.outFreq, value: 4 MHz}
- {id: System_clock.outFreq, value: 4 MHz}
settings:
- {id: MCGMode, value: BLPI}
- {id: powerMode, value: VLPR}
- {id: MCG.CLKS.sel, value: MCG.IRCS}
- {id: MCG.FCRDIV.scale, value: '1'}
- {id: MCG.FRDIV.scale, value: '32'}
- {id: MCG.IRCS.sel, value: MCG.FCRDIV}
- {id: MCG_C1_IRCLKEN_CFG, value: Enabled}
- {id: MCG_C2_OSC_MODE_CFG, value: ModeOscLowPower}
- {id: MCG_C2_RANGE0_CFG, value: Very_high}
- {id: MCG_C2_RANGE0_FRDIV_CFG, value: Very_high}
- {id: SIM.OSC32KSEL.sel, value: PMC.LPOCLK}
- {id: SIM.OUTDIV4.scale, value: '5'}
sources:
- {id: OSC.OSC.outFreq, value: 8 MHz}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE CLOCKS TOOL **/

/*******************************************************************************
 * Variables for BOARD_BootClockVLPR configuration
 ******************************************************************************/
const mcg_config_t mcgConfig_BOARD_BootClockVLPR =
    {
        .mcgMode = kMCG_ModeBLPI,                 /* BLPI - Bypassed Low Power Internal */
        .irclkEnableMode = kMCG_IrclkEnable,      /* MCGIRCLK enabled, MCGIRCLK disabled in STOP mode */
        .ircs = kMCG_IrcFast,                     /* Fast internal reference clock selected */
        .fcrdiv = 0x0U,                           /* Fast IRC divider: divided by 1 */
        .frdiv = 0x0U,                            /* FLL reference clock divider: divided by 32 */
        .drs = kMCG_DrsLow,                       /* Low frequency range */
        .dmx32 = kMCG_Dmx32Default,               /* DCO has a default range of 25% */
        .pll0Config =
            {
                .enableMode = MCG_PLL_DISABLE,    /* MCGPLLCLK disabled */
                .prdiv = 0x0U,                    /* PLL Reference divider: divided by 1 */
                .vdiv = 0x0U,                     /* VCO divider: multiplied by 24 */
            },
    };
const sim_clock_config_t simConfig_BOARD_BootClockVLPR =
    {
        .pllFllSel = SIM_PLLFLLSEL_MCGFLLCLK_CLK, /* PLLFLL select: MCGFLLCLK clock */
        .er32kSrc = SIM_OSC32KSEL_LPO_CLK,        /* OSC32KSEL select: LPO clock */
        .clkdiv1 = 0x40000U,                      /* SIM_CLKDIV1 - OUTDIV1: /1, OUTDIV4: /5 */
    };
const osc_config_t oscConfig_BOARD_BootClockVLPR =
    {
        .freq = 0U,                               /* Oscillator frequency: 0Hz */
        .capLoad = (OSC_CAP0P),                   /* Oscillator capacity load: 0pF */
        .workMode = kOSC_ModeOscLowPower,         /* Oscillator low power */
        .oscerConfig =
            {
                .enableMode = OSC_ER_CLK_DISABLE, /* Disable external reference clock */
            }
    };


void BOARD_BootClockVLPR(void)
{
    /* Set the system clock dividers in SIM to safe value. */
    CLOCK_SetSimSafeDivs();
    /* Set MCG to BLPI mode. */
    CLOCK_BootToBlpiMode(mcgConfig_BOARD_BootClockVLPR.fcrdiv,
                         mcgConfig_BOARD_BootClockVLPR.ircs,
                         mcgConfig_BOARD_BootClockVLPR.irclkEnableMode);
    /* Set the clock configuration in SIM module. */
    CLOCK_SetSimConfig(&simConfig_BOARD_BootClockVLPR);
    /* Set VLPR power mode. */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
#if (defined(FSL_FEATURE_SMC_HAS_LPWUI) && FSL_FEATURE_SMC_HAS_LPWUI)
    SMC_SetPowerModeVlpr(SMC, false);
#else
    SMC_SetPowerModeVlpr(SMC);
#endif
    while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateVlpr)
    {
    }
    /* Set SystemCoreClock variable. */
    SystemCoreClock = BOARD_BOOTCLOCKVLPR_CORE_CLOCK;
}


void BSP_ShowClock(void)
{
	DEBUG("kCLOCK_CoreSysClk:%d\r\n", CLOCK_GetFreq(kCLOCK_CoreSysClk));
	DEBUG("kCLOCK_PlatClk:%d\r\n", CLOCK_GetFreq(kCLOCK_PlatClk));
	DEBUG("kCLOCK_BusClk:%d\r\n", CLOCK_GetFreq(kCLOCK_BusClk));
	DEBUG("kCLOCK_FlashClk:%d\r\n", CLOCK_GetFreq(kCLOCK_FlashClk));
	DEBUG("kCLOCK_PllFllSelClk:%d\r\n", CLOCK_GetFreq(kCLOCK_PllFllSelClk));
	DEBUG("kCLOCK_Er32kClk:%d\r\n", CLOCK_GetFreq(kCLOCK_Er32kClk));
	DEBUG("kCLOCK_McgFixedFreqClk:%d\r\n", CLOCK_GetFreq(kCLOCK_McgFixedFreqClk));
	DEBUG("kCLOCK_McgInternalRefClk:%d\r\n", CLOCK_GetFreq(kCLOCK_McgInternalRefClk));
	DEBUG("kCLOCK_McgFllClk:%d\r\n", CLOCK_GetFreq(kCLOCK_McgFllClk));
	DEBUG("kCLOCK_McgPll0Clk:%d\r\n", CLOCK_GetFreq(kCLOCK_McgPll0Clk));
	DEBUG("kCLOCK_LpoClk:%d\r\n", CLOCK_GetFreq(kCLOCK_LpoClk));
	DEBUG("kCLOCK_Osc0ErClk:%d\r\n", CLOCK_GetFreq(kCLOCK_Osc0ErClk));	
}






/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

