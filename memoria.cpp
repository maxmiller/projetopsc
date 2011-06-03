#include "systemc.h"
#include "memory.h"

class top:public sc_module {
	sync_mem *memoria;
	memtb *tb;
	sc_signal <bool > rst;
	sc_signal <sc_uint < 10 > >address;
	sc_signal <sc_uint < 8 > >datai;
	sc_signal <sc_uint < 8 > >datao;
	sc_signal <bool > write;
	top(std::string name):
		sc_module(name) {

	FILE *tf = fopen("out.vcd", "w");
	sc_clock systemclk
		("systemclk", 5, SC_NS);
	memoria = new sync_mem("duv");
	memoria = new memtb("testbench");
	memoria->rst(rst);
	memoria->clk(systemclk);
	memoria->address(address);
	memoria->datai(datai);
	memoria->datao(datao);
	memoria->write(write);
	tb->clk(systemclk);
		tb->rst(rst);
	tb->address(address);
	tb->datai(datai);
	tb->datao(datao);
	tb->write(write);
	sc_trace(tf,systemclk,"clk");
	sc_trace(tf, address, "addr");
	sc_trace(tf, datai, "di");
	sc_trace(tf, datao, "do");
	sc_trace(tf, write, "wr");
	}
	~top() {
		delete memoria;
		delete tb;
	}
};