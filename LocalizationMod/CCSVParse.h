//
//  CCSVParse.hpp
//  CPPAlgorithm
//
//  Created by xujw on 16/2/26.
//  Copyright © 2016年 xujw. All rights reserved.
//

#ifndef CCSVParse_h
#define CCSVParse_h

#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
/*
 * 全局函数，转换数据类型
 * toInt
 * toFloat
 * toDouble
 */
int conToInt(std::string &source);
float conToFloat(std::string &source);
double conToDouble(std::string &source);
//转成std::string
std::string conToString(int s);
std::string conToString(float s);
std::string conToString(double s);


class CCSVParse
{
public:
    CCSVParse();
    ~CCSVParse();

    /*
     *  解析成map形式（数据要保证第一行为数据类型名，第一列为id名,id不可重复)
     *  使用时通过id获取一行数据（也是map形式），然后通过类型名作为key获取数据
     *  格式如下：
     *   id   name    age
     *   1    小明     20
     *   2    小红     19
     *   3    小光     18
     */
    std::map<std::string, std::map<std::string, std::string> > parseCsvFileToMap(const std::string &fileName,const std::string &separator = ",");

    //解析出行列数据 separtor只能是一个字符(比如 , # 等)
    std::vector< std::vector<std::string> > parseCsvFile(const std::string &fileName,const std::string &separator = ",");

    //打印出解析的数据 测试用
    void printParseData() const;

    inline size_t getRowNum() const {return _gridData.size();};

    inline void useSimpleModel(bool flag){_useSimpleModel = flag;};
    /*
        分隔字符串
        str:要分隔的字符串/文件
        seperator:分隔符
     */
    std::vector<std::string> splitString(const std::string &str,const std::string &sparator);

    std::string loadCsvFile(const std::string &fileName);

private:
    //原始数据
    std::vector< std::vector<std::string> > _gridData;
    bool _useSimpleModel;   //是否使用简单模式
};

#endif /* CCSVParse_h */
