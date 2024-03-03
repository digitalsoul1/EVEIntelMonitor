//
// Created by Lukasz Klepacki on 08/11/2023.
//

#ifndef HIVEPROTECTOR_FILEDOWNLOADER_H
#define HIVEPROTECTOR_FILEDOWNLOADER_H
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace EVEIntelMonitor::Utility {

    class FileDownloader : public QObject {
    Q_OBJECT

    public:
        explicit FileDownloader(QUrl imageUrl, QObject *parent = nullptr);

        ~FileDownloader() override;

        [[nodiscard]] QByteArray downloadedData() const;

    signals:
        void downloaded();

    private slots:
        void fileDownloaded(QNetworkReply *pReply);

    private:
        QNetworkAccessManager m_WebCtrl;
        QByteArray m_DownloadedData;
    };

} // EVEIntelMonitor
// Utility

#endif //HIVEPROTECTOR_FILEDOWNLOADER_H
