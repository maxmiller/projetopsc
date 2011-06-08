#ifndef Memory_h
#define Memory_h
#include <systemc.h>
#include "config.h"

SC_MODULE(Memory) {
	public:
		//portas
		sc_in<bool> clk, rst, write;
		sc_in<sc_uint<WORD_SIZE> > address;
		sc_in<sc_uint<WORD_SIZE> > datai;
		sc_out<sc_uint<WORD_SIZE> > datao;
		//internos
		sc_uint<WORD_SIZE> mem[MEMORY_SIZE];
	public:
		void t_sync_mem();

		SC_CTOR(Memory) {
			SC_CTHREAD(t_sync_mem, clk.pos());
		}
};

#endif
