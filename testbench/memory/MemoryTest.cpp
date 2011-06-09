/*
 *  memorytest.cpp
 *  projeto
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MemoryTest.h"
#include <iostream>

using namespace std;

void MemoryTest::do_test() {
	
	//clk=0; //clock 
	rst  = 0; //reset
	datai    = 0; //sem entrada
	//datao    = 0; // sem saida
	address = 0; //sem adress
	writer  = 0; //sem escrita
	//clk=0;
	wait(2); 
	cout << "@" << sc_time_stamp() <<endl;
	
	//clk=1; //mudança de clock
	writer = 1; // habilita escrita
	address=(0x10); //escreve o endereco
	datai=(1); // escreve a entra
	rst  = 1; // habilita o reset
	
	wait(2);
	cout << "@" << sc_time_stamp() <<endl;
	
	writer = 0; // desabilita o writer
	rst  = 0; //desabilita o reset
	//clk=0; //muda o clock
	address=(0x10); //escreve o endereco
	
	wait(2);	
	cout << "@" << sc_time_stamp() <<endl;
	
	sc_stop();
}