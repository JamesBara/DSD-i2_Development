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

HANDLE binary_file_h;

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
