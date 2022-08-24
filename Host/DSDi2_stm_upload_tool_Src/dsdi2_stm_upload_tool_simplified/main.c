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

#include "main.h"

static int get_reply(int reply_timeout);
static int upload_to_stm(int binary_file_size);
static int test_for_bootloader(void);

static int get_reply(int reply_timeout)
{
       
    int start_tick = GetTickCount64();
    int elapsed_millisecs =0;

    while (elapsed_millisecs < reply_timeout)
    {
        
        if (ComPortAvailable() > 0)
        {
            break;
        }
        elapsed_millisecs = GetTickCount64() - start_tick;
    }

    /*Add a timeout maybe*/
    byte ans = 0;

    if (elapsed_millisecs >= reply_timeout || ComPortRead(&ans, sizeof(ans)) || ans != (byte)ACK)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}


int main(int argc, char* argv[])
{
    int binary_file_size;
    char* port;
    char* filePath;

    SetConsoleOutputCP(1253);


    if (argc = !2)
    {
        printf("> Wrong number of arguments.\n");
        return -1;
    }

    if (argv[1] == NULL || argv[2] == NULL)
    {
        printf("> No arguments found.\n");
        return -1;
    }

    printf("> Welcome to the upload tool for DSD-i2 board for Arduino.\n");
    fflush(stdout);
    filePath = calloc(strlen(argv[1]) + 1, sizeof(*filePath));

    if (filePath == NULL)
    {
        printf("> Memory allocation error for file path.\n");
        return -1;
    }
    else
    {
        memcpy(filePath, argv[1], strlen(argv[1]));
    }


    if (strlen(argv[2]) == 4)
    {
        port = calloc(strlen(argv[2]) + 1, sizeof(*port));

        if (port == NULL)
        {
            printf("> Memory allocation error for COM port.\n");
            return -1;
        }
        else
        {
            memcpy(port, argv[2], strlen(argv[2]));
        }
    }
    else
    {
        port = calloc(strlen("\\\\.\\") + strlen(argv[2]) + 1, sizeof(*port));

        if (port == NULL)
        {
            printf("> Memory allocation error for COM port.\n");
            return -1;
        }
        else
        {
            strcpy(port, "\\\\.\\");
            strcat(port, argv[2]);
        }
    }

    /*Initializiing the file handle.*/
    if (BinaryFileOpen(filePath))
    {
        printf("> Couldn't open .bin file.\n");
        fflush(stdout);
        printf("> Please make sure that the path is correct and the file is not in use.\n");
        free(port);
        free(filePath);
        return -1;
    }
    else
    {
        binary_file_size = BinaryGetSize();
    }

    /*Calculate if the file can fit the chip.*/
    if (binary_file_size == 0 || binary_file_size > (int)(FLASH_SIZE)-(int)(BOOTLOADER_SIZE))
    {
        printf("> File size error. \n");
        free(port);
        free(filePath);
        return -1;
    }

    /* Since the binary file that we are going to upload to STM32 has a header in which we are storing
     * the size and the CRC, we need to add those since they are blank.
     * The following procedure stores the size first, then calculates and stores the CRC value.
     */
     /*Patch the binary file header with the size.*/
    printf("> Patching the file... \n");
    fflush(stdout);
    if (BinaryPatchHeader(binary_file_size, (int)HEADER_SIZE_OFFSET))
    {
        printf("> Failed to apply size patch.\n");
        free(port);
        free(filePath);
        return -1;
    }
    printf("> Size patch applied successfully.\n");
    fflush(stdout);
    printf("> Calculating CRC32 please wait...\n");
    fflush(stdout);
    uint32_t crc_val = BinaryCRCCalc(binary_file_size);
    /*Calculate the CRC*/
    if (crc_val == -1)
    {
        printf("> Failed to calculate CRC checksum.\n");
        free(port);
        free(filePath);
        return -1;
    }
    printf("> CRC value is 0x%08X \n", crc_val);
    fflush(stdout);
    printf("> Patching the file... \n");
    fflush(stdout);
    /*Patch the binary file header with the size.*/
    if (BinaryPatchHeader(crc_val, (int)HEADER_CRC_OFFSET))
    {
        printf("> Failed to apply CRC checksum patch.\n");
        free(port);
        free(filePath);
        return -1;
    }
    printf("> CRC patch applied successfully.\n");
    fflush(stdout);

    /*Initializiing the com port handle.*/
    if (try_to_open_com_port(port))
    {
        free(port);
        free(filePath);
        return -1;
    }
    printf("> COM Port: %s opened successfully.\n", port);
    fflush(stdout);
    printf("> Trying to initialize DSD-i2 bootloader...\n");
    fflush(stdout);

    if (test_for_bootloader())
    {
        /*Send a reset in order to access the bootloader.*/
        if (reset_maple_method())
        {
            printf("> Failed to initialize DSD-i2 bootloader.\n");
            fflush(stdout);
            printf("> Please disconnect the DSD-i2 board and try again.\n");
            free(port);
            free(filePath);
            return -1;
        }
        else
        {
            if (try_to_close_com_port(port))
            {
                printf("> Failed to initialize DSD-i2 bootloader.\n");
                fflush(stdout);
                printf("> Please disconnect the DSD-i2 board and try again.\n");
                free(port);
                free(filePath);
                return -1;
            }
            if (try_to_open_com_port(port))
            {
                printf("> Failed to initialize DSD-i2 bootloader.\n");
                fflush(stdout);
                printf("> Please disconnect the DSD-i2 board and try again.\n");
                free(port);
                free(filePath);
                return -1;
            }
            if (test_for_bootloader())
            {
                printf("> Failed to initialize DSD-i2 bootloader.\n");
                fflush(stdout);
                printf("> Please disconnect the DSD-i2 board and try again.\n");
                free(port);
                free(filePath);
                return -1;
            }
        }
    }

    if (upload_to_stm(binary_file_size) || try_to_close_com_port(port) || BinaryFileClose())
    {
        free(port);
        free(filePath);
        return -1;
    }
    
    free(port);
    free(filePath);
    printf("> DSD-i2 board STM32L452RE chip flashed successfully.\n");
    return 0;
}


static int upload_to_stm(int binary_file_size)
{
    byte tx_buffer[4];

    /*Start the upload*/
    printf("> DSD-i2 bootloader initialized.\n");
    fflush(stdout);

    tx_buffer[0] = (byte)(binary_file_size >> 24) & 0xFF;
    tx_buffer[1] = (byte)(binary_file_size >> 16) & 0xFF;
    tx_buffer[2] = (byte)(binary_file_size >> 8) & 0xFF;
    tx_buffer[3] = (byte)(binary_file_size) & 0xFF;
    printf("> Sending the size of the binary file...\n");
    /*Send the file size*/
    if (ComPortWrite(tx_buffer, sizeof(tx_buffer)))
    {
        printf("> Failed to send the size.\n");
        return -1;
    }

    if (get_reply(50))
    {
        printf("> Failed to send the size.\n");
        return -1;
    }
    printf("> Size sent succesfully.\n");
    fflush(stdout);
    printf("> Erasing flash memory. Please wait...\n");
    fflush(stdout);
    if (get_reply(MAX_ERASE_TIME))
    {
        printf("> Failed to erase flash memory.\n");
        /*Close COM Port and binary file here.*/
        return -1;
    }

    printf("> Flash memory erased succesfully.\n");
    fflush(stdout);
    printf("> Program uploading. Please wait...\n");
    fflush(stdout);
    /*Send the file in chunks of 64 bytes*/
    int i = 0;
    byte data[64];

    for (;i < (binary_file_size / 64);i++)
    {
        /*Read the file and write the bytes to the local buffer*/
        if (BinaryFileRead(data, sizeof(data), i * 64))
        {
            printf("> Program upload failed. Couldn't read the binary file.\n");
            return -1;
        }
        /*Send the bytes to the MCU*/
        if (ComPortWrite(data, sizeof(data)))
        {
            printf("> Program upload failed. Couldn't send data to DSD-i2 board.\n");
            return -1;
        }
        if (get_reply(50))
        {
            printf("> Program upload failed. Couldn't receive reply.\n");
            return -1;
        }
        else
        {
            printf("> [=============%.2f%%=============]\r", (double)(((double)(i + 1) / (double)(binary_file_size / 64)) * 100));
            fflush(stdout);
        }
    }
    /*Leftover bytes*/
    if (binary_file_size % 64 != 0)
    {
        /*Read the file and write the bytes to the local buffer*/
        if (BinaryFileRead(data, binary_file_size % 64, i * 64))
        {
            printf("> Program upload failed. Couldn't read the binary file.\n");
            return -1;
        }
        /*Send the final bytes to the MCU*/
        if (ComPortWrite(data, binary_file_size % 64))
        {
            printf("> Program upload failed. Couldn't send data to DSD-i2 board.\n");
            return -1;
        }

        if (get_reply(50))
        {
            printf("> Program upload failed. Couldn't receive reply.\n");
            return -1;
        }
        else
        {
            printf("> [=============%.2f%%=============]\n", (double)100); //fancy way of printing 100.00%
            fflush(stdout);
        }
    }
    return 0;
}


static int test_for_bootloader(void)
{
    byte tx_buffer[4];
    /*Start the upload*/

    tx_buffer[0] = (byte)(STM_UPLOAD_PROGRAM >> 24) & 0xFF;
    tx_buffer[1] = (byte)(STM_UPLOAD_PROGRAM >> 16) & 0xFF;
    tx_buffer[2] = (byte)(STM_UPLOAD_PROGRAM >> 8) & 0xFF;
    tx_buffer[3] = (byte)(STM_UPLOAD_PROGRAM) & 0xFF;

    if (ComPortWrite(tx_buffer, sizeof(tx_buffer)))
    {
        return -1;
    }

    if (get_reply(100))
    {
        return 1;
    }
    return 0;
}