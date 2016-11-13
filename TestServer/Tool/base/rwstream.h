﻿#ifndef PROTOCOLSTREAM__H__
#define PROTOCOLSTREAM__H__

#include <string>
#include <string.h>

namespace Tool
{

	typedef enum _PACKAGELEN_TYPE
	{
		BINARY_PACKLEN_LEN = 2,
		TEXT_PACKLEN_LEN = 4
	}PACKAGELEN_TYPE;

#define PACKAGE_MAXLEN	 0xffff//65535

	class ReadStream
	{
	public:
		ReadStream(PACKAGELEN_TYPE htype_=BINARY_PACKLEN_LEN,bool net_=true):htype(htype_),net(net_){}
		virtual ~ReadStream(){}

		virtual bool readCom(/*out*/void* buffer,/*in*/size_t len_to_read,/*out*/size_t* len_readed) = 0;
		virtual size_t getSize() = 0;
		virtual const char* getData() = 0;
		virtual size_t tell() = 0;
		virtual bool isEmpty() const= 0;
		virtual bool isEnd() const= 0;
		virtual bool skip(size_t offset,bool peek=true)=0;

		bool read(/* out */ int & i);
		bool read(/* out */ short & i);
		bool read(/* out */ char & c);
		bool read(/* out */ char* str,/* in */  size_t strlen, /* out */ size_t & len);
		size_t readAll(/* out */ char * szBuffer, /*in*/size_t iLen);
	private:
		bool readLength(size_t & len);
		bool readLengthWithoutOffset(size_t & outlen);
	private:
		bool							  net;
	public:
		PACKAGELEN_TYPE htype;
	};

	class WriteStream
	{
	public:
		WriteStream(PACKAGELEN_TYPE htype_=BINARY_PACKLEN_LEN,bool net_=true):htype(htype_),net(net_){}
		virtual ~WriteStream(){}
		bool write(const char* str, size_t length);
		bool writeWithoutLength(const char* str, size_t length);
		bool write(int i);
		bool write(short i);
		bool write(char c);
		bool writeLength(size_t len);

		virtual bool writeCom(const void* buffer,size_t len_to_write,size_t* len_writed) = 0;
		virtual void flush() = 0;
		virtual const char* getData() const = 0;
		virtual size_t getSize() const = 0;
		virtual bool isValid() const = 0;

		//当peek为true时，只是查看下还有多少的内容，够不够offset的大小
		//当peek为false时，跳过指定的offset数据内容。
		virtual bool skip(size_t offset,bool peek=true)=0;
	protected:
		bool							  net;
		PACKAGELEN_TYPE htype;
	};

	class BinaryReadStream : public ReadStream
	{
	private:

		BinaryReadStream(const BinaryReadStream&);
		BinaryReadStream& operator=(const BinaryReadStream&);

	public:
		BinaryReadStream(const char* ptr_, size_t len_,PACKAGELEN_TYPE htype_=BINARY_PACKLEN_LEN);

		virtual bool readCom(/*out*/void* buffer,/*in*/size_t len_to_read,/*out*/size_t* len_readed);

		virtual size_t getSize(){ return len;}
		virtual const char* getData(){return start;}
		virtual size_t tell() {return (size_t)(cur-start);};
		virtual bool isEmpty() const {return len<=(size_t)htype;}
		virtual bool isEnd() const {return cur == end;}
		virtual bool skip(size_t offset,bool peek=true);

	private:
		const char* const start;
		const size_t len;
		const char* cur;
		const char* end;
	};

	class BinaryWriteStream : public WriteStream
	{
	private:
		BinaryWriteStream(const BinaryWriteStream&);
		BinaryWriteStream& operator=(const BinaryWriteStream&);

	public:
		BinaryWriteStream(char* ptr, size_t len,PACKAGELEN_TYPE htype_=BINARY_PACKLEN_LEN);
		virtual bool writeCom(/*in*/const void* buffer,/*in*/size_t len_to_write,/*out*/size_t* len_writed);

		virtual bool skip(size_t offset,bool peek=false);
		virtual size_t getSize() const {return (size_t)(cur - start);}
		virtual bool isValid() const;
		virtual void flush();
		virtual const char* getData() const {return start;}
	private:
		char* const start;
		const size_t len;
		char*  cur;
		char*  end;
	};
}
#endif//PROTOCOLSTREAM__H__
