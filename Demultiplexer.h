#ifndef Demultiplexer_h
#define Demultiplexer_h
#include <systemc.h>
#include "config.h"

SC_MODULE (Demultiplexer) {
	// Memory output, Register file output, PC, IR
	sc_in<sc_int<WORD_SIZE> > input; 
	sc_in<sc_int<WORD_SIZE> > sel;

	sc_out<sc_int<WORD_SIZE> > *outputs;

	void DemultiplexerBehaviour();

	int numOutputs;

	SC_HAS_PROCESS (Demultiplexer);

	Demultiplexer(sc_module_name name, int numOutputs) : sc_module(name){
		SC_METHOD(DemultiplexerBehaviour);
		sensitive << sel << input;
		this->numOutputs = numOutputs;
		outputs = new sc_out<sc_int<WORD_SIZE> >[numOutputs];
	}

};
#endif


