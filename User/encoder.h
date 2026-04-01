#ifndef ENCODER_H
#define ENCODER_H

#include "main.h"

#define NUM_ENCODERS 7

void Encoder_Init(void);
void Encoder_Task(void);

extern float Joint_Angle[NUM_ENCODERS];

#endif
