/**
 * @file
 * @brief A convenient macro for declaring opaque data types.
 */

#ifndef SMOLRTS_OPAQUE_TYPE_H
#define SMOLRTS_OPAQUE_TYPE_H

#define SMOLRTSP_OPAQUE_TYPE(T)                                                                    \
    struct T;                                                                                      \
    typedef struct T T

#endif // SMOLRTS_OPAQUE_TYPE_H
