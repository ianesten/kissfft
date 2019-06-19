/*
 *  Copyright (c) 2003-2004, Mark Borgerding. All rights reserved.
 *  This file is part of KISS FFT - https://github.com/mborgerding/kissfft
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *  See COPYING file for more information.
 */

#ifndef KISS_FTR_H
#define KISS_FTR_H

#include "kiss_fft.h"
#ifdef __cplusplus
extern "C" {
#endif

    
/* 
 
 Real optimized version can save about 45% cpu time vs. complex fft of a real seq.

 
 
 */

typedef struct kiss_fftr_state *kiss_fftr_cfg;


kiss_fftr_cfg kiss_fftr_alloc(int nfft,int inverse_fft,void * mem, size_t * lenmem);
kiss_fftr_cfg kiss_fftr_alloc_with_twiddles(int nfft, int inverse_fft, const kiss_fft_cpx* super_twiddles,
                                            const kiss_fft_cpx* substate_twiddles, void * mem, size_t * lenmem);
/*
 nfft must be even

 If you don't care to allocate space, use mem = lenmem = NULL 
*/
void kiss_fftr_get_twiddles(int nfft, int inverse_fft,
                            kiss_fft_cpx* super_twiddles, size_t* super_twiddles_lenmem,
                            kiss_fft_cpx* substate_twiddles, size_t* substate_twiddles_lenmem);


void kiss_fftr(kiss_fftr_cfg cfg,const kiss_fft_scalar *timedata,kiss_fft_cpx *freqdata);
/*
 input timedata has nfft scalar points
 output freqdata has nfft/2+1 complex points
*/

void kiss_fftri(kiss_fftr_cfg cfg,const kiss_fft_cpx *freqdata,kiss_fft_scalar *timedata);
/*
 input freqdata has  nfft/2+1 complex points
 output timedata has nfft scalar points
*/

#define kiss_fftr_free KISS_FFT_FREE

#ifdef __cplusplus
}
#endif
#endif
