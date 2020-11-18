#ifndef SMOLRTSP_OOP_H
#define SMOLRTSP_OOP_H

#define SMOLRTSP_INTERFACE(name, methods)                                                          \
    typedef struct methods name##VTable;                                                           \
                                                                                                   \
    typedef struct {                                                                               \
        const name##VTable *vptr;                                                                  \
        const void *self;                                                                          \
    } name

#define SMOLRTSP_MUT_INTERFACE(name, methods)                                                      \
    typedef struct methods Mut##name##VTable;                                                      \
                                                                                                   \
    typedef struct {                                                                               \
        const Mut##name##VTable *vptr;                                                             \
        void *self;                                                                                \
    } Mut##name

#define SMOLRTSP_INTERFACE_OBJ(derived_type, obj) ({.vptr = &derived_type##_vtable, .self = (obj)})

#endif // SMOLRTSP_OOP_H
