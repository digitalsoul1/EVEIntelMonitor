#pragma once

#include <QString>
#include <QDateTime>

namespace EVEIntelMonitor::Intel {

    class Intel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString m_qsChannelName READ getChannelName CONSTANT)
    Q_PROPERTY(QDateTime m_qdtIntelTime READ getIntelTime CONSTANT)
    Q_PROPERTY(QString m_qsSolarSystemName READ getSolarSystemName CONSTANT)
    Q_PROPERTY(short m_jumpsOut READ getJumpsOut CONSTANT)
    Q_PROPERTY(QString m_qsOriginalMessage READ getOriginalMessage CONSTANT)
    Q_PROPERTY(QString m_qsCharacterName READ getCharacterName CONSTANT)

    
    public:
        explicit Intel(QString channelName, QDateTime&& intelTime,
              QString&& solarSystemName, QString&& characterName, short jumpsOut, QString &&originalMessage, QObject *parent = nullptr);
        [[nodiscard]] QString getChannelName() const;

        [[nodiscard]] QDateTime getIntelTime() const;

        [[nodiscard]] QString getSolarSystemName() const;

        [[nodiscard]] short getJumpsOut() const;

        [[nodiscard]] QString getOriginalMessage() const;

        [[nodiscard]] QString getCharacterName() const;

        bool operator==(const Intel &intel);
        QVariant operator[](int i) const;

    private:
        QDateTime m_qdtIntelTime;
        QString m_characterName;
        QString m_qsSolarSystemName;
        short m_jumpsOut;
        QString m_qsOriginalMessage;
        QString m_qsChannelName;
    };

}
