/*
 *  RegisterTest.h
 *  projetoregistrador
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "systemc.h"
#include "ULA.h"
#include "config.h"

#ifndef ULATest_h
#define ULATest_h

SC_MODULE(ULATest) {
	
	sc_in<bool> clock;
	
	sc_signal<sc_int<WORD_SIZE> > opA;
	sc_signal<sc_int<WORD_SIZE> > opB;
	sc_signal<sc_uint<WORD_SIZE> > opSel;
	
	sc_signal<sc_int<WORD_SIZE> > ulaOut;
	
	
	ULA *ula;
	
	void do_test();
	SC_CTOR(ULATest){
		ula = new ULA("ULA");
		
		ula->opA(opA);
		ula->opB(opB);
		ula->opSel(opSel);
		ula->ulaOut(ulaOut);
				
		SC_THREAD (do_test);
		dont_initialize();
		sensitive << clock.pos();
		
	}
	
};

#endif