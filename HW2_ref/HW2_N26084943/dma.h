#ifndef DMA_H
#define DMA_H

#include <iomanip>
#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

SC_MODULE(DMA){
    sc_in<bool>clk;	
    sc_out<bool> interrupt;
    sc_in<bool> int_clear;
    sc_in<bool> rst;

    bool s_rw;
    bool m_rw; // to trace DMA read_write (read : 0, write : 1)

    tlm_utils::simple_target_socket<DMA> socket_s;
    tlm_utils::simple_initiator_socket<DMA> socket_m;
    //unsigned char data_m;
    unsigned int data_m;
    unsigned int reg_m;

    // Control registers, externally addressable, addressed as:
    // 0x0  R_SOURCE
    // 0x4  R_TARGET
    // 0x8  R_SIZE
    // 0xc  R_START
    sc_uint<32> R_SOURCE,
                R_TARGET,
                R_SIZE;
    bool        R_START;

    // Internal registers, not externally addressable
    sc_signal<sc_uint<32> > DMA_baseaddr;
    sc_uint<32>             temp1, temp2, temp_s, temp_t;
    sc_uint<32>             n1, n2;
    unsigned char           R_data;
    sc_uint<32>             ctrl_addr;
    unsigned int*           data1;		
    unsigned int            data2;			
    unsigned int            addr, addr2;	

    tlm::tlm_command cmd_s;
    uint64 addr_s;
    unsigned char data_s;

    void dma();
    void b_transport(tlm::tlm_generic_payload&, sc_time&);

    SC_CTOR(DMA) : socket_s("socket_s"), socket_m("socket_m") {
        socket_s.register_b_transport(this, &DMA::b_transport);

        SC_CTHREAD(dma,clk.pos());
        reset_signal_is(rst,true);

        DMA_baseaddr = 0xa0000000;
        temp1=0;
        temp2=0;
    }
};

#endif
