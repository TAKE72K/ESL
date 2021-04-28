#include "mem.h"

void MEM::b_transport(tlm::tlm_generic_payload& payload,sc_time& delay){
	tlm::tlm_command cmd_s = payload.get_command();
	uint64 addr=payload.get_address();
	unsigned char* data = payload.get_data_ptr();
	unsigned int len = payload.get_data_length();
	if(cmd_s==tlm::TLM_READ_COMMAND){
		switch(addr){
			case 0x0:
				ram[addr]=0x20212021;
				memcpy(data, &ram[addr], len);
			break;
			case 0x4:
				ram[addr]=0x44444444;
				memcpy(data, &ram[addr], len);
			break;
			case 0x8:
				ram[addr]=0x98989898;
				memcpy(data, &ram[addr], len);
			break;
			case 0xc:
				ram[addr]=0xcccccccc;
				memcpy(data, &ram[addr], len);
			break;
			default:
				ram[addr]=0xffffffff;
				memcpy(data, &ram[addr], len);
			break;
		}
		
	}
	else if(cmd_s==TLM_WRITE_COMMAND){
		ram[addr]=*(reinterpret_cast<int*>(data));
	}
	cout<<sc_time_stamp()<<'\n';
	cout<<"command is"<<(cmd_s)?"r":"w")<<'\n';
	printf("Address: %#010x\n",addr);
	printf("Value: %#010x\n",data);
	wait(10,SC_NS);
}