#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "edge-impulse-sdk/porting/ei_classifier_porting.h"

EI_IMPULSE_ERROR ei_run_impulse_check_canceled() {
    return EI_IMPULSE_OK;
}

void ei_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void ei_printf_float(float f) {
    printf("%f", f);
}

void *ei_malloc(size_t size) {
    return malloc(size);
}
void *ei_calloc(size_t nitems, size_t size) {
    return calloc(nitems, size);
}

void ei_free(void *ptr) {
    free(ptr);
}

#include "esp_timer.h"

uint64_t ei_read_timer_us() {
    return esp_timer_get_time();
}

uint64_t ei_read_timer_ms() {
    return esp_timer_get_time() / 1000;
}
