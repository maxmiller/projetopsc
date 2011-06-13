#include "systemc.h"

#ifndef RULA_h
#define RULA_h

SC_MODULE(memory) {
public:
	//portas
	sc_in<bool>  rst,writer,clk;
	sc_in<sc_uint<32> > address;
	sc_in<sc_uint<32> > datai;
	sc_out<sc_uint<32> > datao;
	//internos
	sc_signal<sc_uint<32> > mem[32];
	
public:
	void t_sync_mem();
	
	SC_CTOR(memory) {
		SC_CTHREAD(t_sync_mem, clk.pos());
		//SC_METHOD(t_sync_mem);
		sensitive << rst << writer;
		sensitive << clk.pos();
		int size=0;
	    for (size = 0; size < 32; size++) {
			mem[size].write( 0x0 );
			
        }
		
		//sensitive << writer;
	//	watching(rst.delayed());
	}
};

#endif
