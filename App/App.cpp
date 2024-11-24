#include <stdio.h>
#include <iostream>
#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <asm/unistd.h>
#include <sys/syscall.h>
#include <xmmintrin.h>
#include <pmmintrin.h>
#include <inttypes.h>

// #define CONFIG 0x01B1 // UOPS_EXECUTED.THREAD
#define CONFIG 0X10B1 // UOPS_EXECUTED.X87

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

static long perf_event_open(
    struct perf_event_attr *hw_event,
    pid_t pid,
    int cpu,
    int group_fd,
    unsigned long flags
    ) {

	int ret;
	ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
	return ret;
}

// OCall implementations
void enable_perf_event(long event_fd) {
    ioctl(event_fd, PERF_EVENT_IOC_RESET, 0);
	ioctl(event_fd, PERF_EVENT_IOC_ENABLE, 0);
}

void disable_perf_event(long event_fd) {
    ioctl(event_fd, PERF_EVENT_IOC_DISABLE, 0);
}

void read_perf_event(long event_fd) {
    long long event_count;
    ssize_t status = read(event_fd, &event_count, sizeof(long long));
    if(status == -1) {
        printf("Unable to call read\n");
        close(event_fd);
        exit(1);
    }
	fprintf(stdout, "%lld\n", event_count);
}

int main(int argc, char const *argv[]) {
    if (initialize_enclave(&global_eid, "enclave.token", "enclave.signed.so") < 0) {
        std::cout << "Fail to initialize enclave." << std::endl;
        return 1;
    }

    struct perf_event_attr event;
	int event_fd;
	memset(&event, 0, sizeof(struct perf_event_attr));
	event.type = PERF_TYPE_RAW;
	event.size = sizeof(struct perf_event_attr);
	event.config = CONFIG;
	event.disabled = 1;
	event.exclude_kernel = 1;
	event.exclude_hv = 1;
	event_fd = perf_event_open(&event, 0, -1, -1, 0);
	assert(event_fd > 0);

    // int ptr;
    if(argc == 2 && strcmp(argv[1], "1") == 0) {
        sgx_status_t status = ecall_1(global_eid, event_fd);
        // std::cout << status << std::endl;
        if (status != SGX_SUCCESS) {
            std::cout << "noob" << std::endl;
        }
    } else {
        sgx_status_t status = ecall_2(global_eid, event_fd);
        // std::cout << status << std::endl;
        if (status != SGX_SUCCESS) {
            std::cout << "noob" << std::endl;
        }
    }


    // printf("Random number: %d\n", ptr);

    return 0;
}
