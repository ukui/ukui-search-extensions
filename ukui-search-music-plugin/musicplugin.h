/*
 * 1. The data source of this plugin is to from the public server of Netease
 * Cloud Music platform. this plugin is not responsible for the accuracy of
 * the data.
 *
 * 2. Copyright data may be generated during the use of this plugin. For the
 * data with copyright, the plugin does not own the ownership. In order to
 * avoid infringement, users must clear the data with copyright generated
 * during the use of this plugin within 24 hours.
 *
 * 3. Any direct, indirect, special, accidental or consequential damage of
 * any nature arising from the use of this plugin, including but not limited
 * to due to loss of goodwill, work stoppage, Compensation for computer
 * failure or damage caused by the failure, or any and all other commercial
 * damage or loss) is the responsibility of the user.
 */

#ifndef MUSICPLUGIN_H
#define MUSICPLUGIN_H

#include <QObject>
#include <QThreadPool>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QAction>
#include <QVector>
#include <QTranslator>
#include <QApplication>

#include "musicPlugin_global.h"
#include "search-plugin-iface.h"
#include "action-label.h"
#include "networkutil.h"
#include "musicInfo.h"

namespace Zeeker{

class NetworkUtil;

class MUSICPLUGIN_EXPORT MusicPlugin : public QObject, public SearchPluginIface
{
    friend class NetworkUtil;
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SearchPluginIface_iid FILE "common.json")
    Q_INTERFACES(Zeeker::SearchPluginIface)
public:
    MusicPlugin(QObject *parent = nullptr);
    ~MusicPlugin();

    PluginType pluginType() {return PluginType::SearchPlugin;}
    const QString name() {return tr("Online Music Search");}
    const QString description() {return tr("Music search.");}
    const QIcon icon() {return QIcon::fromTheme("folder");}
    void setEnable(bool enable) {m_enable = enable;}
    bool isEnable() {return m_enable;}

    QString getPluginName();
    void KeywordSearch(QString keyword,DataQueue<ResultInfo> *searchResult);
    QList<Actioninfo> getActioninfo(int type);
    void openAction(int actionkey, QString key, int type);
    QWidget *detailPage(const ResultInfo &ri);

public Q_SLOTS:
    void musicDownloadSuccess();
    void musicDownloadFail();


private:
    void initDetailPage();
    QWidget *m_detailPage = nullptr;
    QString m_currentActionKey;
    QVBoxLayout *m_detailLyt = nullptr;
    QLabel *m_iconLabel = nullptr;
    QFrame *m_nameFrame = nullptr;
    QHBoxLayout *m_nameFrameLyt = nullptr;
    QLabel *m_nameLabel = nullptr;
    QLabel *m_pluginLabel = nullptr;
    QFrame *m_line_1 = nullptr;
    QFrame *m_artistsFrame = nullptr;
    QLabel *m_artistsLabel1 = nullptr;
    QLabel *m_artistsLabel2 = nullptr;
    QHBoxLayout *m_artistsFrameLyt = nullptr;
    QFrame *m_albumFrame = nullptr;
    QLabel *m_albumLabel1 = nullptr;
    QLabel *m_albumLabel2 = nullptr;
    QHBoxLayout *m_albumFrameLyt = nullptr;

    QFrame *m_line_2 = nullptr;
    QFrame *m_actionFrame = nullptr;
    QVBoxLayout *m_actionFrameLyt = nullptr;
    ActionLabel *m_actionLabel1 = nullptr;
    QVBoxLayout * m_actionLyt = nullptr;

    QLabel *m_statusLabel = nullptr;
    QLabel *m_copyrightLabel = nullptr;

    static size_t uniqueSymbol;

    bool m_enable = true;
    QList<SearchPluginIface::Actioninfo> m_actionInfo;
    NetworkUtil* m_networkUtil;
    QVector<MusicInfo> m_infos;
};
}

#endif // MUSICPLUGIN_H
