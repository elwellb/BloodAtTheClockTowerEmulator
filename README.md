README Mech&Art



***********************************************
***************Project Scope*******************
***********************************************
Our Mechatronics and Art project will be used to control the ambience of the scene for the Blood on the Clocktower social deception game.
Included in this project is the controls which give the "Ghost of the Story Teller" the ability to change the lighting all around the room from their phone.



***********************************************
********Packages, Software & Hardware**********
***********************************************

ESP32 Microcontrroller


PlatformIO - Visual Studio Code

OSC Library by Adrian Freed
FastLED Library by Daniel Garcia

CleanOSC App (iOS) - https://drive.google.com/file/d/17lonoMOboATWMAYjYpn-0LgKWG9CXQJQ/view?usp=sharing

***********************************************
*****************Functions:********************
***********************************************

Lights Video: https://drive.google.com/file/d/141De7I2WXfT_pGKDUq8cdS1eSMpls4IO/view?usp=sharing

Setup: Allows for the connection of board to phone via OSC and initializes the LEDs with address

Flicker: The main control function, adjusts brightness and uses a switch state to tell lights what to do

Loop: Read in OSC messages and calls functions below to update paramters, then calls the Flicker function

WipeOn & WipeOff: Functions will be used to control the animation of lights sweeping on and lights sweeping off

button_msg: Function will read in toggle command and send the system to the proper state of WipeOn or WipeOff (Turning on and off the lights)

brightness_msg: Function will use a slider to contol brightness of lights. Limited from 27% to 66% 

saturation_msg: Function will use a slider to allow for a white light surge

stepper_msg: Function will use the stepper buttons to control the colors of the lights ranging from 40 (Yellow) to 3 (Red)
