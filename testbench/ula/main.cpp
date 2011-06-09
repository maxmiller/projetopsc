#include "systemc.h"
#include "ULATest.h"

int sc_main (int argc, char * argv[]) {
    // insert code here...
    sc_clock clk ("my_clock",1,0.5);
	ULATest *rs = new ULATest("ULATest");
	rs->clock(clk.signal());
	
	
	sc_trace_file *tf = sc_create_vcd_trace_file("ULATest");
	sc_trace(tf,rs->clock,"clock");
	sc_trace(tf,rs->opA,"opA");
	sc_trace(tf,rs->opB, "opB");
	sc_trace(tf,rs->opSel, "opSel");
	sc_trace(tf,rs->ulaOut, "ulaOut");
	
	sc_start();
	
	sc_close_vcd_trace_file(tf);
	return 0;
}
