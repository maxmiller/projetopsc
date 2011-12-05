#ifndef CustomizableControlUnit_h
#define CustomizableControlUnit_h
#include <systemc.h>
#include "config.h"
#include "PipelineControl.h"
#include <map>
#include <vector>
#include <deque>

typedef struct WatingInstruction_t{
	std::vector< std::string > microInstructions;
	int instructionPosition;
	int waitTime;
	bool isFetchInstruction;
}WaitingInstruction;

SC_MODULE (CustomizableControlUnit) {
	public:
		sc_in<bool> clock;

	
		/*!
		 * Essa hash mapeia cada valor de IR em um conjunto de valores que devem ser setados na saída da unidade de controle 
		 */
		std::map<std::string, std::vector<std::string> > stateOutputMap;
		std::map<std::string, std::vector<std::string> > pipeLineControlMap;

		std::deque< WaitingInstruction* > instructionPipeline;


		/*!
		 * Essa hash mapeia cada nome de porta em uma saída da unidade de controle
		 */
		std::vector<std::string> outputNames;
		std::vector<std::string> inputNames;

		sc_out<sc_int<WORD_SIZE> > *outputs;
		sc_in<sc_int<WORD_SIZE> > *inputs;

		int state; //controls the control unit state
		int instructionSubstate; //controls each instruction individual substate
		int numPipelineStages;

		PipelineControl *pipelineControl;
	public:
		void customizableControlUnitBehavior();
		//!processa instrução
		/*!
		 * executa todas as instruções no vetor
		 * \param tinstructions o vetor contendo as instruções
		 * \param op o valor de op extraído de IR
		 * \param dest o valor de dest extraído de IR
		 * \param src1 o valor de src1 extraído de IR
		 * \param src2 o valor de src2 extraído de IR
		 */
		void executeMicroInstructions(std::vector<std::string> instructions, int &instructionPosition, int &waitTime, bool pipelineControl);

		//!processa um conjunto de strings que representam as micro instruções
		/*!
		 * processa as strings de mircor instruções e as transforma em uma 
		 * map<string,valor> o primeiro elemento representa a saída o segundo
		 * elemento representa o valor daquela saída
		 * \param instructions o vetor contendo as micro instruções
		 */
		std::vector< std::pair<std::string, int> > parseInstruction(std::string instruction);

		//!atribui um signal a um output
		/*!
		 * \param outputName o nome do output desejado
		 * \param signal o signal a ser atribuido à esse output
		 */
		void assignOutput(std::string outputName, sc_signal< sc_int<WORD_SIZE> > *signal);
		void assignOutput(std::string outputName, sc_out< sc_int<WORD_SIZE> > *signal);
		void assignInput(std::string inputName, sc_signal< sc_int<WORD_SIZE> > *signal);
		int getInputValue(std::string inputName);

		//! checa se uma dada condição é verdadeira
		/* \brief checa se uma dada condição é válida, os formatos esperados para as condições são:
		 *  1 - test eq/neq inputName[value,value] value. Ex: test eq irInput[0,1] 1, testa se a sub palavra irInput[0,1] é igual a 1.
		 *  2 - test eq/neq inputName[] value. Ex: test eq irInput[] 10, testa se irInput é igual a 10.
		 *  3 - test eq/neq inputName[value,value]/input[] inputName[value,value]/input[]. Ex: test eq irInput[0,1] irExStageInput[0,1], testa se irInput[0,1] and irExStageInput[0,1] são iguais.
		 * \param condition a condição no formato string
		*/
		bool checkConditions(std::string conditions);

		bool processPipelineControl();


		SC_HAS_PROCESS (CustomizableControlUnit);
		CustomizableControlUnit(sc_module_name name, std::vector<std::string> outputNames, std::vector<std::string> inputNames, std::map<std::string, std::vector<std::string> > stateOutputMap, PipelineControl *pipelineControl, int numPipelineStages);


};

#endif
