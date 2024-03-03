/*
* Copyright (c) 2024 github.com/digitalsoul1
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

#include <QString>

namespace EVEIntelMonitor::SSO {
    class User {
    public:
        User() = default;
        User(const QString &&characterName, unsigned long characterID, const QString &&accessToken,
             const QString &&refreshToken, unsigned long expiresIn);

        [[nodiscard]] QString getCharacterName() const;

        [[nodiscard]] unsigned long getCharacterID() const;

        [[nodiscard]] QString getAccessToken() const;

        [[nodiscard]] QString getRefreshToken() const;

        [[nodiscard]] unsigned long getExpiresIn() const;

    private:
        QString m_qsCharacterName;
        unsigned long m_qsCharacterID{};
        QString m_qsAccessToken;
        QString m_qsRefreshToken;
        unsigned int m_expiresIn{};
    };

} // EVEIntelMonitor
// SSO
