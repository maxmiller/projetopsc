#include "systemc.h"
#include "MemoryTest.h"


int sc_main (int argc, char* argv[]) {
	//iniciomemory memoria("memory");
	sc_clock clk ("my_clock",1,0.5);
	MemoryTest *memoria = new MemoryTest("MemoryTest");
	memoria->clk(clk.signal());
	
		
	sc_trace_file *tf = sc_create_vcd_trace_file("memory");
	sc_trace(tf,memoria->clk,"clk");
	sc_trace(tf,memoria->rst,"rst");
	sc_trace(tf,memoria->address, "address");
	sc_trace(tf,memoria->datai, "datai");
	sc_trace(tf,memoria->datao, "datao");
	sc_trace(tf,memoria->writer, "writer");
	
	sc_start();
	
	sc_close_vcd_trace_file(tf);
	return 0;
	
}