#include "24l01.h"

//改进：中断函数编写一下哦

extern SPI_HandleTypeDef hspi1;
extern osSemaphoreId_t NrfIT_BinaryHandle;

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0x15,0x35,0x16,0x63,0x96}; //发送地址
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x15,0x35,0x16,0x63,0x96}; //发送地址


//检测24L01是否存在
//返回值:0，成功;1，失败	
uint8_t NRF24L01_Check(void)
{
	
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;    	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //读出写入的地址  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//检测24L01错误	
	return 0;		 //检测到24L01
}	 	 

//返回0时初始化完毕
uint8_t NRF24L01_Init(void)
{
	__HAL_SPI_ENABLE(&hspi1);
	NRF24_CE_LOW();
	NRF24_CSN_HIGH();
	while(NRF24L01_Check())
	{
		HAL_Delay(100);
		//printf("NRF24初始化失败\r\n");
	}
	return 0;
}


//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
uint8_t NRF24L01_Write_Reg(uint8_t reg,uint8_t value)
{
	  uint8_t status;	
   	NRF24_CSN_LOW();                  //使能SPI传输
  	status = HAL_SPI_Transmit(&hspi1,&reg,1,100);  //发送寄存器号 
		HAL_SPI_Transmit(&hspi1,&value,1,100);    	//写入寄存器的值
  	NRF24_CSN_HIGH();                 //禁止SPI传输	   
  	return(status);       		        //返回状态值
}

//读取SPI寄存器值
//reg:要读的寄存器
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
	  uint8_t reg_val;	
		uint8_t	nop=0xff;
   	NRF24_CSN_LOW();                  //使能SPI传输		
  	HAL_SPI_Transmit(&hspi1,&reg,1,100);         //发送寄存器号
	  HAL_SPI_TransmitReceive(&hspi1,&nop,&reg_val,1, 100);//读取寄存器内容
  	NRF24_CSN_HIGH();                 //禁止SPI传输		    
  	return(reg_val);                  //返回状态值
}	
//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值 
uint8_t NRF24L01_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
{
		uint8_t status,nop;	
		nop = NOP;
  	NRF24_CSN_LOW();                  //使能SPI传输
  	status=HAL_SPI_Transmit(&hspi1,&reg,1,100);//发送寄存器值(位置),并读取状态值   	   	
		HAL_SPI_TransmitReceive(&hspi1,&nop,pBuf,len,1000);	
  	NRF24_CSN_HIGH();                 //关闭SPI传输
  	return status;                    //返回读到的状态值
}

//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status;	    
	NRF24_CSN_LOW();                    //使能SPI传输
  status = HAL_SPI_Transmit(&hspi1,&reg,1,100); //发送寄存器值(位置),并读取状态值
	HAL_SPI_Transmit(&hspi1,pBuf,len,1000);  //写入数据	
  NRF24_CSN_HIGH();                 //关闭SPI传输
  return status;                    //返回读到的状态值
}			

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
	uint8_t sta;   
	 NRF24_CE_LOW();
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	NRF24_CE_HIGH();                         //启动发送	   
	osSemaphoreAcquire(NrfIT_BinaryHandle, 1000);                //等待发送完成
	sta=NRF24L01_Read_Reg(STATUS);          //读取状态寄存器的值	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)                          //达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);  //清除TX FIFO寄存器 
		return 1; 
	}
	if(sta&TX_OK)                           //发送完成
	{
		return 0;
	}
	return 2;//其他原因发送失败
}
//启动NRF24L01接收一次数据
//rxbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
	uint8_t sta;		    							   
	osSemaphoreAcquire(NrfIT_BinaryHandle, 1000);
	sta=NRF24L01_Read_Reg(STATUS);          //读取状态寄存器的值    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(FLUSH_RX,0xff);  //清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//没收到任何数据
}			

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void NRF24L01_RX_Mode(void)
{
	 NRF24_CE_LOW();	  
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
	  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);       //使能通道0的自动应答    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);   //使能通道0的接收地址  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	        //设置RF通信频率		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);    //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);     //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  	NRF24_CE_HIGH(); //CE为高,进入接收模式 
}		

//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void NRF24L01_TX_Mode(void)
{														 
	 NRF24_CE_LOW();	    
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    
	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  
	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //设置RF通道为40
	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	NRF24_CE_HIGH();//CE为高,10us后启动发送
}


