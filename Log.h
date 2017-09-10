#pragma once

#include <string>
#define LOG_FILE_PATH _T("CLog.log")

#ifdef TEST
	#define LogData Logger::Instance().log
	#define SetLogFile Logger::SetLogFilePath
#else
	#define LogData
	#define SetLogFile
#endif

using namespace std;

typedef enum tagLogLevel
{
	LOG_TRACE = 0,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_FATAL,
	LOG_NONE,
}LogLevel;

class CLog
{
public:
	static CLog& Instance();
	void SetLogFilePath(CString strFilePath, bool bAppend);
	void SetLogLevel(const int Level);
	int GetLogLevel(void);
	void Initialise(bool bAppend = false);
	void Dispose();

	void Log(const LogLevel level, CString logstring);
	void Log(const LogLevel Level, LPCTSTR Format, ...);

private:
	CLog();
	CLog(CLog const&);
	CLog& operator=(CLog const&);
	~CLog();

	CRITICAL_SECTION critical_section;

	errno_t err;
	FILE *m_File;
	CString m_strFilePath;
	LogLevel m_LogLevel;
};
