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
#include <stdbool.h>
#include "virtual_com_port.h"
#include "binary_file_processing.h"
#include "10cl_conf.h"
#if 0
static int get_reply(void);

static int get_reply()
{
    byte ans=0;

    if (ComPortRead(&ans, sizeof(ans)) || ans != (byte)ACK)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int cl_init(char* port)
{
    int res = FlushBuf();

    bool is_app_err = false;
    /*We do 5 repeated checks with 50ms delay between them to make sure that we are not in bootloader mode
    * and the user application has started normally. If we receive an RDY_TO_RCV from host then there is an
    * issue with the users application and we have to flash a new one.
    */
    for (int i = 0; i < 5;i++)
    {            
        byte ans = 0;
        if (ComPortAvailable() > 0)
        {
            if (ComPortRead(&ans, sizeof(ans)))
            {
                return -1;
            }
        }
        if (ans == (byte)RDY_TO_RCV)
        {
            /*Not in application mode*/
            is_app_err = true;
            break;
        }
        else
        {
            Sleep(50);
        }
    }

    if (!is_app_err) {

        /*Call the reset command*/
        if (reset_mapple_method())
        {
            return -1;
        }
        if (ComPortOpen(port, 115200, 8, NOPARITY, ONESTOPBIT))
        {
            return -1;
        }
    }
#if 0
    /*Using while here could potentially cause the program to get stuck in the loop.*/
    while (1)
    {
#endif
        byte ans=0;
        if (ComPortRead(&ans, sizeof(ans)))
        {
            return -1;
        }        
        if (ans == (byte)RDY_TO_RCV)
        {
            byte cmd[4];

            cmd[0] = (byte)(INIT_PS >> 24) & 0xFF;
            cmd[1] = (byte)(INIT_PS >> 16) & 0xFF;
            cmd[2] = (byte)(INIT_PS >> 8) & 0xFF;
            cmd[3] = (byte)(INIT_PS) & 0xFF;

            if (ComPortWrite(cmd, sizeof(cmd)))
            {
                return -1;
            }
            
            return get_reply();
        }
        else
        {
#if 0
            Sleep(50);
#endif
            return -1;
        }
#if 0
    }
#endif
}

int cl_begin_upload(int file_size)
{
#if 0
    /*Using while here could potentially cause the program to get stuck in the loop.*/
    while (1)
    {
#endif
        byte ans=0;
        if (ComPortRead(&ans, sizeof(ans)))
        {
            return -1;
        }
        if (ans == (byte)PS_RDY)
        {
            byte cmd = (byte)PS_PROGRAM;
            byte size[4];

            if (ComPortWrite(&cmd, sizeof(cmd)))
            {
                return -1;
            }

            if (get_reply())
            {
                return -1;
            }

            size[0] = (byte)(file_size >> 24) & 0xFF;
            size[1] = (byte)(file_size >> 16) & 0xFF;
            size[2] = (byte)(file_size >> 8) & 0xFF;
            size[3] = (byte)(file_size) & 0xFF;

            /*Send the file size*/
            if (ComPortWrite(size, sizeof(size)))
            {
                return -1;
            }

            if (get_reply())
            {
                return -1;
            }
            /*Wait for FPGA to acknowledge the start of programming*/
            return get_reply();
        }
        else
        {
#if 0;
            Sleep(50);
#endif
            return -1;
        }
#if 0
    }
#endif
}

/*read the selected binary file data and upload a chunk to the mcu*/
int upload_chunk(int chunk_size, int offset)
{
    byte data[64];

    /*Read the file and write the bytes to the local buffer*/
    if (BinaryFileRead(data, chunk_size, offset))
    {
        return -1;
    }
    /*Send the bytes to the MCU*/
    if (ComPortWrite(data, chunk_size))
    {
        return -1;
    }
    return get_reply();
}

int cl_finished_upload(void)
{
#if 0
    int no_of_bytes = 0;

    while (!no_of_bytes)
    {
    no_of_bytes = ComPortAvailable();
    }      
       
        if (no_of_bytes > 0)
        {
            byte* ans = calloc(no_of_bytes, sizeof(*ans));
            if (ans == NULL)
            {

            }
            if (ComPortRead(ans, sizeof(ans)))
            {
                return -1;
            }
        }
#endif
    //ComPortRead(ans, sizeof(ans));
    /*Wait for CONF_DONE*/
    /*Wait for INIT_DONE*/
    return get_reply();
}

int enable_fpga_console(void)
{
    /*Using while here could potentially cause the program to get stuck in the loop.*/
    while (1)
    {
        byte ans=0;
        if (ComPortRead(&ans, sizeof(ans)))
        {
            return -1;            
        }

        if (ans == (byte)PS_RDY)
        {
            byte cmd = (byte)FPGA_COM;
            if (ComPortWrite(&cmd, sizeof(cmd)))
            {
                return -1;
            }
            else
            {
                return get_reply();
            }
        }
        else
        {
            /*a small delay*/
            Sleep(50);
        }
    }
}
#endif