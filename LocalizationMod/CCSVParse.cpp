//
//  CCSVParse.cpp
//  CPPAlgorithm
//
//  Created by xujw on 16/2/26.
//  Copyright © 2016年 xujw. All rights reserved.
//

#include "CCSVParse.h"
#include <assert.h>

CCSVParse::CCSVParse():_useSimpleModel(false){}
CCSVParse::~CCSVParse(){}

std::vector<std::string> CCSVParse::splitString(const std::string &str, const std::string &separator)
{
    std::vector<std::string> resVec;
    if ("" == str)
    {
        return resVec;
    }
    //方便截取最后一段数据
    std::string dataStr = str + separator;

    size_t pos = dataStr.find(separator);
    size_t size = dataStr.size();

    while (pos != std::string::npos)
    {
        std::string x = dataStr.substr(0,pos);
        resVec.push_back(x);
        dataStr = dataStr.substr(pos+1,size);
        pos = dataStr.find(separator);
    }

    return resVec;

//    //Method 2
//    size_t nStartPosFound = str.find(separator, 0);
//    size_t nFieldStart = 0;
//    for (; nStartPosFound != -1; nStartPosFound = str.find(separator, nStartPosFound))
//    {
//        std::string strSub = str.substr(nFieldStart, nStartPosFound - nFieldStart);
//        nStartPosFound = nStartPosFound + separator.size();
//        nFieldStart = nStartPosFound;
//
//
//        resVec.push_back(strSub);
//    }
//
//    // 加入最后一个字段
//    if (nFieldStart < str.size())
//    {
//        std::string strSub = str.substr(nFieldStart, str.size() - nFieldStart);
//        resVec.push_back(strSub);
//    }
//    return resVec;

}

std::string CCSVParse::loadCsvFile(const std::string &fileName)
{
    FILE *pFile = fopen(fileName.c_str(), "rb");
    if (0 == pFile)
    {
        return "";
    }

    fseek(pFile, 0, SEEK_END);      //指针移动到文件结尾
    long len = ftell(pFile);        //获取文件大小

    char *pBuffer = new char[len+1];

    fseek(pFile, 0, SEEK_SET);      //指针移动到文件开头
    fread(pBuffer, 1, len, pFile);  //读取文件
    fclose(pFile);

    //等价于std::string s;s.assign(pBuffer,len);
    pBuffer[len] = 0;
    std::string strRead(pBuffer,len);

    delete [] pBuffer;

    return strRead;
}

std::vector<std::vector<std::string>> CCSVParse::parseCsvFile(const std::string &fileName,const std::string &separator)
{
    _gridData.clear();

    std::string strAllData = loadCsvFile(fileName);

    if (strAllData.size() == 0)
    {
        return _gridData;
    }

    //分隔符只能是一个字符
    assert(separator.size() == 1);

    //简易模式，字段里面不能包含分隔符
    if (_useSimpleModel)
    {
        //分出行和字段
        std::vector<std::string> ret = splitString(strAllData, "\n");
        for (size_t i=0; i<ret.size(); i++)
        {
            std::vector<std::string> rowData = splitString(ret.at(i), separator);
            _gridData.push_back(rowData);
        }
        return _gridData;
    }

    //标准模式，字段里面可以包含分隔符
    //定义状态
    typedef enum stateType
    {
        kNewFieldStart = 0,  //新字段开始
        kNonQuotesField,     //非引号字段
        kQuotesField,        //引号字段
        kFieldSeparator,     //字段分隔
        kQuoteInQuotesField, //引号字段中的引号
        kBackSlash,          //转义符号
        kBackSlashInQuotesField, //引号字段中的转义符号
        kRowSeparator,       //行分隔符（回车）
        kError               //语法错误
    }StateType;

    //分出行
    std::vector<std::string> vecRows = splitString(strAllData, "\n");
    for (int i=0; i<vecRows.size(); i++)
    {
        //一行一行处理
        std::string strRowData = vecRows.at(i);
        if (0 == strRowData.size())
        {
            continue;
        }

        std::vector< std::string > vecFields;
        std::string strField;
        StateType state = kNewFieldStart;
        for (int j=0; j<strRowData.size(); j++)
        {
            const char &ch = strRowData.at(j);
            switch ( state )
            {
                case kNewFieldStart:
                {
                    if (ch == '"')
                    {
                        state = kQuotesField;
                    }
                    else if (ch == separator.at(0))
                    {
                        vecFields.push_back("");
                        state = kFieldSeparator;
                    }
                    else if (ch == '\r' || ch == '\n')
                    {
                        state = kRowSeparator;
                    }
                    else if (ch == '\\'){
                        state = kBackSlash;
                    }
                    else
                    {
                        strField.push_back(ch);
                        state = kNonQuotesField;
                    }
                }
                    break;

                case kNonQuotesField:
                {
                    if (ch == separator.at(0))
                    {
                        vecFields.push_back(strField);
                        strField.clear();
                        state = kFieldSeparator;
                    }
                    else if (ch == '\r' || ch == '\n')
                    {
                        vecFields.push_back(strField);
                        state = kRowSeparator;
                    }
                    else if (ch == '\\'){
                        state = kBackSlash;
                    }
                    else
                    {
                        strField.push_back(ch);
                    }
                }
                    break;

                case kQuotesField:
                {
                    if (ch == '"')
                    {
                        state = kQuoteInQuotesField;
                    }
                    else if (ch == '\\')
                    {
                        state = kBackSlashInQuotesField;
                    }
                    else
                    {
                        strField.push_back(ch);
                    }
                }
                    break;

                case kFieldSeparator:
                {
                    if (ch == separator.at(0))
                    {
                        vecFields.push_back("");
                    }
                    else if (ch == '"')
                    {
                        strField.clear();
                        state = kQuotesField;
                    }
                    else if (ch == '\r' || ch == '\n')
                    {
                        vecFields.push_back("");
                        state = kRowSeparator;
                    }
                    else
                    {
                        strField.push_back(ch);
                        state = kNonQuotesField;
                    }

                }
                    break;

                case kQuoteInQuotesField:
                {
                    if (ch == separator.at(0))
                    {
                        //引号闭合
                        vecFields.push_back(strField);
                        strField.clear();
                        state = kFieldSeparator;
                    }
                    else if (ch == '\r' || ch == '\n')
                    {
                        vecFields.push_back(strField);
                        state = kRowSeparator;
                    }
                    else if (ch == '"')
                    {
                        //转义引号
                        strField.push_back(ch);
                        state = kQuotesField;
                    }
                    else
                    {
                        //引号字段里包含引号时，需要对内引号进行加引号转义
                        std::cout<<"语法错误： 转义字符 \" 不能完成转义 或 引号字段结尾引号没有紧贴字段分隔符"<<std::endl;
                        assert(false);
                    }

                }
                    break;
                case kBackSlash:
                {
                    if (ch == '"')
                    {
                        //转义引号
                        strField.push_back(ch);
                        state = kNonQuotesField;
                    }
                    else if (ch == 'n')
                    {
                        //转义引号
                        strField.push_back('\n');
                        state = kNonQuotesField;
                    }
                    else if (ch == '\\')
                    {
                        //转义反斜杠
                        strField.push_back('\\');
                        state = kNonQuotesField;
                    }
                    else
                    {
                        //字段里包含引号时，需要对内引号进行加引号转义
                        std::cout<<"语法错误： 转义字符 \" 不能完成转义 或 引号字段结尾引号没有紧贴字段分隔符"<<std::endl;
                        assert(false);
                    }
                }
                    break;
                case kBackSlashInQuotesField:
                {
                    if (ch == '"')
                    {
                        //转义引号
                        strField.push_back(ch);
                        state = kQuotesField;
                    }
                    else if (ch == 'n')
                    {
                        //转义引号
                        strField.push_back('\n');
                        state = kQuotesField;
                    }
                    else if (ch == '\\')
                    {
                        //转义反斜杠
                        strField.push_back('\\');
                        state = kQuotesField;
                    }
                    else
                    {
                        //引号字段里包含引号时，需要对内引号进行加引号转义
                        std::cout<<"语法错误： 转义字符 \" 不能完成转义 或 引号字段结尾引号没有紧贴字段分隔符"<<std::endl;
                        assert(false);
                    }
                }
                    break;
                case kRowSeparator:
                {
                    _gridData.push_back(vecFields);
                    continue;
                }
                    break;

                case kError:
                {

                }
                    break;

                default:
                    break;
            }

        }

        switch (state)
        {
            case kNonQuotesField:
            {
                vecFields.push_back(strField);
                _gridData.push_back(vecFields);
            }
                break;
            case kQuoteInQuotesField:
            {
                vecFields.push_back(strField);
                _gridData.push_back(vecFields);
            }
                break;
            case kFieldSeparator:
            {
                vecFields.push_back("");
                _gridData.push_back(vecFields);
            }
                break;
            case kRowSeparator:
            {
                _gridData.push_back(vecFields);
            }
                break;
            default:
                break;
        }
    }

    return _gridData;
}

void CCSVParse::printParseData() const
{
    std::cout<<"以下是解析的csv数据:"<<std::endl;
    std::cout<<"row counts:"<<_gridData.size()<<std::endl;

    for (int row=0; row<_gridData.size(); row++)
    {
        std::vector<std::string> rowData = _gridData.at(row);
        for (int col = 0; col<rowData.size(); col++)
        {
            std::cout<<rowData.at(col)<<"\t";
        }
        std::cout<<"\n"<<std::endl;
    };
}

std::map<std::string, std::map<std::string, std::string> > CCSVParse::parseCsvFileToMap(const std::string &fileName,const std::string &separator)
{
    //先获取所有的行列数据
    std::vector<std::vector<std::string>> allData = parseCsvFile(fileName,separator);
    //转为字典形式
    std::map<std::string, std::map<std::string, std::string> > mapAllData;
    for (size_t i=1; i<allData.size(); i++)
    {
        std::vector<std::string> rowData = allData.at(i);
        //数据第一行为数据类型key
        std::vector<std::string> keyData = allData.at(0);

        std::map<std::string, std::string> mapRow;
        for (int i=0; i<keyData.size(); i++)
        {
            std::string key = keyData.at(i);
            std::string value = rowData.at(i);
            mapRow[key] = value;
        }
        //每一行数据的第一列是id
        mapAllData[rowData.at(0)] = mapRow;
    }

    return mapAllData;
}

#pragma mark--全局函数 类型转换
int conToInt(std::string &source)
{
    std::stringstream ss;
    int res;
    ss<<source;
    ss>>res;

    return res;
}
float conToFloat(std::string &source)
{
    std::stringstream ss;
    float res;
    ss<<source;
    ss>>res;

    return res;
}
double conToDouble(std::string &source)
{
    std::stringstream ss;
    double res;
    ss<<source;
    ss>>res;

    return res;
}

std::string conToString(int s)
{
    std::stringstream ss;
    std::string res;
    ss<<s;
    ss>>res;

    return res;
}
std::string conToString(float s)
{
    std::stringstream ss;
    std::string res;
    ss<<s;
    ss>>res;

    return res;
}
std::string conToString(double s)
{
    std::stringstream ss;
    std::string res;
    ss<<s;
    ss>>res;

    return res;
}
