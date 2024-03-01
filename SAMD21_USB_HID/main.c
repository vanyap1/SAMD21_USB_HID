#include <atmel_start.h>
#include "atmel_start_pins.h"
#include <string.h>

static uint8_t hid_generic_in_report[64];
static uint8_t hid_generic_out_report[64];
static uint8_t b_btn_last_state;

static bool usb_device_cb_generic_out(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
	hiddf_generic_read(hid_generic_out_report, 64);
	
	if (hid_generic_out_report[0] == 0) {
		// led be off
		switch (hid_generic_out_report[1]) {
			case 1:
			gpio_set_pin_level(GLD, true);
			break;
			case 2:
			gpio_set_pin_level(GLD, false);
			break;
		}
	}
	hid_generic_in_report[5] = 0x55;
	hiddf_generic_write(hid_generic_in_report, 64);
	return false;
}

static void usbd_sof_event(void)
{
	static uint16_t interval;
	if (interval++ > 100) {
		interval = 0;
		//usb routine
		gpio_toggle_pin_level(GLD);
	}
}

static struct usbdc_handler usbd_sof_event_h = {NULL, (FUNC_PTR)usbd_sof_event};


int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	hid_generic_example();
	usbdc_register_handler(USBDC_HDL_SOF, &usbd_sof_event_h);
	hiddf_generic_register_callback(HIDDF_GENERIC_CB_READ, (FUNC_PTR)usb_device_cb_generic_out);
	hiddf_generic_read(hid_generic_out_report, 64);

	EXTERNAL_IRQ_0_example();


	
	/* Replace with your application code */
	while (1) {
		delay_ms(100);
		
		gpio_set_pin_level(LED_SD, GetBtnState());
	}
}
