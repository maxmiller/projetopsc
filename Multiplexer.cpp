#include "Multiplexer.h"

void Multiplexer::MultiplexerBehaviour(){
	if(sel.read().to_int() < numOutputs)
		output = inputs[sel.read().to_int()];
	else
		output = inputs[0];
}
