

#include "tlm/tlmModule.hpp"
#include "tlm/tlmDecoder.hpp"
#include "tlm/tlmMemory.hpp"
#include "andes.ovpworld.org/processor/riscv/1.0/tlm/processor.igen.hpp"
#include "DMA.h"
#include "Adaptor.h"


using namespace sc_core;

////////////////////////////////////////////////////////////////////////////////
//                      BareMetal Class                                       //
////////////////////////////////////////////////////////////////////////////////

class BareMetal : public sc_module 
{
public:
    BareMetal (sc_module_name name);
    sc_in <bool>          clk;
    sc_in <bool>          rst;

    tlmModule             Platform;
    tlmDecoder            bus1;
    tlmDecoder            bus2;
    tlmRam                ram1;
    tlmRam                ram3;
    tlmRam                ram4;
    tlmRam                ram5;
    riscv                 cpu1;
    DMA*                  dma1;
    Adaptor              adaptor1;

private:
	sc_signal <bool>      interrupt_connect; 
    params paramsForcpu1() 
    {
        params p;
        p.set("defaultsemihost", true);
        return p;
    }

}; /* BareMetal */

BareMetal::BareMetal (sc_module_name name)
    : sc_module (name)
    , Platform ("")
    , bus1 (Platform, "bus1", 3, 5)
    , bus2 (Platform, "bus2", 0, 0)
    , ram1 (Platform, "ram1", 0x000fffff)
    , ram3 (Platform, "ram3", 0x000fffff)
    , ram4 (Platform, "ram4", 0x000fffff)
    , ram5 (Platform, "ram5", 0x2fffffff)
    , cpu1 (Platform, "cpu1",  paramsForcpu1())
    , adaptor1("adaptor1")
{

    dma1 = new DMA("DMA1");
    dma1 -> clk(clk);

    dma1 -> interrupt(interrupt_connect);
    dma1 -> rst(rst);
    adaptor1.clk(clk);
    adaptor1.interrupt_in(interrupt_connect);
    adaptor1.interrupt_out(cpu1.MExternalInterrupt);

    cpu1.INSTRUCTION.socket(bus1.target_socket[0]);
    cpu1.DATA.socket(bus1.target_socket[1]);
    dma1->master_p(bus1.target_socket[2]);

	
    bus1.initiator_socket[0](ram1.sp1);
    bus1.initiator_socket[1](ram3.sp1);
    bus1.initiator_socket[2](dma1->slave_p);
    bus1.initiator_socket[3](ram5.sp1);
    bus1.initiator_socket[4](ram4.sp1);	
	
    bus1.setDecode(0,0x00000000,0x000fffff);	//ram1 range
    bus1.setDecode(1,0x00200000,0x002fffff);	//ram3 range
    bus1.setDecode(2,0x00100000,0x0010001f);	//dma range
    bus1.setDecode(3,0xd0000000,0xffffffff);	//ram5 dummy
    bus1.setDecode(4,0x00300000,0x003fffff);	//ram4 range
    	
/*
    bus1.connect(cpu1.INSTRUCTION);
    bus1.connect(cpu1.DATA);
    bus1.connect(ram1.sp1, 0x0, 0xffffffff);
*/
}

int sc_main (int argc, char *argv[]) {

    sc_time clkprd(10,SC_NS),clkDly(0,SC_NS);
    sc_clock clk("clk",clkprd,0.50,clkDly,false);
    sc_signal<bool> rst;
    sc_signal<bool> interrupt;

    // start the CpuManager session
    session s;

    // create a standard command parser and parse the command line
    parser  p(argc, (const char**) argv);

    // create an instance of the platform
    BareMetal top ("top");

    top.clk(clk);
    //top.rst(rst);
    top.rst(rst);
    //top.interrupt(interrupt);
    //rst=true;

    // start SystemC
    sc_start();

    return 0;
}

