
//logger.h
/*
//������CLogger
//���ܽ��ܣ�Winƽ̨��־��¼���ܣ����̰߳�ȫ��֧��д��־��������ã���־��ʽ������־�ȼ�����־ʱ�䣬�ļ������к���Ϣ
//���ߣ�sunflover 2016-1-15 14:31:27
//ʹ�÷�����
1����logger.h��logger.cpp��ӵ���Ŀ��
2������logger.cpp��Ԥ����ͷѡ��Ϊ����ʹ��Ԥ����ͷ��
3��ʹ�ô���ʾ����
#include "Logger.h"
using namespace LOGGER;
CLogger logger(LogLevel_Info,CLogger::GetAppPathA().append("log\\"));
void main()
{
logger.TraceFatal("TraceFatal %d", 1);
logger.TraceError("TraceError %s", "sun");
logger.TraceWarning("TraceWarning");
logger.TraceInfo("TraceInfo");
logger.ChangeLogLevel(LOGGER::LogLevel_Error);
logger.TraceFatal("TraceFatal %d", 2);
logger.TraceError("TraceError %s", "sun2");
logger.TraceWarning("TraceWarning");
logger.TraceInfo("TraceInfo");
}
ִ�н����20160126_101329.log�ļ���������
Fatal	2016-01-26 10:13:29 TraceFatal 1
Error	2016-01-26 10:13:29 TraceError sun
Warning	2016-01-26 10:13:29 TraceWarning
Info	2016-01-26 10:13:29 TraceInfo
Fatal	2016-01-26 10:13:29 TraceFatal 2
Error	2016-01-26 10:13:29 TraceError sun2
*/

#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <Windows.h>
#include <stdio.h>
#include <string>

namespace LOGGER
{
	//��־�������ʾ��Ϣ
	static const std::string strFatalPrefix = "Fatal\t";
	static const std::string strErrorPrefix = "Error\t";
	static const std::string strWarningPrefix = "Warning\t";
	static const std::string strInfoPrefix = "Info\t";

	//��־����ö��
	typedef enum EnumLogLevel
	{
		LogLevel_Stop = 0,	//ʲô������¼
		LogLevel_Fatal,		//ֻ��¼���ش���
		LogLevel_Error,		//��¼���ش�����ͨ����
		LogLevel_Warning,	//��¼���ش�����ͨ���󣬾���
		LogLevel_Info		//��¼���ش�����ͨ���󣬾��棬��ʾ��Ϣ(Ҳ����ȫ����¼)
	};

	class CLogger
	{
	public:
		//nLogLevel����־��¼�ĵȼ����ɿ�
		//strLogPath����־Ŀ¼���ɿ�
		//strLogName����־���ƣ��ɿ�
		CLogger(EnumLogLevel nLogLevel = EnumLogLevel::LogLevel_Info, const std::string strLogPath = "", const std::string strLogName = "");
		//��������
		virtual ~CLogger();
	public:
		//д���ش�����Ϣ
		void TraceFatal(const char *lpcszFormat, ...);
		//д������Ϣ
		void TraceError(const char *lpcszFormat, ...);
		//д������Ϣ
		void TraceWarning(const char *lpcszFormat, ...);
		//д��ʾ��Ϣ
		void TraceInfo(const char *lpcszFormat, ...);
		//�ı�д��־����
		void ChangeLogLevel(EnumLogLevel nLevel);
		//��ȡ��������·��
		static std::string GetAppPathA();
		//��ʽ���ַ���
		static std::string FormatString(const char *lpcszFormat, ...);
	private:
		//д�ļ�����
		void Trace(const std::string &strLog);
		//��ȡ��ǰϵͳʱ��
		std::string GetTime();
		//�ļ�ȫ·���õ��ļ���
		const char *path_file(const char *path, char splitter);
	private:
		//д��־�ļ���
		FILE * m_pFileStream;
		//д��־����
		EnumLogLevel m_nLogLevel;
		//��־Ŀ¼
		std::string m_strLogPath;
		//��־������
		std::string m_strLogName;
		//��־�ļ�ȫ·��
		std::string m_strLogFilePath;
		//�߳�ͬ�����ٽ�������
		CRITICAL_SECTION m_cs;
	};
}

#endif
