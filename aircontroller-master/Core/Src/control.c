#include "control.h"


//角度环PID 
PID PID_ROL_Angle;
PID PID_PIT_Angle;

PID PID_YAW_Angle;

//角速度环PID 
PID PID_ROL_Rate;
PID PID_PIT_Rate;




FLOAT_POSTURE   Measurement_Angle;
FLOAT_XYZ				Measurement_Gyroscope;

static float YAW_Limit(float yaw_remote,float yaw_measure);

Motor_PWM_Typedef Motor_PWM;
extern osEventFlagsId_t CanFlyEventHandle;
extern REMOTE_DATA_TYPEDEF Remote_Date;



extern float test_rate_rol_P;
extern float test_rate_pit_P;
/******************************************************************************************
*函  数：void Control(FLOAT_POSTURE *att_in,FLOAT_XYZ *gyr_in, RC_TYPE *RemData_in)
*功  能：姿态控制,角度环控制和角速度环控制
*参  数：att_in：测量值
*        gry_in: MPU6050读取的角速度值
*        RemData_in : 遥控器设定值

*返回值：无
*备  注： 小四轴机头与电机示意图	
					 机头(Y+)
					   
				  M1    ↑    M2
					\   |   /
					 \  |  /
					  \ | /
			    ————————+————————>X+	
					  / | \
					 /  |  \
					/   |   \
				  M4    |    M3
					
	1. M1 M3电机逆时针旋转，M2 M4电机顺时针旋转
	2. pitch 抬头   正  Y变大     低头  负  Y变小
		 roll  右肩高 正  X变大     ***
		 yaw   左旋   正  Z变大     *** 
*******************************************************************************************/

void Control(FLOAT_POSTURE *att_in,FLOAT_XYZ *gyr_in, REMOTE_DATA_TYPEDEF *RemData_in)
{

	if(fabs(RemData_in->YAW)<4.0)
	{
		RemData_in->YAW = 0;
	}

	//角度环
	//PITH角度环PID （输入角度 输出角速度）
	PID_Process(&PID_PIT_Angle,RemData_in->PITCH,att_in->pit);
	//ROLL角度环PID （输入角度 输出角速度）
  PID_Process(&PID_ROL_Angle,RemData_in->ROLL,att_in->rol);
	
   
	//角速度环
	//PITH角速度环PID （输入角度环的输出，输出电机控制量）
	PID_Process(&PID_PIT_Rate,PID_PIT_Angle.PID_Out,(gyr_in->Y + 11));
	//PID_Process(&PID_PIT_Rate,0,(gyr_in->Y + 11));
	
	//ROLL角速度环PID （输入角度环的输出，输出电机控制量）
	PID_Process(&PID_ROL_Rate,PID_ROL_Angle.PID_Out,(gyr_in->X)); 
	//PID_Process(&PID_ROL_Rate,0,(gyr_in->X)); 
	
	//YAW角度环PID （输入角度，输出电机控制量）
	//让测量值和目标值控制在45度误差以内
	//防止自选过大系统过量调整带来的不稳定效果 
	//导致系统转向 需要加入每次转向或者翻转时的yaw角度判定  无头模式？
	RemData_in->YAW = YAW_Limit(RemData_in->YAW,att_in->yaw);
	
	PID_Process(&PID_YAW_Angle,RemData_in->YAW,att_in->yaw); 
		
	//动力分配（自己DIY是动力分配一定要好好研究，动力分配搞错飞机肯定飞不起来!!!）
	if(RemData_in->THRUST>1150)//当油门大于1150时和飞机解锁时动力分配才生效
	{                                                                                 
		Motor_PWM.Motor1_Compare = RemData_in->THRUST - PID_ROL_Rate.PID_Out + PID_PIT_Rate.PID_Out + PID_YAW_Angle.PID_Out;   
		Motor_PWM.Motor2_Compare = RemData_in->THRUST + PID_ROL_Rate.PID_Out + PID_PIT_Rate.PID_Out - PID_YAW_Angle.PID_Out;   
		Motor_PWM.Motor3_Compare = RemData_in->THRUST + PID_ROL_Rate.PID_Out - PID_PIT_Rate.PID_Out + PID_YAW_Angle.PID_Out;   
		Motor_PWM.Motor4_Compare = RemData_in->THRUST - PID_ROL_Rate.PID_Out - PID_PIT_Rate.PID_Out - PID_YAW_Angle.PID_Out;   
			
		Motor_PWM.Motor1_Compare = thrust_limit2(Motor_PWM.Motor1_Compare);
		Motor_PWM.Motor2_Compare = thrust_limit2(Motor_PWM.Motor2_Compare);
		Motor_PWM.Motor3_Compare = thrust_limit2(Motor_PWM.Motor3_Compare);
		Motor_PWM.Motor4_Compare = thrust_limit2(Motor_PWM.Motor4_Compare);	
	} 
	else
	{
		Motor_PWM.Motor1_Compare = 990;
		Motor_PWM.Motor2_Compare = 990;
		Motor_PWM.Motor3_Compare = 990;
		Motor_PWM.Motor4_Compare = 990;
	}
	
	//将此数值分配到定时器，输出对应占空比的PWM波
	/*
	通道      管脚       电调			电机
	CH1 ------PB4--------M2----- 左上 1	
	CH2 ------PB5--------M1----- 左下 4
	CH3 ------PB0--------M3----- 右上 2
	CH4 ------PB1--------M4----- 右下 3
	*/
    Motor_PWM_Config(Motor_PWM.Motor1_Compare,Motor_PWM.Motor4_Compare,Motor_PWM.Motor2_Compare,Motor_PWM.Motor3_Compare);
} 





//按键处理任务 ，在dataprocess中被调用
void KEY_Process_Task(uint8_t KEY_Value)
{
	//	uint8_t i;	
	if(KEY_Value == Joystick_L_Value)
	{
		//一键降落 ？
		//需要挂起平衡任务吗？
		//需要配合任务管理函数 或者新建一个专门处理此类的任务
		
//		for(i=0;i<=100;i++)
//    {
//			Remote_Date.THRUST-=8*i;
//			
//			Remote_Date.THRUST = thrust_limit2(Remote_Date.THRUST);
//		
//			BEEP_ON();		
//    }

		
		
		test_rate_rol_P = test_rate_rol_P + 0.1f;
		PID_Init();
		
		BEEP_ON();
		
		osDelay(500);
		
		BEEP_OFF();
		
		
		//printf("Joystick_L_Value\r\n");
	}
	if(KEY_Value == Joystick_R_Value)
	{
		//一键起飞
		
		test_rate_rol_P = test_rate_rol_P - 0.1f;
		PID_Init();
		
		BEEP_ON();
		osDelay(500);
		BEEP_OFF();
		//printf("Joystick_R_Value\r\n");
	}
	if(KEY_Value == KEY3_Value)
	{
		
		BEEP_ON();
		osDelay(500);
		BEEP_OFF();
		BEEP_ON();
		osDelay(500);
		BEEP_OFF();
		//printf("KEY3_Value\r\n");
	}
	if(KEY_Value == KEY2_Value)
	{
		
		BEEP_ON();
		osDelay(500);
		BEEP_OFF();
		BEEP_ON();
		osDelay(500);
		BEEP_OFF();
		
		//printf("KEY2_Value\r\n");
	}
	if(KEY_Value == KEY1_Value)
	{
		BEEP_ON();
		osDelay(500);
		BEEP_OFF();
		BEEP_ON();
		osDelay(500);
		BEEP_OFF();
		//printf("KEY1_Value\r\n");
	}
	else return;
}




bool PID_Init(void)
{
	PID_Arg_Init(&PID_PIT_Angle,&PID_ROL_Angle,&PID_PIT_Rate ,
								&PID_ROL_Rate ,&PID_YAW_Angle);
	return true;
}

/**
bit0 MPU检测初始化
bit1 NRF24L01初始化标志
bit2 电调初始化
bit3 PID控制初始化


返回
     0 完成
		 1 正在初始化
     2 累计十次失败，自检失败 

*/
uint8_t AirCraft_TestSelf(void)
{
	//检测标志位
	//
	printf("enter air testself\r\n");
	uint32_t fly_flag;
  static  uint8_t i;
	fly_flag = osEventFlagsGet(CanFlyEventHandle);
	
	if((fly_flag&CanFly_Status_Flag) == CanFly_Status_Flag)
	{
		
		link2rem_SendResult_of_Test(Test_Res_OK);
		link2rem_SendResult_of_Test(Test_Res_OK);
		link2rem_SendResult_of_Test(Test_Res_OK);
		NRF24L01_RX_Mode();
		printf("Test_Res_OK\r\n");
		//这里给信号量恢复任务？
		return 0;
	}
	else
	{
		link2rem_SendResult_of_Test(Test_Res_Initing);
		i++;
		osDelay(500);
		if(i == 50)
		{
			link2rem_SendResult_of_Test(Test_Res_Error);
			osDelay(100);
			i=0;
			printf("Test_Res_Error\r\n");
			return 2;
		}
		else
		{
			printf("Test_Res_Initing\r\n");
			return 1;		
		}
	}
	//各种测试 
	//返回值 0 完成   其他报错
}


//任务入口供调用
void AirCraft_Control_Task(void)
{

	Remote_Data_Process();
	
	//MPU数据获取？ 还是单独一个任务来处理？
	Get_Float_Gyroscope(&Measurement_Gyroscope.X,&Measurement_Gyroscope.Y,&Measurement_Gyroscope.Z);
	
	while(mpu_dmp_get_data(&Measurement_Angle.pit,&Measurement_Angle.rol,&Measurement_Angle.yaw));
	
			
	//temp=MPU_Get_Temperature();		
	
	//控制任务 未改完输入输出
	Control(&Measurement_Angle,&Measurement_Gyroscope,&Remote_Date);
	
	
/*
	//test 
	printf("Test 10.11 of Lee\r\n");
	printf("\r\n");
	printf("Gyroscope.X = %f\r\n",Measurement_Gyroscope.X);
	printf("Gyroscope.Y = %f\r\n",Measurement_Gyroscope.Y);
	printf("Gyroscope.Z = %f\r\n",Measurement_Gyroscope.Z);
	printf("\r\n");
	
	//欧拉角没有输出 ？ 怎么回事？  ->要循环读取FIFO，仅读取一次可能会失败
	
	printf("Angle.pit = %f\r\n",Measurement_Angle.pit);
	printf("Angle.rol = %f\r\n",Measurement_Angle.rol);
	printf("Angle.yaw = %f\r\n",Measurement_Angle.yaw);
	printf("\r\n");
	
	Motor_PWM_Config(990,990,990,990);
	
	osDelay(1000);
			
	//Motor_PWM_Config(Remote_Date.THRUST,Remote_Date.THRUST,
	//								Remote_Date.THRUST,Remote_Date.THRUST);
*/


}



static float YAW_Limit(float yaw_remote,float yaw_measure)
{
	
	if((yaw_measure<45)&&(yaw_measure>-45))
		return yaw_remote;
	else if((yaw_measure<135)&&(yaw_measure>45))
		return (yaw_remote+90);
	else if((yaw_measure<-45)&&(yaw_measure>-135))
		return (yaw_remote-90);
	else if((yaw_measure<180)&&(yaw_measure>135))
		return (yaw_remote*0.5f+157.5f);
	else if((yaw_measure<-135)&&(yaw_measure>-180))
		return (yaw_remote*0.5f-157.5f);
	else 
		return 0;
}



