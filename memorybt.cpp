#include "systemc.h"
#include "memory.h"

int sc_main (int argc, char* argv[]) {
	memory memoria("memory");
	sc_set_time_resolution(1, SC_PS);
	
	sc_trace_file *tf = sc_create_vcd_trace_file("memory");
	sc_trace(tf,memoria.clk,"clk");
	sc_trace(tf,memoria.rst,"rst");
	sc_trace(tf,memoria.addres, "addres");
	sc_trace(tf,memoria.data, "data");
	sc_trace(tf,memoria.writer, "writer");
	
	sc_signal <bool > clk,rst,writer;
	sc_signal <sc_uint < 8 > >addres;
	sc_signal <sc_uint < 8 > >data;
	
	memoria.rst(rst);
	memoria.clk(clk);
	memoria.writer(writer);
	memoria.addres(addres);
	memoria.data(data);
	memoria.writer(writer);
	
	//sc_start();
	
	
	//int i=0;
	rst  = 0;
	data    = 0;
	addres = 0;
	writer  = 0;
	clk=0;
	sc_start(0, SC_PS);
	
	writer      = 1;
	addres.write(0x10);
	data.write(0x110011);
	rst  = 1;
	clk=1;
	sc_start(5, SC_PS);
	
	rst  = 0;
	
	sc_start(5, SC_PS);
	addres.write(0x12);
	data.write(0x1100);
	rst  = 1;
	clk=0;
	
	sc_start(5, SC_PS);
	
	addres.write(0x8);
	data.write(0x1010);
	sc_start(5, SC_PS);
	
	writer      = 0;
	addres = 0x10;
	
	sc_start(5, SC_PS);
	
	addres = 0x1;
	
	sc_start(5, SC_PS);
	
	
	addres = 0x3;
	
	sc_start(5, SC_PS);
	
	rst  = 0;
	clk=1;
	sc_start(1, SC_PS);
	
	
	cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;
	
	sc_close_vcd_trace_file(tf);
	return 0;
	
}