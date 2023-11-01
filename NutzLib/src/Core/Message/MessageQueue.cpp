#include "nutz_pch.h"
#include "MessageQueue.h"


#include <iostream>


namespace Nutz
{


	void MessageQueue::Add(Ref<Message>& message)
	{
		std::cout << "added message to queue\n";

		s_Messages.push_front(message);
	}

	void MessageQueue::Process()
	{
		while (!s_Messages.empty())
		{
			Ref<Message> message = s_Messages.back();
			s_Messages.pop_back();

			for (auto& fn : s_MessageReceiverMap[message->Type])
			{
				if (fn(message))
				{
					message->State = MessageState::UsedUp;
				}
				else
				{
					message->State = MessageState::InUse;
				}
			}
		}
	}


	void MessageQueue::Subscribe(MessageType type, std::function<bool(Ref<Message>)> fn)
	{
		s_MessageReceiverMap[type].emplace_back(fn);
	}


}
