/*
 *
 * Copyright (c) 2005-2019 Imperas Software Ltd., www.imperas.com
 *
 * The contents of this file are provided under the Software License
 * Agreement that you accepted before downloading this file.
 *
 * This source forms part of the Software and can be used for educational,
 * training, and demonstration purposes but cannot be used for derivative
 * works except in cases where the derivative works require OVP technology
 * to run.
 *
 * For open source models released under licenses that you can use for
 * derivative works, please visit www.OVPworld.org or www.imperas.com
 * for the location of the open source models.
 *
 */

#include "ovpworld.org/modelSupport/tlmModule/1.0/tlm/tlmModule.hpp"
#include "ovpworld.org/modelSupport/tlmDecoder/1.0/tlm/tlmDecoder.hpp"
#include "ovpworld.org/memory/ram/1.0/tlm/tlmMemory.hpp"
//#include "ovpworld.org/processor/or1k/1.0/tlm/processor.igen.hpp"
#include "andes.ovpworld.org/processor/riscv/1.0/tlm/processor.igen.hpp"
#include "DMA.h"
#include "Adaptor.h"

// alternative processors:
// #include "arm.ovpworld.org/processor/arm/1.0/tlm/processor.igen.hpp"
// #include "mips.ovpworld.org/processor/mips32/1.0/tlm/processor.igen.hpp"

using namespace sc_core;

////////////////////////////////////////////////////////////////////////////////
//                      BareMetal Class                                       //
////////////////////////////////////////////////////////////////////////////////

class BareMetal : public sc_module {

public:
    BareMetal (sc_module_name name);
    sc_in <bool>          clk;
    //sc_in <bool>          rst;
    sc_in <bool>          clean;
    tlmModule             Platform;
    tlmDecoder            bus1;
    tlmRam                ram1;
    tlmRam                ram2;
    riscv                 cpu1;
    DMA*                  dma1;
    Adaptor              adaptor1;

    // alternative processors:
    // arm                   cpu1;
    // mips32                cpu1;

private:
	sc_signal <bool>      interrupt_connect; 
    params paramsForcpu1() {
        params p;
        p.set("defaultsemihost", true);
        return p;
    }

}; /* BareMetal */

BareMetal::BareMetal (sc_module_name name)
    : sc_module (name)
    , Platform ("")
    , bus1 (Platform, "bus1", 3, 3)
    , ram1 (Platform, "ram1", 0x2fffffff)
    , ram2 (Platform, "ram2", 0x2fffffff)
    , cpu1 (Platform, "cpu1",  paramsForcpu1())
    , adaptor1("adaptor1")
{

    dma1 = new DMA("DMA1");
    dma1 -> clk(clk);
    dma1 -> clean(clean);
    dma1 -> interrupt(interrupt_connect);
    //dma1 -> rst(rst);
    adaptor1.clk(clk);
    adaptor1.interrupt_in(interrupt_connect);
    adaptor1.interrupt_out(cpu1.MExternalInterrupt);

    cpu1.INSTRUCTION.socket(bus1.target_socket[0]);
    cpu1.DATA.socket(bus1.target_socket[1]);
    dma1->master_p(bus1.target_socket[2]);

	
    bus1.initiator_socket[0](ram1.sp1);
    bus1.initiator_socket[1](ram2.sp1);
    bus1.initiator_socket[2](dma1->slave_p);
	
	
    bus1.setDecode(0,0x00000000,0x2fffffff);	//ram1 range
    bus1.setDecode(1,0xd0000000,0xffffffff);	//ram2 range
    bus1.setDecode(2,0xa0000000,0xafffffff);	//dma range

/*
    bus1.connect(cpu1.INSTRUCTION);
    bus1.connect(cpu1.DATA);
    bus1.connect(ram1.sp1, 0x0, 0xffffffff);
*/
}

int sc_main (int argc, char *argv[]) {

    sc_time clkprd(10,SC_NS),clkDly(0,SC_NS);
    sc_clock clk("clk",clkprd,0.50,clkDly,false);
    //sc_signal<bool> rst;
    sc_signal<bool> clean,interrupt;

    // start the CpuManager session
    session s;

    // create a standard command parser and parse the command line
    parser  p(argc, (const char**) argv);

    // create an instance of the platform
    BareMetal top ("top");

    top.clk(clk);
    //top.rst(rst);
    top.clean(clean);
    //top.interrupt(interrupt);
    //rst=true;

    // start SystemC
    sc_start();

    return 0;
}

