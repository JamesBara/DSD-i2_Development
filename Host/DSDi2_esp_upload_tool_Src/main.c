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

bool port_is_open = false;
static int get_reply(int reply_timeout);
static int test_for_bootloader(void);


static int get_reply(int reply_timeout)
{
    
    int start_tick = GetTickCount64();
    int elapsed_millisecs = 0;

    while (elapsed_millisecs < reply_timeout)
    {

        if (ComPortAvailable() > 0)
        {
            break;
        }
        elapsed_millisecs = GetTickCount64() - start_tick;
    }

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
    SetConsoleOutputCP(1253);

    if (argc = !7)
    {
        printf("> Wrong number of arguments.\n");
        return -1;
    }

    if (argc = !2)
    {

        return -1;
    }

    if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL || argv[4] == NULL || argv[5] == NULL || argv[6] == NULL || argv[7] == NULL)
    {
        printf("> No arguments found.\n");
        return -1;
    }
    printf("> Welcome to the upload tool for DSD-i2 board for Arduino.\n");
    fflush(stdout);
    /*Figure out the total length of the arguments that we need to send to the esptool.*/
    long len = strlen(argv[1])+1 + strlen(" --chip esp32c3 --port ") + strlen(argv[2])+1 +
        strlen(" --baud 115200 --before no_reset --after no_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0xe000 ") +
        strlen(argv[3])+1 + strlen(" ") + strlen(argv[4])+1 + strlen(" ") + strlen(argv[5])+1 + strlen(" 0x10000 ") + strlen(argv[6])+1 + strlen(" 0x8000 ") +
        strlen(argv[7])+1;
    /*Create a big char array out of those arguments.*/
    char* esp_args = calloc(len, sizeof(*esp_args));
    if (esp_args == NULL)
    {
        printf("> Memory allocation error.\n");
        return -1;
    }
    /*Finally insert those arguments to the char array.*/
    strcpy(esp_args, argv[1]);
    strcat(esp_args, " --chip esp32c3  --port ");
    strcat(esp_args, argv[2]);
    strcat(esp_args, " --baud 115200 --before no_reset --after no_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0xe000 "); /*add --trace option for debugging*/
    strcat(esp_args, argv[3]);
    strcat(esp_args, " ");
    strcat(esp_args, argv[4]);
    strcat(esp_args, " ");
    strcat(esp_args, argv[5]);
    strcat(esp_args, " 0x10000 ");
    strcat(esp_args, argv[6]);
    strcat(esp_args, " 0x8000 ");
    strcat(esp_args, argv[7]);

    //printf("%s", esp_args);

    char* port;
    /*If the COM port number is >9 use \\.\ before the COM port*/
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

    /*Initializiing the com port handle.*/
    if (try_to_open_com_port(port))
    {
        free(esp_args);
        free(port);        
        return -1;
    }
    printf("> COM Port: %s opened successfully.\n", port);
    fflush(stdout);
    printf("> Trying to initialize DSD-i2 bootloader...\n");
    fflush(stdout);
    int res = test_for_bootloader();
    if (res == -1)
    {
        printf("> Failed to initialize DSD-i2 bootloader.\n");
        fflush(stdout);
        printf("> Please disconnect the DSD-i2 board and try again.\n");
        return -1;
    }
    else if (res)
    {
        /*Send a reset in order to access the bootloader.*/
        if (reset_maple_method())
        {
            printf("> Failed to initialize DSD-i2 bootloader.\n");
            fflush(stdout);
            printf("> Please disconnect the DSD-i2 board and try again.\n");
            free(esp_args);
            free(port);
            return -1;
        }
        else
        {
            if (try_to_close_com_port(port))
            {
                printf("> Failed to initialize DSD-i2 bootloader.\n");
                fflush(stdout);
                printf("> Please disconnect the DSD-i2 board and try again.\n");
                free(esp_args);
                free(port);
                return -1;
            }
            if (try_to_open_com_port(port))
            {
                printf("> Failed to initialize DSD-i2 bootloader.\n");
                fflush(stdout);
                printf("> Please disconnect the DSD-i2 board and try again.\n");
                free(esp_args);
                free(port);
                return -1;
            }
            if (test_for_bootloader())
            {
                printf("> Failed to initialize DSD-i2 bootloader.\n");
                fflush(stdout);
                printf("> Please disconnect the DSD-i2 board and try again.\n");
                free(esp_args);
                free(port);
                return -1;
            }
        }
    }

    /*Start the upload*/
    if (try_to_close_com_port(port))
    {
        free(esp_args);
        free(port);
        return -1;
    }

    printf("> DSD-i2 bootloader initialized.\n");
    fflush(stdout);
    //Sleep(500); //Give time for ESP32-C3 to enter download mode.
    res = system(esp_args);    
    fflush(stdout);

    if (try_to_open_com_port(port))
    {
        printf("> Failed to reset the board. please try to reset the board manually.\n");
        fflush(stdout);
        printf("> Please try to reset the DSD-i2 board manually, by using the ESP_EN and STM_RST buttons.\n");
        free(esp_args);
        free(port);
        return -1;
    }
    if (reset_bootloader())
    {
        try_to_close_com_port(port);
        printf("> Failed to reset the board. please try to reset the board manually.\n");
        fflush(stdout);
        printf("> Please try to reset the DSD-i2 board manually, by using the ESP_EN and STM_RST buttons.\n");
        free(esp_args);
        free(port);
        return -1;
    }
    if (try_to_close_com_port(port))
    {
        printf("> Failed to reset the board. please try to reset the board manually.\n");
        fflush(stdout);
        printf("> Please try to reset the DSD-i2 board manually, by using the ESP_EN and STM_RST buttons.\n");
        free(esp_args);
        free(port);
        return -1;
    }

    if (res)
    {
        printf("> Failed to flash ESP32-C3 please try again.\n");
    }
    else
    {
        printf("> DSD-i2 board ESP32-C3 chip flashed successfully.\n");
    }
    
    free(esp_args);
    free(port);
    return 0;

}

static int test_for_bootloader(void)
{
    byte tx_buffer[4];


    tx_buffer[0] = (byte)(ESP_UPLOAD_PROGRAM >> 24) & 0xFF;
    tx_buffer[1] = (byte)(ESP_UPLOAD_PROGRAM >> 16) & 0xFF;
    tx_buffer[2] = (byte)(ESP_UPLOAD_PROGRAM >> 8) & 0xFF;
    tx_buffer[3] = (byte)(ESP_UPLOAD_PROGRAM) & 0xFF;

    if (ComPortWrite(tx_buffer, sizeof(tx_buffer)))
    {
        return -1;
    }

    if (get_reply(50))
    {
        return 1;
    }
    return 0;
}
