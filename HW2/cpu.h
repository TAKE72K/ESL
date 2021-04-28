#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <iostream>

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm_utils;


SC_MODULE(CPU){
	sc_in<bool>clk;
	sc_in<bool> Interrupt;
	sc_out<bool>reset;


	simple_initiator_socket<CPU> master_p;
	unsigned int data;

	void cpu_p();

	SC_CTOR(CPU) : master_p("master_p"){
		SC_CTHREAD(cpu_p,clk.pos());
		reset_signal_is(reset,true);
	}
};

#endif