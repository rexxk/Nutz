#pragma once



namespace Nutz
{


	class DataBuffer
	{
	public:
		DataBuffer() = default;
		DataBuffer(uint32_t size);
		DataBuffer(const uint8_t* data, uint32_t size);

		void SetData(const uint8_t* data, uint32_t size);

		uint32_t Size() const { return m_Size; }
		uint32_t Stride() const { return m_Stride; }

		uint32_t Count() const { return m_Count; }

		bool Crunch() const { return m_Crunch; }
		bool Encode() const { return m_Encode; }

		uint8_t* Data() { return m_Data.data(); }
		const uint8_t* Data() const { return m_Data.data(); }


	private:
		uint32_t m_Size = 0;
		uint32_t m_Stride = 1;

		uint32_t m_Count = 1;

		bool m_Encode = false;
		bool m_Crunch = false;

		std::vector<uint8_t> m_Data;
	};



}
