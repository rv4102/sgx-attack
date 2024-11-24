#include "Enclave_t.h"

#define NUM_TRACES 100000
#define NUM_SAMPLES 1000

void ecall_1(long event_fd) {
    for(int outer = 0; outer < NUM_TRACES; outer++) {
        double a = 0.6128, b = 0.3912;
        __asm__ __volatile__(
            "FLD %0"
            :
            : "m" (a)
        );
        __asm__ __volatile__(
            "FLD %0"
            :
            : "m" (b)
        );
        enable_perf_event(event_fd);
        for(int inner = 0; inner < NUM_SAMPLES; inner++){
            __asm__ __volatile__("FADD ST(0), ST(1)");
        }
        disable_perf_event(event_fd);
        read_perf_event(event_fd);
    }
}

void ecall_2(long event_fd) {
    for(int outer = 0; outer < NUM_TRACES; outer++){
        uint32_t a = 2, b = 5;
        __asm__ __volatile__(
            "MOV eax, %0"
            :
            : "r" (a)
            : "eax"
        );
        __asm__ __volatile__(
            "MOV ebx, %0"
            :
            : "r" (b)
            : "ebx"
        );
        enable_perf_event(event_fd);
        for(int inner = 0; inner < NUM_SAMPLES; inner++){
            __asm__ __volatile__("ADD eax, ebx");
        }
        disable_perf_event(event_fd);
        read_perf_event(event_fd);
    }
}