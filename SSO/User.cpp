//
// Created by Lukasz Klepacki on 02/10/2023.
//

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

    } // EVEIntelMonitor
// SSO