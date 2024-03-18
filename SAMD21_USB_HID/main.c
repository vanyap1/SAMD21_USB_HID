#include <atmel_start.h>
#include "pin_config.h"
#include <string.h>
#include "hw_driver.h"

#include "socket.h"
#include "http_parser.h"

#include "RFM69registers.h"
#include "RFM69.h"

#include "rtc.h"


#define POWERBANKID		0x21
#define VFDSCREEN		0x03
#define DEVMODULE		0x22
#define DEVMODULE2		0xFE
#define DEVUSBHID		0xFD


#define NETWORKID		33
#define NODEID			DEVUSBHID
#define ALLNODES		0xfe
#define SMARTSCREEN		0xf0
#define RX_MODE			1
#define RTC_SYNC		0x81
#define MSG				0x82
#define POWERBANK		0x83

#define TX_MUTE			0
#define TX_UNMUTE		1


uint8_t txLen;
uint8_t txCRC;
rfHeader rfTxDataPack;



rtc_date sys_rtc = {
	.date = 4,
	.month = 12,
	.year = 23,
	.dayofweek = 1,
	.hour = 23,
	.minute = 54,
	.second = 00
};

wiz_NetInfo netInfo = { .mac  = {0x20, 0xcf, 0xF0, 0x82, 0x76, 0x00}, // Mac address
.ip   = {192, 168, 1, 99},         // IP address
.sn   = {255, 255, 255, 0},         // Subnet mask
.dns =  {8,8,8,8},			  // DNS address (google dns)
.gw   = {192, 168, 1, 1}, // Gateway address
.dhcp = NETINFO_STATIC};    //Static IP configuration

uint16_t socketPort[8] = {80, 23, 8080, 8080, 8080, 8080, 8080, 5000};
uint8_t rx_tx_buff_sizes[]={2,2,2,2,2,2,2,2};
	
uint8_t rtcData[64];	
uint8_t testMsg[64];	

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
	hid_generic_in_report[6] = GetBtnState() ? 0x00 : 0xff;
	hiddf_generic_write(hid_generic_in_report, 64);
	return false;
}

//static void usbd_sof_event(void)
//{
	//static uint16_t interval;
	//if (interval++ > 100) {
		//interval = 0;
		////usb routine
		////gpio_toggle_pin_level(GLD);
	//}
//}
//
//static struct usbdc_handler usbd_sof_event_h = {NULL, (FUNC_PTR)usbd_sof_event};


int main(void)
{
	
	//atmel_start_init();
	mcu_init();
	rtc_set(&sys_rtc);
	rtc_int_enable(&sys_rtc);
	
		reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
		reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_Write_byte);
		reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);
		
		rfm69_init(868, NODEID, NETWORKID);
		setHighPower(true);
		
		
		wizphy_reset();
		delay_ms(100);
		wizchip_init(rx_tx_buff_sizes,rx_tx_buff_sizes);
		wizchip_setnetinfo(&netInfo);
		ctlnetwork(CN_SET_NETINFO, (void*) &netInfo);


		char http_ansver[128]="\0";
		
	
	
	hid_generic_example();
	//usbdc_register_handler(USBDC_HDL_SOF, &usbd_sof_event_h);
	hiddf_generic_register_callback(HIDDF_GENERIC_CB_READ, (FUNC_PTR)usb_device_cb_generic_out);
	hiddf_generic_read(hid_generic_out_report, 64);

	

	
	/* Replace with your application code */
	while (1) {
		delay_ms(50);
		ETH_SPI_WriteBuff(hid_generic_in_report, 8);
		gpio_set_pin_level(LED_SD, GetBtnState());
		
		
		if (RTC_IRQ_Ready())
		{
			rtc_sync(&sys_rtc);
			sprintf(rtcData, "%02d:%02d:%02d", sys_rtc.hour, sys_rtc.minute, sys_rtc.second);
			
			if(TX_UNMUTE){
				rfTxDataPack.destinationAddr = ALLNODES;
				rfTxDataPack.senderAddr = NODEID;
				rfTxDataPack.opcode = MSG;
				rfTxDataPack.rxtxBuffLenght = sizeof(rtcData);
				rfTxDataPack.dataCRC = simpleCRC(&rtcData, sizeof(rtcData));
				sendFrame(&rfTxDataPack, &rtcData);
			}
			
			
			
			gpio_toggle_pin_level(RLD);
		}
		
		if (rf_isReady()) {
			gpio_set_pin_level(GLD, true);
			rfHeader* rfRxDataMsg=rfMsgType();
			
			switch(rfRxDataMsg->opcode) {
				case MSG:
				memcpy(&testMsg, DATA, sizeof(testMsg));
				break;
				case RTC_SYNC:
				memcpy(&sys_rtc, DATA, sizeof(sys_rtc));
				rtc_set(&sys_rtc);
				//if(sys_rtc.second == 0){lcdInitReq=1;}
				break;
				default:
				delay_us(1);
				//gpio_toggle_pin_level(GLD);
			}
			gpio_set_pin_level(GLD, false);
			}
		
		
		
		
		
		
		uint8_t HTTP_SOCKET = 0;
		for(HTTP_SOCKET = 0; HTTP_SOCKET <= 7; HTTP_SOCKET++){
			if(getSn_SR(HTTP_SOCKET) == SOCK_ESTABLISHED){
				uint8_t rIP[4];
				getsockopt(HTTP_SOCKET, SO_DESTIP, rIP);
				
				uint16_t res_size = getSn_RX_RSR(HTTP_SOCKET);
				if (res_size > sizeof(TCP_RX_BUF)){
					res_size = sizeof(TCP_RX_BUF);
				}
				memset(TCP_RX_BUF, 0, sizeof(DATA_BUFF_SIZE));
				
				
				
				sprintf(http_ansver ,"<p><span style=\"color: #00ff00;\"><strong>data</strong></span></p>\n\r");
				send(HTTP_SOCKET, (uint8_t*)http_ansver, strlen(http_ansver));
				
				
				
				recv(HTTP_SOCKET, (uint8_t*)TCP_RX_BUF, res_size);
				sprintf(http_ansver, "SOCKET NUM: %d;<br>RTC: %02d:%02d:%02d; \nRead bytes: %d<br>" , HTTP_SOCKET, sys_rtc.hour, sys_rtc.minute, sys_rtc.second,res_size);
				send(HTTP_SOCKET, (uint8_t*)http_ansver, strlen(http_ansver));
				sprintf(http_ansver ,"IP:%d.%d.%d.%d<br>", rIP[0],rIP[1],rIP[2],rIP[3]);
				send(HTTP_SOCKET, (uint8_t*)http_ansver, strlen(http_ansver));
				
				//send(HTTP_SOCKET, (uint8_t*)TCP_RX_BUF, strlen(TCP_RX_BUF));
				
				
				
				
				char *get_request = strtok(TCP_RX_BUF, "\r");
				//if(strstr(get_request, "GET") != NULL && strstr(get_request, "favicon") == NULL){
					////uint16_t index = position - get_request;
					//printf("IP:%d.%d.%d.%d\r\n", rIP[0],rIP[1],rIP[2],rIP[3]);
					//printf("%s\n\r",get_request);
//
					//int value;
					//if(extractValue(get_request,"value1", &value)){
						//printf("value1 = %d\n\r", value);
					//}
					//
					//if(extractValue(get_request,"value2", &value)){
						//printf("value2 = %d\n\r", value);
					//}
					//char valueStr[20];
					//if(extractString(get_request,"time", valueStr)){
						//printf("time = %s\n\r", valueStr);
						//sscanf(valueStr, "%hhu:%hhu:%hhu", &sys_rtc.hour, &sys_rtc.minute, &sys_rtc.second);
						//rtc_set(&sys_rtc);
					//}
					//if(extractString(get_request,"help", valueStr)){
						//send(HTTP_SOCKET, (uint8_t*)TCP_RX_BUF, strlen(TCP_RX_BUF));
					//}
					////sprintf(http_ansver ,"HTTP/1.0 200 OK\n\r");
					////send(HTTP_SOCKET, (uint8_t*)http_ansver, strlen(http_ansver));
				//}
				//
				disconnect(HTTP_SOCKET);
				//close(HTTP_SOCKET);
				//set_pin_level(&ldA, false);
			}
			
			if(getSn_SR(HTTP_SOCKET) == SOCK_CLOSE_WAIT){
				disconnect(HTTP_SOCKET);
				//close(HTTP_SOCKET);
			}
			
			if(getSn_SR(HTTP_SOCKET) == SOCK_CLOSED){
				socket(HTTP_SOCKET, Sn_MR_TCP, socketPort[HTTP_SOCKET], 0);
				listen(HTTP_SOCKET);
			}
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	}
}
