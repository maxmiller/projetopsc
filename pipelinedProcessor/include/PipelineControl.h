#ifndef PipelineControl_h
#define PipelineControl_h

#include <map>
#include <iostream>
#include <vector>

class CustomizableControlUnit;

class PipelineControl{
	public:
		std::map<std::string,std::vector<std::string> > pipelineControlStateCommands;
		std::map<std::string,std::string> pipelineControlStateMachineTransitionCondictions;
		std::map<std::string,std::vector<std::string> > pipelineControlStateMachineStateTransitions;

		std::string actualState;
		std::string initialState;

	public:
		PipelineControl(std::map<std::string,std::vector<std::string> > pipeLineControlMap, std::map<std::string,std::string> pipelineControlStateMachineTransitionCondictions, std::map<std::string,std::vector<std::string> > pipelineControlStateMachineStateTransitions, std::string initialState);
		bool nextTransition(CustomizableControlUnit *controlUnit);
		std::vector<std::string> getActualStateInstructions();
};
#endif
