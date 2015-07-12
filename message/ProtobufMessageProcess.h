#ifndef KISS_PROTOBUF_MESSAGE_PROCESS_H
#define KISS_PROTOBUF_MESSAGE_PROCESS_H

#include<map>
#include<google/protobuf/message_lite.h>

namespace kiss
{
	namespace pb
	{
		/**
		 * protobuf消息处理模版类,只负责处理已经解析成具体结构的消息
		 * class SubClass 是继承此类的具体的消息处理类
		 */
		template<typename SubClass>
		class ProtobufMessageProcess
		{
			typedef bool(SubClass::*subcls_member_func)(const google::protobuf::MessageLite* msg);
			typedef std::pair<subcls_member_func, google::protobuf::MessageLite*> func_message_item;

			std::map<int, func_message_item>  client_session_func_map;

		public:

			bool Process(const int msgId, const void* data, const int size)
			{
				auto it = client_session_func_map.find(msgId);
				if (it == client_session_func_map.end())
					return false;

				auto func = it->second.first;
				auto msg = it->second.second;

				msg->Clear();
				if (!msg->ParseFromArray(data, size))
					return false;

				bool result = (((SubClass*)this)->*(func))(msg);
				return result;
			}

			bool RegisterMessage(const int msgId, subcls_member_func func_ptr, google::protobuf::MessageLite* msg)
			{
				if (client_session_func_map.find(msgId) != client_session_func_map.end())
					return false;

				client_session_func_map[msgId] = std::make_pair(func_ptr,msg);
				return true;
			}
		};//MessageProcess
	}//namespace pb	
}//namespace kiss

#endif//KISS_PROTOBUF_MESSAGE_PROCESS_H