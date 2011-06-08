#include "systemc.h"

#ifndef RULA_h
#define RULA_h

SC_MODULE(memory) {
public:
	//portas
	sc_in<bool> clk, rst, writer;
	sc_in<sc_uint<8> > addres;
	sc_in<sc_uint<8> > datai;
	sc_out<sc_uint<8> > datao;
	//internos
	sc_uint<8> mem[32];
	
public:
	void t_sync_mem();
	
	SC_CTOR(memory) {
		SC_CTHREAD(t_sync_mem, clk.pos());
		sensitive << rst;
		sensitive << clk.pos();
		//sensitive << writer;
	//	watching(rst.delayed());
	}
};

#endif
