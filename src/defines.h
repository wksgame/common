#ifndef DEFINES_H
#define DEFINES_H

typedef unsigned char						byte;
typedef char								int8;
typedef unsigned char						uint8;
typedef short								int16;
typedef unsigned short						uint16;
typedef int									int32;
typedef unsigned int						uint32;
typedef long long int						int64;
typedef unsigned long long int				uint64;

typedef uint32 buff_t;

#define SAFE_DELETE(object)	do{delete object; object = nullptr;}while(0)
#define SAFE_DELETE_ARRAY(object)	do{delete [] object; object = nullptr;}while(0)

#endif//DEFINES_H