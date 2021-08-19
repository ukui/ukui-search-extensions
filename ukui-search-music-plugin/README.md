# Online music search plugin for ukui-search

A plugin for ukui-search implementing online music search functionality based on Netease Cloud Music API.

## Usage

The plugin searches music by input keyword and shows the search result (album image, artists and album name) on ukui-search. The search result is based on data from Netease Cloud Music. If the music is downloadable, the user can download the music to `home/$username/Music` or `home/$username/音乐` according to the system language by clicking "download". The user interface will prompt whether download is successful.

## Install

`cd` to the repository directory and type in the following commands:

```
debuild
sudo dpkg -i ../libmusicplugin_0.0.1_amd64.deb 
```

If everything works correctly, open the ukui-search tool and type in the keyword you want to search for, then you will see the online music search result:

<img src="https://s3.bmp.ovh/imgs/2021/08/a3af5a9ce7b74ea3.png" style="zoom: 67%;" />

## Common questions

If the plugin does not function normally, you can check with following steps:

+ `libmusicplugin.so` has been correctly installed to `usr/lib/*/ukui-search-plugins/`
+ The version of libmusicplugin meets the requirement of ukui-search
+ ukui-search has loaded plugins by `PluginManger`

If you are using Chinese as system language but the plugin does not show Chinese correctly, please check whether `zh_CN.qm` has been correctly installed to `usr/share/ukui-search-plugins/musicPlugin/translations`

## Copyright

The data source of this plugin is from the public server of Netease Cloud Music platform. this plugin is not responsible for the accuracy of the data.

Copyright data may be generated during the use of this plugin. For the data with copyright, the plugin does not own the ownership. In order to avoid infringement, users must clear the data with copyright generated during the use of this plugin within 24 hours. 

Any direct, indirect, special, accidental or consequential damage of any nature arising from the use of this plugin, (including but not limited to due to loss of goodwill, work stoppage, Compensation for computer failure or damage caused by the failure, or any and all other commercial damage or loss) is the responsibility of the user.