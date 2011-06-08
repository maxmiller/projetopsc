#include "Demultiplexer.h"

void Demultiplexer::DemultiplexerBehaviour(){
	if(sel.read().to_int() < numInputs)
		outputs[sel.read().to_int()] = input;
	else
		outputs[0] = input;
}
