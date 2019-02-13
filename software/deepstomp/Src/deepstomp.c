/*
 * deepstomp.c
 * Deepstomp processor application configuration
 *  Created on: 18 Sep 2018
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
#include "deepstomp.h"

// BEGIN_CONFIG
#include "deepstomp_multieffect.h"
#define MODULETYPE multieffect
#define MODULESETUP multieffect_setup
// END_CONFIG


/*
// BEGIN_CONFIG
#include "deepstomp_expansor.h"
#define MODULETYPE expansor
#define MODULESETUP expansor_setup
// END_CONFIG
 *
 */


/*
// BEGIN_CONFIG
#include "deepstomp_distortion.h"
#define MODULETYPE distortion
#define MODULESETUP distortion_setup
// END_CONFIG
 */

/*
// BEGIN_CONFIG
#include "deepstomp_delay.h"
#define MODULETYPE delay
#define MODULESETUP delay_setup
// END_CONFIG
*/

//define your module data structure
MODULETYPE module;
//set up your module
int deepstomp_modulesetup()
{
	USE_DEBUG_MONITOR = 1;
	USE_LEVEL_MONITOR = 0;
	int setupstatus = MODULESETUP(&module);
	MAINMODULE = (dsthandle) &module;
	return setupstatus;
}

//call the module's processing method
void deepstomp_process(q15_t* input,q15_t* output)
{
	module.interface.process((dsthandle)&module,input,output);
}
