
#include "ControlUnit.h"

void ControlUnit::ControlUnitBehaviour(){
	while(true){
		resetAllLoads();
		cout<<"checking actual state"<<endl;
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
				cout<<"loading pc to AR"<<endl;
				wait(1);//espera um evento
				cout<<"\n\n\n"<<endl;
				break;
			case 1:
				//IR = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_IR; //IR
				loadIR = 1; //Loads IR
				state ++;
				cout<<"loading DR to IR"<<endl;
				wait(1);//espera um evento
				cout<<"\n\n\n"<<endl;
				break;
				/*instruction fetching IR = mem[PC] end*/
			case 2:
				/*increment pc PC = PC+1 begin*/
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;
				state ++;
				wait(1);//espera um evento
				cout<<"increment pc"<<endl;
				break;
				cout<<"\n\n\n"<<endl;
				/*increment pc PC = PC+1 end*/
			case 3:
				cout<<"process instruction"<<endl;
				if(processInstruction())
					state = 0;
				break;
		}
	}
}


bool ControlUnit::processInstruction(){
	//tipo está nos primeiros 2 bits de IR
	cout<<__FILE__<<"::"<<__LINE__<<endl;
	for(int i=0;i<WORD_SIZE; i++)
		cout<<iRInput.read()[i]<<endl;
	switch(iRInput.read().range(1,0).to_int()){
		case 0://instrução de registradores
			{
				cout<<__FILE__<<"::"<<__LINE__<<endl;
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
	rfSel = iRInput.read().range(9,11);
	rfReadWriteBit = 0;
	loadRA = 1;

	wait(1);//espera um evento
	resetAllLoads();

	//RB = RF(src2)
	ulaInBMuxSel = UlaInputSelection_RF;
	rfSel = iRInput.read().range(12,15);
	rfReadWriteBit = 0;
	loadRB = 1;

	wait(1);//espera um evento
	resetAllLoads();

	//Status = ulaOut
	ulaOp = iRInput.read().range(2,5);
	ulaInBMuxSel = UlaOutputSelection_RStatus;
	//loadStatus = 1;
	return true;
}

bool ControlUnit::processControlInstruction(){
	switch(iRInput.read().range(2,5).to_int()){
		case 0: // PC = address
			{
				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(1);//espera um evento
				resetAllLoads();

				//AR = PC
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(1);//espera um evento
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

					wait(1);//espera um evento
					resetAllLoads();

					//AR = PC
					ulaOp = ulaOperation_doNothing;
					ulaInAMuxSel = UlaInputSelection_PC; //PC
					ulaOutDemuxSel = UlaOutputSelection_AR; //AR
					loadAR = 1; //Loads AR
					writeMemory = 0; //read from memory

					wait(1);//espera um evento
					resetAllLoads();

					//PC = DR
					ulaOp = ulaOperation_doNothing;
					ulaInAMuxSel = UlaInputSelection_DR; //DR
					ulaOutDemuxSel = UlaOutputSelection_PC; //PC
					loadPC = 1; //Loads PC
					return true;				
				}
				else{
					//PC = PC + 1 para pular o address
					ulaOp = ulaOperation_inc;
					ulaInAMuxSel = UlaInputSelection_PC; //PC 
					ulaOutDemuxSel = UlaOutputSelection_PC; //PC
					loadPC = 1;

					wait(1);//espera um evento
				}
				return true;
			}
			break;
	}
	return false;
}


bool ControlUnit::processRegisterInstruction(){
	cout<<__FILE__<<"::"<<__LINE__<<endl;
	switch (iRInput.read().range(5,2).to_int()){
		case 0: //RF[dest] = CTE
			{
				cout<<__FILE__<<"::"<<__LINE__<<endl;
				//guarda o endereço onde será guardado address
				rfSel = iRInput.read().range(8,6); 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(1);//espera um evento
				resetAllLoads();
				
				//AR = PC
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(1);//espera um evento
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
				rfSel = iRInput.read().range(11,9);
				rfReadWriteBit = 0;
				loadRA = 1;

				wait(1);//espera um evento
				resetAllLoads();

				//RB = RF(src2)
				ulaInBMuxSel = UlaInputSelection_RF;
				rfSel = iRInput.read().range(15,12);
				rfReadWriteBit = 0;
				loadRB = 1;
				ulaOp = iRInput.read().range(5,2);

				wait(1);//espera um evento
				resetAllLoads();

				//RF(dest) = ulaOut
				ulaInBMuxSel = UlaOutputSelection_RF;
				rfSel = iRInput.read().range(8,6);
				rfReadWriteBit = 1;

				return true;
			}
			break;
	}
}

bool ControlUnit::processMemoryInstruction(){
	switch (iRInput.read().range(2,5).to_int()){
		case 0: //RF[dest] = address
			{
				//guarda o endereço onde será guardado address
				rfSel = iRInput.read().range(6,8); 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(1);//espera um evento
				resetAllLoads();
				
				//RF[dest] = AR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_RF; //RF
				rfReadWriteBit = 1; //carrega RF com o valor em rfSel

				wait(1);//espera um evento
				resetAllLoads();

				return true;
			}
			break;
		case 1: //RF[dest] = Mem[address]
			{
				//guarda o endereço onde será guardado address
				rfSel = iRInput.read().range(6,8); 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(1);//espera um evento
				resetAllLoads();
				
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(1);//espera um evento
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
				rfSel = iRInput.read().range(6,8); 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(1);//espera um evento
				resetAllLoads();
				
				//AR = PC
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(1);//espera um evento
				resetAllLoads();

				//AR = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //RF
				writeMemory = 0; //read from memory

				wait(1);//espera um evento
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
				rfSel = iRInput.read().range(9,11); 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(1);//espera um evento
				resetAllLoads();
				
				//AR = PC
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory
				
				wait(1);//espera um evento
				resetAllLoads();
			
				//AR = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR

				wait(1);//espera um evento
				resetAllLoads();
				
				//DR = RF[rfSel]
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_RF; //RF
				ulaOutDemuxSel = UlaOutputSelection_DR; //DR
				rfReadWriteBit = 0; //lê valor de RF[rfSel]
				loadDR = 1; //Loads DR
				
				wait(1);//espera um evento
				resetAllLoads();
				//Mem[AR] = DR
				writeMemory = 1;	
				return true;
			}
			break;
		case 4: //Mem[Mem[address]] = RF[src1]
			{
				//guarda o endereço onde será guardado address
				rfSel = iRInput.read().range(9,11); 

				//PC = PC + 1
				ulaOp = ulaOperation_inc;
				ulaInAMuxSel = UlaInputSelection_PC; //PC 
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1;

				wait(1);//espera um evento
				resetAllLoads();
				
				//AR = PC
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_PC; //PC
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(1);//espera um evento
				resetAllLoads();

				//AR = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(1);//espera um evento
				resetAllLoads();
				
				//AR = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				writeMemory = 0; //read from memory

				wait(1);//espera um evento
				resetAllLoads();

				//DR = RF[rfSel]
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_RF; //RF
				ulaOutDemuxSel = UlaOutputSelection_DR; //DR
				loadDR = 1; //Loads DR
	
				wait(1);//espera um evento
				resetAllLoads();

				//Mem[AR] = DR
				writeMemory = 1;	
				return true;
			}
			break;
	}
	return false;
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
	//loadStatus = 0;
}

