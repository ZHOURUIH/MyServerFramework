#project name
PROJECT(MicroLegend_Server_Frame)

#minimum_version
cmake_minimum_required(VERSION 2.8)

MESSAGE("\nstart cmake ${PROJECT_NAME}")

#设置为Release模式
SET(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
#release模式下关闭优化,开启gdb调试
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O0 -Wall -g")
#release模式下开启全部优化,关闭gdb调试
#set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)  
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)  
IF(COMPILER_SUPPORTS_CXX17)  
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
	MESSAGE("c++17")
ELSEIF(COMPILER_SUPPORTS_CXX0X)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	MESSAGE("c++0x")
ELSE()  
    MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")  
ENDIF() 

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib)
MESSAGE("output path:${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

#确保在add_library之前就添加了库目录
LINK_DIRECTORIES(/usr/lib/mysql/plugin)
LINK_DIRECTORIES(/usr/lib/x86_64-linux-gnu)

#添加平台宏
IF(WIN32)
MESSAGE("this is windows!")
ADD_DEFINITIONS(-DWINDOWS)
ENDIF(WIN32)

IF(UNIX)
MESSAGE("this is linux!")
ADD_DEFINITIONS(-DLINUX)
ENDIF(UNIX)

ADD_DEFINITIONS(-D_MYSQL)
ADD_DEFINITIONS(-DMICROLEGENDSERVERFRAME_EXPORTS)

#head file path
INCLUDE_DIRECTORIES(
.;
Frame;
Frame/Character;
Frame/Character/Component;
Frame/Character/Component/DecisionTree;
Frame/Character/Component/StateMachine;
Frame/ClassPool;
Frame/ClassPool/ArrayPool;
Frame/ClassPool/Pool;
Frame/ClassPool/VectorPool;
Frame/CommandSystem;
Frame/CommandSystem/CmdCharacterManager;
Frame/CommandSystem/CmdGlobal;
Frame/CommandSystem/CmdMySQLManager;
Frame/CommandSystem/CmdNetServer;
Frame/Common;
Frame/Component;
Frame/Component/BaseComponent;
Frame/Component/ComponentMovableObject;
Frame/Config;
Frame/DataBase;
Frame/DataBase/Excel;
Frame/DataBase/MySQL;
Frame/DataBase/SQLite;
Frame/Dependency;
Frame/Dependency/lz4;
Frame/Dependency/md5;
Frame/Dependency/sqlite3;
Frame/EventSystem;
Frame/Factory;
Frame/InputSystem;
Frame/KeyframeManager;
Frame/KeyframeManager/Curve;
Frame/Log;
Frame/Math;
Frame/Memory;
Frame/Net;
Frame/Net/Http;
Frame/Net/TCPClient;
Frame/Net/TCPServer;
Frame/Net/UDPServer;
Frame/Net/WebSocketServer;
Frame/Param;
Frame/Serialize;
Frame/ServerFramework;
Frame/StateManager;
Frame/STL;
Frame/Thread;
Frame/TimePointSystem;
Frame/TimePointSystem/TimePoint;
Frame/TimeTaskSystem;
Frame/UnitTest;
Frame/Utility;
Frame/Utility/FileStruct;
Frame/Utility/MathStruct;
Frame/Utility/TypeTraits;
IF(WINDOWS)
$(MYSQL_HOME)/include;
ENDIF(WINDOWS)
IF(LINUX)
/usr/include/mysql;
ENDIF(LINUX)
)

SET(SRC_LIST 
Frame/Dependency/sqlite3/sqlite3.c;
Frame/Dependency/lz4/lz4.c;
Frame/Dependency/lz4/lz4file.c;
Frame/Dependency/lz4/lz4frame.c;
Frame/Dependency/lz4/lz4hc.c;
Frame/Dependency/lz4/xxhash.c;
Frame/UnityBuildFrame.cpp;
)

#add output so file
add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

#add_library以后才能链接静态库
TARGET_LINK_LIBRARIES(${PROJECT_NAME} libmysqlclient.a)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -ldl)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lpthread)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lz)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lssl)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lcrypto)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lzstd)