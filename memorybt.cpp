#include "systemc.h"
#include "memory.h"


int sc_main (int argc, char* argv[]) {
	memory memoria("memory");
	sc_set_time_resolution(1, SC_PS);
	
	sc_trace_file *tf = sc_create_vcd_trace_file("memory");
	sc_trace(tf,memoria.clk,"clk");
	sc_trace(tf,memoria.rst,"rst");
	sc_trace(tf,memoria.address, "address");
	sc_trace(tf,memoria.datai, "datai");
	sc_trace(tf,memoria.datao, "datao");
	sc_trace(tf,memoria.writer, "writer");
	
	sc_signal <bool > rst,writer,clk;
	sc_signal <sc_uint < 32 > >address;
	sc_signal <sc_uint < 32 > >datai;
	sc_signal <sc_uint < 32 > >datao;
	
	memoria.rst(rst);
	memoria.clk(clk);
	memoria.writer(writer);
	memoria.address(address);
	memoria.datai(datai);
	memoria.datao(datao);
	
	//Valores iniciais
	clk=0; //clock 
	rst  = 0; //reset
	datai    = 0; //sem entrada
	datao    = 0; // sem saida
	address = 0; //sem adress
	writer  = 0; //sem escrita
	//clk=0;
	sc_start(1, SC_PS); //inicio
	clk=1; //mudança de clock
	writer = 1; // habilita escrita
	address.write(0x10); //escreve o endereco
	datai.write(20); // escreve a entra
	rst  = 1; // habilita o reset
	sc_start(1, SC_PS);
	clk=0; //muda o clock
	writer = 0; // desabilita o writer
	rst  = 0; //desabilita o reset
	address.write(0x10); //escreve o endereco
	
	sc_start(1, SC_PS);
	
	cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;
	
	sc_close_vcd_trace_file(tf);
	return 0;
	
}