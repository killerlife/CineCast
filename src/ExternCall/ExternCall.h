#pragma once
#include <string>

class IExternCall
{
public:
	virtual ~IExternCall(){};
	virtual std::string GetOutput() = 0;
	virtual void SetInput(std::string strInput) = 0;
	virtual bool IsFinish() = 0;
	virtual void SetCommandLine(std::string strCmdLine) = 0;
	virtual bool RunCommand() = 0;
	virtual bool RunCommand(std::string strCmdLine) = 0;
};

IExternCall* CreateExternCall();
void ReleaseExternCall(IExternCall* extCall);
