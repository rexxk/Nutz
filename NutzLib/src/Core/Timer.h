#pragma once


#include <chrono>



namespace Nutz
{


	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		void Reset()
		{
			m_StartTime = std::chrono::steady_clock::now();
		}

		double ElapsedTime()
		{
			std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();

			return (double)std::chrono::duration_cast<std::chrono::milliseconds>(t - m_StartTime).count() / 1000.0;
		}

	private:
		std::chrono::steady_clock::time_point m_StartTime;

	};


}
