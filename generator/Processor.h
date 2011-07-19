#ifndef Processor_h
#define Processor_h

#include <systemc.h>
#include "config.h"

#include "RegisterAssyncReset.h"
#include "CustomizableControlUnit.h"
#include "RegisterFile.h"
#include "Multiplexer.h"
#include "Demultiplexer.h"
#include "ULA.h"

SC_MODULE(Processor){

	//components
	//Conjunto de registradores 
RegisterAssyncReset *RA;
RegisterAssyncReset *RB;
RegisterAssyncReset *AR;
RegisterAssyncReset *DR;
RegisterAssyncReset *PC;
RegisterAssyncReset *IR;
RegisterFile *registerFile;
Multiplexer *raMultiplexer;
Multiplexer *dRInMux;
Demultiplexer *dROutDemux;
Multiplexer *rbMultiplexer;
Demultiplexer *ulaOutputDemultiplexer;
ULA *ula;
CustomizableControlUnit *controlUnit;


	//Signals 
sc_signal<sc_int<WORD_SIZE> >aRoutToMemorySignal;
sc_signal<sc_int<WORD_SIZE> >dRinMuxSelSignal;
sc_signal<sc_int<WORD_SIZE> >dRoutDemux;
sc_signal<sc_int<WORD_SIZE> >rFSelSignal;
sc_signal<sc_int<WORD_SIZE> >rFoutSignal;
sc_signal<sc_int<WORD_SIZE> >rFinSignal;
sc_signal<sc_int<WORD_SIZE> >muxRaOutSignal;
sc_signal<sc_int<WORD_SIZE> >muxRaSelSignal;
sc_signal<sc_int<WORD_SIZE> >muxRbOutSignal;
sc_signal<sc_int<WORD_SIZE> >muxRbSelSignal;
sc_signal<sc_int<WORD_SIZE> >demuxUlaInSignal;
sc_signal<sc_int<WORD_SIZE> >demuxUlaSelSignal;
sc_signal<sc_int<WORD_SIZE> >demuxUlaOutSignal;
sc_signal<sc_int<WORD_SIZE> >ulaOpSignal;
sc_signal<sc_int<WORD_SIZE> >rAoutSignal;
sc_signal<sc_int<WORD_SIZE> >rBoutSignal;
sc_signal<sc_int<WORD_SIZE> >aRoutSignal;
sc_signal<sc_int<WORD_SIZE> >dRoutSignal;
sc_signal<sc_int<WORD_SIZE> >pCoutSignal;
sc_signal<sc_int<WORD_SIZE> >iRoutSignal;
sc_signal<sc_int<WORD_SIZE> >rAinSignal;
sc_signal<sc_int<WORD_SIZE> >rBinSignal;
sc_signal<sc_int<WORD_SIZE> >aRinSignal;
sc_signal<sc_int<WORD_SIZE> >dRinSignal;
sc_signal<sc_int<WORD_SIZE> >pCinSignal;
sc_signal<sc_int<WORD_SIZE> >iRinSignal;
sc_signal<bool >ulaStatusSignal;
sc_signal<sc_int<WORD_SIZE> >writeMemorySignal;
sc_signal<sc_int<WORD_SIZE> >rALoadSignal;
sc_signal<sc_int<WORD_SIZE> >rBLoadSignal;
sc_signal<sc_int<WORD_SIZE> >aRLoadSignal;
sc_signal<sc_int<WORD_SIZE> >dRLoadSignal;
sc_signal<sc_int<WORD_SIZE> >pCLoadSignal;
sc_signal<sc_int<WORD_SIZE> >iRLoadSignal;
sc_signal<sc_int<WORD_SIZE> >ula_Mux_to_dRoutMux_signal;
sc_signal<sc_int<WORD_SIZE> >ula_Mux_to_dRintMux_signal;
sc_signal<sc_int<WORD_SIZE> >ulaInAMuxSelSignal;
sc_signal<sc_int<WORD_SIZE> >ulaInBMuxSelSignal;
sc_signal<sc_int<WORD_SIZE> >ulaOutDemuxSelSignal;
sc_signal<sc_int<WORD_SIZE> >rfSelSignal;
sc_signal<sc_int<WORD_SIZE> >dROutDemuxSelSignal;
sc_signal<sc_int<WORD_SIZE> >dRoutMuxSelSignal;
sc_signal<sc_int<WORD_SIZE> >demuxUlaOutSingal;
sc_signal<sc_int<WORD_SIZE> >rFWriteBitSignal;
sc_signal<sc_int<WORD_SIZE> >rfReadWriteBitSignal;

	
	//Inputs 
sc_in<sc_int<WORD_SIZE> >memoryDataInput;
sc_in<bool >clock;
sc_in<bool >resetRegister;

	
	//Outputs 
sc_out<sc_int<WORD_SIZE> >memoryDataOutput;
sc_out<sc_int<WORD_SIZE> >writeMemory;
sc_out<sc_int<WORD_SIZE> >memoryAddress;

		
	SC_CTOR(Processor);
	
	void processorBehavior();
	void processorBehavior2();
};
#endif
