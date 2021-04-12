#ifndef MEM_H
#define MEM_H

#include <iomanip>
#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

SC_MODULE(mem){
	sc_in<bool>clk;

	tlm_utils::simple_target_socket<mem> socket_s;
	unsigned int ram[0x10000];

	void b_transport(tlm::tlm_generic_payload&, sc_time&);
	
	SC_CTOR(mem) : socket_s("socket_s"){
		socket_s.register_b_transport(this, &mem::b_transport);
	}
};

#endif