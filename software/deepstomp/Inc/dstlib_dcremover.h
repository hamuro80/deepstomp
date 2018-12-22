/*
 * dstlib_dcremover.h
 *  Filter out any dc component in the signal by 1st order high-pass filter
 *  at 10 Hz cut-off frequency (with 44.1 kHz sampling frequency)
 *
 *  Created on: 2 Nov 2018
 *      Author: HAMURO
 *      * COPYRIGHT(c) 2018 DEEPTRONIC.COM
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of DEEPTRONIC.COM nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#ifndef DSTLIB_DCREMOVER_H_
#define DSTLIB_DCREMOVER_H_

#include <deepstomp.h>

typedef struct
{
	q31_t hxv[2];
	q31_t hyv[2];
} dst_dcremover_struct;
typedef dst_dcremover_struct* dst_dcremover_handle;

dst_dcremover_handle dst_dcremover_create();
void dst_dcremover_destroy(dst_dcremover_handle handle);
void dst_dcremover_process(dst_dcremover_handle handle, q15_t *input, q15_t *output);


#endif /* DSTLIB_DCREMOVER_H_ */
