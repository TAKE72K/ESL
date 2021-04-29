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
		is_data_wrote=false;
	}
	//else{//read op
		
	//}
	wait(10,SC_NS);
	trans.set_response_status(tlm::TLM_OK_RESPONSE);

}
void DMA::dma_p(){
	START_CLEAR=0;
	SOURCE=0;
	TARGET=0;
	SIZE=0;
	data_m=0;
	is_data_wrote=true;

	Interrupt.write(0);
	while(1){//main dma process
		wait();
		tlm::tlm_generic_payload* trans_m = new tlm::tlm_generic_payload;
		sc_time delay = sc_time(10,SC_NS);
		if(Interrupt.read()==1){
			if(offset==12){
				START_CLEAR=data;
			}
			if(START_CLEAR==0){
				Interrupt.write(0);
			}
		}
		if(START_CLEAR==0){
			//cout<<offset<<"\n";
			is_data_wrote=true;
			switch(offset){
				case 0x0:
					SOURCE=data;
					//cout<<"Write SOURCE:%#010x\n",SOURCE);
					//cout<<hex<<SOURCE;
					break;
				case 0x4:
					TARGET=data;
					//printf("Write TARGET:%#010x\n",TARGET);
					break;
				case 0x8:
					SIZE=data;
					//printf("Write SIZE:%d\n",SIZE);
					break;
				case 0xc:
					START_CLEAR=data;
					//printf("Write START\n");
					break;
			}
		}//if end
		if(START_CLEAR==1&&Interrupt.read()==0){
			cout<<"DMA start to work\n";
			cout<<"SOURCE: 0x"<<hex<<SOURCE<<'\n';
			cout<<"TARGET: 0x"<<hex<<TARGET<<'\n';
			cout<<"SIZE:"<<SIZE<<'\n';
			
			int size_iter=SIZE;
			int addr_s=SOURCE;
			int addr_t=TARGET;
			
			while(size_iter>0){//moving data
				cout<<"--------------DMA read data---------------\n";
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
				
				cout<<"source address:"<<hex<<addr_s<<",  target address"<<hex<<addr_t<<",  data:"<<hex<<data_m<<'\n';
				
				tlm::tlm_command cmd_mw;
				cmd_mw=tlm::TLM_WRITE_COMMAND;
				//mask data

				if(size_iter==1) data_m=data_m&0x000000FF;
				else if(size_iter==2) data_m=data_m&0x0000FFFF;
				else if(size_iter==3) data_m=data_m&0x00FFFFFF;
				
				cout<<"--------------DMA write data---------------\n";
				int l=(size_iter>4)?4:(size_iter%4==0)?4:(size_iter%4);
				trans_m->set_command(cmd_mw);
				trans_m->set_address(addr_t);
				trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data_m));
				trans_m->set_data_length(l);
				trans_m->set_streaming_width(4);
				trans_m->set_byte_enable_ptr(0);
				trans_m->set_dmi_allowed(false);
				trans_m->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
				master_p->b_transport(*trans_m,delay);
				cout<<"source address:"<<hex<<addr_s<<",  target address"<<hex<<addr_t<<",  data:"<<hex<<data_m<<'\n';
				
				//iter
				size_iter-=4;
				addr_s+=4;
				addr_t+=4;
			}//moving data loop end
			
			Interrupt.write(1);
		}

	}//process end
}
