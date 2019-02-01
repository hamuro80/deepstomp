DEEPSTOMP MAIN BOARD
Deepstomp's main board schematic board is shown in the deepstomp-main-board-schematic.png. This schematic diagram has no component reference yet, so you can assign the references yourself when designing your own PCB.

The standard features of this main bord is:
- Power supply input: 9V DC (negative at the outer circle/tube and ground at the center)
- One LED for power indicator
- One LED for bypass-state check
- Six LEDs for control/params/module/preset indicator
- Ten LEDs for indicator bar to show parameter values
- One power switch to turn on/off and power-on-reset the device
- One prog/run switch to select programming or running operation when switched-on
- One rotary encoder with a click-push-button for user interface
- One input level potentiometer to ensure optimal input conversion
- One output level potentiometer to ensure the balance between processed output and bypassed input volume.
- One 40 pins female header socket for plugging the deepstomp CPU board

Change Notes:
Version 1.0 (deepstomp-main-board-schematic.gif)
- First published, modified from unpublished prototype, the unpublished circuit failed to meet noise-level restriction, and this published version is not tested
Version 1.1 (deepstomp-main-board-schematic-1.1.gif)
- Added component reference
- Changed some components values
- Successfully tested

DEEPSTOMP CPU BOARD
The CPU board is a blue-pill compatible board, as shown in the deepstomp-cpu-board-stm32f1-blue-pill.png file. The STLINK port and the USB port are not used, so you can omit them if you design your own custom PCU board.

