// test.cpp : Defines the entry point for the console application.
//

#include "cppunit.h"

using namespace std;

int main(int argc, char* argv[])
{
	//TODO: pass a test config file;
	
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
	runner.addTest( registry.makeTest() );
	runner.run();
	
	return 0;
}
