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
		{ //preset 0 (0.0) - clean
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 1 (0.5) - compressed, clean
				{0,1,8,9}, //noise gate off, compressor on(4.0,9.0)
				{0,0,0,0}, //compressor attack time = 0, release time = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 2 (0.5) - compressed, noise-gated, clean
				{6,8,8,9}, //noise gate on(3.0,4.0), compressor on(4.0,9.0)
				{0,0,0,0}, //compressor attack time = 0, release time = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 3 (1.5) - compressed, noise-gated, clean, delay echoed
				{6,8,8,9}, //noise gate on(3.0,4.0), compressor on(4.0,9.0)
				{0,0,0,0}, //compressor attack time = 0, release time = 0
				{0,0,0,0}, //distortion off (type:clean)
				{18,6,9,0}  //delay (9.0, 3.0, 4.5)
		},
		{ //preset 4 (1.5) - mild over-driven
				{0,1,1,0}, //noise gate off (0.0,0.5), compressor off(0.5,0.0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{9,1,9,0}, //distortion on(4.5,1.0,4.5)
				{0,0,0,0}  //delay off
		},
		{ //preset 5 (2.5) - mild fuzz distorted
				{0,1,1,0}, //noise gate off (0.0,0.5), compressor off(0.5,0.0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{9,2,9,0}, //distortion on(4.5,2.0,4.5)
				{0,0,0,0}  //delay off
		},
		{ //preset 6 (3.0) - max fuzz distorted
				{0,1,1,0}, //noise gate off (0.0,0.5), compressor off
				{0,0,0,0}, //compressor attack = 0, release = 0
				{18,2,9,0}, //distortion on(9.0,2.0,4.5)
				{0,0,0,0}  //delay off
		},
		{ //preset 7 (3.5) - noise gated, max fuzz distorted
				{6,8,1,0}, //noise gate on(3.0,4.0), compressor off
				{0,0,0,0}, //compressor attack time = 0, release time = 0
				{18,2,9,0}, //distortion on(9.0,2.0,4.5)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 8 (4.0) - noise gated, max fuzz distorted, delay echoed
				{6,8,1,0}, //noise gate on(3.0,4.0), compressor off
				{0,0,0,0}, //compressor attack time = 0, release time = 0
				{18,2,9,0}, //distortion on(9.0,2.0,4.5)
				{18,6,9,0}  //delay (9.0, 3.0, 4.5)
		},
		{ //preset 9 (4.5) - noise gated, compressed, max fuzz distorted
				{6,8,8,9}, //noise gate on(3.0,4.0), compressor on(4.0,9.0)
				{0,0,0,0}, //compressor attack time = 0, release time = 0
				{18,2,9,0}, //distortion on(9.0,2.0,4.5)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 10 (5.0) - noise gated, compressed, max fuzz distorted, delay echoed
				{6,8,8,9}, //noise gate on(3.0,4.0), compressor on(4.0,9.0)
				{0,0,0,0}, //compressor attack time = 0, release time = 0
				{18,2,9,0}, //distortion on(9.0,2.0,4.5)
				{18,6,9,0}  //delay (9.0, 3.0, 4.5)
		},
		{ //preset 11 (5.5) - clean
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 12 (6.0) - clean
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 13 (6.5) - clean
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 14 (7.0) - clean
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 15 (7.5) - clean
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 16 (8.0) - clean
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 17 (8.5) - clean
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 18 (9.0) - clean
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		},
		{ //preset 19 (active setting layer) - clean (at default)
				{0,1,1,0}, //noise gate off (onlevel:0), compressor off(compression:0)
				{0,0,0,0}, //compressor attack = 0, release = 0
				{0,0,0,0}, //distortion off (type:clean)
				{0,0,0,0}  //delay off (balance:0)
		}
};

#endif /* DEEPSTOMP_MULTIEFFECT_H_ */
