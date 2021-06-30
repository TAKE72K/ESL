#ifndef DMAC
#define DMAC

#include "systemc.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "stdio.h"

	SC_MODULE(DMA) {
		sc_in<bool> clk;
		sc_out<bool> interrupt;
		sc_in<bool> rst;		
		tlm_utils::simple_initiator_socket<DMA> master_p;
		tlm_utils::simple_target_socket<DMA> slave_p;
		unsigned int data_s;//,temp_t;
		char *data_c;
		sc_uint<32> temp_t;
		sc_int<2> state;
		sc_uint<32> base_addr;
		sc_uint<32> source,target,size;
		sc_uint<1> start;

		
		void dma_m();
		void b_transport(tlm::tlm_generic_payload&, sc_time&);
		
		SC_CTOR(DMA){
			data_c=new char[4];
			slave_p.register_b_transport(this, &DMA::b_transport);
			SC_CTHREAD(dma_m, clk.pos());
			
			base_addr = 0;
			source = 0;
			target = 0;
			size = 0;
			start = 0;
			state = 0;
		}
	};

#endif
