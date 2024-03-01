
/*
 * hw_init.h
 *
 * Created: 28.02.2024 22:16:34
 *  Author: Vanya
 */ 
#ifndef HW_INIT_INCLUDED
#define HW_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>
#include <hal_ext_irq.h>
#include <hal_spi_m_sync.h>
#include <hal_spi_m_sync.h>
#include <hal_i2c_m_sync.h>
#include <hal_i2c_m_sync.h>
#include "hal_usb_device.h"



static void BTN_Handler(void);
uint8_t GetBtnState(void);

static void EXT_I2C_IRQ_int_Handler(void);
static void I2C_RTC_Handler(void);
static void RF_int_Handler(void);
uint8_t EXT_I2C_IRQ_isReady(void);


void usb_hid_init(void);
void gpio_init(void);
void EXTERNAL_IRQ_init(void);




void mcu_init(void);

#ifdef __cplusplus
}
#endif
#endif // HW_INIT_INCLUDED