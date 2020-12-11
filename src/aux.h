#ifndef SMOLRTSP_AUX_H
#define SMOLRTSP_AUX_H

#include <stdbool.h>

#define CRLF "\r\n"

#define CAT(x, y)           PRIMITIVE_CAT(x, y)
#define PRIMITIVE_CAT(x, y) x##y

#define STRINGIFY(x)           PRIMITIVE_STRINGIFY(x)
#define PRIMITIVE_STRINGIFY(x) #x

#define SUB_SLICE(size, data, len)                                                                 \
    do {                                                                                           \
        size -= len;                                                                               \
        data += len;                                                                               \
    } while (false)

#define OBJ(type, expr) ((type *)(type[]){expr})

#endif // SMOLRTSP_AUX_H
