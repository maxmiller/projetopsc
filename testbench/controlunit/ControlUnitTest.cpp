/*
 *  ControlUnitTest.cpp
 *  controlunittestbench
 *
 *  Created by Max Miller on 14/06/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ControlUnitTest.h"

#include <iostream>

using namespace std;

void ControlUnitTest::do_test() {
	
	iRInput = 0;
	wait(1);
	cout << "@" << sc_time_stamp() << endl;
	wait(5);
	iRInput = 1;
	wait(1);
	cout << "@" << sc_time_stamp() << endl;

	iRInput = 2;
	wait(1);
	cout << "@" << sc_time_stamp() << endl;

	iRInput = 3;
	wait(1);
	cout << "@" << sc_time_stamp() << endl;

	iRInput = 4;
	wait(1);
	cout << "@" << sc_time_stamp() << endl;

	iRInput = 5;
	wait(1);
	cout << "@" << sc_time_stamp() << endl;

	sc_stop();
}