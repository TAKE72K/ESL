

#include "mem.h"
#include "DMA.h"
#include "cpu.h"


int sc_main(int argc,char* argv[]){
	sc_time clkprd(10,SC_NS),clkDly(10,SC_NS);
	sc_signal<bool> Interrupt,reset;
	sc_clock clk("clk",clkprd,0.50,clkDly,true);

//MASTER
	CPU *cpu1 = new CPU("cpu1");
	cpu1->clk(clk);
	cpu1->Interrupt(Interrupt);
	cpu1->reset(reset);
//MASTER
/*
					|
					|
					|
					V
*/
	DMA *DMA1 = new DMA("DMA1");
	DMA1->clk(clk);
	DMA1->Interrupt(Interrupt);
	DMA1->reset(reset);

	MEM *mem1 = new MEM("mem1");
	mem1->clk(clk);

	cpu1->master_p.bind( DMA1->slave_p );
	DMA1->master_p.bind( mem1->slave_p );

	sc_trace_file *tf = sc_create_vcd_trace_file("RESULT");
	sc_trace(tf, clk, "clock");
	sc_trace(tf, Interrupt, "Interrupt");

	sc_trace(tf, DMA1->SOURCE, "SOURCE");
	sc_trace(tf, DMA1->TARGET, "TARGET");
	sc_trace(tf, DMA1->SIZE, "SIZE");
	sc_trace(tf, DMA1->START_CLEAR, "START");
	sc_trace(tf, DMA1->data_m, "Mem_Data");

	sc_start(clkprd*20);


	sc_close_vcd_trace_file(tf);
  	return 0;

}

