#include "Processor.h"
#include "PipelineControl.h"
#include <vector>
#include <map>

using namespace std;

Processor::Processor(sc_module_name name) : sc_module(name){

	DR = new RegisterAssyncReset("DR");
	DR->clk(clock);
	DR->dataOut(DR_dataOut2registerFileInputMux_inputs1);
	DR->dataIn(dataMemoryDataI);
	DR->load(controlUnit2DR_load);
	DR->reset(resetRegisters_input);

	PC = new RegisterAssyncReset("PC");
	PC->clk(clock);
	PC->dataOut(programMemoryAddress);
	PC->dataIn(pcInputMux2PC_dataIn); //vem da ula ou de um somador
	PC->load(controlUnit2PC_load);
	PC->reset(resetRegisters_input);

	pcAdder = new Adder("PCAdder");
	pcAdder->addValue = 1; //vai adicionar 1 ao valor de entrada
	pcAdder->in(programMemoryAddress);
	pcAdder->out(pcInputMux_inputs02pcAdder_out);

	pcInputMux = new Multiplexer("pcInputMux",2);
	pcInputMux->inputs[0](pcInputMux_inputs02ula_ulaOut);
	pcInputMux->inputs[1](pcInputMux_inputs02pcAdder_out);
	pcInputMux->output(pcInputMux2PC_dataIn);
	pcInputMux->sel(controlUnit2pcInputMux_sel);

	IR = new RegisterAssyncReset("IR");
	IR->clk(clock);
	IR->dataOut(controlUnit2IR_dataOut); //vai para controll unit
	IR->dataIn(programMemoryDataI); //vem da memoria
	IR->load(controlUnit2IR_load);
	IR->reset(resetRegisters_input);

	registerFileOutBusSpliter = new BusSpliter("registerFileOutBusSpliter",2);
	registerFileOutBusSpliter->outputs[0](registerFile_out22ula_opB);
	registerFileOutBusSpliter->outputs[1](dataMemoryDataO);
	registerFileOutBusSpliter->input(registerFile_out22registerFileOutBusSpliter_input);
	
	registerFileSel2BusSpliter = new BusSpliter("registerFileSel2BusSpliter",2);
	registerFileSel2BusSpliter->input(controlUnit2registerFile_selRead2);
	registerFileSel2BusSpliter->outputs[0](registerFileSel2BusSpliter2registerFile_selRead2);
	registerFileSel2BusSpliter->outputs[1](registerFileSel2BusSpliter2ulaOpBMux_input1);

/*register file pipeline registers*/
/*
	registerFileSelread1PipelineRegister = new RegisterAssyncReset("registerFileInputPipelineRegister");
	registerFileSelread1PipelineRegister->dataIn(controlUnit2registerFile_selRead1);
	registerFileSelread1PipelineRegister->dataOut(registerFileSelread1PipelineRegister_dataOut2registerFile_selRead1);
	registerFileSelread1PipelineRegister->reset(resetRegisters_input);
	registerFileSelread1PipelineRegister->clk();//vem da unidade de controle

	registerFileSelread2PipelineRegister = new RegisterAssyncReset("registerFileInputPipelineRegister");
	registerFileSelread2PipelineRegister->dataIn(registerFileSel2BusSpliter2registerFile_selRead2);
	registerFileSelread2PipelineRegister->dataOut(registerFileSelread1PipelineRegister_dataOut2registerFile_selRead2);
	registerFileSelread2PipelineRegister->reset(resetRegisters_input);
	registerFileSelread2PipelineRegister->clk();

	registerFileOut1PipelineRegister = new RegisterAssyncReset("registerFileOut1PipelineRegister");
	registerFileOut1PipelineRegister->dataIn(registerFileOut1PipelineRegister_dataIn2registerFile_out1);
	registerFileOut1PipelineRegister->dataOut(registerFile_out12ula_opA);
	registerFileOut1PipelineRegister->reset(resetRegisters_input);
	registerFileOut1PipelineRegister->clk();
	
	registerFileOut2PipelineRegister = new RegisterAssyncReset("registerFileOut1PipelineRegister");
	registerFileOut2PipelineRegister->dataIn(registerFileOut1PipelineRegister_dataIn2registerFile_out2);
	registerFileOut2PipelineRegister->dataOut(registerFile_out22registerFileOutBusSpliter_input);
	registerFileOut2PipelineRegister->reset(resetRegisters_input);
	registerFileOut2PipelineRegister->clk();

	registerFileSelWritePipelineRegister = new RegisterAssyncReset("registerFileSelWritePipelineRegister");
	registerFileSelWritePipelineRegister->dataIn(controlUnit2registerFile_selWrite);
	registerFileSelWritePipelineRegister->dataOut(registerFileSelWritePipelineRegister_dataOut2registerFile_selWrite);
	registerFileSelWritePipelineRegister->reset(resetRegisters_input);
	egisterFileSelWritePipelineRegister->clk();
	
	registerFileInPipelineRegister = new RegisterAssyncReset("registerFileInPipelineRegister");
	registerFileInPipelineRegister->dataIn(registerFileInputMux_output2registerFile_in);
	registerFileInPipelineRegister->dataOut(registerFileInPipelineRegister_dataOut2registerFile_in);
	registerFileInPipelineRegister->reset(resetRegisters_input);
	registerFileInPipelineRegister->clk();
	
	registerFileRwBitPipelineRegister = new RegisterAssyncReset("registerFileRwBitPipelineRegister");
	registerFileRwBitPipelineRegister->dataIn(controlUnit2registerFile_rwBit);
	registerFileRwBitPipelineRegister->dataOut(registerFileRwBitPipelineRegister_dataOut2registerFile_rwBit);
	registerFileRwBitPipelineRegister->reset(resetRegisters_input);
	registerFileRwBitPipelineRegister->clk();
*/


	registerFile = new RegisterFile("RegisterFile");
	registerFile->clock(clock);
//	registerFile->selRead1(registerFileSelread1PipelineRegister_dataOut2registerFile_selRead1);
	registerFile->selRead1(controlUnit2registerFile_selRead1);
//	registerFile->selRead2(registerFileSelread1PipelineRegister_dataOut2registerFile_selRead2);
	registerFile->selRead2(registerFileSel2BusSpliter2registerFile_selRead2);
//	registerFile->out1(registerFileOut1PipelineRegister_dataIn2registerFile_out1);
	registerFile->out1(registerFile_out12ula_opA);
//	registerFile->out2(registerFileOut1PipelineRegister_dataIn2registerFile_out2);
	registerFile->out2(registerFile_out22registerFileOutBusSpliter_input);
//	registerFile->selWrite(registerFileSelWritePipelineRegister_dataOut2registerFile_selWrite);
	registerFile->selWrite(controlUnit2registerFile_selWrite);
//	registerFile->in(registerFileInPipelineRegister_dataOut2registerFile_in);
	registerFile->in(registerFileInputMux_output2registerFile_in);
//	registerFile->rwBit(registerFileRwBitPipelineRegister_dataOut2registerFile_rwBit);
	registerFile->rwBit(controlUnit2registerFile_rwBit);

	
	registerFileInputMux = new Multiplexer("registerFileInputMux",2);
	registerFileInputMux->inputs[0](ula_ulaOut2registerFileInputMux_inputs0);
	registerFileInputMux->inputs[1](DR_dataOut2registerFileInputMux_inputs1);
	registerFileInputMux->output(registerFileInputMux_output2registerFile_in);
	registerFileInputMux->sel(controlUnit2registerFileInputMux_sel);

	ulaBusSpliter	= new BusSpliter("ulaBusSpliter",3);
	ulaBusSpliter->outputs[0](ula_ulaOut2registerFileInputMux_inputs0);
	ulaBusSpliter->outputs[1](dataMemoryAddress);
	ulaBusSpliter->outputs[2](pcInputMux_inputs02ula_ulaOut);
	ulaBusSpliter->input(ula_ulaOut2ulaBusSpliter_input);

/*ula pipeline register*/
/*
	ulaOutPipelineRegister = new RegisterAssyncReset("ulaOutPipelineRegister");
	ulaOutPipelineRegister->dataIn(ula_ulaOut2ulaOutPipelineRegister_input);
	ulaOutPipelineRegister->dataOut(ula_ulaOut2ulaBusSpliter_input);
	ulaOutPipelineRegister->clk(); //ligar no control unit
	ulaOutPipelineRegister->reset(resetRegisters_input);
*/

	ulaOpBMux = new Multiplexer("ulaOpBMux",2);
	ulaOpBMux->inputs[0](registerFile_out22ula_opB);
	ulaOpBMux->inputs[1](registerFileSel2BusSpliter2ulaOpBMux_input1);
	ulaOpBMux->output(ulaOpBMux_output2ula_opB);
	ulaOpBMux->sel(controlUnit2ulaOpBMux);

	ula = new ULA("Ula");
	ula->ulaOut(ula_ulaOut2ulaBusSpliter_input);
//	ula->ulaOut(ula_ulaOut2ulaOutPipelineRegister_input);
	ula->opSel(controlUnit2ula_opSel);
	ula->ulaStatus(controlUnit2ula_ulaStatus);
	ula->opA(registerFile_out12ula_opA);
	ula->opB(ulaOpBMux_output2ula_opB);


	vector<std::string> outputNames;
	outputNames.push_back("loadPC");
	outputNames.push_back("loadIR");
	outputNames.push_back("loadDR");
	outputNames.push_back("pcInputMuxSel");
	outputNames.push_back("registerFileSel1");
	outputNames.push_back("registerFileSel2");
	outputNames.push_back("registerFileRwBit");
	outputNames.push_back("registerFileInputMuxSel");
	outputNames.push_back("registerFileSelWrite");
	outputNames.push_back("ulaOpSel");
	outputNames.push_back("programMemoryRWBit");
	outputNames.push_back("dataMemoryRWBit");
	outputNames.push_back("controlUnit2ulaOpBMux");

	vector<std::string> inputNames;
	inputNames.push_back("ulaStatus");
	inputNames.push_back("irInput");

	//pipeline control state machine in format
	//stage => next stages
	string pipelineControlInitialState = "0";
	map<string,vector<string> > pipelineControlStateMachineStateTransitions;
	{
		vector<string > nextStates;
    nextStates.push_back("1");

    pipelineControlStateMachineStateTransitions["0"] = nextStates;
	}

	//stage_to_stage => conditions
	map<string,string> pipelineControlStateMachineTransitionCondictions;
	{
		string conidtion = "anyOther";
		pipelineControlStateMachineTransitionCondictions["0_to_1"] = conidtion;
	}

	//state => commands for this actual stage
	map<string,vector<string> > pipeLineControlMap;
	{
		vector<string > microInstructions;

		pipeLineControlMap["0"] = microInstructions;
	}
	{
		vector<string > microInstructions;
		microInstructions.push_back("fetchInstruction");

		pipeLineControlMap["1"] = microInstructions;
	}

	PipelineControl *pipelineControl = new PipelineControl(pipelineControlStateMachineStateTransitions, pipelineControlStateMachineTransitionCondictions, pipeLineControlMap, pipelineControlInitialState);
	
	map<string,vector<string> > stateOutputMap;	
	{ 
		/*microinstructions for fetchInstructionbegin */
		vector<string > microInstructions;
		/*microinstructions for macro resetAllLoads*/

		//IF
		microInstructions.push_back("programMemoryRWBit 0 loadIR 1 wait 1 loadIR 0 pcInputMuxSel 1 loadPC 1 wait 1 loadPC 0 wait 1");
		/*microinstructions for macro loadsARWithPc*/
		stateOutputMap["fetchInstruction"] = microInstructions;
	} /*microinstructions for fetchInstruction end*/
	{ //reg simple instruction
		/*microinstructions for fetchInstructionbegin */
		vector<string > microInstructions;
		/*microinstructions for macro resetAllLoads*/
		//ID/EX/MA/WB
		microInstructions.push_back("registerFileSel1 irInput[5,3] registerFileSel2 irInput[2,0] wait 1 controlUnit2ulaOpBMux 0 ulaOpSel irInput[13,9] registerFileSelWrite irInput[8,6] registerFileRwBit 1 registerFileInputMuxSel 0 wait 1 registerFileRwBit 0");
		/*microinstructions for macro loadsARWithPc*/
		stateOutputMap["0_all"] = microInstructions;
	} /*microinstructions for fetchInstruction end*/
	{ //reg imediate add instruction
		/*microinstructions for fetchInstructionbegin */
		vector<string > microInstructions;
		/*microinstructions for macro resetAllLoads*/
		microInstructions.push_back("registerFileSel1 irInput[5,3] registerFileSel2 irInput[2,0] wait 1 controlUnit2ulaOpBMux 1 ulaOpSel 1 registerFileSelWrite irInput[8,6] registerFileRwBit 1 registerFileInputMuxSel 0 wait 1 registerFileRwBit 0");
		/*microinstructions for macro loadsARWithPc*/
		stateOutputMap["0_4"] = microInstructions;
	} /*microinstructions for fetchInstruction end*/
	{ //load instruction RF [dest] = Mem[RF[src1] + src2] 
		/*microinstructions for fetchInstructionbegin */
		vector<string > microInstructions;
		/*microinstructions for macro resetAllLoads*/
		microInstructions.push_back("registerFileSel1 irInput[5,3] registerFileSel2 irInput[2,0] wait 1 controlUnit2ulaOpBMux 1 ulaOpSel 1 dataMemoryRWBit 0 wait 1 loadDR 1 wait 1 loadDR 0 registerFileInputMuxSel 1 registerFileRwBit 1 registerFileSelWrite irInput[8,6] wait 1 registerFileRwBit 0");
		/*microinstructions for macro loadsARWithPc*/
		stateOutputMap["1_0"] = microInstructions;
	} /*microinstructions for fetchInstruction end*/

	controlUnit = new CustomizableControlUnit("ControlUnit", outputNames, inputNames, stateOutputMap, pipelineControl, 5);

	controlUnit->clock(clock);
	

	
//loads dos registradores
	//TODO status register
//	controlUnit->loadStatus(statusLoadSignal);

	controlUnit->assignOutput("loadPC",&controlUnit2PC_load);
	controlUnit->assignOutput("loadIR",&controlUnit2IR_load);
	controlUnit->assignOutput("loadDR",&controlUnit2DR_load);
	controlUnit->assignOutput("pcInputMuxSel",&controlUnit2pcInputMux_sel);	
	controlUnit->assignOutput("registerFileSel1",&controlUnit2registerFile_selRead1);	
	controlUnit->assignOutput("registerFileSel2",&controlUnit2registerFile_selRead2);	
	controlUnit->assignOutput("registerFileRwBit",&controlUnit2registerFile_rwBit);
	controlUnit->assignOutput("registerFileInputMuxSel",&controlUnit2registerFileInputMux_sel);
	controlUnit->assignOutput("registerFileSelWrite",&controlUnit2registerFile_selWrite);
	controlUnit->assignOutput("ulaOpSel",&controlUnit2ula_opSel);
	controlUnit->assignOutput("programMemoryRWBit",&programMemoryRWBit);
	controlUnit->assignOutput("dataMemoryRWBit",&dataMemoryRWBit);
	controlUnit->assignOutput("controlUnit2ulaOpBMux",&controlUnit2ulaOpBMux);

	controlUnit->assignInput("ulaStatus",&controlUnit2ula_ulaStatus);
	controlUnit->assignInput("irInput",&controlUnit2IR_dataOut);


	SC_METHOD(processorBehavior);
	sensitive  << clock.pos();
	SC_METHOD(processorBehavior2);

}

void Processor::processorBehavior2(){
//	memoryAddress.write(AR->dataOut.read());

}

void Processor::processorBehavior(){
	//cout<<"arOutsignal "<<aRoutSignal.read()<<endl;
	//cout<<"arOutsignal "<<AR->dataOut.read()<<endl;
	//cout<<"memoryData "<<memoryDataInput.read()<<endl;
	cout<<"PC in value "<<PC->dataIn.read()<<endl;
	cout<<"PC out value "<<PC->dataOut.read()<<endl;
	cout<<"IR in value "<<IR->dataIn.read()<<endl;
	cout<<"IR out value "<<IR->dataOut.read()<<endl;
	cout<<"DR in value"<<DR->dataIn.read()<<endl;
	cout<<"DR out value"<<DR->dataOut.read()<<endl;

	cout<<"RF out1["<<registerFile->selRead1.read()<<"]="<<registerFile->out1.read()<<endl;
	cout<<"RF out2["<<registerFile->selRead2.read()<<"]="<<registerFile->out2.read()<<endl;
	cout<<"RF trying to write RF["<<registerFile->selWrite.read()<<"]="<<registerFile->in.read()<<endl;
	cout<<"ula("<<ula->opSel.read()<<")"<<"{"<<ula->opA.read()<<","<<ula->opB.read()<<"} = "<<ula->ulaOut.read()<<endl;
//	cout<<"bus spliter input :"<<registerFileSel2BusSpliter->input.read()<<endl;
/*	cout<<"program memory addr "<<programMemoryAddress.read()<<endl;
	cout<<"program memory data "<<programMemoryDataI.read()<<endl;
	cout<<"data memory addr "<<dataMemoryAddress.read()<<endl;
	cout<<"data memory data "<<dataMemoryDataI.read()<<endl;
*/
	////cout<<"DRinSel " <<dRinMuxSelSignal.read()<<endl;
	////cout<<"DRoutSel " <<dRoutDemuxSelSignal.read()<<endl;

	//writeMemory.write(writeMemorySignal.read());
}

