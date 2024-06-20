
#include "pid.h"



//测试 需要调节的参数
float test_rate_rol_P = PID_ROL_Rate_P;
float test_rate_pit_P = PID_PIT_Rate_P;



/*******************************
	@funcation  void PID_Arg_Init(PID* sptr)

	@brief   参数初始化

	@para     PID* sptr   
					 
	@retval : none
	
 */

void PID_Arg_Init(PID* pitch_angle,PID* roll_angle,
									PID* pitch_rate ,PID* roll_rate ,PID* yaw_angle)
{
	//pitch angle
	pitch_angle->LastError = 0;        //上次偏差值
	pitch_angle->Errorsum = 0;        //上上次偏差值                            
	pitch_angle->P = PID_PIT_Angle_P;                //比例常数
	pitch_angle->I = PID_PIT_Angle_I;                //积分常数
	pitch_angle->D = PID_PIT_Angle_D;                //微分常数	  
	pitch_angle->PID_Out = 0;
	pitch_angle->I_Out_MAX = PID_PIT_Angle_I_Out_MAX;
	pitch_angle->I_Out_MIN = PID_PIT_Angle_I_Out_MIN;
	pitch_angle->PID_Out_MAX = PID_PIT_Angle_PID_Out_MAX;
	pitch_angle->PID_Out_MIN = PID_PIT_Angle_PID_Out_MIN;
	pitch_angle->ErrorRang = PID_PIT_Angle_ErrorRang;
	
	//roll angle
	roll_angle->LastError = 0;        //上次偏差值
	roll_angle->Errorsum = 0;        //上上次偏差值                            
	roll_angle->P = PID_ROL_Angle_P;                //比例常数
	roll_angle->I = PID_ROL_Angle_I;                //积分常数
	roll_angle->D = PID_ROL_Angle_D;                //微分常数	  	
	roll_angle->PID_Out = 0;
	roll_angle->I_Out_MAX = PID_ROL_Angle_I_Out_MAX;
	roll_angle->I_Out_MIN = PID_ROL_Angle_I_Out_MIN;
	roll_angle->PID_Out_MAX = PID_ROL_Angle_PID_Out_MAX;
	roll_angle->PID_Out_MIN = PID_ROL_Angle_PID_Out_MIN;
	roll_angle->ErrorRang = PID_ROL_Angle_ErrorRang;
	
	//
	pitch_rate->LastError = 0;        //上次偏差值
	pitch_rate->Errorsum = 0;        //上上次偏差值                            
	pitch_rate->P = test_rate_pit_P;                //比例常数
	pitch_rate->I = PID_PIT_Rate_I;                //积分常数
	pitch_rate->D = PID_PIT_Rate_D;                //微分常数	  
	pitch_rate->PID_Out = 0;
	pitch_rate->I_Out_MAX = PID_PIT_Rate_I_Out_MAX;
	pitch_rate->I_Out_MIN = PID_PIT_Rate_I_Out_MIN;
	pitch_rate->PID_Out_MAX = PID_PIT_Rate_PID_Out_MAX;
	pitch_rate->PID_Out_MIN = PID_PIT_Rate_PID_Out_MIN;
	pitch_rate->ErrorRang = PID_PIT_Rate_ErrorRang;

	roll_rate->LastError = 0;        //上次偏差值
	roll_rate->Errorsum = 0;        //上上次偏差值                            
	roll_rate->P = test_rate_rol_P;                //比例常数
	roll_rate->I = PID_ROL_Rate_I;                //积分常数
	roll_rate->D = PID_ROL_Rate_D;                //微分常数	  	
	roll_rate->PID_Out = 0;
	roll_rate->I_Out_MAX = PID_ROL_Rate_I_Out_MAX;
	roll_rate->I_Out_MIN = PID_ROL_Rate_I_Out_MIN;
	roll_rate->PID_Out_MAX = PID_ROL_Rate_PID_Out_MAX;
	roll_rate->PID_Out_MIN = PID_ROL_Rate_PID_Out_MIN;
	roll_rate->ErrorRang = PID_ROL_Rate_ErrorRang;
	

	yaw_angle->LastError = 0;        //上次偏差值
	yaw_angle->Errorsum = 0;        //上上次偏差值                            
	yaw_angle->P = PID_YAW_Rate_P;                //比例常数
	yaw_angle->I = PID_YAW_Rate_I;                //积分常数
	yaw_angle->D = PID_YAW_Rate_D;                //微分常数	  
	yaw_angle->PID_Out = 0;
	yaw_angle->I_Out_MAX = PID_YAW_Rate_I_Out_MAX;
	yaw_angle->I_Out_MIN = PID_YAW_Rate_I_Out_MIN;
	yaw_angle->PID_Out_MAX = PID_YAW_Rate_PID_Out_MAX;
	yaw_angle->PID_Out_MIN = PID_YAW_Rate_PID_Out_MIN;
	yaw_angle->ErrorRang = PID_YAW_Rate_ErrorRang;
	
	
	
}




void PID_Process(PID* sptr,float Target,float Measure)
{
	float iError,P_Out_Part,I_Out_Part,D_Out_Part;
	
	iError = Target - Measure;	//当前误差  设定的目标值和实际值的偏差						
	P_Out_Part  =  sptr->P * iError;
	I_Out_Part  =  sptr->I * sptr->Errorsum;
	D_Out_Part  =  sptr->D * (iError - sptr->LastError);
			
		//积分分离、限幅  进入稳态温差时加入积分
	if( (iError > sptr->ErrorRang)   ||  (iError < -sptr->ErrorRang) )
		{
			sptr->PID_Out = P_Out_Part+D_Out_Part;
		}
		else 
		{
			if(I_Out_Part < sptr->I_Out_MIN)
				I_Out_Part = sptr->I_Out_MIN;
				else if(I_Out_Part > sptr->I_Out_MAX)
				I_Out_Part = sptr->I_Out_MAX;
				
			sptr->PID_Out = P_Out_Part+I_Out_Part+D_Out_Part;
		}

	 //限幅
	if( (sptr->PID_Out >= sptr->PID_Out_MAX))   
	{
		sptr->PID_Out = sptr->PID_Out_MAX;
	}		
	else if( (sptr->PID_Out < sptr->PID_Out_MIN))
		{
			sptr->PID_Out = sptr->PID_Out_MIN;
		}
	
		
	sptr->Errorsum += iError;
		
	if((sptr->Errorsum > Errorsum_MAX)||(sptr->Errorsum < -Errorsum_MAX))
	{
		sptr->Errorsum = 0;
	}
	
	sptr->LastError = iError;

}
