/*
 *  RegisterTest.h
 *  projetoregistrador
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "systemc.h"
#include "Demultiplexer.h"
#include "config.h"

#ifndef DemultiplexerTest_h
#define DemultiplexerTest_h

SC_MODULE(DemultiplexerTest) {
	
	sc_in<bool> clock;
	sc_signal<sc_int<WORD_SIZE> > input; 
	sc_signal<sc_int<WORD_SIZE> > sel;
	
	sc_signal<sc_int<WORD_SIZE> > *outputs;
	
	
	Demultiplexer *mult;
	
	void do_test();
	SC_CTOR(DemultiplexerTest){
		mult = new Demultiplexer("Demultiplexer",2);
		outputs = new sc_signal<sc_int<WORD_SIZE> > [2];
		
		mult->input(input);
		mult->sel(sel);
		mult->outputs[0](outputs[0]);
		mult->outputs[1](outputs[1]);
		
		SC_THREAD (do_test);
		dont_initialize();
		sensitive << clock.pos();
		
	}
	
};

#endif
