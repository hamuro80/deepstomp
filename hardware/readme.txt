Deepstomp's main board schematic board is shown in the deepstomp-main-board-schematic.png. This schematic diagram has no component reference yet, you can assign the reference as you design the PCB yourself.

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
- One 40 pins female header socket for plugging you deepstomp CPU board


CPU Board
The CPU board is a blue-pill compatible board, as shown in the deepstomp-cpu-board-stm32f1-blue-pill.png file. The STLINK port and the USB port are not used, so you can omit them if you design your own custom Deepstomp CPU board.

