/**
 * @file nvic.h
 * @brief NVIC header only driver
 *
 *
 *
 * Usage:
 *  - Include this header in your project.
 *  - Use `nvic_enable_interrupt()` and `nvic_disable_interrupt()` to control
 *    individual interrupt vectors.
 *
 */

#pragma once
#ifndef CRAP_NVIC_H

#include "stdint.h"

#include "common.h"

typedef enum
{
    NVIC_WWDG_INTR                   = 0,
    NVIC_PVD_INTR                    = 1,
    NVIC_TAMP_STAMP_INTR             = 2,
    NVIC_RTC_WKUP_INTR               = 3,
    NVIC_FLASH_INTR                  = 4,
    NVIC_RCC_INTR                    = 5,
    NVIC_EXTI0_INTR                  = 6,
    NVIC_EXTI1_INTR                  = 7,
    NVIC_EXTI2_INTR                  = 8,
    NVIC_EXTI3_INTR                  = 9,
    NVIC_EXTI4_INTR                  = 10,
    NVIC_DMA1_STREAM0_INTR           = 11,
    NVIC_DMA1_STREAM1_INTR           = 12,
    NVIC_DMA1_STREAM2_INTR           = 13,
    NVIC_DMA1_STREAM3_INTR           = 14,
    NVIC_DMA1_STREAM4_INTR           = 15,
    NVIC_DMA1_STREAM5_INTR           = 16,
    NVIC_DMA1_STREAM6_INTR           = 17,
    NVIC_ADC_INTR                    = 18,
    NVIC_EXTI9_5_INTR                = 23,
    NVIC_TIM1_BRK_TIM9_INTR          = 24,
    NVIC_TIM1_UP_TIM10_INTR          = 25,
    NVIC_TIM1_TRG_COM_TIM11_INTR     = 26,
    NVIC_TIM1_CC_INTR                = 27,
    NVIC_TIM2_INTR                   = 28,
    NVIC_TIM3_INTR                   = 29,
    NVIC_TIM4_INTR                   = 30,
    NVIC_I2C1_EV_INTR                = 31,
    NVIC_I2C1_ER_INTR                = 32,
    NVIC_I2C2_EV_INTR                = 33,
    NVIC_I2C2_ER_INTR                = 34,
    NVIC_SPI1_INTR                   = 35,
    NVIC_SPI2_INTR                   = 36,
    NVIC_USART1_INTR                 = 37,
    NVIC_USART2_INTR                 = 38,
    NVIC_EXTI15_10_INTR              = 40,
    NVIC_RTC_ALARM_INTR              = 41,
    NVIC_OTG_FS_WKUP_INTR            = 42,
    NVIC_DMA1_STREAM7_INTR           = 47,
    NVIC_SDIO_INTR                   = 49,
    NVIC_TIM5_INTR                   = 50,
    NVIC_SPI3_INTR                   = 51,
    NVIC_DMA2_STREAM0_INTR           = 56,
    NVIC_DMA2_STREAM1_INTR           = 57,
    NVIC_DMA2_STREAM2_INTR           = 58,
    NVIC_DMA2_STREAM3_INTR           = 59,
    NVIC_DMA2_STREAM4_INTR           = 60,
    NVIC_OTG_FS_INTR                 = 67,
    NVIC_DMA2_STREAM5_INTR           = 68,
    NVIC_DMA2_STREAM6_INTR           = 69,
    NVIC_DMA2_STREAM7_INTR           = 70,
    NVIC_USART6_INTR                 = 71,
    NVIC_I2C3_EV_INTR                = 72,
    NVIC_I2C3_ER_INTR                = 73,
    NVIC_FPU_INTR                    = 81,
    NVIC_SPI4_INTR                   = 84,
    NVIC_SPI5_INTR                   = 85
} NVIC_programmable_intr_t;

typedef struct
{
    __IO  uint32_t ISER[8U];              
    uint32_t RESERVED0[24U];
    __IO  uint32_t ICER[8U];               
    uint32_t RESERVED1[24U];
    __IO  uint32_t ISPR[8U];               
    uint32_t RESERVED2[24U];
    __IO  uint32_t ICPR[8U];               
    uint32_t RESERVED3[24U];
    __IO  uint32_t IABR[8U];               
    uint32_t RESERVED4[56U];
    __IO  uint8_t  IP[240U];               
    uint32_t RESERVED5[644U];
    __IO  uint32_t STIR;                   
}  NVIC_typedef_t;





#define NVIC_BASE (0xE000E100UL)

#define NVIC ((NVIC_typedef_t*) NVIC_BASE)


ALWAYS_INLINE void nvic_enable_interrupt(NVIC_programmable_intr_t intrnum)
{
    uint8_t ISER_idx = intrnum >> 5; //deside the register by dividing by 32 
    uint32_t ISER_intr_mask = 1 << (intrnum & 0x1F); // the remainder will be the bit number to which we should write
    NVIC->ISER[ISER_idx] = ISER_intr_mask;
   
}

ALWAYS_INLINE void nvic_disable_interrupt(NVIC_programmable_intr_t intrnum)
{
    uint8_t ICER_idx = intrnum >> 5;
    uint32_t ICER_intr_mask = 1 << (intrnum & 0x1F);
    NVIC->ICER[ICER_idx] = ICER_intr_mask;
} 


#endif