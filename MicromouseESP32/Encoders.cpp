#include "Encoders.h"
#include "Config.h"
static volatile long lc=0,rc=0;
static void IRAM_ATTR leftISR(){lc += (digitalRead(L_A)==digitalRead(L_B))?1:-1;}
static void IRAM_ATTR rightISR(){rc += (digitalRead(R_A)==digitalRead(R_B))?1:-1;}
void Encoders::begin(){pinMode(L_A,INPUT_PULLUP);pinMode(L_B,INPUT_PULLUP);pinMode(R_A,INPUT_PULLUP);pinMode(R_B,INPUT_PULLUP);attachInterrupt(digitalPinToInterrupt(L_A),leftISR,CHANGE);attachInterrupt(digitalPinToInterrupt(R_A),rightISR,CHANGE);}
void Encoders::reset(){noInterrupts();lc=rc=0;interrupts();}
long Encoders::left(){noInterrupts();long n=lc;interrupts();return n;}
long Encoders::right(){noInterrupts();long n=rc;interrupts();return n;}
