
#ifndef RegisterAssyncReset_h
#define RegisterAssyncReset_h

#include <systemc.h>
#include "config.h"

SC_MODULE(RegisterAssyncReset)
{
	public:
	sc_in<bool> clk, reset;
	sc_in<sc_int<WORD_SIZE> > load;
	sc_in<sc_int<WORD_SIZE> > dataIn;
	sc_out<sc_int<WORD_SIZE> > dataOut;

	sc_int<WORD_SIZE> internalData;

	public:
	void RegisterBehaviour();
	SC_CTOR(RegisterAssyncReset){
		SC_METHOD(RegisterBehaviour);
		sensitive<<reset;
		sensitive<<clk.pos();
	}
};
#endif
