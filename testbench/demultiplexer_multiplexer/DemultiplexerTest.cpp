/*
 *  RegisterTest.cpp
 *  projetoregistrador
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "DemultiplexerTest.h"

#include <iostream>

using namespace std;

void DemultiplexerTest::do_test() {
	
	sel = 0; //
	input  = 1; //
	
	wait(1); 
	cout << "@" << sc_time_stamp() <<endl;
	
	sel = 1; //
	//input  = 1; //
	
	wait(1);
	cout << "@" << sc_time_stamp() <<endl;
	
	sel = 2; //
	
	wait(1);	
	cout << "@" << sc_time_stamp() <<endl;
	
	
	sc_stop();	
}