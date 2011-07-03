/*
 *  ProcessorTest.h
 *  projetoregistrador
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ProcessorTest_h
#define ProcessorTest_h
#include "systemc.h"
#include "Memory.h"
#include "Processor.h"
#include "config.h"


SC_MODULE(ProcessorTest) {
	
	sc_in<bool> clock;
	
	sc_signal<sc_int<WORD_SIZE> > memoryAddressSignal;
	sc_signal<sc_int<WORD_SIZE> > memoryDataISignal;
	sc_signal<sc_int<WORD_SIZE> > memoryDataOSignal;
	sc_signal<sc_int<WORD_SIZE> > writeMemorySignal;
	sc_signal<bool> memoryAndRegistersResetSignal;

	Memory *memory;
	Processor * processor;
	
	void do_test();
	SC_CTOR(ProcessorTest);
	
};

#endif
