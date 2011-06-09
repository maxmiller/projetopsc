/*
 *  memorytest.h
 *  projeto
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "systemc.h"
#include "memory.h"

#ifndef MemoryTest_h
#define MemoryTest_h

SC_MODULE(MemoryTest) {
	
	sc_signal <bool > rst,writer;
	sc_in<bool> clk;
	sc_signal <sc_uint < 32 > >address;
	sc_signal <sc_uint < 32 > >datai;
	sc_signal <sc_uint < 32 > >datao;
	
	memory *mem;
	
	void do_test();
	SC_CTOR(MemoryTest){
		mem = new memory("memory");
		
		mem->rst(rst);
		mem->clk(clk);
		mem->writer(writer);
		mem->address(address);
		mem->datai(datai);
		mem->datao(datao);
		
		SC_THREAD (do_test);
		dont_initialize();
		sensitive << clk.pos();
		
	}
    	
};

#endif