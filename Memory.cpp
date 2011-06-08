#include "memory.h"
#include "systemc.h"

void memory::t_sync_mem() {
	if(rst) {
		datao = 0x00;
		wait();
	}
	while(1) {
		if(writer) mem[addres.read()] =  (datai);
		else datao =mem[addres.read()]  ;
		wait();
	}
}
