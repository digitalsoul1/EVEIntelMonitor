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
#include "User.h"

    namespace EVEIntelMonitor::SSO {
        QString User::getCharacterName() const {
            return m_qsCharacterName;
        }

        unsigned long User::getCharacterID() const {
            return m_qsCharacterID;
        }

        QString User::getAccessToken() const {
            return m_qsAccessToken;
        }

        QString User::getRefreshToken() const {
            return m_qsRefreshToken;
        }

        unsigned long User::getExpiresIn() const {
            return m_expiresIn;
        }

        User::User(const QString &&characterName, unsigned long characterID, const QString &&accessToken,
                   const QString &&refreshToken, unsigned long expiresIn) : m_qsCharacterName(characterName),
                                                                             m_qsCharacterID(characterID),
                                                                             m_qsAccessToken(accessToken),
                                                                             m_qsRefreshToken(refreshToken),
                                                                             m_expiresIn(expiresIn) {
        }

    }