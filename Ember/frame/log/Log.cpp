#include "Log.h"
#include <time.h>

NAMESPACEBEGINE
namespace Log
{
	LogTask::LogTask(Log* log, enLogType logType, String msg, bool controlOut /*= false*/)
	{
		m_pLog = log;
		m_logType = logType;
		m_msg = msg;
		m_controlOut = controlOut;
	}

	void LogTask::Run()
	{
		if (!m_pLog){ return; }
		FILE * pFile = 0;
		if (DebugLog == m_logType)
		{
			pFile = m_pLog->m_pDebugFile;
		}
		else if (NormalLog == m_logType)
		{
			pFile = m_pLog->m_pNormalFile;
		}
		else if (ErrorLog == m_logType)
		{
			pFile = m_pLog->m_pErrorFile;
		}
		char buf[64];
		time_t t = time(0);
		struct tm * pTm = localtime(&t);
		strftime(buf, sizeof(buf), "%m/%d/%Y %H:%M:%S  ", pTm);
		String strMsg = buf + m_msg;
		String m_str;
		if (m_controlOut)
		{
			printf("%s\n", strMsg.c_astr());
		}
		if (!pFile){ return; }
		fwrite(m_msg.c_astr(), 1, m_msg.size(), pFile);
		fflush(pFile);
	}

	Log::Log()
	{
		m_pDebugFile = 0;
		m_pNormalFile = 0;
		m_pErrorFile = 0;
		m_fillter = NoneLog;
	}

	Log::~Log()
	{
		if (m_pDebugFile)
		{
			fclose(m_pDebugFile);
		}
		if (m_pNormalFile)
		{
			fclose(m_pNormalFile);
		}
		if (m_pErrorFile)
		{
			fclose(m_pErrorFile);
		}
	}

	void Log::write(enLogType lt, String strMsg)
	{
		if (lt != m_fillter){ return; }
		if (!m_pDebugFile)
		{
			if (0 == m_debugFileName.size())
			{
				m_debugFileName = "debug.log";
			}
			m_pDebugFile = fopen(m_debugFileName.c_astr(), "a+");
		}
		else if (!m_pNormalFile)
		{
			if (0 == m_normalFileName.size())
			{
				m_normalFileName = "normal.log";
			}
			m_pNormalFile = fopen(m_normalFileName.c_astr(), "a+");
		}
		else if (!m_pErrorFile)
		{
			if (0 == m_errorFileName.size())
			{
				m_errorFileName = "error.log";
			}
			m_pErrorFile = fopen(m_errorFileName.c_astr(), "a+");
		}
		if (isAsynchronous)
		{
			gThreadPool.addTask(new LogTask(this, lt, strMsg, !(m_param & lt)));
		}
		else
		{
			LogTask lt(this, lt, strMsg, !(m_param & lt));
			lt.run();
		}
	}

	void Log::setFillter(enLogType lt)
	{
		m_fillter = lt;
	}

	void Log::setFileName(enLogType lt, String fileName)
	{
		if (DebugLog == lt)
		{
			m_debugFileName = fileName;
		}
		else if (NormalLog == lt)
		{
			m_normalFileName = fileName;
		}
	}

	void Log::setParam(unsigned int param)
	{
		m_param = param;
	}

	void Log::closeFile(enLogType lt)
	{
		if (DebugLog == lt && m_pDebugFile)
		{
			fclose(m_pDebugFile);
			m_pDebugFile = 0;
		}
		else if (DebugLog == lt && m_pNormalFile)
		{
			fclose(m_pNormalFile);
			m_pNormalFile = 0;
		}
		else if (ErrorLog == lt && m_pErrorFile)
		{
			fclose(m_pErrorFile);
			m_pErrorFile = 0;
		}
	}
}
NAMESPACEEND