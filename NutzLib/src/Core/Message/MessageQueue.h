#pragma once


#include "Message.h"




namespace Nutz
{



	class MessageQueue
	{
	public:
		static void Add(Ref<Message>& message);

		static void Process();

		static void Subscribe(MessageType type, std::function<bool(Ref<Message>)> fn);


	private:


		inline static std::deque<Ref<Message>> s_Messages;

		inline static std::unordered_map<MessageType, std::vector<std::function<bool(Ref<Message>)>>> s_MessageReceiverMap;


	};


}
