#include "Processor.h"

Processor::Processor(){
	SC_THREAD(processorBehaviour);

	RA = new RegisterAssyncReset("RA");
	RA->clk(clock);
	RA->dataOut(rAoutSignal);
	RB = new RegisterAssyncReset("RB");
	RB->clk(clock);
	RB->dataOut(rBoutSignal);
	AR = new RegisterAssyncReset("AR");
	AR->clk(clock);
	AR->dataOut(aRoutSignal);
	DR = new RegisterAssyncReset("DR");
	DR->clk(clock);
	DR->dataOut(dRoutSignal);
	PC = new RegisterAssyncReset("PC");
	PC->clk(clock);
	PC->dataOut(pCoutSignal);
	IR = new RegisterAssyncReset("IR");
	IR->clk(clock);
	IR->dataOut(iRoutSignal);

	ula = new ULA("Ula");
	ula->ulaOut(demuxUlaInSignal);


	raMultiplexer = new Multiplexer("raMultiplexer",4);
	raMultiplexer->inputs[0](aRoutSignal);
	raMultiplexer->inputs[1](dRoutSignal);
	raMultiplexer->inputs[2](pCoutSignal);
	raMultiplexer->inputs[3](iRoutSignal);

	raMultiplexer->output(muxRaOutSignal);
	RA->input(muxRaOutSignal);

	ula->opA(rAoutSignal); ////conecta a saída de RA na entrada A da ula

	rbMultiplexer = new Multiplexer("rbMultiplexer",4);
	rbMultiplexer->inputs[0](aRoutSignal);
	rbMultiplexer->inputs[1](dRoutSignal);
	rbMultiplexer->inputs[2](pCoutSignal);
	rbMultiplexer->inputs[3](iRoutSignal);

	rbMultiplexer->output(muxRbOutSignal);
	RB->input(muxRbOutSignal);
	
	ula->opB(rBoutSignal); //conecta a saída de RB na entrada B da ula
	
	ulaOutputDemultiplexer = new Demultiplexer("ulaOutputDemultiplexer", 4);
	
	ulaOutputDemultiplexer->input(demuxUlaOutSignal); //conecta a saída da ula na entrada do demux
	
	ulaOutputDemultiplexer->outputs[0](aRinSignal);
	ulaOutputDemultiplexer->outputs[1](dRinSignal);
	ulaOutputDemultiplexer->outputs[2](pCinSignal);
	ulaOutputDemultiplexer->outputs[3](iRinSignal);


	controlUnit = new ControlUnit("ControlUnit");
	controlUnit->clock(clock);
	sensitive << clock;


}

Processor::processorBehaviour(){
		
}
