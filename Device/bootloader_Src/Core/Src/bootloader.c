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

/*********************************************************************************************************************************/
/*Includes*/
#include "bootloader.h"

/*********************************************************************************************************************************/
/*Variables*/
uint8_t usb_rx_buf[64];
uint8_t uart_rx_buf[64];

/*********************************************************************************************************************************/
/*External Variables*/

extern __IO bool is_reset;
extern CRC_HandleTypeDef hcrc;
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern UART_HandleTypeDef huart2;
/*********************************************************************************************************************************/
/*Type definitions*/
typedef void (*func_ptr)(void);

/*********************************************************************************************************************************/
/*Functions*/
static uint8_t flash_stm_program(void);
static uint8_t conf_ps_fpga(void);
static uint8_t fpga_usb_to_uart(void);
static uint8_t flash_esp_program(void);
static uint32_t calculate_crc(CRC_HandleTypeDef hcrc);
static void reset(uint8_t reply);
/*********************************************************************************************************************************/

static void reset(uint8_t reply)
{
	if (reply)
	{
		send_reply((uint8_t) NACK);
	}
	else
	{
		send_reply((uint8_t) ACK);
	}
	/*Reset STM*/
	HAL_Delay(100);
	HAL_NVIC_SystemReset();
}


/**
  * @brief  bootloader_init
  *         Initialize the bootloader.
  *         @note
  *
  *
  * @param
  * @retval
  */
uint8_t bootloader_init(void)
{
	if (*(volatile uint64_t*)(USER_APPLICATION_STARTING_ADDRESS) == -1 || \
			((*(volatile uint32_t*)(USER_APPLICATION_HEADER_SIZE_ADDRESS)) & 0xFFFFFFFF) == 0 || \
			(((*(volatile uint32_t*)(USER_APPLICATION_HEADER_SIZE_ADDRESS)) & 0xFFFFFFFF) > (FLASH_SIZE-BOOTLOADER_SIZE)) || \
			(*(volatile uint32_t*)(USER_APPLICATION_HEADER_CRC_ADDRESS) != calculate_crc(hcrc)))
	{
		HAL_GPIO_WritePin(ERROR_LED_GPIO_Port, ERROR_LED_Pin, GPIO_PIN_SET);
		return 1;
	}
	else
	{
		HAL_GPIO_WritePin(OK_LED_GPIO_Port, OK_LED_Pin, GPIO_PIN_SET);
		return 0;
	}
}

/**
  * @brief  mode_handler
  *         Select mode depending on a command from the host.
  *         @note
  *
  *
  * @param
  * @retval
  */
void command_handler(uint32_t command)
{
	switch(command)
	{
	case STM_FLASH_PROGRAM:
		reset(flash_stm_program());
	break;
	case ESP_FLASH_PROGRAM:
		reset(flash_esp_program());
	break;
	case FPGA_PS_CONF:
		if (conf_ps_fpga())
		{
			reset(1);
		}
		else
		{
			reset(fpga_usb_to_uart());
		}
	break;
	}
}

/**
  * @brief  jump_to_user_application
  *         Jump to the user application.
  *         @note
  *         Perform the jump to the user application.
  *         Before performing the jump make sure to disable all peripherals (uart,spi,usb,timers,adcs,dacs and all gpios)
  *         Don't forget to disable and re-enable Interrupts
  *
  *
  * @param
  * @retval
  */
void jump_to_user_application()
{
	typedef void (*func_ptr)(void);
	func_ptr jump_to_app;
	uint32_t address = (uint32_t) USER_APPLICATION_STARTING_ADDRESS;
	uint32_t stack = (uint32_t)*((uint32_t*)address);
	jump_to_app = (func_ptr)*(uint32_t*)(address+4);
	deInitializer();
	__disable_irq();
	__set_MSP(stack);
	SCB->VTOR = address;
	__enable_irq();
	jump_to_app();
}


/**
  * @brief  flash_program_cmd
  *         Writes a program to memory.
  *         @note
  *
  *
  *
  * @param
  * @retval
  */

static uint8_t flash_stm_program()
{
	uint32_t address = (uint32_t)USER_APPLICATION_STARTING_ADDRESS;
	uint32_t file_size;
	uint32_t chunk_size = 64; /*Size of a usb packet*/
	uint32_t bytes_written = 0; /*bytes written in flash*/
	uint32_t loop_correction = 8; /*correcting the loop which make the bytes into doublewords depending on the leftover bytes*/
	uint8_t data[64];
	uint64_t dword = 0;

	send_reply((uint8_t) ACK);

	if (usb_cmd_timeout(50))
	{
		return 1;
	}
	else
	{
		uint8_t temp_num[4];
		CDC_Read_FS(temp_num,sizeof(temp_num));
		file_size = (uint32_t)(temp_num[0]<<24) | (uint32_t)(temp_num[1]<<16) | (uint32_t)(temp_num[2]<<8) | (uint32_t)(temp_num[3]);
		/*if the number of bytes is 0 or bigger than the available size then something is wrong*/
		if (file_size == 0 || file_size > (FLASH_END - USER_APPLICATION_STARTING_ADDRESS))
		{
			return 1;
		}

		/*Notify the host that we started erasing*/
		send_reply((uint8_t) ACK);

		/*Calculate the first page that we need to erase*/
		uint8_t first_page = (USER_APPLICATION_STARTING_ADDRESS - FLASH_BASE) / 0x800; /*0x800 is the page size*/

		/*Erase part*/
		FLASH_EraseInitTypeDef pEraseInit;
		pEraseInit.TypeErase=FLASH_TYPEERASE_PAGES;
		pEraseInit.Banks=FLASH_BANK_1;
		pEraseInit.Page = first_page;
		pEraseInit.NbPages= 256-first_page;
		uint32_t PageError = 0xFFFFFFFF;
		HAL_FLASH_Unlock();
		HAL_FLASHEx_Erase(&pEraseInit,&PageError);
		/*Notify the host if the erase was successful or not*/
		HAL_FLASH_Lock();
		if (PageError != 0xFFFFFFFF)
		{
			return 1;
		}
/*Write in flash part*/
		do
		{
			/*Request the next packet*/
			send_reply((uint8_t) ACK);
			HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_SET);
			if(usb_cmd_timeout(50))
			{
				return 1;
			}
			else
			{
				uint8_t usb_data_rx_size = CDC_Available_FS();
				CDC_Read_FS(data, usb_data_rx_size);
				HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);

				HAL_FLASH_Unlock();
				/*No mutex is needed here since we won't get new chunk of data unless we send an ack*/
				for (uint32_t i=0;i<loop_correction;i++)
				{
					dword = (uint64_t)(data[i*8])   | (uint64_t)(data[i*8+1]) << 8 | (uint64_t)(data[i*8+2]) << 16 | (uint64_t)(data[i*8+3]) << 24
							| (uint64_t)(data[i*8+4]) << 32 | (uint64_t)(data[i*8+5]) << 40 | (uint64_t)(data[i*8+6]) << 48 | (uint64_t)(data[i*8+7])<< 56;
					HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, dword);
					/*Check if the data written matches the data in the buffer*/
					if (dword != *(volatile uint64_t*)(address))
					{
						HAL_FLASH_Lock();
						HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);
						return 1;
					}
					else
					{
						/*clear the dword*/
						dword = 0;
						/*Increment the address*/
						address+=8;
					}
				}
				HAL_FLASH_Lock();
				/*Increment the bytes_written*/
				bytes_written+=chunk_size;
				/*Clear the buffer*/
				memset(data,'\0',sizeof(data));
				/*Correcting the loop due to leftover bytes*/
				if(file_size - bytes_written < chunk_size)
				{
					chunk_size = file_size - bytes_written;

					if (chunk_size%8)
					{
						loop_correction = (chunk_size/8)+1;
					}
					else
					{
						loop_correction = (chunk_size/8);
					}
				}
				HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);
			}
		}
		while(bytes_written-file_size);
	}
	return 0;
}


/**
  * @brief  usb_cmd_timeout
  *			Simple delay function.
  *         @note
  *
  *
  *
  * @param timeout: Timeout set by the user.
  * @retval uint8_t: Return 0 if the host sends data else return 1 if the time run out.
  */
uint8_t usb_cmd_timeout(uint32_t timeout)
{
	uint32_t timer = HAL_GetTick();

	while (HAL_GetTick()-timer <timeout)
	{
		if (CDC_Available_FS()>0)
		{
			return 0;
		}
	}
	return 1;
}
/**
  * @brief  send_reply
  *			Send a reply to notify the host.
  *         @note
  *
  *
  *
  *
  * @param data: byte to be sent.
  * @retval uint8_t: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t send_reply(uint8_t data)
{
	uint8_t reply = data;
	return CDC_Transmit_FS(&reply,1);
}
/**
  * @brief  calculate_crc
  *			Calculate the crc value of the application.
  *         @note
  *			This function is specific to the way we patch the binary file in the host.
  *
  *
  *
  * @param hcrc: crc handler.
  * @retval uint32_t: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
static uint32_t calculate_crc(CRC_HandleTypeDef hcrc)
{
	/*Retrieve the size of the application from the header.*/
	uint32_t app_size = *(volatile uint32_t*)(USER_APPLICATION_STARTING_ADDRESS+0x194);

	/*Calculate the crc value of the vector table (404 bytes) plus the size of the application (4 bytes)*/
	HAL_CRC_Calculate(&hcrc, (uint32_t*)USER_APPLICATION_STARTING_ADDRESS, 0x198);

	return HAL_CRC_Accumulate(&hcrc, (uint32_t*)(USER_APPLICATION_STARTING_ADDRESS+0x19C), (app_size-0x19C));
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART1)
	{
		HAL_UART_DMAPause(&huart1);
		CDC_Transmit_FS(uart_rx_buf, Size);
		memset(uart_rx_buf,'\0',sizeof(uart_rx_buf));
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1,uart_rx_buf,sizeof(uart_rx_buf));
		HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);
		HAL_UART_DMAResume(&huart1);
	}
	if (huart->Instance == USART2)
	{
		HAL_UART_DMAPause(&huart2);
		CDC_Transmit_FS(uart_rx_buf, Size);
		memset(uart_rx_buf,'\0',sizeof(uart_rx_buf));
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2,uart_rx_buf,sizeof(uart_rx_buf));
		HAL_UART_DMAResume(&huart2);
	}
}


uint8_t flash_esp_program()
{
	send_reply((uint8_t) ACK);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,uart_rx_buf,sizeof(uart_rx_buf));
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

	/*ESP download mode sequence*/
	HAL_GPIO_WritePin(ESP_IO9_GPIO_Port, ESP_IO9_Pin, GPIO_PIN_RESET); /*IO9 strapping pin high*/
	HAL_GPIO_WritePin(ESP_EN_GPIO_Port, ESP_EN_Pin, GPIO_PIN_SET); /*Reset pin low*/
	HAL_Delay(100);
	HAL_GPIO_WritePin(ESP_IO9_GPIO_Port, ESP_IO9_Pin, GPIO_PIN_SET); /*IO9 strapping pin low */
	HAL_GPIO_WritePin(ESP_EN_GPIO_Port, ESP_EN_Pin, GPIO_PIN_RESET); /*Reset pin high, esp is out of reset and in bootloader mode*/
	HAL_Delay(50);
	HAL_GPIO_WritePin(ESP_IO9_GPIO_Port, ESP_IO9_Pin, GPIO_PIN_RESET); /*IO9 strapping pin high*/
	HAL_Delay(400);
	while(!is_reset)
	{
		uint32_t data_length = CDC_Available_FS();
		if (data_length>0)
		{
			HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_SET);
			CDC_Read_FS(usb_rx_buf, data_length);
			HAL_Delay(10);
			HAL_UART_Transmit(&huart1,usb_rx_buf,data_length,10);
			memset(usb_rx_buf,'\0',data_length);
		}
	}
	HAL_GPIO_WritePin(ESP_EN_GPIO_Port, ESP_EN_Pin, GPIO_PIN_SET);
	HAL_Delay(500);
return 0;
}

static uint8_t conf_ps_fpga()
{
	uint32_t file_size;
	uint32_t bytes_written = 0;
	uint32_t timer = 0;
	/*Flash fpga cmd ACK*/
	send_reply((uint8_t) ACK);

	if (usb_cmd_timeout(50))
	{
		return 1;
	}
	else
	{
		uint8_t temp_num[4];
		CDC_Read_FS(temp_num,sizeof(temp_num));
		file_size = (uint32_t)(temp_num[0]<<24) | (uint32_t)(temp_num[1]<<16) | (uint32_t)(temp_num[2]<<8) | (uint32_t)(temp_num[3]);
		/*if the number of bytes is different than a standard bitstram size then something is wrong*/
		if (file_size != BITSTREAM_MAX_SIZE)
		{
			return 1;
		}
		/*File Size received ACK*/
		send_reply((uint8_t) ACK);
		/*Start configuration process.*/
		/*To begin we set the nCONFIG pin to low and high*/
		HAL_GPIO_WritePin(nCONFIG_GPIO_Port, nCONFIG_Pin, GPIO_PIN_RESET);
		HAL_Delay(50);
		HAL_GPIO_WritePin(nCONFIG_GPIO_Port, nCONFIG_Pin, GPIO_PIN_SET);

		/*Wait for nSTATUS to go high.*/
		HAL_Delay(300);
		send_reply((uint8_t) ACK);
		do
		{
			if (HAL_GPIO_ReadPin(nSTATUS_GPIO_Port, nSTATUS_Pin) == GPIO_PIN_RESET)
			{
				HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);
				return 1;
			}
			if(usb_cmd_timeout(50))
			{
				return 1;
			}
			else
			{
				uint8_t usb_data_rx_size = CDC_Available_FS();
				uint8_t data[usb_data_rx_size];
				if (CDC_Read_FS(data, usb_data_rx_size))
				{
					return 1;
				}
				for(uint32_t i=0;i<usb_data_rx_size;i++)
				{
					uint8_t temp=data[i];
					HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_SET);
					for (uint8_t j=0;j<8;j++)
					{
						//Bitbang the LSB of every byte.
						HAL_GPIO_WritePin(DATA0_GPIO_Port,DATA0_Pin,temp&0x01);
						temp=temp>>1;
						//Pulse the clock.
						HAL_GPIO_WritePin(DCLK_GPIO_Port,DCLK_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(DCLK_GPIO_Port,DCLK_Pin,GPIO_PIN_RESET);
					}
					HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);
				}
				bytes_written+=usb_data_rx_size;
				/*Request the next packet*/
				send_reply((uint8_t) ACK);
			}
		}while(bytes_written-file_size);
		/*Once all the data has been transmitted wait for CONF_DONE to be SET and set pins DCLK and DATA0 to high*/
		timer = HAL_GetTick();
		while (HAL_GetTick() - timer< 200)
		{
			if (HAL_GPIO_ReadPin(CONF_DONE_GPIO_Port, CONF_DONE_Pin) == GPIO_PIN_SET)
			{
				HAL_GPIO_WritePin(DATA0_GPIO_Port,DATA0_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(DCLK_GPIO_Port,DCLK_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(OK_LED_GPIO_Port, OK_LED_Pin, GPIO_PIN_SET);
				return 0;
			}
		}
	}
	return 1;
}

static uint8_t fpga_usb_to_uart()
{
	send_reply((uint8_t) ACK);
	while(!is_reset)
	{
		/*There is a race condition that I can't detect. Adding a 10 ms delay is a workaround that works for whatever reason.*/
		uint32_t data_length = CDC_Available_FS();
		if (data_length>0)
		{
			CDC_Read_FS(usb_rx_buf, data_length);
			HAL_Delay(10);
			HAL_UART_Transmit(&huart2,usb_rx_buf,data_length,10);
			memset(usb_rx_buf,'\0',data_length);
		}
	}
	return 0;
}


