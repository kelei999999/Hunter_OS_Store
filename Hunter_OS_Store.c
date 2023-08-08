/* Hunter.ORG all rights reserved.  You may use this software
 * and any derivatives exclusively with Hunter.ORG products.
 *
 * THIS SOFTWARE IS SUPPLIED BY Hunter.ORG "AS IS".  NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH Hunter.ORG PRODUCTS, COMBINATION
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT WILL Hunter.ORG BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF Hunter.ORG HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE
 * FULLEST EXTENT ALLOWED BY LAW, Hunter.ORG'S TOTAL LIABILITY ON ALL CLAIMS
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF
 * ANY, THAT YOU HAVE PAID DIRECTLY TO Hunter.ORG FOR THIS SOFTWARE.
 *
 * Hunter.ORG PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
 * TERMS.
 */

/*
 * File: Hunter_OS_Store.c
 * Author: Hunter.ORG
 * Created on: 2019年2月11日
 * Revision history: 1.0
 */

/*
 * This is a guard condition so that contents of this file are not included
 * more than once.
 */
#include "Hunter_OS_Store.h"
//#include "Peripheral_Driver.h"
//#define		MEMORY_NODE					4
static void OS_Dummy(void){}
extern uint16 CHIP_max_sector;
extern uint16 CHIP_max_block;
file_bsw_reset File_BSW_Reset = (file_bsw_reset)OS_Dummy;
file_bsw_read File_BSW_Read = (file_bsw_read)OS_Dummy;
file_bsw_write File_BSW_Write = (file_bsw_write)OS_Dummy;
eeprom_read EepromRead = (eeprom_read)OS_Dummy;
eeprom_write EepromWrite = (eeprom_write)OS_Dummy;
extern const uint16 Queue_Length;
uint32 File_OperaTimeBase = 0;
extern uint8 wFile_Executed[File_Queue_Length];
extern uint32 wFile_StartAdress[File_Queue_Length];
extern uint32 wFile_Length_Byte[File_Queue_Length];
extern file_data *wFile_Contents[File_Queue_Length];
extern uint8 wFile_Node[File_Queue_Length];
extern uint8 rFile_Executed[File_Queue_Length];
extern uint32 rFile_StartAdress[File_Queue_Length];
extern uint32 rFile_Length_Byte[File_Queue_Length];
extern file_data *rFile_Contents[File_Queue_Length];
extern uint8 rFile_Node[File_Queue_Length];
extern uint8 cFile_Executed[File_Queue_Length];
extern uint32 cFile_StartAdress[File_Queue_Length];
extern uint32 cFile_Length_Byte[File_Queue_Length];
extern uint8 cFile_Node[File_Queue_Length];
extern uint8 dFile_Executed[File_Queue_Length];
extern uint32 dFile_StartNumber[File_Queue_Length];
extern uint32 dFile_Count[File_Queue_Length];
extern uint8 dFile_Type[File_Queue_Length];
extern uint8 dFile_Node[File_Queue_Length];
volatile file_write_queue File_Write_Queue = {0,0,wFile_StartAdress,wFile_Length_Byte,wFile_Contents,wFile_Executed,wFile_Node};
volatile file_read_queue File_Read_Queue = {0,0,rFile_StartAdress,rFile_Length_Byte,rFile_Contents,rFile_Executed,rFile_Node};
volatile file_create_queue File_Create_Queue = {0,0,cFile_StartAdress,cFile_Length_Byte,cFile_Executed,cFile_Node};
volatile file_delete_queue File_Delete_Queue = {0,0,dFile_Type,dFile_StartNumber,dFile_Count,dFile_Executed,dFile_Node};
volatile callback CallBack = {0,-1};
static uint8 File_Init_Flag = 0;
/*****************************************************************************
 * @函数名	void File_SystemInit(file_bsw_reset File_Bsw_Reset,file_bsw_read File_Bsw_Read,file_bsw_write File_Bsw_Write)
 *----------------------------------------------------------------------------
 * @描述	文件系统的初始化函数
 *----------------------------------------------------------------------------
 * @输入	File_Bsw_Reset:底层驱动复位函数
 * 			File_Bsw_Read：底层读取函数
 *			File_Bsw_Write：底层写入
 *----------------------------------------------------------------------------
 * @输出	无
 *
 *----------------------------------------------------------------------------
 * @返回值	无
 *
 *----------------------------------------------------------------------------
 * @日期	2018.8.22
 *
 ****************************************************************************/
void File_SystemInit(file_bsw_reset File_Bsw_Reset,file_bsw_read File_Bsw_Read,file_bsw_write File_Bsw_Write)
{
	File_BSW_Reset = File_Bsw_Reset;
	File_BSW_Read = File_Bsw_Read;
	File_BSW_Write = File_Bsw_Write;
	File_Init_Flag = 1;
}
/****************************************************************************
 * @函数名	int8 File_WriteQueue_Query(int8 Write_Queue_Number)
 *----------------------------------------------------------------------------
 * @描述	根据队列号查询该任务是否被执行
 *----------------------------------------------------------------------------
 * @输入	Write_Queue_Number：队列入队时的编号，入队时返回该编号，应用程序自行保
 *			存以便使用
 *----------------------------------------------------------------------------
 * @返回值	0：已经被执行
 *			1：尚未执行
 *			-1：没有该队列
 ****************************************************************************/
static int8 File_WriteQueue_Query(int8 Write_Queue_Number)
{
	if(Write_Queue_Number > Queue_Length)
		return -1;
	else
		return File_Write_Queue.File_Executed[(uint8)Write_Queue_Number];
}
/****************************************************************************
 * @函数名	int8 File_Write_CMD(uint8 Node,uint32 StartAdress,uint32 Length,file_data* data)
 *----------------------------------------------------------------------------
 * @描述	写入文件入队函数，当有数据要写入FLASH时，调用本函数执行入队
 *----------------------------------------------------------------------------
 * @输入	Node：存储器节点号，当系统存在多个存储器时用于区分；
 *			StartAdress：指定存储的起始地址；
 *			data：要存储的数据；注意：队列不会拷贝数据副本，应用程序必须保证在数据
 *			被保存到存储器之前不能发生变化，否则将存入最后变化的数据；
 *			Length：要保存数据的长度
 *----------------------------------------------------------------------------
 * @返回值	-1：入队失败，队列已满
 *			≥0：入队成功，返回值为入队编号，应用程序应自行保存好该编号以便后续查询
 *			该队数据是否被保存。
 ****************************************************************************/
static int8 File_Write_CMD(uint8 Node,uint32 StartAdress,uint32 Length,file_data* data)
{
	int8 temp;
	if(1 == File_Init_Flag)
	{
		File_Write_Queue.StartAdress[File_Write_Queue.WritePoint] = StartAdress;
		File_Write_Queue.File_Length_Byte[File_Write_Queue.WritePoint] = Length;
		File_Write_Queue.File_Contents[File_Write_Queue.WritePoint] = data;
		File_Write_Queue.File_Executed[File_Write_Queue.WritePoint] = 1;
		File_Write_Queue.Node[File_Write_Queue.WritePoint] = Node;
		temp = File_Write_Queue.WritePoint;
		if(File_Write_Queue.WritePoint < Queue_Length - 1)
		{
			File_Write_Queue.WritePoint ++;
			if(File_Write_Queue.WritePoint == File_Write_Queue.ReadPoint)
			{
				File_Write_Queue.WritePoint --;
				return -1;
			}
		}
		else
		{
			if(File_Write_Queue.ReadPoint == 0)
			{
				File_Write_Queue.WritePoint = Queue_Length - 1;
				return -1;
			}
			else
				File_Write_Queue.WritePoint = 0;
		}
	}
	else
		temp = -1;
	return temp;
}
/****************************************************************************
 * @函数名	uint8 File_Write(void)
 *----------------------------------------------------------------------------
 * @描述	文件写入状态机函数，执行真正的函数写入。该函数将在文件时基中周期的执行
 *----------------------------------------------------------------------------
 * @输入	无
 *----------------------------------------------------------------------------
 * @返回值	无
 ****************************************************************************/
static uint8 Write_State;
static uint8 File_Write(void)
{
	static uint8 Write_CMD[16];
	static uint32 File_WriteByte_Point = 0;
	static uint32 File_WriteByte_Length = 0;
	static uint32 File_WriteTempAdress;
	static uint8 File_WriteExecuted = 0;
	static uint32 File_WriteStartAdress = 0;
	static uint32 File_WriteLength_Byte = 0;
	static uint8 MEMORY_NODE = 0;
	static file_data *File_WriteContents = 0;
	static uint16 Case_4_OverTime = 0;
	static uint8 *WriteByte;
	#ifndef OVERTIME
		#define		OVERTIME				100
	#else
		#undef OVERTIME
		#define		OVERTIME				100
	#endif
	//-----------------------------------------------------------------
	switch(Write_State)
	{
		case 0:
		{
			if(File_Write_Queue.WritePoint != File_Write_Queue.ReadPoint)
			{
				File_WriteStartAdress = File_Write_Queue.StartAdress[File_Write_Queue.ReadPoint];
				File_WriteLength_Byte = File_Write_Queue.File_Length_Byte[File_Write_Queue.ReadPoint];
				File_WriteContents = File_Write_Queue.File_Contents[File_Write_Queue.ReadPoint];
				MEMORY_NODE = File_Write_Queue.Node[File_Write_Queue.ReadPoint];
				File_WriteExecuted = File_Write_Queue.ReadPoint;
				if(File_Write_Queue.ReadPoint < Queue_Length - 1)
					File_Write_Queue.ReadPoint ++;
				else
					File_Write_Queue.ReadPoint = 0;
				Write_State = 1;//启动状态机
				File_WriteByte_Point = 0;

				//UART.OS_Uart_Tx_DMA(4,&File_Contents->File_DATA[0],File_Length_Byte);
			}
		}break;
		case 1:
		{
			if(File_WriteLength_Byte < 256)
			{
				if(File_WriteLength_Byte > 0)
				{
					if(File_WriteLength_Byte < (256 - File_WriteStartAdress % 256))
						File_WriteByte_Length = File_WriteLength_Byte;
					else
						File_WriteByte_Length = 256 - File_WriteStartAdress % 256;
					File_WriteByte_Point = 0;
					Write_State = 2;//开始新的写入
				}
				else
				{
					Write_State = 0;//结束状态机
					File_Write_Queue.File_Executed[File_WriteExecuted] = 0;
				}
			}
			else
			{
				File_WriteByte_Length = 256 - File_WriteStartAdress % 256;
				File_WriteByte_Point = 0;
				Write_State = 2;//开始新的写入
			}
		}break;
		case 2:
		{
			//SPI.OS_SPI_Reset(MEMORY_NODE);
			File_BSW_Reset(MEMORY_NODE);
			Case_4_OverTime = 0;
			Write_State = 3;
		}break;
		case 3:/*打开写使能*/
		{
			//---------------------------------------------------------
			Write_CMD[0] = 0x06;/*写使能命令*/
			//CallBack.Flag = 0;//复位回调标志
			if(0 == File_BSW_Write(MEMORY_NODE,1,Write_CMD))
			//if(0 == SPI.OS_SPI_Tx_DMA(MEMORY_NODE,1,Write_CMD))
			{
				Write_State = 4;
				CallBack.Flag = 0;//复位回调标志
				CallBack.Node = -1;
			}
		}break;
		case 4:/*读取WEL位是否为1*/
		{
			if(1 == CallBack.Flag)
			{
				File_BSW_Reset(MEMORY_NODE);
				//SPI.OS_SPI_Reset(MEMORY_NODE);
				//----------------------------------------------
				if(1 == ((Write_CMD[1] >> 1) & 1))
				{
					Write_State = 5;//成功设置写入使能位
				}
				//----------------------------------------------
				CallBack.Flag = 0;//复位回调标志
			}
			else
			{
				Write_CMD[0] = 0x05;/*读取状态字*/
				Write_CMD[1] = 0x00;/*读取状态字*/
				CallBack.Flag = 0;//复位回调标志
				if(0 == File_BSW_Read(MEMORY_NODE,2,Write_CMD,0))
				//if(0 == SPI.OS_SPI_ReadDATA(MEMORY_NODE,2,Write_CMD,0))
				{
					//CallBack.Flag = 0;//复位回调标志
					//CallBack.Node = -1;
				}
			}
			//--------------------------------------------------检测是否超时
			if(Case_4_OverTime < OVERTIME)
				Case_4_OverTime ++;
			else
			{
				Write_CMD[0] = 0x04;/**/
				if(0 == File_BSW_Write(MEMORY_NODE,1,Write_CMD))
				//if(0 == SPI.OS_SPI_Tx_DMA(MEMORY_NODE,1,Write_CMD))
				Write_State = 2;//重新开始新的写入
			}
		}break;
		case 5:/*开始写入数据*/
		{
			WriteByte = (uint8 *)File_WriteContents;
			File_WriteTempAdress = File_WriteStartAdress + File_WriteByte_Point;
			//------------------------------------------------------------
			WriteByte = &WriteByte[File_WriteByte_Point];
			//------------------------------------------------------------
			WriteByte[0] = 0x02;
			WriteByte[1] = (File_WriteTempAdress >> 16) & 0xFF;
			WriteByte[2] = (File_WriteTempAdress >> 8) & 0xFF;
			WriteByte[3] = (File_WriteTempAdress >> 0) & 0xFF;
			//SPI.OS_SPI_Reset(MEMORY_NODE);
			//UART.OS_Uart_Tx_DMA(4,&WriteByte[4],File_WriteByte_Length);
			CallBack.Flag = 0;//复位回调标志
			//------------------------------------------------------------
			if(0 == File_BSW_Write(MEMORY_NODE,File_WriteByte_Length + 4,WriteByte))
			//if(0 == SPI.OS_SPI_Tx_DMA(MEMORY_NODE,File_WriteByte_Length + 4,WriteByte))
			{
				Write_State = 6;//等待写入完成
				File_WriteByte_Point += File_WriteByte_Length;
			}

		}break;
		case 6:/*等待写入完成*/
		{
			uint32 temp = 256 - (File_WriteStartAdress + File_WriteByte_Point) % 256;
			if((File_WriteByte_Point + temp) < File_WriteLength_Byte)//剩余的字节数多余256个字节
			{
				File_WriteByte_Length = temp;
				Write_State = 2;//开始新的写入
			}
			else
			{
				if(File_WriteByte_Point < File_WriteLength_Byte)//把最后的字节写完
				{
					File_WriteByte_Length = File_WriteLength_Byte - File_WriteByte_Point;
					Write_State = 2;//开始新的写入
				}
				else//所有数据写入完毕，结束状态机
				{
					Write_State = 0;//结束状态机
					File_Write_Queue.File_Executed[File_WriteExecuted] = 0;
				}
			}
		}
		default:break;
	}
	return Write_State;
}
/*****************************************************************************
 * @函数名	int8 File_ReadQueue_Query(int8 Read_Queue_Number)
 *----------------------------------------------------------------------------
 * @描述	根据队列号查询该任务是否被执行
 *----------------------------------------------------------------------------
 * @输入	Read_Queue_Number：队列入队时的编号，入队时返回该编号，应用程序自行保
 *			存以便使用
 *----------------------------------------------------------------------------
 * @返回值	0：已经被执行
 *			1：尚未执行
 *			-1：没有该队列
 ****************************************************************************/
static int8 File_ReadQueue_Query(int8 Read_Queue_Number)
{
	if(Read_Queue_Number > Queue_Length)
		return -1;
	else
		return File_Read_Queue.File_Executed[(uint8)Read_Queue_Number];
}
/*****************************************************************************
 * @函数名	int8 File_Read_CMD(uint8 Node,uint32 StartAdress,uint32 Length,file_data* data)
 *----------------------------------------------------------------------------
 * @描述	文件写入状态机函数，执行真正的函数写入。该函数将在文件时基中周期的执行
 *----------------------------------------------------------------------------
 * @输入	Node:指定文件操作的存器块，默认为0.当有多个存储器时，应指定编号
 * 			StartAdress：指定从从存储器的地址开始读取数据
 * 			Length：指定读取数据长度
 * 			* data：指定读取出来的数据的存放的放的位置注意：队列不会拷贝数据副本，应用程序必须保证在数据
 *			被保存到存储器之前不能发生变化，否则将存入最后变化的数据；
 *----------------------------------------------------------------------------
 * @返回值	-1：读取错误。可能是读取队列已满，或者系统未初始化
 * 			其他值：队列序号，应用程序可以使用该队列序号去查询是否已经完成读取
 ****************************************************************************/
static int8 File_Read_CMD(uint8 Node,uint32 StartAdress,uint32 Length,file_data* data)
{
	int8 temp;
	if(1 == File_Init_Flag)
	{
		File_Read_Queue.StartAdress[File_Read_Queue.WritePoint] = StartAdress;
		File_Read_Queue.File_Length_Byte[File_Read_Queue.WritePoint] = Length;
		File_Read_Queue.File_Contents[File_Read_Queue.WritePoint] = data;
		File_Read_Queue.File_Executed[File_Read_Queue.WritePoint] = 1;
		File_Read_Queue.Node[File_Read_Queue.WritePoint] = Node;
		temp = File_Read_Queue.WritePoint;
		if(File_Read_Queue.WritePoint < Queue_Length - 1)
		{
			File_Read_Queue.WritePoint ++;
			if(File_Read_Queue.WritePoint == File_Read_Queue.ReadPoint)
			{
				File_Read_Queue.WritePoint --;
				return -1;
			}
		}
		else
		{
			if(File_Read_Queue.ReadPoint == 0)
			{
				File_Read_Queue.WritePoint = Queue_Length - 1;
				return -1;
			}
			else
				File_Read_Queue.WritePoint = 0;
		}
	}
	else
		temp = -1;
	return temp;
}
/*****************************************************************************
 * @函数名	int8 File_Read(void)
 *----------------------------------------------------------------------------
 * @描述	文件写入状态机函数，执行真正的函数写入。该函数将在文件时基中周期的执行
 *----------------------------------------------------------------------------
 * @输入	无
 *----------------------------------------------------------------------------
 * @返回值	无
 ****************************************************************************/
static uint8 Read_State = 0;
static uint8 File_Read(void)
{
	static uint8 File_Executed = 0;
	static uint32 File_StartAdress = 0;
	static uint32 File_Length_Byte = 0;
	static uint8 MEMORY_NODE = 0;
	static file_data *File_Contents = 0;
	static uint16 Case_2_OverTime = 0;
	#ifndef OVERTIME
		#define		OVERTIME				100
	#else
		#undef 		OVERTIME
		#define		OVERTIME				500
	#endif
	switch(Read_State)
	{
		case 0:
		{
			if(File_Read_Queue.WritePoint != File_Read_Queue.ReadPoint)
			{
				File_StartAdress = File_Read_Queue.StartAdress[File_Read_Queue.ReadPoint];
				File_Length_Byte = File_Read_Queue.File_Length_Byte[File_Read_Queue.ReadPoint];
				File_Contents = File_Read_Queue.File_Contents[File_Read_Queue.ReadPoint];
				MEMORY_NODE = File_Read_Queue.Node[File_Read_Queue.ReadPoint];
				File_Read_Queue.File_Executed[File_Read_Queue.ReadPoint] = 1;
				File_Executed = File_Read_Queue.ReadPoint;
				if(File_Read_Queue.ReadPoint < Queue_Length - 1)
					File_Read_Queue.ReadPoint ++;
				else
					File_Read_Queue.ReadPoint = 0;
				Read_State = 1;//启动状态机
			}
		}break;
		case 1:
		{
			//SPI.OS_SPI_Reset(MEMORY_NODE);
			File_BSW_Reset(MEMORY_NODE);
			Case_2_OverTime = 0;
			Read_State = 2;
		}break;
		case 2:
		{
			uint8* ReadByte = (uint8*)File_Contents;
			ReadByte[0] = 0x03;
			ReadByte[1] = (File_StartAdress >> 16) & 0xff;
			ReadByte[2] = (File_StartAdress >> 8) & 0xff;
			ReadByte[3] = (File_StartAdress >> 0) & 0xff;
			CallBack.Flag = 0;//复位回调标志
			if(0 == File_BSW_Read(MEMORY_NODE,File_Length_Byte + 4,ReadByte,0))
			//if(0 == SPI.OS_SPI_ReadDATA(MEMORY_NODE,File_Length_Byte + 4,ReadByte,0))
			{
				//CallBack.Flag = 0;//复位回调标志
				//CallBack.Node = -1;
				Read_State = 3;//
			}
		}break;
		case 3:
		{
			if(1 == CallBack.Flag)
			{
				//SPI.OS_SPI_Reset(MEMORY_NODE);
				File_BSW_Reset(MEMORY_NODE);
				File_Read_Queue.File_Executed[File_Executed] = 0;
				Read_State = 0;//停止状态机
				CallBack.Flag = 0;//复位回调标志
				CallBack.Node = -1;
			}
			//---------------------------------------
			if(Case_2_OverTime < OVERTIME)
				Case_2_OverTime ++;
			else
				Read_State = 0;//停止状态机
		}break;
	}
	return Read_State;
}
/*****************************************************************************
 * @函数名	int8 File_Read_CallBack(uint8 Node,uint32 Length,void *data)
 *----------------------------------------------------------------------------
 * @描述	文件读取回调函数，当发送完读取命令后，等待读取返回，一旦读取数据完毕，本
 *			回调函数将被执行。本质上本函数为物理层读取中断
 *----------------------------------------------------------------------------
 * @输出	Node：当前回调的节点，当存在多个外设回调时，用于区分外设编号
 *			Length：本次回调的数据的长度，单位为字节
 *			*data：本次回调的数据
 *----------------------------------------------------------------------------
 * @返回值	无
 ****************************************************************************/
static void File_Read_CallBack(uint8 Node,uint32 Length,void *data)
{
	CallBack.Flag = 1;//设置回调标志
	CallBack.Node = Node;//设置回调标志
}
/*****************************************************************************
 * @函数名	int8 File_CreateQueue_Query(int8 Create_Queue_Number)
 *----------------------------------------------------------------------------
 * @描述	根据队列号查询该任务是否被执行
 *----------------------------------------------------------------------------
 * @输入	Create_Queue_Number：队列入队时的编号，入队时返回该编号，应用程序自行保
 *			存以便使用
 *----------------------------------------------------------------------------
 * @返回值	0：已经被执行
 *			1：尚未执行
 *			-1：没有该队列
 ****************************************************************************/
static int8 File_CreateQueue_Query(int8 Create_Queue_Number)
{
	if(Create_Queue_Number > Queue_Length)
		return -1;
	else
		return File_Create_Queue.File_Executed[(uint8)Create_Queue_Number];
}
/*****************************************************************************
 * @函数名	int8 File_Create_CMD(uint8 Node,uint32 Length,uint32 *StartAdress)
 *----------------------------------------------------------------------------
 * @描述	文件创建函数，本质上就是在FLASH中开辟一段地址，并将其擦除。严格保证Sector
 *			对齐
 *----------------------------------------------------------------------------
 * @输入	Node:指明底层存储器节点号
 * 			Length：欲开辟的文件长度，单位为字节(byte)
 *----------------------------------------------------------------------------
 * @输出	*StartAdress：开辟成功后的起始地址
 *----------------------------------------------------------------------------
 * @返回值	无
 ****************************************************************************/
static uint8 Create_State;
static int8 File_Create_CMD(uint8 Node,uint32 Length,uint32 *StartAdress)
{
	int8 temp;
	if(1 == File_Init_Flag)
	{
		File_Create_Queue.StartAdress[File_Create_Queue.WritePoint] = (uint32)StartAdress;
		File_Create_Queue.File_Length_Byte[File_Create_Queue.WritePoint] = Length;
		File_Create_Queue.File_Executed[File_Create_Queue.WritePoint] = 1;
		File_Create_Queue.Node[File_Create_Queue.WritePoint] = Node;
		temp = File_Create_Queue.WritePoint;
		if(File_Create_Queue.WritePoint < Queue_Length - 1)
		{
			File_Create_Queue.WritePoint ++;
			if(File_Create_Queue.WritePoint == File_Create_Queue.ReadPoint)
			{
				File_Create_Queue.WritePoint --;
				return -1;
			}
		}
		else
		{
			if(File_Create_Queue.ReadPoint == 0)
			{
				File_Create_Queue.WritePoint = Queue_Length - 1;
				return -1;
			}
			else
				File_Create_Queue.WritePoint = 0;
		}
	}
	else
		temp = -1;
	return temp;

}
/*****************************************************************************
 * @函数名	uint8 File_Create(void)
 *----------------------------------------------------------------------------
 * @描述	文件创建状态机函数，执行真正的文件创建。该函数将在文件时基中周期的执行。
 *----------------------------------------------------------------------------
 * @输入	无
 *----------------------------------------------------------------------------
 * @返回值	无
 ****************************************************************************/
static uint8 File_Create(void)
{
	static uint8 Create_CMD[16];
	static uint8 File_Executed = 0;
	uint32 File_Length_Byte = 0;
	static uint16 Create_Length;
	static uint16 Erase_Length;
	static uint8 MEMORY_NODE = 0;
	static uint16 Case_5_OverT = 0;
	switch(Create_State)
	{
		case 0:
		{
			if(File_Create_Queue.WritePoint != File_Create_Queue.ReadPoint)
			{
				File_Length_Byte = File_Create_Queue.File_Length_Byte[File_Create_Queue.ReadPoint];
				MEMORY_NODE = File_Create_Queue.Node[File_Create_Queue.ReadPoint];
				File_Executed = File_Create_Queue.ReadPoint;
				if(File_Create_Queue.ReadPoint < Queue_Length - 1)
					File_Create_Queue.ReadPoint ++;
				else
					File_Create_Queue.ReadPoint = 0;
				uint32 TEMP1,TEMP2;
				TEMP1 = File_Length_Byte >> 16;
				TEMP2 = File_Length_Byte - (TEMP1 << 16);
				if(0 == TEMP2)
					Create_Length = TEMP1;
				else
					Create_Length = TEMP1 + 1;
				Erase_Length = 0;
				Create_State = 1;//启动状态机
			}
		}break;
		case 1:/*初始化*/
		{
			Create_State = 2;
			Case_5_OverT = 0;
			//SPI.OS_SPI_Reset(MEMORY_NODE);
			File_BSW_Reset(MEMORY_NODE);
		}break;
		case 2:/*打开写使能*/
		{
			Create_CMD[0] = 0x06;/*写使能命令*/
			if(0 == File_BSW_Write(MEMORY_NODE,1,Create_CMD))
			//if(0 == SPI.OS_SPI_Tx_DMA(MEMORY_NODE,1,Create_CMD))
			Create_State = 3;
		}break;
		case 3:/*读取WEL位是否为1,并等待回调*/
		{
			if(1 == CallBack.Flag)
			{
				//SPI.OS_SPI_Reset(MEMORY_NODE);
				File_BSW_Reset(MEMORY_NODE);
				//----------------------------------------------
				if(1 == ((Create_CMD[1] >> 1) & 1))
				{
					Create_State = 4;//成功设置写入使能位
				}
				//---------------------------------------------
				CallBack.Flag = 0;//复位回调标志
				CallBack.Node = -1;
			}
			else
			{
				Create_CMD[0] = 0x05;/*读取状态字*/
				if(0 == File_BSW_Read(MEMORY_NODE,2,Create_CMD,0))
				//if(0 == SPI.OS_SPI_ReadDATA(MEMORY_NODE,2,Create_CMD,0))
				{

				}
			}
		}break;
		case 4:/*擦除扇区*/
		{
			Create_CMD[0] = 0xD8;/*擦除Block命令*/
			Create_CMD[1] = Erase_Length;/*擦除Block 1*/
			Create_CMD[2] = 0x0;
			Create_CMD[3] = 0x0;
			if(0 == File_BSW_Write(MEMORY_NODE,4,Create_CMD))
			//if(0 == SPI.OS_SPI_Tx_DMA(MEMORY_NODE,4,Create_CMD))
			{
				Create_State = 5;
			}
			if(Erase_Length < Create_Length)
				Erase_Length ++;
		}break;
		case 5:/*读取WEL位是否为0*/
		{
			if(1 == CallBack.Flag)
			{
				//SPI.OS_SPI_Reset(MEMORY_NODE);
				File_BSW_Reset(MEMORY_NODE);
				//----------------------------------------------
				if(0 == ((Create_CMD[1] >> 1) & 1))
				{
					if(Erase_Length < Create_Length)
						Create_State = 1;//上一次擦除成功，继续下一次擦除
					else
					{
						uint32 *p = (uint32 *)File_Create_Queue.StartAdress[File_Executed];
						*p = 0;
						File_Create_Queue.File_Executed[File_Executed] = 0;
						Create_State = 0;//完成擦除
					}
				}
				CallBack.Flag = 0;//复位回调标志
				CallBack.Node = -1;
			}
			else
			{
				Create_CMD[0] = 0x05;/*读取状态字*/
				if(0 == File_BSW_Read(MEMORY_NODE,2,Create_CMD,0))
				//if(0 == SPI.OS_SPI_ReadDATA(MEMORY_NODE,2,Create_CMD,0))
				{

				}
			}
			if(Case_5_OverT < 10000)
				Case_5_OverT ++;
			else
			{
				Create_CMD[0] = 0x04;/**/
				if(0 == File_BSW_Write(MEMORY_NODE,1,Create_CMD))
				//if(0 == SPI.OS_SPI_Tx_DMA(MEMORY_NODE,1,Create_CMD))
				Create_State = 1;
			}
		}break;
		default:break;
	}
	return Create_State;
}
/*****************************************************************************
 * @函数名	int8 File_DeleteQueue_Query(int8 Delete_Queue_Number)
 *----------------------------------------------------------------------------
 * @描述	根据队列号查询该任务是否被执行
 *----------------------------------------------------------------------------
 * @输入	Delete_Queue_Number：队列入队时的编号，入队时返回该编号，应用程序自行保
 *			存以便使用
 *----------------------------------------------------------------------------
 * @返回值	0：已经被执行
 *			1：尚未执行
 *			-1：没有该队列
 *			-2：擦除sector超出最大长度
 *			-3：擦除Block超出最大长度
 ****************************************************************************/
static int8 File_DeleteQueue_Query(int8 Delete_Queue_Number)
{
	if(Delete_Queue_Number > Queue_Length)
		return -1;
	else
		return File_Delete_Queue.File_Executed[(uint8)Delete_Queue_Number];
}
/*****************************************************************************
 * @函数名	int8 File_Delete_CMD(uint8 TYPE,uint32 StartNumber,uint32 Count)
 *----------------------------------------------------------------------------
 * @描述	文件操作删除函数命令，调用本函数将删除文件命令添加到删除队列中
 *----------------------------------------------------------------------------
 * @输入	Node:指明底层存储器节点号
 * 			TYPE:0，(Sector)扇区删除；1，(Block)板块擦除
 *			StartNumber:对应扇区或者版块的序号，该处应根据实际的芯片来节定。比如GD25Q16E的Sector
 *			序号范围是0～511，Block的序号范围是0～31
 *			Count:本次要删除的块数量
 *----------------------------------------------------------------------------
 * @返回值	删除队列序号
 ****************************************************************************/
static int8 File_Delete_CMD(uint8 Node,uint8 TYPE,uint32 StartNumber,uint32 Count)
{
	int8 temp;
	if(1 == File_Init_Flag)
	{
		if(0 == TYPE)
		{
			if((StartNumber + Count) > CHIP_max_sector)
				return -2;
		}
		else if(1 == TYPE)
		{
			if((StartNumber + Count) > CHIP_max_block)
				return -3;
		}
		File_Delete_Queue.Type[File_Delete_Queue.WritePoint] = (uint32)TYPE;
		File_Delete_Queue.StartNumber[File_Delete_Queue.WritePoint] = (uint32)StartNumber;
		File_Delete_Queue.Count[File_Delete_Queue.WritePoint] = Count;
		File_Delete_Queue.File_Executed[File_Delete_Queue.WritePoint] = 1;
		File_Delete_Queue.Node[File_Delete_Queue.WritePoint] = Node;
		temp = File_Delete_Queue.WritePoint;
		if(File_Delete_Queue.WritePoint < Queue_Length - 1)
		{
			File_Delete_Queue.WritePoint ++;
			if(File_Delete_Queue.WritePoint == File_Delete_Queue.ReadPoint)
			{
				File_Delete_Queue.WritePoint --;
				return -1;
			}
		}
		else
		{
			if(File_Delete_Queue.ReadPoint == 0)
			{
				File_Delete_Queue.WritePoint = Queue_Length - 1;
				return -1;
			}
			else
				File_Delete_Queue.WritePoint = 0;
		}
	}
	else
		temp = -1;
	return temp;
}
/*****************************************************************************
 * @函数名	File_Delete(void)
 *----------------------------------------------------------------------------
 * @描述	文件创建状态机函数，执行真正的文件创建。该函数将在文件时基中周期的执行。
 *----------------------------------------------------------------------------
 * @输入	无
 *----------------------------------------------------------------------------
 * @返回值	无
 ****************************************************************************/
static uint8 Delete_State = 0;
static uint8 File_Delete(void)
{
	static uint8 Delete_CMD[16];
	static uint8 File_Executed = 0;
	static uint32 Erase_Count = 0;
	static uint16 Erase_TYPE;
	static uint16 Erase_StartNumber;
	static uint16 Erase_Length;
	static uint8 MEMORY_NODE = 0;
	static uint16 Case_5_OverT = 0;
	switch(Delete_State)
	{
		case 0:
		{
			if(File_Delete_Queue.WritePoint != File_Delete_Queue.ReadPoint)
			{
				Erase_Count = File_Delete_Queue.Count[File_Delete_Queue.ReadPoint];
				Erase_TYPE = File_Delete_Queue.Type[File_Delete_Queue.ReadPoint];
				Erase_StartNumber = File_Delete_Queue.StartNumber[File_Delete_Queue.ReadPoint];
				MEMORY_NODE = File_Delete_Queue.Node[File_Delete_Queue.ReadPoint];
				File_Executed = File_Delete_Queue.ReadPoint;
				if(File_Delete_Queue.ReadPoint < Queue_Length - 1)
					File_Delete_Queue.ReadPoint ++;
				else
					File_Delete_Queue.ReadPoint = 0;
				Delete_State = 1;//启动状态机
				Erase_Length = 0;//已经擦除的长度
			}
		}break;
		case 1:/*初始化*/
		{
			Delete_State = 2;
			Case_5_OverT = 0;
			//SPI.OS_SPI_Reset(MEMORY_NODE);
			File_BSW_Reset(MEMORY_NODE);
		}break;
		case 2:/*打开写使能*/
		{
			Delete_CMD[0] = 0x06;/*写使能命令*/
			if(0 == File_BSW_Write(MEMORY_NODE,1,Delete_CMD))
			//if(0 == SPI.OS_SPI_Tx_DMA(MEMORY_NODE,1,Create_CMD))
				Delete_State = 3;
		}break;
		case 3:/*读取WEL位是否为1,并等待回调*/
		{
			if(1 == CallBack.Flag)
			{
				//SPI.OS_SPI_Reset(MEMORY_NODE);
				File_BSW_Reset(MEMORY_NODE);
				//----------------------------------------------
				if(1 == ((Delete_CMD[1] >> 1) & 1))
				{
					Delete_State = 4;//成功设置写入使能位
				}
				//---------------------------------------------
				CallBack.Flag = 0;//复位回调标志
				CallBack.Node = -1;
			}
			else
			{
				Delete_CMD[0] = 0x05;/*读取状态字*/
				if(0 == File_BSW_Read(MEMORY_NODE,2,Delete_CMD,0))
				//if(0 == SPI.OS_SPI_ReadDATA(MEMORY_NODE,2,Create_CMD,0))
				{

				}
			}
		}break;
		case 4:/*擦除扇区*/
		{
			if(0 == Erase_TYPE)
			{
				Delete_CMD[0] = 0x20;/*擦除Sector命令*/
				Delete_CMD[1] = ((Erase_StartNumber + Erase_Length) >> 4);/*擦除*/
				Delete_CMD[2] = ((Erase_StartNumber + Erase_Length) << 4);
				Delete_CMD[3] = 0x0;
			}
			else if(1 == Erase_TYPE)
			{
				Delete_CMD[0] = 0xD8;/*擦除Block命令*/
				Delete_CMD[1] = Erase_StartNumber + Erase_Length;/*擦除Block*/
				Delete_CMD[2] = 0x0;
				Delete_CMD[3] = 0x0;
			}
			if(0 == File_BSW_Write(MEMORY_NODE,4,Delete_CMD))
			//if(0 == SPI.OS_SPI_Tx_DMA(MEMORY_NODE,4,Create_CMD))
			{
				Delete_State = 5;
				if(Erase_Length < Erase_Count)
					Erase_Length ++;
			}
		}break;
		case 5:/*读取WEL位是否为0*/
		{
			if(1 == CallBack.Flag)
			{
				//SPI.OS_SPI_Reset(MEMORY_NODE);
				File_BSW_Reset(MEMORY_NODE);
				//----------------------------------------------
				if(0 == ((Delete_CMD[1] >> 1) & 1))
				{
					if(Erase_Length < Erase_Count)
						Delete_State = 1;//上一次擦除成功，继续下一次擦除
					else
					{
						File_Delete_Queue.File_Executed[File_Executed] = 0;
						Delete_State = 0;//完成擦除
					}
				}
				CallBack.Flag = 0;//复位回调标志
				CallBack.Node = -1;
			}
			else
			{
				Delete_CMD[0] = 0x05;/*读取状态字*/
				if(0 == File_BSW_Read(MEMORY_NODE,2,Delete_CMD,0))
				//if(0 == SPI.OS_SPI_ReadDATA(MEMORY_NODE,2,Create_CMD,0))
				{

				}
			}
			if(Case_5_OverT < 10000)
				Case_5_OverT ++;
			else
			{
				Delete_CMD[0] = 0x04;/**/
				if(0 == File_BSW_Write(MEMORY_NODE,1,Delete_CMD))
				//if(0 == SPI.OS_SPI_Tx_DMA(MEMORY_NODE,1,Create_CMD))
					Delete_State = 1;
			}
		}break;
		default:break;
	}
	return Delete_State;
}
/*****************************************************************************
 * @函数名	void File_Operation_TimeBase(void* Parameter)
 *----------------------------------------------------------------------------
 * @描述	文件操作时基函数，该函数应为操作系统的一个周期任务，推荐1ms周期
 *----------------------------------------------------------------------------
 * @输入	无
 *----------------------------------------------------------------------------
 * @返回值	无
 ****************************************************************************/
#define File_Opera_Task_Max			4
uint8 (*File_Opera_Task[File_Opera_Task_Max])(void) = {File_Read,File_Create,File_Write,File_Delete};
uint8 (*File_Opera_TaskX)(void);
static void File_Operation_TimeBase(void* Parameter)
{
	uint8 Priority;
	File_OperaTimeBase ++;
	//--------------------------------------------------------自动优先级排序，当三个任务有一个开始执行
	//--------------------------------------------------------另外3个自动阻塞，同一时刻三个只能任选其一执行
	for(uint8 Opera_Task_Count = 0;Opera_Task_Count < File_Opera_Task_Max;Opera_Task_Count ++)
	{
		Priority = File_Opera_Task[0]();
		//----------------------------------------------------该状态机尚未被执行,重新排列优先级
		if(0 == Priority)
		{
			File_Opera_TaskX = File_Opera_Task[0];
			File_Opera_Task[0] = File_Opera_Task[1];
			File_Opera_Task[1] = File_Opera_Task[2];
			File_Opera_Task[2] = File_Opera_Task[3];
			File_Opera_Task[3] = File_Opera_TaskX;
		}
		else
			break;
	}
}
/*****************************************************************************
 * @函数名	void EEPROM_StoreInit(eeprom_read EEPROM_Read,eeprom_write EEPROM_Write)
 *----------------------------------------------------------------------------
 * @描述	文件系统的初始化函数
 *----------------------------------------------------------------------------
 * @输入
 * 			EEPROM_Read：底层读取函数
 *			EEPROM_Write：底层写入
 *----------------------------------------------------------------------------
 * @输出	无
 *
 *----------------------------------------------------------------------------
 * @返回值	无
 *
 *----------------------------------------------------------------------------
 * @日期	2018.8.22
 *
 ****************************************************************************/
static void EEPROM_StoreInit(eeprom_read EEPROM_Read,eeprom_write EEPROM_Write)
{
	EepromRead = EEPROM_Read;
	EepromWrite = EEPROM_Write;
}
extern uint32 EEPROM_Write_buff[EEPROM_WriteBuff_length];
extern uint16 EEPROM_Write_adr[EEPROM_WriteBuff_length];
extern uint16 eepromWriteBuff_length;
static uint16 W_Pointer = 0,W_Pointer_shadow = 0;
//****************************************************************************
// @函数名	int8 Store_Write(uint16 address,uint32 data)
//----------------------------------------------------------------------------
// @描述		存储器写入函数
//----------------------------------------------------------------------------
// @输入		address：指定的存储器内存地址
//				data：要存储的数据
//----------------------------------------------------------------------------
// @输出		无
//----------------------------------------------------------------------------
// @返回值		-1：存储失败，底层存储器可能错误
//----------------------------------------------------------------------------
// @日期		2018.3.1
//****************************************************************************
static int8 Store_Write(uint16 address,uint32 data)
{
	int16 temp;
	if(W_Pointer < W_Pointer_shadow)
		temp = W_Pointer + eepromWriteBuff_length - W_Pointer_shadow;
	else
		temp = W_Pointer - W_Pointer_shadow;
	if(temp >= eepromWriteBuff_length)//缓冲器已满，返回错误
		return -1;
	if(W_Pointer < eepromWriteBuff_length - 1)
		W_Pointer ++;
	else
		W_Pointer = 0;
	EEPROM_Write_buff[W_Pointer] = data;
	EEPROM_Write_adr[W_Pointer] = address;
	return 0;
}
//****************************************************************************
// @函数名	uint32 Store_read(uint16 address)
//----------------------------------------------------------------------------
// @描述		存储器读取函数
//----------------------------------------------------------------------------
// @输入		address：指定的存储器内存地址
//----------------------------------------------------------------------------
// @输出		指定地址的存储值
//----------------------------------------------------------------------------
// @返回值
//----------------------------------------------------------------------------
// @日期		2018.3.1
//****************************************************************************
static uint32 Store_read(uint16 address,int8 *Verify)
{
	return EepromRead(address,Verify);
}
//****************************************************************************
// @函数名	void Store_Time_Gape(uint32 *DATA)
//----------------------------------------------------------------------------
// @描述		存储器写入任务
//----------------------------------------------------------------------------
// @输入		*DATA：保留地址
//----------------------------------------------------------------------------
// @输出		无
//----------------------------------------------------------------------------
// @返回值
//----------------------------------------------------------------------------
// @日期		2018.3.1
//****************************************************************************
static void Store_Time_Gape(uint32 *DATA)
{
	if(W_Pointer_shadow != W_Pointer)
	{
		if(W_Pointer_shadow < (eepromWriteBuff_length - 1))
			W_Pointer_shadow ++;
		else
			W_Pointer_shadow = 0;
		if(-1 == EepromWrite(EEPROM_Write_adr[W_Pointer_shadow],EEPROM_Write_buff[W_Pointer_shadow]))//if return -1 signify write unsuccessed
		{
			if(W_Pointer_shadow > 0)
			W_Pointer_shadow --;
			else
				W_Pointer_shadow = 	(eepromWriteBuff_length - 1);
		}
	}
}
volatile htos_store HTOS_Store = {
		File_SystemInit,
		File_Operation_TimeBase,
		File_Read_CallBack,
		File_CreateQueue_Query,
		File_Create_CMD,
		File_ReadQueue_Query,
		File_Read_CMD,
		File_WriteQueue_Query,
		File_Write_CMD,
		File_DeleteQueue_Query,
		File_Delete_CMD,
		EEPROM_StoreInit,
		Store_Write,
		Store_read,
		Store_Time_Gape
};
