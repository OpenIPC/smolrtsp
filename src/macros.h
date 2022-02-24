#pragma once

#include <stddef.h>

#define CHK_WRITE_ERR(result, call)                                            \
    do {                                                                       \
        ssize_t CHK_WRITE_ERR_ret = call;                                      \
        if (CHK_WRITE_ERR_ret < 0) {                                           \
            return result;                                                     \
        } else {                                                               \
            result += CHK_WRITE_ERR_ret;                                       \
        }                                                                      \
    } while (0)
