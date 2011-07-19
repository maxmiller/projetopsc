import os
import xml.dom.minidom
from xml.dom.minidom import Node


def findInDicList(l, key, obj): #encontra o retorna o i-esimo elemento de l cujo l[i][key] == obj
	for i in l:
		if i[key] == obj:
			return i
	return None

#processa todas as fsm do diretorio
def processFsm(fsmDir):
	#todo process every file in the dirctory
	files = os.listdir(fsmDir)
	fsm_set = list()
	for f in files:
		if not f.endswith(".xml"):
			continue
		print ('parsing '+ fsmDir + f)
		doc = xml.dom.minidom.parse(fsmDir + f)

		mapping = {}

		transitionTable = {} #cada elemento eh uma tripla estado, proxEstado, commandos

		states = list()
		transitions = list()
		places = list()
		arcs = list()

		initialStateId = {}

		#pega todos os lugares e label atribuidas a cada um deles
		for placeNode in doc.getElementsByTagName("place"):
			placeId = placeNode.getAttribute("id")
			name = placeNode.getElementsByTagName("name") #lista com tamanho 1
			command = name[0].getElementsByTagName("value")[0].childNodes[0].nodeValue #pegando o comando primeiro filho da tag <value> filha de <name>
			place = {'id': placeId, 'command': command}
			initialMarkingValue = ((placeNode.getElementsByTagName("initialMarking")[0]).getElementsByTagName("value")[0]).childNodes[0].nodeValue
			if initialMarkingValue == "1":
				initialStateId = placeId
			places.append(place)

		#pega todas as transicoes
		for transition in doc.getElementsByTagName("transition"):
			transitionId = transition.getAttribute("id")
			t = {'id':transitionId}
			transitions.append(t)

		#pega todos os arcos
		for arcIt in doc.getElementsByTagName("arc"):
			arcId = arcIt.getAttribute("id")
			arcFrom = arcIt.getAttribute("source")
			arcTo = arcIt.getAttribute("target")
			place = findInDicList(places, "id", arcFrom)
			arc = None
			if place == None:
				transition = findInDicList(transitions, "id", arcFrom)
				place = findInDicList(places, "id", arcTo)
				arc = {"id" : arcId, "from" : transition, "to" : place}
			else:
				transition = findInDicList(transitions, "id", arcTo)
				arc = {"id" : arcId, "from" : place, "to" : transition}
			
			arcs.append(arc)

		for t in transitions:
			arcA = findInDicList(arcs,"to", t) #de um lugar para uma transicao
			arcB = findInDicList(arcs,"from", t) #de uma transicao para um lugar
			
			actPlace = arcA["from"]
			nextPlace = arcB["to"]
			actState = findInDicList(states, "id", actPlace["id"])
			if(actState == None):
				actState = {"id" : actPlace["id"], "command" : actPlace["command"] , "nextState" : None}
				states.append(actState)
			nextState = findInDicList(states, "id", nextPlace["id"])

			if(nextState == None):
				nextState = {"id" : nextPlace["id"], "command" : nextPlace["command"] , "nextState" : None}
				states.append(nextState)
			actState["nextState"] = nextState
		initialState = findInDicList(states, "id", initialStateId)
		fsm = {"initialState":initialState, "states":states} #fsm para essa instrucao
		
		fsm_struct = {"name" : f[0:len(f)-4], "fsm" : fsm}

		fsm_set.append(fsm_struct)

		#print (states)
	#	s = fsm["initialState"]
	#	s_ = s["nextState"]
	#	print (s["id"], s["command"])
	#	while not s_ == None:
	#		s = s_
	#		print (s["id"], s["command"])
	#		s_ = s["nextState"]
	return fsm_set
