#include "nutz_pch.h"
#include "DataBuffer.h"



namespace Nutz
{


	DataBuffer::DataBuffer(uint32_t size)
	{
		m_Data.resize(size);
	}

	DataBuffer::DataBuffer(const uint8_t* data, uint32_t size)
	{
		SetData(data, size);
	}

	void DataBuffer::SetData(const uint8_t* data, uint32_t size)
	{
		if (size != m_Data.size())
			m_Data.resize(size);

		std::memcpy(m_Data.data(), (void*)data, size);
	}

}
