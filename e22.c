#include "e22.h"
#include <string.h>
#include <stdio.h>

__STATIC_INLINE void Set_M0_High(void) {
    HAL_GPIO_WritePin(E22_M0_GPIO_Port, E22_M0_Pin, GPIO_PIN_SET);
}
__STATIC_INLINE void Set_M0_Low(void) {
    HAL_GPIO_WritePin(E22_M0_GPIO_Port, E22_M0_Pin, GPIO_PIN_RESET);
}
__STATIC_INLINE void Set_M1_High(void) {
    HAL_GPIO_WritePin(E22_M1_GPIO_Port, E22_M1_Pin, GPIO_PIN_SET);
}
__STATIC_INLINE void Set_M1_Low(void) {
    HAL_GPIO_WritePin(E22_M1_GPIO_Port, E22_M1_Pin, GPIO_PIN_RESET);
}
static void Wait_For_E22(void) {
    HAL_Delay(15);
    while (HAL_GPIO_ReadPin(E22_AUX_GPIO_Port, E22_AUX_Pin) == GPIO_PIN_RESET) {}
    HAL_Delay(5); //模块手册建议
}

static e22_uart_buffer_struct_t e22_uart_buffer;

void E22_FullSet_Register(E22_ConfigTypeDef *E22_ConfigStruct) {
    E22_Mode(E22_Mode_Config);
    // 从0字节开始传输9字节数据
    e22_uart_buffer.send_buffer[0] = 0xC0;
    e22_uart_buffer.send_buffer[1] = 0x00;
    e22_uart_buffer.send_buffer[2] = 0x09;
    e22_uart_buffer.send_buffer[3] = (0xFF00 & (E22_ConfigStruct->Address)) >> 8;
    e22_uart_buffer.send_buffer[4] = (0x00FF & (E22_ConfigStruct->Address));
    e22_uart_buffer.send_buffer[5] = E22_ConfigStruct->NetID;
    e22_uart_buffer.send_buffer[6] =
        E22_ConfigStruct->UARTSpeed | E22_ConfigStruct->ParityBit | E22_ConfigStruct->AirRate;
    e22_uart_buffer.send_buffer[7] =
        E22_ConfigStruct->SubPacket | E22_ConfigStruct->RSSI | E22_ConfigStruct->TransmissionPower;
    e22_uart_buffer.send_buffer[8] = E22_ConfigStruct->Channel;
    e22_uart_buffer.send_buffer[9] = E22_ConfigStruct->RSSIByte | E22_ConfigStruct->TransmissionMode
                                     | E22_ConfigStruct->Repeater | E22_ConfigStruct->LBT
                                     | E22_ConfigStruct->WORControl | E22_ConfigStruct->WORPeriod;
    e22_uart_buffer.send_buffer[10] = (0xFF00 & (E22_ConfigStruct->CRYPT)) >> 8;
    e22_uart_buffer.send_buffer[11] = (0x00FF & (E22_ConfigStruct->CRYPT));
    Wait_For_E22();
    HAL_UART_Transmit_DMA(&E22_UART_HANDLE, e22_uart_buffer.send_buffer, 12);
}

void E22_Read_PID(void) {
    E22_Mode(E22_Mode_Config);
    e22_uart_buffer.send_buffer[0] = 0xC1;
    e22_uart_buffer.send_buffer[1] = 0x80;
    e22_uart_buffer.send_buffer[2] = 0x07;
    Wait_For_E22();
    HAL_UART_Transmit_DMA(&E22_UART_HANDLE, e22_uart_buffer.send_buffer, 3);
}
void E22_Read_Register(void) {
    E22_Mode(E22_Mode_Config);
    e22_uart_buffer.send_buffer[0] = 0xC1;
    e22_uart_buffer.send_buffer[1] = 0x00;
    e22_uart_buffer.send_buffer[2] = 0x09;
    Wait_For_E22();
    HAL_UART_Transmit_DMA(&E22_UART_HANDLE, e22_uart_buffer.send_buffer, 3);
}

void E22_Mode(E22Mode_TypeDef mode) {
    Wait_For_E22();

    switch (mode) {
    case E22_Mode_Transmission:
        Set_M0_Low();
        Set_M1_Low();
        break;
    case E22_Mode_WOR:
        Set_M0_High();
        Set_M1_Low();
        break;
    case E22_Mode_Config:
        Set_M0_Low();
        Set_M1_High();
        break;
    case E22_Mode_Sleep:
        Set_M0_High();
        Set_M1_High();
        break;
    default: break;
    }
    Wait_For_E22();
}

void E22_Init(void) {
    //缓存初始化
    e22_uart_buffer.count  = 0;
    e22_uart_buffer.inPos  = 0;
    e22_uart_buffer.outPos = 0;
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    //注册回调函数
    HAL_UART_RegisterRxEventCallback(&E22_UART_HANDLE, E22_PacketReceived_Callback);
#endif
}

uint8_t E22_UART_Packet_Pop(uint8_t *buffer) {
    e22_uart_packet_struct_t *packet = &e22_uart_buffer.receivedPackets[e22_uart_buffer.outPos];
    //无缓存时返回0
    if (e22_uart_buffer.count == 0) { return 0; }
    memcpy(buffer, packet->buffer, sizeof(uint8_t) * (packet->length));
    (e22_uart_buffer.outPos)++;
    if (e22_uart_buffer.outPos == E22_UART_BUFFER_DEPTH) { e22_uart_buffer.outPos = 0; }
    // printf("count=%d,length=%d\n", e22_uart_buffer.count, packet->length);
    (e22_uart_buffer.count)--;
    return packet->length;
}

uint8_t E22_UART_Buffer_isEmpty(void) {
    if (e22_uart_buffer.count == 0) {
        return 1;
    } else {
        return 0;
    }
}

void E22_Set(void) {
    E22_ConfigTypeDef E22_ConfigStruct;
    E22_ConfigStruct.Address           = E22_ADDRESS;
    E22_ConfigStruct.NetID             = E22_NET_ID;
    E22_ConfigStruct.UARTSpeed         = E22_UART_SPEED;
    E22_ConfigStruct.ParityBit         = E22_PARITY_BIT;
    E22_ConfigStruct.AirRate           = E22_AIR_RATE;
    E22_ConfigStruct.SubPacket         = E22_SUB_PACKET;
    E22_ConfigStruct.RSSI              = E22_RSSI;
    E22_ConfigStruct.TransmissionPower = E22_TRANSMISSION_POWER;
    E22_ConfigStruct.Channel           = E22_CHANNEL;
    E22_ConfigStruct.RSSIByte          = E22_RSSI_BYTE;
    E22_ConfigStruct.TransmissionMode  = E22_TRANSMISSION_MODE;
    E22_ConfigStruct.Repeater          = E22_REPEATER;
    E22_ConfigStruct.LBT               = E22_LBT;
    E22_ConfigStruct.WORControl        = E22_WOR_CONTROL;
    E22_ConfigStruct.WORPeriod         = E22_WOR_PERIOD;
    E22_ConfigStruct.CRYPT             = E22_CRYPT;
    E22_FullSet_Register(&E22_ConfigStruct);
}

void E22_StartReceive(void) {
    e22_uart_packet_struct_t *currentPacket =
        &e22_uart_buffer.receivedPackets[e22_uart_buffer.inPos];
    HAL_UARTEx_ReceiveToIdle_DMA(&E22_UART_HANDLE, currentPacket->buffer,
                                 E22_UART_BUFFER_MAX_LENGTH);
}

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
void E22_PacketReceived_Callback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == E22_UART_HANDLE.Instance) {
        e22_uart_packet_struct_t *currentPacket =
            &e22_uart_buffer.receivedPackets[e22_uart_buffer.inPos];
        currentPacket->length = Size;
        e22_uart_buffer.inPos++;
        if (e22_uart_buffer.inPos == E22_UART_BUFFER_DEPTH) { e22_uart_buffer.inPos = 0; }
        e22_uart_buffer.count++;
        E22_StartReceive();
    }
}
#else
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == E22_UART_HANDLE.Instance) {
        e22_uart_packet_struct_t *currentPacket =
            &e22_uart_buffer.receivedPackets[e22_uart_buffer.inPos];
        currentPacket->length = Size;
        e22_uart_buffer.inPos++;
        if (e22_uart_buffer.inPos == E22_UART_BUFFER_DEPTH) { e22_uart_buffer.inPos = 0; }
        e22_uart_buffer.count++;
        E22_StartReceive();
    }
}
#endif

void E22_Send(uint16_t address, uint8_t channel, e22_uart_packet_struct_t *packet2Send) {
    e22_uart_buffer.send_buffer[0] = (address >> 8) & 0xFF;
    e22_uart_buffer.send_buffer[1] = (address)&0xFF;
    e22_uart_buffer.send_buffer[2] = channel;
    uint16_t length                = packet2Send->length;
    memcpy(&e22_uart_buffer.send_buffer[3], packet2Send->buffer, length);
    Wait_For_E22();
    HAL_UART_Transmit_DMA(&E22_UART_HANDLE, e22_uart_buffer.send_buffer, length + 3);
}
