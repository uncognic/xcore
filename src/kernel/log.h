#ifndef LOG_H
#define LOG_H
typedef enum {
    LOG_INFO,
    LOG_OK,
    LOG_ERROR
} log_level_t;
void klog(log_level_t level, const char* msg);

#endif