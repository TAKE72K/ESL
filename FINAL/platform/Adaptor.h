#ifndef DMA_ADAPTOR
#define DMA_ADAPTOR

#include "systemc.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;
#include "tlm.h"
#include "/usr/local/systemc/systemc-2.3.2/include/tlm_core/tlm_1/tlm_analysis/tlm_analysis_port.h"
#include "stdio.h"
#include "op/op.hpp"


SC_MODULE(Adaptor){
	sc_in<bool> clk;
	sc_in<bool> interrupt_in;
	tlm::tlm_analysis_port<Uns32> interrupt_out;
	void Process();
	
	SC_CTOR(Adaptor){
		//SC_THREAD(Process);		
		SC_CTHREAD(Process, clk.pos());

	}
};
#endif
