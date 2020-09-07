//
// Created by Michal_Marszalek on 11.08.2020.
//

#include "OSLogSink.h"

#if defined(_MSC_VER) || defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004

#include <Windows.h>

#endif

GameApi::OSLogSink::OSLogSink() {
#if defined(_MSC_VER) || defined(_WIN32)
    HANDLE console_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (console_output_handle == INVALID_HANDLE_VALUE) {
        return;
    }

    DWORD original_output_mode;
    if (!GetConsoleMode(console_output_handle, &original_output_mode)) {
        return;
    }

    DWORD request_output_mode = original_output_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(console_output_handle, request_output_mode);
#endif
}

void GameApi::OSLogSink::outputLogMessage(const GameApi::Message &message) {
    std::puts(std::string{message}.c_str());
    std::fflush(stdout);
}


