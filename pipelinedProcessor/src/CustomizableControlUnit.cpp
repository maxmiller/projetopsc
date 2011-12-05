#include "CustomizableControlUnit.h"
#include <sstream>
#include <algorithm>

using namespace std;
vector<string> tokenize(string str);

CustomizableControlUnit::CustomizableControlUnit(sc_module_name name, std::vector<std::string> outputNames, std::vector<std::string> inputNames, std::map<std::string,vector<std::string> > stateOutputMap, PipelineControl *pipelineControl, int numPipelineStages) : sc_module(name){
	SC_THREAD(customizableControlUnitBehavior);
	sensitive<<clock.pos();
	this->outputNames = outputNames;
	this->inputNames = inputNames;
	this->outputs = new sc_out<sc_int<WORD_SIZE> >[outputNames.size()]; 
	this->inputs = new sc_in<sc_int<WORD_SIZE> >[inputNames.size()]; 
	this->stateOutputMap = stateOutputMap;
	this->pipeLineControlMap = pipeLineControlMap;
	this->pipelineControl = pipelineControl;
}

//tests conditions for structural and data hazards
//the string given is in the format:
//test eq <input> <input>/<value>
//where <input> is in the format:
//inputName[(number,number)?]
bool CustomizableControlUnit::checkConditions(string conditions){
	vector<string> conditionsSplited = tokenize(conditions);
	vector<string>::iterator it = conditionsSplited.begin();
	for(it = conditionsSplited.begin(); it != conditionsSplited.end(); it++){
		if(*it == "test"){
			string operation = *(it+1);
			string operandA = *(it+2);
			string operandB = *(it+3);
			if(operation == "eq"){
				return (getInputValue(operandA) == getInputValue(operandB));
			}
		}
	}
}

bool CustomizableControlUnit::processPipelineControl(){

	if(pipelineControl->nextTransition(this)){
		vector<string> instructions = pipelineControl->getActualStateInstructions();
		if(instructions.size() == 1 && instructions[0] == "fetchInstruction")
			return true;
		else{
			//instruções do pipeline control devem bloquear toda a unidade de
			//execuçao
			int aux1 = 0, aux2 =0;
			executeMicroInstructions(instructions,aux1, aux2,true);
			return false;
		}
	}
	cout<<"could not find a next transition in the pipeline control statemachine"<<endl;
	return true;
}

void CustomizableControlUnit::customizableControlUnitBehavior(){
	int loadedStructions = 0;
	while(true){
		
		if(processPipelineControl() && stateOutputMap.count("fetchInstruction") > 0){
			vector<std::string> microInstructions = stateOutputMap["fetchInstruction"];
			WaitingInstruction *instruction = new WaitingInstruction;
			instruction->microInstructions = microInstructions;
			instruction->waitTime = 0;
			instruction->isFetchInstruction = true;	
			instructionPipeline.push_back(instruction);
		}
		else
			cout<<"Cannot execute a control unit without fetchInstrucion procedure"<<endl;

		for(deque< WaitingInstruction* >::iterator it=instructionPipeline.begin(); it != instructionPipeline.end(); it++){
			
			if((*it)->waitTime == 0)
				executeMicroInstructions((*it)->microInstructions, (*it)->instructionPosition, (*it)->waitTime, false);

			int numerOfMicroInstructions = (*it)->microInstructions.size();
			//fetch acabou e nao esta esperando wait
			if((*it)->isFetchInstruction && (*it)->instructionPosition == numerOfMicroInstructions && (*it)->waitTime == 0){
				vector<std::string>::iterator portIndexIt = std::find(inputNames.begin(),inputNames.end(),"irInput");
				int index = std::distance(inputNames.begin(), portIndexIt);
				
				sc_int<WORD_SIZE> iRInput = inputs[index].read();
				int type = iRInput.range(15,14);
				int op = iRInput.range(13,9);
				int dest = iRInput.range(8,6);
				int src1 = iRInput.range(5,3);
				int src2 = iRInput.range(2,0);

				cout<<endl<<" putting instruction in pipeline"<<endl;
				cout<<" type:"<<type<<endl;
				cout<<" op:"<<op<<endl;
				cout<<" dest:"<<dest<<endl;
				cout<<" src1:"<<src1<<endl;
				cout<<" src2:"<<src2<<endl;


				int instructionIndex = std::distance(instructionPipeline.begin(), it);
				stringstream aux_1, aux_2;
				aux_1<<type<<"_"<<op; //type_op instruction type
				aux_2<<type<<"_all"; //type_all instruction type
				vector<std::string> microInstructions;
				if(stateOutputMap.count(aux_1.str()) > 0){
					microInstructions = stateOutputMap[aux_1.str()];
				}
				else if(stateOutputMap.count(aux_2.str()) > 0){
					microInstructions = stateOutputMap[aux_2.str()];
				}
				else{
					cout<<" IR has a invalid value for this control unit"<<endl;
				}
				
				//atualizando o vetor de instruções para a nova instrução
				(*it)->microInstructions = microInstructions;
				(*it)->instructionPosition = 0;
				(*it)->waitTime = 0;
				(*it)->isFetchInstruction = false;
			}
		}
		bool haveToWait = false;
		//processa se alguma instrução precisa de wait
		for(deque< WaitingInstruction* >::iterator it=instructionPipeline.begin(); it != instructionPipeline.end(); it++){
			if((*it)->waitTime > 0){
				haveToWait = true;
				(*it)->waitTime --;
			}
		}
		if(haveToWait)
			wait(1);
	}
}

//! retorna um vetor onde cada elemento é um token do vetor s, usando o delimitador como " "
vector<string> tokenize(string str){
	string strR = "";

	string::size_type next = str.find_first_of(" ");

	vector<string> retVector;
	string aux = str;
	string value = "";

	while (next != string::npos) {
		value = aux.substr(0,next);

		aux = aux.substr(next+1,aux.size());
		retVector.push_back(value);
		next = aux.find_first_of(" ");
		if(next == string::npos)
			retVector.push_back(aux);
	}
	return retVector;
}

int CustomizableControlUnit::getInputValue(string value){
	unsigned int bracketsOpen = value.find("[");
	unsigned int bracketsClose = value.find("[");

	if(bracketsOpen != string::npos && bracketsClose != string::npos){ //inputName[number,number] or inputName[]
		string inputName = value.substr(0, value.find("["));
		vector<std::string>::iterator portIndexIt = std::find(inputNames.begin(),inputNames.end(),inputName);
		if(portIndexIt != inputNames.end()){
			int inputIndex = std::distance(inputNames.begin(), portIndexIt);
			if(bracketsOpen + 1 != bracketsClose){ //[number,number]
				unsigned int commaPosition = value.find(",");
				if (commaPosition != string::npos){
					int first = atoi(value.substr(bracketsOpen + 1, commaPosition).c_str());
					int second = atoi(value.substr(commaPosition+1,bracketsClose).c_str());
					cout<<" received "<<value<<" returning "<<inputName<<"["<<first<<","<<second<<"]="<<inputs[inputIndex].read().range(first,second)<<endl;
					return inputs[inputIndex].read().range(first,second);
				}
				else{
					cout<<" wrong format to input[number,number]"<<endl;
					return 0;
				}
			}
			else{ //[]
				cout<<" received "<<value<<" returning "<<inputName<<endl;
				return inputs[inputIndex].read();
			}
		}
		else{
			cout<<" trying to access a non existing input "<<inputName<<endl;
			return 0;
		}
	}
	else{
		return atoi(value.c_str());
	}
	return 0;//never reaches here
}




void CustomizableControlUnit::executeMicroInstructions(vector<std::string> instructions, int &instructionPosition, int &waitTime, bool pipelineControl){
	//intera em todas as instruçoes para esse estado e executa cada uma delas
	vector<std::string >::iterator microInstructionIt = instructions.begin();
	bool canExecute = true;
	for(;microInstructionIt != instructions.end(); microInstructionIt++){
		//intera em todas as saídas para essa instrução seta cada uma delas
		//cout<<"parsing instruction "<<*microInstructionIt<<endl;
		vector< pair<string, int> > stateCommands = parseInstruction(*microInstructionIt);
		vector< pair<string, int> >::iterator commandsIt = stateCommands.begin();
		for(;commandsIt != stateCommands.end(); commandsIt ++){
			//cout<<"trying to execute "<<commandsIt->first <<" "<<commandsIt->second<<endl;
			vector<string> aux = tokenize(commandsIt->first);
			if(aux.size() == 0 && canExecute){
				if(commandsIt->first == "wait"){
					if(!pipelineControl){ 
						waitTime = commandsIt->second;
						instructionPosition = std::distance(stateCommands.begin(), commandsIt);	
						return;
					}
					else{
						wait(commandsIt->second);
					}
				}
				else{
					vector<std::string>::iterator portIndexIt = std::find(outputNames.begin(),outputNames.end(),commandsIt->first);
					if(portIndexIt != outputNames.end()){
						int index = std::distance(outputNames.begin(), portIndexIt);
						outputs[index] = commandsIt->second;
					}
					else
						cout<<"control unit trying to access a non existing output "<<*portIndexIt<<endl;
				}
			}
			else if(aux.size() == 2){ //test command
				string command = aux[0];
				string testPort = aux[1];
				if(command == "if") //TODO test other inputs
					canExecute = 1;//statusBit.read();
				if(command == "else")
					canExecute = !canExecute;
			}
		}
	}
}

vector< pair<string, int> > CustomizableControlUnit::parseInstruction(string instruction){
	vector<string> aux = tokenize(instruction);
	vector<string>::iterator it;
	vector< pair<string, int> > retVector;
	int output;
	string port;
	bool portAquired = false;
	bool ifAquired = false;
	bool elseAquired = false;

	//cout<<"vector got from tokenize:"<<endl;
	//for(it = aux.begin(); it != aux.end(); it ++){
	//	cout<<*it<<" ";
	//}
	//cout<<endl;
	
	pair<string, int> val;
	for(it = aux.begin(); it != aux.end(); it ++){
		if(!portAquired && !ifAquired && !elseAquired){
			if(*it == "if"){
				ifAquired = true;
			}
			else if(*it == "else"){
				elseAquired = true;
			}
			else{
				port = *it;
				portAquired = true;
			}
		}
		else if(portAquired){
			output = getInputValue(*it);
/*			if(*it == "op"){
				output = op; 
			}
			else if(*it == "dest"){
				output = dest; 
			}
			else if(*it == "src1"){
				output = src1;
			}
			else if(*it == "src2"){
				output = src2; 
			}
			else{
				output = atoi((*it).c_str());
			}
*/
			val.first = port;
			val.second = output;
			retVector.push_back(val);
			portAquired = false;
		}
		else if(ifAquired){
			val.first = "if " + *it; //port to be tested
			val.second = 0;
			ifAquired = false;
			retVector.push_back(val);
		}
		else if(elseAquired){
			val.first = "else " + *it;
			val.second = 0;
			elseAquired = false;
			retVector.push_back(val);
		}
	}
	return retVector;
}

void CustomizableControlUnit::assignOutput(string outputName, sc_signal< sc_int<WORD_SIZE> > *signal){
	vector<std::string>::iterator portIndexIt = std::find(outputNames.begin(),outputNames.end(),outputName);
	if(portIndexIt != outputNames.end()){
		int index = std::distance(outputNames.begin(), portIndexIt);
		outputs[index](*signal);
	}
	else
		cout<<"trying to access a non existing output "<<outputName<<endl;
}

void CustomizableControlUnit::assignOutput(string outputName, sc_out< sc_int<WORD_SIZE> > *signal){
	vector<std::string>::iterator portIndexIt = std::find(outputNames.begin(),outputNames.end(),outputName);
	if(portIndexIt != outputNames.end()){
		int index = std::distance(outputNames.begin(), portIndexIt);
		outputs[index](*signal);
	}
	else
		cout<<"trying to access a non existing output "<<outputName<<endl;
}



void CustomizableControlUnit::assignInput(string inputName, sc_signal< sc_int<WORD_SIZE> > *signal){
	vector<std::string>::iterator portIndexIt = std::find(inputNames.begin(),inputNames.end(),inputName);
	if(portIndexIt != inputNames.end()){
		int index = std::distance(inputNames.begin(), portIndexIt);
		inputs[index](*signal);
	}
	else
		cout<<"trying to access a non existing input "<<inputName<<endl;
}




