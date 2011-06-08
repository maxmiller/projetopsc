#include "ULA.h"

void ULA::ULABehaviour(){
	switch(opSel.read().to_int()){
		case ulaOperation_add: //sum A + B
			ulaOut = opA.read() + opB.read();
			break;
		case ulaOperation_multiply: //mult
			ulaOut = opA.read() * opB.read();
			break;
		case ulaOperation_and: //and
			ulaOut = opA.read() & opB.read();
			break;
		case ulaOperation_inc: //inc A
			ulaOut = opA.read() + 1;
			break;
		case ulaOperation_doNothing: //do nothing
			ulaOut = opA.read();
			break;
	}	
}
