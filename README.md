使用方式:

windows:

使用VisualStudio打开MyServerFramework.sln,编译,然后运行,启动服务器以后,再打开Unity,运行MyFramework

里面也有CMakeLists文件,可直接部署到linux上

并且有自动更新CMakeLists的工具,可以在windows上修改完代码,无论新增还是删除任何代码,任何文件夹,都可以直接更新CMakeLists.txt,正常的linux上编译

当然,前提是linux上需要安装对应的环境,如mysql,libevent,openssl,libssl
