
#ifndef ControlUnit_h
#define ControlUnit_h
#include <systemc.h>
#include "config.h"

typedef enum UlaInputSelection_t{
	UlaInputSelection_RF,
	UlaInputSelection_DR,
	UlaInputSelection_AR,
	UlaInputSelection_PC,
	UlaInputSelection_IR,
	UlaInputSelection_RStatus,
}UlaInputSelection;

typedef enum UlaOutputSelection_t{
	UlaOutputSelection_RF,
	UlaOutputSelection_DR,
	UlaOutputSelection_AR,
	UlaOutputSelection_PC,
	UlaOutputSelection_IR,
	UlaOutputSelection_RStatus,
}UlaOutputSelection;

SC_MODULE (ControlUnit) {
	public:
		sc_in<bool> clock;
		//vem diretamente da saída de IR
		//
		sc_in<sc_int<WORD_SIZE> > iRInput;
//		int instructionType;
//		int operation;
//		int dest;
//		int src1;
//		int src2;
		
		sc_in<bool> statusBit;


		sc_out<sc_int<WORD_SIZE> > ulaOp;
		sc_out<sc_int<WORD_SIZE> > ulaOutDemuxSel;
		sc_out<sc_int<WORD_SIZE> > ulaInAMuxSel;
		sc_out<sc_int<WORD_SIZE> > ulaInBMuxSel;


		sc_out<sc_int<WORD_SIZE> > rfSel;
		sc_out<bool> rfReadWriteBit;
		sc_out<bool> writeMemory;
	
		sc_out<bool> loadRA;
		sc_out<bool> loadRB;
		sc_out<bool> loadIR;
		sc_out<bool> loadAR;
		sc_out<bool> loadPC;
		sc_out<bool> loadDR;
	//	sc_out<bool> loadStatus;


		int state; //controls the control unit state
		int instructionSubstate; //controls each instruction individual substate
	public:
		void ControlUnitBehaviour();
		//!processa instrução qualquer e a subdivide em cada um dos tipos
		/*!
		 * Quando o processador incrementa o PC e carrega o registrador de instru-
		 * ção com a nova instrução ele irá sempre executar esse método
		 * \return true se a operação foi completada
		 */
		bool processInstruction();

		//!processa instruções de micelania
		/*!
		 * Processa as instrucoes
		 * TODO listar instrucoes implementadas
		 * \return true se a operação foi completada
		 */
		bool processMiscellaneousInstruction();

		//!processa instruções de controle
		/*!
		 * Processa as instrucoes
		 * TODO listar instrucoes implementadas
		 * \return true se a operação foi completada
		 */
		bool processControlInstruction();
		//!processa instruções de operação com registradores
		/*!
		 * Processa as instrucoes
		 * TODO listar instrucoes implementadas
		 * \return true se a operação foi completada
		 */
		bool processRegisterInstruction();

		//!processa instruções de operação com memória
		/*!
		 * Processa as instrucoes
		 * TODO listar instrucoes implementadas
		 * \return true se a operação foi completada
		 */
		bool processMemoryInstruction();

		void resetAllLoads();

		SC_CTOR(ControlUnit){
			state = 0;
			SC_THREAD(ControlUnitBehaviour);
			sensitive<<clock;
		}
};

#endif
