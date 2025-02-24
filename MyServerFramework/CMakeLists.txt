#project name
PROJECT(MyServerFramework)

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

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
MESSAGE("output path:${EXECUTABLE_OUTPUT_PATH}")

#确保在ADD_EXECUTABLE之前就添加了库目录
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../lib)

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

#head file path
INCLUDE_DIRECTORIES(
.;
Game;
Game/Character;
Game/Common;
Game/Component;
Game/Config;
Game/DataBase;
Game/DataBase/MySQL;
Game/DataBase/MySQL/Data;
Game/DataBase/MySQL/Table;
Game/DataBase/SQLite;
Game/Game;
Game/Socket;
Game/Socket/ClientServer;
Game/Socket/ServerClient;
Game/Socket/Struct;
Game/Utility;
../MyServerFramework_Frame/Frame;
../MyServerFramework_Frame/Frame/Character;
../MyServerFramework_Frame/Frame/Character/Component;
../MyServerFramework_Frame/Frame/Character/Component/DecisionTree;
../MyServerFramework_Frame/Frame/Character/Component/StateMachine;
../MyServerFramework_Frame/Frame/ClassPool;
../MyServerFramework_Frame/Frame/ClassPool/ArrayPool;
../MyServerFramework_Frame/Frame/ClassPool/Pool;
../MyServerFramework_Frame/Frame/ClassPool/VectorPool;
../MyServerFramework_Frame/Frame/CommandSystem;
../MyServerFramework_Frame/Frame/CommandSystem/CmdCharacterManager;
../MyServerFramework_Frame/Frame/CommandSystem/CmdGlobal;
../MyServerFramework_Frame/Frame/CommandSystem/CmdMySQLManager;
../MyServerFramework_Frame/Frame/CommandSystem/CmdNetServer;
../MyServerFramework_Frame/Frame/Common;
../MyServerFramework_Frame/Frame/Component;
../MyServerFramework_Frame/Frame/Component/BaseComponent;
../MyServerFramework_Frame/Frame/Component/ComponentMovableObject;
../MyServerFramework_Frame/Frame/Config;
../MyServerFramework_Frame/Frame/DataBase;
../MyServerFramework_Frame/Frame/DataBase/Excel;
../MyServerFramework_Frame/Frame/DataBase/MySQL;
../MyServerFramework_Frame/Frame/DataBase/SQLite;
../MyServerFramework_Frame/Frame/Dependency;
../MyServerFramework_Frame/Frame/Dependency/lz4;
../MyServerFramework_Frame/Frame/Dependency/md5;
../MyServerFramework_Frame/Frame/Dependency/sqlite3;
../MyServerFramework_Frame/Frame/EventSystem;
../MyServerFramework_Frame/Frame/Factory;
../MyServerFramework_Frame/Frame/InputSystem;
../MyServerFramework_Frame/Frame/KeyframeManager;
../MyServerFramework_Frame/Frame/KeyframeManager/Curve;
../MyServerFramework_Frame/Frame/Log;
../MyServerFramework_Frame/Frame/Math;
../MyServerFramework_Frame/Frame/Memory;
../MyServerFramework_Frame/Frame/Net;
../MyServerFramework_Frame/Frame/Net/Http;
../MyServerFramework_Frame/Frame/Net/TCPClient;
../MyServerFramework_Frame/Frame/Net/TCPServer;
../MyServerFramework_Frame/Frame/Net/UDPServer;
../MyServerFramework_Frame/Frame/Net/WebSocketServer;
../MyServerFramework_Frame/Frame/Param;
../MyServerFramework_Frame/Frame/Serialize;
../MyServerFramework_Frame/Frame/ServerFramework;
../MyServerFramework_Frame/Frame/StateManager;
../MyServerFramework_Frame/Frame/STL;
../MyServerFramework_Frame/Frame/Thread;
../MyServerFramework_Frame/Frame/TimePointSystem;
../MyServerFramework_Frame/Frame/TimePointSystem/TimePoint;
../MyServerFramework_Frame/Frame/TimeTaskSystem;
../MyServerFramework_Frame/Frame/UnitTest;
../MyServerFramework_Frame/Frame/Utility;
../MyServerFramework_Frame/Frame/Utility/FileStruct;
../MyServerFramework_Frame/Frame/Utility/MathStruct;
../MyServerFramework_Frame/Frame/Utility/TypeTraits;
IF(WINDOWS)
$(MYSQL_HOME)/include;
ENDIF(WINDOWS)
IF(LINUX)
/usr/include/mysql;
ENDIF(LINUX)
)

SET(SRC_LIST 
Game/UnityBuildGame.cpp;
)

#add executable file
ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_LIST})

#ADD_EXECUTABLE以后才能链接库
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lMyServerFramework_Frame)