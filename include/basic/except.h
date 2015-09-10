#ifndef EXCEPT_INCLUDE_H
#define EXCEPT_INCLUDE_H

#include "base_def.h"
#include <exception>
#include <string>

// 下面两行代码是为解决C4251警告
template class BASE_EXPORT std::allocator<char>;
template class BASE_EXPORT std::basic_string<char>;

class BASE_EXPORT Except : public std::exception
{
public:
    Except(int code, const char * strWhat = NULL)
        : m_strWhat(NULL == strWhat ? "" : strWhat)
        , m_code(code)
    {
    }

    virtual ~Except() throw() {};

    int32_t GetCode() const {return m_code;}

    virtual const char* what() const throw()
    {
        return m_strWhat.c_str();
    }

protected:
    int m_code;
    std::string m_strWhat;
};
#endif