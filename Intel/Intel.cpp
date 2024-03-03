#include "Intel.h"

#include <utility>


namespace EVEIntelMonitor::Intel {

    Intel::Intel(QString channelName, QDateTime &&intelTime, QString &&characterName, QString &&solarSystemName, short jumpsOut,
                 QString &&originalMessage, QObject *parent) : QObject(parent) {
        m_characterName = std::move(characterName);
        m_qdtIntelTime = std::move(intelTime);
        m_qsSolarSystemName = std::move(solarSystemName);
        m_jumpsOut = jumpsOut;
        m_qsOriginalMessage = std::move(originalMessage);
        m_qsChannelName = std::move(channelName);
    }

    QDateTime Intel::getIntelTime() const {
        return m_qdtIntelTime;
    }

    QString Intel::getSolarSystemName() const {
        return m_qsSolarSystemName;
    }

    short Intel::getJumpsOut() const {
        return m_jumpsOut;
    }

    QString Intel::getOriginalMessage() const {
        return m_qsOriginalMessage;
    }

    QString Intel::getCharacterName() const {
        return m_characterName;
    }

    bool Intel::operator==(const Intel &intel) {
        return m_qdtIntelTime == intel.m_qdtIntelTime &&
               m_characterName == intel.m_characterName &&
               m_qsSolarSystemName == intel.m_qsSolarSystemName &&
               m_jumpsOut == intel.m_jumpsOut &&
               m_qsOriginalMessage == intel.m_qsOriginalMessage;
    }

    QVariant Intel::operator[](int i) const
    {
        switch (i) {
            case 0:
                return m_qsChannelName;
            case 1:
                return m_qdtIntelTime.toString("hh:mm:ss");
            case 2:
                return m_characterName;
            case 3:
                return m_qsSolarSystemName;
            case 4:
                return m_jumpsOut;
            case 5:
                return m_qsOriginalMessage;
            default:
                return QVariant();
        }
    }

    QString Intel::getChannelName() const {
        return m_qsChannelName;
    }
}
