#pragma once
#include <exception>
#include <utility>

namespace EVEIntelMonitor::Exceptions {
class RefreshTokenInvalidException : public std::exception {
public:
    RefreshTokenInvalidException(char *pszMessage) : m_pszMessage(pszMessage) {}

    const char *what() const noexcept override {
        return m_pszMessage;
    }

private:
    char *m_pszMessage;
};
} // EVEIntelMonitor::Exceptions