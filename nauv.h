/**********************************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2014 NAN contributors:
 *   See nan.h for details.
 *
 * See https://github.com/rvagg/nan for the latest update to this file
 **********************************************************************************/

#ifndef NAUV_H_
#define NAUV_H_

#include <uv.h>

#ifdef UV_VERSION_MAJOR
#ifndef UV_VERSION_PATCH
#define UV_VERSION_PATCH 0
#endif
#define NAUV_UVVERSION  ((UV_VERSION_MAJOR << 16) | \
                     (UV_VERSION_MINOR <<  8) | \
                     (UV_VERSION_PATCH))
#else
#define NAUV_UVVERSION 0x000b00
#endif


#if NAUV_UVVERSION < 0x000b00
#define NAUV_WORK_CB(func) \
    void func(uv_async_t *async, int)
#else
#define NAUV_WORK_CB(func) \
    void func(uv_async_t *async)
#endif

#endif  // NAUV_H_
