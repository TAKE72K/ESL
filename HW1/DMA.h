#include "systemc.h"

SC_MODULE(dma){
	sc_in<bool> reset;
	sc_out<bool> interrupt;
	
	//slave port
	sc_in<sc_uint<32> >  addr_s, wdata_s;
	sc_out<sc_unit<32> > rdata_s;
	sc_in<bool> rw_s;
	//master port
	sc_in<sc_uint<32> >  wdata_m;
	sc_out<sc_unit<32> > rdata_m, addr_m;
	sc_in<bool> rw_m;
	
	//reg
	sc_uint<32> BASE,SOURCE,TARGET,SIZE,DATA;
	bool START;
	
	void dma_master();
	SC_CTOR(dma){
		SC_THREAD(dma_master,clk.pos());
		reset_signal_is(reset,true);
	}
}