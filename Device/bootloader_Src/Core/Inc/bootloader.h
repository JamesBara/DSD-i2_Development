/*
*MIT License
*
*Copyright (c) 2022 Dimitris Baradakis
*
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in all
*copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*SOFTWARE.
*/
#ifndef INC_BOOTLOADER_H_
#define INC_BOOTLOADER_H_
#include "main.h"
#include "usbd_cdc_if.h"
/*********************************************************************************************************************************/
/*Commands*/
#define STM_FLASH_PROGRAM 0x57932145
#define ESP_FLASH_PROGRAM 0x5EC932C3
#define FPGA_PS_CONF 0xF1A541C1
/*********************************************************************************************************************************/
/*STM addresses and offsets*/
#define BOOTLOADER_SIZE 0x10000
#define USER_APPLICATION_STARTING_ADDRESS 0x08010000
#define VECTOR_TABLE_LENGTH 0x194
#define USER_APPLICATION_HEADER_SIZE_ADDRESS (USER_APPLICATION_STARTING_ADDRESS + VECTOR_TABLE_LENGTH)
#define USER_APPLICATION_HEADER_CRC_ADDRESS (USER_APPLICATION_STARTING_ADDRESS + VECTOR_TABLE_LENGTH + 0x4)
/*********************************************************************************************************************************/
#define ACK 0x79U
#define NACK 0x6EU
/*********************************************************************************************************************************/
#define BITSTREAM_MAX_SIZE 368011 /*Cyclone 10 LP CL006 Bitstream max size in bytes*/
/*********************************************************************************************************************************/
/*Functions*/
uint8_t bootloader_init(void);
void command_handler(uint32_t command);
void jump_to_user_application(void);
uint8_t send_reply(uint8_t data);
uint8_t usb_cmd_timeout(uint32_t timeout);
#endif /* INC_BOOTLOADER_H_ */
