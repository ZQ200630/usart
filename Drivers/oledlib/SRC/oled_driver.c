#include "oled_driver.h"

extern I2C_HandleTypeDef hi2c1;

#if (TRANSFER_METHOD ==HW_IIC)
//I2C_Configuration����ʼ��Ӳ��IIC����
void I2C_Configuration(void)
{

}

 /**
  * @brief  I2C_WriteByte����OLED�Ĵ�����ַдһ��byte������
  * @param  addr���Ĵ�����ַ
	*					data��Ҫд�������
  * @retval ��
  */
void I2C_WriteByte(char addr,char data)
{
    char dat[1];
    dat[0] = data;
    HAL_I2C_Mem_Write(&hi2c1, 0x78, addr, I2C_MEMADD_SIZE_8BIT, dat, 1, 100);
}

void WriteCmd(unsigned char cmd)//д����
{
		I2C_WriteByte(0x00, cmd);
}

void WriteDat(unsigned char dat)//д����
{
		I2C_WriteByte(0x40, dat);
}

void OLED_FILL(unsigned char BMP[])
{
    int i,n;
    for(i=0;i<8;i++)
    {
        WriteCmd(0xb0+i);    //����ҳ��ַ��0~7��
        WriteCmd(0x00);      //������ʾλ�á��е͵�ַ
        WriteCmd(0x10);      //������ʾλ�á��иߵ�ַ
        for(n=0;n<128;n++)WriteDat(BMP[128*i+n]);
    }
}



#elif  (TRANSFER_METHOD ==SW_IIC)

#elif  (TRANSFER_METHOD ==HW_SPI)

	#define OLED_RESET_LOW()					GPIO_ResetBits(SPI_RES_GPIOX, SPI_RES_PIN)	//�͵�ƽ��λ
	#define OLED_RESET_HIGH()					GPIO_SetBits(SPI_RES_GPIOX, SPI_RES_PIN)
	 
	#define OLED_CMD_MODE()						GPIO_ResetBits(SPI_DC_GPIOX, SPI_DC_PIN) 		//����ģʽ
	#define OLED_DATA_MODE()					GPIO_SetBits(SPI_DC_GPIOX, SPI_DC_PIN)			//����ģʽ

	#define OLED_CS_HIGH()   					GPIO_SetBits(SPI_CS_GPIOX, SPI_CS_Pin_X)
	#define OLED_CS_LOW()  		  			GPIO_ResetBits(SPI_CS_GPIOX, SPI_CS_Pin_X)
	
	
void SPI_Configuration(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	#if 	(USE_HW_SPI==SPI_2)
		RCC_APB1PeriphClockCmd(SPI_RCC_APB1Periph_SPIX, ENABLE);
	#elif (USE_HW_SPI==SPI_1)
		RCC_APB2PeriphClockCmd(SPI_RCC_APB2Periph_SPIX, ENABLE);
	#endif
	RCC_APB2PeriphClockCmd(SPI_RCC_APB2Periph_GPIOX|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SPI_CS_Pin_X;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_CS_GPIOX, &GPIO_InitStructure);  
	OLED_CS_HIGH();
	
	GPIO_InitStructure.GPIO_Pin = SPI_HW_ALL_PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_HW_ALL_GPIOX, &GPIO_InitStructure);   
	
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI_Direction_1Line_Tx; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 			
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 	
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		 	
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  			
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Init(SPIX, &SPI_InitStructure);  
	SPI_Cmd(SPIX, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SPI_RES_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_RES_GPIOX, &GPIO_InitStructure);  
	GPIO_InitStructure.GPIO_Pin = SPI_DC_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_DC_GPIOX, &GPIO_InitStructure);  
	OLED_DATA_MODE();
	
	OLED_RESET_HIGH();
	WaitTimeMs(100);
	OLED_RESET_LOW();
	WaitTimeMs(100);
	OLED_RESET_HIGH();
}

void SPI_WriterByte(unsigned char dat)
{

	while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_TXE) == RESET ){}; //���ָ����SPI��־λ�������:���ͻ���ձ�־λ	  
		SPI_I2S_SendData(SPIX, dat); //ͨ������SPIx����һ������
	while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_RXNE) == RESET){};//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ  						    
	  SPI_I2S_ReceiveData(SPIX); //����ͨ��SPIx������յ�����	

}

void WriteCmd(unsigned char cmd)
{
	OLED_CMD_MODE();
	OLED_CS_LOW();
	SPI_WriterByte(cmd);
	OLED_CS_HIGH();
	OLED_DATA_MODE();
}

void WriteDat(unsigned char dat)
{
	OLED_DATA_MODE();
	OLED_CS_LOW();
	SPI_WriterByte(dat);
	OLED_CS_HIGH();
	OLED_DATA_MODE();
}

void OLED_FILL(unsigned char BMP[])
{
	u8 i,j;
	unsigned char *p;
	p=BMP;

  for(i=0;i<8;i++)
	{
		WriteCmd(0xb0+i);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);	

		for(j=0;j<128;j++)
		{
			WriteDat(*p++);
		}
	}
}

#endif //(TRANSFER_METHOD ==HW_IIC)

void OLED_Init(void)
{
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
	OLED_CLS();
}


void OLED_CLS(void)//����
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
		{
				WriteDat(0x00);
		}
	}
}

void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}



void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ�
	WriteCmd(0XAE);  //OLED����
}



