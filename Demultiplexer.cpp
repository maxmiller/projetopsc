#include "Demultiplexer.h"

Demultiplexer::DemultiplexerBehaviour(){
	if(sel < 4)
		outputs[sel] = input;
	else
		outputs[0] = input;
}
