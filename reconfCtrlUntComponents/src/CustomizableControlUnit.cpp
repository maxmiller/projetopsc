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

	while(true){
		cout<<"fetching instruction "<<endl;
		if(stateOutputMap.count("fetchInstruction") > 0){
			vector<std::string> microInstructions = stateOutputMap["fetchInstruction"];
			executeMicroInstructions(microInstructions, 0, 0, 0, 0);
		}
		else
			cout<<"Cannot execute a control unit without fetchInstrucion procedure"<<endl;

		int type = iRInput.read().range(15,14);
		int op = iRInput.read().range(13,9);
		int dest = iRInput.read().range(8,6);
		int src1 = iRInput.read().range(5,3);
		int src2 = iRInput.read().range(2,0);

		cout<<"executing instruction "<<endl;
		cout<<"type:"<<type<<endl;
		cout<<"op:"<<op<<endl;
		cout<<"dest:"<<dest<<endl;
		cout<<"src1:"<<src1<<endl;
		cout<<"src2:"<<src2<<endl;

		stringstream aux;
		aux<<type<<"_"<<op;

		if(stateOutputMap.count(aux.str()) > 0){
			vector<std::string> microInstructions = stateOutputMap[aux.str()];
			executeMicroInstructions(microInstructions, op, dest, src1, src2);
		}
		else{
			cout<<"IR has a invalid value for this control unit"<<endl;
		}
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



void CustomizableControlUnit::executeMicroInstructions(vector<std::string> instructions, int op, int dest, int src1, int src2){
	//intera em todas as instruçoes para esse estado e executa cada uma delas
	vector<std::string >::iterator microInstructionIt = instructions.begin();
	bool canExecute = true;
	for(;microInstructionIt != instructions.end(); microInstructionIt++){
		//intera em todas as saídas para essa instrução seta cada uma delas
		//cout<<"parsing instruction "<<*microInstructionIt<<endl;
		vector< pair<string, int> > stateCommands = parseInstruction(*microInstructionIt, op, dest, src1, src2);
		vector< pair<string, int> >::iterator commandsIt = stateCommands.begin();
		for(;commandsIt != stateCommands.end(); commandsIt ++){
			//cout<<"trying to execute "<<commandsIt->first <<" "<<commandsIt->second<<endl;
			vector<string> aux = tokenize(commandsIt->first);
			if(aux.size() == 0 && canExecute){
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
			else if(aux.size() == 2){ //test command
				string command = aux[0];
				string testPort = aux[1];
				if(command == "if") //TODO test other inputs
					canExecute = statusBit.read();
				if(command == "else")
					canExecute = !canExecute;
			}
		}
	}
}

vector< pair<string, int> > CustomizableControlUnit::parseInstruction(string instruction, int op, int dest, int src1, int src2){
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
			if(*it == "op"){
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

