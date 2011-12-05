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

	sc_signal<sc_int<WORD_SIZE> > programMemoryAddressSignal;
	sc_signal<sc_int<WORD_SIZE> > programMemoryDataISignal;
	sc_signal<sc_int<WORD_SIZE> > programMemoryDataOSignal;
	sc_signal<sc_int<WORD_SIZE> > programMemoryRWSignal;

	sc_signal<sc_int<WORD_SIZE> > dataMemoryAddressSignal;
	sc_signal<sc_int<WORD_SIZE> > dataMemoryDataISignal;
	sc_signal<sc_int<WORD_SIZE> > dataMemoryDataOSignal;
	sc_signal<sc_int<WORD_SIZE> > dataMemoryRWSignal;
	sc_signal<bool> memoryAndRegistersResetSignal;

	Memory *programMemory;
	Memory *dataMemory;
	Processor * processor;
	
	void do_test();
	SC_CTOR(ProcessorTest);
	
};

#endif
