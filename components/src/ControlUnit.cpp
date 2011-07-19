
#include "ControlUnit.h"

void ControlUnit::controlUnitBehavior(){
	while(true){
		resetAllLoads();
		//cout<<"\nchecking actual state\n"<<endl;
		switch(state){
			
			/*instruction fetching IR = mem[PC] begin*/
			case 0:
				//AR = PC
				//cout<<"\nfetching new instruction 1.1\n"<<endl;
				loadsARWithPc();
				state ++;
				break;
			case 1:
				//IR = DR
				//cout<<"\nfetching new instruction 1.2\n"<<endl;
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				ulaOutDemuxSel = UlaOutputSelection_IR; //IR
				loadRA = 1; //loads RA = DR
				wait(1);
				loadIR = 1; //Loads IR = RA
				wait(1);
				state ++;
				break;
				/*instruction fetching IR = mem[PC] end*/
			case 2:
				/*increment pc PC = PC+1 begin*/
				incrementPC();
				state ++;
				break;
				/*increment pc PC = PC+1 end*/
			case 3:
				//cout<<"\nprocess instruction\n"<<endl;
				if(processInstruction())
					state = 0;
				break;
		}
	}
}

void ControlUnit::loadsARWithPc(){
	//cout<<"\n\nloading ar with pc\n\n"<<endl;
	ulaOp = ulaOperation_doNothing;
	ulaInAMuxSel = UlaInputSelection_PC; //PC
	ulaOutDemuxSel = UlaOutputSelection_AR; //AR
	writeMemory = 0; //read from memory
	dRinMuxSel = 1; //DR->in => Memoria
	dRoutDemuxSel = 0; //DR->out => RA e RB
	loadRA = 1; // RA = PC
	wait(1);
	loadAR = 1; // AR = PC
	wait(1); //espera AR
	wait(1); //espera MemAddr = AR
	loadDR = 1; // DR = memory[PC]
	wait(1);
}

void ControlUnit::incrementPC(){
	//cout<<"\n\nincrement pc\n\n"<<endl;
	resetAllLoads();
	ulaOp = ulaOperation_inc;
	ulaInAMuxSel = UlaInputSelection_PC; //PC
	ulaOutDemuxSel = UlaOutputSelection_PC; //PC
	loadRA = 1;
	wait(1); // RA = PC
	loadPC = 1; // PC = RA
	wait(1);
}


bool ControlUnit::processInstruction(){
	//tipo está nos primeiros 2 bits de IR
	//cout<<__FILE__<<"::"<<__LINE__<<endl;
	//cout<<"type "<<iRInput.read().range(15,14)<<endl;
	//cout<<"op "<<iRInput.read().range(13,9)<<endl;
	//cout<<"dest "<<iRInput.read().range(8,6)<<endl;
	//cout<<"src1 "<<iRInput.read().range(5,3)<<endl;
	//cout<<"src2 "<<iRInput.read().range(2,0)<<endl;
	int instructionType = iRInput.read().range(15,14).to_int();
	switch(instructionType){
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
	//
	int operation = iRInput.read().range(13,9).to_int();
	int src1 = iRInput.read().range(5,3).to_int();
	int src2 = iRInput.read().range(2,0).to_int();
	//cout<<"\n\nRA = RF[src1]\n\n";
	ulaInAMuxSel = UlaInputSelection_RF;
	rfSel = src1;
	rfReadWriteBit = 0;
	wait(1); //espera RF
	loadRA = 1;
	wait(1); //espera RA

	resetAllLoads();

	//RB = RF(src2)
	//cout<<"\n\nRB = RF[src2] && stausBit = RA op RB\n\n";
	ulaInBMuxSel = UlaInputSelection_RF;
	rfSel = src2;
	rfReadWriteBit = 0;
	wait(1); //espera RF
	loadRB = 1;
	wait(1);//espera RB
	ulaOp = operation;
	wait(1); //espera status
	////cout<<"status in "<<statusBit<<endl;
	//wait(1);
	////cout<<"status in "<<statusBit<<endl;
	//Status = ulaOut
	//ulaInBMuxSel = UlaOutputSelection_RStatus;
	//loadStatus = 1;
	return true;
}

bool ControlUnit::processControlInstruction(){
	int operation = iRInput.read().range(13,9).to_int();
	int src1 = iRInput.read().range(5,3).to_int();
	switch(operation){
		case 0: // PC = address
			{
				//cout<<__FILE__<<"::"<<__LINE__<<endl;

				loadsARWithPc();

				resetAllLoads();

				//PC = DR
				//cout<<"\nPC = DR\n";
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				loadRA = 1;
				wait(1); //espera RA
				ulaOutDemuxSel = UlaOutputSelection_PC; //PC
				loadPC = 1; //espera PC
				wait(1);
				return true;
			}
			break;
		case 1: // if status[rel] == 1 then PC = Address end
			{
				//cout<<__FILE__<<"::"<<__LINE__<<endl;
				if(statusBit){
					
					loadsARWithPc();

					resetAllLoads();

					//PC = DR
					//cout<<"\nPC = DR\n";
					ulaOp = ulaOperation_doNothing;
					ulaInAMuxSel = UlaInputSelection_DR; //DR
					loadRA = 1;
					wait(1); //espera RA
					ulaOutDemuxSel = UlaOutputSelection_PC; //PC
					loadPC = 1; //Loads PC
					wait(1); //espera PC

					return true;				
				}
				else{
					//PC = PC + 1 para pular o address
					//
					//cout<<"\nstatus bit not set PC = PC +1\n";
					incrementPC();
				}
				return true;
			}
			break;
	}
	return false;
}


bool ControlUnit::processRegisterInstruction(){
	//cout<<__FILE__<<"::"<<__LINE__<<endl;
	int operation = iRInput.read().range(13,9).to_int();
	switch (operation){
		case 0: //RF[dest] = CTE
			{
				//cout<<__FILE__<<"::"<<__LINE__<<endl;
				int dest = iRInput.read().range(8,6).to_int();
				//guarda o endereço onde será guardado address
				rfSel = dest; 
				//pega o valor da constante
				//cout<<"\nGetting CTE value from mem\n";
				loadsARWithPc();
				resetAllLoads();

				//incrementa o pc para descartar o dado
				//cout<<"\n\n";
				incrementPC();
				resetAllLoads();

				//cout<<"\n\n";
				//RF[rfSel] = DR
				//
				//cout<<"DR to RF[dest]"<<endl;
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //RA = DR
				loadRA = 1;
				wait(1);

				ulaOutDemuxSel = UlaOutputSelection_RF; //RF[dest] = RA
				rfReadWriteBit = 1; //escreve em RF
				wait(1);

				return true;
			}
			break;
		default: //RF [dest] = RF[src1] op RF[src2]
			{
				int dest = iRInput.read().range(8,6).to_int();
				int src1 = iRInput.read().range(5,3).to_int();
				int src2 = iRInput.read().range(2,0).to_int();

				//RA = RF(src1)
				ulaInAMuxSel = UlaInputSelection_RF;
				ulaOp = operation;
				rfSel = src1;//
				rfReadWriteBit = 0;
				//cout<<"loading RA with RF(src1)"<<endl;
				wait(1);//espera por RF
				loadRA = 1;
				wait(1);//espera por loadRA
				resetAllLoads();

				//RB = RF(src2)
				ulaInBMuxSel = UlaInputSelection_RF;
				rfSel = src2;//
				rfReadWriteBit = 0;
				//cout<<"loading RB with RF(src1)"<<endl;
				wait(1); //espera por RF
				loadRB = 1;
				wait(1);//espera por loadRB
				resetAllLoads();

				//RF(dest) = ulaOut
				ulaOutDemuxSel = UlaOutputSelection_RF;
				rfSel = dest;
				rfReadWriteBit = 1;
				wait(1);

				return true;
			}
			break;
	}
}

bool ControlUnit::processMemoryInstruction(){
	
	int operation = iRInput.read().range(13,9).to_int();
	//cout<<__FILE__<<"::"<<__LINE__<<endl;
	switch (operation){
		case 0: //RF[dest] = address
			{
				//cout<<__FILE__<<"::"<<__LINE__<<endl;
				int dest = iRInput.read().range(8,6).to_int();
				//guarda o endereço onde será guardado address
				rfSel = dest; 

				//pega o dado de AR em pc atual
				loadsARWithPc();

				//RF[dest] = PC
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //Address
				loadRA = 1;
				wait(1); //espera RA
				ulaOutDemuxSel = UlaOutputSelection_RF; //RF
				rfReadWriteBit = 1; //carrega RF com o valor em rfSel
				wait(1);//espera RF
				resetAllLoads();
			
				incrementPC();

				return true;
			}
			break;
		case 1: //RF[dest] = Mem[address]
			{
				//cout<<__FILE__<<"::"<<__LINE__<<endl;
				//guarda o endereço onde será guardado address
				int dest = iRInput.read().range(8,6).to_int();
				rfSel = dest; 

				loadsARWithPc();
				
				resetAllLoads();
				
				//RA = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				loadRA = 1;
				wait(1);//espera RA

				//AR = DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				wait(1); //wait for AR
				writeMemory = 0; //read from memory
				loadDR = 1;
				wait(1);//espera DR
				wait(1);//delay da memoria

				resetAllLoads();

				//RF[dest] = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				loadRA= 1; //Loads RA
				wait(1);//espera RA

				ulaOutDemuxSel = UlaOutputSelection_RF; //RF
				rfReadWriteBit = 1; //carrega RF com o valor em rfSel
				wait(1); //espera RF

				incrementPC();

				return true;
			}
			break;
		case 2: //RF[dest] = Mem[Mem[address]]
			{
				//guarda o endereço onde será guardado address
				int dest = iRInput.read().range(8,6).to_int();
				rfSel = dest; 

				loadsARWithPc();
				
				resetAllLoads();
				
				//RA = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				loadRA = 1;
				wait(1);//espera RA

				//AR = DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				wait(1); //wait for AR
				writeMemory = 0; //read from memory
				loadDR = 1;
				wait(1);//espera DR
				wait(1);//delay da memoria

				resetAllLoads();

				//RA = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				loadRA = 1;
				wait(1);//espera RA

				//AR = DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				wait(1); //wait for AR
				writeMemory = 0; //read from memory
				loadDR = 1;
				wait(1);//espera DR
				wait(1);//delay da memoria

				//RF[dest] = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				loadRA= 1; //Loads RA
				wait(1);//espera RA

				ulaOutDemuxSel = UlaOutputSelection_RF; //RF
				rfReadWriteBit = 1; //carrega RF com o valor em rfSel
				wait(1); //espera RF

				incrementPC();

				return true;
			}
			break;
		case 3: //Mem[address] = RF[src1]
			{
				//guarda o endereço onde será guardado address
				int src1 = iRInput.read().range(5,3).to_int();
				rfSel = src1; 

				loadsARWithPc();

				resetAllLoads();

				//RA = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				loadRA = 1;
				wait(1);//espera RA

				//AR = DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				wait(1);

				resetAllLoads();
				
				//DR = RF[rfSel]
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_RF; //RF
				loadRA = 1;
				wait(1);

				ulaOutDemuxSel = UlaOutputSelection_DR; //DR
				dRinMuxSel = 0;
				rfReadWriteBit = 0; //lê valor de RF[rfSel]
				loadDR = 1; //Loads DR
				wait(1);

				resetAllLoads();
				//Mem[AR] = DR
				writeMemory = 1;
				dRoutDemuxSel = 1;

				wait(1);
				wait(1);//delay da memoria
				
				incrementPC();

				return true;
			}
			break;
		case 4: //Mem[Mem[address]] = RF[src1]
			{
				//guarda o endereço onde será guardado address
				int src1 = iRInput.read().range(5,3).to_int();
				rfSel = src1; 

				loadsARWithPc();

				resetAllLoads();

				//cout<<"\nAR = DR\n";
				//RA = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				loadRA = 1;
				wait(1);//espera RA

				//AR = DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				wait(1); //wait for AR
				writeMemory = 0; //read from memory
				loadDR = 1;
				wait(1);//espera DR
				wait(1);//delay da memoria

				resetAllLoads();

				//cout<<"\nAR = DR\n";
				//RA = DR
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_DR; //DR
				loadRA = 1;
				wait(1);//espera RA

				//AR = DR
				ulaOutDemuxSel = UlaOutputSelection_AR; //AR
				loadAR = 1; //Loads AR
				wait(1); //wait for AR
				writeMemory = 0; //read from memory
				loadDR = 1;
				wait(1);//espera DR
				wait(1);//delay da memoria


				resetAllLoads();

				//cout<<"\nDR = RF[src1]\n";
				//DR = RF[rfSel]
				ulaOp = ulaOperation_doNothing;
				ulaInAMuxSel = UlaInputSelection_RF; //RF
				loadRA = 1;
				wait(1);

				ulaOutDemuxSel = UlaOutputSelection_DR; //DR
				dRinMuxSel = 0;
				rfReadWriteBit = 0; //lê valor de RF[rfSel]
				loadDR = 1; //Loads DR
				wait(1);

				resetAllLoads();
				//Mem[AR] = DR
				//cout<<"\nmem[AR] = DR\n";
				writeMemory = 1;
				dRoutDemuxSel = 1;

				wait(1);
				wait(1);//delay da memoria
				

				incrementPC();

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
	dRinMuxSel = 1;
	dRoutDemuxSel = 0;
	//loadStatus = 0;
}

