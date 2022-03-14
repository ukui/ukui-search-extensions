# ukui-search-extensions
Set of extensions for ukui-search, a user-wide desktop search feature of UKUI desktop environment.
编译步骤：
1.安装或本地编译安装ukui-search最新版本（ukss-dev分支）。
2.git clone https://github.com/ukui/ukui-search-extensions.git。
3.cd ukui-search-extensions;mkdir build;cd build;qmake..;make。


tcp搜索配置文件：
在/home/zpf/.config/org.ukui/ukui-search下新建tcp-search-plugin.conf文件,
并添加配置例如：
download_path=/home/zpf/下载
scheme=ftp
user_name=zhangpengfei
password=k123123
host=172.17.128.59
port=8000
download_port=21
