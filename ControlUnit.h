
#ifndef ControlUnit_h
#define ControlUnit_h

typedef enum UlaInputSelection{
	UlaInputSelection_RF,
	UlaInputSelection_DR,
	UlaInputSelection_AR,
	UlaInputSelection_PC,
	UlaInputSelection_IR,
	UlaInputSelection_RStatus,
};

typedef enum UlaOutputSelection{
	UlaOutputSelection_RF,
	UlaOutputSelection_DR,
	UlaOutputSelection_AR,
	UlaOutputSelection_PC,
	UlaOutputSelection_IR,
	UlaOutputSelection_RStatus,
};

SC_MODULE (ControlUnit) {
	public:
		sc_in<bool> clock;
		//vem diretamente da saída de IR
		sc_in<sc_int<2>> instructionType;
		sc_in<sc_int<6>> operation;
		sc_in<sc_int<3>> dest;
		sc_in<sc_int<3>> src1;
		sc_in<sc_int<3>> src2;
		
		sc_in<bool> statusBit;


		sc_out<sc_int<WORD_SIZE>> ulaOp;
		sc_out<sc_int<WORD_SIZE>> ulaOutDemuxSel;
		sc_out<sc_int<WORD_SIZE>> muxASel;
		sc_out<sc_int<WORD_SIZE>> muxBSel;
	
		sc_out<bool> writeMemory;
		sc_out<bool> loadPC;
		//espaço de endereçamento de RF é igual ao tamanho de dest, src1 e src2
		sc_out<sc_int<3>> RF;
		sc_out<bool> loadRA;
		sc_out<bool> loadRB;
		sc_out<bool> loadIR;
		sc_out<bool> loadAR;

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
		void processInstruction();


		SC_CTOR(ControlUnit){
			SC_METHOD(ControlUnitBehaviour);
			sensitive << clock.pos();
		}
};

#endif
