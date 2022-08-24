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


#include "virtual_com_port.h"

HANDLE virtual_com_port_h;
DCB virtual_com_port_conf;
DCB virtual_com_port_conf_original;
COMMTIMEOUTS virtual_com_port_timeouts;
COMSTAT virtual_com_port_status;

static int ComPortOpen(char* port, int baud_rate, int byte_size, int parity, int stop_bits);
static int ComPortClose();

static int ComPortOpen(char* port, int baud_rate, int byte_size, int parity, int stop_bits)
{
    memset(&virtual_com_port_h, 0, sizeof(virtual_com_port_h));
    /*Open the port*/
    virtual_com_port_h = CreateFileA(port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (virtual_com_port_h == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    /*Setting the timeouts*/
    memset(&virtual_com_port_timeouts, 0, sizeof(virtual_com_port_timeouts));
    virtual_com_port_timeouts.ReadIntervalTimeout = MAXDWORD;
    virtual_com_port_timeouts.ReadTotalTimeoutMultiplier = 0;
    virtual_com_port_timeouts.ReadTotalTimeoutConstant = 0;
    virtual_com_port_timeouts.WriteTotalTimeoutMultiplier = 0;
    virtual_com_port_timeouts.WriteTotalTimeoutConstant = 0;

    if (!SetCommTimeouts(virtual_com_port_h, &virtual_com_port_timeouts))
    {
        return -1;
    }

    /*Get the comm state and keep the settings*/
    if (!GetCommState(virtual_com_port_h, &virtual_com_port_conf_original))
    {
        return -1;
    }
    /*Get the comm state*/
    if (!GetCommState(virtual_com_port_h, &virtual_com_port_conf))
    {
        return -1;
    }

    /*Setting the configuration*/
    memset(&virtual_com_port_conf, 0, sizeof(virtual_com_port_conf));  /* clear the configuration  */
    virtual_com_port_conf.DCBlength = sizeof(virtual_com_port_conf);
    virtual_com_port_conf.BaudRate = baud_rate;
    virtual_com_port_conf.ByteSize = byte_size;
    virtual_com_port_conf.Parity = parity;
    virtual_com_port_conf.StopBits = stop_bits;
    virtual_com_port_conf.fBinary = TRUE;
    virtual_com_port_conf.fDtrControl = DTR_CONTROL_DISABLE;
    virtual_com_port_conf.fRtsControl = RTS_CONTROL_DISABLE;

    if (!SetCommState(virtual_com_port_h, &virtual_com_port_conf))
    {
        /*Shows a messagebox with the last error*/
#if 0
        wchar_t err[256];
        memset(err, 0, 256);
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 255, NULL);
        int msgboxID = MessageBoxW(NULL,
            err,
            (LPCWSTR)L"s",
            MB_OK);
#endif
        return -1;
    }
    memset(&virtual_com_port_status, 0, sizeof(virtual_com_port_status));
    return 0;
}

static int ComPortClose()
{
    FlushFileBuffers(virtual_com_port_h);
    SetCommState(virtual_com_port_h, &virtual_com_port_conf_original);
    if (virtual_com_port_h == NULL || !CloseHandle(virtual_com_port_h))
    {
        return -1;
    }
    memset(&virtual_com_port_h, 0, sizeof(virtual_com_port_h));
    memset(&virtual_com_port_conf, 0, sizeof(virtual_com_port_conf));
    memset(&virtual_com_port_timeouts, 0, sizeof(virtual_com_port_timeouts));
    memset(&virtual_com_port_status, 0, sizeof(virtual_com_port_status));
    //Sleep(400);
    //delay_s(400);
    return 0;
}

int ComPortAvailable()
{
    if (ClearCommError(virtual_com_port_h, NULL, &virtual_com_port_status))
    {
        return (int)virtual_com_port_status.cbInQue;
    }
    else
    {
        return -1;
    }
}

int ComPortWrite(byte* data, int size)
{
    int num_of_bytes_written;

    if (!WriteFile(virtual_com_port_h, data, size, &num_of_bytes_written, NULL))
    {
        return -1;
    }
    return 0;
}

int ComPortRead(byte* data, int size)
{
    int num_of_bytes_read;
    if (!ReadFile(virtual_com_port_h, data, size, &num_of_bytes_read, NULL))
    {
        return -1;
    }

    return 0;
}

int reset_maple_method()
{
    /*Setting the initial values of DTR and RTS*/
    EscapeCommFunction(virtual_com_port_h, SETRTS);
    EscapeCommFunction(virtual_com_port_h, SETDTR);

    /*Maple restart sequence.*/
    EscapeCommFunction(virtual_com_port_h, CLRRTS);
    EscapeCommFunction(virtual_com_port_h, CLRDTR);
    EscapeCommFunction(virtual_com_port_h, SETDTR);
    Sleep(50);
    EscapeCommFunction(virtual_com_port_h, CLRDTR);
    EscapeCommFunction(virtual_com_port_h, SETRTS);
    EscapeCommFunction(virtual_com_port_h, SETDTR);
    Sleep(50);
    EscapeCommFunction(virtual_com_port_h, CLRDTR);
    Sleep(50);

    if (ComPortWrite("1EAF", sizeof("1EAF") - 1))
    {
        return -1;
    }
    /*flushing the buffers seems to help*/
    return 0;
}

int FlushBuf()
{
    if (!FlushFileBuffers(virtual_com_port_h))
        return -1;
    else
        return 0;
}

int reset_bootloader()
{
    for (int i = 0;i < 4;i++)
    {
        EscapeCommFunction(virtual_com_port_h, SETRTS);
        EscapeCommFunction(virtual_com_port_h, SETDTR);
        Sleep(50);
        EscapeCommFunction(virtual_com_port_h, CLRRTS);
        EscapeCommFunction(virtual_com_port_h, SETDTR);
    }
    Sleep(100);
    byte rst_cmd = 0x99;
    if (ComPortWrite(&rst_cmd, sizeof(rst_cmd)))
    {
        return -1;
    }
    return 0;
}

int try_to_open_com_port(char* port)
{
    int num_of_retries = 0;

    do
    {
        if (ComPortOpen(port, 115200, 8, NOPARITY, ONESTOPBIT))
        {
            if (GetLastError() == ERROR_ACCESS_DENIED)
            {
                try_to_close_com_port(port);
            }
            else
            {
                Sleep(1);
                num_of_retries++;
            }
        }
        else
        {
            return 0;
        }
    } while (num_of_retries < 400);
    return -1;
}

int try_to_close_com_port(char* port)
{
    if (ComPortClose())
    {
        return -1;
    }
    return 0;
}