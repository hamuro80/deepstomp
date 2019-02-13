/*
 * deepstomp.h
 * Description: Deepstomp application programming interface
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

#ifndef DEEPSTOMP_H_
#define DEEPSTOMP_H_

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <arm_math.h>

/* ####################################################################################
 * DEBUGGING FLAGS
 * Set to 0 to disable, set to 1 to enable
 * Use USE_LEVEL_MONITOR wisely, it consumes significant CPU cycles, activate only the
 * debugged module if necessary. USE_LEVEL_MONITOR won't work if USE_DEBUGMONITOR = 0
 */
int8_t USE_DEBUG_MONITOR;
int8_t USE_LEVEL_MONITOR;

/* ####################################################################################
 * DEBUGGING API
 * Use them in any effect module only for debugging purpose
 */
void debugSetText(char* text);
void debugSetVar(int32_t var, uint8_t index);	//index = 0..6
void debugDetectLevel(q15_t signal, uint8_t channel);	//channel = 0 or 1

/* ####################################################################################
 * GENERAL USEFUL CONSTANTS AND DEFINITIONS
 */
// maximum values
#define Q15MAX	32767
#define Q15MIN	-32767
#define Q31MAX	2147483647
#define Q31MIN	-2147483647

// 10 points exponential steps
static q15_t const EXPSTEP10[] =
{
	0,109,222,454,926,1889,3855,7867,16056,32767
};

// 19 points exponential steps
static q15_t const EXPSTEP19[] =
{
	0,76,109,156,222,317,454,648,926,1322,1889,
	2699,3855,5507,7867,11239,16056,22937,32767
};

/* ####################################################################################
 * ENUMERATED CONSTANTS
 */
/* dyplaytype_* controls how a parameter is displayed by deepstomp's 10-LEDs bar
 * currently only *bar10, *dot10, *dot19, and *dot38 are supported
 */
enum {displaytype_bar10,displaytype_dot10,displaytype_dot19,
		displaytype_dot38,diplaytype_semitones,displaytype_frequency,
		displaytype_period,displaytype_binarydigit};

// return value for module's setup function
enum {setup_error,setup_ok};

/* ####################################################################################
 * NEW TYPE DEFINITION
 */
// handle for referring deepstomp object instance
typedef uint32_t dsthandle;

// pointer to a function that should be executed when changing parameter's value
typedef int8_t (*paramchangecall)(dsthandle handle,int8_t value);

// pointer to a module's audio processing function
typedef void (*dstprocess)(dsthandle handle,q15_t* input,q15_t* output);


// deepstomp module's parameter, manageable by the system's user interface
typedef struct
{
	char* name;		// for debugging or advanced display in the future development
	int8_t value;	// the value of the parameter
	int8_t stepcount;	// ranges from 2-38
	int8_t displaytype;	// refer to displaytype_* enumerated constants
	paramchangecall onchange;	//function pointer to call when the interface core changes the value
}
dstparam;

// module interface, it should be the first element of any module's data structure
typedef struct
{
	char* modulename;	// name your effect module
	uint8_t multimode;	// 0: single-effect mode, 1: multi-effect mode
	dstprocess process;	// a pointer to the audio processing function
	uint8_t paramcount;	// number of controllable parameter max = 6
}
dstinterface;

// single-effect mode module typedef, a pointer of this structure type will be used
// by the core to manage any single-effect module
// this data type should never be used to define new data instance directly except
// the maximum number of 6 dstparams is needed, use it as pointer for reference only
typedef struct
{
	dstinterface interface;
	dstparam params[6];		//support 6 parameters at maximum
} dstsinglemodule;

// multi-effect mode module typedef, a pointer of this structure type will be used
// by the core to manage any multi-effect module
// this data type should never be used to define new data instance directly except
// the maximum number of 10 modules is needed, use it as pointer for reference only
typedef struct
{
	dstinterface interface;
	dstparam control[4];
	dstparam module;
	dstparam preset;
	dstsinglemodule*  modulehandles[10];	//support 10 modules at maximum
} dstmultimodule;

/* ####################################################################################
 * GLOBAL VARIABLES
 */
//handle of main effect module, should be set at deepstomp_configure function
dsthandle MAINMODULE;

//default preset for multi-effect mode, should be defined in multi-effect module
__weak int8_t const DEFAULTPRESET[20][10][4];

// module setup function, should be defined in deepstomp.c
// this function will be called by main core first before loading any parameters
__weak int deepstomp_modulesetup();

// processing function, should be defined in deepstomp.c
// this function will be called by main core at every audio sample reading
// (at 44.1 kHz sampling rate).
__weak void deepstomp_process(q15_t* input,q15_t* output);


#endif /* DEEPSTOMP_H_ */
