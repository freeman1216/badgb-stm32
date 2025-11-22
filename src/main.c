#include <stdint.h>


//Driver includes
#define BAD_RCC_IMPLEMENTATION
#include "rcc.h"
#include "flash.h"

#define BAD_IO_IMPLEMENTATION
#include "io.h"


#define BAD_EXTI_IMPLEMENTATION
#define BAD_EXTI_EXTI0_ISR_IMPLEMENTATION
#define BAD_EXTI_EXTI1_ISR_IMPLEMENTATION
#define BAD_EXTI_EXTI2_ISR_IMPLEMENTATION
#define BAD_EXTI_EXTI3_ISR_IMPLEMENTATION
#define BAD_EXTI_EXTI4_ISR_IMPLEMENTATION
#define BAD_EXTI_EXTI9_5_ISR_IMPLEMENTATION
#define BAD_EXTI_USE_EXTI_5_USER_ISR
#define BAD_EXTI_USE_EXTI_6_USER_ISR
#define BAD_EXTI_USE_EXTI_7_USER_ISR
#include "exti.h"


#define BTIMER_TIM1_UP_TIM10_ISR_IMPLEMENTATION
#define BTIMER_USE_TIM10_USR
#define BAD_TIMER_IMPLEMENTATION
#include "basic_timer.h"

#include "syscfg.h"

#define BAD_USART_IMPLEMENTATION
#include "uart.h"

#define BAD_ILI9341_STATIC
#define BAD_ILI9341_IMPLEMENTATION
#include "ili9341.h"

#include "scb.h"
//emulator includes
#include "cpu.h"
#include "timer.h"
#include "badstate.h"
#include "common.h"
#include "ppu.h"
#include "mem.h"

#define UART_GPIO_PORT          (GPIOA)
#define UART1_TX_PIN            (9)
#define UART1_RX_PIN            (10)
#define UART1_TX_AF             (7)
#define UART1_RX_AF             (7)

//spi pins
#define SPI_GPIO_PORT       (GPIOB)
#define SPI_SCK_PIN         (3)
#define SPI_MISO_PIN        (4)
#define SPI_MOSI_PIN        (5)
#define SPI_SCK_AF          (5)
#define SPI_MISO_AF         (5)
#define SPI_MOSI_AF         (5)
#define ILI9431_GPIO_PORT   (GPIOB)
#define ILI9431_RESET       (8)
#define ILI9341_DC          (7)
#define ILI9341_CS          (6)

//input pins
#define EXTI_GPIO_PORT          (GPIOA)
#define SYS_CFG_INPUT_PORT      (SYSCFG_PAx)
#define INPUT_RIGHT_PIN         (0)
#define INPUT_LEFT_PIN          (1)
#define INPUT_UP_PIN            (2)
#define INPUT_DOWN_PIN          (3)
#define INPUT_A_PIN             (4)
#define INPUT_B_PIN             (5)
#define INPUT_START_PIN         (6)
#define INPUT_SELECT_PIN        (7)


    // HSE  = 25
    // PLLM = 25
    // PLLN = 400
    // PLLQ = 10
    // PLLP = 4
    // Sysclock = 100
#define BAD_GB_PLLM (25)
#define BAD_GB_PLLN (400)
#define BAD_GB_PLLQ (10)
#define BAD_GB_FLASH_LATENCY (FLASH_LATENCY_3ws)

#define BAD_GB_AHB1_PERIPEHRALS    (RCC_AHB1_GPIOA|RCC_AHB1_DMA2|RCC_AHB1_GPIOB)
#define BAD_GB_APB2_PERIPHERALS    (RCC_APB2_USART1|RCC_APB2_SPI1|RCC_APB2_SYSCFGEN|RCC_APB2_TIM10)

extern uint32_t __ram_ivt;


#define BAD_GB_FRAMELIMIT_TIMER_ARR     (65535)
#define BAD_GB_FRAMELIMIT_TIMER_PSC     (1525)
#define BAD_GB_FRAMELIMIT_TIMER_INTR    (BTIMER_UPDATE)
#define BAD_GB_FRAMELIMIT_TIMER         (BTIM10)

ATTR_RAMFUNC void exti0_usr(){
    badstate.buttons.right^=1;
    flag_joypad_interrupt_dpad();
}

ATTR_RAMFUNC void exti1_usr(){
    badstate.buttons.left^=1;
    flag_joypad_interrupt_dpad();
}

ATTR_RAMFUNC void exti2_usr(){
    badstate.buttons.up^=1;
    flag_joypad_interrupt_dpad();
}

ATTR_RAMFUNC void exti3_usr(){
    badstate.buttons.down^=1;
    flag_joypad_interrupt_dpad();
}

ATTR_RAMFUNC void exti4_usr(){
    badstate.buttons.A^=1;
    flag_joypad_interrupt_buttons();
}

ATTR_RAMFUNC void exti5_usr(){
    badstate.buttons.B^=1;
    flag_joypad_interrupt_buttons();
}

ATTR_RAMFUNC void exti6_usr(){
    badstate.buttons.start^=1;    
    flag_joypad_interrupt_buttons();
}

ATTR_RAMFUNC void exti7_usr(){
    badstate.buttons.select^=1;
    flag_joypad_interrupt_buttons();
}


static inline void __main_clock_setup(){
    rcc_enable_hse();
    rcc_pll_setup( PLLP4, BAD_GB_PLLM, BAD_GB_PLLN, BAD_GB_PLLQ, PLL_SOURCE_HSE);
    rcc_bus_prescalers_setup(HPRE_DIV_1, PPRE_DIV_2, PPRE_DIV_1);
    flash_acceleration_setup(BAD_GB_FLASH_LATENCY, FLASH_DCACHE_ENABLE, FLASH_ICACHE_ENABLE);
    rcc_enable_and_switch_to_pll();
}

static inline void __periph_setup(){
    rcc_set_ahb1_clocking(BAD_GB_AHB1_PERIPEHRALS);
    io_setup_pin(UART_GPIO_PORT, UART1_TX_PIN, MODER_af, UART1_TX_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(UART_GPIO_PORT, UART1_RX_PIN, MODER_af, UART1_RX_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    //spi pins
    io_setup_pin(SPI_GPIO_PORT, SPI_SCK_PIN, MODER_af, SPI_SCK_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(SPI_GPIO_PORT, SPI_MISO_PIN, MODER_af, SPI_MISO_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(SPI_GPIO_PORT, SPI_MOSI_PIN, MODER_af, SPI_MOSI_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(ILI9431_GPIO_PORT, ILI9341_CS, MODER_output, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(ILI9431_GPIO_PORT, ILI9431_RESET, MODER_output, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(ILI9431_GPIO_PORT, ILI9341_DC, MODER_output, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    //input exti pins 
    io_setup_pin(EXTI_GPIO_PORT, INPUT_RIGHT_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, INPUT_LEFT_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, INPUT_UP_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, INPUT_DOWN_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, INPUT_A_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, INPUT_B_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, INPUT_START_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, INPUT_SELECT_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);

    //io_setup_pin(INPUT_LEFT_GPIO_PORT, EXTI1_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    rcc_set_apb2_clocking(BAD_GB_APB2_PERIPHERALS);
}


static inline void __input_setup(){
    syscfg_set_exti_pin(SYS_CFG_INPUT_PORT, INPUT_RIGHT_PIN);
    syscfg_set_exti_pin(SYS_CFG_INPUT_PORT, INPUT_LEFT_PIN);
    syscfg_set_exti_pin(SYS_CFG_INPUT_PORT, INPUT_UP_PIN);
    syscfg_set_exti_pin(SYS_CFG_INPUT_PORT, INPUT_DOWN_PIN);
    syscfg_set_exti_pin(SYS_CFG_INPUT_PORT, INPUT_A_PIN);
    syscfg_set_exti_pin(SYS_CFG_INPUT_PORT, INPUT_B_PIN);
    syscfg_set_exti_pin(SYS_CFG_INPUT_PORT, INPUT_START_PIN);
    syscfg_set_exti_pin(SYS_CFG_INPUT_PORT, INPUT_SELECT_PIN);
    
    exti_configure_line(INPUT_RIGHT_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(INPUT_LEFT_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(INPUT_UP_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(INPUT_DOWN_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(INPUT_A_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(INPUT_B_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(INPUT_START_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(INPUT_SELECT_PIN, EXTI_TRIGGER_BOTH);

    nvic_enable_interrupt(NVIC_EXTI0_INTR);
    nvic_enable_interrupt(NVIC_EXTI1_INTR);
    nvic_enable_interrupt(NVIC_EXTI2_INTR);
    nvic_enable_interrupt(NVIC_EXTI3_INTR);
    nvic_enable_interrupt(NVIC_EXTI4_INTR);
    nvic_enable_interrupt(NVIC_EXTI9_5_INTR);

}


static inline void __framelimiter_setup(){
    basic_timer_setup(BAD_GB_FRAMELIMIT_TIMER, BAD_GB_FRAMELIMIT_TIMER_ARR,
                      BAD_GB_FRAMELIMIT_TIMER_PSC, BAD_GB_FRAMELIMIT_TIMER_INTR);
    nvic_enable_interrupt(NVIC_TIM1_UP_TIM10_INTR);
    tim_enable(BAD_GB_FRAMELIMIT_TIMER);
}
static volatile uint8_t second_elapsed;

ATTR_RAMFUNC void tim10_usr(){
   second_elapsed = 1; 
}


ATTR_RAMFUNC __attribute((noinline)) void mainloop(){
    while (1) {
        while (badstate.display.frame<=60) {
            update_cpu();
            update_ppu(badstate.cpu.cycles);
            update_timer(badstate.cpu.cycles);
            badstate.cpu.cycles = 0;
        }
        while(!second_elapsed);
        second_elapsed = 0;
        badstate.display.frame = 0;
    }
}

int main(){
    __DISABLE_INTERUPTS;
    __main_clock_setup();
    __periph_setup();
   
    ili9341_spi_init();
    SCB->VTOR = (uint32_t)&__ram_ivt; 

    
    
    __ENABLE_INTERUPTS;
    ili9341_init();   
    ili9341_fill(0x0000);
    mem_init();
    badstate_init();
    __input_setup();
    __framelimiter_setup();
    mainloop();
    while(1){

    }
    return 0;
}
