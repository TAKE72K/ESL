#ifndef DMA_H
#define DMA_H

#include <stdio.h>
#include <iostream>

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm_utils;


SC_MODULE(DMA){
	//public
	sc_in<bool> reset,clk;
	sc_out<bool> Interrupt;
	
	simple_target_socket<DMA> slave_p;
	simple_initiator_socket<DMA> master_p;
	sc_uint<32> SOURCE,
				TARGET,
				SIZE;
	bool 		START_CLEAR;
	
	//protect
	sc_signal<sc_uint<32> > BASE;
	int offset;
	unsigned int data,data_m;
	unsigned int* data_ptr;
	
	
	void dma_p();
	void b_transport(tlm::tlm_generic_payload&, sc_time&);
	
	SC_CTOR(DMA):master_p("master_p"), slave_p("slave_p"){
		slave_p.register_b_transport(this, &DMA::b_transport);
		SC_CTHREAD(dma_p, clk.pos());
		reset_signal_is(reset,true);
		BASE=0x63000000;
	}
};










#endif
