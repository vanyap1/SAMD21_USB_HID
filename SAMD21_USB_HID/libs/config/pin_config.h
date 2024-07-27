#ifndef PIN_CONFIG_H_INCLUDED
#define PIN_CONFIG_H_INCLUDED

#include <hal_gpio.h>

// SAMD21 has 8 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7

#define RLD GPIO(GPIO_PORTA, 0)
#define PA01 GPIO(GPIO_PORTA, 1)
#define PA04 GPIO(GPIO_PORTA, 4)
#define PA05 GPIO(GPIO_PORTA, 5)
#define PA06 GPIO(GPIO_PORTA, 6)
#define PA08 GPIO(GPIO_PORTA, 8)
#define PA09 GPIO(GPIO_PORTA, 9)
#define PA10 GPIO(GPIO_PORTA, 10)
#define PA18 GPIO(GPIO_PORTA, 18)

#define ETH_CS GPIO(GPIO_PORTA, 21)
#define PA24 GPIO(GPIO_PORTA, 24)
#define PA25 GPIO(GPIO_PORTA, 25)
#define LED1 GPIO(GPIO_PORTB, 0)
#define BTN0 GPIO(GPIO_PORTB, 1)
#define SD_ACT GPIO(GPIO_PORTB, 3)
#define PB08 GPIO(GPIO_PORTB, 8)
#define PB12 GPIO(GPIO_PORTB, 12)
#define PB13 GPIO(GPIO_PORTB, 13)

#define PB30 GPIO(GPIO_PORTB, 30)


#define RLD GPIO(GPIO_PORTB, 31)
#define PA24 GPIO(GPIO_PORTA, 24)
#define PA25 GPIO(GPIO_PORTA, 25)
#define GLD GPIO(GPIO_PORTB, 0)
#define BTN0 GPIO(GPIO_PORTB, 1)

#define RF_LED GPIO(GPIO_PORTB, 10)
#define ETH_LED GPIO(GPIO_PORTB, 11)

//RF module pins
#define PA00 GPIO(GPIO_PORTA, 0)		//MOSI
#define PA01 GPIO(GPIO_PORTA, 1)		//CLK
#define PA18 GPIO(GPIO_PORTA, 18)		//MISO
#define RF_RST GPIO(GPIO_PORTA, 2)		//RF Reset
#define RF_IRQ GPIO(GPIO_PORTB, 30)		//RF IRQ
#define RF_CS GPIO(GPIO_PORTA, 28)		//RF Select pin

//SPI FLASH MEMORY (25q32)
//USED RF SPI
#define ROM_CS GPIO(GPIO_PORTA, 23)		//SPI flash CS

//SD Card pins



//USED RF SPI
#define LED_SD GPIO(GPIO_PORTA, 3)		//SD Card activity LED
#define SD_CS GPIO(GPIO_PORTB, 4)		//SD Cart select pin
#define SD_DETECT GPIO(GPIO_PORTB, 5)	//SD Card insertion detect pin

//EXT LCD SPI
#define PA04 GPIO(GPIO_PORTA, 4)		//MOSI
#define PA05 GPIO(GPIO_PORTA, 5)		//CLK
#define PA06 GPIO(GPIO_PORTA, 6)		//MISO

//ETH_SPI
#define PB16 GPIO(GPIO_PORTB, 16)		//MISO
#define PB17 GPIO(GPIO_PORTB, 17)		//CLK
#define PA20 GPIO(GPIO_PORTA, 20)		//MOSI
#define PA21 GPIO(GPIO_PORTA, 21)		//ETH_CS

//EXT IO HEADER
#define FILAMENT_EN GPIO(GPIO_PORTB, 6)	// IO0
#define IO4 GPIO(GPIO_PORTB, 7)			// IO4
#define IO3 GPIO(GPIO_PORTB, 8)			// IO3
#define VFD_CS GPIO(GPIO_PORTB, 9)		// IO1
#define LCD_RST GPIO(GPIO_PORTA, 7)		// IO2

//EXT I2C
#define PB12 GPIO(GPIO_PORTB, 12)		//SDA
#define PB13 GPIO(GPIO_PORTB, 13)		//SCL

//RTC I2C and interrupt
#define PA08 GPIO(GPIO_PORTA, 8)		//SDA
#define PA09 GPIO(GPIO_PORTA, 9)		//SCL
#define RTC_IRQ GPIO(GPIO_PORTA, 10)	//IRQ
#endif // ATMEL_START_PINS_H_INCLUDED