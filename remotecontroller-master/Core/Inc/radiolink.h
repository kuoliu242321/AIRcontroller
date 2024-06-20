#ifndef __RADIO_LINK_H
#define __RADIO_LINK_H

#include <stdbool.h>
#include "main.h"
#include "24l01.h"
#include "stm32f030_lee_conf.h"
#include "radiolink.h"
#include "management.h"



/*上行指令*/
#define  U_RADIO_RSSI		0x01
#define  U_RADIO_CONFIG		0x02	

/*下行指令*/
#define  D_RADIO_HEARTBEAT	0xFF
#define  D_RADIO_GET_CONFIG	0x01
#define  D_RADIO_SET_CONFIG	0x02

#define Test_Res_Initing 0xAA
#define Test_Res_Error   0xBB
#define Test_Res_OK      0xCC



void radiolinkInit(void);
osStatus_t radiolinkSendPacket(const uint8_t *p);
osStatus_t radiolinkSendPacketBlocking(const uint8_t *p);

void radiolinkTask(void);
uint16_t radioinkFailRxcount(void);



#endif /*RADIO_LINK_H*/
