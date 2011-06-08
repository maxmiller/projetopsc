
#include "ControlUnit.h"

void ControlUnit::ControlUnitBehaviour(){
	while(true){
		resetAllLoads();
		switch(state){
			/*instruction fetching IR = mem[PC] begin*/
			case 0:
				//AR = PC
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory
				state ++;
				wait(clock);
				break;
			case 1:
				//IR = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_IR; //IR
				loadIR = 1; //Loads IR
				state ++;
				wait(clock);
				break;
				/*instruction fetching IR = mem[PC] end*/
			case 2:
				/*increment pc PC = PC+1 begin*/
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;
				state ++;
				wait(clock);
				break;
				/*increment pc PC = PC+1 end*/
			case 3:
				if(processInstruction())
					state = 0;
				break;
		}
	}
}

bool ControlUnit::processInstruction(){
	//tipo está nos primeiros 2 bits de IR
	switch(instructionType.read().to_int()){
		case 0://instrução de registradores
			{
				if(processRegisterInstruction())
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
	ulaInAMuxSel = UlaInputSelection_RF;
	rfSel = src1.read();
	rfReadWriteBit = 0;
	loadRA = 1;

	wait(clock);
	resetAllLoads();

	//RB = RF(src2)
	ulaInBMuxSel = UlaInputSelection_RF;
	rfSel = src2.read();
	rfReadWriteBit = 0;
	loadRB = 1;

	wait(clock);
	resetAllLoads();

	//Status = ulaOut
	ulaOp = operation.read();
	ulaInBMuxSel = UlaOutputSelection_RStatus;
	loadStatus = 1;
	return true;
}

bool ControlUnit::processControlInstruction(){
	switch(operation.read().to_int()){
		case 0: // PC = address
			{
				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();

				//AR = PC
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//PC = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1; //Loads PC
				return true;
			}
			break;
		case 1: // if status[rel] == 1 then PC = Address end
			{
				if(statusBit){
					//PC = PC + 1
					ulaOp = ulaOperation_inc;
					ulaInAMuxSel = UlaInputSelection_PC; //PC 
					ulaOutDemuxSel = UlaOutputSelection_PC; //PC
					loadPC = 1;

					wait(clock);
					resetAllLoads();

					//AR = PC
					ulaOp = ulaOperation_doNothing;
					ulaInAMuxSel = UlaInputSelection_PC; //PC
					ulaOutDemuxSel = UlaOutputSelection_AR; //AR
					loadAR = 1; //Loads AR
					writeMemory = 0; //read from memory

					wait(clock);
					resetAllLoads();

					//PC = DR
					ulaOp = ulaOperation_doNothing;
					ulaInAMuxSel = UlaInputSelection_DR; //DR
					ulaOutDemuxSel = UlaOutputSelection_PC; //PC
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
	switch (operation.read().to_int()){
		case 0: //RF[dest] = CTE
			{
				//guarda o endereço onde será guardado address
				rfSel = dest.read(); 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();
				
				//AR = PC
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//RF[rfSel] = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_RF; //RF
				rfReadWriteBit = 1; //escreve em RF

				return true;
			}
			break;
		default:
			{
				//RA = RF(src1)
				ulaInAMuxSel = UlaInputSelection_RF;
				rfSel = src1.read();
				rfReadWriteBit = 0;
				loadRA = 1;

				wait(clock);
				resetAllLoads();

				//RB = RF(src2)
				ulaInBMuxSel = UlaInputSelection_RF;
				rfSel = src2.read();
				rfReadWriteBit = 0;
				loadRB = 1;
				ulaOp = operation.read();

				wait(clock);
				resetAllLoads();

				//RF(dest) = ulaOut
				ulaInBMuxSel = UlaOutputSelection_RF;
				rfSel = dest.read();
				rfReadWriteBit = 1;

				return true;
			}
			break;
	}
}

bool ControlUnit::processMemoryInstruction(){
	switch (operation.read().to_int()){
		case 0: //RF[dest] = address
			{
				//guarda o endereço onde será guardado address
				rfSel = dest.read(); 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();
				
				//RF[dest] = AR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_RF; //RF
				rfReadWriteBit = 1; //carrega RF com o valor em rfSel

				wait(clock);
				resetAllLoads();

				return true;
			}
			break;
		case 1: //RF[dest] = Mem[address]
			{
				//guarda o endereço onde será guardado address
				rfSel = dest.read(); 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();
				
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//RF[dest] = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_RF; //RF
				rfReadWriteBit = 1; //carrega RF com o valor em rfSel

				return true;
			}
		case 2: //RF[dest] = Mem[Mem[address]]
			{
				//guarda o endereço onde será guardado address
				rfSel = dest.read(); 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();
				
				//AR = PC
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//AR = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //RF
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//RF[dest] = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_RF; //R
				rfReadWriteBit = 1; //carrega RF com o valor em rfSel

				return true;
			}
			break;
		case 3: //Mem[address] = RF[src1]
			{
				//guarda o endereço onde será guardado address
				rfSel = src1.read(); 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();
				
				//AR = PC
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory
				
				wait(clock);
				resetAllLoads();
			
				//AR = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR

				wait(clock);
				resetAllLoads();
				
				//DR = RF[rfSel]
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_RF; //RF
				ulaOutDemuxSel = UlaOutputSelection_DR; //DR
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
				rfSel = src1.read(); 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(clock);
				resetAllLoads();
				
				//AR = PC
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//AR = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();
				
				//AR = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(clock);
				resetAllLoads();

				//DR = RF[rfSel]
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_RF; //RF
				ulaOutDemuxSel = UlaOutputSelection_DR; //DR
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
	rfReadWriteBit = 0;
	writeMemory = 0;
	loadRA = 0;
	loadRB = 0;
	loadIR = 0;
	loadAR = 0;
	loadPC = 0;
	loadDR = 0;
	loadStatus = 0;
}

