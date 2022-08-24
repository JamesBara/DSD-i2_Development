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

/*Initialization sequence*/
#define STM_UPLOAD_PROGRAM 0x57932145

#define ACK 0x79
#define NACK 0x6E



#define DEV_ID_STM32L45x 0x0462 /*Device ID*/        
#define RDP_LEVEL_0 0xAA /*Readout unprotected*/


#define FLASH_SIZE 0x80000
#define BOOTLOADER_SIZE 0x10000  /*Change this depending on the size of the bootloader*/


#define ERASE_PAGE 25 /*Max erase time of a single page in ms*/
#define PAGE_WORD 100 /*Max write time of 64 bits (an mcu word) in ìs*/

#define HEADER_SIZE_OFFSET 0x194
#define HEADER_CRC_OFFSET HEADER_SIZE_OFFSET+4


#ifdef __cplusplus
extern "C" {
#endif
	int stm_rst(void);
	int stm_init(void);
	int stm_info(void);
	int stm_upload(int binary_file_size);
	int get_reply(void);
#ifdef __cplusplus
}
#endif
#endif /* INC_BOOTLOADER_H_ */

