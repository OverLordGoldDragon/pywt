// Copyright (c) 2006-2007 Filip Wasilewski <filip.wasilewski@gmail.com>
// See COPYING for license details.

// $Id$

// Common constants, typedefs and functions

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdlib.h>
#include <math.h>
#include <memory.h>

#ifdef __GNUC__
    #define INLINE __inline__
#elif _WIN32
    #define INLINE __inline
#else
    #define INLINE
#endif

#ifdef MSVC
    #pragma inline_depth(1)
#endif

///////////////////////////////////////////////////////////////////////////////
// Typedefs

#ifdef PY_EXTENSION
    // another declaration is in .c file generated by Pyrex
    #ifndef PY_SSIZE_T_CLEAN
        #define PY_SSIZE_T_CLEAN
        #include "Python.h"
        #include "structmember.h"
        #ifndef PY_LONG_LONG
            #define PY_LONG_LONG LONG_LONG
        #endif
        #if PY_VERSION_HEX < 0x02050000
            typedef int Py_ssize_t;
            #define PY_SSIZE_T_MAX INT_MAX
            #define PY_SSIZE_T_MIN INT_MIN
            #define PyInt_FromSsize_t(z) PyInt_FromLong(z)
            #define PyInt_AsSsize_t(o)   PyInt_AsLong(o)
        #endif
    #else
        #include "Python.h"
    #endif
    
    typedef Py_ssize_t index_t;

    // using Python's memory manager
    #define wtmalloc(size)      PyMem_Malloc(size)
    #define wtfree(ptr)         PyMem_Free(ptr)
    void *wtcalloc(size_t, size_t);
#else
    typedef int index_t; 
    // standard c memory management
    #define wtmalloc(size)      malloc(size)
    #define wtfree(ptr)         free(ptr)
    #define wtcalloc(len, size) calloc(len, size)
#endif


typedef const index_t const_index_t;


// Signal extension modes
typedef enum {
       MODE_INVALID = -1,
       MODE_ZEROPAD = 0,   // default, signal extended with zeros
       MODE_SYMMETRIC,     // signal extended symmetrically (mirror)
       MODE_CONSTANT_EDGE, // signal extended with the border value
       MODE_SMOOTH,        // linear extrapolation (first derivative)
       MODE_PERIODIC,      // signal is treated as being periodic
       MODE_PERIODIZATION, // signal is treated as being periodic, minimal output lenght
       MODE_MAX,
       MODE_ASYMMETRIC // TODO
} MODE;


///////////////////////////////////////////////////////////////////////////////
// Calculating buffer lengths for various operations

// Length of DWT coeffs for specified input data length, filter length and
// signal extension mode 

index_t dwt_buffer_length(index_t input_len, index_t filter_len, MODE mode);

// Length of reconstructed signal for specified input coeffs length and filter
// length. It is used for direct reconstruction from coefficients (normal
// convolution of upsampled coeffs with filter).

index_t reconstruction_buffer_length(index_t coeffs_len, index_t filter_len);

// Length of IDWT reconstructed signal for specified input coeffs length, filter
// length and extension mode.

index_t idwt_buffer_length(index_t coeffs_len, index_t filter_len, MODE mode);

// Length of SWT coefficients for specified input signal length.
// Equals to input_len

index_t swt_buffer_length(index_t input_len);


///////////////////////////////////////////////////////////////////////////////
// Maximum useful level of DWT decomposition.

int dwt_max_level(index_t input_len, index_t filter_len);


///////////////////////////////////////////////////////////////////////////////
// Maximum useful level of SWT decomposition.

int swt_max_level(index_t input_len);


#endif //_COMMON_H_
