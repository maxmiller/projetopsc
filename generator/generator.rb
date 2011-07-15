require 'rexml/document'
include REXML
#require "rubypython"

@xml = nil
@constructor = ""

  
def readXML 
  file = File.open('processor.xml')
  @xml = Document.new(file)
  file.close
end

def readConstructor(node)
    @constructor.concat(node.attributes['value'])
    @constructor.concat(" = new "+node.attributes['type'])
    parameters="\""+node.attributes['value']+"\""
    if node.elements.size>0 then
        node.each_element("parameter") do |p|
            parameters = parameters+","+p.text
        end
    end
    @constructor.concat("("+parameters+");\n")
end

def readAssigns
  vector = ""
  variavel = @xml.elements["//assigns/assign"].attributes["variable"] + "->assign"
  @xml.elements.each('//assigns/assign/parameter') do |x|
    vector.concat(variavel)
    vector.concat("(\""+x.attributes['internal']+"\",&"+x.attributes['external']+");\n")
  end
  vector
end

def readVector
  vector = ""
  nomevector = @xml.elements["//vectors"].attributes["name"]
  vector = "vector <std::string> "+ nomevector + ";\n"
  @xml.elements.each('//vectors/vector') do |x|
    vector.concat(nomevector+".push_back(\"");
    vector.concat(x.text);
    vector.concat("\");\n")
  end
  vector
end

def readSignalInternal
  signal=""
  @xml.elements.each('//processorsignals/psignals') do |x|
    signal.concat(x.attributes['variable'])
    signal.concat("->")
    signal.concat(x.elements["psignal"].attributes["internal"])
    signal.concat("(")
    signal.concat(x.elements["psignal"].text)
    signal.concat(");\n")
  end
  signal
end

def readComponents
  components = "//Conjunto de registradores \n"
  @xml.elements.each('//components/component') do |x|
    readConstructor(x)
    components.concat(x.attributes['type'])
    if x.attributes['style']=='pointer'
      components.concat(' *')
    end
    components.concat(x.attributes['value'])
    components.concat(";\n")   
  end
  components
end

def readSignal
  signal = "//Signals \n"
  @xml.elements.each('//signals/signal') do |x|
    signal.concat("sc_signal<")
    signal.concat(x.attributes['type'])
    if x.attributes['length']!='0'
      signal.concat("<")
      signal.concat(x.attributes['length'])
      signal.concat(">")
    end
    signal.concat(" >")
    signal.concat(x.text)
    signal.concat(";\n")   
  end
  signal
end

def readMethods
  method = "//Signals \n"
  @xml.elements.each('//methods/method') do |x|
    method.concat(x.attributes['type'])
    method.concat("(")
    method.concat(x.text)
    method.concat(");\n")
  end
  method
end

def readSensitive
  method = "//Signals \n"
  @xml.elements.each('//sensitives/sensitive') do |x|
    method.concat("sensitive << ")
    method.concat(x.text)
    method.concat(";\n")
  end
  method
end

def readInput
  signal = "//Inputs \n"
  @xml.elements.each('//inputs/input') do |x|
    signal.concat("sc_in<")
    signal.concat(x.attributes['type'])
    if x.attributes['length']!='0'
      signal.concat("<")
      signal.concat(x.attributes['length'])
      signal.concat(">")
    end
    signal.concat(" >")
    signal.concat(x.text)
    signal.concat(";\n")   
  end
  signal
end

def readOutput
  signal = "//Outputs \n"
  @xml.elements.each('//outputs/output') do |x|
    signal.concat("sc_out<")
    signal.concat(x.attributes['type'])
    if x.attributes['length']!='0'
      signal.concat("<")
      signal.concat(x.attributes['length'])
      signal.concat(">")
    end
    signal.concat(" >")
    signal.concat(x.text)
    signal.concat(";\n")   
  end
  signal
end

def readFile(file)
  lines = '';
  File.open(file,'r') do |l|
    while line = l.gets
      lines.concat(line)
    end
  end
  lines
end

def writeProcessor
  processor = File.open('Processor.h','wb')
  content_model = readFile("model.h")
  #readR = readRegister
  content_model =  content_model.gsub('{components}',readComponents)
  content_model =  content_model.gsub('{signals}',readSignal)
  content_model =  content_model.gsub('{inputs}',readInput)
  content_model =  content_model.gsub('{outputs}',readOutput)
  puts content_model
  processor.write(content_model.to_s)
  processor.close
end

def writeProcessorMain
  processor = File.open('Processor.cpp','wb')
  content_model = readFile("model.cpp")
  python  = readFile("controlUnitCode")
  content_model =  content_model.gsub('{constructor}',@constructor)
  content_model =  content_model.gsub('{signals}',readSignalInternal)
  content_model =  content_model.gsub('{vectors}',readVector)
  content_model =  content_model.gsub('{python}',python)  
  content_model =  content_model.gsub('{method}',readMethods)  
  content_model =  content_model.gsub('{sensitive}',readSensitive)  
  content_model =  content_model.gsub('{assigns}',readAssigns)  
  puts content_model
  processor.write(content_model.to_s)
  processor.close
end

def writeProcessorDoc
    readXML
    writeProcessor
    writeProcessorMain
end
#readXML
#writeProcessor
#readRegister
writeProcessorDoc
#rubyPythonShell