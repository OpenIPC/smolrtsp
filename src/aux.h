#ifndef SMOLRTSP_AUX_H
#define SMOLRTSP_AUX_H

#define CRLF "\r\n"

#define CAT(x, y)           PRIMITIVE_CAT(x, y)
#define PRIMITIVE_CAT(x, y) x##y

#define STRINGIFY(x)           PRIMITIVE_STRINGIFY(x)
#define PRIMITIVE_STRINGIFY(x) #x

#endif // SMOLRTSP_AUX_H
