#pragma once
const int DEFAULT_LOG_BAUDRATE = 115200;

void log_init(int baudrate = DEFAULT_LOG_BAUDRATE);

const int LOG_LEVEL_DEBUG = 0;
const int LOG_LEVEL_INFO  = 1;
const int LOG_LEVEL_WARN  = 2;
const int LOG_LEVEL_ERROR = 3;  

void log_printf(int level, const char* format, ...);
int set_log_level(int level);
int get_log_level();    