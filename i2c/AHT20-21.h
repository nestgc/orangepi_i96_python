#include <wiringPi.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <lcd.h>

#ifndef _AHT20_DEMO_
#define _AHT20_DEMO_
#endif


//#include "stm32f10x.h"

void Delay_N10us(uint32_t t);//función de retardo
void SensorDelay_us(uint32_t t);//función de retardo
void Delay_4us(void); //función de retardo
void Delay_5us(void); //función de retardo
void Delay_1ms(uint32_t t);
void AHT20_Clock_Init(void); //función de retardo
void SDA_Pin_Output_High(void); //Configure PB15 como una salida y ajústelo a un nivel alto, PB15 se usa como SDA de I2C
void SDA_Pin_Output_Low(void); //Configurar P15 como salida y ponerlo en nivel bajo
void SDA_Pin_IN_FLOATING(void); //SDA está configurado como entrada flotante
void SCL_Pin_Output_High(void); //SCL salida de alto nivel, P14 como I2C SCL
void SCL_Pin_Output_Low(void); //Nivel bajo de salida SCL
void Init_I2C_Sensor_Port(void); //Inicialice la interfaz I2C, la salida es alta
void I2C_Start(void); //El anfitrión I2C envía la señal de INICIO
void AHT20_WR_Byte(uint8_t Byte); //escribe un byte en AHT20
uint8_t AHT20_RD_Byte(void);//Lee un byte de AHT20
uint8_t Receive_ACK(void); //Comprobar si AHT20 ha respondido ACK
void Send_ACK (void); // Señal de ACK de respuesta del host
void Send_NOT_ACK(void); //El host no responde ACK
void Stop_I2C(void); //Finaliza un protocolo
uint8_t AHT20_Read_Status(void);//Lee el registro de estado de AHT20
uint8_t AHT20_Read_Cal_Enable(void); //Compruebe si el bit de activación de calibración está habilitado
void AHT20_SendAC(void); //Enviar comando AC a AHT20
uint8_t Calc_CRC8(uint8_t *messaje, uint8_t Num);
void AHT20_Read_CTdata(uint32_t *ct); //Sin verificación de CRC, lea directamente los datos de temperatura y humedad de AHT20
void AHT20_Read_CTdata_crc(uint32_t *ct); //Después de la verificación de CRC, lea los datos de temperatura y humedad de AHT20
void AHT20_Init(void); //Inicializar AHT20
void JH_Reset_REG(uint8_t addr);///reiniciar registro
void AHT20_Start_Init(void);///inicialización de encendido para ingresar al estado de medición normal