/*
 *  RegisterTest.h
 *  projetoregistrador
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "systemc.h"
#include "RegisterAssyncReset.h"
#include "config.h"

#ifndef RegisterAssyncTest_h
#define RegisterAssyncTest_h

SC_MODULE(RegisterAssyncTest) {
	
	sc_in<bool> clock;
	
	sc_signal<bool>  reset, load;
	sc_signal<sc_uint<WORD_SIZE> > dataIn;
	sc_signal<sc_uint<WORD_SIZE> > dataOut;
	
	
	
	
	RegisterAssyncReset *reg;
	
	void do_test();
	SC_CTOR(RegisterAssyncTest){
		reg = new RegisterAssyncReset("RegisterAssyncReset");
		reg->clk(clock);
		reg->reset(reset);
		reg->load(load);
		reg->dataIn(dataIn);
		reg->dataOut(dataOut);
		
		SC_THREAD (do_test);
		dont_initialize();
		sensitive << clock.pos();
		
	}
	
};

#endif