#ifndef SMOLRTSP_HEADER_H
#define SMOLRTSP_HEADER_H

#include <writer.h>

#include <stddef.h>

typedef struct {
    const char *(*key)(void);
    void (*write_value)(const void *self, SmolRTSP_Writer writer, const void *user_cx);
} SmolRTSP_HeaderVTable;

typedef struct {
    const SmolRTSP_HeaderVTable *vptr;
    const void *self;
} SmolRTSP_Header;

#endif // SMOLRTSP_HEADER_H
