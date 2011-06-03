#ifndef Demultiplexer_h
#define Demultiplexer_h
#include <systemc.h>

SC_MODULE (Demultiplexer) {
	// Memory output, Register file output, PC, IR
	sc_in<sc_int<WORD_SIZE>> input; 
	sc_in<sc_int<WORD_SIZE>> sel;

	sc_out<sc_int<WORD_SIZE>> outputs[4];

	void DemultiplexerBehaviour();

	SC_CTOR(Demultiplexer){
		SC_METHOD(DemultiplexerBehaviour);
		sensitive << sel << input;
	}

};
#endif


