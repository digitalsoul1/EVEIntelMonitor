#pragma once
#include <qmutex.h>
#include <QQmlEngine>

#include "IntelTableView.h"
#include "IntelChannelsListView.h"

namespace EVEIntelMonitor {

    class MainWindowView : public QObject {
    Q_OBJECT

        QML_ELEMENT
    public:
        // constructors
        explicit MainWindowView(QObject *parent = nullptr);

        // getters
        Q_INVOKABLE QString getCharacterName() const;

        Q_INVOKABLE QString getCharacterLocation() const;

        Q_INVOKABLE IntelTableView *getIntelTableModel();
        Q_INVOKABLE IntelChannelsListView *getIntelChannelsListModel();

        void downloadCharacterPortrait();

        Q_INVOKABLE QUrl getCharacterPortraitUrl() const;

        Q_INVOKABLE short getTheraClosestConnectionJumps();
        Q_INVOKABLE QString getTheraClosestConnectionName();

        Q_INVOKABLE short getTurnurClosestConnectionJumps();
        Q_INVOKABLE QString getTurnurClosestConnectionName();

        // setters
        void setCharacterName(QString characterName);

        void setCharacterLocation(QString characterLocation);

        void setCharacterId(qulonglong characterID);

        void setTheraClostestConnection(short jumps, QString solarSystemName);
        void setTurnurClostestConnection(short jumps, QString solarSystemName);

        // destructor
        ~MainWindowView() override;
    public slots:
        void newIntelChannelAdded(const QString &channelName);

    private:
        QString m_qsCharacterName;
        QString m_qsTheraClosestConnectionName;
        QString m_qsTurnurClosestConnectionName;
        short m_sTheraClosestConnectionJumps;
        short m_sTurnurClosestConnectionJumps;
        QSharedPointer<QString> m_qsCharacterLocationPtr;
        QUrl m_qsCharacterPortraitUrl;
        qulonglong m_ullCharacterID{};
        IntelTableView *m_intelTableModel;
        IntelChannelsListView *m_intelChannelsListModel;
        QMutex m_qmCharacterLocationMutex;
        QMutex m_qmEveScoutMutex;

        void setCharacterPortraitUrl(const QUrl &characterPortraitUrl);


    signals:
        void characterLocationChanged();
        void newClosestTurnurConnection();
        void newClosestTheraConnection();
        void characterPortraitReady() const;
    };

}
