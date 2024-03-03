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

#include "Parser.h"
#include <QtConcurrentRun>
#include <QtCore/qtextstream.h>
#include <QDateTime>
#include "Monitor.h"
#include "QQueueSizeConstrained.h"
#include "RoutePlanner/RoutePlanner.h"

namespace EVEIntelMonitor::Intel {
    Parser::Parser(QObject* parent) : QObject(parent) {
        m_bParserThreadIsNeeded = true;
    }

    void Parser::startParserThread() {
        QMutexLocker locker(&m_qmParserThreadMutex);
        m_bParserThreadIsNeeded = true;
        locker.unlock();

        std::ignore = QtConcurrent::run([this]() {;
            qInfo() << "Parser thread started with thread id: << " << QThread::currentThreadId();
            auto documentsDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
            auto intelDir = documentsDir + "/EVE/logs/Chatlogs/";
            while (this->m_bParserThreadIsNeeded) {
                if (!m_qqIntelFileQueue.isEmpty()) {
                    const auto intelFile = this->safeDequeue();
                    QString fileName = intelFile->getFileName();
                    QFile file(intelDir + fileName);

                    // open the file to read
                    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                        qDebug() << "Could not open file " << fileName;
                        continue;
                    }
                    QQueueSizeConstrained<QString> qQueue(5);
                    QTextStream qTextStream(&file);

                    QString messageWordTemp{};
                    while (!qTextStream.atEnd()) {
                        QString line = qTextStream.readLine();

                        if (line.startsWith("\uFEFF"))
                            line.remove(0, 1);

                        if (!line.startsWith("["))
                            continue;

                        qQueue.enqueue(std::move(line));

                        while (!qQueue.isEmpty()) {
                            QString lineDequeued = qQueue.dequeue();
                            int messagePos;
                            QStringList splitLine = lineDequeued.split(" ");
                            QString date = splitLine[1];
                            QString time = splitLine[2];
                            QDateTime intelTime = QDateTime::fromString(date + " " + time, "yyyy.MM.dd hh:mm:ss");

                            if (intelTime < intelFile->getEnabledSince())
                                continue;

                            QString characterName = splitLine[4];

                            if (splitLine[5] == ">")
                                messagePos = 6;
                            else {
                                messagePos = 7;
                                characterName += " " + splitLine[5];
                            }

                            if(characterName == "EVE System")
                                continue;

                            QString message = splitLine.mid(messagePos).join(" ");

                            for (const auto&messageWord: message.split(" ")) {
                                if (messageWord.endsWith("*"))
                                    messageWordTemp = messageWord.left(messageWord.size() - 1);

                                if (RoutePlanner::getInstance()->solarSystemExists((messageWord.endsWith("*") ? messageWord.left(messageWord.size() - 1) : messageWord).toStdString())) {
                                    emit newIntel(intelFile->getChannelName(), intelTime,
                                                  messageWord.endsWith("*") ? messageWord.left(messageWord.size() - 1) : messageWord,
                                                  0,
                                                  message, characterName);
                                    break;
                                }
                            }
                        }
                    }
                    file.close();
                }

                QThread::msleep(500);
            }
            qInfo() << "Parser thread interrupted.";
        });
    }

    Parser::~Parser() {
        stopParserThread();
    }

    void Parser::stopParserThread() {
        QMutexLocker locker(&m_qmParserThreadMutex);
        if (m_bParserThreadIsNeeded)
            m_bParserThreadIsNeeded = false;
    }

    IntelChannel *Parser::safeDequeue() {
        QMutexLocker locker(&m_qmParserThreadMutex);
        if (!m_qqIntelFileQueue.isEmpty())
            return m_qqIntelFileQueue.dequeue();
        else
            return nullptr;
    }

    void Parser::safeEnqueue(const IntelChannel * intelFile) {
        QMutexLocker locker(&m_qmParserThreadMutex);
        m_qqIntelFileQueue.enqueue(const_cast<IntelChannel *>(intelFile));
    }
} // EVEIntelMonitor
// Intel
