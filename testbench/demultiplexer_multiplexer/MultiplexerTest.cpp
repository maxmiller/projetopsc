/*
 *  RegisterTest.cpp
 *  projetoregistrador
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "MultiplexerTest.h"

#include <iostream>

using namespace std;

void MultiplexerTest::do_test() {
	
	sel = 0; //
	inputs[0]  = 1; //
	
	wait(1); 
	cout << "@" << sc_time_stamp() <<endl;
	
	sel = 1; //
	inputs[1]  = 1; //
	
	wait(1);
	cout << "@" << sc_time_stamp() <<endl;
	
	sel = 2; //
	//inputs[1]  = 1; //
	
	wait(1);	
	cout << "@" << sc_time_stamp() <<endl;
	
		
	sc_stop();
}