/*
 * deepstomp_distortion.h
 * Distortion effect module, based on logistic/sigmoid function, implemented
 * using table-lookup and linear interpolation.
 *
 *  Created on: 29 Okt 2018
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

#ifndef DEEPSTOMP_DISTORTION_H_
#define DEEPSTOMP_DISTORTION_H_

#include <deepstomp.h>

enum {distortiontype_clean,distortiontype_overdrive,distortiontype_fuzz,distortiontype_foldback};
enum {interpolation_linear,interpolation_linear4p};

typedef struct
{
	//dstinterface should always be the first element in the deepstomp's data structure
	dstinterface interface;

	//don't separate the following dstparams to construct an array-like structure
	//begin array-like dstparams structure
	dstparam distortion;
	dstparam type;
	dstparam tone;
	//end of array-like dstparams structure

	//next, add additional variables as much as needed
	q31_t pregain;	//for controlling the distortion level
	q31_t postgain;	//for normalizing the output gain
	q15_t* lookup;

	//tone control internal variables
	q31_t xv[2];
	q31_t yv[2];
	q31_t hxv[2];
	q31_t hyv[2];
	q15_t lowmix;
	q15_t himix;
} distortion;

int distortion_setup(distortion* handle);

#endif /* DEEPSTOMP_DISTORTION_H_ */
