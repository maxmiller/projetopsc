#ifndef CustomizableControlUnit_h
#define CustomizableControlUnit_h
#include <systemc.h>
#include "config.h"
#include <map>
#include <vector>

SC_MODULE (CustomizableControlUnit) {
	public:
		sc_in<bool> clock;

	
		//vem diretamente da saída de IR
		sc_in<sc_int<WORD_SIZE> > iRInput;
		
		/*!
		 * Essa hash mapeia cada valor de IR em um conjunto de valores que devem ser setados na saída da unidade de controle 
		 */
		std::map<std::string, std::vector<std::string> > stateOutputMap;


		sc_in<bool> statusBit;

		/*!
		 * Essa hash mapeia cada nome de porta em uma saída da unidade de controle
		 */
		std::vector<std::string> outputNames;

		sc_out<sc_int<WORD_SIZE> > *outputs;

		int state; //controls the control unit state
		int instructionSubstate; //controls each instruction individual substate
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
		void executeMicroInstructions(std::vector<std::string> instructions, int op, int dest, int src1, int src2);

		//!processa um conjunto de strings que representam as micro instruções
		/*!
		 * processa as strings de mircor instruções e as transforma em uma 
		 * map<string,valor> o primeiro elemento representa a saída o segundo
		 * elemento representa o valor daquela saída
		 * \param tinstructions o vetor contendo as micro instruções
		 * \param op o valor de op extraído de IR
		 * \param dest o valor de dest extraído de IR
		 * \param src1 o valor de src1 extraído de IR
		 * \param src2 o valor de src2 extraído de IR
		 */
		std::vector< std::pair<std::string, int> > parseInstruction(std::string instruction,int op, int dest, int src1, int src2);

		//!atribui um signal a um output
		/*!
		 * \param outputName o nome do output desejado
		 * \param signal o signal a ser atribuido à esse output
		 */
		void assign(std::string outputName, sc_signal< sc_int<WORD_SIZE> > *signal);

		void assign(std::string outputName, sc_out< sc_int<WORD_SIZE> > *out);


		SC_HAS_PROCESS (CustomizableControlUnit);
		CustomizableControlUnit(sc_module_name name, std::vector<std::string> outputNames, std::map<std::string, std::vector<std::string> > stateOutputMap);


};

#endif
