import customCtlUnitFsmProcessor
import sys
import os

#TODO gerar expandir macro
def getMacroMicroinstructions(microInstructionMacro):
	if microInstructionMacro == "loadsARWithPc":
		return "ulaOp 5 ulaInAMuxSel 2 ulaOutDemuxSel 0 writeMemory 0 dRinMuxSel 1 dRoutDemuxSel 0 loadRA 1 wait 1 loadAR 1 wait 1 wait 1 loadDR 1 wait 1"
	if microInstructionMacro == "resetAllLoads":
		 return "rfReadWriteBit 0 writeMemory 0 loadRA 0 loadRB 0 loadIR 0 loadAR 0 loadPC 0 loadDR 0 dRinMuxSel 1 dRoutDemuxSel 0"
	if microInstructionMacro == "incrementPC":
		resetAllLoads = "rfReadWriteBit 0 writeMemory 0 loadRA 0 loadRB 0 loadIR 0 loadAR 0 loadPC 0 loadDR 0 dRinMuxSel 1 dRoutDemuxSel 0"
		return resetAllLoads + " ulaOp 4 ulaInAMuxSel 2 ulaOutDemuxSel 2 loadRA 1 wait 1 loadPC 1 wait 1"
	return None

	
def isMacro(microInstructionMacro):
 if microInstructionMacro == "incrementPC":
	 return True
 if microInstructionMacro == "resetAllLoads":
	 return True
 if microInstructionMacro == "loadsARWithPc":
	 return True
 return False


#gera codigo para um conjunto de maquinas de estado dadas
def generateCode(fsm_set):
	codeFile = open('controlUnitCode', 'w')
	codeFile.write("map<string,vector<string> > stateOutputMap;\n")
	for fsmIt in fsm_set:
		instructionName = fsmIt["name"]
		fsm = fsmIt["fsm"]
		codeFile.write("{ /*microinstructions for " + instructionName + "begin */\n")
		codeFile.write("vector<string > microInstructions;\n")
		s = fsm["initialState"]
		s_ = s["nextState"]

		commands = ""
		if not s == None:
			if isMacro(s["command"]):
				codeFile.write("/*microinstructions for macro " + s["command"] + "*/\n")
				commands = getMacroMicroinstructions(s["command"])
			else:
				commands = s["command"]
		
		codeFile.write("microInstructions.push_back(\"" + commands + "\");\n")
		while not s_ == None:
			s = s_
			if isMacro(s["command"]):
				codeFile.write("/*microinstructions for macro " + s["command"] + "*/\n")
				commands = getMacroMicroinstructions(s["command"])
			else:
				commands = s["command"]
			codeFile.write("microInstructions.push_back(\"" + commands + "\");\n")
			s_ = s["nextState"]

		codeFile.write("stateOutputMap[\"" + instructionName + "\"] = microInstructions;\n")
		codeFile.write("} /*microinstructions for " + instructionName + " end*/\n")
	codeFile.close()

if not len(sys.argv) == 2:
	print('usage: python customizableControllUnitGenerator.py <fsmDirectory>')
else:
	if os.path.isdir(sys.argv[1]):
		fsmList = customCtlUnitFsmProcessor.processFsm(sys.argv[1])
		generateCode(fsmList)
	else:
		print('usage: python customizableControllUnitGenerator.py <fsmDirectory>')
