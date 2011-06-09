/*
 *  RegisterTest.h
 *  projetoregistrador
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "systemc.h"
#include "Multiplexer.h"
#include "config.h"

#ifndef MultiplexerTest_h
#define MultiplexerTest_h

SC_MODULE(MultiplexerTest) {
	
	sc_in<bool> clock;
	sc_signal<sc_int<WORD_SIZE> > *inputs; 
	sc_signal<sc_int<WORD_SIZE> > sel;
	
	sc_signal<sc_int<WORD_SIZE> > output;
	
		
	Multiplexer *mult;
	
	void do_test();
	SC_CTOR(MultiplexerTest){
		mult = new Multiplexer("Multiplexer",2);
		inputs = new sc_signal< sc_int<WORD_SIZE> > [2];
		mult->inputs[0](inputs[0]);
		mult->inputs[1](inputs[1]);
		mult->sel(sel);
		mult->output(output);
		
		SC_THREAD (do_test);
		dont_initialize();
		sensitive << clock.pos();
		
	}
	
};

#endif