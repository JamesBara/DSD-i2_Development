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

#include "binary_file_processing.h"

static HANDLE binary_file_h;

int BinaryFileOpen(char* file_path)
{
    memset(&binary_file_h, 0, sizeof(binary_file_h));
    /*Open the file*/
    binary_file_h = CreateFileA(file_path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (binary_file_h == INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    return 0;
}

int BinaryGetSize(void)
{
    LARGE_INTEGER  binary_file_size_li;

    if (GetFileSizeEx(binary_file_h, &binary_file_size_li) == 0)
    {
        return -1;
    }

    return binary_file_size_li.LowPart;
}


int BinaryFileClose()
{
    if (binary_file_h == NULL || !CloseHandle(binary_file_h))
    {
        return -1;
    }
    memset(&binary_file_h, 0, sizeof(binary_file_h));
    Sleep(400);
    return 0;
}

int SetBinaryPointer(int offset)
{
    LARGE_INTEGER int_to_li;
    int_to_li.QuadPart = offset;
    /*Set the file pointer*/
    if (!SetFilePointerEx(binary_file_h, int_to_li, NULL, FILE_BEGIN))
    {
        return -1;
    }
    return 0;
}

int ResetBinaryPointer()
{
    /*Reset the file pointer*/
    if (SetBinaryPointer(0))
    {
        return -1;
    }
    return 0;
}

int BinaryFileRead(byte* data, int size, int offset)
{
    /*Set the file pointer*/
    if (SetBinaryPointer(offset))
    {
        return -1;
    }

    int num_of_bytes_read;
    if (!ReadFile(binary_file_h, data, size, &num_of_bytes_read, NULL))
    {
        return -1;
    }

    return 0;
}

int BinaryPatchHeader(int value, int offset)
{
    byte val[4];

    /* Vector table length is 404 bytes (0x194) for STM32L452RE.
    * The header bytes that store the size of the bin file
    * reside in the 4 bytes after the vector table, and the crc
    * value 4 bytes after that.
    * STM32L452RE is little endian so apply the patch accordingly.
    */
    val[0] = (byte)(value) & 0xFF;
    val[1] = (byte)(value >> 8) & 0xFF;
    val[2] = (byte)(value >> 16) & 0xFF;
    val[3] = (byte)(value >> 24) & 0xFF;

    if (SetBinaryPointer(offset))
    {
        return -1;
    }

    int num_of_bytes_written;
    if (!WriteFile(binary_file_h, val, sizeof(val), &num_of_bytes_written, NULL))
    {
        return -1;
    }
    /*Reset the file pointer*/
    ResetBinaryPointer();
    return 0;
}

/*Size patch should be applied before checksum calculation*/
/*Calculating the CRC32 */
int BinaryCRCCalc(int binary_file_size)
{
    uint32_t offset = 0;
    uint8_t buf[4];
    uint32_t data;
    uint32_t crc_val = (int)DEFAULT_CRC_INITVALUE;
    uint32_t xbit;
    uint32_t size_no_header = (binary_file_size - 0x19C);

    /*Calculate CRC for the vector table bytes*/
    for (uint32_t i = 0;i < (0x198 / 4);i++)
    {
        if (BinaryFileRead(buf, sizeof(buf), offset))
        {
            return -1;
        }

        data = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];

        xbit = 0x80000000;
        for (uint32_t bits = 0; bits < 32; bits++)
        {
            if (crc_val & 0x80000000)
            {
                crc_val <<= 1;
                crc_val ^= (uint32_t)DEFAULT_CRC32_POLY;
            }
            else
            {
                crc_val <<= 1;
            }
            if (data & xbit)
            {
                crc_val ^= (uint32_t)DEFAULT_CRC32_POLY;
            }
            xbit >>= 1;
        }
        offset += 4;
        memset(buf, 0, sizeof(buf));
        data = 0;
    }

    /*We break the calculation in 2 parts to bypass the 4 bytes that will store the CRC value*/
    /*Set the pointer offset*/
    offset = 0x19C;
    /*Calculate CRC for the rest of the binary file without the header, using the already calculated crc value as the init value*/

    for (uint32_t i = 0;i < (size_no_header / 4);i++)
    {
        /*Set the file pointer*/
        if (BinaryFileRead(buf, sizeof(buf), offset))
        {
            return -1;
        }

        data = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];

        xbit = 0x80000000;
        for (uint32_t bits = 0; bits < 32; bits++)
        {
            if (crc_val & 0x80000000)
            {
                crc_val <<= 1;
                crc_val ^= (uint32_t)DEFAULT_CRC32_POLY;
            }
            else
            {
                crc_val <<= 1;
            }
            if (data & xbit)
            {
                crc_val ^= (uint32_t)DEFAULT_CRC32_POLY;
            }
            xbit >>= 1;
        }
        offset += 4;
        memset(buf, 0, 4);
        data = 0;
    }
    /*Handle the extra bytes*/
    if ((size_no_header % 4) != 0)
    {
        if ((size_no_header % 4) == 1)
        {
            data = ((uint32_t)buf[0] << 24) & 0xFF000000;
        }
        else if ((size_no_header % 4) == 2)
        {
            data = (((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16)) & 0xFFFF0000;
        }
        else if ((size_no_header % 4) == 3)
        {
            data = (((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8)) & 0xFFFFFF00;
        }
        xbit = 0x80000000;
        for (uint32_t bits = 0; bits < 32; bits++)
        {
            if (crc_val & 0x80000000)
            {
                crc_val <<= 1;
                crc_val ^= (uint32_t)DEFAULT_CRC32_POLY;
            }
            else
            {
                crc_val <<= 1;
            }
            if (data & xbit)
            {
                crc_val ^= (uint32_t)DEFAULT_CRC32_POLY;
            }
            xbit >>= 1;
        }
    }
    ResetBinaryPointer();
    return crc_val;
}

#if 0 
/*STM32 algorith found in https://www.codetd.com/en/article/10243425 */
uint32_t crc32_st(uint32_t* pbuf, uint32_t size)
{
    const uint32_t st_const_value = 0x04c11db7;
    uint32_t	crc_value = 0xffffffff;
    uint32_t	xbit;
    uint32_t 	bits;
    uint32_t	i;

    for (i = 0; i < size; i++)
    {
        xbit = 0x80000000;
        for (bits = 0; bits < 32; bits++)
        {
            if (crc_value & 0x80000000)
            {
                crc_value <<= 1;
                crc_value ^= st_const_value;
            }
            else
            {
                crc_value <<= 1;
            }
            if (pbuf[i] & xbit)
            {
                crc_value ^= st_const_value;
            }
            xbit >>= 1;
        }
    }
    return crc_value;
}
#endif