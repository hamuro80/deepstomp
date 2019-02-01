/*
 * deepstomp_delay.c
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
#include "deepstomp_delay.h"

static int8_t setdelay(dsthandle handle, int8_t value)
{
	// delay (0-18) => 10 - 190 ms (approximation)
	delay* h = (delay*) handle;
	//always work unconditionally, directly set with the new value
    h->delay.value = value;

	h->delayinsamples =  220 * ((int)value+1);
	if(h->delayinsamples >= h->bufferlength)
		h->delayinsamples = h->bufferlength-1;

	return value;	//return the new successfully set value
}

static int8_t setfeedback(dsthandle handle, int8_t value)
{
	delay* h = (delay*)handle;
	//always work unconditionally, directly set with the new value
    h->feedback.value = value;

    h->feedbackgain = 60 * (q15_t)value;
    if(h->feedbackgain>1024) h->feedbackgain = 1024;
    //return the new successfully set value
	return value;
}

static int8_t setbalance(dsthandle handle, int8_t value)
{
	delay* h = (delay*) handle;
	//always work unconditionally, directly set with the new value
    h->balance.value = value;

    h->drygain = 1024 - (value*60);
	if(h->drygain<0) h->drygain = 0;
	h->delaygain = 1024-h->drygain;

	//return the new successfully set value
	return value;
}

//push the input to a down-sampled delay line and pull the delayed signal from it
static void delayline(delay* handle, q15_t* input, q15_t* output)
{
	if(handle->downsamplingstate)
	{
		handle->downsamplingstate = 0;

		//just record the current sample into prevsample for down-sampling
		handle->previnputsample = (*input);

		//up-sample:
		//get the output from interpolation of the prevdelayedsample and lastdelayedsample to up-sample
		(*output) = (q15_t)(((q31_t)handle->prevdelayedsample + (q31_t)handle->lastdelayedsample)>>1);
	}
	else
	{
		handle->downsamplingstate = 1;

		//down-sample the input and put into the delay line buffer
		handle->delaybuffer[handle->writepointer] = (q15_t)(((q31_t)handle->previnputsample + (q31_t)(*input))>>1);

		int readpointer = handle->writepointer - handle->delayinsamples;
		if(readpointer<0)
			readpointer = handle->bufferlength + readpointer;
		handle->writepointer++;
		if(handle->writepointer>=handle->bufferlength) handle->writepointer = 0;

		//up-sample:
		(*output) = handle->prevdelayedsample;

		//update the lastdelayed and prevdelayed for up-sampling
		handle->prevdelayedsample = handle->lastdelayedsample;
		handle->lastdelayedsample = handle->delaybuffer[readpointer];

	}
}

static void process(dsthandle handle,q15_t* input,q15_t* output)
{
	//The DSP algorithm:
	//(input/2 + feedback_gain * delay_line_out) -> delay_line -> delay_line_out
	//output = input * dry_gain + 2 * delay_line_out * delay_gain

	delay* h = (delay*) handle;
	if(h->delaygain==0)
	{
		*output = *input;
		return;
	}

	q31_t temp  = (((q31_t)(*input))<<9) + ((q31_t)h->feedbackgain * (q31_t)h->delaylineout);
	q15_t temp2 = (q15_t)(temp>>10);
	delayline(h,&temp2,&(h->delaylineout));
	temp = ((q31_t)(*input) * (q31_t)h->drygain )
			+ ((q31_t)((q31_t)h->delaylineout * (q31_t)h->delaygain)<<1);
	temp = temp >> 10;
	*output = temp;
}

int delay_setup(delay* handle)
{
	handle->interface.modulename = "DELAY";
	handle->interface.multimode = 0;
	handle->interface.paramcount = 3;
	handle->interface.process = (dstprocess) process;

	//setting up the dstparams
	//delay length
	handle->delay.name = "DELAY LENGTH";
	handle->delay.stepcount = 19;
	handle->delay.displaytype = displaytype_dot19;
	handle->delay.onchange = setdelay;
	//delay feedback
	handle->feedback.name = "DELAY FEEDBACK";
	handle->feedback.stepcount = 19;
	handle->feedback.displaytype = displaytype_dot19;
	handle->feedback.onchange = setfeedback;
	//delay balance
	handle->balance.name = "DELAY BALANCE";
	handle->balance.stepcount = 19;
	handle->balance.displaytype = displaytype_dot19;
	handle->balance.onchange = setbalance;

	//set the parameter values
	handle->feedback.onchange((dsthandle)handle,9);
	handle->balance.onchange((dsthandle)handle,9);

	//allocate for max delay = 190 ms at down sampled 22.05 kHz sampling rate
	handle->bufferlength = 19 * 220;
	handle->delaybuffer = malloc(sizeof(q15_t) * handle->bufferlength);
	if(handle->delaybuffer)
	{
		//initialize the buffer to zero
		for(int i=0;i<handle->bufferlength;i++)
			handle->delaybuffer[i]=0;
		handle->delay.onchange((dsthandle)handle,18);
		handle->downsamplingstate = 0;
		handle->delaylineout = 0;
		return setup_ok;
	}
	else
		return setup_error;
}

