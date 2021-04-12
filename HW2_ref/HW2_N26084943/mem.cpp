#include "mem.h"

void mem::b_transport( tlm::tlm_generic_payload& payload, sc_time& delay ){		
		tlm::tlm_command cmd_s = payload.get_command();
	  	sc_dt::uint64 addr_s = payload.get_address();
	  	unsigned char* data_s = payload.get_data_ptr();
	  	unsigned int len = payload.get_data_length();

	  	if ( cmd_s == tlm::TLM_READ_COMMAND && addr_s==0x40){
	  		ram[addr_s] = 0xFFFF1111;
      		memcpy(data_s, &ram[addr_s], len);
	  	}
	  	else if ( cmd_s == tlm::TLM_READ_COMMAND && addr_s==0x44){
	  		ram[addr_s] = 0xFFFF2222;
      		memcpy(data_s, &ram[addr_s], len);
	  	}
	  	else if ( cmd_s == tlm::TLM_READ_COMMAND && addr_s==0x48){
	  		ram[addr_s] = 0xFFFF3333;
      		memcpy(data_s, &ram[addr_s], len);
	  	}
	  	else if ( cmd_s == tlm::TLM_READ_COMMAND && addr_s==0x4c){
	  		ram[addr_s] = 0xFFFF4444;
      		memcpy(data_s, &ram[addr_s], len);
	  	}
	  	else if(cmd_s == tlm::TLM_WRITE_COMMAND){
	  		ram[addr_s] = *(reinterpret_cast<int*>(data_s));
	  	}

	  	cout << sc_time_stamp() << endl;
		cout << "Command : "<< (cmd_s ? "write" : "read") <<endl;
		cout << "Address : 0x" << setw(8) << setfill('0') << hex << addr_s <<endl;
		cout << "Data    : 0x" << setw(8) << setfill('0');
	    cout << hex << *(reinterpret_cast<int*>(data_s)) << endl;

	    cout << endl;
	    cout << "ram[40] = " << 0xFFFF1111 	<< ",  ram[8000] = " << setw(8) << setfill('0') << ram[0x8000] <<endl;
	    cout << "ram[44] = " << 0xFFFF2222  << ",  ram[8004] = " << setw(8) << setfill('0') << ram[0x8004] <<endl;
	    cout << "ram[48] = " << 0xFFFF3333 	<< ",  ram[8008] = " << setw(8) << setfill('0') << ram[0x8008] <<endl;
	    cout << "ram[4c] = " << 0xFFFF4444 	<< ",  ram[800c] = " << setw(8) << setfill('0') << ram[0x800c] <<endl;
		

		wait(10,SC_NS);

	}
