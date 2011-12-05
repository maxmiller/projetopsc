
#ifndef RegisterFile_h
#define RegisterFile_h

#include <systemc.h>
#include "RegisterAssyncReset.h"
#include "config.h"

SC_MODULE (RegisterFile) {
	public:
		sc_in<bool> clock;
		
		//seleciona qual endereço será lido/escrito
		sc_in< sc_int<WORD_SIZE> > selRead1;
		sc_in< sc_int<WORD_SIZE> > selRead2;
		//se true write, se false read
		sc_in< sc_int<WORD_SIZE> > rwBit;
		
		sc_in< sc_int<WORD_SIZE> > selWrite;
		sc_in<sc_int<WORD_SIZE> > in;
		sc_out<sc_int<WORD_SIZE> > out1;
		sc_out<sc_int<WORD_SIZE> > out2;

		sc_int<WORD_SIZE> registers[RF_SIZE];

	public:
		void RegisterFileBehaviour();

		SC_CTOR(RegisterFile){
			SC_METHOD(RegisterFileBehaviour);
			sensitive << clock.pos();
		}
};

#endif
