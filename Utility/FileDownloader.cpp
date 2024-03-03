//
// Retrieved from https://wiki.qt.io/Download_Data_from_URL
//

#include "FileDownloader.h"

namespace EVEIntelMonitor::Utility {
    FileDownloader::FileDownloader(QUrl imageUrl, QObject *parent) :
            QObject(parent) {
        connect(
                &m_WebCtrl, SIGNAL (finished(QNetworkReply * )),
                this, SLOT (fileDownloaded(QNetworkReply * ))
        );

        qInfo() << "Downloading " << imageUrl;
        QNetworkRequest request(imageUrl);
        m_WebCtrl.get(request);
    }

    FileDownloader::~FileDownloader() = default;

    void FileDownloader::fileDownloaded(QNetworkReply *pReply) {
        m_DownloadedData = pReply->readAll();
        //emit a signal
        pReply->deleteLater();
        qInfo() << "Downloaded " << m_DownloadedData.size() << " bytes";
        emit downloaded();
    }

    QByteArray FileDownloader::downloadedData() const {
        return m_DownloadedData;
    }
} // EVEIntelMonitor
// Utility