#ifndef Multiplexer_h
#define Multiplexer_h
#include <systemc.h>

SC_MODULE (Multiplexer) {
	// Memory output, Register file output, PC, IR
	sc_in<sc_int<WORD_SIZE>> inputs[4]; 
	sc_in<sc_int<WORD_SIZE>> sel;

	sc_out<sc_int<WORD_SIZE>> output;

	void MultiplexerBehaviour();

	SC_CTOR(Multiplexer){
		SC_METHOD(MultiplexerBehaviour);
		sensitive << sel;
		for(int i=0;i<4;i++)
			sensitive<<inputs[i];
	}

};
#endif


