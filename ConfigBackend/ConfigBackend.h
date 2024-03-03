#pragma once

#include <QObject>
#include <QSettings>
#include <QMutexLocker>

namespace EVEIntelMonitor {
    class ConfigBackend : public QObject {
    Q_OBJECT
    public:
        static ConfigBackend *getInstance(QObject *parent = nullptr);
        void setValue(const QString& key, const QVariant& value);
        QVariant getValue(const QString& key, const QVariant& defaultValue = QVariant());
        QStringList getGroups(const QString& group);
        void removeGroup(const QString& group);
        [[nodiscard]] long long groupCount(const QString &key);
        [[maybe_unused]] void wipe();
    private:
        explicit ConfigBackend(QObject *parent = nullptr);
        static ConfigBackend *m_pInstance;
        QSettings *m_pSettings;
        QMutex m_mutex;
    };


}
