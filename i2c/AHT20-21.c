//*******************************************/
/*@Derechos de autor: Guangzhou Aosong Electronics Co., Ltd. */
/*@Autor: División de Sensores de Temperatura y Humedad */
/* @versión: V1.2                            */
/*******************************************/
#include "stm32f10x.h" 
#include "AHT20-21.h" 




void Delay_N10us(uint32_t t)// función de retraso
{
  uint32_t k;

   while(t--)
  {
    for (k = 0; k < 2; k++);//110
  }
}

void SensorDelay_us(uint32_t t)// función de retraso
{
		
	for(t = t-2; t>0; t--)
	{
		Delay_N10us(1);
	}
}

void Delay_4us(void)		// función de retraso
{	
	Delay_N10us(1);
	Delay_N10us(1);
	Delay_N10us(1);
	Delay_N10us(1);
}
void Delay_5us(void)		// función de retraso
{	
	Delay_N10us(1);
	Delay_N10us(1);
	Delay_N10us(1);
	Delay_N10us(1);
	Delay_N10us(1);

}

void Delay_1ms(uint32_t t)		// función de retraso
{
   while(t--)
  {
    SensorDelay_us(1000);//////Retraso 1ms
  }
}


void AHT20_Clock_Init(void)		// función de retraso
{
	RCC_APB2PeriphClockCmd(CC_APB2Periph_GPIOB,ENABLE);
}

void SDA_Pin_Output_High(void) //Configure PB15 como salida y ajústelo a un nivel alto, PB15 se usa como SDA de I2C
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;// salida push-pull
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,& GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}

void SDA_Pin_Output_Low(void)  //Configurar P15 como salida y ponerlo en nivel bajo
{

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;// salida push-pull
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,& GPIO_InitStruct);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}

void SDA_Pin_IN_FLOATING(void)  //SDA está configurado como una entrada flotante
{

	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB,&GPIO_InitStruct);
}

void SCL_Pin_Output_High(void) //SCL salida de alto nivel, P14 como I2C SCL
{
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}

void SCL_Pin_Output_Low(void) //Salida SCL nivel bajo
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
}

void Init_I2C_Sensor_Port(void) //Inicialice la interfaz I2C, la salida es de alto nivel
{	
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;// salida push-pull
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,& GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);// salida de alto nivel
	
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;// salida push-pull
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,& GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);// salida de alto nivel
	
}
void I2C_Start(void)	// El host I2C envía la señal de INICIO
{
	SDA_Pin_Output_High();
	SensorDelay_us(8);
	SCL_Pin_Output_High();
	SensorDelay_us(8);
	SDA_Pin_Output_Low();
	SensorDelay_us(8);
	SCL_Pin_Output_Low();
	SensorDelay_us(8);   
}


void AHT20_WR_Byte(uint8_t Byte) //Escribir un byte en AHT20
{
	uint8_t Data,N,i;	
	Data=Byte;
	i = 0x80;
	for(N=0;N<8;N++)
	{
		SCL_Pin_Output_Low(); 
		Delay_4us();	
		if(i&Data)
		{
			SDA_Pin_Output_High();
		}
		else
		{
			SDA_Pin_Output_Low();
		}	
			
    SCL_Pin_Output_High();
		Delay_4us();
		Data <<= 1;
		 
	}
	SCL_Pin_Output_Low();
	SensorDelay_us(8);   
	SDA_Pin_IN_FLOATING();
	SensorDelay_us(8);	
}	


uint8_t AHT20_RD_Byte(void)//Leer un byte de AHT20
{
	uint8_t Byte,i,a;
	Byte = 0;
	SCL_Pin_Output_Low();
	SDA_Pin_IN_FLOATING();
	SensorDelay_us(8);	
	for(i=0;i<8;i++)
	{
    SCL_Pin_Output_High();		
		Delay_5us();
		a=0;
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)) a=1;
		Byte = (Byte<<1)|a;
		SCL_Pin_Output_Low();
		Delay_5us();
	}
  SDA_Pin_IN_FLOATING();
	SensorDelay_us(8);	
	return Byte;
}


uint8_t Receive_ACK(void)   //Ver si AHT20 responde con ACK
{
	uint16_t CNT;
	CNT = 0;
	SCL_Pin_Output_Low();	
	SDA_Pin_IN_FLOATING();
	SensorDelay_us(8);	
	SCL_Pin_Output_High();	
	SensorDelay_us(8);	
	while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15))  && CNT < 100) 
	CNT++;
	if(CNT == 100)
	{
		return 0;
	}
 	SCL_Pin_Output_Low();	
	SensorDelay_us(8);	
	return 1;
}

void Send_ACK(void)		  // Señal ACK de respuesta del host
{
	SCL_Pin_Output_Low();	
	SensorDelay_us(8);	
	SDA_Pin_Output_Low();
	SensorDelay_us(8);	
	SCL_Pin_Output_High();	
	SensorDelay_us(8);
	SCL_Pin_Output_Low();	
	SensorDelay_us(8);
	SDA_Pin_IN_FLOATING();
	SensorDelay_us(8);
}

void Send_NOT_ACK(void)	//El host no responde ACK
{
	SCL_Pin_Output_Low();	
	SensorDelay_us(8);
	SDA_Pin_Output_High();
	SensorDelay_us(8);
	SCL_Pin_Output_High();	
	SensorDelay_us(8);		
	SCL_Pin_Output_Low();	
	SensorDelay_us(8);
    SDA_Pin_Output_Low();
	SensorDelay_us(8);
}

void Stop_I2C(void)	  // fin de un protocolo
{
	SDA_Pin_Output_Low();
	SensorDelay_us(8);
	SCL_Pin_Output_High();	
	SensorDelay_us(8);
	SDA_Pin_Output_High();
	SensorDelay_us(8);
}

uint8_t AHT20_Read_Status(void)//Lee el registro de estado de AHT20
{

	uint8_t Byte_first;	
	I2C_Start();
	AHT20_WR_Byte(0x71);
	Receive_ACK();
	Byte_first = AHT20_RD_Byte();
	Send_NOT_ACK();
	Stop_I2C();
	return Byte_first;
}

uint8_t AHT20_Read_Cal_Enable(void)  //Compruebe si el bit de habilitación de calibración está habilitado
{
	uint8_t val = 0;//ret = 0,
  val = AHT20_Read_Status();
	 if((val & 0x68)==0x08)
		 return 1;
   else  return 0;
 }

void AHT20_SendAC(void) //Enviar comando AC a AHT20
{

	I2C_Start();
	AHT20_WR_Byte(0x70);
	Receive_ACK();
	AHT20_WR_Byte(0xac);//Comando de colección 0xAC
	Receive_ACK();
	AHT20_WR_Byte(0x33);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	Stop_I2C();

}

// Tipo de verificación CRC: CRC8/MAXIM
//Polinomio: X8+X5+X4+1
// Poli: 0011 0001 0x31
//Pon el bit alto atrás y se convierte en 1000 1100 0x8c
//C código real:
uint8_t Calc_CRC8(uint8_t *message,uint8_t Num)
{
        uint8_t i;
        uint8_t byte;
        uint8_t crc=0xFF;
  for(byte=0; byte<Num; byte++)
  {
    crc^=(message[byte]);
    for(i=8;i>0;--i)
    {
      if(crc&0x80) crc=(crc<<1)^0x31;
      else crc=(crc<<1);
    }
  }
        return crc;
}

void AHT20_Read_CTdata(uint32_t *ct) // Sin verificación de CRC, lea directamente los datos de temperatura y humedad de AHT20
{
	volatile uint8_t  Byte_1th=0;
	volatile uint8_t  Byte_2th=0;
	volatile uint8_t  Byte_3th=0;
	volatile uint8_t  Byte_4th=0;
	volatile uint8_t  Byte_5th=0;
	volatile uint8_t  Byte_6th=0;
	 uint32_t RetuData = 0;
	uint16_t cnt = 0;
	AHT20_SendAC();//Enviar comando AC a AHT10
	Delay_1ms(80);//Retraso de unos 80ms
    cnt = 0;
	while(((AHT20_Read_Status()&0x80)==0x80))//Hasta que el bit de estado[7] sea 0, significa estado inactivo, si es 1, significa estado ocupado
	{
		SensorDelay_us(1508);
		if(cnt++>=100)
		{
		 break;
		 }
	}
	I2C_Start();
	AHT20_WR_Byte(0x71);
	Receive_ACK();
	Byte_1th = AHT20_RD_Byte();
	//Palabra de estado, el estado consultado es 0x98, lo que indica estado ocupado, el bit [7] es 1; el estado es 0x1C, o 0x0C, o 0x08, lo que indica estado inactivo, el bit [7] es 0
	Send_ACK();
	Byte_2th = AHT20_RD_Byte();//humedad
	Send_ACK();
	Byte_3th = AHT20_RD_Byte();//humedad
	Send_ACK();
	Byte_4th = AHT20_RD_Byte();//humedad/temperatura
	Send_ACK();
	Byte_5th = AHT20_RD_Byte();//la temperatura
	Send_ACK();
	Byte_6th = AHT20_RD_Byte();//la temperatura
	Send_NOT_ACK();
	Stop_I2C();

	RetuData = (RetuData|Byte_2th)<<8;
	RetuData = (RetuData|Byte_3th)<<8;
	RetuData = (RetuData|Byte_4th);
	RetuData =RetuData >>4;
	ct[0] = RetuData;//humedad
	RetuData = 0;
	RetuData = (RetuData|Byte_4th)<<8;
	RetuData = (RetuData|Byte_5th)<<8;
	RetuData = (RetuData|Byte_6th);
	RetuData = RetuData&0xfffff;
	ct[1] =RetuData; //la temperatura

}


void AHT20_Read_CTdata_crc(uint32_t *ct) //Después de la verificación CRC, lea los datos de temperatura y humedad de AHT20
{
	volatile uint8_t  Byte_1th=0;
	volatile uint8_t  Byte_2th=0;
	volatile uint8_t  Byte_3th=0;
	volatile uint8_t  Byte_4th=0;
	volatile uint8_t  Byte_5th=0;
	volatile uint8_t  Byte_6th=0;
	volatile uint8_t  Byte_7th=0;
	 uint32_t RetuData = 0;
	 uint16_t cnt = 0;
	// uint8_t  CRCDATA=0;
	 uint8_t  CTDATA[6]={0};//Usado para matriz de transferencia CRC
	
	AHT20_SendAC();//Enviar comando AC a AHT10
	Delay_1ms(80);//Retraso de unos 80ms
    cnt = 0;
	while(((AHT20_Read_Status()&0x80)==0x80))//Hasta que el bit de estado[7] sea 0, significa estado inactivo, si es 1, significa estado ocupado
	{
		SensorDelay_us(1508);
		if(cnt++>=100)
		{
		 break;
		}
	}
	
	I2C_Start();

	AHT20_WR_Byte(0x71);
	Receive_ACK();
	CTDATA[0]=Byte_1th = AHT20_RD_Byte();
	//Palabra de estado, el estado consultado es 0x98, lo que indica estado ocupado, el bit [7] es 1; el estado es 0x1C, o 0x0C, o 0x08, lo que indica estado inactivo, el bit [7] es 0
	Send_ACK();
	CTDATA[1]=Byte_2th = AHT20_RD_Byte();//humedad
	Send_ACK();
	CTDATA[2]=Byte_3th = AHT20_RD_Byte();//humedad
	Send_ACK();
	CTDATA[3]=Byte_4th = AHT20_RD_Byte();//humedad/temperatura
	Send_ACK();
	CTDATA[4]=Byte_5th = AHT20_RD_Byte();//humedad
	Send_ACK();
	CTDATA[5]=Byte_6th = AHT20_RD_Byte();//humedad
	Send_ACK();
	Byte_7th = AHT20_RD_Byte();//Datos CRC
	Send_NOT_ACK();                           // Nota: el último es enviar NAK
	Stop_I2C();
	
	if(Calc_CRC8(CTDATA,6)==Byte_7th)
	{
	RetuData = (RetuData|Byte_2th)<<8;
	RetuData = (RetuData|Byte_3th)<<8;
	RetuData = (RetuData|Byte_4th);
	RetuData =RetuData >>4;
	ct[0] = RetuData;//humedad
	RetuData = 0;
	RetuData = (RetuData|Byte_4th)<<8;
	RetuData = (RetuData|Byte_5th)<<8;
	RetuData = (RetuData|Byte_6th);
	RetuData = RetuData&0xfffff;
	ct[1] =RetuData; //humedad
		
	}
	else
	{
		ct[0]=0x00;
		ct[1]=0x00;//Valor de retorno de error de validación, los clientes pueden cambiarlo según sus propias necesidades
	}//Datos CRC
}


void AHT20_Init(void)   //Inicializar AHT20
{	
	Init_I2C_Sensor_Port();
	I2C_Start();
	AHT20_WR_Byte(0x70);
	Receive_ACK();
	AHT20_WR_Byte(0xa8);//0xA8 entra en modo de trabajo NOR
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	Stop_I2C();

	Delay_1ms(10);//Retraso de unos 10ms

	I2C_Start();
	AHT20_WR_Byte(0x70);
	Receive_ACK();
	AHT20_WR_Byte(0xbe);// Comando de inicialización 0xBE, el comando de inicialización AHT20 es 0xBE, el comando de inicialización AHT10 es 0xE1
	Receive_ACK();
	AHT20_WR_Byte(0x08);//El bit de registro relacionado[3] se establece en 1 para la salida de calibración
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	Stop_I2C();
	Delay_1ms(10);
//Retraso de unos 10ms
}
void JH_Reset_REG(uint8_t addr)
{
	
	uint8_t Byte_first,Byte_second,Byte_third,Byte_fourth;
	I2C_Start();
	AHT20_WR_Byte(0x70);//Resultó ser 0x70
	Receive_ACK();
	AHT20_WR_Byte(addr);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	AHT20_WR_Byte(0x00);
	Receive_ACK();
	Stop_I2C();

	Delay_1ms(5);//Retraso de unos 5ms
	I2C_Start();
	AHT20_WR_Byte(0x71);//
	Receive_ACK();
	Byte_first = AHT20_RD_Byte();
	Send_ACK();
	Byte_second = AHT20_RD_Byte();
	Send_ACK();
	Byte_third = AHT20_RD_Byte();
	Send_NOT_ACK();
	Stop_I2C();
	
    Delay_1ms(10);//Retraso de unos 10ms
	I2C_Start();
	AHT20_WR_Byte(0x70);///
	Receive_ACK();
	AHT20_WR_Byte(0xB0|addr);////Registrar comando
	Receive_ACK();
	AHT20_WR_Byte(Byte_second);
	Receive_ACK();
	AHT20_WR_Byte(Byte_third);
	Receive_ACK();
	Stop_I2C();
	
	Byte_second=0x00;
	Byte_third =0x00;
}

void AHT20_Start_Init(void)
{
	JH_Reset_REG(0x1b);
	JH_Reset_REG(0x1c);
	JH_Reset_REG(0x1e);
}

int32_t main(void)
{
    uint32_t CT_data[2];
	volatile int  c1,t1;
	/***********************************************************************************/
	/**///①Justo después del encendido, el chip del producto tarda en estar listo, el retraso es de 100~500ms, se recomiendan 500ms
	/***********************************************************************************/
	Delay_1ms(500);
	/***********************************************************************************/
	/**///② Envíe 0x71 para leer la palabra de estado por primera vez después del encendido y juzgue si la palabra de estado es 0x18, si no es 0x18, inicialice el registro
	/***********************************************************************************/
	if((AHT20_Read_Status()&0x18)!=0x18)
	{
	AHT20_Start_Init(); // reinicializar registros
	Delay_1ms(10);
	}
	
	/***********************************************************************************/
	/**///③Envíe comandos de medición para leer datos de temperatura y humedad de acuerdo con las necesidades del cliente. Actualmente, mientras (1) bucles para enviar comandos de medición para leer datos de temperatura y humedad, es solo para referencia
	/***********************************************************************************/
	while(1)
	{
	 AHT20_Read_CTdata(CT_data);       // lea directamente los datos de temperatura y humedad de AHT20 sin verificación de CRC, se recomienda leer una vez cada más de 1S
    //AHT20_Read_CTdata_crc(CT_data); //Después de la verificación de CRC, lea los datos de temperatura y humedad de AHT20 
	

	 c1 = CT_data[0]*100*10/1024/1024;  // Calcular el valor de humedad c1 (ampliado por 10 veces)
	 t1 = CT_data[1]*200*10/1024/1024-500;// Calcular el valor de temperatura t1 (ampliado por 10 veces)
////El siguiente paso es procesar los datos de visualización del cliente,
	 }
     printf("\nhum %i", c1);
     printf("\ntemp %i", t1);
     
}	
