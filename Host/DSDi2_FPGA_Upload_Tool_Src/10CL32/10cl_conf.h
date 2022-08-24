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

#ifndef INC_10CL_ESP32_CONF_H_
#define INC_10CL_ESP32_CONF_H_

/*Initialization sequences*/
#define FPGA_PS_CONF 0xF1A541C1

#define ACK 0x79


#ifdef __cplusplus
extern "C" {
#endif
#if 0
	__declspec(dllexport) int cl_init(char* port);
	__declspec(dllexport) int cl_begin_upload(int file_size);
	__declspec(dllexport) int cl_finished_upload(void);
	__declspec(dllexport) int upload_chunk(int chunk_size, int offset);
	__declspec(dllexport) int enable_fpga_console(void);
#endif 
	
#ifdef __cplusplus
}
#endif
#endif /* INC_10CL_ESP32_CONF_H_ */
