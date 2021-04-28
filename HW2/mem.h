#ifndef MEM_H
#define MEM_H

#include <stdio.h>
#include <iostream>

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;
using namespace tlm_utils;

//mem is slave
SC_MODULE(MEM){
	sc_in<bool>clk;

	tlm_utils::simple_target_socket<MEM> slave_p;
	unsigned int ram[0x10000];

	void b_transport(tlm::tlm_generic_payload&, sc_time&);
	
	SC_CTOR(MEM) : slave_p("slave_p"){
		slave_p.register_b_transport(this, &MEM::b_transport);
	}
};





#endif
