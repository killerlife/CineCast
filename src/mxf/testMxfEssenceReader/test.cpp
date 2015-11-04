using namespace std;
#include "../mxfEssenceReader/IEssenceReader.h"
#include <iostream>
using namespace std;
using namespace mxflib;
int main(int argc, char* argv[])
{
	 if(argc <2 )
	 {
	 	cout << "usage: testMxfEssenceReader mxfile" << endl;
	 	    return 0;
	 }
			ITaMxfEssenceReader* pReader = createEssenceReader();
			string m_testFile = argv[1];
			char buf[200];
			sprintf(buf, "read op %s ", m_testFile.c_str());
			
			pReader->open(m_testFile.c_str());
			UInt64 num = 0;
			int keyFrameOffset;
			pReader->seekElement(1, keyFrameOffset);
			while(num < 10)
			{
				DataChunkPtr data = pReader->readElement();
				if(!data)
					break;
				UInt64 len=data->Size;
				unsigned char* pbuffer=data->Data;
				++num;
				cout << num << ",frame len = " << len << endl;
				
			}
			releaseEssenceReader(pReader);
			return 0;
}
