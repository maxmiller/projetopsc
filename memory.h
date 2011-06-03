#include "systemc.h"

SC_MODULE(sync_mem) {
//portas
	sc_in<bool> clk, rst, write;
	sc_in<sc_uint<10> > address;
	sc_in<sc_uint<8> > datai;
	sc_out<sc_uint<8> > datao;
//internos
	sc_uint<sc_uint<8> > mem[32];
	void t_sync_mem() {
		if(rst) {
			datao = 0x00;
			wait();
		}
		while(1) {
			if(write) mem[address] = datai;
			else datao = mem[address];
			wait();
		}
	}
	SC_CTOR(sync_mem) {
		SC_CTHREAD(t_sync_mem, clk.pos());
		watching(rst.delayed());
	}
}