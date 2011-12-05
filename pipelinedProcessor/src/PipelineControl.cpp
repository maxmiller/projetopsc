#include "PipelineControl.h"
#include "CustomizableControlUnit.h"

using namespace std;

PipelineControl::PipelineControl(map<string,vector<string> > pipelineControlStateCommands, map<string,string> pipelineControlStateMachineTransitionCondictions, map<string, vector<string> > pipelineControlStateMachineStateTransitions, string initialState){
	this->pipelineControlStateCommands = pipelineControlStateCommands;
	this->pipelineControlStateMachineTransitionCondictions = pipelineControlStateMachineTransitionCondictions;
	this->pipelineControlStateMachineStateTransitions = pipelineControlStateMachineStateTransitions;

	actualState = initialState;
}

bool PipelineControl::nextTransition(CustomizableControlUnit *controlUnit){
	
	vector<string> nextStates = pipelineControlStateMachineStateTransitions[actualState];
	string anyOtherState = "";
	string nextState = "";

	if(nextStates.size() > 0){ //any other non end state
		for(vector<string>::iterator it = nextStates.begin(); it != nextStates.end(); it++){
			string conditions = pipelineControlStateMachineTransitionCondictions[actualState + "_" + *it];
			if(conditions == "anyOther"){
				anyOtherState = *it;
			}
			else if(controlUnit->checkConditions(conditions)){
				nextState = *it;	
			}
		}
	}
	else{ //end state, return to the first state of operation
		nextState = initialState;
	}

	if(nextState != "")
		actualState = nextState;
	else if(anyOtherState != "")
		actualState = anyOtherState;
	else{
		cout<<"cant comput an next state to the pipeline state machine";
		return false;
	}
	return true;
}

vector<string> PipelineControl::getActualStateInstructions(){
	return pipelineControlStateCommands[actualState];
}

