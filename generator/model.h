#ifndef Processor_h
#define Processor_h

#include <systemc.h>
#include "config.h"

#include "RegisterAssyncReset.h"
#include "CustomizableControlUnit.h"
#include "RegisterFile.h"
#include "Multiplexer.h"
#include "Demultiplexer.h"
#include "ULA.h"

SC_MODULE(Processor){

	//components
	{components}

	{signals}
	
	{inputs}
	
	{outputs}
		
	SC_CTOR(Processor);
	
	void processorBehavior();
	void processorBehavior2();
};
#endif
