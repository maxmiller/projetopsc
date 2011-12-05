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
#include "Adder.h"
#include "BusSpliter.h"

SC_MODULE(Processor){

	//entradas
	sc_in<bool> clock;  //0
	sc_in<bool> resetRegisters_input; //1
	//! se conecta com a entrada de DR
	sc_in<sc_int<WORD_SIZE> > programMemoryDataI;
	sc_out<sc_int<WORD_SIZE> > programMemoryDataO;
	sc_out<sc_int<WORD_SIZE> > programMemoryAddress;
	sc_out<sc_int<WORD_SIZE> > programMemoryRWBit; // 0 para read, 1 para write //3

	sc_in<sc_int<WORD_SIZE> > dataMemoryDataI; //2
	sc_out<sc_int<WORD_SIZE> > dataMemoryDataO; //2
	sc_out<sc_int<WORD_SIZE> > dataMemoryAddress; //2
	sc_out<sc_int<WORD_SIZE> > dataMemoryRWBit; // 0 para read, 1 para write //3

	//saídas

	//registradores
	//DR
	RegisterAssyncReset *DR;
	sc_signal<sc_int<WORD_SIZE> > DRdataOut2registerFileInputMux_inputs1;
	sc_signal<sc_int<WORD_SIZE> > controlUnit2DR_load;
	
	//PC
	RegisterAssyncReset *PC;
	sc_signal<sc_int<WORD_SIZE> > pcInputMux2PC_dataIn;
	sc_signal<sc_int<WORD_SIZE> > controlUnit2PC_load;

	//IR
	RegisterAssyncReset *IR;
	sc_signal<sc_int<WORD_SIZE> > controlUnit2IR_dataOut;
	sc_signal<sc_int<WORD_SIZE> > controlUnit2IR_load;
	
	//pipeline registers

	//multiplexers
	//pcInputMux
	Multiplexer *ulaOpBMux;
	sc_signal<sc_int<WORD_SIZE> > controlUnit2ulaOpBMux;
	sc_signal<sc_int<WORD_SIZE> > ulaOpBMux_output2ula_opB;

	Multiplexer *pcInputMux;
	sc_signal<sc_int<WORD_SIZE> > pcInputMux_inputs02ula_ulaOut;
	sc_signal<sc_int<WORD_SIZE> > pcInputMux_inputs02pcAdder_out;
	sc_signal<sc_int<WORD_SIZE> > controlUnit2pcInputMux_sel;	
	
	//registerFileInputMux
	Multiplexer *registerFileInputMux;
	sc_signal<sc_int<WORD_SIZE> > ula_ulaOut2registerFileInputMux_inputs0;
	sc_signal<sc_int<WORD_SIZE> > DR_dataOut2registerFileInputMux_inputs1;
	sc_signal<sc_int<WORD_SIZE> > registerFileInputMux_output2registerFile_in;
	sc_signal<sc_int<WORD_SIZE> > controlUnit2registerFileInputMux_sel;

	//register file
	RegisterFile *registerFile;
	sc_signal<sc_int<WORD_SIZE> > controlUnit2registerFile_selRead1;
	sc_signal<sc_int<WORD_SIZE> > controlUnit2registerFile_selRead2;
	sc_signal<sc_int<WORD_SIZE> > controlUnit2registerFile_selWrite;
	sc_signal<sc_int<WORD_SIZE> > registerFile_out12ula_opA;
	sc_signal<sc_int<WORD_SIZE> > registerFile_out22ula_opB;
	sc_signal<sc_int<WORD_SIZE> > controlUnit2registerFile_rwBit;

	//ula
	ULA *ula;
	sc_signal<sc_int<WORD_SIZE> > controlUnit2ula_opSel;
	sc_signal<sc_int<WORD_SIZE> > controlUnit2ula_ulaStatus;

	//adder
	Adder *pcAdder;

	//bus spliters
	BusSpliter *registerFileSel2BusSpliter;
	sc_signal<sc_int<WORD_SIZE> > registerFileSel2BusSpliter2ulaOpBMux_input1;
	sc_signal<sc_int<WORD_SIZE> > registerFileSel2BusSpliter2registerFile_selRead2;

	BusSpliter *ulaBusSpliter;
	sc_signal<sc_int<WORD_SIZE> > ula_ulaOut2ulaBusSpliter_input;

	BusSpliter *registerFileOutBusSpliter;
	sc_signal<sc_int<WORD_SIZE> > registerFile_out22registerFileOutBusSpliter_input;

	//unidade de controle
	CustomizableControlUnit *controlUnit;


	SC_CTOR(Processor);

	void processorBehavior();
	void processorBehavior2();
};
#endif
