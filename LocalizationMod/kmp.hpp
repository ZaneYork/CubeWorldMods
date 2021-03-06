/**
 * @file -
 * @author jingqi
 * @date 2012-10-18
 * @last-edit 2012-11-13 21:36:50 jingqi
 * @ref https://blog.csdn.net/jingqi_se/article/details/14055709
 */

#ifndef ___HEADFILE_A04A7FB7_1516_4EF0_A8B9_44C5AABBF7EC_
#define ___HEADFILE_A04A7FB7_1516_4EF0_A8B9_44C5AABBF7EC_

#include <assert.h>
#include <string>

namespace nut
{

/**
 * 构建KMP自动状态机(特征码)
 *
 * @param target 长度为 len 的字符串，即要搜索的字符串子串
 * @param next 长度为 len 的缓冲区，用于存放生成的KMP自动状态机(特征码)
 */
inline void kmp_build_next(const char *target, int *next, size_t len)
{
    assert(NULL != target && NULL != next && len > 0);
    if (len > 0)
        next[0] = 0;
    size_t i = 1;
    int state = 0;
    while (i < len)
    {
        if (target[i] == target[state])
            next[i++] = ++state;
        else if (0 == state)
            next[i++] = 0;
        else
            state = next[state - 1];
    }
}

/**
 * 更新KMP状态
 *
 * @param c 输入的单个字符
 * @param state 当前状态
 * @param target 要搜索的字符串子串
 * @param next KMP特征码
 * @return 新的状态
 */
inline int kmp_update(char c, int state, const char *target, const int *next)
{
    assert(state >= 0 && NULL != target && NULL != next);
    while (true)
    {
        if (c == target[state])
            return ++state;
        else if (0 == state)
            return 0;
        else
            state = next[state - 1];
    }
}

/**
 * KMP字符串搜索
 *
 * @param src 被搜索的字符串
 * @param start 开始搜索的位置
 * @param target 要搜索的字符串子串
 * @param next KMP特征码(长度与target相同)
 */
inline int kmp_search(const char *src, size_t len_src, size_t start, const char *target, const int *next, size_t len_target)
{
    assert(NULL != src && NULL != target && NULL != next);
    size_t i = start;
    int state = 0; // 状态，其实代表着已经匹配的字符串长度
    while (i < len_src && ((size_t) state) < len_target)
        state = kmp_update(src[i++], state, target, next);
    if (state == (int)len_target)
        return i - len_target;
    return -1;
}

/**
 * KMP字符串搜索
 *
 *      这里对于要搜索字符串的KMP特征码只做局部缓存，如果有特殊需求，例如被搜索的字符串不连续的存放在不同位置，
 * 则可使用其他函数组合使用(参见本函数的实现)
 *
 * @param src 被搜索的字符串
 * @param start 搜索开始的位置
 * @param target 要搜索的字符串子串
 */
inline int kmp_search(const std::string& src, size_t start, const std::string& target)
{
    int *next = new int[target.length()];
    kmp_build_next(target.c_str(), next, target.length());
    const int ret = kmp_search(src.c_str(), src.length(), start, target.c_str(), next, target.length());
    delete[] next;
    return ret;
}

inline int kmp_search(const char *src, size_t len_src, const char *target, size_t len_target)
{
    int *next = new int[len_target];
    kmp_build_next(target, next, len_target);
    const int ret = kmp_search(src, len_src, 0, target, next, len_target);
    delete[] next;
    return ret;
}

}

#endif
