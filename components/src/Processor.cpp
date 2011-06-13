#include "Processor.h"

Processor::Processor(sc_module_name name){
	//SC_THREAD(processorBehaviour);

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
	AR->load(aRLoadSignal);
	AR->reset(resetRegisters);
	
	DR = new RegisterAssyncReset("DR");
	DR->clk(clock);
	DR->dataOut(dRoutSignal);
	DR->load(dRLoadSignal);
	DR->reset(resetRegisters);

	PC = new RegisterAssyncReset("PC");
	PC->clk(clock);
	PC->dataOut(pCoutSignal);
	PC->load(pCLoadSignal);
	PC->reset(resetRegisters);

	IR = new RegisterAssyncReset("IR");
	IR->clk(clock);
	IR->dataOut(iRoutSignal);
	IR->load(iRLoadSignal);
	IR->reset(resetRegisters);

	ula = new ULA("Ula");
	ula->ulaOut(demuxUlaInSignal);
	ula->opSel(ulaOpSignal);
	ula->ulaStatus(ulaStatusSignal);


	registerFile = new RegisterFile("RegisterFile");
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

	raMultiplexer->output(muxRaOutSignal);
	RA->dataIn(muxRaOutSignal);

	ula->opA(rAoutSignal); ////conecta a saída de RA na entrada A da ula

	rbMultiplexer = new Multiplexer("rbMultiplexer",5);
	rbMultiplexer->inputs[0](aRoutSignal);
	rbMultiplexer->inputs[1](dRoutSignal);
	rbMultiplexer->inputs[2](pCoutSignal);
	rbMultiplexer->inputs[3](iRoutSignal);
	raMultiplexer->inputs[4](rFoutSignal);

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

	sensitive << clock;

	controlUnit->writeMemory(writeMemorySignal);
	writeMemory(writeMemorySignal);


	//deve dar problema
	memoryData(dRoutSignal);
	memoryDataInput(dRinSignal);
	memoryAddress(aRoutSignal);


}

