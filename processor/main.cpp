
#include <systemc.h>
#include "config.h"
#include "Memory.h"
#include "Processor.h"

int main(){

	sc_clock clk ("my_clock",1,0.5);
	sc_signal<sc_int<WORD_SIZE> > memoryAddressSignal;
	sc_signal<sc_int<WORD_SIZE> > memoryDataISignal;
	sc_signal<sc_int<WORD_SIZE> > memoryDataOSignal;
	sc_signal<bool > writeMemorySignal;
	sc_signal<bool> memoryAndRegistersResetSignal;
	memoryAndRegistersResetSignal.write(false); //não reset



	Processor * processor = new Processor("Processor");
	processor->clock(clk);
	processor->resetRegisters(memoryAndRegistersResetSignal);

	Memory *memory = new Memory("Memory");
	memory->clk(clk);
	memory->rst(memoryAndRegistersResetSignal);
	memory->address(memoryAddressSignal);
	memory->datai(memoryDataISignal);
	memory->datao(memoryDataOSignal);
	memory->write(writeMemorySignal);

	processor->memoryDataInput(memoryDataOSignal);
	processor->memoryData(memoryDataISignal);
	processor->memoryAddress(memoryAddressSignal);
	processor->writeMemory(writeMemorySignal);


		return 0;
}
