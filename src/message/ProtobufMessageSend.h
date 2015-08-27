#ifndef KISS_PROTOBUF_MESSAGE_SEND_H
#define KISS_PROTOBUF_MESSAGE_SEND_H

#include<cstring>
#include<defines.h>
#include<logger/logger.h>

namespace kiss
{
	namespace pb
	{
		template<int buffSize = 4096>
		class ProtobufMessageSend
		{
		public:

			char buff[buffSize];
			int curPos;

		public:

			ProtobufMessageSend()
			{
				ClearData();
			}

			virtual ~ProtobufMessageSend(){};

			void ClearData()
			{
				curPos=0;
			}

			template<typename T> 
			bool Append(const T *msg)
			{
				int32 dataSize=msg->ByteSize();

				if(dataSize<0)
					return(true);
				
				if(dataSize>buffSize-curPos)
					return(false);

				int32 *mb_int=(int32 *)(buff+curPos);

				*mb_int=dataSize+sizeof(int32);		//dataSize+msgIdSize
				++mb_int;
				*mb_int=T::id;
				++mb_int;

				if(dataSize==0)
					return(true);

				if(!msg->SerializeToArray(mb_int,dataSize))
				{
					syslogger.error("protobuf SerializeToArray error");
					return(false);
				}
				
				curPos+=dataSize+sizeof(int32)*2;

				return(true);
			}

			template<typename T>
			ProtobufMessageSend &operator << (const T *msg)
			{
				Append(msg);

				return(*this);
			}
		};//class ProtobufMessageSend
	}//namespace pb
}//namespace kiss
#endif//KISS_PROTOBUF_MESSAGE_SEND_H