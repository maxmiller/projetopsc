require 'rexml/document'
include REXML

@xml = nil

def readXML 
  file = File.open('processor.xml')
  @xml = Document.new(file)
  file.close
end

def readRegister
  regS = "//Conjunto de registradores \n"
  @xml.elements.each('//registers/register') do |register|
    regS.concat(register.attributes['type'])
    if register.attributes['style']=='pointer'
      regS.concat(' *')
    end
    regS.concat(register.text)
    regS.concat(";\n")   
  end
  regS
end

def readRegisterFile
  regS = "//Conjunto de registerFile \n"
  @xml.elements.each('//registersfiles/registerfile') do |register|
    regS.concat(register.attributes['type'])
    if register.attributes['style']=='pointer'
      regS.concat(' *')
    end
    regS.concat(register.text)
    regS.concat(";\n")   
  end
  regS
end

def readMultiplexer
  multiplexer = "//Conjunto de registerFile \n"
  @xml.elements.each('//multiplexers/multiplexer') do |x|
    multiplexer.concat(x.attributes['type'])
    if x.attributes['style']=='pointer'
      multiplexer.concat(' *')
    end
    multiplexer.concat(x.text)
    multiplexer.concat(";\n")   
  end
  multiplexer
end

def readDemultiplexer
  demultiplexer = "//Conjunto de registerFile \n"
  @xml.elements.each('//demultiplexers/demultiplexer') do |x|
    demultiplexer.concat(x.attributes['type'])
    if x.attributes['style']=='pointer'
      demultiplexer.concat(' *')
    end
    demultiplexer.concat(x.text)
    demultiplexer.concat(";\n")   
  end
  demultiplexer
end

def readULA
  ula = "//Conjunto de registerFile\n"
  @xml.elements.each('//ulas/ula') do |x|
    ula.concat(x.attributes['type'])
    if x.attributes['style']=='pointer'
      ula.concat(' *')
    end
    ula.concat(x.text+';')
    ula.concat("\n")   
  end
  ula
end

def readControlUnit
  controlunit = "//Conjunto de registerFile \n"
  @xml.elements.each('//controlunits/controlunit') do |x|
    controlunit.concat(x.attributes['type'])
    if x.attributes['style']=='pointer'
      controlunit.concat(' *')
    end
    controlunit.concat(x.text)
    controlunit.concat(";\n")   
  end
  controlunit
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

def readModel
  lines = '';
  File.open('model.h','r') do |l|
    while line = l.gets
      lines.concat(line)
    end
  end
  lines
end

def writeProcessor
  processor = File.open('Processor_gen.h','wb')
  content_model = readModel
  readR = readRegister
  content_model =  content_model.gsub('{register}',readRegister)
  content_model =  content_model.gsub('{registerfile}',readRegisterFile)
  content_model =  content_model.gsub('{multiplexer}',readMultiplexer)
  content_model =  content_model.gsub('{demultiplexer}',readDemultiplexer)
  content_model =  content_model.gsub('{ula}',readULA)
  content_model =  content_model.gsub('{controlunit}',readControlUnit)
  content_model =  content_model.gsub('{signal}',readSignal)
  content_model =  content_model.gsub('{input}',readInput)
  content_model =  content_model.gsub('{output}',readOutput)
  puts content_model
  processor.write(content_model.to_s)
  processor.close
end

readXML
writeProcessor
#readRegister
