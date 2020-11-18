#ifndef SMOLRTSP_WRITER_H
#define SMOLRTSP_WRITER_H

#include <stddef.h>

typedef void (*SmolRTSP_Writer)(size_t size, const void *data, const void *user_cx);

#endif // SMOLRTSP_WRITER_H
