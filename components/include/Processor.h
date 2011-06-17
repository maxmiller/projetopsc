#ifndef Processor_h
#define Processor_h

#include <systemc.h>
#include "config.h"

#include "RegisterAssyncReset.h"
#include "ControlUnit.h"
#include "RegisterFile.h"
#include "Multiplexer.h"
#include "Demultiplexer.h"
#include "ULA.h"

SC_MODULE(Processor){

	//entradas
	sc_in<bool> clock;  //0
	sc_in<bool> resetRegisters; //1
	//! se conecta com a entrada de DR
	sc_in<sc_int<WORD_SIZE> > memoryDataInput; //2

	//saídas
	sc_out<bool> writeMemory; // 0 para read, 1 para write //3

	//! se conecta com a saída de DR
	sc_inout<sc_int<WORD_SIZE> > memoryData; //4
	//! se conecta com a saída de AR
	sc_out<sc_int<WORD_SIZE> > memoryAddress; //5

	sc_signal<bool> writeMemorySignal;

	//registradores
	RegisterAssyncReset *RA;
	RegisterAssyncReset *RB;
	RegisterAssyncReset *AR;
	RegisterAssyncReset *DR;
	RegisterAssyncReset *PC;
	RegisterAssyncReset *IR;

	//register file
	RegisterFile *registerFile;
	sc_signal<sc_int<WORD_SIZE> > rFSelSignal;
	sc_signal<bool > rFWriteBitSignal;
	sc_signal<sc_int<WORD_SIZE> > rFoutSignal;
	sc_signal<sc_int<WORD_SIZE> > rFinSignal;


	//seletores
	Multiplexer *raMultiplexer;
	sc_signal<sc_int<WORD_SIZE> > muxRaOutSignal;
	sc_signal<sc_int<WORD_SIZE> > muxRaSelSignal;

	Multiplexer *rbMultiplexer;
	sc_signal<sc_int<WORD_SIZE> > muxRbOutSignal;
	sc_signal<sc_int<WORD_SIZE> > muxRbSelSignal;

	Demultiplexer *ulaOutputDemultiplexer;
	sc_signal<sc_int<WORD_SIZE> > demuxUlaInSignal;
	sc_signal<sc_int<WORD_SIZE> > demuxUlaOutSignal;
	sc_signal<sc_int<WORD_SIZE> > demuxUlaSelSignal;

	//ula
	ULA *ula;
	sc_signal<sc_int<WORD_SIZE> > ulaOpSignal;
	sc_signal<bool > ulaStatusSignal;

	
	//unidade de controle
	ControlUnit *controlUnit;

	sc_signal<sc_int<WORD_SIZE> > rAoutSignal;
	sc_signal<sc_int<WORD_SIZE> > rBoutSignal;
	sc_signal<sc_int<WORD_SIZE> > aRoutSignal;
	sc_signal<sc_int<WORD_SIZE> > dRoutSignal;
	sc_signal<sc_int<WORD_SIZE> > pCoutSignal;
	sc_signal<sc_int<WORD_SIZE> > iRoutSignal;

	sc_signal<sc_int<WORD_SIZE> > rAinSignal;
	sc_signal<sc_int<WORD_SIZE> > rBinSignal;
	sc_signal<sc_int<WORD_SIZE> > aRinSignal;
	sc_signal<sc_int<WORD_SIZE> > dRinSignal;
	sc_signal<sc_int<WORD_SIZE> > pCinSignal;
	sc_signal<sc_int<WORD_SIZE> > iRinSignal;

	sc_signal<bool > rALoadSignal;
	sc_signal<bool > rBLoadSignal;
	sc_signal<bool > aRLoadSignal;
	sc_signal<bool > dRLoadSignal;
	sc_signal<bool > pCLoadSignal;
	sc_signal<bool > iRLoadSignal;




	SC_CTOR(Processor);

	void processorBehavior();
};
#endif
