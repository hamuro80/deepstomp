# deepstomp

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
72MHz clock speed, 1 MB RAM, 64KB flash eprom
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
- designing and testing power-on silencer circuit to mute the 
	automatic calibration noise on power switching-on.

SOFTWARE FEATURES
- True real-time, buffered only 1 sample (1/44100 seconds) for precessing
	while doing input and output conversion
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
- Parameter control and preset management for single- and multi-effect modes
- Automatic calibration for 16-bit PWM output
- Serial debugging display to monitor some internal parameters and signal probes

CORE FUNCTION IN PROGRESS
- Manual ADC calibration operation mode

CORE FUNCTION FUTURE OWRKS
- Guitar tunner

IMPLEMENTED EFFECT MODULES
- Expansor, an integrated noise gate expander and compressor 
	with syncronized gate-on level, gate-off level, compression-start,
	compression-stop, and compression factor, compression attack,
	and compression-release controls.
- Distortion, featured with overdrive mode, fuzz mode, and hi-lo tone control
- Delay, featured with time, feedback, and balance control

CHANGE NOTES
Version 1.1
  - Modified hardware schematic, fully tested
Version 1.0
- Initial version

