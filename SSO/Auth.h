#pragma once
#include <QObject>
#include <QUrl>
#include <QtHttpServer>
#include "User.h"
#include "../Generic/Result.h"

namespace EVEIntelMonitor::SSO {
    struct CharNameIdPair {
        unsigned long characterId;
        QString characterName;
    };


    class Auth : public QObject {
    Q_OBJECT

    friend class CharacterLocation;

    public:
        enum class Status {
            SUCCESS,
            PAYLOAD_ERROR,
            UNKNOWN_ERROR
        };

        explicit Auth(QObject *parent = nullptr);
        // destructor
        ~Auth() override;
        QString getAuthUrl();
        void startCallbackServer();

        static CharNameIdPair getCharacterInfo(const QString& accessToken);
        public slots:
        void getAccessTokenPayload(const QString& authCode);
        void accessTokenProcessed();

        static QString authStatusToString(Status status);
    public slots:
        void shutdownCallbackServer();

    private:
        QHttpServer *m_callbackServer;
        QString m_qsClientID;

        QString m_qsAuthCallback;
        QUrl m_quAuthEndpoint;
        QUrl m_quTokenEndpoint;
        QString m_qsScopes;
        QString m_qsCodeChallenge;
        QByteArray m_qsCodeVerifier;
        QNetworkReply* m_pAccessTokenReply;


        static QString generateRandomString(int length = 31);
        static QString errorResponseToHumanReadable(const QJsonObject& jsonObject);

        signals:
        void callbackServerAccessCodeReceived(const QString& authCode);
        void callbackServerStarted();
        void callbackServerFailed();
        void callbackServerNoAccessCode();
        void accessTokenResponseRead(Generic::Result<Status, User> respone);
        void accessTokenTimeout();
    };

}
