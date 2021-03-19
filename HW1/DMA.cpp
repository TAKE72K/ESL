
#include "DMA.h"

void dma::dma_master(){
	//reg init
	BASE=0x2021;
	SOURCE=0x0;
	TARGET=0x0;
	SIZE=0x0;
	START=0;
	//port init
	interrupt.write(0);
	printf("dma reset\n");
	
	while(1){
		wait();
		if(interrupt==0){
			if(START==0){
				//slave port recieve control data
			if(addr_s.read()==0x2021) {SOURCE=wdata_s.read();printf("write SOURCE success");}
			if(addr_s.read()==BASE+0x4) TARGET=wdata_s.read();
			if(addr_s.read()==BASE+0x8) SIZE=wdata_s.read();
			if(addr_s.read()==BASE+0xC) START=wdata_s.read();
					
				
			}
			else{
				for(int i=0;i<SIZE;i++){
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
