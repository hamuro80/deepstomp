# deepstomp
INTRODUCTION
Deepstomp is a DIY digital multi-effect stompbox/pedal processor platform
based on stm32f1 core. The platform consist of software package and hardware 
circuit design wich enable easy development of single- or multi-effect 
stompbox/pedal using the very low cost deepstomp's hardware platform.

HARDWARE FEATURES
- Pluggable Blue-Pill-Compatible CPU/Core board: ARM Cortex M3,
74MHz clock speed, 1 MB RAM, 64KB flash eprom
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
- Serial debugging display with 6-elements integer variable array to display and 
	two signal probes to display any processed signals

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

