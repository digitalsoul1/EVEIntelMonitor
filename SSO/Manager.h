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

#include "../ConfigBackend/ConfigBackend.h"
#include "User.h"

namespace EVEIntelMonitor::SSO {
    class Manager : public QObject {
    Q_OBJECT

    public:
        enum class Status {
            SUCCESS,
            ERROR_USER_EXISTS,
            UNKNOWN_ERROR
        };

        explicit Manager(QObject *parent = nullptr);

        [[nodiscard]] long long userCount();
        [[nodiscard]] QStringList getCharactersIDs() const;
        bool userExists(unsigned long characterId);

        Status storeUser(User &&user);

        [[nodiscard]] QString getAccessToken(unsigned long characterId) const;
        [[nodiscard]] QString getRefreshToken(unsigned long characterId) const;
        [[nodiscard]] QString getCharacterName(unsigned long characterId) const;
        [[nodiscard]] bool accessTokenExpired(unsigned long characterId) const;
        void updateAccessToken(unsigned long characterId, const QString& accessToken, const QString& refreshToken, long long expiresIn) const;
        void deleteUserData() const;

    private:
        ConfigBackend *m_pConfigBackend;
    };

}
