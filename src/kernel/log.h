#ifndef LOG_H
#define LOG_H
typedef enum {
    LOG_INFO,
    LOG_OK,
    LOG_ERROR
} log_level_t;
void klog(log_level_t level, const char* msg);
void khex(log_level_t level, uint32_t n);
void kprintf_hex(uint32_t n);
static inline void kprinterr(const char* msg) {
    klog(LOG_ERROR, msg);
}
static inline void kprintinfo(const char* msg) {
    klog(LOG_INFO, msg);
}
static inline void kprintok(const char* msg) {
    klog(LOG_OK, msg);
}
static inline void kprinterrhex(uint32_t n) {
    khex(LOG_ERROR, n);
}
static inline void kprintinfohex(uint32_t n) {
    khex(LOG_INFO, n);
}
static inline void kprintokhex(uint32_t n) {    
    khex(LOG_OK, n);
}
#endif