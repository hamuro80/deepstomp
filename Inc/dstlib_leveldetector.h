/*
 * dstlib_leveldetector.h
 * Level detector module, search the maximum absolute value of the signal
 * within the window length
 *
 *  Created on: 27 Okt 2018
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


#ifndef DSTLIB_LEVELDETECTOR_H_
#define DSTLIB_LEVELDETECTOR_H_

#include <deepstomp.h>

typedef struct
{
	unsigned int samplecount;
	unsigned int maxsamplecount;
	uint8_t state;
	q15_t max2;
	q15_t max1;
	q15_t level;

} dst_leveldetector_struct;
typedef dst_leveldetector_struct* dst_leveldetector_handle;

dst_leveldetector_handle dst_leveldetector_create(unsigned length);
void  dst_leveldetector_destroy(dst_leveldetector_handle handle);
void  dst_leveldetector_init(dst_leveldetector_handle handle);
void  dst_leveldetector_process(dst_leveldetector_handle handle,q15_t* input, q15_t* output);

#endif /* DSTLIB_LEVELDETECTOR_H_ */
