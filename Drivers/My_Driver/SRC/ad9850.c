#include "ad9850.h"

/*--------------------并行模式-----------------------*/
#ifdef MODE_PARALLEL
/********************************************
函数名称：AD9850_Reset_Parallel
功    能：在并行模式下复位AD9850
参    数：无
返回值  ：无
*********************************************/
void MyAD9850ResetParallel(void)
{
    MyOperateWCLK(MY_PIN_RESET);
    MyOperateFQUD(MY_PIN_RESET);
    //RST信号
    MyOperateRST(MY_PIN_RESET);
    MyOperateRST(MY_PIN_SET);
    MyDelayMs(2);
    MyOperateRST(MY_PIN_RESET);
}

/********************************************
函数名称：AD9850_Write_Parallel
功    能：在并行模式下写AD9850寄存器
参    数：W0 - W0寄存器的值
          freq - 频率值
返回值  ：无
*********************************************/
void MyAD9850WriteParallel(unsigned char W0,unsigned long freq)
{
    unsigned char wdata ;
    unsigned long  y=(unsigned long)(4294967295/MY_AD9850_SYSTEM_COLCK) ;
    y *= freq ;
    wdata=W0 ;  //写w0
    MyOperateDataParallel(wdata); //MY_AD9850_DATA_PORT
    MyOperateWCLK(MY_PIN_SET);
    MyOperateWCLK(MY_PIN_RESET);

    wdata=y>>24 ;  //写w1
    MyOperateDataParallel(wdata);
    MyOperateWCLK(MY_PIN_SET);
    MyOperateWCLK(MY_PIN_RESET);

    wdata=y>>16 ;  //写w2
    MyOperateDataParallel(wdata);
    MyOperateWCLK(MY_PIN_SET);
    MyOperateWCLK(MY_PIN_RESET);

    wdata=y>>8 ;  //写w3
    MyOperateDataParallel(wdata);
    MyOperateWCLK(MY_PIN_SET);
    MyOperateWCLK(MY_PIN_RESET);

    wdata=y>>0 ;  //写w4
    MyOperateDataParallel(wdata);
    MyOperateWCLK(MY_PIN_SET);
    MyOperateWCLK(MY_PIN_RESET);

    MyOperateFQUD(MY_PIN_SET);  //移入使能
    MyDelayMs(2);
    MyOperateFQUD(MY_PIN_RESET);
}
#endif /* MODE_PARALLEL */
/*------------------------并行模式-------------------------*/
/*---------------------------------------------------------*/

/*------------------------串行模式-------------------------*/
#ifdef MODE_SEIAL
/********************************************
函数名称：AD9850_Reset_Sreial
功    能：在串行模式下复位AD9850
参    数：无
返回值  ：无
*********************************************/
void MyAD9850ResetSreial(void)
{
    MyOperateWCLK(MY_PIN_RESET);
    MyOperateFQUD(MY_PIN_RESET);

    //RST信号
    MyOperateRST(MY_PIN_RESET);
    MyOperateRST(MY_PIN_SET);
    MyDelayMs(2);
    MyOperateRST(MY_PIN_RESET);

    //WCLK信号
    MyOperateWCLK(MY_PIN_RESET);
    MyOperateWCLK(MY_PIN_SET);
    MyDelayMs(2);
    MyOperateWCLK(MY_PIN_RESET);

    //FQUD信号
    MyOperateFQUD(MY_PIN_RESET);
    MyOperateFQUD(MY_PIN_SET);

    MyDelayMs(2);
    MyOperateFQUD(MY_PIN_RESET);
}

/********************************************
函数名称：AD9850_Write_Serial
功    能：在串行模式下写AD9850寄存器
参    数：W0 - W0寄存器的值
          freq - 频率值
返回值  ：无
*********************************************/
void MyAD9850WriteSerial(unsigned char W0,unsigned long freq)
{
    unsigned char i,wdata ;
    unsigned long  y ;
    //y = (unsigned long )268435456.0/MY_AD9850_SYSTEM_COLCK*freq ;
    //y = (unsigned long )268435456.0/MY_AD9850_SYSTEM_COLCK*freq ;
    y = 34.35*freq;
    //y = (unsigned long )4617582385.32/MY_AD9850_SYSTEM_COLCK*freq ;

    wdata = y>>0 ;   //写w4
    for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
            MyOperateDataSerial(MY_PIN_SET);
        else
            MyOperateDataSerial(MY_PIN_RESET);
        MyOperateWCLK(MY_PIN_SET);
        wdata >>= 1 ;
        MyOperateWCLK(MY_PIN_RESET);
    }
    wdata = y>>8 ;  //写w3
    for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
            MyOperateDataSerial(MY_PIN_SET);
        else
            MyOperateDataSerial(MY_PIN_RESET);
        MyOperateWCLK(MY_PIN_SET);
        wdata >>= 1 ;
        MyOperateWCLK(MY_PIN_RESET);
    }
    wdata = y>>16 ;  //写w2
    for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
            MyOperateDataSerial(MY_PIN_SET);
        else
            MyOperateDataSerial(MY_PIN_RESET);
        MyOperateWCLK(MY_PIN_SET);
        wdata >>= 1 ;
        MyOperateWCLK(MY_PIN_RESET);
    }
    wdata = y>>24 ;  //写w1
    for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
            MyOperateDataSerial(MY_PIN_SET);
        else
            MyOperateDataSerial(MY_PIN_RESET);
        MyOperateWCLK(MY_PIN_SET);
        wdata >>= 1 ;
        MyOperateWCLK(MY_PIN_RESET);
    }
    wdata = W0 ;  //写w0
    for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
            MyOperateDataSerial(MY_PIN_SET);
        else
            MyOperateDataSerial(MY_PIN_RESET);
        MyOperateWCLK(MY_PIN_SET);
        wdata >>= 1 ;
        MyOperateWCLK(MY_PIN_RESET);
    }

    MyOperateFQUD(MY_PIN_SET);  //移入使能
    MyDelayMs(2);
    MyOperateFQUD(MY_PIN_RESET);
}
#endif /* MODE_SERIAL */
/*------------------------串行模式-------------------------*/
/*---------------------------------------------------------*/

void MyOperateWCLK(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9850_CONTROL_PORT, MY_AD9850_WCLK_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9850_CONTROL_PORT, MY_AD9850_WCLK_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void MyOperateFQUD(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9850_CONTROL_PORT, MY_AD9850_FQUD_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9850_CONTROL_PORT, MY_AD9850_FQUD_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void MyOperateRST(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9850_CONTROL_PORT, MY_AD9850_RST_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9850_CONTROL_PORT, MY_AD9850_RST_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

#ifdef MODE_PARALLEL
void MyOperateDataParallel(unsigned char W0) {
    MY_AD9850_DATA_PORT_PARALLEL->ODR = W0;
}
#endif

#ifdef MODE_SEIAL
void MyOperateDataSerial(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_AD9850_DATA_PORT_SERIAL, MY_AD9850_DATA_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_AD9850_DATA_PORT_SERIAL, MY_AD9850_DATA_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}
#endif

char MyCalculatePhase(int phase) {
    char temp =  phase * 16 / 360;
    return temp << 4;
}


/********************************************
函数名称：Init_AD9834
功    能：初始化AD9834
参    数：无
返回值  ：无
*********************************************/
void Init_AD9850(void) {

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin = MY_AD9850_FQUD_PIN | MY_AD9850_WCLK_PIN | MY_AD9850_RST_PIN;
    HAL_GPIO_Init(MY_AD9850_CONTROL_PORT, &GPIO_InitStruct);
#ifdef MODE_SEIAL   //串行模式
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    HAL_GPIO_Init(MY_AD9850_DATA_PORT_SERIAL, &GPIO_InitStruct);
    MyAD9850ResetSreial();
#endif

#ifdef MODE_PARALLEL //并行模式
    GPIO_InitStruct.Pin = GPIO_PIN_All;
    HAL_GPIO_Init(MY_AD9850_DATA_PORT_PARALLEL, &GPIO_InitStruct);
    MyAD9850ResetParallel();
#endif
}