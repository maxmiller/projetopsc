#ifndef Processor_h
#define Processor_h

#include <systemc.h>
#include "config.h"

#include "RegisterAssyncReset.h"
#include "Multiplexer.h"
#include "Demultiplexer.h"
#include "ULA.h"

SC_MODULE(Processor){

	//registradores
	RegisterAssyncReset *RA;
	RegisterAssyncReset *RB;
	RegisterAssyncReset *AR;
	RegisterAssyncReset *DR;
	RegisterAssyncReset *PC;
	RegisterAssyncReset *IR;

	//register file
	RegisterFile *registerFile;

	//seletores
	Multiplexer *raMultiplexer;
	Multiplexer *rbMultiplexer;
	Demultiplexer *ulaOutputDemultiplexer;

	//ula
	ULA *ula;

	//entradas
	sc_in<bool> clock;
	//! se conecta com a entrada de DR
	sc_in<sc_int<WORD_SIZE> > memoryDataInput;

	//saídas
	sc_out<bool> writeMemory; // 0 para read, 1 para write
	//! se conecta com a saída de DR
	sc_out<sc_int<WORD_SIZE> > memoryData;
	//! se conecta com a saída de AR
	sc_out<sc_int<WORD_SIZE> > memoryAddress;

	//unidade de controle
	ControlUnit *controlUnit;

	sc_signal<sc_uint<WORD_SIZE> > rAoutSignal;
	sc_signal<sc_uint<WORD_SIZE> > rBoutSignal;
	sc_signal<sc_uint<WORD_SIZE> > aRoutSignal;
	sc_signal<sc_uint<WORD_SIZE> > dRoutSignal;
	sc_signal<sc_uint<WORD_SIZE> > pCoutSignal;
	sc_signal<sc_uint<WORD_SIZE> > iRoutSignal;

	sc_signal<sc_uint<WORD_SIZE> > rAinSignal;
	sc_signal<sc_uint<WORD_SIZE> > rBinSignal;
	sc_signal<sc_uint<WORD_SIZE> > aRinSignal;
	sc_signal<sc_uint<WORD_SIZE> > dRinSignal;
	sc_signal<sc_uint<WORD_SIZE> > pCinSignal;
	sc_signal<sc_uint<WORD_SIZE> > iRinSignal;


	sc_signal<sc_uint<WORD_SIZE> > muxRaOutSignal;
	sc_signal<sc_uint<WORD_SIZE> > muxRbOutSignal;
	
	sc_signal<sc_uint<WORD_SIZE> > demuxUlaOutSignal;
	void processorBehaviour();
	SC_CTOR(Processor);
};
#endif
