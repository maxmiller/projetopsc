#include "ULA.h"

void ULA::ULABehaviour(){
	switch(opSel){
		case ulaOperation_add: //sum A + B
			ulaOut = opA + opB;
			break;
		case ulaOperation_multiply: //mult
			ulaOut = opA * opB;
			break;
		case ulaOperation_and: //and
			ulaOut = opA & opB;
			break;
		case ulaOperation_inc: //inc A
			ulaOut = opA + 1;
			break;
		case ulaOperation_doNothing: //do nothing
			ulaOut = opA;
			break;
	}	
}
