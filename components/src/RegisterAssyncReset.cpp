
#include "RegisterAssyncReset.h"

void RegisterAssyncReset::RegisterBehaviour(){
	bool reset_val = reset.read();
	if(reset_val){ // if reset == true reset output
		internalData = 0;
	}
	else if(load){ //else loads the input to de output
		cout<<"loading internal data "<<dataIn<<" to register "<<this->name()<<endl;
		internalData = dataIn;
	}
	dataOut.write(internalData);
}


