/*
 *  RegisterTest.cpp
 *  projetoregistrador
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "RegisterTest.h"
#include <iostream>

using namespace std;

void RegisterTest::do_test() {
	
	rwBit = 1; //reset
	in  = 1; //sem entrada
	sel = 0x02; //sem adress
	
	wait(2); 
	cout << "@" << sc_time_stamp() <<endl;
	
	//clk=1; //mudança de clock
	rwBit = 0; // habilita escrita
	sel=0x02; //escreve o endereco
	
	wait(2);
	cout << "@" << sc_time_stamp() <<endl;
	
	rwBit = 1; // desabilita o writer
	sel  = 0x00; //desabilita o reset
	in=3;
	
	wait(2);	
	cout << "@" << sc_time_stamp() <<endl;
	rwBit = 0; // habilita escrita
	sel=0x00; //escreve o endereco
	
	wait(2);
	
	sc_stop();
}