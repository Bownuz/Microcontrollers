#ifndef ULTRASONE_H
#define ULTRASONE_H

#include <avr/io.h>

void ultrasoneInit();
void sendTrigger();
uint16_t measurePulse();
float calculateDistance(float pulseDuration);

#endif 
