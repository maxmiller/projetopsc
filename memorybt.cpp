#include "systemc.h"
#include "memory.h"

int sc_main (int argc, char* argv[]) {
	sc_signal <bool > clk,rst,writer;
	sc_signal <sc_uint < 8 > >addres;
	sc_signal <sc_uint < 8 > >datai;
	sc_signal <sc_uint < 8 > >datao;
	sc_signal <bool > write;
	memory memoria("memoria");
	//memoria = new memory("duv");
	//tb = new memory("testbench");
	memoria.rst(rst);
	memoria.clk(clk);
	memoria.writer(writer);
	memoria.addres(addres);
	memoria.datai(datai);
	memoria.datao(datao);
	memoria.writer(write);
	
	sc_start();
	
	sc_trace_file *tf = sc_create_vcd_trace_file("memoria");
	sc_trace(tf,clk,"clk");
	sc_trace(tf,rst,"rst");
	sc_trace(tf, addres, "address");
	sc_trace(tf, datai, "datai");
	sc_trace(tf, datao, "datao");
	sc_trace(tf, writer, "writer");
	//int i=0;
	rst = 0;       
	writer = 0;     
	datai = 1;
	clk = 0; 
	sc_start(1);
	clk = 1; 
	sc_start(1);
	
	rst = 1;    // Assert the reset
	clk = 0; 
	datai = 1;
	//sc_start(1);
	clk = 1; 
	sc_start(1);
	rst = 0;    // De-assert the reset
	clk = 0; 
	sc_start(1);
	clk = 1; 
	sc_start(1);
	writer = 1;  // Assert enable
	clk = 0; 
	sc_start(1);
	clk = 1; 
	sc_start(1);
	writer = 0; // De-assert enable
	
	cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;
	
	sc_close_vcd_trace_file(tf);
	return 0;
	
}