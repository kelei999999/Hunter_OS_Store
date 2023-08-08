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
 * File: Hunter_OS_Store_Variable.c
 * Author: Hunter.ORG
 * Created on: 2019年2月11日
 * Revision history: 1.0
 */

/*
 * This is a guard condition so that contents of this file are not included
 * more than once.
 */
#include "Hunter_OS_Store.h"
//-----------------------------------------------------------------------
uint16 CHIP_max_sector = CHIP_MAX_SECTOR;
uint16 CHIP_max_block = CHIP_MAX_BLOCK;
uint8 wFile_Executed[File_Queue_Length] = {0};
uint32 wFile_StartAdress[File_Queue_Length] = {0};
uint32 wFile_Length_Byte[File_Queue_Length] = {0};
file_data *wFile_Contents[File_Queue_Length];
uint8 wFile_Node[File_Queue_Length] = {0};
uint8 rFile_Executed[File_Queue_Length] = {0};
uint32 rFile_StartAdress[File_Queue_Length] = {0};
uint32 rFile_Length_Byte[File_Queue_Length] = {0};
file_data *rFile_Contents[File_Queue_Length] = {0};
uint8 rFile_Node[File_Queue_Length] = {0};
uint8 cFile_Executed[File_Queue_Length] = {0};
uint32 cFile_StartAdress[File_Queue_Length] = {0};
uint32 cFile_Length_Byte[File_Queue_Length] = {0};
uint8 cFile_Node[File_Queue_Length] = {0};
uint8 dFile_Executed[File_Queue_Length] = {0};
uint32 dFile_StartNumber[File_Queue_Length] = {0};
uint32 dFile_Count[File_Queue_Length] = {0};
uint8 dFile_Type[File_Queue_Length] = {0};
uint8 dFile_Node[File_Queue_Length] = {0};
const uint16 Queue_Length = File_Queue_Length;
uint32 EEPROM_Write_buff[EEPROM_WriteBuff_length] = {0};
uint16 EEPROM_Write_adr[EEPROM_WriteBuff_length] = {0};
uint16 eepromWriteBuff_length = EEPROM_WriteBuff_length;
