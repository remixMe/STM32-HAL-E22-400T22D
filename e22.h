#ifndef __E22_H
#define __E22_H
#include "main.h"
#include "usart.h"

#define E22_UART_BUFFER_MAX_LENGTH (240)
#define E22_UART_BUFFER_DEPTH      (20)

#define E22_UART_HANDLE            huart2

typedef enum {
    E22_Mode_Transmission = 0x00, //!< E22����ģʽ
    E22_Mode_WOR          = 0x01, //!< E22WORģʽ
    E22_Mode_Config       = 0x02, //!< E22����ģʽ
    E22_Mode_Sleep        = 0x03  //!< E22˯��ģʽ
} E22Mode_TypeDef;
// REG0
typedef enum {
    E22_UARTSpeed_1200   = 0x00,
    E22_UARTSpeed_2400   = 0x20,
    E22_UARTSpeed_4800   = 0x40,
    E22_UARTSpeed_9600   = 0x60,
    E22_UARTSpeed_19200  = 0x80,
    E22_UARTSpeed_38400  = 0xA0,
    E22_UARTSpeed_57600  = 0xC0,
    E22_UARTSpeed_115200 = 0xE0,
} E22UARTSpeed_TypeDef;

typedef enum {
    E22_UARTParityBit_8N1 = 0x00,
    E22_UARTParityBit_8O1 = 0x08,
    E22_UARTParityBit_8E1 = 0x10,
} E22UARTParityBit_TypeDef;

typedef enum {
    E22_AirRate_300   = 0x00,
    E22_AirRate_1200  = 0x01,
    E22_AirRate_2400  = 0x02,
    E22_AirRate_4800  = 0x03,
    E22_AirRate_9600  = 0x04,
    E22_AirRate_19200 = 0x05,
    E22_AirRate_38400 = 0x06,
    E22_AirRate_62500 = 0x07,
} E22AirRate_TypeDef;
// REG1
typedef enum {
    E22_SubPacket_240B = 0x00,
    E22_SubPacket_128B = 0x40,
    E22_SubPacket_64B  = 0x80,
    E22_SubPacket_32B  = 0xC0,
} E22SubPacket_TypeDef;

typedef enum {
    E22_RSSI_Disable = 0x00,
    E22_RSSI_Enable  = 0x20,
} E22RSSI_TypeDef;

typedef enum {
    E22_TransmissionPower_22dBm = 0x00,
    E22_TransmissionPower_17dBm = 0x01,
    E22_TransmissionPower_13dBm = 0x02,
    E22_TransmissionPower_10dBm = 0x03,
} E22TransmissionPower_TypeDef;
// REG3
typedef enum {
    E22_RSSIByte_Disable = 0x00,
    E22_RSSIByte_Enable  = 0x80,
} E22RSSIByte_TypeDef;

typedef enum {
    E22_TransmissionMode_Transparent = 0x00,
    E22_TransmissionMode_FixedPoint  = 0x40,
} E22TransmissionMode_TypeDef;

typedef enum {
    E22_Repeater_Disable = 0x00,
    E22_Repeater_Enable  = 0x20,
} E22Repeater_TypeDef;

typedef enum {
    E22_LBT_Disable = 0x00,
    E22_LBT_Enable  = 0x10,
} E22LBT_TypeDef;

typedef enum {
    E22_WOR_Receiver    = 0x00,
    E22_WOR_Transmitter = 0x08,
} E22WORControl_TypeDef;

typedef enum {
    E22_WORPeriod_500ms  = 0x00,
    E22_WORPeriod_1000ms = 0x01,
    E22_WORPeriod_1500ms = 0x02,
    E22_WORPeriod_2000ms = 0x03,
    E22_WORPeriod_2500ms = 0x04,
    E22_WORPeriod_3000ms = 0x05,
    E22_WORPeriod_3500ms = 0x06,
    E22_WORPeriod_4000ms = 0x07,
} E22WORPeriod_TypeDef;

typedef struct {
    // 00H~01H
    uint16_t                     Address;
    // 02H
    uint8_t                      NetID;
    // 03H
    E22UARTSpeed_TypeDef         UARTSpeed;
    E22UARTParityBit_TypeDef     ParityBit;
    E22AirRate_TypeDef           AirRate;
    // 04H
    E22SubPacket_TypeDef         SubPacket;
    E22RSSI_TypeDef              RSSI;
    E22TransmissionPower_TypeDef TransmissionPower;
    // 05H
    uint8_t                      Channel;
    // 06H
    E22RSSIByte_TypeDef          RSSIByte;
    E22TransmissionMode_TypeDef  TransmissionMode;
    E22Repeater_TypeDef          Repeater;
    E22LBT_TypeDef               LBT;
    E22WORControl_TypeDef        WORControl;
    E22WORPeriod_TypeDef         WORPeriod;
    // 07H~08H
    uint16_t                     CRYPT;
} E22_ConfigTypeDef;

void E22_Buffer_Reset(void);
void E22_Read_PID(void);
void E22_Read_Register(void);
void E22_FullSet_Register(E22_ConfigTypeDef *E22_ConfigStruct);
void E22_Mode(E22Mode_TypeDef mode);

/**
 * @brief ��E22���ջ��������ȡ��һ����
 *
 * @param buffer ���������
 * @retval uint8_t ���������
 */
uint8_t E22_UART_Packet_Pop(uint8_t *buffer);
/**
 * @brief ���E22���ջ���������Ƿ���δȡ���İ�
 *
 * @retval uint8_t T/F
 */
uint8_t E22_UART_Buffer_isEmpty(void);
/**
 * @brief ����E22
 *
 */
void    E22_Set(void);
/**
 * @brief ����E22���ڽ����ж�
 *
 */
void    E22_StartReceive(void);
/**
 * @brief
 *
 * @param address  Ŀ���ַ
 * @param channel  Ŀ���ŵ�
 * @param data     �����׵�ַ
 * @param length   ���ݰ�����
 */
void    E22_Send(uint16_t address, uint8_t channel, uint8_t *data, uint16_t length);
#endif // !__E22_H
