#include "nutz_pch.h"
#include "RenderThread.h"

#include "Renderer/Renderer.h"


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
		Renderer::Get()->Pump();
	}



}
