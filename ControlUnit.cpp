
#include "ControlUnit.h"

ControlUnit::ControlUnitBehaviour(){
	//
	resetAllLoads();
	switch(state){
		/*instruction fetching IR = mem[PC] begin*/
		case 0:
			//AR = PC
			ulaOp = ulaOperation_doNothing;
			muxASel = controlSelectionPC; //PC
			demuxSel = demuxSelectionAR; //AR
			loadAR = 1; //Loads AR
			writeMemory = 0; //read from memory
			state ++;
			break;
		case 1:
			//IR = DR
			ulaOp = ulaOperation_doNothing;
			muxASel = controlSelection_DR; //DR
			ulaOutDemuxSel = demuxSelection_IR; //IR
			loadIR = 1; //Loads IR
			state ++;
			break;
		/*instruction fetching IR = mem[PC] end*/
		case 2:
		/*increment pc PC = PC+1 begin*/
			ulaOp = ulaOperation_inc;
			muxASel = controlSelectionPC; //PC
			demuxSel = demuxSelectionPC; //PC
			loadPC = 1;
			state ++;
			break;
		/*increment pc PC = PC+1 end*/
		case 3:
			if(processInstruction())
				state = 1;
			break;
	}
}

bool ControlUnit::processInstruction(){
	//tipo está nos primeiros 2 bits de IR
	switch(instructionType){
		case 0://instrução de registradores
			{
				if(processRegiterInstruction())
					return true;
			}
			break;
		case 1://instrução de acesso à memoria
			{
				if(processMemoryInstruction())
					return true;
			}
			break;
		case 2:
			{
				if(processControlInstruction())
					return true;
			}
			break;
		case 3:
			{
				if(processMiscellaneousInstruction()){
					return true;
				}
			}
			break;
	}
	return false;
}

bool ControlUnit::processMiscellaneousInstruction(){
	//if (RF(src1) op RF(src2)) statusBit = 1 else statusBit = 0
	//RA = RF(src1)
	muxASel = UlaInputSelection_RF;
	rfSel = src1;
	rfReadWriteBit = 0;
	loadRA = 1;

	wait(clk);
	resetAllLoads();

	//RB = RF(src2)
	muxBSel = UlaInputSelection_RF;
	rfSel = src2;
	rfReadWriteBit = 0;
	loadRB = 1;
	ulaOp = operation;

	wait(clk);
	resetAllLoads();

	//Status = ulaOut
	ulaOp = operation;
	muxBSel = UlaOutputSelection_RStatus;
	loadStatus = 1;
}

bool ControlUnit::processControlInstruction(){
	switch(operation){
		case 0: // PC = address
			{
				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				muxASel = controlSelectionPC; //PC 
				ulaOutDemuxSel = demuxSelectionPC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();

				//AR = PC
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionPC; //PC
				demuxSel = demuxSelectionAR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//PC = DR
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionDR; //DR
				demuxSel = demuxSelectionPC; //PC
				loadPC = 1; //Loads PC
				return true;
			}
			break;
		case 1: // if status[rel] == 1 then PC = Address end
			{
				int rel = dest;
				if(statusBit){
					//PC = PC + 1
					ulaOp = ulaOperation_inc;
					muxASel = controlSelectionPC; //PC 
					ulaOutDemuxSel = demuxSelectionPC; //PC
					loadPC = 1;

					wait(clock);
					resetAllLoads();

					//AR = PC
					ulaOp = ulaOperation_doNothing;
					muxASel = controlSelectionPC; //PC
					demuxSel = demuxSelectionAR; //AR
					loadAR = 1; //Loads AR
					writeMemory = 0; //read from memory

					wait(clock);
					resetAllLoads();

					//PC = DR
					ulaOp = ulaOperation_doNothing;
					muxASel = controlSelectionDR; //DR
					demuxSel = demuxSelectionPC; //PC
					loadPC = 1; //Loads PC
					return true;				
				}
				return true;
			}
			break;
	}
	return false;
}


bool ControlUnit::processRegisterInstruction(){
	switch (operation){
		case 0: //RF[dest] = CTE
			{
				//guarda o endereço onde será guardado address
				rfSel = dest; 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				muxASel = controlSelectionPC; //PC 
				ulaOutDemuxSel = demuxSelectionPC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();
				
				//AR = PC
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionPC; //PC
				demuxSel = demuxSelectionAR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//RF[rfSel] = DR
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionDR; //DR
				demuxSel = demuxSelectionRF; //RF
				rfReadWriteBit = 1; //escreve em RF

				return true;
			}
			break;
		default:
			{
				//RA = RF(src1)
				muxASel = UlaInputSelection_RF;
				rfSel = src1;
				rfReadWriteBit = 0;
				loadRA = 1;

				wait(clk);
				resetAllLoads();

				//RB = RF(src2)
				muxBSel = UlaInputSelection_RF;
				rfSel = src2;
				rfReadWriteBit = 0;
				loadRB = 1;
				ulaOp = operation;

				wait(clk);
				resetAllLoads();

				//RF(dest) = ulaOut
				muxBSel = UlaOutputSelection_RF;
				rfSel = dest;
				rfReadWriteBit = 1;

				return true;
			}
			break;
	}
}

bool ControlUnit::processMemoryInstruction(){
	switch (operation){
		case 0: //RF[dest] = address
			{
				//guarda o endereço onde será guardado address
				rfSel = dest; 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				muxASel = controlSelectionPC; //PC 
				ulaOutDemuxSel = demuxSelectionPC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();
				
				//RF[dest] = AR
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionPC; //PC
				demuxSel = demuxSelectionRF; //RF
				rfReadWriteBit = 1; //carrega RF com o valor em rfSel

				wait(clock);
				resetAllLoads();

				return true;
			}
			break;
		case 1: //RF[dest] = Mem[address]
			{
				//guarda o endereço onde será guardado address
				rfSel = dest; 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				muxASel = controlSelectionPC; //PC 
				ulaOutDemuxSel = demuxSelectionPC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();
				
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionPC; //PC
				demuxSel = demuxSelectionAR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//RF[dest] = DR
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionDR; //DR
				demuxSel = demuxSelectionRF; //RF
				rfReadWriteBit = 1; //carrega RF com o valor em rfSel

				return true;
			}
		case 2: //RF[dest] = Mem[Mem[address]]
			{
				//guarda o endereço onde será guardado address
				rfSel = dest; 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				muxASel = controlSelectionPC; //PC 
				ulaOutDemuxSel = demuxSelectionPC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();
				
				//AR = PC
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionPC; //PC
				demuxSel = demuxSelectionAR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//AR = DR
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionDR; //DR
				demuxSel = demuxSelectionAR; //RF
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//RF[dest] = DR
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionDR; //DR
				demuxSel = demuxSelectionRF; //R
				rfReadWriteBit = 1; //carrega RF com o valor em rfSel

				return true;
			}
			break;
		case 3: //Mem[address] = RF[src1]
			{
				//guarda o endereço onde será guardado address
				rfSel = src1; 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				muxASel = controlSelectionPC; //PC 
				ulaOutDemuxSel = demuxSelectionPC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();
				
				//AR = PC
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionPC; //PC
				demuxSel = demuxSelectionAR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory
				
				wait(clock);
				resetAllLoads();
			
				//AR = DR
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionDR; //DR
				demuxSel = demuxSelectionAR; //AR
				loadAR = 1; //Loads AR

				wait(clock);
				resetAllLoads();
				
				//DR = RF[rfSel]
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionRF; //RF
				demuxSel = demuxSelectionDR; //DR
				rfReadWriteBit = 0; //lê valor de RF[rfSel]
				loadDR = 1; //Loads DR
				
				wait(clock);
				resetAllLoads();
				//Mem[AR] = DR
				writeMemory = 1;	
				return true;
			}
			break;
		case 4: //Mem[Mem[address]] = RF[src1]
			{
				//guarda o endereço onde será guardado address
				rfSel = src1; 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				muxASel = controlSelectionPC; //PC 
				ulaOutDemuxSel = demuxSelectionPC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();
				
				//AR = PC
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionPC; //PC
				demuxSel = demuxSelectionAR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//AR = DR
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionDR; //DR
				demuxSel = demuxSelectionAR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();
				
				//AR = DR
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionDR; //DR
				demuxSel = demuxSelectionAR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//DR = RF[rfSel]
				ulaOp = ulaOperation_doNothing;
				muxASel = controlSelectionRF; //RF
				demuxSel = demuxSelectionDR; //DR
				loadDR = 1; //Loads DR
	
				wait(clock);
				resetAllLoads();

				//Mem[AR] = DR
				writeMemory = 1;	
				return true;
			}
			break;

	}
}


void ControlUnit::resetAllLoads(){
	writeMemory = 0;
	loadPC = 0;
	loadRF = 0;
	loadIR = 0;
	loadAR = 0;
}

