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
	
	wait(60);	
	
	sc_stop();
}

ProcessorTest::ProcessorTest(sc_module_name name): sc_module(name){

	memoryAndRegistersResetSignal.write(false); //não reset

	memoryAddressSignal.write(0);


	processor = new Processor("Processor");
	processor->clock(clock);
	processor->resetRegisters(memoryAndRegistersResetSignal);

	//valores iniciais da memoria
	int inSize = 10;
	sc_int<WORD_SIZE> *inValues = new sc_int<WORD_SIZE>[inSize];

	sc_int<WORD_SIZE> value;
	sc_int<WORD_SIZE> address;
	sc_int<2> type;
	sc_int<5> op;
	sc_int<3> dest;
	sc_int<3> src1;
	sc_int<3> src2;


/* teste de operaçoes em registrador*/
/*
	type = 0;
	op = ulaOperation_multiply;
	dest = 0;
	src1 = 1;
	src2 = 2;
	inValues[4] = (type, op, dest, src1, src2); //RF [dest] = RF[src1] + RF[src2]

	type = 0;
	op = 0;
	dest = 1;
	src1 = 0;
	src2 = 0;
	value = 20;
	inValues[0] = (type, op, dest, src1, src2); //RF [dest] = CTE
	inValues[1] = value;

	type = 0;
	op = 0;
	dest = 2;
	src1 = 0;
	src2 = 0;
	value = 10;
	inValues[2] = (type, op, dest, src1, src2); //RF [dest] = CTE
	inValues[3] = value;
*/

/* instruções de memória
	type = 1;
	op = 0;
	dest = 1;
	src1 = 0;
	src2 = 0;
	address = 49;
	inValues[0] = (type, op, dest, src1, src2); //RF [dest] = address
	inValues[1] = address;

	type = 1;
	op = 1;
	dest = 1;
	src1 = 0;
	src2 = 0;
	address = 2;
	inValues[0] = (type, op, dest, src1, src2); //RF [dest] = Mem[address]
	inValues[1] = address;
	inValues[2] = 40;


	type = 1;
	op = 2;
	dest = 1;
	src1 = 0;
	src2 = 0;
	address = 2;
	inValues[0] = (type, op, dest, src1, src2); //RF [dest] = Mem[mem[address]]
	inValues[1] = address;
	inValues[2] = 3;
	inValues[3] = 40;


	type = 0;
	op = 0;
	dest = 2;
	src1 = 0;
	src2 = 0;
	value = 10;
	inValues[0] = (type, op, dest, src1, src2); //RF [dest] = CTE
	inValues[1] = value;
 
	type = 1;
	op = 3;
	dest = 1;
	src1 = 2;
	src2 = 0;
	address = 4;
	inValues[2] = (type, op, dest, src1, src2); //Mem[address] = RF [src1]
	inValues[3] = address;

	type = 1;
	op = 4;
	dest = 0;
	src1 = 2;
	src2 = 0;
	address = 4;
	inValues[2] = (type, op, dest, src1, src2); //Mem[mem[address]] = RF[src1]
	inValues[3] = address;
	inValues[4] = 3;

	*/

/*instruçoes de miscelania*/
	type = 0;
	op = 0;
	dest = 1;
	src1 = 0;
	src2 = 0;
	value = 20;
	inValues[0] = (type, op, dest, src1, src2); //RF [dest] = CTE
	inValues[1] = value;

	type = 0;
	op = 0;
	dest = 2;
	src1 = 0;
	src2 = 0;
	value = 10;
	inValues[2] = (type, op, dest, src1, src2); //RF [dest] = CTE
	inValues[3] = value;

/*
	type = 3;
	op = ulaOperation_lt;
	dest = 0;
	src1 = 2;
	src2 = 1;
	inValues[4] = (type, op, dest, src1, src2); // statusBit = RF[src1] op RF[src2]
*/
/*operacoes de controle de fluxo*/
/*
	type = 2;
	op = 1;
	dest = 1;
	src1 = 2;
	src2 = 0;
	address = 8;
	inValues[5] = (type, op, dest, src1, src2); //if statusBit PC = address else pc = pc + 1
	inValues[6] = address;

	type = 2;
	op = 0;
	dest = 1;
	src1 = 2;
	src2 = 0;
	address = 8;
	inValues[5] = (type, op, dest, src1, src2); //PC = address
	inValues[6] = address;
*/

	memory = new Memory("Memory", inValues, inSize);
	memory->clk(clock);
	memory->rst(memoryAndRegistersResetSignal);
	memory->address(memoryAddressSignal);
	memory->datai(memoryDataISignal);
	memory->datao(memoryDataOSignal);
	memory->write(writeMemorySignal);

	processor->memoryDataInput(memoryDataOSignal);
	processor->memoryDataOutput(memoryDataISignal);
	processor->memoryAddress(memoryAddressSignal);
	processor->writeMemory(writeMemorySignal);

	SC_THREAD (do_test);
	dont_initialize();
	sensitive << clock.pos();	

}
