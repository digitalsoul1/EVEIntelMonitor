//
// Created by Lukasz Klepacki on 02/10/2023.
//

#ifndef HIVEPROTECTOR_USER_H
#define HIVEPROTECTOR_USER_H

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

#endif //HIVEPROTECTOR_USER_H
