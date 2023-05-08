
/* Kelei999999(WangLiang) all rights reserved.  You may use this software
 * and any derivatives exclusively with Kelei999999(WangLiang) products.
 *
 * THIS SOFTWARE IS SUPPLIED BY Kelei999999(WangLiang) "AS IS".  NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH Kelei999999(WangLiang) PRODUCTS, COMBINATION
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT WILL Kelei999999(WangLiang) BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF Kelei999999(WangLiang) HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE
 * FULLEST EXTENT ALLOWED BY LAW, Kelei999999(WangLiang)'S TOTAL LIABILITY ON ALL CLAIMS
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF
 * ANY, THAT YOU HAVE PAID DIRECTLY TO Kelei999999(WangLiang) FOR THIS SOFTWARE.
 *
 * Kelei999999(WangLiang) PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
 * TERMS.
 */

/*
 * File: Hunter_OS_Store.h
 * Author: Kelei999999(WangLiang)
 * Created on: 2014年2月11日
 * Revision history: 1.1
 */

/*
 * This is a guard condition so that contents of this file are not included
 * more than once.
 */

#ifndef HUNTER_OS_STORE_H_
#define HUNTER_OS_STORE_H_
#include "Typedef.h"
/*
 *	 以下为文件系统的API集合使用说明，需要特注意的是，File_CallBack(uint8 Node,uint32 Length,void* data)本质上本函数为物理层读取中断，请将此函数回调至物理层的中断
 * typedef struct
 *	{
 *	void (*File_SystemInit)(file_bsw_reset File_Bsw_Reset,file_bsw_read File_Bsw_Read,file_bsw_write File_Bsw_Write);
 *	void (*File_TimeBase)(void* Parameter);
 *	void (*File_CallBack)(uint8 Node,uint32 Length,void* data);
 *	int8 (*File_CreateQueue_Query)(int8 Create_Queue_Number);
 *	int8 (*File_Create_CMD)(uint8 Node,uint32 Length,uint32* StartAdress);
 *	int8 (*File_ReadQueue_Query)(int8 Read_Queue_Number);
 *	int8 (*File_Read_CMD)(uint8 Node,uint32 StartAdress,uint32 Length,file_data* data);
 *	int8 (*File_WriteQueue_Query)(int8 Write_Queue_Number);
 *	int8 (*File_Write_CMD)(uint8 Node,uint32 StartAdress,uint32 Length,file_data* data);
 *	int8 (*File_DeleteQueue_Query)(int8 Delete_Queue_Number);
 *	int8 (*File_Delete_CMD)(uint8 Node,uint8 TYPE,uint32 StartNumber,uint32 Count);
 *	void (*EEPROM_Store_Init)(eeprom_read EEPROM_Read,eeprom_write EEPROM_Write);
 *	int8 (*EEPROM_Store_Write)(uint8 Node,uint16 address,uint32 data);
 *	uint32 (*EEPROM_Store_read)(uint8 Node,uint16 address,int8 *Verify);
 *	void (*EEPROM_Store_TimeBase)(uint32 *Parameter);
 *	}htos_store;
 *	extern volatile htos_store HTOS_Store;
 */
/*
 * 	文件操作结构体类型。所有要操作的文件，必须转换为file_data格式。
 * 	举例说明：
 * 1、欲从FLASH中读取1024个数据到内存中，代码如下：
 * uint8 ReadByte[1028];
 * file_data *ReadByteFile = (file_data *)ReadByte;
 * //(1024 + 4)中的4代表命令字节数，任何读取都需增加固定长度4
 * uint8 temp = HTOS_Store.File_Read_CMD(4,0,1024 + 4,ReadByteFile);
 * //等待读取数据完毕
 * while(0 == HTOS_Store.File_ReadQueue_Query(temp))
 * {
 * 		//do something
 * 		ReadByteFile.File_DATA;//这才是正在的数据，对应从ReadByte[4]开始，因为前四个字节用于命令
 * }
 * 2、欲将1024个数据写入FLASH中(假设原始数据存储于DATA[]中)，代码如下：
 * uint8 WriteByte[1028];
 * file_data *WriteByteFile = (file_data *)WriteByte;
 * //填充原始数据
 * for(uint16 i = 0;i < 1024;i ++)
 * {
 * 		//DATA[i]即原始数据，要写入FLASH需先拷贝出来，或者在获取数据时就按file_data格式排列
 * 		WriteByteFile.File_DATA[i] = DATA[i];
 * }
 * uint8 temp = HTOS_Store.File_Write_CMD(4,0,1024 + 4,WriteByteFile);
 * //等待写入完毕
 * while(0 == HTOS_Store.File_WriteQueue_Query(temp))
 * {
 *
 * }
 * //do something
 */
#define 	CHIP_MAX_SECTOR						511				/*spi flash 芯片的Sector数量，该数据请根据SPI Flash芯片的规格书填写*/
#define 	CHIP_MAX_BLOCK						31				/*spi flash 芯片的Block数量，该数据请根据SPI Flash芯片的规格书填写*/
#define		File_Queue_Length					64				/*spi flash 文件系统存储队列长度，该数据请根据主控MCU芯片的RAM空间填写，但不应过小*/
#ifdef		EEPROM_WriteBuff_length
#undef 		EEPROM_WriteBuff_length
#endif
#define		EEPROM_WriteBuff_length				1024			/*EEPROM存储数据写入缓冲长度*/
/*
 * 	文件处理指针，任何欲处理的数据都必须转换为该格式才能处理。其本质就在于指定操作的内存地址的
 * 	前4个字节保留，真正的数据是从第4个字节开始
 */
typedef struct
{
	uint8 File_Head[4];/*SPI FLASH前4个字节为固定的操作头*/
	uint8 File_DATA[];
}file_data;

/*
 *	 文件写入队列格式定义
 */
typedef struct
{
	uint8 ReadPoint;
	uint8 WritePoint;
	uint32 *StartAdress;
	uint32 *File_Length_Byte;
	file_data **File_Contents;
	uint8 *File_Executed;
	uint8 *Node;
}file_write_queue;
extern volatile file_write_queue File_Write_Queue;
typedef struct
{
	uint8 ReadPoint;
	uint8 WritePoint;
	uint32 *StartAdress;
	uint32 *File_Length_Byte;
	uint8 *File_Executed;
	uint8 *Node;
}file_create_queue;
extern volatile file_create_queue File_Create_Queue;
typedef struct
{
	uint8 ReadPoint;
	uint8 WritePoint;
	uint8 *Type;
	uint32 *StartNumber;
	uint32 *Count;
	uint8 *File_Executed;
	uint8 *Node;
}file_delete_queue;
extern volatile file_delete_queue File_Delete_Queue;
/*
 * 	文件读取队列格式定义
 */
typedef struct
{
	uint8 ReadPoint;
	uint8 WritePoint;
	uint32 *StartAdress;
	uint32 *File_Length_Byte;
	file_data **File_Contents;
	uint8 *File_Executed;
	uint8 *Node;
}file_read_queue;
extern volatile file_read_queue File_Read_Queue;
/*
 * 	文件操作函数API集合,所有的操作必须调用其成员函数
 */
typedef void (*file_bsw_reset)(uint8 Node);
typedef int8 (*file_bsw_read)(uint8 Node,uint16 Length,uint8 *data,uint16 StoreAdress);
typedef int8 (*file_bsw_write)(uint8 Node,uint16 Tx_Length,uint8 *TransData);
typedef uint32 (*eeprom_read)(uint16 address,int8 *Verify);
typedef int8 (*eeprom_write)(uint16 address,uint32 data);
typedef struct
{
	/*****************************************************************************
	 * @函数名	void File_SystemInit(void)
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
	void (*File_SystemInit)(file_bsw_reset File_Bsw_Reset,file_bsw_read File_Bsw_Read,file_bsw_write File_Bsw_Write);
	/*****************************************************************************
	 * @函数名	void File_Operation_TimeBase(void* Parameter)
	 *----------------------------------------------------------------------------
	 * @描述	文件操作时基函数，该函数应为操作系统的一个周期任务，推荐1ms周期
	 *----------------------------------------------------------------------------
	 * @输入	无
	 *----------------------------------------------------------------------------
	 * @返回值	无
	 ****************************************************************************/
	void (*File_TimeBase)(void* Parameter);
	/*****************************************************************************
	 * @函数名	int8 File_CallBack(uint8 Node,uint32 Length,void *data)
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
	void (*File_CallBack)(uint8 Node,uint32 Length,void* data);
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
	int8 (*File_CreateQueue_Query)(int8 Create_Queue_Number);
	/*****************************************************************************
	 * @函数名	int8 File_Create_CMD(uint8 Node,uint32 Length,uint32 *StartAdress)
	 *----------------------------------------------------------------------------
	 * @描述	文件创建函数，本质上就是在FLASH中开辟一段地址，并将其擦除。严格保证Sector
	 *			对齐
	 *----------------------------------------------------------------------------
	 * @输入	Node:指明底层存储器节点号
	 * 			Length：欲开辟的文件长度，单位为字节(byte)
	 * 			*StartAdress：返回开辟的空间的首地址
	 *----------------------------------------------------------------------------
	 * @输出	*StartAdress：开辟成功后的起始地址
	 *----------------------------------------------------------------------------
	 * @返回值	无
	 ****************************************************************************/
	int8 (*File_Create_CMD)(uint8 Node,uint32 Length,uint32* StartAdress);
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
	int8 (*File_ReadQueue_Query)(int8 Read_Queue_Number);
	/*****************************************************************************
	 * @函数名	int8 File_Read_CMD(uint8 Node,uint32 StartAdress,uint32 Length,file_data* data)
	 *----------------------------------------------------------------------------
	 * @描述	文件写入状态机函数，执行真正的函数写入。该函数将在文件时基中周期的执行
	 *----------------------------------------------------------------------------
	 * @输入	Node:指定文件操作的存器块，默认为0.当有多个存储器时，应指定编号
	 * 			StartAdress：指定从从存储器的地址开始读取数据
	 * 			Length：指定读取数据长度
	 * 			* data：指定读取出来的数据的存放的放的位置。注意：队列不会拷贝数据副本，应用程序必须保证在数据
	 *			被保存到存储器之前不能发生变化，否则将存入最后变化的数据；
	 *----------------------------------------------------------------------------
	 * @返回值	-1：读取错误。可能是读取队列已满，或者系统未初始化
	 * 			其他值：队列序号，应用程序可以使用该队列序号去查询是否已经完成读取
	 ****************************************************************************/
	int8 (*File_Read_CMD)(uint8 Node,uint32 StartAdress,uint32 Length,file_data* data);
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
	int8 (*File_WriteQueue_Query)(int8 Write_Queue_Number);
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
	int8 (*File_Write_CMD)(uint8 Node,uint32 StartAdress,uint32 Length,file_data* data);
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
	int8 (*File_DeleteQueue_Query)(int8 Delete_Queue_Number);
	/*****************************************************************************
	 * @函数名	int8 File_Delete_CMD(uint8 Node,uint8 TYPE,uint32 StartNumber,uint32 Count)
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
	int8 (*File_Delete_CMD)(uint8 Node,uint8 TYPE,uint32 StartNumber,uint32 Count);
	/*****************************************************************************
	 * @函数名	void (*EEPROM_Store_Init)(eeprom_read EEPROM_Read,eeprom_write EEPROM_Write);
	 *----------------------------------------------------------------------------
	 * @描述	EEPROM存储系统初始化
	 *----------------------------------------------------------------------------
	 * @输入	EEPROM_Read:底层读取函数
	 * 			EEPROM_Write:底层写入函数
	 *----------------------------------------------------------------------------
	 * @返回值	无
	 ****************************************************************************/
	void (*EEPROM_Store_Init)(eeprom_read EEPROM_Read,eeprom_write EEPROM_Write);
	/*****************************************************************************
	 * @函数名	int8 EEPROM_Store_Write(uint16 address,uint32 data);
	 *----------------------------------------------------------------------------
	 * @描述	EEPROM存储器数据写入
	 *----------------------------------------------------------------------------
	 * @输入	address:指定要写入存储器数据的操作地址
	 *			data:要写入的数据
	 *----------------------------------------------------------------------------
	 * @返回值	-1：存储失败，底层存储器可能错误；0：写入成功
	 ****************************************************************************/
	int8 (*EEPROM_Store_Write)(uint16 address,uint32 data);
	/*****************************************************************************
	 * @函数名	uint32 EEPROM_Store_Read(uint16 address,int8 *Verify);
	 *----------------------------------------------------------------------------
	 * @描述	EEPROM存储器数据读取
	 *----------------------------------------------------------------------------
	 * @输入	address:指定要写入存储器数据的操作地址
	 *----------------------------------------------------------------------------
	 * @输出	*Verify:读取校验情况。0：校验正常，返回的数据有效；-1：校验错误，返回的数据无效
	 *----------------------------------------------------------------------------
	 * @返回值	读取到的数据
	 *----------------------------------------------------------------------------
	 * @日期	2014.3.12
	 ****************************************************************************/
	uint32 (*EEPROM_Store_Read)(uint16 address,int8 *Verify);
	/*****************************************************************************
	 * @函数名	void EEPROM_Store_TimeBase(void* Parameter)
	 *----------------------------------------------------------------------------
	 * @描述	文件操作时基函数，该函数应为操作系统的一个周期任务，推荐5ms周期
	 *----------------------------------------------------------------------------
	 * @输入	无
	 *----------------------------------------------------------------------------
	 * @返回值	无
	 ****************************************************************************/
	void (*EEPROM_Store_TimeBase)(uint32 *Parameter);
}htos_store;
extern volatile htos_store HTOS_Store;
typedef struct
{
	uint8 Flag;
	int8 Node;
}callback;
extern volatile callback CallBack;

#endif /* HUNTER_OS_STORE_H_ */
