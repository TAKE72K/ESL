//main.cpp
#include "DMA.h"
int sc_main(int argc, char* argv[]){
	
	
	
	sc_signal<bool> reset;
	sc_signal<bool> interrupt;
	
	//slave port
	sc_signal<sc_uint<32> >  addr_s, wdata_s;
	sc_signal<sc_uint<32> > rdata_s;
	sc_signal<bool> rw_s;
	//master port
	sc_signal<sc_uint<32> >  wdata_m;
	sc_signal<sc_uint<32> > rdata_m, addr_m;
	sc_signal<bool> rw_m;
	
	sc_time clkPrd(20, SC_NS);
	sc_clock clk("clk", clkPrd, 1, SC_ZERO_TIME, true);
	
	
	dma d1("DMA");
	d1.clk(clk);
	//connect port
	d1.reset(reset);
	d1.interrupt(interrupt);
	
	//slave port
	d1.addr_s(addr_s);
	d1.wdata_s(wdata_s);
	d1.rdata_s(rdata_s);
	d1.rw_s(rw_s);
	
	//master port
	d1.addr_m(addr_m);
	d1.wdata_m(wdata_m);
	d1.rdata_m(rdata_m);
	d1.rw_m(rw_m);
	
	
	sc_trace_file *tf= sc_create_vcd_trace_file("DMA_RESULT");
	sc_trace(tf,reset,"reset");
	sc_trace(tf,interrupt,"interrupt");
	//slv
	sc_trace(tf,addr_s,"addr_s");
	sc_trace(tf,wdata_s,"wdata_s");
	sc_trace(tf,rdata_s,"rdata_s");
	sc_trace(tf,rw_s,"rw_s");
	//mst
	sc_trace(tf,wdata_m,"wdata_m");
	sc_trace(tf,rdata_m,"rdata_m");
	sc_trace(tf,addr_m,"addr_m");
	sc_trace(tf,rw_m,"rw_m");
	//reg
	sc_trace(tf,d1.SOURCE,"SOURCE");
	sc_trace(tf,d1.TARGET,"TARGET");
	sc_trace(tf,d1.BASE,"BASE");
	sc_trace(tf,d1.SIZE,"SIZE");
	sc_trace(tf,d1.START,"START");
	sc_trace(tf,d1.DATA,"DATA_reg");
	
	//simulation
	
	/*
	1. send control data by slave port, init SOURCE, SIZE, TARGET, and START.
	2. move data by master port.
	3 done
	
	*/
	sc_start(0, SC_NS); //clk in nano second scale
	reset.write(0);
	sc_start(clkPrd);
	reset.write(1);
	sc_start(clkPrd*2);
	
	printf("DMA RESET\n");
	
	//write control
	reset.write(0);
	wdata_s.write(0x20000);
	addr_s.write(0x2021);
	sc_start(clkPrd);
	printf("write SOURCE...\n");
	wdata_s.write(0x30000);
	addr_s.write(0x2025);
	sc_start(clkPrd);
	printf("write TRAGET...\n");
	wdata_s.write(0x4);
	addr_s.write(0x2029);
	sc_start(clkPrd);
	printf("write SIZE...\n");
	wdata_s.write(1);
	addr_s.write(0x202D);
	sc_start(clkPrd);
	printf("write START...\n");
	
	
	rw_s.write(0);
	wdata_s.write(0x0);
	addr_s.write(0x0);
	rw_m.read();
	
	addr_m.read();
	rdata_m.write(0x2021);
	sc_start(clkPrd);
	printf("read 1st\n");
	addr_m.read();
	wdata_m.read();
	rw_m.read();
	sc_start(clkPrd);
	printf("write 1st\n");
	//1st
	
	addr_m.read();
	rdata_m.write(0x2022);
	sc_start(clkPrd);
	printf("read 2nd\n");
	addr_m.read();
	wdata_m.read();
	rw_m.read();
	sc_start(clkPrd);
	printf("write 2nd\n");
	//2nd

	addr_m.read();
	rdata_m.write(0x2023);
	sc_start(clkPrd);
	printf("read 3rd\n");
	addr_m.read();
	wdata_m.read();
	rw_m.read();
	sc_start(clkPrd);
	printf("write 3rd\n");
	//3rd
	
	addr_m.read();
	rdata_m.write(0x2024);
	sc_start(clkPrd);
	printf("read 4th\n");
	addr_m.read();
	wdata_m.read();
	rw_m.read();
	sc_start(clkPrd);
	printf("write 4th\n");
	//4th

	sc_start(clkPrd*10);

	wdata_s.write(0);
	addr_s.write(0x202D);
	sc_start(clkPrd);
	printf("write CLEAR...\n");
	sc_start(clkPrd*10);
	
	
	wdata_s.write(0xF0000);
	addr_s.write(0x2025);
	sc_start(clkPrd);
	printf("write TRAGET...\n");
	wdata_s.write(1);
	addr_s.write(0x202D);
	sc_start(clkPrd);
	printf("write START...\n");

	//second rnd
	rw_s.write(0);
	wdata_s.write(0x0);
	addr_s.write(0x0);
	rw_m.read();
	
	addr_m.read();
	rdata_m.write(0x2021);
	sc_start(clkPrd);
	printf("read 1st\n");
	addr_m.read();
	wdata_m.read();
	rw_m.read();
	sc_start(clkPrd);
	printf("write 1st\n");
	//1st
	
	addr_m.read();
	rdata_m.write(0x2022);
	sc_start(clkPrd);
	printf("read 2nd\n");
	addr_m.read();
	wdata_m.read();
	rw_m.read();
	sc_start(clkPrd);
	printf("write 2nd\n");
	//2nd
	
	//test reset
	reset.write(1);
	printf("test reset\n");
	sc_start(clkPrd*10);

	sc_close_vcd_trace_file(tf);
	return(0);
}
