
#include "RegisterAssyncReset.h"

void RegisterAssyncReset::RegisterBehaviour(){
	bool reset_val = reset.read();
	int load_val = load.read();
	if(reset_val){ // if reset == true reset output
		internalData = 0;
	}
	else if(load_val == 1){ //else loads the input to de output
		internalData = dataIn;
	}
	dataOut.write(internalData);
}


