//logger.cpp
#include "logger.h"
#include <time.h>
#include <stdarg.h>
#include <direct.h>
#include <vector>

using std::string;
using std::vector;

namespace LOGGER
{
	CLogger::CLogger(EnumLogLevel nLogLevel, const std::string strLogPath, const std::string strLogName)
		:m_nLogLevel(nLogLevel),
		m_strLogPath(strLogPath),
		m_strLogName(strLogName)
	{
		//��ʼ��
		m_pFileStream = NULL;
		if (m_strLogPath.empty())
		{
			m_strLogPath = GetAppPathA();
		}
		if (m_strLogPath[m_strLogPath.length()-1] != '\\')
		{
			m_strLogPath.append("\\");
		}
		//�����ļ���
		//MakeSureDirectoryPathExists(m_strLogPath.c_str());
		//������־�ļ�
		if (m_strLogName.empty())
		{
			time_t curTime;
			time(&curTime);
			tm tm1;
			localtime_s(&tm1, &curTime);
			//��־�������磺201601012130.log
			m_strLogName = FormatString("%04d%02d%02d_%02d%02d%02d.log", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
		}
		m_strLogFilePath = m_strLogPath.append(m_strLogName);

		//��׷�ӵķ�ʽ���ļ���
		fopen_s(&m_pFileStream, m_strLogFilePath.c_str(), "a+");

		InitializeCriticalSection(&m_cs);
	}


	//��������
	CLogger::~CLogger()
	{
		//�ͷ��ٽ���
		DeleteCriticalSection(&m_cs);
		//�ر��ļ���
		if (m_pFileStream)
		{
			fclose(m_pFileStream);
			m_pFileStream = NULL;
		}
	}

	//�ļ�ȫ·���õ��ļ���
	const char *CLogger::path_file(const char *path, char splitter)
	{
		return strrchr(path, splitter) ? strrchr(path, splitter) + 1 : path;
	}

	//д���ش�����Ϣ
	void CLogger::TraceFatal(const char *lpcszFormat, ...)
	{
		//�жϵ�ǰ��д��־����
		if (EnumLogLevel::LogLevel_Fatal > m_nLogLevel)
			return;
		string strResult;
		if (NULL != lpcszFormat)
		{
			va_list marker = NULL;
			va_start(marker, lpcszFormat); //��ʼ����������
			size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //��ȡ��ʽ���ַ�������
			std::vector<char> vBuffer(nLength, '\0'); //�������ڴ洢��ʽ���ַ������ַ�����
			int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
			if (nWritten > 0)
			{
				strResult = &vBuffer[0];
			}
			va_end(marker); //���ñ�������
		}
		if (strResult.empty())
		{
			return;
		}
		string strLog = strFatalPrefix;
		strLog.append(GetTime()).append(strResult);

		//д��־�ļ�
		Trace(strLog);
	}

	//д������Ϣ
	void CLogger::TraceError(const char *lpcszFormat, ...)
	{
		//�жϵ�ǰ��д��־����
		if (EnumLogLevel::LogLevel_Error > m_nLogLevel)
			return;
		string strResult;
		if (NULL != lpcszFormat)
		{
			va_list marker = NULL;
			va_start(marker, lpcszFormat); //��ʼ����������
			size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //��ȡ��ʽ���ַ�������
			std::vector<char> vBuffer(nLength, '\0'); //�������ڴ洢��ʽ���ַ������ַ�����
			int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
			if (nWritten > 0)
			{
				strResult = &vBuffer[0];
			}
			va_end(marker); //���ñ�������
		}
		if (strResult.empty())
		{
			return;
		}
		string strLog = strErrorPrefix;
		strLog.append(GetTime()).append(strResult);

		//д��־�ļ�
		Trace(strLog);
	}

	//д������Ϣ
	void CLogger::TraceWarning(const char *lpcszFormat, ...)
	{
		//�жϵ�ǰ��д��־����
		if (EnumLogLevel::LogLevel_Warning > m_nLogLevel)
			return;
		string strResult;
		if (NULL != lpcszFormat)
		{
			va_list marker = NULL;
			va_start(marker, lpcszFormat); //��ʼ����������
			size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //��ȡ��ʽ���ַ�������
			std::vector<char> vBuffer(nLength, '\0'); //�������ڴ洢��ʽ���ַ������ַ�����
			int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
			if (nWritten > 0)
			{
				strResult = &vBuffer[0];
			}
			va_end(marker); //���ñ�������
		}
		if (strResult.empty())
		{
			return;
		}
		string strLog = strWarningPrefix;
		strLog.append(GetTime()).append(strResult);

		//д��־�ļ�
		Trace(strLog);
	}


	//дһ����Ϣ
	void CLogger::TraceInfo(const char *lpcszFormat, ...)
	{
		//�жϵ�ǰ��д��־����
		if (EnumLogLevel::LogLevel_Info > m_nLogLevel)
			return;
		string strResult;
		if (NULL != lpcszFormat)
		{
			va_list marker = NULL;
			va_start(marker, lpcszFormat); //��ʼ����������
			size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //��ȡ��ʽ���ַ�������
			std::vector<char> vBuffer(nLength, '\0'); //�������ڴ洢��ʽ���ַ������ַ�����
			int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
			if (nWritten > 0)
			{
				strResult = &vBuffer[0];
			}
			va_end(marker); //���ñ�������
		}
		if (strResult.empty())
		{
			return;
		}
		string strLog = strInfoPrefix;
		strLog.append(GetTime()).append(strResult);

		//д��־�ļ�
		Trace(strLog);
	}

	//��ȡϵͳ��ǰʱ��
	string CLogger::GetTime()
	{
		time_t curTime;
		time(&curTime);
		tm tm1;
		localtime_s(&tm1, &curTime);
		//2016-01-01 21:30:00
		string strTime = FormatString("%04d-%02d-%02d %02d:%02d:%02d ", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, tm1.tm_sec);

		return strTime;
	}

	//�ı�д��־����
	void CLogger::ChangeLogLevel(EnumLogLevel nLevel)
	{
		m_nLogLevel = nLevel;
	}

	//д�ļ�����
	void CLogger::Trace(const string &strLog)
	{
		try
		{
			//�����ٽ���
			EnterCriticalSection(&m_cs);
			//���ļ���û�д򿪣������´�
			if (NULL == m_pFileStream)
			{
				fopen_s(&m_pFileStream, m_strLogFilePath.c_str(), "a+");
				if (!m_pFileStream)
				{
					return;
				}
			}
			//д��־��Ϣ���ļ���
			fprintf(m_pFileStream, "%s\n", strLog.c_str());
			fflush(m_pFileStream);
			//�뿪�ٽ���
			LeaveCriticalSection(&m_cs);
		}
		//�������쳣�������뿪�ٽ�������ֹ����
		catch (...)
		{
			LeaveCriticalSection(&m_cs);
		}
	}

	string CLogger::GetAppPathA()
	{
		char szFilePath[MAX_PATH] = { 0 }, szDrive[MAX_PATH] = { 0 }, szDir[MAX_PATH] = { 0 }, szFileName[MAX_PATH] = { 0 }, szExt[MAX_PATH] = { 0 };
		GetModuleFileNameA(NULL, szFilePath, sizeof(szFilePath));
		_splitpath_s(szFilePath, szDrive, szDir, szFileName, szExt);

		string str(szDrive);
		str.append(szDir);
		return str;
	}

	string CLogger::FormatString(const char *lpcszFormat, ...)
	{
		string strResult;
		if (NULL != lpcszFormat)
		{
			va_list marker = NULL;
			va_start(marker, lpcszFormat); //��ʼ����������
			size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //��ȡ��ʽ���ַ�������
			std::vector<char> vBuffer(nLength, '\0'); //�������ڴ洢��ʽ���ַ������ַ�����
			int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
			if (nWritten > 0)
			{
				strResult = &vBuffer[0];
			}
			va_end(marker); //���ñ�������
		}
		return strResult;
	}
}
