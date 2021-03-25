
#include "DMA.h"

void dma::dma_master(){
	//reg init
	BASE=0x2000;
	SOURCE=0x0;
	TARGET=0x0;
	SIZE=0x0;
	DATA=0x0;
	START=0;
	//port init
	interrupt.write(0);
	rw_m.write(0);
	addr_m.write(0);
	wdata_m.write(0);
	
	printf("dma reset\n");
	
	while(1){
		wait();
		if(interrupt==0){
			if(START==0){
				//slave port recieve control data
			if(addr_s.read()-BASE==0) {SOURCE=wdata_s.read();printf("write SOURCE success");}
			if(addr_s.read()-BASE==0x4) TARGET=wdata_s.read();
			if(addr_s.read()-BASE==0x8) SIZE=wdata_s.read();
			if(addr_s.read()-BASE==0xC) START=wdata_s.read();
					
				
			}
			else{
				while(SIZE!=0){
					if(SIZE>=4){
						SIZE-=4;
						rw_m.write(0);
						addr_m.write(SOURCE);
						SOURCE+=4;
						DATA=rdata_m.read();
						wait();
						rw_m.write(1);
						addr_m.write(TARGET);
						TARGET+=4;
						wdata_m.write(DATA);
						wait();
					}
					else if(SIZE==3){
						SIZE-=3;
						rw_m.write(0);
						addr_m.write(SOURCE);
						SOURCE+=4;
						DATA=rdata_m.read().range(23,0);
						wait();
						rw_m.write(1);
						addr_m.write(TARGET);
						TARGET+=4;
						wdata_m.write(DATA);
						wait();
					}
					else if(SIZE==2){
						SIZE-=2;
						rw_m.write(0);
						addr_m.write(SOURCE);
						SOURCE+=4;
						DATA=rdata_m.read().range(15,0);
						wait();
						rw_m.write(1);
						addr_m.write(TARGET);
						TARGET+=4;
						wdata_m.write(DATA);
						wait();
					}
					else if(SIZE==1){
						SIZE-=1;
						rw_m.write(0);
						addr_m.write(SOURCE);
						SOURCE+=4;
						DATA=rdata_m.read().range(7,0);
						wait();
						rw_m.write(1);
						addr_m.write(TARGET);
						TARGET+=4;
						wdata_m.write(DATA);
						wait();
					}
				}
				for(int i=0;i<((SIZE%4==0)?(SIZE/4):(SIZE/4+1));i++){
					rw_m.write(0);
					addr_m.write(SOURCE+(i<<2));
					DATA=rdata_m.read();
					wait();
					
					rw_m.write(1);
					addr_m.write(TARGET+(i<<2));
					wdata_m.write(DATA);
					wait();
					
				}
				rw_m.write(0);
				wdata_m.write(0x0);
				addr_m.write(0x0);
				interrupt.write(1);
			}
		}
		else{//INTERRUPT==1
			if(addr_s.read()==BASE+0xC){//GET CLEAR
				START=wdata_s.read();
				if(START==0)interrupt.write(0);
				}
		}
	}
		
}
