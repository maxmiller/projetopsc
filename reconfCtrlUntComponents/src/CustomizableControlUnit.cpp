#include "CustomizableControlUnit.h"
#include <sstream>
#include <algorithm>

using namespace std;

CustomizableControlUnit::CustomizableControlUnit(sc_module_name name, std::vector<std::string> outputNames, std::map<std::string,vector<std::string> > stateOutputMap) : sc_module(name){
	SC_THREAD(customizableControlUnitBehavior);
	sensitive<<clock.pos();
	this->outputNames = outputNames;
	this->outputs = new sc_out<sc_int<WORD_SIZE> >[outputNames.size()]; 
	this->stateOutputMap = stateOutputMap;
}

void CustomizableControlUnit::customizableControlUnitBehavior(){
	int type = iRInput.read().range(15,14);
	int op = iRInput.read().range(13,9);
	int dest = iRInput.read().range(8,6);
	int src1 = iRInput.read().range(5,3);
	int src2 = iRInput.read().range(2,0);

	stringstream aux;
	aux<<type<<","<<op;
	
	if(stateOutputMap.count(aux.str()) > 0){
		vector<std::string> microInstructions = stateOutputMap[aux.str()];
		executeMicroInstructions(microInstructions, op, dest, src1, src2);
	}
	else{
		cout<<"IR has a invalid value for this control unit"<<endl;
	}
}

void CustomizableControlUnit::executeMicroInstructions(vector<std::string> instructions, int op, int dest, int src1, int src2){
	//intera em todas as instruçoes para esse estado e executa cada uma delas
	vector<std::string >::iterator microInstructionIt = instructions.begin();
	for(;microInstructionIt != instructions.end(); microInstructionIt++){
		//intera em todas as saídas para essa instrução seta cada uma delas
		map<string, int > stateCommands = parseInstruction(*microInstructionIt);
		map<string, int >::iterator commandsIt = stateCommands.begin();
		for(;commandsIt != stateCommands.end(); commandsIt ++){
			if(commandsIt->first == "wait")
				wait(commandsIt->second);
			else{
				vector<std::string>::iterator portIndexIt = std::find(outputNames.begin(),outputNames.end(),commandsIt->first);
				if(portIndexIt != outputNames.end()){
					int index = std::distance(outputNames.begin(), portIndexIt);
					outputs[index] = commandsIt->second;
				}
				else
					cout<<"control unit trying to access a non existing output"<<endl;
			}
		}
	}
}

//! retorna um vetor onde cada elemento é um token do vetor s, usando o delimitador como " "
vector<string> tokenize(string str){
	string strR = "";

	string::size_type lastPos = str.find_first_not_of(" ", 0);
	string::size_type pos = str.find_first_of(" ", lastPos);

	vector<string> retVector;

	while (string::npos != pos || string::npos != lastPos) {
		retVector.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(" ", pos);
		pos = str.find_first_of(" ", lastPos);
	}
	return retVector;
}

map<string, int > CustomizableControlUnit::parseInstruction(string instruction){
	vector<string> aux = tokenize(instruction);
	vector<string>::iterator it;
	map<string, int > retMap;
	int output;
	string port;
	bool portAquired = false;

	for(it = aux.begin(); it != aux.end(); it ++){
		if(!portAquired){
			port = *it;
			portAquired = true;
		}
		else{
			output = atoi((*it).c_str());
			retMap[port] = output;
			portAquired = false;
		}
	}
	return retMap;
}

void CustomizableControlUnit::assign(string outputName, sc_signal< sc_int<WORD_SIZE> > *signal){
	vector<std::string>::iterator portIndexIt = std::find(outputNames.begin(),outputNames.end(),outputName);
	if(portIndexIt != outputNames.end()){
		int index = std::distance(outputNames.begin(), portIndexIt);
		outputs[index](*signal);
	}
	else
		cout<<"trying to access a non existing output"<<endl;
}


void CustomizableControlUnit::assign(string outputName, sc_out< sc_int<WORD_SIZE> > *out){
	vector<std::string>::iterator portIndexIt = std::find(outputNames.begin(),outputNames.end(),outputName);
	if(portIndexIt != outputNames.end()){
		int index = std::distance(outputNames.begin(), portIndexIt);
		outputs[index](*out);
	}
	else
		cout<<"trying to access a non existing output"<<endl;
}

