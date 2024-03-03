//
// Created by Lukasz Klepacki on 10/10/2023.
//

#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include "CharacterLocation.h"
#include "../Exceptions/RefreshTokenInvalidException.h"


namespace EVEIntelMonitor::SSO {
    unsigned long CharacterLocation::getLocation(unsigned int characterId) {
        // Check if access token expired
        if (m_pManager->accessTokenExpired(characterId)) {
            qDebug() << "Access token expired, refreshing";
            try {
                refreshToken(characterId);
            } catch (Exceptions::RefreshTokenInvalidException &e) {
                qDebug() << "Refresh token invalid, deleting character";
                throw;
            }
        }

        QNetworkAccessManager manager;
        QNetworkRequest request(QUrl("https://esi.evetech.net/latest/characters/" + QString::number(characterId) + "/location/"));
        request.setRawHeader("Authorization", "Bearer " + m_pManager->getAccessToken(characterId).toLatin1());

        QScopedPointer<QNetworkReply> reply(manager.get(request));
        QEventLoop loop;
        connect(reply.data(), &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();



        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            QJsonObject jsonObject = jsonDoc.object();

            // Check if the json returned contains all data we need
            if (jsonObject.contains("solar_system_id")) {
                // Get the data from the json
                unsigned long solarSystemId = jsonObject["solar_system_id"].toInt();
                return solarSystemId;
            }
        }

        return 0;
    }

    CharacterLocation::CharacterLocation(const Auth *auth, const SSO::Manager *manager, QObject *parent) : QObject(parent) {
        m_pAuth = auth;
        m_pManager = manager;
    }

    void CharacterLocation::refreshToken(unsigned int characterId) {
        QNetworkAccessManager manager;
        QUrlQuery postData;
        postData.addQueryItem("grant_type", "refresh_token");
        postData.addQueryItem("refresh_token", m_pManager->getRefreshToken(characterId));
        postData.addQueryItem("client_id", this->getAuth()->m_qsClientID);
        QNetworkRequest request(QUrl("https://login.eveonline.com/v2/oauth/token"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setRawHeader("Host", "login.eveonline.com");

        QScopedPointer reply(manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8()));
        QEventLoop loop;
        connect(reply.data(), &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            QJsonObject jsonObject = jsonDoc.object();

            // Check if the json returned contains all data we need
            if (jsonObject.contains("access_token") && jsonObject.contains("refresh_token") &&
                jsonObject.contains("expires_in")) {
                // Get the data from the json
                QString accessToken = jsonObject["access_token"].toString();
                QString refreshToken = jsonObject["refresh_token"].toString();
                unsigned long expiresIn = jsonObject["expires_in"].toInt();

                // Store the data in the manager
                m_pManager->updateAccessToken(characterId, accessToken, refreshToken, expiresIn);
                return;
            }
        }

        // if we got this far, something went wrong
        qWarning() << "Error refreshing token: ";
        qWarning() << this->getAuth()->errorResponseToHumanReadable(QJsonDocument::fromJson(reply->readAll()).object());

        throw Exceptions::RefreshTokenInvalidException(const_cast<char *>("Error refreshing token"));
    }

   Auth * CharacterLocation::getAuth()  {
        QMutexLocker locker(&m_qmMutex);
        if(m_pAuth == nullptr)
            throw std::runtime_error("Auth pointer is null");

        return const_cast<Auth *>(m_pAuth);
    }
} // EVEIntelMonitor
// SSO
