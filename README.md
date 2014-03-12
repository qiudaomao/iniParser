iniParser
=========

A simple tool to read/write .ini file, already test it under QT and cocos2d-x


###1. Sample file: test.ini

	#comment line
	[section1]
	intValue=1
	floatValue=0.100000
  
	[section2]
	stringValue=hello, world
	boolValue=true

###2. C++ Code to read/write it

    //write
	ConfigINI *ini = new ConfigINI("test.ini");
	ini->setIntValue("section1", "intValue", 1);
	ini->setFloatValue("section1", "floatValue", 0.1);
	ini->setStringValue("section2", "stringValue", "hello, world");
	ini->setBoolValue("section2", "boolValue", true);
	ini->writeConfigFile();
	
    //read
    int intValue = ini->getIntValue("section1", "intValue");
    float floatValue = ini->getFloatValue("section1", "floatValue");
    const char *stringValue = ini->getStringValue("section2", "stringValue");
    bool boolValue = ini->getBoolValue("section2", "boolValue");
 

A simple way to use ini files.

```make run```

```./test.out```

Done.
