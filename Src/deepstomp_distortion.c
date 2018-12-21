/*
 * deepstomp_distortion.c
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

#include "deepstomp_distortion.h"

#define lpfgain 15374
#define hpfgain 1097
#define lpfconst 888
#define hpfconst 888

static q15_t const fuzztable[129] =
{
		0,1036,2039,3011,3953,4866,5751,6608,7438,8243,9023,9778,10510,11220,11907,
		12573,13219,13844,14450,15037,15606,16158,16692,17210,17711,18197,18668,
		19125,19567,19995,20410,20813,21202,21580,21946,22301,22644,22977,23300,
		23613,23916,24209,24494,24769,25036,25295,25546,25789,26024,26252,26473,
		26688,26895,27096,27291,27480,27663,27840,28012,28178,28340,28496,28647,
		28794,28936,29074,29208,29337,29462,29584,29701,29815,29926,30033,30137,
		30237,30335,30429,30521,30609,30695,30778,30859,30937,31013,31086,31157,
		31226,31293,31357,31420,31481,31540,31597,31652,31705,31757,31808,31856,
		31903,31949,31993,32036,32078,32118,32157,32195,32232,32267,32302,32335,
		32368,32399,32429,32459,32487,32515,32541,32567,32593,32617,32640,32663,
		32685,32707,32728,32748,32767,32767
};

static q15_t const overdrivetable[129] =
{
		0,592,1175,1749,2314,2871,3418,3957,4488,5010,5524,6030,6528,7018,7501,7976,
		8444,8904,9357,9803,10242,10674,11100,11519,11931,12337,12736,13130,13517,
		13898,14273,14642,15005,15363,15715,16062,16403,16739,17070,17395,17716,
		18031,18342,18647,18948,19244,19536,19823,20105,20383,20657,20926,21191,
		21452,21709,21962,22211,22456,22698,22935,23169,23399,23626,23849,24068,
		24284,24497,24706,24912,25115,25315,25511,25705,25895,26083,26267,26449,
		26628,26804,26977,27148,27316,27481,27644,27804,27962,28117,28270,28420,
		28568,28714,28857,28998,29137,29274,29409,29541,29672,29800,29927,30051,
		30174,30294,30413,30530,30645,30758,30870,30980,31088,31194,31299,31402,
		31503,31603,31701,31798,31893,31987,32079,32170,32259,32347,32434,32519,
		32603,32686,32767,32767
};

static void toneprocess(distortion* handle, q15_t* input, q15_t* output)
{
	q31_t o1,o2;
	//low pass filter
	handle->xv[0] = handle->xv[1];
	handle->xv[1] = (((q31_t)(*input))<<10) / lpfgain;
	handle->yv[0] = handle->yv[1];
	handle->yv[1] =  ((handle->xv[0] + handle->xv[1])<<10)
				 + (lpfconst * handle->yv[0]);
	handle->yv[1] = handle->yv[1]>>10;
	o1 = handle->lowmix * handle->yv[1];

	//hi pass filter
	handle->hxv[0] = handle->hxv[1];
	handle->hxv[1] = (((q31_t)(*input))<<10) / hpfgain;
	handle->hyv[0] = handle->hyv[1];
	handle->hyv[1] =   ((handle->hxv[1] - handle->hxv[0])<<10)
				 + (hpfconst * handle->hyv[0]);
	handle->hyv[1] = handle->hyv[1]>>10;
	o2 = handle->himix * handle->hyv[1];

	//sum the low and high output
	*output = (q15_t) ((o1 + o2)/18);
}

static void process(dsthandle handle, q15_t* input,q15_t* output)
{
	distortion* h = (distortion*)handle;

	//bypass if distortion type is dst_distortion_type_clean
	if(h->type.value == distortiontype_clean)
	{
		*output = *input;
		return;
	}

	//get the signal sign
	int8_t sign = -1;
	if((*input)>0)
		sign = 1;

	//get the magnitude
	q15_t value ;
	arm_abs_q15(input,&value,1);

	// apply pre-gain
	q31_t temp = h->pregain * value;
	value = clip_q31_to_q15(temp);

	// temporary variable
	q31_t out;

	//get the base index
	q15_t firstindex ;
	firstindex = value >> 8;

	//get the fractional distance
	q15_t distance1 = value & 0b11111111;
	q15_t secondindex = firstindex+1;
	q31_t gradient = h->lookup[secondindex] - h->lookup[firstindex];

	// interpolate the sample from lookup table
	q31_t lininterp = h->lookup[firstindex] 	//max lininterp = Q15MAX
				+ ((((q31_t) distance1)*gradient)>>8);

	// compute the upper and lower gradients
	q31_t gradlower = gradient;
	q31_t gradupper = gradient;
	q15_t distance2 = 256 - distance1;
	if(firstindex>0)
		gradlower = h->lookup[firstindex] - h->lookup[firstindex-1];
	if(secondindex < 127)
		gradupper = h->lookup[secondindex+1] - h->lookup[secondindex];

	// get the lower and upper gradient extrapolated values
	q31_t linextrap1 = h->lookup[firstindex]
			+ (((((q31_t) distance1)*gradlower))>>8);
	q31_t linextrap2 = h->lookup[secondindex]
			- (((((q31_t) distance2)*gradupper))>>8);
	// compute the weight of for interpolated and the extrapolated
	q31_t weightextrap1 = (Q15MAX/distance1)>>2;
	q31_t weightextrap2 = (Q15MAX/distance2)>>2;
	q31_t weightinterp = (weightextrap1>weightextrap2)?weightextrap1:weightextrap2;
	weightinterp = weightinterp << 1;	//double the linear weight

	out = weightinterp * lininterp + weightextrap1 * linextrap1 + weightextrap2 * linextrap2 ;
	out = out / (weightinterp + weightextrap1 + weightextrap2);

	//apply post gain
	out = (h->postgain * out)>>5;
	value = clip_q31_to_q15(out * sign);
	toneprocess(h,&value,output);
}

static int8_t setdistortion(dsthandle handle,int8_t newvalue)
{
	distortion* h = (distortion*) handle;
	h->pregain =2*newvalue+1;	//1,3,5,7,..
	h->postgain = 32/h->pregain;	//32,10,6,..
	if(h->postgain<6)
		h->postgain = 6;
	h->distortion.value = newvalue;
	return newvalue;
}
static int8_t settype(dsthandle handle, int8_t newvalue)
{
	distortion* h = (distortion*)handle;
	if(newvalue == distortiontype_clean)
	{
		// do nothing
	}
	else if(newvalue == distortiontype_overdrive)
	{
		h->lookup = (q15_t*) overdrivetable;
	}
	else if(newvalue == distortiontype_fuzz)
	{
		h->lookup = (q15_t*) fuzztable;
	}

	h->type.value = newvalue;
	return newvalue;
}

static int8_t settone(dsthandle handle, int8_t newvalue)
{
	distortion* h = (distortion*) handle;
	h->lowmix = 18-newvalue;
	h->himix = newvalue;
	h->tone.value = newvalue;
	return newvalue;
}

int distortion_setup(distortion* handle)
{
	handle->interface.modulename = "DISTORTION";
	handle->interface.multimode = 0;
	handle->interface.paramcount = 3;
	handle->interface.process = process;

	//distortion level
	handle->distortion.name = "DISTORTION LEVEL";
	handle->distortion.stepcount = 19;
	handle->distortion.displaytype = displaytype_dot19;
	handle->distortion.onchange = setdistortion;
	//distortion type
	handle->type.name = "DISTORTION TYPE";
	handle->type.stepcount = 3;
	handle->type.displaytype = displaytype_dot10;
	handle->type.onchange = settype;
	//tone control
	handle->tone.name = "DISTORTION TONE";
	handle->tone.stepcount = 19;
	handle->tone.displaytype = displaytype_dot19;
	handle->tone.onchange = settone;

	handle->distortion.onchange((dsthandle) handle, 1);
	handle->type.onchange((dsthandle) handle, distortiontype_clean);
	handle->tone.onchange((dsthandle) handle,9);

	//initialize tone control variables
	handle->lookup = (q15_t*) overdrivetable;
	handle->xv[0] = 0;
	handle->xv[1] = 0;
	handle->yv[0] = 0;
	handle->yv[1] = 0;
	handle->hxv[0] = 0;
	handle->hxv[1] = 0;
	handle->hyv[0] = 0;
	handle->hyv[1] = 0;

	return setup_ok;
}
