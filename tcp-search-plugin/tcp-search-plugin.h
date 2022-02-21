#ifndef TCPSEARCHPLUGIN_H
#define TCPSEARCHPLUGIN_H

#include <QObject>
#include <QThreadPool>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QAction>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include "common.h"
#include <action-label.h>
#include <separation-line.h>
#include "tcp-search-plugin_global.h"
#include "ukui-search/search-plugin-iface.h"

namespace Zeeker{

class TCPSEARCHPLUGIN_EXPORT TcpSearchPlugin: public QObject, public SearchPluginIface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SearchPluginIface_iid FILE "common.json")
    Q_INTERFACES(Zeeker::SearchPluginIface)

public:
    friend class SearchWorker;
    TcpSearchPlugin(QObject *parent = nullptr);
    ~TcpSearchPlugin();

    PluginType pluginType() {return PluginType::SearchPlugin;}
    const QString name() {return "File server search";}
    const QString description() {return tr("File server search search.");}
    const QIcon icon() {return QIcon::fromTheme("folder");}
    void setEnable(bool enable) {m_enable = enable;}
    bool isEnable() {return m_enable;}

    QString getPluginName();
    void KeywordSearch(QString keyword,DataQueue<ResultInfo> *searchResult);
    void stopSearch();
    QList<Actioninfo> getActioninfo(int type);
    void openAction(int actionkey, QString key, int type);
    QWidget *detailPage(const ResultInfo &ri);

Q_SIGNALS:
    void needDownloadFile(QUrl url);
private:
    void initDetailPage();
    void startSearch();
    void handelReplyFile(QNetworkReply *reply);
    QString m_currentActionKey;
    QWidget *m_detailPage;
    QVBoxLayout *m_detailLyt = nullptr;
    QLabel *m_iconLabel = nullptr;
    QFrame *m_nameFrame = nullptr;
    QHBoxLayout *m_nameFrameLyt = nullptr;
    QLabel *m_nameLabel = nullptr;
    QLabel *m_pluginLabel = nullptr;
    SeparationLine *m_line_1 = nullptr;
    QFrame *m_pathFrame = nullptr;
    QLabel *m_pathLabel1 = nullptr;
    QLabel *m_pathLabel2 = nullptr;
    QHBoxLayout *m_pathFrameLyt = nullptr;
    QFrame *m_timeFrame = nullptr;
    QLabel *m_timeLabel1 = nullptr;
    QLabel *m_timeLabel2 = nullptr;
    QHBoxLayout *m_timeFrameLyt = nullptr;

    SeparationLine *m_line_2 = nullptr;
    QFrame *m_actionFrame = nullptr;
    QVBoxLayout *m_actionFrameLyt = nullptr;
    ActionLabel *m_actionLabel1 = nullptr;
    ActionLabel *m_actionLabel2 = nullptr;
    ActionLabel *m_actionLabel3 = nullptr;

    QVBoxLayout * m_actionLyt = nullptr;

    bool m_enable = true;
    QList<SearchPluginIface::Actioninfo> m_actionInfo;
    QThreadPool m_pool;
    QTimer *m_searchTimer = nullptr;
    QString m_keyword;
    DataQueue<ResultInfo> *m_searchResult = nullptr;
    QNetworkAccessManager *m_netAccessMananer = nullptr;
    static QSettings m_settings;
    static size_t m_uniqueSymbol;
    static QMutex m_mutex;

};

class SearchWorker : public QObject, public QRunnable
{
    Q_OBJECT
public:
    SearchWorker(DataQueue<SearchPluginIface::ResultInfo> *resultQueue, const QString& keyword, size_t uniqueSymbol);
protected:
    void run();
private:
    bool creatResultInfo(SearchPluginIface::ResultInfo &ri, QString path);
    DataQueue<SearchPluginIface::ResultInfo> *m_resultQueue = nullptr;
    size_t m_uniqueSymbol;
    QString m_keyword;

};
}
#endif // TCPSEARCHPLUGIN_H
