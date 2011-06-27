#include "Memory.h"
#include "systemc.h"
Memory::Memory(sc_module_name name, sc_int<WORD_SIZE> *inValues, int numValues) : sc_module(name){
	SC_THREAD(t_sync_mem);
	sensitive<<clk.pos();

	for(int i=0; i<numValues; i++){
		mem[i] = inValues[i];
	}
}

void Memory::t_sync_mem() {
	if(rst.read()==1) {
		datao = 0x00;
		wait();
	}
	while(1) {
		if(write){
			//cout<<"memory write"<<endl;
			mem[address.read()] =  (datai);
		}
		else{
			datao.write(mem[address.read()]);
			//cout<<"memory data written to out "<<mem[address.read()]<<endl;
		}
		wait(1);
	}	
}
