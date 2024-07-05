#include "24l01.h"

//�Ľ����жϺ�����дһ��Ŷ

extern SPI_HandleTypeDef hspi1;
extern osSemaphoreId_t NrfIT_BinaryHandle;

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0x15,0x35,0x16,0x63,0x96}; //���͵�ַ
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x15,0x35,0x16,0x63,0x96}; //���͵�ַ


//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��	
uint8_t NRF24L01_Check(void)
{
	
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;    	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}	 	 

//����0ʱ��ʼ�����
uint8_t NRF24L01_Init(void)
{
	__HAL_SPI_ENABLE(&hspi1);
	NRF24_CE_LOW();
	NRF24_CSN_HIGH();
	while(NRF24L01_Check())
	{
		HAL_Delay(100);
		//printf("NRF24��ʼ��ʧ��\r\n");
	}
	return 0;
}


//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
uint8_t NRF24L01_Write_Reg(uint8_t reg,uint8_t value)
{
	  uint8_t status;	
   	NRF24_CSN_LOW();                  //ʹ��SPI����
  	status = HAL_SPI_Transmit(&hspi1,&reg,1,100);  //���ͼĴ����� 
		HAL_SPI_Transmit(&hspi1,&value,1,100);    	//д��Ĵ�����ֵ
  	NRF24_CSN_HIGH();                 //��ֹSPI����	   
  	return(status);       		        //����״ֵ̬
}

//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
	  uint8_t reg_val;	
		uint8_t	nop=0xff;
   	NRF24_CSN_LOW();                  //ʹ��SPI����		
  	HAL_SPI_Transmit(&hspi1,&reg,1,100);         //���ͼĴ�����
	  HAL_SPI_TransmitReceive(&hspi1,&nop,&reg_val,1, 100);//��ȡ�Ĵ�������
  	NRF24_CSN_HIGH();                 //��ֹSPI����		    
  	return(reg_val);                  //����״ֵ̬
}	
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
uint8_t NRF24L01_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
{
		uint8_t status,nop;	
		nop = NOP;
  	NRF24_CSN_LOW();                  //ʹ��SPI����
  	status=HAL_SPI_Transmit(&hspi1,&reg,1,100);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   	
		HAL_SPI_TransmitReceive(&hspi1,&nop,pBuf,len,1000);	
  	NRF24_CSN_HIGH();                 //�ر�SPI����
  	return status;                    //���ض�����״ֵ̬
}

//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status;	    
	NRF24_CSN_LOW();                    //ʹ��SPI����
  status = HAL_SPI_Transmit(&hspi1,&reg,1,100); //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	HAL_SPI_Transmit(&hspi1,pBuf,len,1000);  //д������	
  NRF24_CSN_HIGH();                 //�ر�SPI����
  return status;                    //���ض�����״ֵ̬
}			

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
	uint8_t sta;   
	 NRF24_CE_LOW();
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF24_CE_HIGH();                         //��������	   
	osSemaphoreAcquire(NrfIT_BinaryHandle, 1000);                //�ȴ��������
	sta=NRF24L01_Read_Reg(STATUS);          //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)                          //�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);  //���TX FIFO�Ĵ��� 
		return 1; 
	}
	if(sta&TX_OK)                           //�������
	{
		return 0;
	}
	return 2;//����ԭ����ʧ��
}
//����NRF24L01����һ������
//rxbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
	uint8_t sta;		    							   
	osSemaphoreAcquire(NrfIT_BinaryHandle, 1000);
	sta=NRF24L01_Read_Reg(STATUS);          //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);  //���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}			

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void NRF24L01_RX_Mode(void)
{
	 NRF24_CE_LOW();	  
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);       //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);   //ʹ��ͨ��0�Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	        //����RFͨ��Ƶ��		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);    //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);     //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  	NRF24_CE_HIGH(); //CEΪ��,�������ģʽ 
}		

//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void NRF24L01_TX_Mode(void)
{														 
	 NRF24_CE_LOW();	    
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF24_CE_HIGH();//CEΪ��,10us����������
}


