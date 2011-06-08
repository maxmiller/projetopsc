
#ifndef RegisterAssyncReset_h
#define RegisterAssyncReset_h

#include <systemc.h>
#include "config.h"

SC_MODULE(RegisterAssyncReset)
{
	public:
	sc_in<bool> clk, reset, load;
	sc_in<sc_uint<WORD_SIZE> > dataIn;
	sc_out<sc_uint<WORD_SIZE> > dataOut;

	sc_uint<WORD_SIZE> internalData;

	public:
	void RegisterBehaviour();
	SC_CTOR(RegisterAssyncReset){
		SC_METHOD(RegisterBehaviour);
		sensitive<<reset;
		sensitive<<clk.pos();
	}
};
#endif
