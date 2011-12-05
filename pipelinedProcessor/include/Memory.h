#ifndef Memory_h
#define Memory_h
#include <systemc.h>
#include "config.h"

SC_MODULE(Memory) {
	public:
		//portas
		sc_in<bool> clk, rst;
	 	sc_in<sc_int<WORD_SIZE> >	write;
		sc_in<sc_int<WORD_SIZE> > address;
		sc_out<sc_int<WORD_SIZE> > datao;
		sc_in<sc_int<WORD_SIZE> > datai;

		//internos
		sc_int<WORD_SIZE> mem[MEMORY_SIZE];
	public:
		void t_sync_mem();
		SC_HAS_PROCESS (Memory);
		Memory(sc_module_name name, sc_int<WORD_SIZE> *inValues=NULL, int numValues=0);
};

#endif
