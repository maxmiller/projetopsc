#ifndef ULA_h
#define ULA_h
#include <systemc.h>
#include "config.h"

SC_MODULE (ULA) {
	sc_in<sc_int<WORD_SIZE> > opA;
	sc_in<sc_int<WORD_SIZE> > opB;
	sc_in<sc_int<WORD_SIZE> > opSel;

	sc_out<sc_int<WORD_SIZE> > ulaOut;
	sc_out<bool> ulaStatus;

	void ULABehavior();

	SC_CTOR(ULA){
		SC_METHOD(ULABehavior);
		sensitive << opA << opB << opSel;
	}

};
#endif


