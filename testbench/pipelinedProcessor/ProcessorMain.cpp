#include <systemc.h>
#include "ProcessorTest.h"

int sc_main (int argc, char * argv[]) {
 
	sc_clock clk ("my_clock",1,0.5);
	ProcessorTest *processorTest = new ProcessorTest("ProcessorTest");
	processorTest->clock(clk.signal());
	
	/*
	sc_trace_file *tf = sc_create_vcd_trace_file("ProcessorTest");
	sc_trace(tf,processorTest->clock,"clock");
	sc_trace(tf,processorTest->memoryDataISignal,"memoryDataISignal");
	sc_trace(tf,processorTest->memoryDataOSignal,"memoryDataOSignal");


	sc_trace(tf,processorTest->processor->pCoutSignal,"pcValue");
	sc_trace(tf,processorTest->processor->iRoutSignal,"instruction");
*/

	sc_start();
	
//	sc_close_vcd_trace_file(tf);
	return 0;
}
