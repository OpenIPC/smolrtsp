#ifndef SMOLRTSP_AUX_MACROS_H
#define SMOLRTSP_AUX_MACROS_H

// Constructs a pointer to an unnamed object of a specified type in-place.
// See https://en.cppreference.com/w/c/language/compound_literal.
#define OBJ(type, expr) ((type *)(type[]){expr})

#endif // SMOLRTSP_AUX_MACROS_H
