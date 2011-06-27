#ifndef Processor_h
#define Processor_h

#include <systemc.h>
#include "config.h"

#include "RegisterAssyncReset.h"
#include "ControlUnit.h"
#include "RegisterFile.h"
#include "Multiplexer.h"
#include "Demultiplexer.h"
#include "ULA.h"

SC_MODULE(Processor){

	//registradores
	{register}

	//register file
	{registerfile}

	//seletores
	{multiplexer}
	{demultiplexer}

	//ula
	{ula}
	
	//unidade de controle
	{controlunit}

	{signal}
	
	{input}
	
	{output}
		
	SC_CTOR(Processor);
};
#endif
