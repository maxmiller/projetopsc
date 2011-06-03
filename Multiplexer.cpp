#include "Multiplexer.h"

Multiplexer::MultiplexerBehaviour(){
	if(sel < 4)
		output = inputs[sel];
	else
		output = inputs[0];
}
