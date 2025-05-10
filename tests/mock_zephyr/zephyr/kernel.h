#ifndef MOCK_ZEPHYR_KERNEL_H
#define MOCK_ZEPHYR_KERNEL_H

#include <cstdint>

// ===== Mock k_work struct and related APIs =====

struct k_work {
    void (*handler)(struct k_work*);
};

// Dummy time functions
inline int64_t k_uptime_get() {
    extern int64_t fake_time;
    return fake_time;
}

inline uint32_t k_uptime_get_32() {
    extern int64_t fake_time;
    return static_cast<uint32_t>(fake_time);
}

#endif // MOCK_ZEPHYR_KERNEL_H
