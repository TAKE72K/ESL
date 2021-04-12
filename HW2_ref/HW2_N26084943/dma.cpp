#include "dma.h"

/////////////////   DMA is slave   ///////////////
void DMA::b_transport( tlm::tlm_generic_payload& trans, sc_time& delay ){	
		
    tlm::tlm_command cmd_s = trans.get_command();
    sc_dt::uint64 addr_s = trans.get_address();
    unsigned char* data_s = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();

    if (cmd_s ==tlm::TLM_WRITE_COMMAND) {

        addr = addr_s - DMA_baseaddr.read();
		    
        data1 = (reinterpret_cast<unsigned int*>(data_s));
        data2 = *(reinterpret_cast<int*>(data_s));
        s_rw = 0;
    } else {
        s_rw = 1;
    }

    wait(10,SC_NS);		
    trans.set_response_status( tlm::TLM_OK_RESPONSE);
		
}

void DMA::dma() {
    R_SOURCE = 0;
    R_TARGET = 0;
    R_SIZE = 0;
    R_START = 0;
    temp2 = 0;
    temp_s = 0;
    temp_t = 0;

    //R_data = 0;
    while (1) {
        wait();
        tlm::tlm_generic_payload* trans_m = new tlm::tlm_generic_payload;
        sc_time delay = sc_time(10,SC_NS);

        if (s_rw == 0 && R_START == 0) {
            // write control register
            if (addr == 0x0) {
                // R_SOURCE = *data1;
                R_SOURCE = data2;
	        } else if (addr == 0x4) {
                // R_TARGET = *data1;
                R_TARGET = data2;
	        } else if (addr==0x8) {
                // R_SIZE = *data1;
                R_SIZE = data2;
            } else if (addr == 0xc) {
                temp1 = R_SIZE;
                // || interrupt
                /*if (temp1 %4 != 0 || interrupt) {
                    //R_START.write(0);
					R_START = 0;
                } else {
                    //R_START = *data1;
                    R_START = data2;
                }*/
                if(temp1 <= 0 || interrupt){
                    R_START = 0;
                }
                else{
                    R_START = data2;
                }
            }
        }

        if (R_START == 1 && R_SIZE != 0) {
            temp2 = R_SIZE;
            temp_s = R_SOURCE;
            temp_t = R_TARGET;
            cout << "R_SIZE : " << temp2 <<endl;
            cout << "R_SOURCE : " << hex << temp_s <<endl;
            cout << "R_TARGET : " << hex << temp_t <<endl << endl;

            for (int i = temp2; i > 0; i = i - 4) {
                int count = 0;
                while (count < 2) {
                    tlm::tlm_command cmd_m;
                    if (count == 0) {
                        cmd_m = tlm::TLM_READ_COMMAND;
                        m_rw = 0;
                        cout << "--- DMA read data from source memory ! --- "
                             << endl;
                        trans_m->set_command( cmd_m );
                        trans_m->set_address( temp_s );
                        trans_m->set_data_ptr(
                            reinterpret_cast<unsigned char*>(&data_m) );
                        trans_m->set_data_length(4);
                        trans_m->set_streaming_width(4);
                        trans_m->set_byte_enable_ptr(0);
                        trans_m->set_dmi_allowed(false);
                        trans_m->set_response_status(
                            tlm::TLM_INCOMPLETE_RESPONSE );
                        socket_m->b_transport( *trans_m, delay );
                    } else if (count == 1) {
                        cmd_m = tlm::TLM_WRITE_COMMAND;
                        m_rw = 1;
                        /* Deal with size is smaller than 4 */
                        if(temp2 == 1){
                            data_m = data_m & 0x000000ff;
                        }
                        else if(temp2 == 2){
                            data_m = data_m & 0x0000ffff;
                        }
                        else if(temp2 == 3){
                            data_m = data_m & 0x00ffffff;
                        }
                        else {
                            data_m = data_m;
                        }
                        //=================//
                        cout << "--- DMA write data to target memory ! ---- "
                             << endl;
                        trans_m->set_command( cmd_m );
                        trans_m->set_address( temp_t );
                        trans_m->set_data_ptr(
                            reinterpret_cast<unsigned char*>(&data_m) );
                        trans_m->set_data_length(4);
                        trans_m->set_streaming_width(4);
                        trans_m->set_byte_enable_ptr(0);
                        trans_m->set_dmi_allowed(false);
                        trans_m->set_response_status(
                            tlm::TLM_INCOMPLETE_RESPONSE );
                        socket_m->b_transport( *trans_m, delay );
                    }
                    count++;
                    R_data = *(reinterpret_cast<int*>(&data_m));                    
                    cout << "addr_source = " << temp_s
                         << ",   addr_target = " << temp_t;
                    cout << ",   data = " << setw(8) << setfill('0') << *(reinterpret_cast<int*>(&data_m))
                         << endl << endl;
                }
                temp_s = temp_s + 4;
                temp_t = temp_t + 4;
                temp2 = temp2 - 4;
            }
            temp2 = 0;
            R_START=0;
            m_rw = 0;
            interrupt.write(1);
            //wait();
        }

        if(int_clear.read()==1){

            interrupt.write(0);
            //wait();
        }

        //wait();
    }
}