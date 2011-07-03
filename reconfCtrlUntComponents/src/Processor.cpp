#include "Processor.h"
#include <vector>
#include <map>

using namespace std;

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
//	AR->dataOut(aRoutToMemorySignal)
	AR->dataIn(aRinSignal);
	AR->load(aRLoadSignal);
	AR->reset(resetRegisters);
	
	DR = new RegisterAssyncReset("DR");
	DR->clk(clock);
	DR->dataOut(dRoutSignal);
	DR->dataIn(dRinSignal);
	DR->load(dRLoadSignal);
	DR->reset(resetRegisters);

	dRinMux = new Multiplexer("dRinMux", 2); //entrada da ula e entrada da memória 
	dRinMux->output(dRinSignal);
	dRinMux->inputs[0](ulaDemux_to_dRinMux_signal); //saída da ula
	dRinMux->inputs[1](memoryDataInput); //entrada de dados da memória
	dRinMux->sel(dRinMuxSelSignal);

	dRoutDemux = new Demultiplexer("dRoutDemux", 2); //saída para o multiplexador da Ula, e saída para memória para escrita.
	dRoutDemux->input(dRoutSignal);
	dRoutDemux->outputs[0](ulaMux_to_dRoutMux_signal); //entrada da ula
	dRoutDemux->outputs[1](memoryDataOutput); //entrada da ula
	dRoutDemux->sel(dRoutDemuxSelSignal);

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
	raMultiplexer->inputs[1](ulaMux_to_dRoutMux_signal);
	raMultiplexer->inputs[2](pCoutSignal);
	raMultiplexer->inputs[3](iRoutSignal);
	raMultiplexer->inputs[4](rFoutSignal);
	raMultiplexer->sel(muxRaSelSignal);
	
	raMultiplexer->output(muxRaOutSignal);
	RA->dataIn(muxRaOutSignal);

	ula->opA(rAoutSignal); ////conecta a saída de RA na entrada A da ula

	rbMultiplexer = new Multiplexer("rbMultiplexer",5);
	rbMultiplexer->inputs[0](aRoutSignal);
	rbMultiplexer->inputs[1](ulaMux_to_dRoutMux_signal);
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
	ulaOutputDemultiplexer->outputs[1](ulaDemux_to_dRinMux_signal);
	ulaOutputDemultiplexer->outputs[2](pCinSignal);
	ulaOutputDemultiplexer->outputs[3](iRinSignal);
	ulaOutputDemultiplexer->outputs[4](rFinSignal);
	ulaOutputDemultiplexer->sel(demuxUlaSelSignal);
	



	vector<std::string> outputNames;
	outputNames.push_back("ulaOp");
	outputNames.push_back("ulaOutDemuxSel");
	outputNames.push_back("ulaInAMuxSel");
	outputNames.push_back("ulaInBMuxSel");
	outputNames.push_back("dRinMuxSel");
	outputNames.push_back("dRoutDemuxSel");
	outputNames.push_back("rfSel");
	outputNames.push_back("rfReadWriteBit");
	outputNames.push_back("writeMemory");
	outputNames.push_back("loadRA");
	outputNames.push_back("loadRB");
	outputNames.push_back("loadIR");
	outputNames.push_back("loadAR");
	outputNames.push_back("loadPC");
	outputNames.push_back("loadDR");

	map<string,vector<string> > stateOutputMap;

	vector<string > incPc;
	//resetAllLoads()
	incPc.push_back("rfReadWriteBit 0 writeMemory 0 loadRA 0 loadRB 0 loadIR 0 loadAR 0 loadPC 0 loadDR 0 dRinMuxSel 1 dRoutDemuxSel 0");
	incPc.push_back("ulaOp 4"); //ulaOp = ulaOperation_inc;
	incPc.push_back("ulaInAMuxSel 2"); //ulaInAMuxSel = UlaInputSelection_PC;
 	incPc.push_back("ulaOutDemuxSel 2"); //ulaOutDemuxSel = UlaOutputSelection_PC;
 	incPc.push_back("loadRA 1"); //loadRA = 1;
 	incPc.push_back("wait 1");
	incPc.push_back("loadPC 1"); //loadPC = 1;
 	incPc.push_back("wait 1");

	stateOutputMap["0,0"] = incPc;

	controlUnit = new CustomizableControlUnit("ControlUnit", outputNames, stateOutputMap);

	controlUnit->clock(clock);
//loads dos registradores
	controlUnit->assign("loadRA",&rALoadSignal);
	controlUnit->assign("loadRB",&rBLoadSignal);
	controlUnit->assign("loadIR",&iRLoadSignal);
	controlUnit->assign("loadAR",&aRLoadSignal);
	controlUnit->assign("loadPC",&pCLoadSignal);
	controlUnit->assign("loadDR",&dRLoadSignal);
	//TODO status register
//	controlUnit->loadStatus(statusLoadSignal);

	//seletor e load do RF
	controlUnit->assign("rfSel",&rFSelSignal);
	controlUnit->assign("rfReadWriteBit",&rFWriteBitSignal);

	//seletores dos muxes e demux
	controlUnit->assign("ulaInAMuxSel",&muxRaSelSignal);
	controlUnit->assign("ulaInBMuxSel",&muxRbSelSignal);
	controlUnit->assign("ulaOutDemuxSel",&demuxUlaSelSignal);
	controlUnit->assign("dRinMuxSel",&dRinMuxSelSignal);
	controlUnit->assign("dRoutDemuxSel",&dRoutDemuxSelSignal);

	//operação da ula
	controlUnit->assign("ulaOp",&ulaOpSignal);
	controlUnit->statusBit(ulaStatusSignal);

	//entrada de ir na unidade de controle
	controlUnit->iRInput(iRoutSignal);


	controlUnit->assign("writeMemory",&writeMemory);



	SC_METHOD(processorBehavior);
	sensitive  << clock.pos();
	SC_METHOD(processorBehavior2);
	sensitive  << AR->dataOut;

}

void Processor::processorBehavior2(){
	memoryAddress.write(AR->dataOut.read());

}

void Processor::processorBehavior(){
	//cout<<"arOutsignal "<<aRoutSignal.read()<<endl;
	//cout<<"arOutsignal "<<AR->dataOut.read()<<endl;
	//cout<<"memoryData "<<memoryDataInput.read()<<endl;
	//cout<<"statusBit "<<ulaStatusSignal<<endl;
	cout<<"memory data from data input "<<memoryDataInput.read()<<endl;
	cout<<"memory data from data output "<<memoryDataOutput.read()<<endl;
	cout<<" PC value val "<<pCoutSignal.read()<<endl;
	////cout<<"DRinSel " <<dRinMuxSelSignal.read()<<endl;
	////cout<<"DRoutSel " <<dRoutDemuxSelSignal.read()<<endl;

	switch (controlUnit->state){
		case 1:
			break;
		case 2:
			break;
		case 4:
			break;
	}
	//writeMemory.write(writeMemorySignal.read());
}

