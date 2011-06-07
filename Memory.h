#ifndef RULA_h
#define RULA_h
#include "config.h"

SC_MODULE(Memory) {
	public:
		//portas
		sc_in<bool> clk, rst, write;
		sc_in<sc_uint<WORD_SIZE> > address;
		sc_in<sc_uint<WORD_SIZE> > datai;
		sc_out<sc_uint<WORD_SIZE> > datao;
		//internos
		sc_uint<sc_uint<WORD_SIZE> > mem[MEMORY_SIZE];
	public:
		void t_sync_mem();

		SC_CTOR(sync_mem) {
			SC_CTHREAD(t_sync_mem, clk.pos());
			watching(rst.delayed());
		}
};

#endif
