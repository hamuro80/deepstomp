/*
 * dstlib_leveldetector.c
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

#include <dstlib_leveldetector.h>

dst_leveldetector_handle dst_leveldetector_create(unsigned length)
{
	dst_leveldetector_handle handle = malloc(sizeof(dst_leveldetector_struct));
	if(handle)
	{
		handle->maxsamplecount = length;
		handle->max1 = 0;
		handle->max2 = 0;
		handle->state = 0;
		handle->samplecount = 0;
	}
	return handle;
}

void  dst_leveldetector_destroy(dst_leveldetector_handle handle)
{
	if(handle)
		free((void*)handle);
}

void  dst_leveldetector_process(dst_leveldetector_handle handle,q15_t* input, q15_t* output)
{
	q15_t abslevel;
	arm_abs_q15(input,&abslevel,1);

	if(abslevel > handle->max1) handle->max1 = abslevel;
	if(abslevel > handle->max2) handle->max2 = abslevel;
	handle->samplecount++;
	if(handle->samplecount > handle->maxsamplecount)
	{
		handle->samplecount = 0;
		if(handle->state)
		{
			handle->level = handle->max1;
			handle->max1 = 0;
			handle->state = 0;
		}
		else
		{
			handle->level = handle->max2;
			handle->max2 = 0;
			handle->state = 1;
		}
	}

	*output = handle->level;
}