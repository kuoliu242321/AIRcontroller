#ifndef   _DATACONFIGURATION_H
#define   _DATACONFIGURATION_H

#include "main.h"




#define PACK_DATE 0xA1 //�Ƕ�����֡   (int16)((persent*Rang +500)*100)
#define PACK_KEY  0xA8 //��������֡
#define PACK_TEST 0xA2 //����֡�����������������ɿط��Ϳ�ʼ�Լ�ָ��
#define PACK_ERROR 0xA3 //���Ͷ���ȡ������ʧ��
#define PACK_END  0xA5 //����֡


/*����ֵ*/
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
//��һλ
#define LINK_Status_Flag  0x01
//�ڶ�λ
#define MOTOR_Status_Flag  0x02
//����λ
#define CanFly_Status_Flag  0x04

//����λ
#define CONTROL_TASK_Flag   0x10


//    �⻷
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




//�ڻ� ������ 

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


//YAW ����
#define PID_YAW_Rate_P 						0.71
#define PID_YAW_Rate_I 						0.002
#define PID_YAW_Rate_D 						0.001

#define PID_YAW_Rate_I_Out_MAX		  	20
#define PID_YAW_Rate_I_Out_MIN			 -20
#define PID_YAW_Rate_PID_Out_MAX			 50
#define PID_YAW_Rate_PID_Out_MIN			-50
#define PID_YAW_Rate_ErrorRang				5



//���ḡ����
typedef struct
{
	float X;
	float Y;
	float Z;
}FLOAT_XYZ;

// ��������̬������
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

