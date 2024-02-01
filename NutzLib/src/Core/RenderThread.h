#pragma once



namespace Nutz
{


	class RenderThread
	{
	public:
		enum class State
		{
			Idle = 0,
			Busy,
			Kick,
		};

	public:
		RenderThread();
		~RenderThread();

		void Run();

		bool IsRunning() const { return m_IsRunning; }
		void Terminate();

		void Wait(State waitState);
		void WaitAndSet(State waitState, State setState);
		void Set(State setState);

		void NextFrame();
		void BlockUntilRenderComplete();
		void Kick();

		void Pump();

	private:

		RenderThread::State m_State = RenderThread::State::Idle;

		std::thread m_RenderThread;

		bool m_IsRunning = false;

		std::atomic<uint32_t> m_AppThreadFrame = 0;
	};


}
