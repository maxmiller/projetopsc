#include "RegisterFile.h"	

void RegisterFile::RegisterFileBehaviour(){
	int rwBit_value = rwBit.read();
	if(rwBit_value == 0){
		out1 = registers[selRead1.read().to_int()];
		out2 = registers[selRead2.read().to_int()];
	}
	else{
		cout<<" writing RF["<<selWrite.read().to_int()<<"] = "<<in<<endl;
		registers[selWrite.read().to_int()] = in;
	}
}

