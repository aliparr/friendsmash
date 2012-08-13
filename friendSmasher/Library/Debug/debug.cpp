#include <stdio.h>

#include "debug.h"

namespace FriendSmasher
{
	std::map<u32, u32> Debug::m_sPreviousExpressionHashes;

	void Debug::Assert(const char* Expression, const char* file, unsigned int line)
	{
		//char buf[1024];
		//sprintf(buf, "*Warning* Assert Failed: %s\nOccurred at:\nFile: %s\nLine: %d\n", Expression, file, line);
		//namespace FriendSmasherLogMessage(buf);
		abort();
    }

	void Debug::AssertMsg(const char* msg, const char* Expression, const char* file, unsigned int line)
	{
    	abort();
    }
}