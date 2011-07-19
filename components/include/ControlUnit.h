
#ifndef ControlUnit_h
#define ControlUnit_h
#include <systemc.h>
#include "config.h"

typedef enum UlaInputSelection_t{
	UlaInputSelection_AR, //0
	UlaInputSelection_DR, //1
	UlaInputSelection_PC, //2
	UlaInputSelection_IR, //3
	UlaInputSelection_RF, //4
	UlaInputSelection_RStatus, //5
}UlaInputSelection;

typedef enum UlaOutputSelection_t{
	UlaOutputSelection_AR, //0
	UlaOutputSelection_DR, //1
	UlaOutputSelection_PC, //2
	UlaOutputSelection_IR, //3
	UlaOutputSelection_RF, //4
	UlaOutputSelection_RStatus, //5
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
		
		/*!
		 * Esse seletor escolhe se o dado de DR vem da memória ou da ula
		 * 0 => dado vem da ula
		 * 1 => dado vem da memória
		 */
		sc_out<sc_int<WORD_SIZE> > dRinMuxSel;
		/*!
		 * Esse seletor escolhe se o dado de DR vai para a memória ou para algum
		 * dos registradores da ULA
		 * 0 => dado vai para a ULA da ula
		 * 1 => dado vai para a memória
		 */
		sc_out<sc_int<WORD_SIZE> > dRoutDemuxSel;


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
		void controlUnitBehavior();
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

		//!reseta todas as saídas de load
		/*!
		 * Seta 0 em rfReadWriteBit, writeMemory, loadXX, coloca o dRinMuxSel = 1 e dRoutDemuxSel = 0
		 */
		void resetAllLoads();

		//!utilitária para incrementar PC
		void incrementPC();

		//!utilitária para carregar AR em PC
		void loadsARWithPc();

		SC_CTOR(ControlUnit){
			state = 0;
			SC_THREAD(controlUnitBehavior);
			sensitive<<clock.pos();
		}
};

#endif
