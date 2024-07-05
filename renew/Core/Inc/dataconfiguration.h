#ifndef   _DATACONFIGURATION_H
#define   _DATACONFIGURATION_H

#include "main.h"




#define PACK_DATE 0xA1 //角度数据帧   (int16)((persent*Rang +500)*100)
#define PACK_KEY  0xA8 //按键数据帧
#define PACK_TEST 0xA2 //测试帧，检查连接情况，给飞控发送开始自检指令
#define PACK_ERROR 0xA3 //发送队列取出数据失败
#define PACK_END  0xA5 //结束帧


/*按键值*/
#define	Joystick_L_Value  0xB1
#define	Joystick_R_Value  0xB2
#define	KEY1_Value   0xB3
#define	KEY2_Value   0xB4
#define	KEY3_Value   0xB5



//test res
#define Test_Res_Initing 0xAA
#define Test_Res_Error   0xBB
#define Test_Res_OK      0xCC


//flag
//第一位
#define LINK_Status_Flag  0x01
//第二位
#define MOTOR_Status_Flag  0x02
//第三位
#define CanFly_Status_Flag  0x04

//第五位
#define CONTROL_TASK_Flag   0x10


//    外环
#define PID_PIT_Angle_P 						0.58
#define PID_PIT_Angle_I 						0
#define PID_PIT_Angle_D 						0
#define PID_PIT_Angle_I_Out_MAX		 	10
#define PID_PIT_Angle_I_Out_MIN			-10
#define PID_PIT_Angle_PID_Out_MAX			50
#define PID_PIT_Angle_PID_Out_MIN			-50
#define PID_PIT_Angle_ErrorRang				5

//
#define PID_ROL_Angle_P 						0.38
#define PID_ROL_Angle_I 						0
#define PID_ROL_Angle_D 						0
#define PID_ROL_Angle_I_Out_MAX		 		10
#define PID_ROL_Angle_I_Out_MIN			 -10
#define PID_ROL_Angle_PID_Out_MAX			50
#define PID_ROL_Angle_PID_Out_MIN			-50
#define PID_ROL_Angle_ErrorRang				5




//内环 主调节 

#define PID_PIT_Rate_P 						0.388
#define PID_PIT_Rate_I 						0.003
#define PID_PIT_Rate_D 						0.001
#define PID_PIT_Rate_I_Out_MAX		 	 20
#define PID_PIT_Rate_I_Out_MIN			-20
#define PID_PIT_Rate_PID_Out_MAX			 80
#define PID_PIT_Rate_PID_Out_MIN			-80
#define PID_PIT_Rate_ErrorRang				 20

//
#define PID_ROL_Rate_P 						0.388
#define PID_ROL_Rate_I 						0.003
#define PID_ROL_Rate_D 						0.001
#define PID_ROL_Rate_I_Out_MAX		 	 20
#define PID_ROL_Rate_I_Out_MIN			-20
#define PID_ROL_Rate_PID_Out_MAX			 80
#define PID_ROL_Rate_PID_Out_MIN			-80
#define PID_ROL_Rate_ErrorRang				20


//YAW 参数
#define PID_YAW_Rate_P 						0.71
#define PID_YAW_Rate_I 						0.002
#define PID_YAW_Rate_D 						0.001

#define PID_YAW_Rate_I_Out_MAX		  	20
#define PID_YAW_Rate_I_Out_MIN			 -20
#define PID_YAW_Rate_PID_Out_MAX			 50
#define PID_YAW_Rate_PID_Out_MIN			-50
#define PID_YAW_Rate_ErrorRang				5



//三轴浮点型
typedef struct
{
	float X;
	float Y;
	float Z;
}FLOAT_XYZ;

// 浮点型姿态角数据
typedef struct
{
	float rol;
	float pit;
	float yaw;
}FLOAT_POSTURE;

typedef struct
{
	uint16_t THRUST;
	float ROLL;
	float PITCH;
	float YAW;
}REMOTE_DATA_TYPEDEF;




#endif

