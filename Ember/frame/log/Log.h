#pragma once
#include "warper/string_wper.h"
#include "thread/thread.h"
NAMESPACEBEGINE
namespace Log
{
	enum enLogType
	{
		NoneLog,
		DebugLog = 0x1,
		NormalLog = 0x4,
		ErrorLog = 0x8,
	};

	class Log;

	class LogTask : public ThreadTask
	{
	public:
		LogTask(Log* log, enLogType logType, String msg, bool controlOut = false);
		virtual void Run();
	private:
		Log *m_pLog;
		enLogType m_logType;
		String m_msg;
		bool m_controlOut;
	};

	class Log
	{
		friend class LogTask;
	public:
		Log();
		~Log();
		/*
		�����Ϣ��ָ�����ض�������
		*/
		void write(enLogType lt, String strMsg);
		void setFillter(enLogType lt);
		void setFileName(enLogType lt, String fileName);

		///ָ���Ƿ��������̨
		//@param enLogType�Ļ�ֵ
		//SetParam(DebugLog|NormalLog|ErrorLog);
		void setParam(unsigned int param);
		void closeFile(enLogType lt);
	private:
		enLogType m_fillter;
		FILE *m_pDebugFile;
		FILE *m_pNormalFile;
		FILE *m_pErrorFile;
		String m_debugFileName;
		String m_normalFileName;
		String m_errorFileName;
		///ָ���Ƿ��������̨
		unsigned int m_param;
		bool isAsynchronous = false;
	};

}
NAMESPACEEND