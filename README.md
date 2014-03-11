iniParser
=========

A simple tool to read/write .ini file, test under QT and cocos2d-x


##How to use it:


###1.Sameple file: test.ini

	#comment this file is for test
	[section1]
	intValue=1
	floatValue=0.100000
  
	[section2]
	stringValue=hello, world
	boolValue=true

###2.C++ Code to read/write it

	ConfigINI *ini = new ConfigINI("test.ini");
	ini->setIntValue("section1", "intValue", 1);
	ini->setFloatValue("section1", "floatValue", 0.1);
	ini->setStringValue("section2", "stringValue", "hello, world");
	ini->setBoolValue("section2", "boolValue", true);
	ini->writeConfigFile();
	
    int intValue = ini->getIntValue("section1", "intValue");
    float floatValue = ini->getFloatValue("section1", "floatValue");
    const char *stringValue = ini->getStringValue("section2", "stringValuee");
    bool boolValue = ini->getIntValue("section2", "boolValue");
 

A simple way to use ini files.

```make run```

```./test.out```

