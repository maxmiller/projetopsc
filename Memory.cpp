#include <Memory.h>

void Memory::t_sync_mem() {
	if(rst) {
		datao = 0x00;
		wait();
	}
	while(1) {
		if(write) mem[address] = datai;
		else datao = mem[address];
		wait();
	}
}

