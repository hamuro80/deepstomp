/*
 * deepstom_multieffect.h
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

#ifndef DEEPSTOMP_MULTIEFFECT_H_
#define DEEPSTOMP_MULTIEFFECT_H_

#include <deepstomp.h>
#include <deepstomp_expansor.h>
#include <deepstomp_distortion.h>
#include <deepstomp_delay.h>
#include <deepstomp_modextension.h>


typedef struct
{
	dstinterface interface;
	dstparam control[4];
	dstparam module;
	dstparam preset;
	dstsinglemodule* modulehandles[4];
	expansor expansormodule;
	modextension expansorextmodule;
	distortion distortionmodule;
	delay delaymodule;
} multieffect;

int multieffect_setup(multieffect* handle);

int8_t const DEFAULTPRESET[20][10][4]=
{
		{ //preset 0 (0.0)
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 1 (0.5)
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{9,1,9,0}, //distortion level = 4.5, overdrive (type:1), tone:4.5
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 2 (1.0)
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{18,9,9,0}  //190 ms (delay:4), feedback: 4.5, balance: 4.5
		},
		{ //preset 3 (1.5)
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{9,1,9,0}, //distortion level = 4.5, overdrive (type:1), tone:4.5
				{8,9,9,0}  //80 ms (delay:4), feedback: 4.5, balance: 4.5
		},
		{ //preset 4 (2.0)
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 5 (2.5)
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 6
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 7
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 8
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 9
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 10
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 11
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 12
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 13
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 14
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 15
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 16
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 17
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 18
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 19, temporary preset layer, the last active setting storage
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		}
};

#endif /* DEEPSTOMP_MULTIEFFECT_H_ */
