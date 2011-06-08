#ifndef RULA_h
#define RULA_h

SC_MODULE(Memory) {
	public:
		//portas
		sc_in<bool> clk, rst, write;
		sc_in<sc_uint<8> > address;
		sc_in<sc_uint<8> > datai;
		sc_out<sc_uint<8> > datao;
		//internos
		sc_uint<sc_uint<8> > mem[32];
	public:
		void t_sync_mem();

		SC_CTOR(sync_mem) {
			SC_CTHREAD(t_sync_mem, clk.pos());
			watching(rst.delayed());
		}
};

#endif
