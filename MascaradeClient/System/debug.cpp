
#include "debug.h"
#include <cassert>


void GfxDbgMessageBox( const char *pMessage );

struct TGfxDbgStrBuffer
{
	TGfxDbgStrBuffer() { string[ 0 ] = '\0'; }
	operator const char * () const { return string; }
	char string[ 1024 ];
};
struct TGfxDbgStrStream
{
	TGfxDbgStrBuffer buffer;

	void Append( int value )
	{
		sprintf( buffer.string, "%s%d", buffer.string, value );
	}
	void Append( const char * value )
	{
		sprintf( buffer.string, "%s%s", buffer.string, value );
	}
	TGfxDbgStrBuffer GetBuffer()
	{
		return buffer;
	}
	static TGfxDbgStrStream & GetInstance()
	{
		static TGfxDbgStrStream str_stream;
		str_stream.buffer = TGfxDbgStrBuffer();
		return str_stream;
	}
};

template<typename T> TGfxDbgStrStream & operator,(TGfxDbgStrStream & stream, const T & value)
{
	stream.Append( value );
	return stream;
}

#define GfxDbgStrFormat( ... ) ( TGfxDbgStrStream::GetInstance(), __VA_ARGS__ ).GetBuffer()

LPWSTR charToLPWSTR(const char* input)
{
	size_t wn = mbsrtowcs(NULL, &input, 0, NULL);
	wchar_t * buf = new wchar_t[wn + 1]();  // value-initialize to 0 (see below)
	mbstowcs(buf, input, wn + 1);//Plus null
	assert(input == NULL); // successful conversion
	LPWSTR ptr = buf;
	delete buf;
	return ptr;
}

void GfxDbgMsgHelper( const char * prefix, const char * text, const char * expr, const char * file, int line )
{
	TGfxDbgStrBuffer message;

	if( text != 0 && text[ 0 ] != 0 )
	{
		message = GfxDbgStrFormat( prefix, text, "\nFile : ", file, "\nLine : ", line );
	}
	else
	{
		message = GfxDbgStrFormat( prefix, expr, "\nFile : ", file, "\nLine : ", line  );
	}

	GfxDbgPrintf( "%s\n", message.string );
	GfxDbgMessageBox( message.string );
}

#define GfxDbgError() GfxDbgErrorInfo( "" )
#define GfxDbgErrorInfo( ... ) \
{ \
	TGfxDbgStrBuffer tMessage = GfxDbgStrFormat( __VA_ARGS__ ); \
	GfxDbgMsgHelper( "Critical error : ", tMessage.string, "???", __FILE__, __LINE__ ); \
	GfxDbgBreak(); \
}

#define GfxDbgBreak() if( IsDebuggerPresent() ) { __asm { _emit 0xF1 }; } else DebugBreak()

void GfxDbgMessageBox(const char *pMessage)
{
	MessageBox(0, charToLPWSTR(pMessage), 0, MB_OK | MB_ICONERROR);
}

int GfxDbgPrintf(const char *pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);

	int count = _vscprintf(pFormat, args);
	char * buffer = (char *)_malloca(count + 1);
	vsnprintf_s(buffer, count + 1, _TRUNCATE, pFormat, args);
	OutputDebugString(charToLPWSTR(buffer));

	CharToOem(charToLPWSTR(buffer), buffer);
	fputs(buffer, stdout);

	_freea(buffer);
	return count;
	return 0;
}

void GfxDbgAssert( bool bExpectedTrue, const char * pMessage )
{
	if( !bExpectedTrue )
	{
		// if your application breaks here, go back one level in the callstack
		GfxDbgErrorInfo( !pMessage ? "assertion failure" : pMessage );
	}
}

#define GfxDbgAssert( expr ) GfxDbgAssertInfo( expr, "" )
#define GfxDbgAssertInfo( expr, ... ) \
for(bool value = (expr); !value; value = true) \
{ \
	TGfxDbgStrBuffer tMessage = GfxDbgStrFormat( __VA_ARGS__ ); \
	GfxDbgMsgHelper( "Assertion failed : ", tMessage.string, #expr, __FILE__, __LINE__ ); \
	GfxDbgBreak(); \
}