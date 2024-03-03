//
// Created by Lukasz Klepacki on 26/09/2023.
//

#ifndef HIVEPROTECTOR_MANAGER_H
#define HIVEPROTECTOR_MANAGER_H

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

} // EVEIntelMonitor
// SSO

#endif //HIVEPROTECTOR_MANAGER_H
