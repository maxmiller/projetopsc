#include "Processor.h"
#include <vector>
#include <map>

using namespace std;

Processor::Processor(sc_module_name name) : sc_module(name){
	//SC_THREAD(processorBehaviour);

	{constructor}
 
	{signals}

	{assigns}
	
	{vectors}
	
	{python}
	
	{sensitive}
	{method}
	

}

	void Processor::processorBehavior2(){
		memoryAddress.write(AR->dataOut.read());

	}

	void Processor::processorBehavior(){
		//cout<<"arOutsignal "<<aRoutSignal.read()<<endl;
		//cout<<"arOutsignal "<<AR->dataOut.read()<<endl;
		//cout<<"memoryData "<<memoryDataInput.read()<<endl;
		//cout<<"statusBit "<<ulaStatusSignal<<endl;
		cout<<"memory data from data input "<<memoryDataInput.read()<<endl;
		cout<<"memory data from data output "<<memoryDataOutput.read()<<endl;
		cout<<" PC value val "<<pCoutSignal.read()<<endl;
		////cout<<"DRinSel " <<dRinMuxSelSignal.read()<<endl;
		////cout<<"DRoutSel " <<dRoutDemuxSelSignal.read()<<endl;

		switch (controlUnit->state){
			case 1:
				break;
			case 2:
				break;
			case 4:
				break;
		}
		//writeMemory.write(writeMemorySignal.read());
	}

