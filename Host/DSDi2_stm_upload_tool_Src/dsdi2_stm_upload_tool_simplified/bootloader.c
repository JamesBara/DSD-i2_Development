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

#include<windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "virtual_com_port.h"
#include "binary_file_processing.h"
#include "main.h"
#include "bootloader.h"



int stm_upload(int binary_file_size)
{  
    byte size[4];
    byte cmd[4];

    cmd[0] = (byte)(STM_UPLOAD_PROGRAM >> 24) & 0xFF;
    cmd[1] = (byte)(STM_UPLOAD_PROGRAM >> 16) & 0xFF;
    cmd[2] = (byte)(STM_UPLOAD_PROGRAM >> 8) & 0xFF;
    cmd[3] = (byte)(STM_UPLOAD_PROGRAM) & 0xFF;

    if (ComPortWrite(cmd, sizeof(cmd)))
    {

        return -1;
    }

    if (get_reply())
    {
        return -1;
    }
    printf("> DSD-i2 bootloader initialized.\n");
    fflush(stdout);

    size[0] = (byte)(binary_file_size >> 24) & 0xFF;
    size[1] = (byte)(binary_file_size >> 16) & 0xFF;
    size[2] = (byte)(binary_file_size >> 8) & 0xFF;
    size[3] = (byte)(binary_file_size) & 0xFF;

    /*Send the file size*/
    if (ComPortWrite(size, sizeof(size)))
    {
        return -1;
    }

    if (get_reply())
    {
        return -1;
    }
    printf("> Erasing flash memory. Please wait...\n");
    fflush(stdout);
    if (get_reply())
    {
        return -1;
    }

    printf("> Flash memory erased succesfully.\n");
    fflush(stdout);
    printf("> Program uploading. Please wait...\n");
    fflush(stdout);
    /*Send the file in chunks of 64 bytes*/
    int i;
    byte data[64];

    for (i = 0;i < (binary_file_size / 64);i++)
    {
        /*Read the file and write the bytes to the local buffer*/
        if (BinaryFileRead(data, sizeof(data), i * 64))
        {
            return -1;
        }
        /*Send the bytes to the MCU*/
        if (ComPortWrite(data, sizeof(data)))
        {
            return -1;
        }
        if (get_reply())
        {
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
            return -1;
        }
        /*Send the final bytes to the MCU*/
        if (ComPortWrite(data, binary_file_size % 64))
        {
            return -1;
        }

        if (get_reply())
        {
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