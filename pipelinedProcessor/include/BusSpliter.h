#ifndef BusSpliter_h
#define BusSpliter_h
#include <systemc.h>
#include "config.h"

SC_MODULE (BusSpliter) {
	// Memory output, Register file output, PC, IR
	sc_in<sc_int<WORD_SIZE> > input; 

	sc_out<sc_int<WORD_SIZE> > *outputs;

	int numOutputs;

	void busSpliterBehavior();

	SC_HAS_PROCESS (BusSpliter);
	BusSpliter(sc_module_name name, int numOutputs) : sc_module(name){
		SC_METHOD(busSpliterBehavior);
		outputs = new sc_out<sc_int<WORD_SIZE> >[numOutputs];
		this->numOutputs = numOutputs;
		sensitive<<input;
	}

};
#endif


