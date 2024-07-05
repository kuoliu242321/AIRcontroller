#ifndef __COMMANDER_H
#define __COMMANDER_H
#include <stdbool.h>
#include "cmsis_os.h"
#include "radiolink.h"
#include "joystick.h"
#include <string.h>


// 1低速   2中速    3高速
#define Flightspeed	1

// remoter to aircraft 帧头
#define PACK_DATE    0xA1 //角度数据帧   (int16)((persent*Rang +500)*100)
#define PACK_KEY     0xA8 //按键数据帧
#define PACK_TEST    0xA2 //测试帧，检查连接情况，给飞控发送开始自检指令
#define PACK_ERROR   0xA3 //发送队列取出数据失败
#define PACK_END     0xA5 //结束帧

//addition

/*按键值*/
#define	Joystick_L_Value   0xB1
#define	Joystick_R_Value   0xB2
#define	KEY1_Value         0xB3
#define	KEY2_Value         0xB4
#define	KEY3_Value         0xB5



void commanderTask(void);
float limit(float value,float min, float max);
void Packdata(uint8_t PACK_START,uint8_t PACK_ADDITION);


#endif /*__COMMANDER_H*/
