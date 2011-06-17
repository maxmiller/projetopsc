/*
 *  ProcessorTest.cpp
 *  projetoregistrador
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ProcessorTest.h"
#include <iostream>

using namespace std;

void ProcessorTest::do_test() {
	
	wait(10);	
	
	sc_stop();
}

ProcessorTest::ProcessorTest(sc_module_name name): sc_module(name){

	memoryAndRegistersResetSignal.write(false); //não reset

	memoryAddressSignal.write(0);


	processor = new Processor("Processor");
	processor->clock(clock);
	processor->resetRegisters(memoryAndRegistersResetSignal);

	//valores iniciais da memoria
	sc_int<WORD_SIZE> *inValues = new sc_int<WORD_SIZE>[2];

	sc_int<2> type = 0;
	sc_int<5> op = 0;
	sc_int<3> dest = 0;
	sc_int<3> src1 = 1;
	sc_int<3> src2 = 2;

	inValues[0] = (type, op, dest, src1, src2); //RF [dest] = RF[src1] + RF[src2]
	inValues[1] = 0;
	memory = new Memory("Memory", inValues, 2);

	memory->clk(clock);
	memory->rst(memoryAndRegistersResetSignal);
	memory->address(memoryAddressSignal);
	memory->datai(memoryDataISignal);
	memory->datao(memoryDataOSignal);
	memory->write(writeMemorySignal);

	processor->memoryDataInput(memoryDataOSignal);
	processor->memoryData(memoryDataISignal);
	processor->memoryAddress(memoryAddressSignal);
	processor->writeMemory(writeMemorySignal);

	SC_THREAD (do_test);
	dont_initialize();
	sensitive << clock.pos();	

}
