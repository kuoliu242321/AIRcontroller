#include "dataprocess.h"

REMOTE_DATA_TYPEDEF Remote_Date;



void Remote_Data_Process(void)
{
	//�ӽ��ܶ�����ȡ�����ݿ�ʼ���� 0��ʾȡ���ɹ�
	uint8_t RX_DATA_Buffer[12];
	
	int16_t receive_thrust;
	
	if(Link2Rem_ReceivePacket(RX_DATA_Buffer) == osOK)  //����������⣿
	{
		//printf("ȡ�����ݳɹ�\r\n");
		if(RX_DATA_Buffer[11] == PACK_END)
		{
			if(RX_DATA_Buffer[0] == PACK_DATE)
			{
				
				//������ֱ��ʹ�ã������Ż���Ϊ1500����ʱ���ת�ٽϿ죬��Ҫʱ����Ҫң�ص�����ֵ
			//	Remote_Date.THRUST = (uint16_t)((RX_DATA_Buffer[3]<<8)|RX_DATA_Buffer[2]);
				
				//���ż��书�ܣ�ÿ�δ������ŵ���������ң�л��У�����ֵ���ڸı�
				//���Ŵ���1500 ��������   ����С��1500 ���ż���
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
			//�������������Ӳ��ִ������ڴ˺�����
			else if(RX_DATA_Buffer[0] == PACK_ERROR)
			{
				//�����˴� 
				//�������˸��ʾ����		
			}	
		}
		else
			return;
	}	
}

//�������ͣת
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

//���Ƶ�����ת
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



//�����ǡ�2000dpi 2000*ԭʼֵ/32768 Ϊ�����Ľ��ٶ� 
void Get_Float_Gyroscope (float *gx,float *gy,float *gz)
{
	short gyrox,gyroy,gyroz;		//������ԭʼ����

	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
	
	*gx=(float)((gyrox*2000)/32768);
	*gy=(float)((gyroy*2000)/32768);
	*gz=(float)((gyroz*2000)/32768);

}


	

























