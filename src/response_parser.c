#include "parsing_aux.h"
#include <smolrtsp/response_parser.h>

#include <stdlib.h>

typedef enum { s } SmolRTSP_ResponseParserState;

struct SmolRTSP_ResponseParser {

    SmolRTSP_ResponseParserState state;
};

SmolRTSP_ResponseParser *SmolRTSP_ResponseParser_new(void) {
    SmolRTSP_ResponseParser *parser;
    if ((parser = malloc(sizeof(*parser))) == NULL) {
        return NULL;
    }
}

void SmolRTSP_ResponseParser_free(SmolRTSP_ResponseParser *parser) {
    free(parser);
}

SmolRTSP_ParseResponseResult SmolRTSP_ResponseParser_parse(
    SmolRTSP_ResponseParser *parser, SmolRTSP_Response *response, size_t input_size,
    const void *input) {}
