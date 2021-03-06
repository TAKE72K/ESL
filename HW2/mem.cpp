#include "mem.h"

void MEM::b_transport(tlm::tlm_generic_payload& payload,sc_time& delay){
	tlm::tlm_command cmd_s = payload.get_command();
	uint64 addr=payload.get_address();
	unsigned char* data = payload.get_data_ptr();
	unsigned int len = payload.get_data_length();
	if(cmd_s==tlm::TLM_READ_COMMAND){
		switch(addr){
			case 0x2020:
				ram[addr]=0x20212021;
				memcpy(data, &ram[addr], len);
			break;
			case 0x2024:
				ram[addr]=0x44444444;
				memcpy(data, &ram[addr], len);
			break;
			case 0x2028:
				ram[addr]=0x10242048;
				memcpy(data, &ram[addr], len);
			break;
			case 0x202c:
				ram[addr]=0xcccccccc;
				memcpy(data, &ram[addr], len);
			break;
			default:
				ram[addr]=0x00000000;
				memcpy(data, &ram[addr], len);
			break;
		}
		
	}
	else if(cmd_s==tlm::TLM_WRITE_COMMAND){

		if(len==4) ram[addr]=*(reinterpret_cast<int*>(data));
		if(len==3) ram[addr]=(ram[addr]&0xFF000000)+*(reinterpret_cast<int*>(data));
		if(len==2) ram[addr]=(ram[addr]&0xFFFF0000)+*(reinterpret_cast<int*>(data));
		if(len==1) ram[addr]=(ram[addr]&0xFFFFFF00)+*(reinterpret_cast<int*>(data));
	}
	cout<<sc_time_stamp()<<'\n';
	cout<<"Command: "<<((cmd_s)?"READ":"WRITE")<<'\n';
	cout<<"Address: 0x"<<hex<<addr<<'\n';
	cout<<"Data: 0x"<<hex<<ram[addr]<<'\n';
	
	cout<<"----Memory status----\n";
	cout<<"mem[0x2020]="<<hex<<ram[0x2020]<<" mem[0x2024]="<<hex<<ram[0x2024]<<"\nmem[0x2028]="<<hex<<ram[0x2028]<<" mem[0x202c]="<<hex<<ram[0x202c]<<'\n';
	cout<<"mem[0x20]="<<hex<<ram[0x20]<<" mem[0x24]="<<hex<<ram[0x24]<<"\nmem[0x28]="<<hex<<ram[0x28]<<" mem[0x2c]="<<hex<<ram[0x2c]<<'\n';
	wait(10,SC_NS);
}
