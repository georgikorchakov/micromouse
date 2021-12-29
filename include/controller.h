#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include "header.h"
#include "sensors.h"

// This library provides us a regular interupt capabilities
#include <IntervalTimer.h>
IntervalTimer myTimer;

// This function will be executed with the interupts 
// from the IntervalTimer.h library on every 1 milisecond
void systick();


#endif // CONTROLLER_H