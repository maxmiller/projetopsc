#include "systemc.h"
#include "MultiplexerTest.h"
#include "DemultiplexerTest.h"

int sc_main (int argc, char * argv[]) {
    // insert code here...
    sc_clock clk ("my_clock",1,0.5);
	MultiplexerTest *rs = new MultiplexerTest("MultiplexerTest");
	rs->clock(clk.signal());
	
	
	sc_trace_file *tf = sc_create_vcd_trace_file("MultiplexerTest");
	sc_trace(tf,rs->clock,"clock");
	sc_trace(tf,rs->inputs[0],"inputs[0]");
	sc_trace(tf,rs->inputs[1],"inputs[1]");
	sc_trace(tf,rs->output, "output");
	sc_trace(tf,rs->sel, "sel");
	
	
		
	DemultiplexerTest *dem = new DemultiplexerTest("DemultiplexerTest");
	dem->clock(clk.signal());
	
	
	sc_trace_file *tw = sc_create_vcd_trace_file("DemultiplexerTest");
	sc_trace(tw,dem->clock,"clock");
	sc_trace(tw,dem->input,"input");
	sc_trace(tw,dem->outputs[0], "outputs[0]");
	sc_trace(tw,dem->outputs[1], "outputs[1]");
	sc_trace(tw,dem->sel, "sel");
	
	
	sc_start();
	sc_close_vcd_trace_file(tf);

	sc_close_vcd_trace_file(tw);
	return 0;
}
