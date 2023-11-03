#pragma once


#include <chrono>


namespace Nutz
{


	class Timestep
	{
	public:
		Timestep()
		{
			m_StartTime = std::chrono::steady_clock::now();
		}

		void Reset()
		{
			std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

			m_Duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - m_StartTime).count() / 1000.0 / 1000.0;

			m_StartTime = t1;
		}

		operator double()
		{
			return m_Duration;
		}

	private:
		
		std::chrono::steady_clock::time_point m_StartTime;

		double m_Duration = 0.0;

	};


}
