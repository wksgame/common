#ifndef KISS_MESSAGE_PROCESS_H
#define KISS_MESSAGE_PROCESS_H

#include<map>

/**
 * 消息处理模版类,只负责处理已经解析成具体结构的消息
 * class SubClass 是继承此类的具体的消息处理类
 * class MsgClass 是要处理的消息类的基类
 */
template<typename SubClass, typename MsgClass>
class MessageProcess
{
	typedef bool(SubClass::*subcls_member_func)(const MsgClass* msg);
	std::map<int, subcls_member_func>  client_session_func_map;

public:

	bool Process(const int msgId, const MsgClass* msg)
	{
		auto it = client_session_func_map.find(msgId);
		if (it == client_session_func_map.end())
			return false;

		bool result = (((SubClass*)this)->*(it->second))(msg);
		return result;
	}

	bool RegisterMessage(const int msgId, subcls_member_func func_ptr)
	{
		if (client_session_func_map.find(msgId) != client_session_func_map.end())
			return false;

		client_session_func_map[msgId] = func_ptr;
		return true;
	}
};//MessageProcess

#endif//KISS_MESSAGE_PROCESS_H