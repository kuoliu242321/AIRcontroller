#ifndef   _LINK2REMOTE_H
#define   _LINK2REMOTE_H



#include "control.h"
#include "24l01.h"
#include "main.h"
#include "stm32f411_lee_conf.h"




osStatus_t Link2Rem_ReceivePacket(uint8_t *p);
osStatus_t Link2Rem_ReceivePacketBlocking(uint8_t *p);

void radiolinkTask(void);
void link2rem_SendResult_of_Test(uint8_t Test_Status);
void Link2Remote_Init(void);



#endif /* _LINK2REMOTE_H */

