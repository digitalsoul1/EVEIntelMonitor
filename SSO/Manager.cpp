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
#include <QDateTime>
#include "Manager.h"
#include "User.h"


namespace EVEIntelMonitor::SSO {
    Manager::Manager(QObject *parent) : QObject(parent), m_pConfigBackend(EVEIntelMonitor::ConfigBackend::getInstance()) {
    }

    // Get user count
    long long Manager::userCount() {
        return m_pConfigBackend->groupCount("SSO-Users");
    }

    // Store user
    Manager::Status Manager::storeUser(User &&user) {
        if (userExists(user.getCharacterID())) {
            return Status::ERROR_USER_EXISTS;
        }
        m_pConfigBackend->setValue("SSO-Users/" + QString::number(user.getCharacterID()) + "/CharacterName", user.getCharacterName());
        m_pConfigBackend->setValue("SSO-Users/" + QString::number(user.getCharacterID()) + "/AccessToken", user.getAccessToken());
        m_pConfigBackend->setValue("SSO-Users/" + QString::number(user.getCharacterID()) + "/RefreshToken", user.getRefreshToken());
        m_pConfigBackend->setValue("SSO-Users/" + QString::number(user.getCharacterID()) + "/ExpiresIn", QString::number(QDateTime::currentDateTime().toSecsSinceEpoch() + user.getExpiresIn()));

        return Status::SUCCESS;
    }

    // Get user names of currently authenticated users
    QStringList Manager::getCharactersIDs() const {
        return m_pConfigBackend->getGroups("SSO-Users");
    }

    // Check if user exists
    bool Manager::userExists(const unsigned long characterId)  {
        return m_pConfigBackend->getGroups("SSO-Users").contains(QString::number(characterId));
    }

    // Get current access token
    QString Manager::getAccessToken(unsigned long characterId) const {
        return m_pConfigBackend->getValue("SSO-Users/" + QString::number(characterId) + "/AccessToken").toString();
    }

    // Check if access token is expired
    bool Manager::accessTokenExpired(unsigned long characterId) const {
        return QDateTime::currentDateTime().toSecsSinceEpoch() > m_pConfigBackend->getValue("SSO-Users/" + QString::number(characterId) + "/ExpiresIn").toLongLong();
    }

    // Get current refresh token
    QString Manager::getRefreshToken(unsigned long characterId) const {
        return m_pConfigBackend->getValue("SSO-Users/" + QString::number(characterId) + "/RefreshToken").toString();
    }

    // Update access token
    void Manager::updateAccessToken(unsigned long characterId, const QString &accessToken, const QString &refreshToken,
                                    long long int expiresIn) const {

        // expires in to minutes and seconds
        qInfo() << "Updating refresh token for character " << characterId << ". The new token expires at " << QDateTime::fromSecsSinceEpoch(QDateTime::currentDateTime().toSecsSinceEpoch() + expiresIn).toString("hh:mm:ss") << ".";

        m_pConfigBackend->setValue("SSO-Users/" + QString::number(characterId) + "/AccessToken", accessToken);
        m_pConfigBackend->setValue("SSO-Users/" + QString::number(characterId) + "/RefreshToken", refreshToken);
        m_pConfigBackend->setValue("SSO-Users/" + QString::number(characterId) + "/ExpiresIn", QString::number(QDateTime::currentDateTime().toSecsSinceEpoch() + expiresIn));
    }

    void Manager::deleteUserData() const {
        m_pConfigBackend->wipe();
    }

    QString Manager::getCharacterName(unsigned long characterId) const {
        // print all values from group SSO-Users
        return m_pConfigBackend->getValue("SSO-Users/" + QString::number(characterId) + "/CharacterName").toString();
    }
} // EVEIntelMonitor
// SSO