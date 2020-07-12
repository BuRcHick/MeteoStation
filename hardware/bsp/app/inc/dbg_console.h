#ifndef dbg_console_h
#define dbg_console_h
#include "main.h"

extern "C" {
    void startDbgConsole(UART_HandleTypeDef*);
}

#endif //dbg_console_h