#ifndef MAIN
#define MAIN

#include "mem.h"
#include "dma.h"
#include "cpu.h"


int sc_main(int argc,char* argv[]){
	sc_time clkprd(10,SC_NS),clkDly(10,SC_NS);
	sc_signal<bool> interrupt,int_clear,rst;
	sc_clock clk("clk",clkprd,0.50,clkDly,true);

	cpu *cpu1 = new cpu("cpu1");
	cpu1->clk(clk);
	cpu1->interrupt(interrupt);
	cpu1->int_clear(int_clear);
	cpu1->rst(rst);

	DMA *DMA1 = new DMA("DMA1");
	DMA1->clk(clk);
	DMA1->interrupt(interrupt);
	DMA1->int_clear(int_clear);
	DMA1->rst(rst);

	mem *mem1 = new mem("mem1");
	mem1->clk(clk);

	cpu1->socket_m.bind( DMA1->socket_s );
	DMA1->socket_m.bind( mem1->socket_s );

	sc_trace_file *tf = sc_create_vcd_trace_file("RESULT");
	sc_trace(tf, clk, "clock");
	sc_trace(tf, interrupt, "Interrupt");
	sc_trace(tf, int_clear, "Clear");

	sc_trace(tf, DMA1->temp_s, "SOURCE");
	sc_trace(tf, DMA1->temp_t, "TARGET");
	sc_trace(tf, DMA1->temp2, "SIZE");
	sc_trace(tf, DMA1->R_START, "START");
	sc_trace(tf, DMA1->m_rw, "Master_rw");
	sc_trace(tf, DMA1->data_m, "Mem_Data");

	sc_start(clkprd*20);


	sc_close_vcd_trace_file(tf);
  	return 0;

}

#endif
