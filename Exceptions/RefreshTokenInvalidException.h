/*
* Copyright (c) 2023 github.com/digitalsoul1
*
* This program is free software: you can redistribute it and/or modify
        * it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
        * General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
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