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
 * ����KMP�Զ�״̬��(������)
 *
 * @param target ����Ϊ len ���ַ�������Ҫ�������ַ����Ӵ�
 * @param next ����Ϊ len �Ļ����������ڴ�����ɵ�KMP�Զ�״̬��(������)
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
 * ����KMP״̬
 *
 * @param c ����ĵ����ַ�
 * @param state ��ǰ״̬
 * @param target Ҫ�������ַ����Ӵ�
 * @param next KMP������
 * @return �µ�״̬
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
 * KMP�ַ�������
 *
 * @param src ���������ַ���
 * @param start ��ʼ������λ��
 * @param target Ҫ�������ַ����Ӵ�
 * @param next KMP������(������target��ͬ)
 */
inline int kmp_search(const char *src, size_t len_src, size_t start, const char *target, const int *next, size_t len_target)
{
    assert(NULL != src && NULL != target && NULL != next);
    size_t i = start;
    int state = 0; // ״̬����ʵ�������Ѿ�ƥ����ַ�������
    while (i < len_src && ((size_t) state) < len_target)
        state = kmp_update(src[i++], state, target, next);
    if (state == (int)len_target)
        return i - len_target;
    return -1;
}

/**
 * KMP�ַ�������
 *
 *      �������Ҫ�����ַ�����KMP������ֻ���ֲ����棬����������������类�������ַ����������Ĵ���ڲ�ͬλ�ã�
 * ���ʹ�������������ʹ��(�μ���������ʵ��)
 *
 * @param src ���������ַ���
 * @param start ������ʼ��λ��
 * @param target Ҫ�������ַ����Ӵ�
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
