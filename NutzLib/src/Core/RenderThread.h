#pragma once



namespace Nutz
{


	enum class ThreadState
	{
		Idle,
		Executing,
		Waiting,
	};

	class RenderThread
	{
	public:
		RenderThread();


		void Join();

		static void ThreadFunction();

	private:
		std::thread m_Thread;
	};


}
