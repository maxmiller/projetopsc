#include "memory.h"
#include "systemc.h"

void memory::t_sync_mem() {
	/*if(rst.read()==1) {
	 data = 0x00;
	 wait();
	 }
	 while(1) {
	 if(writer) mem[addres.read()] =  (datai);
	 else datao =mem[addres.read()]  ;
	 wait();
	 }*/
	
	if(rst.read()==1){
		if(writer.read() == 1) {            //write
			mem[addres.read().to_int()].write( data.read() );
			
		} else {                        //read
			data.write(mem[addres.read().to_int()].read());
			
		}
	}
	else {
		data.write("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
		
	}
	
}
