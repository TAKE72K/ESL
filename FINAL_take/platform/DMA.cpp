#include "DMA.h"

void DMA::b_transport(tlm::tlm_generic_payload& trans, sc_time& delay1){
	tlm::tlm_command cmd_s = trans.get_command();
	sc_dt::uint64 addr_s = trans.get_address();
	unsigned char* data_s = trans.get_data_ptr();
	//unsigned int len = trans.get_data_length();
	if(cmd_s == tlm::TLM_WRITE_COMMAND){
		switch(addr_s-base_addr)//offset
		{
			case 0x0:
				source = (*reinterpret_cast<int*>(data_s));
				break;
			case 0x4:
				target = (*reinterpret_cast<int*>(data_s));
				break;
			case 0x8:
				size   = (*reinterpret_cast<int*>(data_s));
				break;
			case 0xC:
				start  = ((*reinterpret_cast<int*>(data_s))==1)?1:0;

		}
	}
	wait(delay1);
	trans.set_response_status(tlm::TLM_OK_RESPONSE);
}

void DMA::dma_m(){
	interrupt = 0;
	while(1){
		wait();
		tlm::tlm_generic_payload* trans_m = new tlm::tlm_generic_payload;
		tlm::tlm_command cmd_m;
		sc_time delay = sc_time(10,SC_NS);
		if (rst) {
			cout << "=============Reset=============" << endl;
			source = 0;
			target = 0;
			size = 0;
			start = 0;
			interrupt = false;
		}
		if(interrupt){
			if(start==0){
				interrupt=0;
				cout<<"DMA reset";
			}
			
		}
		if(start == 1&&interrupt==0){
			cout << "size : " << hex << size << endl;
			cout << "source : " << hex << source << endl;
			cout << "target : " << hex << target << endl;
			if(state == 0){
				cout << "DMA read data for source address" << endl;
				cmd_m = tlm::TLM_READ_COMMAND;
				trans_m->set_command(cmd_m);
				trans_m->set_address(source);
				trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data_s));
				trans_m->set_data_length(4);
				trans_m->set_streaming_width(4);
				trans_m->set_byte_enable_ptr(0);
				trans_m->set_dmi_allowed(false);
				trans_m->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
				master_p->b_transport(*trans_m, delay);
				state = 1;
			}
			else if(state == 1){
             		   	cout << "DMA write data to target address" << endl;
				cmd_m = tlm::TLM_WRITE_COMMAND;
                		trans_m->set_command(cmd_m);
                		trans_m->set_address(target);
				temp_t = data_s;
               		 	trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&temp_t) );
				//cout << "temp_t = " << temp_t << endl;
				data_c[0]=temp_t(7,0);
				data_c[1]=temp_t(15,8);
				data_c[2]=temp_t(23,16);
				data_c[3]=temp_t(31,24);
				printf(" and the write data is %c %c %c %c \n",data_c[0],data_c[1],data_c[2],data_c[3]);
               		 	trans_m->set_streaming_width(4);
                		trans_m->set_byte_enable_ptr(0);
                		trans_m->set_dmi_allowed(false);
				if(size ==1) {
					//temp_t = data_s;
					trans_m->set_data_length(1);
				}
				else if(size == 2){
					//temp_t = data_s;
					trans_m->set_data_length(2);
				}
				else if(size == 3){
					//temp_t = data_s;
					trans_m->set_data_length(3);
				}
				else{
					//temp_t = data_s;
					trans_m->set_data_length(4);
				}
                		trans_m->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
                		master_p->b_transport(*trans_m, delay);
				if(size > 4){
					source = source + 4;
					target = target + 4;
					size = size - 4;
					state = 0;
				}
				else{
					source = 0;
					target = 0;
					size = 0;
					//start = 0;
					state = 1;
					cout << "DMA send interrupt " << endl;
					interrupt = 1;
					wait(1);
					cout <<"interrupt = " << interrupt <<endl;
					//interrupt = 0;
					//wait(1);
					//cout << "interrupt = " << interrupt <<endl;
				}
			}

		}	
		
	}		
}	
	


