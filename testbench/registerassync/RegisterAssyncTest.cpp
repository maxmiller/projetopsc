/*
 *  RegisterTest.cpp
 *  projetoregistrador
 *
 *  Created by Max Miller da Silveira on 09/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "RegisterAssyncTest.h"

#include <iostream>

using namespace std;

void RegisterAssyncTest::do_test() {
	
	reset = 1; //reset
	load = 1;
	dataIn  = 2; //sem entrada
	wait(1); 
	cout << "@" << sc_time_stamp() <<endl;

	reset = 0;
	load = 1;
	dataIn = 3;
	wait(1);
	cout << "@" << sc_time_stamp() <<endl;
	
	reset = 0;
	load = 0;
	wait(1);
	cout << "@" << sc_time_stamp() <<endl;
	
	sc_stop();
}