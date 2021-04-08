#include "xc9104.h"

void MyOperateCS(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_XC9104_CS_PORT, MY_XC9104_CS_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_XC9104_CS_PORT, MY_XC9104_CS_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void MyOperateINC(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_XC9104_INC_PORT, MY_XC9104_INC_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_XC9104_INC_PORT, MY_XC9104_INC_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void MyOperateUD(MY_PinState PinState) {
    switch (PinState) {
        case MY_PIN_SET:
            HAL_GPIO_WritePin(MY_XC9104_UD_PORT, MY_XC9104_UD_PIN, GPIO_PIN_SET);
            break;
        case MY_PIN_RESET:
            HAL_GPIO_WritePin(MY_XC9104_UD_PORT, MY_XC9104_UD_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void MyXC9104Init() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin = MY_XC9104_CS_PIN;
    HAL_GPIO_Init(MY_XC9104_CS_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MY_XC9104_UD_PIN;
    HAL_GPIO_Init(MY_XC9104_UD_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = MY_XC9104_INC_PIN;
    HAL_GPIO_Init(MY_XC9104_INC_PORT, &GPIO_InitStruct);

    MyXC9104Clear();
}

void MyXC9104Inc() {
    MyOperateUD(MY_PIN_SET);
    MyDelayUs(1);
    MyOperateCS(MY_PIN_RESET);
    MyDelayUs(1);
    MyOperateINC(MY_PIN_RESET);
    MyDelayUs(10);
    MyOperateINC(MY_PIN_SET);
    MyDelayUs(10);
    MyOperateCS(MY_PIN_SET);
}

void MyXC9104Dec() {
    MyOperateUD(MY_PIN_RESET);
    MyDelayUs(1);
    MyOperateCS(MY_PIN_RESET);
    MyDelayUs(1);
    MyOperateINC(MY_PIN_RESET);
    MyDelayUs(10);
    MyOperateINC(MY_PIN_SET);
    MyDelayUs(10);
    MyOperateCS(MY_PIN_SET);
}

void MyXC9104Clear() {
    for (int i = 0; i < 120; ++i) {
        MyXC9104Dec();
    }
}

void MyXC9104IncValue(int Value) {
    for (int i = 0; i < Value; i++) {
        MyXC9104Inc();
    }
}

void MyXC9104DecValue(int Value) {
    for (int i = 0; i < Value; i++) {
        MyXC9104Dec();
    }
}

void MyXC9104ToValue(int Value) {
    MyXC9104Clear();
    MyXC9104DecValue(Value);
}