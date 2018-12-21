/*
 * deepstomp_expansor.c
 * An integrated expander-compressor library for automatic noise gate and
 * dynamic range compressor
 *
 *  Created on: 7 Nov 2018
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
#include <deepstomp_expansor.h>

//LPF at 20Hz
// decay is scaled to 256x (<<8)
#define GAIN   703
#define DECAY	255

// prototypes to avoid undefined function compilation when they call each other
static int8_t setofflevel(dsthandle handle, int8_t newvalue);
static int8_t setonlevel(dsthandle handle, int8_t newvalue);
static int8_t setcompstart(dsthandle handle, int8_t newvalue);
static int8_t setcompression(dsthandle handle, int8_t newvalue);
static int8_t setcompattack(dsthandle handle, int8_t newvalue);
static int8_t setcomprelease(dsthandle handle, int8_t newvalue);


static void levelfilter(dsthandle handle, q15_t* input, q15_t* output)
{
	((expansor*)handle)->xv[0] = ((expansor*)handle)->xv[1];
	((expansor*)handle)->xv[1] = (*input);
	((expansor*)handle)->yv[0] = ((expansor*)handle)->yv[1];
	((expansor*)handle)->yv[1] =   ((((expansor*)handle)->xv[0] + ((expansor*)handle)->xv[1])<<8)
				 + (DECAY * ((expansor*)handle)->yv[0]);
	((expansor*)handle)->yv[1] = ((expansor*)handle)->yv[1]>>8;
	q31_t temp = ((expansor*)handle)->yv[1]/GAIN;
	*output = clip_q31_to_q15(temp<<2);
}

static int8_t setcompstart(dsthandle handle, int8_t newvalue)
{
	if(newvalue > 17) newvalue = 17;
	expansor* h = (expansor*) handle;
	//check the integrity with onlevel
	if(newvalue < h->onlevel.value)
		newvalue = h->onlevel.value;
	h->compstart.value = newvalue;
	h->compstartq15 = EXPSTEP19[newvalue];

	//set the correction gain to amplify the maximum compressed level to 1/2 of Q15MAX
	h->correctiongain = (Q15MAX - h->compstartq15)/((h->compression.value<<1)+1);
	h->correctiongain = (q15_t)((q31_t)((q31_t)Q15MAX / (h->compstartq15 + h->correctiongain))>>1);
	return newvalue;
}

static int8_t setofflevel(dsthandle handle, int8_t newvalue)
{
	expansor* h = (expansor*) handle;
	//force the maximum level = 17
	if(newvalue >= 17)
		newvalue = 17;

	h->offlevel.value = newvalue;
	h->offlevelq15 = EXPSTEP19[newvalue];

	//check the integrity with onlevel
	if(h->onlevel.value <= newvalue)
		setonlevel(handle,newvalue + 1);

	h->expansionrange = h->onlevelq15 - h->offlevelq15;
	return newvalue;
}

static int8_t setonlevel(dsthandle handle, int8_t newvalue)
{
	expansor* h = (expansor*) handle;
	//check the integrity with offlevel
	if(newvalue <= h->offlevel.value)
		newvalue = h->offlevel.value + 1;
	h->onlevel.value = newvalue;
	h->onlevelq15 = EXPSTEP19[newvalue];
	//compute the expansion range
	h->expansionrange = h->onlevelq15 - h->offlevelq15;

	//check the integrity with compstart
	if(h->compstart.value < newvalue)
		setcompstart(handle,newvalue);

	return newvalue;
}

static int8_t setcompression(dsthandle handle, int8_t newvalue)
{
	expansor* h = (expansor*) handle;
	h->compression.value = newvalue;
	//set the correction gain to amplify the maximum compressed level to 1/2 of Q15MAX
	h->correctiongain = (Q15MAX - h->compstartq15)/((h->compression.value<<1)+1);
	h->correctiongain = (q15_t)((q31_t)((q31_t)Q15MAX / (h->compstartq15 + h->correctiongain))>>1);
	return newvalue;
}

static int8_t setcompattack(dsthandle handle, int8_t newvalue)
{
	expansor* h = (expansor*) handle;
	h->compattack.value = newvalue;
	return newvalue;
}

static int8_t setcomprelease(dsthandle handle, int8_t newvalue)
{
	expansor* h = (expansor*) handle;
	h->comprelease.value = newvalue;
	return newvalue;
}


static void process(dsthandle handle,q15_t* input,q15_t* output)
{
	expansor* h = (expansor*)handle;

	//detect the level by taking the absolute value of the sample
	//plus the absolute value of the difference, then apply 20Hz LPF
	q15_t tempq15;
	q15_t level=0;

	//do the level detection if noise gate or compressor is on
	if((h->offlevel.value > 0) || (h->compression.value > 0))
	{
		arm_abs_q15(input,&tempq15,1);
		levelfilter(handle,&tempq15,&level);
	}

	//do the noise gating if noise gate is on
	if(h->offlevel.value > 0)
	{
		q31_t gategain = 0;
		//process the signal by noise gate
		if(level > h->onlevelq15)
		{
			gategain = Q15MAX;
		}
		else if(level > h->offlevelq15)
		{
			gategain = (Q15MAX * (level-h->offlevelq15))/h->expansionrange;
		}
		tempq15 = (q15_t)(((q31_t)(*input) * gategain)>>15);
	}
	else
	{
		tempq15 = *input;
	}

	//do the compression if compressor is on
	if(h->compression.value > 0)
	{
		//filter the level using attack and decay control
		q31_t leveldrive = ((q31_t)level - h->compstartq15)<<13;
		if(leveldrive<0)
			leveldrive = 0;

		leveldrive = leveldrive - h->filteredlevel;
		if(leveldrive > 0)
		{
			//drive the filteredlevel by attack control
			leveldrive = (leveldrive*(h->compattack.value + 1))>>12;
			if(leveldrive<1) leveldrive = 1;
			h->filteredlevel += leveldrive;
		}
		else if(leveldrive < 0)
		{
			//drive the filteredlevel by release control
			leveldrive = (leveldrive*(h->comprelease.value + 1))>>12;
			if(leveldrive > -1) leveldrive = -1;
			h->filteredlevel += leveldrive;
		}

		//compress the signal
		//Basic formula: (actual level) x (compressor gain) = (compressed level)
		//(compressor gain) = (compressed level)  / (actual level)
		// compgain = (compstart + (filteredlevel/compression)) / (filteredlevel + compstart)
		q15_t scaledfilteredlevel = h->filteredlevel>>13;

		q15_t compressedlevel = h->compstartq15 + scaledfilteredlevel/(h->compression.value<<1);
		q31_t compgain;
		compgain = (q31_t)compressedlevel;
		compgain = compgain << 15; //scaling for division
		compgain = compgain /(scaledfilteredlevel + h->compstartq15);
		tempq15 = clip_q31_to_q15((compgain*tempq15)>>15); //scale back after division

		tempq15 = clip_q31_to_q15(((q31_t)h->correctiongain * tempq15));
	}

	*output = tempq15;
}

int expansor_setup(expansor* handle)
{
	//INIT BASIC CORE INTERFACE
	handle->interface.modulename = "EXPANSOR";
	handle->interface.multimode = 0;
	handle->interface.paramcount = 6;
	handle->interface.process = process;

	//CONFIGURE THE PARAMETERS
	//off-level
	handle->offlevel.name = "GATE OFF-LEVEL";
	handle->offlevel.displaytype = displaytype_dot19;
	handle->offlevel.stepcount = 17;
	handle->offlevel.onchange = (paramchangecall) setofflevel;

	//on-level
	handle->onlevel.name = "GATE ON-LEVEL";
	handle->onlevel.displaytype = displaytype_dot19;
	handle->onlevel.stepcount = 18;
	handle->onlevel.onchange = (paramchangecall) setonlevel;

	//compstart
	handle->compstart.name = "COMP START-LEVEL";
	handle->compstart.displaytype = displaytype_dot19;
	handle->compstart.stepcount = 18;
	handle->compstart.onchange = (paramchangecall) setcompstart;

	//compression
	handle->compression.name = "COMPRESSION LVL";
	handle->compression.displaytype = displaytype_dot10;
	handle->compression.stepcount = 10;
	handle->compression.onchange = (paramchangecall)setcompression;

	//compression attack
	handle->compattack.name = "COMP ATTACK";
	handle->compattack.displaytype = displaytype_dot38;
	handle->compattack.stepcount = 38;
	handle->compattack.onchange = setcompattack;

	//compression release
	handle->comprelease.name = "COMP RELEASE";
	handle->comprelease.displaytype = displaytype_dot38;
	handle->comprelease.stepcount = 38;
	handle->comprelease.onchange = setcomprelease;

	//initialize the parameter values by calling its on-change callback
	handle->comprelease.onchange((dsthandle)handle,18);
	handle->compattack.onchange((dsthandle)handle,18);
	handle->compression.onchange((dsthandle)handle,0);	//compressor off
	handle->compstart.onchange((dsthandle)handle,5);
	handle->onlevel.onchange((dsthandle)handle,5);
	handle->offlevel.onchange((dsthandle)handle,0);	//noise gate off

	//INIT STATE VARIABLES
	handle->xv[0] = 0;
	handle->xv[1] = 0;
	handle->yv[0] = 0;
	handle->yv[1] = 0;
	handle->filteredlevel = 0;

	return setup_ok;
}
