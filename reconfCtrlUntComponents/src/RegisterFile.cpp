#include "RegisterFile.h"	

void RegisterFile::RegisterFileBehaviour(){
	int rwBit_value = rwBit.read();
	if(rwBit_value == 0){
		out = registers[sel.read().to_int()];
	}
	else{
		registers[sel.read().to_int()] = in;
	}
}

