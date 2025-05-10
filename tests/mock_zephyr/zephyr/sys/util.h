#pragma once

#define CONTAINER_OF(ptr, type, field) \
    ((type *)((char *)(ptr) - (uintptr_t)&(((type *)0)->field)))
