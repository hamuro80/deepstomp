# deepstomp

DEEPSTOMP - OPEN PLATFORM FOR DIY PROGRAMMABLE DIGITAL MULTI-EFFECT PEDAL

Current Version: 1.5
AUTHOR: HAMURO
COPYRIGHT(c) 2018 DEEPTRONIC.COM

INTRODUCTION
- Deepstomp is a DIY digital multi-effect stompbox/pedal processor platform
based on stm32f1 core. The platform consist of software package and hardware 
circuit design wich enable easy development of single- or multi-effect 
stompbox/pedal using the very low cost deepstomp's hardware platform.

LICENSE
- Each source code files in the software package has its own license 
	in its copyright notice at the top of the source code, which we believe 
	that they has the most flexible open-source licensing, that allow everyone
	to use them for their proprietary commercial product, 
	not limiting only for open-source products. Please let me know 
	if there's any file in this distrbution that has a limiting license 
	that prevent anyone to use it in non open-source project/product 
	by contacting me at my email: hamuro at vidisonic dot com.
- The Deepstomp's hardware design (Deepstomp's main board circuit) can be used
	in any commercial or open-source product, with or without modification.
	Everyone is allowed to produce Deepstomp hardware under their own brand
	or trademark.

HARDWARE FEATURES
- Pluggable Blue-Pill-Compatible CPU/Core board: ARM Cortex M3,
72MHz clock speed, 20 kB RAM, 64KB flash eprom
- mono input and output
- 44.1 kHz sampling rate
- 16-bit output using cascaded double 8-bit PWM with auto-calibration 
- 15-bit input using multi-channel expansion oversampling
- 10 LEDs for parameter value/signal indicator display
- 6 LEDs for control/parameter selection indicator
- Input level knob to ensure optimum input conversion
- Output level knob to balance the processed output and the bypassed signal
- Rotary encoder with click-push button to access all operation menus

HARDWARE FUTURE WORKS
- Adding output buffer after level control for better output noise-resistance

SOFTWARE FEATURES
- True real-time, buffered only 1 sample (1/44100 seconds) for precessing
	while doing input and output conversion
- Support guitar (or general musical instrument) tuner 
	with +-(1/12) semitone precision (+- 0.45% frequency deviation)
- Support single-effect mode with 6 controllable parameters
- Support multi-effect mode (max 10 effect modules)
- Support 19 custom programmable presets which can be reset to default 
	(factory setting) independently for each preset
- Support 10-steps, 19-steps, and 38-steps parameters control.
- Easy-to-use modular framework with object-oriented-like C programming style
	with fully managed user interface, so you can focus on the signal processing algorithm
- Using free IDE: System Workbench for STM32 (SW4STM32)

CURRENTLY IMPLEMENTED CORE FUNCTION
- Audio input-output
- Guitar (or general musical instrument) tuner
- Parameter control and preset management for single- and multi-effect modes
- Automatic calibration for 16-bit PWM output
- Calibration operation mode for calibrating the ADC expansion circuitry
- Serial debugging display to monitor some internal parameters and signal probes

IMPLEMENTED EFFECT MODULES
- Expansor, an integrated noise gate expander and compressor 
	with syncronized gate-on level, gate-off level, compression-start,
	compression-stop, and compression factor, compression attack,
	and compression-release controls.
- Distortion, featured with overdrive mode, fuzz mode, and hi-lo tone control
- Delay, featured with time, feedback, and balance control

SOFTWARE FUTURE WORKS
- Implement multi-modal mode
- Adding on/off toggle interface for module management for better memory management
- Implement expression pedal extension through serial port with MIDI protocol

CHANGE NOTES

Version 1.5
- Changed CPU overload indication and dtection method, now the overload is	indicated by the blinking of the last 
 LED of the indicator bar in the normal operation state (where it should normally show the signal level)

Version 1.4
- Hardware improvement from v1.1. to v1.2
- Added semitones display type for module's controllable parameter
- Added guitar (or general musical instrument) tuner
- Fixed last accessed module after storing or restoring the preset at any location
- Added user manual at https://www.deepstomp.com/deepstomps-user-manual/

Version 1.3
- Added CPU overload detection and indication

Version 1.2
- Added manual ADC calibration operation when device is switched on while the rotary switch is pressed and hold
- Added CPU overload detection and indication to prevent user interface hanging up and friendly debugging.
	The overload is	indicated by the blinking of the first 3-LEDs in the LED bar in the 
	normal operation (where it should normally show the signal level)
- Compiler preprocessor for debug monitor has been changed to 
	USE_DEBUG_MONITOR and USE_LEVEL_MONITOR variables for easier setup and 
	implemented with dynamic memory allocation for saving RAM space
Version 1.1
  - Modified hardware schematic, fully tested
Version 1.0
- Initial version

