#include "parsing_aux.h"
#include <smolrtsp/request_parser.h>

#include <stdlib.h>

typedef enum {
    SmolRTSP_RequestParserStateMethod,
    SmolRTSP_RequestParserStateRequestURI,
    SmolRTSP_RequestParserStateRTSPVersion,
    SmolRTSP_RequestParserStateHeaders,
} SmolRTSP_RequestParserState;

struct SmolRTSP_RequestParser {
    SmolRTSP_RequestParserState state;
    SmolRTSP_Cursor cursor;
};

SmolRTSP_RequestParser *SmolRTSP_RequestParser_new(void) {
    SmolRTSP_RequestParser *parser;
    if ((parser = malloc(sizeof(*parser))) == NULL) {
        return NULL;
    }

    parser->state = SmolRTSP_RequestParserStateMethod;
    parser->cursor = NULL;

    return parser;
}

void SmolRTSP_RequestParser_free(SmolRTSP_RequestParser *parser) {
    free(parser);
}

SmolRTSP_ParseRequestResult SmolRTSP_RequestParser_parse(
    SmolRTSP_RequestParser *restrict parser, SmolRTSP_Request *restrict request, size_t size,
    const void *data) {}
