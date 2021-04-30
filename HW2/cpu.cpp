#include "cpu.h"


void CPU::cpu_p(){
	tlm::tlm_generic_payload* trans_m = new tlm::tlm_generic_payload;
	sc_time delay = SC_ZERO_TIME;
	
	tlm::tlm_command cmd_mw=tlm::TLM_WRITE_COMMAND;
	tlm::tlm_command cmd_mr=tlm::TLM_READ_COMMAND;
	wait(10,SC_NS);	


	cout<<"Write control data\n"<<sc_time_stamp()<<'\n';
	
	data = 0x2020;
	trans_m->set_command(cmd_mw);
	trans_m->set_address(0x63000000);
	trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
	trans_m->set_data_length(4);//byte
	master_p->b_transport(*trans_m,delay);
	cout<<sc_time_stamp()<<" Write SOURCE:"<<hex<<data<<'\n';
	
	data = 0x20;
	trans_m->set_command(cmd_mw);
	trans_m->set_address(0x63000004);
	trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
	trans_m->set_data_length(4);//byte
	master_p->b_transport(*trans_m,delay);
	cout<<sc_time_stamp()<<" Write TARGET:"<<hex<<data<<'\n';
	
	data = 0x10;
	trans_m->set_command(cmd_mw);
	trans_m->set_address(0x63000008);
	trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
	trans_m->set_data_length(4);//byte
	master_p->b_transport(*trans_m,delay);
	cout<<sc_time_stamp()<<" Write SIZE:"<<data<<'\n';
	
	data = 0x1;
	trans_m->set_command(cmd_mw);
	trans_m->set_address(0x6300000c);
	trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
	trans_m->set_data_length(4);//byte
	master_p->b_transport(*trans_m,delay);
	cout<<sc_time_stamp()<<" Write START:"<<data<<'\n';
	
	trans_m->set_command(cmd_mr);
	master_p->b_transport(*trans_m,delay);
	while(1){
		wait();
		if(Interrupt==1){
			wait(30,SC_NS);
			data = 0x0;
			trans_m->set_command(cmd_mw);
			trans_m->set_address(0x6300000c);
			trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
			trans_m->set_data_length(4);//byte
			master_p->b_transport(*trans_m,delay);
			cout<<sc_time_stamp()<<" Write CLEAR:"<<data<<'\n';
			break;
		}
	}

	cout<<"Perform an odd size move\n";

		cout<<"Write control data\n"<<sc_time_stamp()<<'\n';
	
	data = 0x2020;
	trans_m->set_command(cmd_mw);
	trans_m->set_address(0x63000000);
	trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
	trans_m->set_data_length(4);//byte
	master_p->b_transport(*trans_m,delay);
	cout<<sc_time_stamp()<<" Write SOURCE:"<<hex<<data<<'\n';
	
	data = 0x24;
	trans_m->set_command(cmd_mw);
	trans_m->set_address(0x63000004);
	trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
	trans_m->set_data_length(4);//byte
	master_p->b_transport(*trans_m,delay);
	cout<<sc_time_stamp()<<" Write TARGET:"<<hex<<data<<'\n';
	
	data = 0x7;
	trans_m->set_command(cmd_mw);
	trans_m->set_address(0x63000008);
	trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
	trans_m->set_data_length(4);//byte
	master_p->b_transport(*trans_m,delay);
	cout<<sc_time_stamp()<<" Write SIZE:"<<data<<'\n';
	
	data = 0x1;
	trans_m->set_command(cmd_mw);
	trans_m->set_address(0x6300000c);
	trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
	trans_m->set_data_length(4);//byte
	master_p->b_transport(*trans_m,delay);
	cout<<sc_time_stamp()<<" Write START:"<<data<<'\n';
	
	trans_m->set_command(cmd_mr);
	master_p->b_transport(*trans_m,delay);
	
	
		while(1){
		wait();
		if(Interrupt==1){
			wait(30,SC_NS);
			data = 0x0;
			trans_m->set_command(cmd_mw);
			trans_m->set_address(0x6300000c);
			trans_m->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
			trans_m->set_data_length(4);//byte
			master_p->b_transport(*trans_m,delay);
			cout<<sc_time_stamp()<<" Write CLEAR:"<<data<<'\n';
			break;
		}
	}
}
