#include "remoter_ctrl.h"


#define  LOW_SPEED_THRUST   (95.0f)
#define  LOW_SPEED_PITCH    (10.0f)
#define  LOW_SPEED_ROLL     (10.0f)

#define  MID_SPEED_THRUST   (95.0f)
#define  MID_SPEED_PITCH    (18.0f)
#define  MID_SPEED_ROLL     (18.0f)

#define  HIGH_SPEED_THRUST  (95.0f)
#define  HIGH_SPEED_PITCH   (30.0f)
#define  HIGH_SPEED_ROLL    (30.0f)

#define  MIN_THRUST			(25.0f)
#define  ALT_THRUST		    (50.0f)
#define  MAX_YAW			(45.0f)

static joystickFlyf_t flydata;
uint8_t packetData[12];
static uint8_t packetNum = 0;

enum dir_e Joystick1_DIR,Joystick2_DIR;
joystickFlyf_t  percent;


/*
para： PACK_START 帧头 判别是按键值还是数据值
			PACK_ADDITION 按键值 则为哪个按键 数据则为方向 先搞个NULL吧
*/
void Packdata(uint8_t PACK_START,uint8_t PACK_ADDITION)
{	
	
	if(packetNum>=200){
		packetNum = 0;
	}else{
		packetNum++;
	}
	
	if(PACK_START == PACK_DATE)
	{
		packetData[0] = PACK_START;
		packetData[1] = PACK_ADDITION;
		
		//+500 保证其为正数  *100保证两位小数的精度  16位有符号最高65536/2 传输完毕后要变换回来 
		//32存储规则  转换回来公式    (float)(((flydata.roll[5]<<8)|flydata.roll[4])/100.0f)-500;  重要一定除以100.0f
		*((uint16_t*)packetData+1) = (uint16_t)(flydata.thrust*10 + 1000);  // 3<<8 | 2     传输过直接拼接使用即可 1000~2000
		*((int16_t*)packetData+2) = (int16_t)((flydata.roll+500)*100);    // 5<<8 | 4
		*((int16_t*)packetData+3) = (int16_t)((flydata.pitch+500)*100);   // 7      6
		*((int16_t*)packetData+4) = (int16_t)((flydata.yaw+500)*100);     // 9      8
	}
	else if(PACK_START == PACK_KEY)
	{
		packetData[0] = PACK_START;
		packetData[1] = PACK_ADDITION;	
	}
	else if(PACK_START == PACK_TEST)
	{
		packetData[0] = PACK_START;
		packetData[1] = PACK_ADDITION;	
	}
	
	packetData[10] = packetNum;					  //这个非常重要，这是防止飞机逃脱遥控的保证
	packetData[11] = PACK_END;						  //校验码：  0xA5 ：1010 0101
	
	 radiolinkSendPacketBlocking(packetData);  
}



float limit(float value,float min, float max)
{
	if(value > max)
	{
		value = max;
	}
	else if(value < min)
	{
		value = min;
	}
	return value;
}


/*发送飞控命令任务*/
void commanderTask(void)
{

		
	ADCtoFlyDataPercent(&percent);
	
	//THRUST  使-1 ~ 1 的百分比 转化到 0~100 的范围值 传输时还要变换到1000~2000 的占空比值
	flydata.thrust = percent.thrust * ALT_THRUST;
	flydata.thrust += ALT_THRUST;
	flydata.thrust = limit(flydata.thrust, MIN_THRUST, LOW_SPEED_THRUST);
	
	//ROLL
	flydata.roll = percent.roll * LOW_SPEED_ROLL;
	flydata.roll = limit(flydata.roll, -LOW_SPEED_ROLL, LOW_SPEED_ROLL);
	//PITCH
	flydata.pitch = percent.pitch * LOW_SPEED_PITCH;
	flydata.pitch = limit(flydata.pitch, -LOW_SPEED_PITCH, LOW_SPEED_PITCH);
	//YAW
	flydata.yaw = percent.yaw * MAX_YAW;
	flydata.yaw = limit(flydata.yaw, -MAX_YAW, MAX_YAW);
	

	/******
	以下内容自行修改为专属代码	
	*********/	
	Joystick1_DIR =  getJoystick1Dir(1);
	Joystick2_DIR =  getJoystick2Dir(1); // 组合成一个字节发送
	/*发送飞控数据*/
	Packdata(PACK_DATE,((Joystick1_DIR<<4)|Joystick2_DIR));
	

}

