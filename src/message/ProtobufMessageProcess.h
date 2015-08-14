#ifndef KISS_PROTOBUF_MESSAGE_PROCESS_H
#define KISS_PROTOBUF_MESSAGE_PROCESS_H

#include<unordered_map>
#include<google/protobuf/message_lite.h>

namespace kiss
{
	class Session;

	namespace pb
	{
		template<typename SubClass>
		class ProtobufMessageProcess
		{
			typedef bool(SubClass::*subcls_member_func)(const google::protobuf::MessageLite* msg);
			typedef std::pair<subcls_member_func, google::protobuf::MessageLite*> func_message_item;

			std::unordered_map<int, func_message_item>  client_session_func_map;

			SubClass* session;

		public:
			ProtobufMessageProcess(SubClass* session)
			{
				this->session = session;
			}

			~ProtobufMessageProcess()
			{
				for(auto&i :client_session_func_map)
					delete i.second.second;
				client_session_func_map.clear();
			}

			bool Process(const char* data, const int size)
			{
				if(!data || size<4)
					return false;

				int msgId = *((int*)data);

				auto it = client_session_func_map.find(msgId);
				if (it == client_session_func_map.end())
					return false;

				auto func = it->second.first;
				auto msg = it->second.second;

				msg->Clear();
				if (!msg->ParseFromArray(data+4, size-4))
					return false;

				bool result = (((SubClass*)session)->*(func))(msg);
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