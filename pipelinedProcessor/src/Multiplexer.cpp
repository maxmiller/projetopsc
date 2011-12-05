#include "Multiplexer.h"

void Multiplexer::multiplexerBehavior(){
	if(sel.read().to_int() < numInputs){
		output = inputs[sel.read().to_int()];
	}
	else
		output = inputs[0];
}
