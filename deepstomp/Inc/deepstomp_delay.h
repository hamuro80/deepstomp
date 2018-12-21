/*
 * deepstomp_delay.h
 * Delay effect module, implemented with down-sampling when buffering the sample
 * into the delay line buffer to save memory space. Up-sampling is used
 * to convert back to 44.1KHz after getting out from the delay line buffer
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

#ifndef DEEPSTOMP_DELAY_H_
#define DEEPSTOMP_DELAY_H_

#include <deepstomp.h>

typedef struct
{
	//dstinterface should always be the first element in the deepstomp's data structure
	dstinterface interface;

	//don't separate the following dstparams to construct an array-like structure
	//begin array-like dstparams structure
	dstparam delay;
	dstparam feedback;
	dstparam balance;
	//end of array-like dstparams structure

	//next, add additional variables as much as needed
	q15_t drygain;
	q15_t delaygain;
	q15_t feedbackgain;
	q15_t delaylineout;
	q15_t* delaybuffer;
	q15_t prevdelayedsample;
	q15_t lastdelayedsample;
	q15_t previnputsample;
	int8_t downsamplingstate;
	int bufferlength;
	int delayinsamples;
	int writepointer;
} delay;

int delay_setup(delay* handle);

#endif /* DEEPSTOMP_DELAY_H_ */
