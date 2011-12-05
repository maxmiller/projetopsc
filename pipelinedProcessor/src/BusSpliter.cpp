#include "BusSpliter.h"
void BusSpliter::busSpliterBehavior(){
	for (int i=0;i<numOutputs; i++)
		outputs[i] = input.read();
}
