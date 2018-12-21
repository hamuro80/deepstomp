/*
 * deepstomp_multieffect.c
 *
 *  Created on: 28 Nov 2018
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
#include <deepstomp_multieffect.h>

static void process(dsthandle handle,q15_t* input, q15_t* output)
{
	multieffect* h = (multieffect*) handle;
	h->expansormodule.interface.process((dsthandle)&(h->expansormodule),input,output);
	*input = *output;
	h->distortionmodule.interface.process((dsthandle)&(h->distortionmodule),input,output);
	*input = *output;
	h->delaymodule.interface.process((dsthandle)&(h->delaymodule),input,output);
}

int multieffect_setup(multieffect* handle)
{
	handle->interface.multimode = 1;
	handle->interface.paramcount = 6;
	handle->interface.process = process;

	handle->preset.name = "PRESET";
	handle->preset.stepcount = 19;	//number of preset
	handle->preset.displaytype = displaytype_dot19;

	handle->module.name = "MODULE";
	handle->module.stepcount = 4;	//number of modules
	handle->module.displaytype = displaytype_dot10;

	//expansor setup
	int setupstatus = expansor_setup(&(handle->expansormodule));
	if(setupstatus != setup_ok)
		return setupstatus;
	//expansor extension setup
	setupstatus = modextension_setup(&(handle->expansorextmodule),(dsthandle)&(handle->expansormodule));
	if(setupstatus != setup_ok)
		return setupstatus;
	//distortion setup
	setupstatus = distortion_setup(&(handle->distortionmodule));
	if(setupstatus != setup_ok)
		return setupstatus;
	//delay setup
	setupstatus = delay_setup(&(handle->delaymodule));
	if(setupstatus != setup_ok)
		return setupstatus;

	handle->modulehandles[0] = (dstsinglemodule*) &(handle->expansormodule);
	handle->modulehandles[1] = (dstsinglemodule*) &(handle->expansorextmodule);
	handle->modulehandles[2] = (dstsinglemodule*) &(handle->distortionmodule);
	handle->modulehandles[3] = (dstsinglemodule*) &(handle->delaymodule);

	return setup_ok;
}


