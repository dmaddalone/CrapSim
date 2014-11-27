#ifndef CRAPSIMEXCEPTION_H
#define CRAPSIMEXCEPTION_H

#include <stdexcept>
#include <string>

class CrapSimException : public std::logic_error
{
    public:
        explicit CrapSimException(const std::string &s) : std::logic_error(s) {}

        CrapSimException(const std::string &sLoc, const std::string &sArg) :
            std::logic_error(sLoc),
            m_sArg(sArg)
        { }

        const std::string m_sArg;
};

#endif // CRAPSIMEXCEPTION_H
