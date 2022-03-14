#include "tcp-search-plugin.h"
#include <QTcpSocket>
#include <QBuffer>
#include <QDataStream>
#include <QHostAddress>
#include <QStandardPaths>
#include <QTranslator>
#include <QApplication>
#include <QTextCodec>
#include <QDir>
#include <QFileInfo>

#define MAIN_SETTINGS QDir::homePath() + "/.config/org.ukui/ukui-search/tcp-search-plugin.conf"
#define DOWNLOAD_PATH "download_path"
#define SCHEME "scheme"
#define USER_NAME "user_name"
#define PASSWORD "password"
#define HOST "host"
#define PORT "port"
#define DOWNLOAD_PORT "download_port"

using namespace UkuiSearch;

size_t TcpSearchPlugin::m_uniqueSymbol = 0;
QMutex TcpSearchPlugin::m_mutex;
QSettings TcpSearchPlugin::m_settings(MAIN_SETTINGS, QSettings::IniFormat);


TcpSearchPlugin::TcpSearchPlugin(QObject *parent) : QObject(parent)
{
    m_settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
    QTranslator *t = new QTranslator(this);
    QString path = "/usr/share/ukui-search-plugins/tcp-search-plugin/translations/"+QLocale::system().name();
    if(!t->load(path)) {
        qDebug() << "Load translations file failed,"<<"system().name and path:"<<QLocale::system().name() << path;
    }
    QApplication::installTranslator(t);

    SearchPluginIface::Actioninfo open { 0, tr("Download")};

    m_actionInfo << open;
    m_pool.setMaxThreadCount(1);
    m_pool.setExpiryTimeout(1000);
    m_searchTimer = new QTimer(this);
    m_searchTimer->setInterval(500);
    m_searchTimer->setSingleShot(true);
    connect(m_searchTimer, &QTimer::timeout, this, &TcpSearchPlugin::startSearch, Qt::DirectConnection);

    m_netAccessMananer = new QNetworkAccessManager(this);
    connect(m_netAccessMananer, &QNetworkAccessManager::finished, this, &TcpSearchPlugin::handelReplyFile);
    initDetailPage();
}

TcpSearchPlugin::~TcpSearchPlugin()
{

}

QString TcpSearchPlugin::getPluginName()
{
    return tr("File server search");
}

void TcpSearchPlugin::KeywordSearch(QString keyword, DataQueue<SearchPluginIface::ResultInfo> *searchResult)
{
    m_mutex.lock();
    ++m_uniqueSymbol;
    m_keyword = keyword;
    m_searchResult = searchResult;
    m_mutex.unlock();
    m_searchTimer->start();
}

void TcpSearchPlugin::stopSearch()
{
    m_mutex.lock();
    ++m_uniqueSymbol;
    m_mutex.unlock();
}

QList<SearchPluginIface::Actioninfo> TcpSearchPlugin::getActioninfo(int type)
{
    return m_actionInfo;
}

void TcpSearchPlugin::openAction(int actionkey, QString key, int type)
{
    switch (actionkey) {
    case 0:
    {
        QUrl url;
        m_settings.sync();
        url.setScheme(m_settings.value(SCHEME, "ftp").toString());
        url.setUserName(m_settings.value(USER_NAME, "default").toString());
        url.setPassword(m_settings.value(PASSWORD, "default").toString());
        url.setHost(m_settings.value(HOST, "default").toString());
        url.setPort(m_settings.value(DOWNLOAD_PORT, "21").toInt());
        url.setPath(key);
        qDebug() << "Download url:" << url;
        QNetworkRequest request(url);
        m_netAccessMananer->get(request);
        break;
    }
    default:
        break;
    }
}

QWidget *TcpSearchPlugin::detailPage(const SearchPluginIface::ResultInfo &ri)
{
    m_currentActionKey = ri.actionKey;
    m_iconLabel->setPixmap(ri.icon.pixmap(120, 120));
    QFontMetrics fontMetrics = m_nameLabel->fontMetrics();
    QString showname = fontMetrics.elidedText(ri.name, Qt::ElideRight, 215); //当字体长度超过215时显示为省略号
    m_nameLabel->setText(showname);
    m_nameLabel->setToolTip(ri.name);

    m_pluginLabel->setText(tr("File"));

    m_pathLabel2->setText(m_pathLabel2->fontMetrics().elidedText(m_currentActionKey, Qt::ElideRight, m_pathLabel2->width()));
    m_pathLabel2->setToolTip(m_currentActionKey);

    return m_detailPage;
}

void TcpSearchPlugin::initDetailPage()
{
    m_detailPage = new QWidget();
    m_detailPage->setFixedWidth(360);
    m_detailPage->setAttribute(Qt::WA_TranslucentBackground);
    m_detailLyt = new QVBoxLayout(m_detailPage);
    m_detailLyt->setContentsMargins(8, 0, 16, 0);
    m_iconLabel = new QLabel(m_detailPage);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->setFixedHeight(128);

    m_nameFrame = new QFrame(m_detailPage);
    m_nameFrameLyt = new QHBoxLayout(m_nameFrame);
    m_nameFrame->setLayout(m_nameFrameLyt);
    m_nameFrameLyt->setContentsMargins(8, 0, 0, 0);
    m_nameLabel = new QLabel(m_nameFrame);
    m_nameLabel->setMaximumWidth(280);
    m_pluginLabel = new QLabel(m_nameFrame);
    m_pluginLabel->setEnabled(false);
    m_nameFrameLyt->addWidget(m_nameLabel);
    m_nameFrameLyt->addStretch();
    m_nameFrameLyt->addWidget(m_pluginLabel);

    m_line_1 = new SeparationLine(m_detailPage);

    m_pathFrame = new QFrame(m_detailPage);
    m_pathFrameLyt = new QHBoxLayout(m_pathFrame);
    m_pathLabel1 = new QLabel(m_pathFrame);
    m_pathLabel2 = new QLabel(m_pathFrame);
    m_pathLabel1->setText(tr("Path"));
    m_pathLabel2->setFixedWidth(240);
    m_pathLabel2->setAlignment(Qt::AlignRight);
    m_pathFrameLyt->addWidget(m_pathLabel1);
    m_pathFrameLyt->addStretch();
    m_pathFrameLyt->addWidget(m_pathLabel2);

    m_line_2 = new SeparationLine(m_detailPage);

    m_actionFrame = new QFrame(m_detailPage);
    m_actionFrameLyt = new QVBoxLayout(m_actionFrame);
    m_actionFrameLyt->setContentsMargins(8, 0, 0, 0);
    m_actionLabel1 = new ActionLabel(tr("Download"), m_currentActionKey, m_actionFrame);

    m_actionFrameLyt->addWidget(m_actionLabel1);
    m_actionFrame->setLayout(m_actionFrameLyt);

    m_detailLyt->addSpacing(50);
    m_detailLyt->addWidget(m_iconLabel);
    m_detailLyt->addWidget(m_nameFrame);
    m_detailLyt->addWidget(m_line_1);
    m_detailLyt->addWidget(m_pathFrame);
    m_detailLyt->addWidget(m_line_2);
    m_detailLyt->addWidget(m_actionFrame);
    m_detailPage->setLayout(m_detailLyt);
    m_detailLyt->addStretch();

    connect(m_actionLabel1, &ActionLabel::actionTriggered, [ & ](){
        openAction(0, m_currentActionKey, 0);
    });
}

void TcpSearchPlugin::startSearch()
{
    qDebug() << "file server startSearch";
    m_mutex.lock();
    SearchWorker *worker = new SearchWorker(m_searchResult, m_keyword, m_uniqueSymbol);
    m_pool.start(worker);
    m_mutex.unlock();
}

void TcpSearchPlugin::handelReplyFile(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        /* 创建本地文件 */

        m_settings.sync();
        QString path;
        if(!m_settings.value(DOWNLOAD_PATH).isNull()){
            path = m_settings.value(DOWNLOAD_PATH).toString() + "/" +reply->url().fileName();
        } else {
            path =  QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/" + reply->url().fileName();
        }
        QFile file(path.toLocal8Bit().constData());
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.write(reply->readAll());
        file.close();
    } else {
        qWarning() << reply->errorString();
    }
    reply->deleteLater();
}

SearchWorker::SearchWorker(DataQueue<SearchPluginIface::ResultInfo> *searchResult, const QString &keyword, size_t uniqueSymbol)
{
    this->setAutoDelete(true);
    m_resultQueue = searchResult;
    m_keyword = keyword;
    m_uniqueSymbol = uniqueSymbol;
}

void SearchWorker::run()
{
    QTcpSocket tcpSocket;
    tcpSocket.connectToHost(TcpSearchPlugin::m_settings.value(HOST, "default").toString(), TcpSearchPlugin::m_settings.value(PORT, "default").toInt());
    tcpSocket.write(m_keyword.toLocal8Bit().data());
    tcpSocket.waitForReadyRead(10000);
    QStringList list;
    QBuffer buffer;
    QDataStream in(&buffer);
    buffer.setData(tcpSocket.readAll());
    buffer.open(QBuffer::ReadWrite);
    in >> list;
    for(QString path : list) {
        SearchPluginIface::ResultInfo ri;
        creatResultInfo(ri, path);
        TcpSearchPlugin::m_mutex.lock();
        if (m_uniqueSymbol == TcpSearchPlugin::m_uniqueSymbol) {
            m_resultQueue->enqueue(ri);
            TcpSearchPlugin::m_mutex.unlock();
            continue;
        } else {
            TcpSearchPlugin::m_mutex.unlock();
            return;
        }
    }
}

bool SearchWorker::creatResultInfo(SearchPluginIface::ResultInfo &ri, QString path)
{
    QFileInfo info(path);

    ri.icon = QIcon::fromTheme("ukui-love-symbolic");
    ri.name = info.fileName();
    ri.description = QVector<SearchPluginIface::DescriptionInfo>() \
            << SearchPluginIface::DescriptionInfo{tr("Path:"), path};
    ri.actionKey = path;
    ri.type = 0;
    return true;
}
