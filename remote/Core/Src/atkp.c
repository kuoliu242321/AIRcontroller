#include "atkp.h"


float plane_yaw,plane_roll,plane_pitch;
float plane_bat;
uint8_t rssi;

/*atkp½âÎö*/
/*
static void atkpAnalyze(atkp_t *p)
{
	if(p->msgID == UP_STATUS)
	{
		plane_roll = (int16_t)(*(p->data+0)<<8)|*(p->data+1);
		plane_roll = plane_roll/100;
		plane_pitch = (int16_t)(*(p->data+2)<<8)|*(p->data+3);
		plane_pitch = plane_pitch/100;
		plane_yaw = (int16_t)(*(p->data+4)<<8)|*(p->data+5);
		plane_yaw = plane_yaw/100;
	}
	else if(p->msgID == UP_POWER)
	{
		plane_bat = (int16_t)(*(p->data+0)<<8)|*(p->data+1);
		plane_bat = plane_bat/100;
	}
	else if(p->msgID == UP_REMOTOR)	
	{
		switch(p->data[0])
		{
			case ACK_MSG:
				miniFlyMsgACKProcess(p);
				break;
		}
	}
	
}
*/


