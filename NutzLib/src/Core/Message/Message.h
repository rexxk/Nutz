#pragma once



namespace Nutz
{


	enum class MessageState
	{
		Unused,
		InUse,
		UsedUp,
	};


	enum class MessageType
	{
		Undefined, WindowClosed, WindowResized,
		KeyPressed, KeyReleased,
	};



	class Message
	{
	public:
		virtual ~Message() {}

		MessageType Type = MessageType::Undefined;
		MessageState State = MessageState::Unused;

	};




}
