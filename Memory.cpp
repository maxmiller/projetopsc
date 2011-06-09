#include "memory.h"
#include "systemc.h"

void memory::t_sync_mem() {
	if(rst.read()==1) {
		datao = 0x00;
		wait();
	}
	while(1) {
		if(writer){
			mem[address.read()] =  (datai);
		}
		else{
			datao.write(mem[address.read()])  ;
		}
		wait();
	}
	
	/*if(rst.read()==1){
	 if(writer.read() == 1) {            //write
	 mem[address.read().to_int()].write( data.read() );
	 
	 } else {                        //read
	 data.write(mem[address.read().to_int()].read());
	 
	 }
	 }
	 else {
	 data.write("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
	 
	 }*/
	
}
