#include "Processor.h"

Processor::Processor(sc_module_name name) : sc_module(name){

	RA = new RegisterAssyncReset("RA");
	RA->clk(clock);
	RA->dataOut(rAoutSignal);
	RA->load(rALoadSignal);
	RA->reset(resetRegisters);

	RB = new RegisterAssyncReset("RB");
	RB->clk(clock);
	RB->dataOut(rBoutSignal);
	RB->load(rBLoadSignal);
	RB->reset(resetRegisters);

	AR = new RegisterAssyncReset("AR");
	AR->clk(clock);
	AR->dataOut(aRoutSignal);
//	AR->dataOut(aRoutToMemorySignal);
	AR->dataIn(aRinSignal);
	AR->load(aRLoadSignal);
	AR->reset(resetRegisters);
	
	DR = new RegisterAssyncReset("DR");
	DR->clk(clock);
	DR->dataOut(dRoutSignal);
	DR->dataIn(dRinSignal);
	DR->load(dRLoadSignal);
	DR->reset(resetRegisters);

	PC = new RegisterAssyncReset("PC");
	PC->clk(clock);
	PC->dataOut(pCoutSignal);
	PC->dataIn(pCinSignal);
	PC->load(pCLoadSignal);
	PC->reset(resetRegisters);

	IR = new RegisterAssyncReset("IR");
	IR->clk(clock);
	IR->dataOut(iRoutSignal);
	IR->dataIn(iRinSignal);
	IR->load(iRLoadSignal);
	IR->reset(resetRegisters);

	ula = new ULA("Ula");
	ula->ulaOut(demuxUlaOutSignal);
	ula->opSel(ulaOpSignal);
	ula->ulaStatus(ulaStatusSignal);


	registerFile = new RegisterFile("RegisterFile");
	registerFile->clock(clock);
	registerFile->sel(rFSelSignal);
	registerFile->rwBit(rFWriteBitSignal);
	registerFile->out(rFoutSignal);
	registerFile->in(rFinSignal);


	raMultiplexer = new Multiplexer("raMultiplexer",5);
	raMultiplexer->inputs[0](aRoutSignal);
	raMultiplexer->inputs[1](dRoutSignal);
	raMultiplexer->inputs[2](pCoutSignal);
	raMultiplexer->inputs[3](iRoutSignal);
	raMultiplexer->inputs[4](rFoutSignal);
	raMultiplexer->sel(muxRaSelSignal);
	
	raMultiplexer->output(muxRaOutSignal);
	RA->dataIn(muxRaOutSignal);

	ula->opA(rAoutSignal); ////conecta a saída de RA na entrada A da ula

	rbMultiplexer = new Multiplexer("rbMultiplexer",5);
	rbMultiplexer->inputs[0](aRoutSignal);
	rbMultiplexer->inputs[1](dRoutSignal);
	rbMultiplexer->inputs[2](pCoutSignal);
	rbMultiplexer->inputs[3](iRoutSignal);
	rbMultiplexer->inputs[4](rFoutSignal);
	rbMultiplexer->sel(muxRbSelSignal);

	rbMultiplexer->output(muxRbOutSignal);
	RB->dataIn(muxRbOutSignal);
	
	ula->opB(rBoutSignal); //conecta a saída de RB na entrada B da ula
	
	ulaOutputDemultiplexer = new Demultiplexer("ulaOutputDemultiplexer", 5);
	
	ulaOutputDemultiplexer->input(demuxUlaOutSignal); //conecta a saída da ula na entrada do demux
	
	ulaOutputDemultiplexer->outputs[0](aRinSignal);
	ulaOutputDemultiplexer->outputs[1](dRinSignal);
	ulaOutputDemultiplexer->outputs[2](pCinSignal);
	ulaOutputDemultiplexer->outputs[3](iRinSignal);
	ulaOutputDemultiplexer->outputs[4](rFinSignal);
	ulaOutputDemultiplexer->sel(demuxUlaSelSignal);
	



	controlUnit = new ControlUnit("ControlUnit");
	controlUnit->clock(clock);
//loads dos registradores
	controlUnit->loadRA(rALoadSignal);
	controlUnit->loadRB(rBLoadSignal);
	controlUnit->loadIR(iRLoadSignal);
	controlUnit->loadAR(aRLoadSignal);
	controlUnit->loadPC(pCLoadSignal);
	controlUnit->loadDR(dRLoadSignal);
	//TODO status register
//	controlUnit->loadStatus(statusLoadSignal);

	//seletor e load do RF
	controlUnit->rfSel(rFSelSignal);
	controlUnit->rfReadWriteBit(rFWriteBitSignal);

	//seletores dos muxes e demux
	controlUnit->ulaInAMuxSel(muxRaSelSignal);
	controlUnit->ulaInBMuxSel(muxRbSelSignal);
	controlUnit->ulaOutDemuxSel(demuxUlaSelSignal);

	//operação da ula
	controlUnit->ulaOp(ulaOpSignal);
	controlUnit->statusBit(ulaStatusSignal);

	//entrada de ir na unidade de controle
	controlUnit->iRInput(iRoutSignal);


	controlUnit->writeMemory(writeMemorySignal);

	SC_METHOD(processorBehavior);
	sensitive  << memoryDataInput << writeMemorySignal
						 << aRoutSignal << dRoutSignal;
	
	
}

void Processor::processorBehavior(){
	memoryAddress.write(aRoutSignal.read());
	writeMemory.write(writeMemorySignal.read());

	if(writeMemorySignal.read()){ //escrever na memoria
		memoryData.write(dRoutSignal.read());
	}
	else{ //ler da memoria e colocar em DR
		dRinSignal.write(memoryDataInput.read());
	}
}

