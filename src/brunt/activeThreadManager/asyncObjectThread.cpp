#include "asyncObjectThread.h"
namespace brunt
{

void CAsyncObjectThread::doit()
{
	m_result = m_asyncObject.doWork(*this);
}

}
