/*
 *  RegisterTest.cpp
 *  projetoregistrador
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ULATest.h"

#include <iostream>

using namespace std;

void ULATest::do_test() {
	
	opSel = ulaOperation_add; //reset
	opA  = 1; //sem entrada
	opB = 1; //sem adress
	
	wait(1); 
	cout << "@" << sc_time_stamp() <<endl;
	
	opSel =ulaOperation_and; //reset
	opA  = 2; //sem entrada
	opB = 2; //sem adress
	
	wait(1);
	cout << "@" << sc_time_stamp() <<endl;
	
	opSel = ulaOperation_multiply; //reset
	opA  = 3; //sem entrada
	opB = 3; //sem adress
	
	wait(1);	
	cout << "@" << sc_time_stamp() <<endl;
	
	opSel = ulaOperation_inc; //reset
	opA  = 1; //sem entrada
	opB = 1; //sem adress
	
	wait(1);
	cout << "@" << sc_time_stamp() <<endl;
	
	opSel = ulaOperation_doNothing; //reset
	opA  = 1; //sem entrada
	opB = 1; //sem adress
	
	wait(1);
	cout << "@" << sc_time_stamp() <<endl;
	
	sc_stop();
}