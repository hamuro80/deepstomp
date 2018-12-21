/*
 * deepstomp_modextension.c
 * An extension module to enable controlling single-effect modules with 5-6 controls
 * inside a multi-effect module
 *
 *  Created on: 15 Des 2018
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
#include <deepstomp_modextension.h>

static int8_t control1change(dsthandle handle, int8_t newvalue)
{
	modextension* h = (modextension*) handle;
	dstsinglemodule* eh = (dstsinglemodule*) h->extendedhandle;
	eh->params[4].onchange((dsthandle)eh,newvalue);
	h->control1.value = newvalue;
	return newvalue;
}

static int8_t control2change(dsthandle handle, int8_t newvalue)
{
	modextension* h = (modextension*) handle;
	dstsinglemodule* eh = (dstsinglemodule*) h->extendedhandle;
	eh->params[5].onchange((dsthandle)eh,newvalue);
	h->control2.value = newvalue;
	return newvalue;
}

int modextension_setup(modextension* handle, dsthandle extended_handle)
{

	dstsinglemodule* eh = (dstsinglemodule*) extended_handle;
	if(eh->interface.paramcount<5)
		return setup_error;	//only module with more than 4 params can be extended

	//INIT BASIC CORE INTERFACE

	handle->interface.modulename = eh->interface.modulename;
	handle->interface.multimode = 0;
	handle->interface.paramcount = eh->interface.paramcount-4;
	handle->interface.process = NULL;

	//assign the extended module
	handle->extendedhandle = extended_handle;

	//control1
	handle->control1.name = eh->params[4].name;
	handle->control1.displaytype = eh->params[4].displaytype;
	handle->control1.stepcount =  eh->params[4].stepcount;
	handle->control1.onchange = control1change;

	//control2
	if(handle->interface.paramcount==2)
	{
		handle->control2.name = eh->params[5].name;
		handle->control2.displaytype = eh->params[5].displaytype;
		handle->control2.stepcount =  eh->params[5].stepcount;
		handle->control2.onchange = control2change;
	}

	//synchronize with the extended's initial value
	handle->control1.value = eh->params[4].value;
	if(handle->interface.paramcount==2)
	{
		handle->control2.value = eh->params[5].value;
	}

	return setup_ok;
}
