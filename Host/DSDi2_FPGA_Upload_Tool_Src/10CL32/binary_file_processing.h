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

#ifndef INC_BINARY_FILE_PROCESSING_H_
#define INC_BINARY_FILE_PROCESSING_H_



#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) int BinaryFileOpen(char* file_path);
	__declspec(dllexport) int BinaryFileClose(void);
	__declspec(dllexport) int BinaryGetSize(void);
	__declspec(dllexport) int BinaryFileRead(byte* data, int size, int offset);
	__declspec(dllexport) int SetBinaryPointer(int offset);
	__declspec(dllexport) int ResetBinaryPointer(void);


#ifdef __cplusplus
}
#endif
#endif /* INC_BINARY_FILE_PROCESSING_H_ */
