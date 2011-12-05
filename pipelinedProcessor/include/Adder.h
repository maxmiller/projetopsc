#ifndef Adder_h
#define Adder_h
#include <systemc.h>
#include "config.h"

SC_MODULE (Adder) {
	public:
		int addValue;
		sc_in<sc_int<WORD_SIZE> > in;
		sc_out<sc_int<WORD_SIZE> > out;

		void adderBehavior();

		SC_CTOR(Adder){
			SC_METHOD(adderBehavior);
			sensitive << in;
		}

};
#endif


