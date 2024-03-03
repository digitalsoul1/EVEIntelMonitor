//
// Created by Lukasz Klepacki on 25/09/2023.
//

#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QUrlQuery>
#include <QtHttpServer>
#include "User.h"
#include "Auth.h"


namespace EVEIntelMonitor::SSO {
    QString Auth::getAuthUrl() {
        QUrlQuery query;
        m_qsCodeVerifier = (QUuid::createUuid().toString(QUuid::WithoutBraces) +
                            QUuid::createUuid().toString(QUuid::WithoutBraces)).toLatin1();
        m_qsCodeChallenge = QCryptographicHash::hash(m_qsCodeVerifier,
                                                     QCryptographicHash::Sha256).toBase64(
                QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);
        QString stateString = generateRandomString(QRandomGenerator::global()->bounded(10, 20));
        query.setQueryItems({{"response_type",         "code"},
                             {"redirect_uri",          QUrl::toPercentEncoding(m_qsAuthCallback)},
                             {"client_id",             m_qsClientID},
                             {"scope",                 m_qsScopes},
                             {"code_challenge",        m_qsCodeChallenge},
                             {"code_challenge_method", "S256"},
                             {"state",                 stateString}});

        return m_quAuthEndpoint.toString() + "?" + query.toString();
    }

    QString Auth::generateRandomString(int length) {
        // generate random string
        QString randomString;
        for (int i = 0; i <= length; ++i) {
            randomString.append(QChar(QRandomGenerator::global()->bounded(65, 90)));
        }

        return randomString;
    }

    Auth::Auth(QObject *parent) : QObject(parent) {
        m_qsClientID = "546d32766f5448c49f69360b6c5670f0";
        m_qsAuthCallback = "http://localhost:8081/callback";
        m_quAuthEndpoint = QUrl("https://login.eveonline.com/v2/oauth/authorize/");
        m_quTokenEndpoint = QUrl("https://login.eveonline.com/v2/oauth/token/");
        m_qsScopes = "esi-location.read_location.v1";

        m_callbackServer = nullptr;
        m_pAccessTokenReply = nullptr;

        connect(this, &Auth::callbackServerAccessCodeReceived, this, &Auth::getAccessTokenPayload);

    }

    Auth::~Auth() {

    }

    void Auth::startCallbackServer() {
        if (m_callbackServer != nullptr) {
            qInfo() << "Callback server already started";

        } else {
            m_callbackServer = new QHttpServer(this);

            // setup route to callback
            m_callbackServer->route("/callback", [this](const QHttpServerRequest &request) {
                if (request.query().hasQueryItem("code")) {
                    emit callbackServerAccessCodeReceived(request.query().queryItemValue("code"));
                } else
                        emit callbackServerNoAccessCode();

                return "You can close this window now.";
            });

            connect(m_callbackServer, &QHttpServer::destroyed, this, [] {
                qInfo() << "Callback server destroyed";
            });
            m_callbackServer->listen(QHostAddress::LocalHost, 8081);
            if (m_callbackServer->serverPorts().empty()) {
                delete m_callbackServer;
                qInfo() << "Failed to start callback server.";
                emit callbackServerFailed();
            } else {
                qInfo() << "Callback server started on port: " << m_callbackServer->serverPorts()[0];
                emit callbackServerStarted();
            }
        }
    }

    void Auth::getAccessTokenPayload(const QString &authCode) {
        qDebug() << "Access token request started";
        QNetworkAccessManager manager;
        QNetworkRequest request;

        request.setUrl(m_quTokenEndpoint.toString());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setRawHeader("Host", "login.eveonline.com");

        QUrlQuery query;
        query.addQueryItem("grant_type", "authorization_code");
        query.addQueryItem("code", authCode);
        query.addQueryItem("client_id", m_qsClientID);
        query.addQueryItem("code_verifier", m_qsCodeVerifier);

        m_qsCodeChallenge.clear();
        m_qsCodeVerifier.clear();

        m_pAccessTokenReply = manager.post(request, query.toString().toUtf8());

        // connect destroyed signal to delete the reply
        connect(m_pAccessTokenReply, &QNetworkReply::destroyed, this, [] {
            qInfo() << "Access token reply object destroyed";
        });

        manager.setTransferTimeout(10000);
        qInfo() << "Waiting for access token response ...";
        QEventLoop loop;
        connect(m_pAccessTokenReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (m_pAccessTokenReply->error() == QNetworkReply::NoError) {
            accessTokenProcessed();
        } else {
            emit accessTokenTimeout();
        }

        delete m_pAccessTokenReply;
    }

    CharNameIdPair Auth::getCharacterInfo(const QString &accessToken) {
        QList<QString> accessTokenComponents = accessToken.split(".");
        QByteArray decodedPayload = QByteArray::fromBase64(accessTokenComponents[1].toUtf8());
        QJsonDocument jsonDoc = QJsonDocument::fromJson(decodedPayload);

        QJsonObject jsonObject = jsonDoc.object();

        QString characterName = jsonObject.value("name").toString();
        unsigned long characterId = jsonObject.value("sub").toString().split(":")[2].toULong();

        return {characterId, characterName};
    }

    void Auth::accessTokenProcessed() {
        QByteArray response = m_pAccessTokenReply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObject = jsonDoc.object();

        // Check if the json returned contains all data we need
        if (jsonObject.contains("access_token") && jsonObject.contains("refresh_token") &&
            jsonObject.contains("expires_in")) {
            // Get the data from the json
            QString accessToken = jsonObject["access_token"].toString();
            QString refreshToken = jsonObject["refresh_token"].toString();
            unsigned long expiresIn = jsonObject["expires_in"].toInt();

            CharNameIdPair characterInfo = getCharacterInfo(accessToken);

            qInfo() << "refresh token is: " << refreshToken;
            emit accessTokenResponseRead({Auth::Status::SUCCESS, {
                    std::move(std::move(characterInfo.characterName)),
                    characterInfo.characterId,
                    std::move(accessToken),
                    std::move(refreshToken),
                    expiresIn}
                                         });


        } else {
            emit accessTokenResponseRead({Auth::Status::UNKNOWN_ERROR, {}});
        }
    }

    QString Auth::authStatusToString(Status status) {
        switch (status) {
            case Auth::Status::SUCCESS:
                return "SUCCESS";
            case Auth::Status::PAYLOAD_ERROR:
                return "PAYLOAD_ERROR";
            case Auth::Status::UNKNOWN_ERROR:
                return "UNKNOWN_ERROR";
        }
    }

    QString Auth::errorResponseToHumanReadable(const QJsonObject &jsonObject) {
        QString errorString;
        if (jsonObject.contains("error_description"))
            errorString.append(jsonObject["error_description"].toString());
        else if (jsonObject.contains("error"))
            errorString.append(jsonObject["error"].toString());
        else
            errorString = "Unknown error";

        return errorString;
    }

    void Auth::shutdownCallbackServer() {
        if (m_callbackServer != nullptr) {
            qInfo() << "Shutting down callback server";
            delete m_callbackServer;
            m_callbackServer = nullptr;
        }
    }
}

// namespace EVEIntelMonitor::SSO
// SSO
