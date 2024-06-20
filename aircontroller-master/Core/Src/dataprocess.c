#include "dataprocess.h"

REMOTE_DATA_TYPEDEF Remote_Date;



void Remote_Data_Process(void)
{
	//从接受队列中取出数据开始处理 0表示取出成功
	uint8_t RX_DATA_Buffer[12];
	
	int16_t receive_thrust;
	
	if(Link2Rem_ReceivePacket(RX_DATA_Buffer) == osOK)  //这里出现问题？
	{
		//printf("取出数据成功\r\n");
		if(RX_DATA_Buffer[11] == PACK_END)
		{
			if(RX_DATA_Buffer[0] == PACK_DATE)
			{
				
				//这种是直接使用，但油门回中为1500，此时电机转速较快，需要时刻主要遥控的油门值
			//	Remote_Date.THRUST = (uint16_t)((RX_DATA_Buffer[3]<<8)|RX_DATA_Buffer[2]);
				
				//油门记忆功能，每次传输油门的增量，即遥感回中，油门值不在改变
				//油门大于1500 油门增加   油门小于1500 油门减少
				receive_thrust = (int16_t)((RX_DATA_Buffer[3]<<8)|RX_DATA_Buffer[2]);
				receive_thrust =(int16_t)((float)(receive_thrust-1500)*0.015f);
				
				Remote_Date.THRUST=(uint16_t)((int16_t)Remote_Date.THRUST+receive_thrust);
				Remote_Date.THRUST = thrust_limit(Remote_Date.THRUST);
				
				
				Remote_Date.ROLL   = (float)((((RX_DATA_Buffer[5]<<8)|RX_DATA_Buffer[4])/100.0f)-500);
				Remote_Date.PITCH  = (float)((((RX_DATA_Buffer[7]<<8)|RX_DATA_Buffer[6])/100.0f)-500);
				Remote_Date.YAW    = (float)((((RX_DATA_Buffer[9]<<8)|RX_DATA_Buffer[8])/100.0f)-500);
					
//				printf("THRUST = %d\r\n",Remote_Date.THRUST);
//				printf("PITCH = %f\r\n",Remote_Date.PITCH);
//				printf("ROLL = %f\r\n",Remote_Date.ROLL);
//				printf("YAW = %f\r\n",Remote_Date.YAW);
//				printf("\r\n");
				
				
			}
			else if(RX_DATA_Buffer[0] == PACK_KEY)
			{		
				KEY_Process_Task(RX_DATA_Buffer[1]);	
				
			}
			//测试任务在连接部分处理，不在此函数中
			else if(RX_DATA_Buffer[0] == PACK_ERROR)
			{
				//跳过此次 
				//航向灯闪烁提示错误		
			}	
		}
		else
			return;
	}	
}

//限制最低停转
uint16_t thrust_limit(uint16_t x_in)
{

	if(x_in<Motor_PWM_Min)
	{
		return Motor_PWM_Min;
	}
	else if(x_in>Motor_PWM_Max)
	{
		return Motor_PWM_Max;
	}
	return x_in;
}

//限制低俗旋转
uint16_t thrust_limit2(uint16_t x_in)
{

	if(x_in<Motor_PWM_Min)
	{
		return Motor_PWM_Min+20;
	}
	else if(x_in>Motor_PWM_Max)
	{
		return Motor_PWM_Max;
	}
	return x_in;
}



//陀螺仪±2000dpi 2000*原始值/32768 为测量的角速度 
void Get_Float_Gyroscope (float *gx,float *gy,float *gz)
{
	short gyrox,gyroy,gyroz;		//陀螺仪原始数据

	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
	
	*gx=(float)((gyrox*2000)/32768);
	*gy=(float)((gyroy*2000)/32768);
	*gz=(float)((gyroz*2000)/32768);

}


	

























