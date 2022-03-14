# ukui-search-extensions
Set of extensions for ukui-search, a user-wide desktop search feature of UKUI desktop environment.
编译步骤：
1.安装或本地编译安装ukui-search最新版本（new-frontend分支或Debian分支）。
2.git clone https://github.com/ukui/ukui-search-extensions.git。
3.cd ukui-search-extensions;mkdir build;cd build;qmake..;make。

tcp搜索插件需要一个局域网服务端 
tcp搜索配置文件： 在~/.config/org.ukui/ukui-search下新建tcp-search-plugin.conf文件, 并添加配置例如： 
download_path=<文件下载路径> 
scheme=ftp 
user_name=<用户名> 
password=<密码> 
host= port=<端口> 
download_port=21
