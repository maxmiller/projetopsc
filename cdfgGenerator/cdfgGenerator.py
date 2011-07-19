
import string
import sys
import re
import instructionProcessor

#remove os comentarios
def removeComments(codeLine, commentCharSet):
	return re.sub(commentCharSet+".*\n", "", codeLine)

def findInList(l, value):
	for item in l:
		return item
	return None
		
def removeAllInList(l, valueRegEx):
	for item in l:
		if re.match(valueRegEx,item):
			l.remove(item)

def clearBlanks(stringList):
	return filter (lambda a: a != '', stringList)

def getUses(lineDelimitedBySpace):
	print lineDelimitedBySpace
	index = lineDelimitedBySpace.find("uses")
	if index != -1:
		index = lineDelimitedBySpace.find("=",index)
		lastIndex = lineDelimitedBySpace.find("\]",index)
		print lastIndex
		print lineDelimitedBySpace[index+1:lastIndex-1]
		return int(lineDelimitedBySpace[index+1:lastIndex-1])
	return -1
	

f = open(sys.argv[1])
lines = list()
splitedLines = list()
registerUseList = list()

#preprocessar para retirar todos os espacos e tabs
for line in f.readlines():
	lineDelimitedBySpace = re.sub("[ \t]+"," ",line)
	registerUses = getUses(lineDelimitedBySpace)
	lineWithoutComments = removeComments(lineDelimitedBySpace, ";")
	lines.append(lineWithoutComments)
	registerUseList.append(registerUses)

for line in lines:
	splitedLine = re.split("[\s,]+",line)
	splitedLine = clearBlanks(splitedLine)
	removeAllInList(splitedLine, "(i32)|(i32\*)|(\*i32)")
	for s in splitedLine:
		s = s.replace(',','')
	if len(splitedLine) > 0:
		splitedLines.append({"uses":registerUseList[lines.index(line)], "instruction":splitedLine})


#for line in splitedLines:
#	print line

cdfgGraph = instructionProcessor.buildCdfg(splitedLines)

states = instructionProcessor.performAsap(cdfgGraph)

print len(states)
for s in states:
	result = ""
	for instruction in s["instructions"]:
		result = result + str(instruction) + "; "
	print result
