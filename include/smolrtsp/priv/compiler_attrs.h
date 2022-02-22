#pragma once

#ifdef __GNUC__
#define SMOLRTSP_PRIV_GNUC_ATTR(attr) __attribute__((attr))
#else
#define SMOLRTSP_PRIV_GNUC_ATTR(_attr)
#endif

#if defined(__GNUC__) && !defined(__clang__)
#define SMOLRTSP_PRIV_GCC_ATTR(attr) __attribute__((attr))
#else
#define SMOLRTSP_PRIV_GCC_ATTR(_attr)
#endif

#define SMOLRTSP_PRIV_MUST_USE SMOLRTSP_PRIV_GNUC_ATTR(warn_unused_result)
