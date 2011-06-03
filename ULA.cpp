#include "ULA.h"

void ULA::ULABehaviour(){
	switch(opSel){
		case 0: //sum A + B
			ulaOut = opA + opB;
			break;
		case 1: //mult
			ulaOut = opA * opB;
			break;
		case 2: //and
			ulaOut = opA & opB;
			break;
		case 3: //inc A
			ulaOut = opA + 1;
			break;
	}	
}
