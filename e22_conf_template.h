#ifndef __E22_CONF_H
#define __E22_CONF_H
#define E22_UART_BUFFER_MAX_LENGTH (240)
#define E22_UART_BUFFER_DEPTH      (20)

#define E22_UART_HANDLE            huart2

//默认配置
#define E22_ADDRESS                ((uint16_t)0x1234)
#define E22_NET_ID                 ((uint8_t)0x07)
#define E22_UART_SPEED             E22_UARTSpeed_9600
#define E22_PARITY_BIT             E22_UARTParityBit_8N1
#define E22_AIR_RATE               E22_AirRate_2400
#define E22_SUB_PACKET             E22_SubPacket_240B
#define E22_RSSI                   E22_RSSI_Disable
#define E22_TRANSMISSION_POWER     E22_TransmissionPower_22dBm
#define E22_CHANNEL                ((uint8_t)13)
#define E22_RSSI_BYTE              E22_RSSIByte_Disable
#define E22_TRANSMISSION_MODE      E22_TransmissionMode_FixedPoint
#define E22_REPEATER               E22_Repeater_Disable
#define E22_LBT                    E22_LBT_Disable
#define E22_WOR_CONTROL            E22_WOR_Receiver
#define E22_WOR_PERIOD             E22_WORPeriod_1000ms
#define E22_CRYPT                  ((uint16_t)0x2222)

#endif // !__E22_CONF_H