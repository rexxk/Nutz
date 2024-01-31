#include "nutz_pch.h"
#include "RenderThread.h"



namespace Nutz
{


	RenderThread::RenderThread()
	{
		m_Thread = std::thread(ThreadFunction);
	}

	void RenderThread::Join()
	{
		m_Thread.join();
	}

	void RenderThread::ThreadFunction()
	{
		LOG_CORE_WARN("RenderThread execution function");
	}

}
