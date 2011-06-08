#include <systemc.h>
#include "ControlUnit.h"

int sc_main(int argc , char *argv[]){

	sc_clock clk ("my_clock",1,0.5);
	
	sc_trace_file *wf = sc_create_vcd_trace_file("register");
	/*sc_trace(wf, test->clk, "clk");
	sc_trace(wf, test->reset, "reset");
	sc_trace(wf, test->dataInA, "dataInA");
	sc_trace(wf, test->dataInB, "dataInB");
	sc_trace(wf, test->dataOut, "dataOut");
	*/
	sc_start();

	sc_close_vcd_trace_file(wf);
	return 0;
}
