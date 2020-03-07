/**
 **************************************************************************************************
 * @file        bsp_led.c
 * @author
 * @version
 * @date        
 * @brief
 **************************************************************************************************
 * @attention
 *
 **************************************************************************************************
 */

#include "bsp_led.h"
#include "bsp_conf.h"
#include "osal.h"
#include "hal_task.h"
/**
 * @addtogroup    XXX 
 * @{  
 */

/**
 * @addtogroup    bsp_led_Modules 
 * @{  
 */

/**
 * @defgroup      bsp_led_IO_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_led_Macros_Defines 
 * @brief         
 * @{  
 */
/* Modes */
#define BSP_LED_MODE_OFF     0x00
#define BSP_LED_MODE_ON      0x01
#define BSP_LED_MODE_BLINK   0x02
#define BSP_LED_MODE_FLASH   0x04
/**
 * @}
 */

/**
 * @defgroup      bsp_led_Constants_Defines 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup       bsp_led_Private_Types
 * @brief         
 * @{  
 */
typedef struct {
  uint8_t mode;       /* Operation mode */
#if (defined BSP_LED_BLINK_ENABLE) && (BSP_LED_BLINK_ENABLE == TRUE)
  uint8_t todo;       /* Blink cycles left */
  uint8_t onPct;      /* On cycle percentage */
  uint16_t time;      /* On/off cycle time (msec) */
  uint32_t next;      /* Time for next change */
  uint8_t preBlinkState;
#endif
} BSPLedControl_t; 
 
 
typedef struct
{
	clock_ip_name_t clock_ip_name;
	PORT_Type *PORT_Type_base;
	GPIO_Type *GPIO_Type_base;
	uint32_t pin;
	port_mux_t mux;
	gpio_pin_config_t gpio_pin_config;
}GPIO_InstanceTypedef;
 
 
typedef struct
{
    GPIO_InstanceTypedef Pin;
    uint8_t OnLevel;
    uint8_t OffLevel;
}LED_IO_Typedef;

const LED_IO_Typedef LED_IO[BSP_LEDCOUNT] = 
{
	{
		{kCLOCK_PortE , PORTE, GPIOE , 18, kPORT_MuxAsGpio ,kGPIO_DigitalOutput ,0},
		.OnLevel = 0,
        .OffLevel = 1,
	},
};


/**
 * @}
 */

/**
 * @defgroup      bsp_led_Private_Variables 
 * @brief         
 * @{  
 */
static BSPLedControl_t s_BSPLedControlTable[BSP_LEDCOUNT] =
{
    {BSP_LED_MODE_OFF},
};
/**
 * @}
 */

/**
 * @defgroup      bsp_led_Public_Variables 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_led_Private_FunctionPrototypes 
 * @brief         
 * @{  
 */

/**
 * @}
 */

/**
 * @defgroup      bsp_led_Functions 
 * @brief         
 * @{  
 */

void BSP_LED_Init(void)
{
	uint8_t i  = 0;

	for(i = 0; i < BSP_LEDCOUNT ; i++)
	{
		CLOCK_EnableClock(LED_IO[i].Pin.clock_ip_name);

		PORT_SetPinMux( LED_IO[i].Pin.PORT_Type_base, LED_IO[i].Pin.pin, LED_IO[i].Pin.mux  );
		GPIO_PinInit(LED_IO[i].Pin.GPIO_Type_base, LED_IO[i].Pin.pin, &LED_IO[i].Pin.gpio_pin_config);
		
		GPIO_WritePinOutput(LED_IO[i].Pin.GPIO_Type_base, LED_IO[i].Pin.pin, LED_IO[i].OffLevel);		
	}

}


void BSP_LED_Open(uint8_t BSP_LEDx)
{
	
	GPIO_WritePinOutput(LED_IO[BSP_LEDx].Pin.GPIO_Type_base, LED_IO[BSP_LEDx].Pin.pin, LED_IO[BSP_LEDx].OnLevel);
	s_BSPLedControlTable[BSP_LEDx].mode = BSP_LED_MODE_ON;
}

void BSP_LED_Close(uint8_t BSP_LEDx)
{
	GPIO_WritePinOutput(LED_IO[BSP_LEDx].Pin.GPIO_Type_base, LED_IO[BSP_LEDx].Pin.pin, LED_IO[BSP_LEDx].OffLevel);
	s_BSPLedControlTable[BSP_LEDx].mode = BSP_LED_MODE_OFF;
}


void BSP_LED_Toggle(uint8_t BSP_LEDx)
{
	
    if (s_BSPLedControlTable[BSP_LEDx].mode & BSP_LED_MODE_ON)
    {
        BSP_LED_Close(BSP_LEDx);
    }
    else 
    {
        BSP_LED_Open(BSP_LEDx);
    }	
	
}


void BSP_LED_Blink(uint8_t BSP_LEDx, uint8_t numBlinks, uint8_t percent, uint16_t period)
{
  #if (defined BSP_LED_BLINK_ENABLE) && (BSP_LED_BLINK_ENABLE == TRUE)
    BSPLedControl_t *ctl;

    if (percent && period)
    {
        ctl = &s_BSPLedControlTable[BSP_LEDx];

        if (ctl->mode < BSP_LED_MODE_BLINK)
        {
            ctl->preBlinkState = ctl->mode;
        }

        ctl->mode = BSP_LED_MODE_OFF;
        ctl->time = period;
        ctl->onPct = percent;
        ctl->todo = numBlinks;
        if (!numBlinks) ctl->mode |= BSP_LED_MODE_FLASH;
        ctl->next = OS_Clock_GetSystemClock();
        ctl->mode |= BSP_LED_MODE_BLINK;
        
        OS_Timer_Stop(g_HalTask_Id, HAL_TASK_LED_BLINK_EVENT);
        OS_Events_Set(g_HalTask_Id, HAL_TASK_LED_BLINK_EVENT);
    }
  #endif
}

void BSP_LED_Update(void)
{
  #if (defined BSP_LED_BLINK_ENABLE) && (BSP_LED_BLINK_ENABLE == TRUE)
    BSPLedControl_t *ctl;
    uint32_t time;
    uint16_t next;
    uint16_t wait;
    uint8_t pct;

    
    for (uint8_t i=0; i < BSP_LEDCOUNT; i++)
    {
        ctl = &s_BSPLedControlTable[i];
        if (ctl->mode & BSP_LED_MODE_BLINK)
        {
            time = OS_Clock_GetSystemClock();
            if (time >= ctl->next)
            {
                if (ctl->mode & BSP_LED_MODE_ON)
                {
                    pct = 100 - ctl->onPct;
                    ctl->mode &= ~BSP_LED_MODE_ON;
                    
					GPIO_WritePinOutput(LED_IO[i].Pin.GPIO_Type_base, LED_IO[i].Pin.pin, LED_IO[i].OffLevel);
					
                    if (!(ctl->mode & BSP_LED_MODE_FLASH))
                    {
                        ctl->todo--;
                    }
                }
                else if ((!ctl->todo) && !(ctl->mode & BSP_LED_MODE_FLASH))
                {
                    ctl->mode ^= BSP_LED_MODE_BLINK;
                }
                else
                {
                    pct = ctl->onPct;
                    ctl->mode |= BSP_LED_MODE_ON;
                   
					GPIO_WritePinOutput(LED_IO[i].Pin.GPIO_Type_base, LED_IO[i].Pin.pin, LED_IO[i].OnLevel);
                }

                if (ctl->mode & BSP_LED_MODE_BLINK)
                {
                    wait = (((uint32_t)pct * (uint32_t)ctl->time) / 100);
                    ctl->next = time + wait; // next change time
                }
                else
                {
                    wait = 0;
                    if (ctl->preBlinkState)
						GPIO_WritePinOutput(LED_IO[i].Pin.GPIO_Type_base, LED_IO[i].Pin.pin, LED_IO[i].OnLevel);
                    else
						GPIO_WritePinOutput(LED_IO[i].Pin.GPIO_Type_base, LED_IO[i].Pin.pin, LED_IO[i].OffLevel);

                    ctl->preBlinkState = BSP_LED_MODE_OFF;
                }
            }
            else
            {
                wait = ctl->next - time; // time left
            }

            if (!next || (wait && (wait < next)))
            {
                next = wait;
            }
        }
    }
    
    if (next)
    {
        OS_Timer_Start(g_HalTask_Id, HAL_TASK_LED_BLINK_EVENT, next);
    }
  #endif
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

