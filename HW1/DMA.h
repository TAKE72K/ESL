#include "systemc.h"

SC_MODULE(dma){
	sc_in<bool> reset;
	sc_out<bool> interrupt;
	
	//slave port
	sc_in<sc_uint<32> >  addr_s, wdata_s;
	sc_out<sc_uint<32> > rdata_s;
	sc_in<bool> rw_s,clk;
	//master port
	sc_in<sc_uint<32> >  rdata_m;
	sc_out<sc_uint<32> > wdata_m, addr_m;
	sc_out<bool> rw_m;
	
	//reg
	sc_uint<32> BASE,SOURCE,TARGET,SIZE,DATA;
	bool START;
	
	void dma_master();
	SC_CTOR(dma){
		SC_CTHREAD(dma_master,clk.pos());
		reset_signal_is(reset,true);
	}
};
