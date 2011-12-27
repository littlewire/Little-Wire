/*
	http://kehribar.me/projects/Little-Wire/pwm_servo.php
		Higher level servo driving library.

	Copyright (C) <2011> ihsan Kehribar <ihsan@kehribar.me>
	
	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
	of the Software, and to permit persons to whom the Software is furnished to do
	so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

*/

#include "littleWire_servo.h"
#include "stdio.h"

/********************************************************************************
* Useful definitions
********************************************************************************/
const float MIN_LIMIT = 0.45; // in miliseconds
const float MAX_LIMIT = 2.45; // in miliseconds
const float STEP_SIZE = 0.062; // in miliseconds
const float RANGE = 180.0; // in degrees
/*******************************************************************************/

/********************************************************************************
* Servo module initialization
********************************************************************************/
void servo_init(littleWire* lwHandle)
{
	initPwm(lwHandle); // Initialize the PWM hardware.
	pinMode(lwHandle,PWMA,OUTPUT); pinMode(lwHandle,PWMB,OUTPUT); // Set PWM pins output.
	updatePwmPrescale(lwHandle,1024); // Make sure the PWM prescaler is set correctly.
}
/*******************************************************************************/

/********************************************************************************
* Servo locations update
*	locationChannelA in degrees
*	locationChannelB in degrees
********************************************************************************/
void servo_updateLocation(littleWire* lwHandle,unsigned char locationChannelA,unsigned char locationChannelB)
{
	locationChannelA=(((float)((float)(locationChannelA/RANGE))*(MAX_LIMIT-MIN_LIMIT))+MIN_LIMIT)/STEP_SIZE;
	locationChannelB=(((locationChannelB/RANGE)*(MAX_LIMIT-MIN_LIMIT))+MIN_LIMIT)/STEP_SIZE;
	updatePwmCompare(lwHandle,locationChannelA,locationChannelB);
}
/*******************************************************************************/