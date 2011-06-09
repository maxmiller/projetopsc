/*
 *  RegisterTest.h
 *  projetoregistrador
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "systemc.h"
#include "RegisterFile.h"
#include "config.h"

#ifndef RegisterTest_h
#define RegisterTest_h

SC_MODULE(RegisterTest) {
	
	sc_in<bool> clock;
	
	//seleciona qual endereço será lido/escrito
	sc_signal< sc_int<WORD_SIZE> > sel;
	//se true write, se false read
	sc_signal<bool> rwBit;
	
	sc_signal<sc_int<WORD_SIZE> > in;
	sc_signal<sc_int<WORD_SIZE> > out;
	
	RegisterFile *rs;
	
	void do_test();
	SC_CTOR(RegisterTest){
		rs = new RegisterFile("RegisterFile");
		
		rs->sel(sel);
		rs->in(in);
		rs->out(out);
		rs->rwBit(rwBit);
		rs->clock(clock);
		
		SC_THREAD (do_test);
		dont_initialize();
		sensitive << clock.pos();
		
	}
	
};

#endif