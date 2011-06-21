#ifndef Multiplexer_h
#define Multiplexer_h
#include <systemc.h>
#include "config.h"

SC_MODULE (Multiplexer) {
	// Memory output, Register file output, PC, IR
	sc_in<sc_int<WORD_SIZE> > *inputs; 
	sc_in<sc_int<WORD_SIZE> > sel;

	sc_out<sc_int<WORD_SIZE> > output;

	int numInputs;

	void multiplexerBehavior();

	SC_HAS_PROCESS (Multiplexer);
	Multiplexer(sc_module_name name, int numInputs) : sc_module(name){
		SC_METHOD(multiplexerBehavior);
		sensitive << sel;
		inputs = new sc_in<sc_int<WORD_SIZE> >[numInputs];
		this->numInputs = numInputs;
		for(int i=0;i<numInputs;i++)
			sensitive<<inputs[i];
	}

};
#endif


