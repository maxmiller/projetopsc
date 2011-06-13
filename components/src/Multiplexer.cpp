#include "Multiplexer.h"

void Multiplexer::MultiplexerBehaviour(){
	if(sel.read().to_int() < numInputs)
		output = inputs[sel.read().to_int()];
	else
		output = inputs[0];
}
