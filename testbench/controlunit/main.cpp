#include "systemc.h"
//#include <iostream>
#include "ControlUnitTest.h"

int sc_main (int argc, char * argv[]) {
    sc_clock clk ("my_clock",1,0.5);
	ControlUnitTest *control = new ControlUnitTest("ControlUnitTest");
	control->clock(clk.signal());
	
	
	sc_trace_file *tf = sc_create_vcd_trace_file("ControlUnitTest");
	sc_trace(tf,control->clock,"clock");
	sc_trace(tf,control->iRInput,"iRInput");
	sc_trace(tf,control->statusBit,"statusBit");
	sc_trace(tf,control->ulaOp,"ulaOP");
	sc_trace(tf,control->ulaOutDemuxSel,"ulaOutDemuxSel");
	sc_trace(tf,control->ulaInAMuxSel,"ulaInAMuxSel");
	sc_trace(tf,control->ulaInBMuxSel,"ulaInBMuxSel");
	sc_trace(tf,control->rfSel,"rfSel");
	sc_trace(tf,control->rfReadWriteBit,"rfReadWriteBit");
	sc_trace(tf,control->writeMemory,"writeMemory");
	sc_trace(tf,control->loadRA,"loadRA");
	sc_trace(tf,control->loadRB,"loadRB");
	sc_trace(tf,control->loadIR,"loadIR");
	sc_trace(tf,control->loadAR,"loadAR");
	sc_trace(tf,control->loadPC,"loadPC");
	sc_trace(tf,control->loadDR,"loadDR");	
	
	
	sc_start();
	
	sc_close_vcd_trace_file(tf);
	return 0;
	
}
