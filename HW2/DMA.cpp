#include "DMA.h"
// create by take72k e24063058



void DMA::b_transport(tlm::tlm_generic_payload& trans, sc_time& delay){
	
	tlm::tlm_command cmd_s=trans.get_command();
	uint64 addr_s=trans.get_address();//64->by def
	unsigned char* data_s = trans.get_data_ptr();//<-handout miss "()", CHAR* BY DEF
	unsigned int len = trans.get_data_length();
	if (cmd_s == tlm::TLM_WRITE_COMMAND){//write op
		data_ptr=(reinterpret_cast<unsigned int*>(data_s));
		data=*(reinterpret_cast<int*>(data_s));
		offset=addr_s-BASE.read();
	}
	//else{//read op
		
	//}

}
void DMA::dma_p(){
	
	while(1){//main dma process
		wait();
		tlm::tlm_generic_payload* trans_m = new tlm::tlm_generic_payload;
		sc_time delay = sc_time(10,SC_NS);
		if(Interrupt==1){
			if(offset==12){
				START_CLEAR=data;
			}
			if(START_CLEAR==0){
				Interrupt.write(0);
			}
		}
		if(START_CLEAR==0){
			switch(offset){
				case 0:
					SOURCE=data;
					printf("Write SOURCE:%#010x\n",SOURCE);
					break;
				case 4:
					TARGET=data;
					printf("Write TARGET:%#010x\n",TARGET);
					break;
				case 8:
					SIZE=data;
					printf("Write SIZE:%d\n",SIZE);
					break;
				case 12:
					START_CLEAR=data;
					printf("Write START\n");
					break;
			}
		}//if end
		if(START_CLEAR==1&&SIZE>0){
			printf("DMA start to work, moving data from %#010x to %#010x, size %d\n",SOURCE,TARGET,SIZE);
			
			int size_iter=SIZE;
			int addr_s=SOURCE;
			int addr_t=TARGET;
			
			while(size_iter>0){//moving data
				tlm::tlm_command cmd_mr;
				cmd_mr = tlm::TLM_READ_COMMAND;
				trans_m->set_command(cmd_mr);
				trans_m->set_address(addr_s);
				trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data_m));
				trans_m->set_data_length(4);
				trans_m->set_streaming_width(4);
				trans_m->set_byte_enable_ptr(0);
				trans_m->set_dmi_allowed(false);
				trans_m->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
				master_p->b_transport( *trans_m, delay );
				
				printf("DMA read from source\n");
				
				tlm::tlm_command cmd_mw;
				cmd_mw=tlm::TLM_WRITE_COMMAND;
				//mask data
				if(size_iter==1) data_m=data_m&0x000000FF;
				else if(size_iter==2) data_m=data_m&0x0000FFFF;
				else if(size_iter==3) data_m=data_m&0x00FFFFFF;
				
				printf("DMA write target\n");
				
				trans_m->set_command(cmd_mw);
				trans_m->set_address(addr_t);
				trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data_m));
				trans_m->set_data_length(4);
				trans_m->set_streaming_width(4);
				trans_m->set_byte_enable_ptr(0);
				trans_m->set_dmi_allowed(false);
				trans_m->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
				master_p->b_transport(*trans_m,delay);
				
				//iter
				size_iter-=4;
				addr_s+=4;
				addr_t+=4;
			}//moving data loop end
			
			Interrupt.write(1);
		}

	}//process end
}