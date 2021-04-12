#include "cpu.h"

void cpu::tb(){
	tlm::tlm_generic_payload* trans_m = new tlm::tlm_generic_payload;
	sc_time delay = SC_ZERO_TIME;
	int_clear = 0;

	wait(10,SC_NS);
	/*rst = 0;
	wait();
	rst = 1;
	wait();*/
	
	tlm::tlm_command cmd_m = tlm::TLM_WRITE_COMMAND;
	cout << "------------------------SOURCE address transaction!  time = " << sc_time_stamp() << endl; 
	data_m = 0x40;
	trans_m->set_command( cmd_m );
	trans_m->set_address( 0xa0000000 );
	trans_m->set_data_ptr( reinterpret_cast<unsigned char*>(&data_m) );
	trans_m->set_data_length( 4 );
	socket_m->b_transport( *trans_m, delay );
		
	cout << "------------------------TARGET address transaction!  time = " << sc_time_stamp() << endl;
	data_m = 0x8000;
	trans_m->set_command( cmd_m );
	trans_m->set_address( 0xa0000004 );
	trans_m->set_data_ptr( reinterpret_cast<unsigned char*>(&data_m) );
	trans_m->set_data_length( 4 );
	socket_m->b_transport( *trans_m, delay );

	cout << "------------------------SIZE transaction!  time = " << sc_time_stamp() << endl;
	data_m = 0x0f; // Move 15 Bytes data
	trans_m->set_command( cmd_m );
	trans_m->set_address( 0xa0000008 );
	trans_m->set_data_ptr( reinterpret_cast<unsigned char*>(&data_m) );
	trans_m->set_data_length( 4 );
	socket_m->b_transport( *trans_m, delay );
	
	cout << "------------------------START transaction!  time = " << sc_time_stamp() << endl;
	data_m = 0x1;
	trans_m->set_command( cmd_m );
	trans_m->set_address( 0xa000000c );
	trans_m->set_data_ptr( reinterpret_cast<unsigned char*>(&data_m) );
	trans_m->set_data_length( 4 );
	socket_m->b_transport( *trans_m, delay );

	cmd_m = tlm::TLM_READ_COMMAND;
	trans_m->set_command( cmd_m );
	socket_m->b_transport( *trans_m, delay );

	while(1){
		wait();
		if(interrupt==1){
			wait(40,SC_NS);
			int_clear = 1;
			wait(10,SC_NS);
			int_clear = 0;
		}
		//wait();
	}

}