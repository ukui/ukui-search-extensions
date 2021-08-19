#include "musicplugin.h"

using namespace Zeeker;

size_t MusicPlugin::uniqueSymbol = 0;

MusicPlugin::MusicPlugin(QObject *parent) : QObject(parent)
{
    QTranslator* translator = new QTranslator(this);
    try {
        if(!translator->load("/usr/share/ukui-search-plugins/musicPlugin/translations/" + QLocale::system().name())) throw - 1;
        QApplication::installTranslator(translator);
    } catch(...) {
        qDebug() << "Load translations file" << QLocale() << "failed!";
    }

    m_networkUtil = new NetworkUtil(m_infos);
    connect(m_networkUtil, &NetworkUtil::musicDownloadSuccess, this, &MusicPlugin::musicDownloadSuccess);
    connect(m_networkUtil, &NetworkUtil::musicDownloadFail, this, &MusicPlugin::musicDownloadFail);

    initDetailPage();


    QString cachePath = QDir::homePath() + "/.cache/ukui-search-musicPlugin/";
    QDir dir(cachePath);
    if (!dir.exists()) // create cache directory if not exists
    {
        QDir().mkpath(cachePath);
    } else { // clear previous cache contents if directory exists
        dir.setNameFilters(QStringList() << "*.*");
        dir.setFilter(QDir::Files);
        foreach(QString dirFile, dir.entryList())
        {
            dir.remove(dirFile);
        }
    }
}

MusicPlugin::~MusicPlugin()
{
    m_networkUtil->deleteLater();
}

QString MusicPlugin::getPluginName() {
    return tr("Music Plugin");
}

void MusicPlugin::KeywordSearch(QString keyword, DataQueue<ResultInfo> *searchResult) {

    if (keyword.trimmed().isEmpty()) return;
    ++MusicPlugin::uniqueSymbol;
    m_networkUtil->getList(keyword, 8, searchResult, MusicPlugin::uniqueSymbol);

}

QList<SearchPluginIface::Actioninfo> MusicPlugin::getActioninfo(int type) {
    return m_actionInfo;
}

void MusicPlugin::openAction(int actionkey, QString key, int type) {

}

QWidget *MusicPlugin::detailPage(const SearchPluginIface::ResultInfo &ri)
{
    m_currentActionKey = ri.actionKey;
    m_iconLabel->setPixmap(QPixmap(ri.description.at(2).value).scaled(180, 180));
    QFontMetrics fontMetrics = m_nameLabel->fontMetrics();
    QString showname = fontMetrics.elidedText(ri.name, Qt::ElideRight, 274); //当字体长度超过215时显示为省略号
    m_nameLabel->setText(QString(showname));
    if(QString::compare(showname, ri.name)) {
        m_nameLabel->setToolTip(ri.name);
    }

    m_artistsLabel2->setText(m_artistsLabel2->fontMetrics().elidedText(ri.description.at(0).value, Qt::ElideRight, m_artistsLabel2->width()));
    //m_artistsLabel2->setToolTip(m_currentActionKey);
    m_albumLabel2->setText(m_albumLabel2->fontMetrics().elidedText(ri.description.at(1).value, Qt::ElideRight, m_albumLabel2->width()));
    m_statusLabel->setText("");
    return m_detailPage;
}

void MusicPlugin::musicDownloadFail()
{
    m_statusLabel->setText(tr("Download failed. The music may be protected by copyright."));
}

void MusicPlugin::musicDownloadSuccess()
{
    QString suffix = QDir(QDir::homePath() + "/Music/").exists() ? "/Music/" : "/音乐/";
    m_statusLabel->setText(tr("Music successfully downloaded to ") + QDir::homePath() + suffix);
}

void MusicPlugin::initDetailPage()
{
    m_detailPage = new QWidget();
    m_detailPage->setFixedWidth(360);
    m_detailPage->setAttribute(Qt::WA_TranslucentBackground);
    m_detailLyt = new QVBoxLayout(m_detailPage);
    m_detailLyt->setContentsMargins(8, 0, 16, 0);
    m_iconLabel = new QLabel(m_detailPage);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->setFixedHeight(180);

    m_nameFrame = new QFrame(m_detailPage);
    m_nameFrameLyt = new QHBoxLayout(m_nameFrame);
    m_nameFrame->setLayout(m_nameFrameLyt);
    m_nameFrameLyt->setContentsMargins(8, 0, 0, 0);
    m_nameLabel = new QLabel(m_nameFrame);
    QFont font = m_nameLabel->font();
    font.setPointSize(14);
    m_nameLabel->setFont(font);
    m_nameLabel->setMaximumWidth(280);
    m_pluginLabel = new QLabel(m_nameFrame);
    m_pluginLabel->setEnabled(false);
    m_pluginLabel->setText(tr("Music"));
    m_nameFrameLyt->addWidget(m_nameLabel);
    m_nameFrameLyt->addStretch();
    m_nameFrameLyt->addWidget(m_pluginLabel);

    m_line_1 = new QFrame(m_detailPage);
    m_line_1->setLineWidth(0);
    m_line_1->setFixedHeight(1);
    m_line_1->setStyleSheet("QFrame{background: rgba(0,0,0,0.2);}");

    m_artistsFrame = new QFrame(m_detailPage);
    m_artistsFrameLyt = new QHBoxLayout(m_artistsFrame);
    m_artistsLabel1 = new QLabel(m_artistsFrame);
    m_artistsLabel2 = new QLabel(m_artistsFrame);
    m_artistsLabel1->setText(tr("Artists"));
    m_artistsLabel2->setFixedWidth(240);
    m_artistsLabel2->setAlignment(Qt::AlignRight);
    m_artistsFrameLyt->addWidget(m_artistsLabel1);
    m_artistsFrameLyt->addStretch();
    m_artistsFrameLyt->addWidget(m_artistsLabel2);

    m_albumFrame = new QFrame(m_detailPage);
    m_albumFrameLyt = new QHBoxLayout(m_albumFrame);
    m_albumLabel1 = new QLabel(m_albumFrame);
    m_albumLabel2 = new QLabel(m_albumFrame);
    m_albumLabel2->setAlignment(Qt::AlignRight);
    m_albumLabel1->setText(tr("Album"));
    m_albumLabel2->setFixedWidth(240);
    m_albumFrameLyt->addWidget(m_albumLabel1);
    m_albumFrameLyt->addStretch();
    m_albumFrameLyt->addWidget(m_albumLabel2);

    m_line_2 = new QFrame(m_detailPage);
    m_line_2->setLineWidth(0);
    m_line_2->setFixedHeight(1);
    m_line_2->setStyleSheet("QFrame{background: rgba(0,0,0,0.2);}");

    m_actionFrame = new QFrame(m_detailPage);
    m_actionFrameLyt = new QVBoxLayout(m_actionFrame);
    m_actionFrameLyt->setContentsMargins(8, 0, 0, 0);
    m_actionLabel1 = new ActionLabel(tr("Download"), m_currentActionKey, m_actionFrame);

    m_actionFrameLyt->addWidget(m_actionLabel1);
    m_actionFrame->setLayout(m_actionFrameLyt);

    m_statusLabel = new QLabel(m_detailPage);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setContentsMargins(8, 0, 8, 0);
    m_statusLabel->setFixedHeight(60);

    m_copyrightLabel = new QLabel(m_detailPage);
    QFont font2 = m_copyrightLabel->font();
    font2.setPointSize(8);
    m_copyrightLabel->setFont(font2);
    m_copyrightLabel->setWordWrap(true);
    m_copyrightLabel->setContentsMargins(8, 0, 8, 0);
    m_copyrightLabel->setEnabled(false);
    m_copyrightLabel->setText(tr("Owner of copyright: Netease Cloud Music"));
    m_copyrightLabel->setAlignment(Qt::AlignBottom);

    m_detailLyt->addSpacing(50);
    m_detailLyt->addWidget(m_iconLabel);
    m_detailLyt->addWidget(m_nameFrame);
    m_detailLyt->addWidget(m_line_1);
    m_detailLyt->addWidget(m_artistsFrame);
    m_detailLyt->addWidget(m_albumFrame);
    m_detailLyt->addWidget(m_line_2);
    m_detailLyt->addWidget(m_actionFrame);
    m_detailLyt->addWidget(m_statusLabel);
    m_detailLyt->addWidget(m_copyrightLabel);
    m_detailPage->setLayout(m_detailLyt);
    m_detailLyt->addStretch();

    connect(m_actionLabel1, &ActionLabel::actionTriggered, [ & ](){
        m_statusLabel->setText(tr("Downloading..."));
        m_networkUtil->downloadMusic(m_currentActionKey.toInt());
    });
}
