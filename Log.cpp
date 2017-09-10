#include "stdafx.h"
#include "afxmt.h"
#include "Log.h"
#include <stdarg.h>
#include <windows.h>

#define LOGBUF_LEN 1024
TCHAR szBuffer[LOGBUF_LEN];
int useLength;

CString LogLevelStr[] = {
	_T("TRACE"),
	_T("DEBUG"),
	_T("INFO"),
	_T("WARNING"),
	_T("ERROR"),
	_T("CRITICAL"),
	_T("FATAL"),
};

CLog::CLog()
{
#ifdef _DEBUG 
	m_LogLevel = LOG_DEBUG;
#else
	m_LogLevel = LOG_INFO;
#endif

	m_File = NULL;
	m_strFilePath = LOG_FILE_PATH;
	InitializeCriticalSection(&critical_section);
	Initialise(true);
}

CLog::~CLog()
{
	Dispose();
}

void CLog::Initialise(bool bAppend)
{
	EnterCriticalSection(&critical_section);
	if (m_File == NULL && m_strFilePath.GetLength() > 0)
	{
		if (bAppend) {
			err = _tfopen_s(&m_File, m_strFilePath, _T("a+"));
		}
		else {
			err = _tfopen_s(&m_File, m_strFilePath, _T("w+"));
		}
	}
	LeaveCriticalSection(&critical_section);
}

void CLog::Dispose()
{
	//EnterCriticalSection(&critical_section);
	if (m_File != NULL) {
		fflush(m_File);
		fclose(m_File);
		m_File = NULL;
	}
	//LeaveCriticalSection(&critical_section);
}

CLog& CLog::Instance()
{
	static CLog logInstance;
	return logInstance;
}

void CLog::SetLogFilePath(CString strFilePath, bool bAppend)
{
	Dispose();
	m_strFilePath = strFilePath;
	Initialise(bAppend);
}

void CLog::SetLogLevel(const int level)
{
	m_LogLevel = (tagLogLevel)level;
}

int CLog::GetLogLevel(void)
{
	return m_LogLevel;
}

void CLog::Log(const LogLevel level, CString logstring)
{
	if (m_LogLevel > level) {
		return;
	}

	Initialise(true);

	EnterCriticalSection(&critical_section);
	DWORD LastError = GetLastError();

	ZeroMemory(szBuffer, LOGBUF_LEN);
	OutputDebugString(logstring);
#ifdef _DEBUG 
	OutputDebugString(_T("\n"));
#else
	SYSTEMTIME st;
	GetLocalTime(&st);
	
#ifdef _UNICODE
	fprintf(m_File, "[%02u:%02u:%02u:%03u] [%s] %S",
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, LogLevelStr[level], logstring);
#else
	fprintf(m_File, "[%02u:%02u:%02u:%03u] [%s] %s",
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, LogLevelStr[level], logstring);
#endif
	
#endif

	SetLastError(LastError);
	LeaveCriticalSection(&critical_section);
}

void CLog::Log(const LogLevel level, LPCTSTR Format, ...)
{
	if (m_LogLevel > level) {
		return;
	}

	Initialise(true);

	EnterCriticalSection(&critical_section);
	DWORD LastError = GetLastError();

	ZeroMemory(szBuffer, LOGBUF_LEN);
	
	va_list args;
	va_start(args, Format);
#ifdef _UNICODE
	wvsprintf(szBuffer, Format, args);
#else
	vsprintf_s(szBuffer, LOGBUF_LEN, Format, args);
#endif
	va_end(args);

	OutputDebugString(szBuffer);
#ifdef _DEBUG 
	OutputDebugString(_T("\n"));
#endif

#ifndef _DEBUG
	SYSTEMTIME st;
	GetLocalTime(&st);

#ifdef _UNICODE
	fprintf(m_File, "[%02u:%02u:%02u:%03u] [%S] %S\n",
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, LogLevelStr[level], szBuffer);
#else
	fprintf(m_File, "[%02u:%02u:%02u:%03u] [%s] %s\n",
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, LogLevelStr[level], szBuffer);
#endif
#endif

	SetLastError(LastError);
	LeaveCriticalSection(&critical_section);
	//Dispose();
}

