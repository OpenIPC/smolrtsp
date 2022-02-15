#pragma once

#define CHK_WRITE_ERR(result, ret)                                                                 \
    do {                                                                                           \
        if (ret < 0) {                                                                             \
            return result;                                                                         \
        } else {                                                                                   \
            result += ret;                                                                         \
        }                                                                                          \
    } while (0)
