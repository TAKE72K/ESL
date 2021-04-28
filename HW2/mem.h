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

	tlm_utils::simple_target_socket<MEM> socket_s;
	unsigned int ram[0x10000];

	void b_transport(tlm::tlm_generic_payload&, sc_time&);
	
	SC_CTOR(mem) : socket_s("socket_s"){
		socket_s.register_b_transport(this, &mem::b_transport);
	}
}





#endif