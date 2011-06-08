#include "Memory.h"

void Memory::t_sync_mem() {
	if(rst) {
		datao = 0x00;
		wait();
	}
	while(1) {
		if(write) mem[address.read().to_int()] = datai;
		else datao = mem[address.read().to_int()];
		wait();
	}
}

