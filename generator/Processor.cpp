#include "Processor.h"
#include <vector>
#include <map>

using namespace std;

Processor::Processor(sc_module_name name) : sc_module(name){
	//SC_THREAD(processorBehaviour);
	vector <std::string> outputNames;
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
{ /*microinstructions for 0_0begin */
vector<string > microInstructions;
microInstructions.push_back("rfSel dest");
/*microinstructions for macro loadsARWithPc*/
microInstructions.push_back("ulaOp 5 ulaInAMuxSel 2 ulaOutDemuxSel 0 writeMemory 0 dRinMuxSel 1 dRoutDemuxSel 0 loadRA 1 wait 1 loadAR 1 wait 1 wait 1 loadDR 1 wait 1");
/*microinstructions for macro resetAllLoads*/
microInstructions.push_back("rfReadWriteBit 0 writeMemory 0 loadRA 0 loadRB 0 loadIR 0 loadAR 0 loadPC 0 loadDR 0 dRinMuxSel 1 dRoutDemuxSel 0");
/*microinstructions for macro incrementPC*/
microInstructions.push_back("rfReadWriteBit 0 writeMemory 0 loadRA 0 loadRB 0 loadIR 0 loadAR 0 loadPC 0 loadDR 0 dRinMuxSel 1 dRoutDemuxSel 0 ulaOp 4 ulaInAMuxSel 2 ulaOutDemuxSel 2 loadRA 1 wait 1 loadPC 1 wait 1");
/*microinstructions for macro resetAllLoads*/
microInstructions.push_back("rfReadWriteBit 0 writeMemory 0 loadRA 0 loadRB 0 loadIR 0 loadAR 0 loadPC 0 loadDR 0 dRinMuxSel 1 dRoutDemuxSel 0");
microInstructions.push_back("ulaOp 5 ulaInAMuxSel 1 loadRA 1");
microInstructions.push_back("wait 1");
microInstructions.push_back("ulaOutDemuxSel 4 rfReadWriteBit 1");
microInstructions.push_back("wait 1");
stateOutputMap["0_0"] = microInstructions;
} /*microinstructions for 0_0 end*/
{ /*microinstructions for 2_1begin */
vector<string > microInstructions;
microInstructions.push_back("if statusBit");
/*microinstructions for macro loadsARWithPc*/
microInstructions.push_back("ulaOp 5 ulaInAMuxSel 2 ulaOutDemuxSel 0 writeMemory 0 dRinMuxSel 1 dRoutDemuxSel 0 loadRA 1 wait 1 loadAR 1 wait 1 wait 1 loadDR 1 wait 1");
/*microinstructions for macro resetAllLoads*/
microInstructions.push_back("rfReadWriteBit 0 writeMemory 0 loadRA 0 loadRB 0 loadIR 0 loadAR 0 loadPC 0 loadDR 0 dRinMuxSel 1 dRoutDemuxSel 0");
microInstructions.push_back("ulaOp 5 ulaInAMuxSel 1 loadRA 1");
microInstructions.push_back("wait 1");
microInstructions.push_back("ulaOutDemuxSel 2 loadPC 1");
microInstructions.push_back("wait 1");
microInstructions.push_back("else statusBit");
/*microinstructions for macro incrementPC*/
microInstructions.push_back("rfReadWriteBit 0 writeMemory 0 loadRA 0 loadRB 0 loadIR 0 loadAR 0 loadPC 0 loadDR 0 dRinMuxSel 1 dRoutDemuxSel 0 ulaOp 4 ulaInAMuxSel 2 ulaOutDemuxSel 2 loadRA 1 wait 1 loadPC 1 wait 1");
stateOutputMap["2_1"] = microInstructions;
} /*microinstructions for 2_1 end*/
{ /*microinstructions for fetchInstructionbegin */
vector<string > microInstructions;
/*microinstructions for macro resetAllLoads*/
microInstructions.push_back("rfReadWriteBit 0 writeMemory 0 loadRA 0 loadRB 0 loadIR 0 loadAR 0 loadPC 0 loadDR 0 dRinMuxSel 1 dRoutDemuxSel 0");
/*microinstructions for macro loadsARWithPc*/
microInstructions.push_back("ulaOp 5 ulaInAMuxSel 2 ulaOutDemuxSel 0 writeMemory 0 dRinMuxSel 1 dRoutDemuxSel 0 loadRA 1 wait 1 loadAR 1 wait 1 wait 1 loadDR 1 wait 1");
microInstructions.push_back("ulaOp 5 ulaInAMuxSel 1 ulaOutDemuxSel 3 loadRA 1");
microInstructions.push_back("wait 1");
microInstructions.push_back("loadIR 1");
microInstructions.push_back("wait 1");
/*microinstructions for macro incrementPC*/
microInstructions.push_back("rfReadWriteBit 0 writeMemory 0 loadRA 0 loadRB 0 loadIR 0 loadAR 0 loadPC 0 loadDR 0 dRinMuxSel 1 dRoutDemuxSel 0 ulaOp 4 ulaInAMuxSel 2 ulaOutDemuxSel 2 loadRA 1 wait 1 loadPC 1 wait 1");
stateOutputMap["fetchInstruction"] = microInstructions;
} /*microinstructions for fetchInstruction end*/


	RA = new RegisterAssyncReset("RA");
RB = new RegisterAssyncReset("RB");
AR = new RegisterAssyncReset("AR");
DR = new RegisterAssyncReset("DR");
PC = new RegisterAssyncReset("PC");
IR = new RegisterAssyncReset("IR");
registerFile = new RegisterFile("registerFile");
raMultiplexer = new Multiplexer("raMultiplexer",5);
dRInMux = new Multiplexer("dRInMux",2);
dROutDemux = new Demultiplexer("dROutDemux",2);
rbMultiplexer = new Multiplexer("rbMultiplexer",5);
ulaOutputDemultiplexer = new Demultiplexer("ulaOutputDemultiplexer",5);
ula = new ULA("ula");
controlUnit = new CustomizableControlUnit("controlUnit",outputNames,stateOutputMap);

 
	RA->clk(clock);
RA->dataOut(rAoutSignal);
RA->load(rALoadSignal);
RA->reset(resetRegister);
RA->dataIn(muxRaOutSignal);
RB->clk(clock);
RB->dataOut(rBoutSignal);
RB->load(rBLoadSignal);
RB->reset(resetRegister);
RB->dataIn(muxRbOutSignal);
AR->clk(clock);
AR->dataOut(aRoutToMemorySignal);
AR->dataIn(aRinSignal);
AR->load(aRLoadSignal);
AR->reset(resetRegister);
DR->clk(clock);
DR->dataOut(dRoutSignal);
DR->dataIn(dRinSignal);
DR->load(dRLoadSignal);
DR->reset(resetRegister);
dRInMux->output(dRinSignal);
dRInMux->inputs[0](demuxUlaInSignal);
dRInMux->inputs[1](memoryDataInput);
dRInMux->sel(dRinMuxSelSignal);
dROutDemux->input(dRoutSignal);
dROutDemux->outputs[0](ula_Mux_to_dRoutMux_signal);
dROutDemux->outputs[1](memoryDataOutput);
dROutDemux->sel(dROutDemuxSelSignal);
PC->clk(clock);
PC->dataOut(pCoutSignal);
PC->dataIn(pCinSignal);
PC->load(pCLoadSignal);
PC->reset(resetRegister);
IR->clk(clock);
IR->dataOut(iRoutSignal);
IR->dataIn(iRinSignal);
IR->load(iRLoadSignal);
IR->reset(resetRegister);
ula->ulaOut(demuxUlaOutSingal);
ula->opSel(ulaOpSignal);
ula->ulaStatus(ulaStatusSignal);
ula->opA(rAoutSignal);
ula->opB(rBoutSignal);
registerFile->clock(clock);
registerFile->sel(rFSelSignal);
registerFile->rwBit(rFWriteBitSignal);
registerFile->out(rFoutSignal);
registerFile->in(rFinSignal);
raMultiplexer->inputs[0](aRoutSignal);
raMultiplexer->inputs[1](ula_Mux_to_dRoutMux_signal);
raMultiplexer->inputs[2](pCoutSignal);
raMultiplexer->inputs[3](iRoutSignal);
raMultiplexer->inputs[4](rFoutSignal);
raMultiplexer->sel(muxRaSelSignal);
raMultiplexer->output(muxRaOutSignal);
rbMultiplexer->inputs[0](aRoutSignal);
rbMultiplexer->inputs[1](ula_Mux_to_dRoutMux_signal);
rbMultiplexer->inputs[2](pCoutSignal);
rbMultiplexer->inputs[3](iRoutSignal);
rbMultiplexer->inputs[4](rFoutSignal);
rbMultiplexer->sel(muxRbSelSignal);
rbMultiplexer->output(muxRbOutSignal);
ulaOutputDemultiplexer->outputs[0](aRoutSignal);
ulaOutputDemultiplexer->outputs[1](ula_Mux_to_dRintMux_signal);
ulaOutputDemultiplexer->outputs[2](pCinSignal);
ulaOutputDemultiplexer->outputs[3](iRinSignal);
ulaOutputDemultiplexer->outputs[4](rFinSignal);
ulaOutputDemultiplexer->sel(demuxUlaSelSignal);
ulaOutputDemultiplexer->input(demuxUlaOutSignal);
controlUnit->clock(clock);
controlUnit->statusBit(ulaStatusSignal);
controlUnit->iRInput(iRoutSignal);


	controlUnit->assign("loadRA",&rALoadSignal);
controlUnit->assign("loadRB",&rBLoadSignal);
controlUnit->assign("loadIR",&iRLoadSignal);
controlUnit->assign("loadAR",&aRLoadSignal);
controlUnit->assign("loadPC",&pCLoadSignal);
controlUnit->assign("loadDR",&dRLoadSignal);
controlUnit->assign("rfSel",&rfSelSignal);
controlUnit->assign("rfReadWriteBit",&rfReadWriteBitSignal);
controlUnit->assign("ulaInAMuxSel",&ulaInAMuxSelSignal);
controlUnit->assign("ulaInBMuxSel",&ulaInBMuxSelSignal);
controlUnit->assign("ulaOutDemuxSel",&ulaOutDemuxSelSignal);
controlUnit->assign("dRinMuxSel",&dRinMuxSelSignal);
controlUnit->assign("dRoutDemuxSel",&dRoutDemuxSelSignal);
controlUnit->assign("ulaOp",&ulaOpSignal);
controlUnit->assign("writeMemory",&writeMemory);

	
	
	//Signals 
sensitive <<  clock.pos();
sensitive <<  AR->dataOut;

	//Signals 
SC_METHOD(processorBehavior);
SC_METHOD(processorBehavior2);

	

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

