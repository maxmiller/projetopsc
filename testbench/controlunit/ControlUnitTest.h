#include "systemc.h"
#include "ControlUnit.h"
#include "config.h"

#ifndef ControlUnitTest_h
#define ControlUnitTest_h

SC_MODULE(ControlUnitTest) {
	
	sc_in<bool> clock;
	
	sc_signal<sc_int<WORD_SIZE> > iRInput;
	//		int instructionType;
	//		int operation;
	//		int dest;
	//		int src1;
	//		int src2;
	
	sc_signal<bool> statusBit;
	
	
	sc_signal<sc_int<WORD_SIZE> > ulaOp;
	sc_signal<sc_int<WORD_SIZE> > ulaOutDemuxSel;
	sc_signal<sc_int<WORD_SIZE> > ulaInAMuxSel;
	sc_signal<sc_int<WORD_SIZE> > ulaInBMuxSel;
	
	
	sc_signal<sc_int<WORD_SIZE> > rfSel;
	sc_signal<bool> rfReadWriteBit;
	sc_signal<bool> writeMemory;
	
	sc_signal<bool> loadRA;
	sc_signal<bool> loadRB;
	sc_signal<bool> loadIR;
	sc_signal<bool> loadAR;
	sc_signal<bool> loadPC;
	sc_signal<bool> loadDR;	
	
	ControlUnit *control;
	
	void do_test();
	SC_CTOR(ControlUnitTest){
		control = new ControlUnit("ControlUnit");
		
		control->clock(clock);
		control->iRInput(iRInput);
		control->statusBit(statusBit);
		control->ulaOp(ulaOp);
		control->ulaOutDemuxSel(ulaOutDemuxSel);
		control->ulaInAMuxSel(ulaInAMuxSel);
		control->ulaInBMuxSel(ulaInBMuxSel);
		control->rfSel(rfSel);
		control->rfReadWriteBit(rfReadWriteBit);
		control->writeMemory(writeMemory);
		control->loadRA(loadRA);
		control->loadRB(loadRB);
		control->loadIR(loadIR);
		control->loadAR(loadAR);
		control->loadPC(loadPC);
		control->loadDR(loadDR);	
		
		SC_THREAD (do_test);
		dont_initialize();
		sensitive << clock.pos();
		
	}
	
};

#endif

