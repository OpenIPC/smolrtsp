#ifndef SMOLRTSP_HEADER_H
#define SMOLRTSP_HEADER_H

#include <oop.h>
#include <writer.h>

#include <stddef.h>

SMOLRTSP_INTERFACE(SmolRTSP_Header, {
    const char *(*key)(void);
    void (*write_value)(const void *self, SmolRTSP_Writer writer, const void *user_cx);
});

#define SMOLRTPS_HEADER SMOLRTPS_INTERFACE_OBJ

#endif // SMOLRTSP_HEADER_H
