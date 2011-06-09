#include "systemc.h"
#include "RegisterTest.h"

int sc_main (int argc, char * argv[]) {
 
	sc_clock clk ("my_clock",1,0.5);
	RegisterTest *rs = new RegisterTest("RegisterTest");
	rs->clock(clk.signal());
	
	
	sc_trace_file *tf = sc_create_vcd_trace_file("RegisterTest");
	sc_trace(tf,rs->clock,"clock");
	sc_trace(tf,rs->sel,"sel");
	sc_trace(tf,rs->in, "in");
	sc_trace(tf,rs->out, "out");
	sc_trace(tf,rs->rwBit, "rwBit");
	
	sc_start();
	
	sc_close_vcd_trace_file(tf);
	return 0;
}
