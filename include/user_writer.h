#ifndef SMOLRTSP_USER_WRITER_H
#define SMOLRTSP_USER_WRITER_H

#include <stddef.h>

typedef void (*SmolRTSP_UserWriter)(size_t size, const void *data, void *user_cx);

#endif // SMOLRTSP_USER_WRITER_H
