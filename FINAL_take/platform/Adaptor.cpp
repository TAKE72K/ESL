#include"Adaptor.h"

void Adaptor::Process(){
	while(1){
	wait();

	  if(interrupt_in.read() == 1){
		interrupt_out.write(1);
		// cout <<"Adaptor interrupt_in  & interrupt_out & = "<<interrupt_in<< endl;
	  }
      else{
		interrupt_out.write(0);
	  }	
	}  
};
