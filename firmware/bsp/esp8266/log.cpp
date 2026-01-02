// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

#include <Arduino.h>
#include <bsp/log.h>
#include <cstdarg>
#include <cstdio>

#ifdef SERIAL_DEBUG

static bool is_initialized = false;
static int current_log_level = LOG_LEVEL_INFO;

void log_init(int baudrate)
{
    if (is_initialized) return;
    Serial.begin(baudrate);
    is_initialized = true;
}

int set_log_level(int level)
{
    int old = current_log_level;
    if (level >= LOG_LEVEL_DEBUG && level <= LOG_LEVEL_ERROR)
        current_log_level = level;
    return old;
}

int get_log_level()
{
    return current_log_level;
}

void log_printf(int level, const char* format, ...)
{
    if (level < LOG_LEVEL_DEBUG || level > LOG_LEVEL_ERROR)
        return;

    // only log messages with severity >= current threshold
    if (level < current_log_level)
        return;

    if (!is_initialized)
        log_init(DEFAULT_LOG_BAUDRATE);

    const char* prefix =
        (level == LOG_LEVEL_DEBUG) ? "[DEBUG] " :
        (level == LOG_LEVEL_INFO)  ? "[INFO] "  :
        (level == LOG_LEVEL_WARN)  ? "[WARN] "  :
                                     "[ERROR] ";

    Serial.print(prefix);

    char buf[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    Serial.println(buf);
}

#else

// compiled-out logger for production

void log_init(int) {}
int set_log_level(int level) { return level; }
int get_log_level() { return LOG_LEVEL_ERROR; }
void log_printf(int, const char*, ...) {}

#endif
