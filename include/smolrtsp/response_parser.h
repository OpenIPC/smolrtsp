#ifndef SMOLRTSP_RESPONSE_PARSER_H
#define SMOLRTSP_RESPONSE_PARSER_H

#include <smolrtsp/response.h>

#include <stddef.h>

struct SmolRTSP_ResponseParser;
typedef struct SmolRTSP_ResponseParser SmolRTSP_ResponseParser;

SmolRTSP_ResponseParser *SmolRTSP_ResponseParser_new(void);
void SmolRTSP_ResponseParser_free(SmolRTSP_ResponseParser *parser);

typedef enum {
    SmolRTSP_ParseResponseResultOk,
    SmolRTSP_ParseResponseResultInvalid,
} SmolRTSP_ParseResponseResult;

SmolRTSP_ParseResponseResult SmolRTSP_ResponseParser_parse(
    SmolRTSP_ResponseParser *parser, SmolRTSP_Response *response, size_t input_size,
    const void *input);

#endif // SMOLRTSP_RESPONSE_PARSER_H
