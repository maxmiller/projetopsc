#include "RegisterFile.h"	

void RegisterFile::RegisterFileBehaviour(){
	if(!rwBit)
		out = registers[sel.read().to_int()];
	else
		registers[sel.read().to_int()] = in;
}

