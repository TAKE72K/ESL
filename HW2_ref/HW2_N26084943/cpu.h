#ifndef CPU_H
#define CPU_H

#include <iomanip>
#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"

SC_MODULE(cpu){
	sc_in<bool>clk;
	sc_in<bool> interrupt;
	sc_out<bool>rst;
	sc_out<bool> int_clear;

	tlm_utils::simple_initiator_socket<cpu> socket_m;
	unsigned int data_m;

	void tb();

	SC_CTOR(cpu) : socket_m("socket_m"){		
		SC_CTHREAD(tb,clk.pos());
        reset_signal_is(rst,true);
	}	
};

#endif
