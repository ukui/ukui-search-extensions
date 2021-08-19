#ifndef NETWORKUTIL_H
#define NETWORKUTIL_H

#include <QObject>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QMap>
#include <QMessageBox>
#include <QThreadPool>
#include <QRunnable>

#include "musicInfo.h"
#include "ukui-search/search-plugin-iface.h"
#include "musicplugin.h"

namespace Zeeker{

class NetworkUtil : public QObject
{
    Q_OBJECT
public:
    explicit NetworkUtil(QVector<MusicInfo>& infos, QObject *parent = nullptr);
    void getList(QString name, int searchLimit, DataQueue<SearchPluginIface::ResultInfo>* searchResult, size_t uniqueSymbol);
    void getList2(QString name, int searchLimit, DataQueue<SearchPluginIface::ResultInfo>* searchResult, size_t uniqueSymbol);
    void downloadMusic(int idx);
    void musicFinish(QNetworkReply* reply, QString name);
    void listFinish(size_t uniqueSymbol);
    void imageFinish(size_t uniqueSymbol, int idx);

Q_SIGNALS:
    void musicDownloadSuccess();
    void musicDownloadFail();

private:
    QNetworkAccessManager m_manager;
    QFile* m_file;
    QString m_name;
    int m_searchLimit;
    QVector<MusicInfo> m_infos;
    DataQueue<SearchPluginIface::ResultInfo>* m_searchResult = nullptr; // Q: should not be filled by networkUtil? connect to musicPlugin instead?
    QThreadPool m_pool;
};

class Worker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    Worker(){this->setAutoDelete(true);}

public slots:
    void run() override {
        QThread::msleep(300);
        emit ready();
    }

signals:
    void ready();
};

}

#endif // NETWORKUTIL_H
