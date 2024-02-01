#include "nutz_pch.h"
#include "RenderThread.h"

#include "Renderer/Renderer.h"


namespace Nutz
{




	RenderThread::RenderThread()
	{
		m_State = RenderThread::State::Idle;
	}

	RenderThread::~RenderThread()
	{

	}

	void RenderThread::Run()
	{
		m_IsRunning = true;

		m_RenderThread = std::thread(Renderer::RenderThreadFunction, this);
	}

	void RenderThread::Terminate()
	{
		m_IsRunning = false;
		Pump();

		m_RenderThread.join();
	}

	void RenderThread::Wait(State waitState)
	{
		while (m_State != waitState)
		{

		}
	}

	void RenderThread::WaitAndSet(State waitState, State setState)
	{
		while (m_State != waitState)
		{

		}

		m_State = setState;
	}

	void RenderThread::Set(State setState)
	{
		m_State = setState;
	}

	void RenderThread::NextFrame()
	{
		m_AppThreadFrame++;
		Renderer::SwapQueues();
	}

	void RenderThread::BlockUntilRenderComplete()
	{
		Wait(State::Idle);
	}

	void RenderThread::Kick()
	{
		Set(State::Kick);
	}

	void RenderThread::Pump()
	{
		NextFrame();
		Kick();
		BlockUntilRenderComplete();
	}

}
