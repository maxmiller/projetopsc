import re

def findInstruction(instructionList):
	for item in instructionList:
		if item == "store" or item == "load" or item == "alloca": #instrucoes de registrador
			return item
		elif item == "sdiv" or item == "mul" or item == "add": #instrucoes de registrador
			return item
	return None

def getOperands(llvmInstruction, instructionType):
	operands = list()
	typeFound = False
	for item in llvmInstruction:
		if item == instructionType: #operandos devem vir depois do instructionType
			typeFound = True
		if typeFound and (re.match('[%@][a-zA-Z$._][a-zA-Z$._0-9]*', item) or re.match('[%][0-9]+', item)): #identificador
			operands.append(item)
	return operands
	
def getDest(llvmInstruction):
	return llvmInstruction[0] #dest sempre eh o primeiro valor da lista

def findEdge(edgeLabel, edgeList):
	for e in edgeList:
		if e["label"] == edgeLabel:
			return e
	return None

#constroi um cdfg no formato de lista de arestas e lista de vertices 
def buildCdfg(llvmInstructions):
	vertexList = list()
	edgeList = list()
	for instruction in llvmInstructions:
		instructionType = findInstruction(instruction["instruction"])
		print "instruction " + str(instruction) + " vIndex= " + str(len(vertexList))
		vertex = {"instruction":instructionType, "inEdges":list(), "outEdges":list(), "label":instruction["instruction"]}
		vertexList.append(vertex)
		
		vertexIndex = len(vertexList) - 1
		operands = getOperands(instruction["instruction"], instructionType)
		
		if instruction["uses"] > 0:
			dest = getDest(instruction["instruction"])

			if instructionType == "alloca":
				print ('creating edge ' + dest)
				edge = {"from":list(), "to":list(), "label":dest}
				edgeList.append(edge)
			else:
				print ('processing instruction ' + instructionType + " with " + str(len(operands)) + " operand(s)")
				for op in operands:
					print ('processing operand ' + op)
					if op.startswith("%"): #registrador
						edge = findEdge(op, edgeList)
						if edge == None:
							print ('edge not found')
							edge = {"from":list(), "to":list(), "label":op}
							edgeList.append(edge)
						else:
							print ('edge found')
						edge["to"].append(vertexIndex)
						vertex["inEdges"].append(edge)

				edge = findEdge(dest, edgeList)
				print ('processing dest ' + dest)
				if edge == None:
					print ('edge not found')
					edge = {"from":list(), "to":list(), "label":dest}
					edgeList.append(edge)
				else:
					print ('edge found')
				edge["from"].append(vertexIndex)
				vertex["outEdges"].append(edge);
		else:
			if instructionType == "store":
				edgeTmpRegister = None
				if len(operands) == 2: #dois registradores
					for op in operands:
						print ('processing operand ' + op)
						if re.match('[%][0-9]+', op): #registrador temporario
							edgeTmpRegister = findEdge(op, edgeList) #essa aresta ja foi alocada na instrucao anterior
						elif edgeTmpRegister != None and re.match('[%@][a-zA-Z$._][a-zA-Z$._0-9]*', op): #registrador destino ja foi alocado no comeco do programa
							edgeDestRegister = findEdge(op, edgeList) #essa aresta ja foi alocada no comeco do programa
							edgeDestRegister["from"].append(vertexIndex)
							edgeTmpRegister["to"].append(vertexIndex)
							vertex["inEdges"].append(edgeTmpRegister)
							vertex["outEdges"].append(edgeDestRegister)
				else: #um registrador e uma cte
					print ('processing operand ' + operands[0])
					edgeDestRegister = findEdge(operands[0], edgeList)
					edgeDestRegister["to"].append(vertexIndex)
						
						
						
	graph = {"vertexList":vertexList, "edgeList":edgeList}
	return graph

def performAsap(cdfGraph):
	#pega os pontos iniciais
	edgeStack = list()
	print "edge list size " + str(cdfGraph["edgeList"])
	stateInputs = {"instructions":list()}
	excludeStates = list()

	for edge in cdfGraph["edgeList"]:
		edge["visited"] = False
		if len(edge["from"]) == 0:
			edgeStack.append(edge)
		for v in edge["to"]:
			if cdfGraph["vertexList"][v]["instruction"] == "store":
				if len(getOperands(cdfGraph["vertexList"][v]["label"], "store")) == 1:
					stateInputs["instructions"].append(cdfGraph["vertexList"][v]["label"])
					excludeStates.append(v)
				
			
	
	print "edge stack " + str(edgeStack)
	stateList = list()
	stateList.append(stateInputs)

	while len(edgeStack) > 0:
		state = {"instructions":list()}
		vertexToCheck = list()
		while len(edgeStack) > 0:
			edge = edgeStack.pop()
			edge["visited"] = True
			for v in edge["to"]:
				vertex = cdfGraph["vertexList"][v]
				if vertexToCheck.count(v) == 0:
					if excludeStates.count(v) == 0:
						vertexToCheck.append(v) #adiciona todos os vertices das variaveis ja processadas em uma lista

		#checa em todos os vertices cujas arestas visitadas incidem
		print "vertex to check " + str(len(vertexToCheck))
		for v in vertexToCheck:
			cont = 0
			vertex = cdfGraph["vertexList"][v]
			for e in vertex["inEdges"]:
				if e["visited"]:
					cont = cont + 1
			# se todas as arestas incidentes foram visitadas entao a instrucao pode ser realizada
			if cont == len(vertex["inEdges"]):
				state["instructions"].append(vertex["label"])
				# se a instrucao foi realizada as que dependem dela podem ser realizadas
				for e in vertex["outEdges"]:
					edgeStack.append(e)
		stateList.append(state)
	return stateList
		
