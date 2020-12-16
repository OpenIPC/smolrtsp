#ifndef SMOLRTSP_AUX_H
#define SMOLRTSP_AUX_H

#include <stdbool.h>

#define SUB_SLICE(size, data, len)                                                                 \
    do {                                                                                           \
        size -= len;                                                                               \
        data += len;                                                                               \
    } while (false)

#define OBJ(type, expr) ((type *)(type[]){expr})

#endif // SMOLRTSP_AUX_H
