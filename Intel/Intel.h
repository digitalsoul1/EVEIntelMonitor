/*
* Copyright (c) 2023 github.com/digitalsoul1
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
