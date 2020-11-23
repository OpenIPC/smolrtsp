#ifndef SMOLRTSP_REQUEST_PARSER_H
#define SMOLRTSP_REQUEST_PARSER_H

#include <smolrtsp/request.h>

#include <stddef.h>

struct SmolRTSP_RequestParser;
typedef struct SmolRTSP_RequestParser SmolRTSP_RequestParser;

SmolRTSP_RequestParser *SmolRTSP_RequestParser_new(void);
void SmolRTSP_RequestParser_free(SmolRTSP_RequestParser *parser);

typedef enum {
    SmolRTSP_ParseRequestResultMethodParsed,
    SmolRTSP_ParseRequestResultRequestURIParsed,
    SmolRTSP_ParseRequestResultRTSPVersionParsed,
    SmolRTSP_ParseRequestResultHeaderParsed,
    SmolRTSP_ParseRequestResultBodyParsed,
} SmolRTSP_ParseRequestResult;

SmolRTSP_ParseRequestResult SmolRTSP_RequestParser_parse(
    SmolRTSP_RequestParser *restrict parser, SmolRTSP_Request *restrict request, size_t size,
    const void *data);

#endif // SMOLRTSP_REQUEST_PARSER_H
