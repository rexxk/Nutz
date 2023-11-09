#pragma once


#include <cstdint>



namespace Nutz
{


	class Statistics
	{
	public:
		static void Initialize()
		{
			if (!s_Instance)
			{
				s_Instance = new Statistics;
			}
		}

		static void Shutdown()
		{
			if (s_Instance)
			{
				delete s_Instance;
			}
		}


		static Statistics& Get() { return *s_Instance; }


		uint32_t FPS = 0;


	private:
		inline static Statistics* s_Instance = nullptr;



	};


}
