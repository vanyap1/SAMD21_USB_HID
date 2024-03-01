
/*
 * hw_init.c
 *
 * Created: 28.02.2024 22:16:59
 *  Author: Vanya
 */ 
#include "hw_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>


uint8_t btnState;
uint8_t i2cIntCount = 0;
uint8_t rtcIntCount = 0;
uint8_t rfIntCount = 0;


	
static void BTN_Handler(void){
	//btnState = gpio_get_pin_level(BTN0);
	btnState = 1;
}

uint8_t GetBtnState(void){
	return btnState;
}

static void EXT_I2C_IRQ_int_Handler(void){
	i2cIntCount++;
	btnState=1;
}

static void I2C_RTC_Handler(void){
	rtcIntCount++;
	btnState=1;
}

static void RF_int_Handler(void){
	rfIntCount++;
	btnState=1;
}

uint8_t EXT_I2C_IRQ_isReady(void){
	uint8_t res = i2cIntCount;
	i2cIntCount = 0;
	return res;
}



void usb_hid_init(void){
	//CLock init
	_pm_enable_bus_clock(PM_BUS_APBB, USB);
	_pm_enable_bus_clock(PM_BUS_AHB, USB);
	_gclk_enable_channel(USB_GCLK_ID, CONF_GCLK_USB_SRC);
	
	usb_d_init();
	//USB io pins init
	gpio_set_pin_direction(PA24, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(PA24, false);
	gpio_set_pin_pull_mode(PA24, GPIO_PULL_OFF);
	gpio_set_pin_function(PA24, PINMUX_PA24G_USB_DM);
	gpio_set_pin_direction(PA25, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(PA25, false);
	gpio_set_pin_pull_mode(PA25, GPIO_PULL_OFF);
	gpio_set_pin_function(PA25,PINMUX_PA25G_USB_DP);
	
}
#if (CONF_GCLK_USB_FREQUENCY > (48000000 + 48000000 / 400)) || (CONF_GCLK_USB_FREQUENCY < (48000000 - 48000000 / 400))
#warning USB clock should be 48MHz ~ 0.25% clock, check your configuration!
#endif


void gpio_init(void){
		gpio_set_pin_level(RLD, false);
		gpio_set_pin_direction(RLD, GPIO_DIRECTION_OUT);
		gpio_set_pin_function(RLD, GPIO_PIN_FUNCTION_OFF);
		
		gpio_set_pin_level(GLD, false);
		gpio_set_pin_direction(GLD, GPIO_DIRECTION_OUT);
		gpio_set_pin_function(GLD, GPIO_PIN_FUNCTION_OFF);

		gpio_set_pin_level(LED_SD, false);
		gpio_set_pin_direction(LED_SD, GPIO_DIRECTION_OUT);
		gpio_set_pin_function(LED_SD, GPIO_PIN_FUNCTION_OFF);
		
		gpio_set_pin_direction(SD_DETECT, GPIO_DIRECTION_IN);
		gpio_set_pin_pull_mode(SD_DETECT, GPIO_PULL_OFF);
		gpio_set_pin_function(SD_DETECT, GPIO_PIN_FUNCTION_OFF);
}


void EXTERNAL_IRQ_init(void){
	_gclk_enable_channel(EIC_GCLK_ID, CONF_GCLK_EIC_SRC);

	gpio_set_pin_direction(BTN0, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(BTN0, GPIO_PULL_OFF);
	gpio_set_pin_function(BTN0, PINMUX_PB01A_EIC_EXTINT1);
	
	gpio_set_pin_direction(IO3, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(IO3, GPIO_PULL_UP);
	gpio_set_pin_function(IO3, PORT_PB08A_EIC_EXTINT8);
	
	gpio_set_pin_direction(RTC_IRQ, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(RTC_IRQ, GPIO_PULL_OFF);
	gpio_set_pin_function(RTC_IRQ, PINMUX_PA10A_EIC_EXTINT10);
		
	gpio_set_pin_direction(RF_IRQ, GPIO_DIRECTION_IN);
	gpio_set_pin_pull_mode(RF_IRQ, GPIO_PULL_DOWN);
	gpio_set_pin_function(RF_IRQ, PINMUX_PB30A_EIC_EXTINT14);
	
	ext_irq_register(BTN0, BTN_Handler);
	ext_irq_register(IO3, EXT_I2C_IRQ_int_Handler);
	ext_irq_register(RTC_IRQ, I2C_RTC_Handler);
	ext_irq_register(RF_IRQ, RF_int_Handler);
	
	

	ext_irq_init();
}


void mcu_init(void)
{
	init_mcu();
	gpio_init();
	usb_hid_init();
	EXTERNAL_IRQ_init();
}