#project name
PROJECT(MicroLegend_Server)

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
Game/Account;
Game/Achivement;
Game/Achivement/Achivement;
Game/Activity;
Game/Activity/GuildTerritory;
Game/Activity/MonsterWar;
Game/Activity/ShabakeBattle;
Game/Activity/WorldBoss;
Game/Auction;
Game/Character;
Game/Character/Component;
Game/Character/Component/Behaviour;
Game/Character/Component/Behaviour/EnvironmentBehaviour;
Game/Character/Component/DecisionTree;
Game/Character/Component/DecisionTree/Action;
Game/Character/Component/DecisionTree/Condition;
Game/Character/Component/DecisionTree/DecisionTreeBuilder;
Game/Character/Component/DecisionTree/DecisionTreeBuilder/DTBuilderMonster;
Game/Character/Component/Skill;
Game/Character/Component/Skill/Bullet;
Game/Character/Component/Skill/Environment;
Game/Character/Component/Skill/Monster;
Game/Character/Component/Skill/Player;
Game/Character/Component/Skill/Player/Fighter;
Game/Character/Component/Skill/Player/Mage;
Game/Character/Component/Skill/Player/Taoist;
Game/Character/Component/StateMachine;
Game/Character/Component/StateMachine/Action;
Game/Character/Component/StateMachine/Behaviour;
Game/Character/Component/StateMachine/CharacterProperty;
Game/Character/Component/StateMachine/Check;
Game/Character/Component/StateMachine/CommonBuff;
Game/Character/Component/StateMachine/EquipStrengthBuff;
Game/Character/Component/StateMachine/EquipStrengthLevel;
Game/Character/Component/StateMachine/ForeverProperty;
Game/Character/Component/StateMachine/LessHPMore;
Game/Character/Component/StateMachine/MoreMPMore;
Game/Character/Component/StateMachine/PlayerLevelBuff;
Game/Character/Component/StateMachine/RangeBuff;
Game/Character/Component/StateMachine/RangePlayerCount;
Game/Character/Component/StateMachine/SkillBuff;
Game/Character/Component/StateMachine/StateGroup;
Game/Character/Component/StateMachine/Trigger;
Game/ChatSystem;
Game/ClassPool;
Game/CommandSystem;
Game/CommandSystem/CmdAuctionSystem;
Game/CommandSystem/CmdCharacter;
Game/CommandSystem/CmdCharacterManager;
Game/CommandSystem/CmdExchangeManager;
Game/CommandSystem/CmdGuildManager;
Game/CommandSystem/CmdMailManager;
Game/CommandSystem/CmdMonsterManager;
Game/CommandSystem/CmdMySQLDataBase;
Game/CommandSystem/CmdNetServer;
Game/CommandSystem/CmdNPCManager;
Game/CommandSystem/CmdObjectItemManager;
Game/CommandSystem/CmdOfferRewardManager;
Game/Common;
Game/Component;
Game/ConditionManager;
Game/ConditionManager/Condition;
Game/Config;
Game/DataBase;
Game/DataBase/MySQL;
Game/DataBase/MySQL/Data;
Game/DataBase/MySQL/Table;
Game/DataBase/SQLite;
Game/DataBase/SQLite/Data;
Game/DataBase/SQLite/Struct;
Game/DataBase/SQLite/Table;
Game/DebugSystem;
Game/EnvironmentSystem;
Game/EventSystem;
Game/EventSystem/Event;
Game/Exchange;
Game/Factory;
Game/FunctionParam;
Game/FunctionParam/MySQLParam;
Game/Game;
Game/GameVerifySystem;
Game/Guild;
Game/LoginGate;
Game/LotteryTicket;
Game/Mail;
Game/Mission;
Game/Monster;
Game/MySQLCheck;
Game/NPC;
Game/ObjectItem;
Game/ObjectItem/Consumable;
Game/ObjectItem/Item;
Game/OfferReward;
Game/PKArenaSystem;
Game/PKArenaSystem/PKArenaSingle;
Game/PKMode;
Game/PKMode/MonsterSearch;
Game/PKMode/PKMode;
Game/QQMail;
Game/RankSystem;
Game/RankSystem/Rank;
Game/RedEnvelope;
Game/Scene;
Game/Scene/MonsterGenerate;
Game/Scene/SceneCopy;
Game/SceneTriggerManager;
Game/SceneTriggerManager/Modifier;
Game/SceneTriggerManager/SceneTrigger;
Game/SeasonSystem;
Game/ServerGlobal;
Game/ShoppingMallSystem;
Game/Socket;
Game/Socket/ClientServer;
Game/Socket/ClientServer/Account;
Game/Socket/ClientServer/Achivement;
Game/Socket/ClientServer/Auction;
Game/Socket/ClientServer/BlackList;
Game/Socket/ClientServer/Buy;
Game/Socket/ClientServer/Chat;
Game/Socket/ClientServer/EnterMap;
Game/Socket/ClientServer/Exchange;
Game/Socket/ClientServer/Friend;
Game/Socket/ClientServer/Guild;
Game/Socket/ClientServer/LotteryTicket;
Game/Socket/ClientServer/Mail;
Game/Socket/ClientServer/Mission;
Game/Socket/ClientServer/OfferReward;
Game/Socket/ClientServer/PKArena;
Game/Socket/ClientServer/QQ;
Game/Socket/ClientServer/RedEnvelope;
Game/Socket/ClientServer/SecondaryPassword;
Game/Socket/ClientServer/Shabake;
Game/Socket/ClientServer/Team;
Game/Socket/ClientServer/TerritoryActivity;
Game/Socket/ClientServer/TreasureHouse;
Game/Socket/ClientServer/Warehouse;
Game/Socket/ClientServer/WuJinJianTa;
Game/Socket/GateClientServer;
Game/Socket/GateServerClient;
Game/Socket/MailClientServer;
Game/Socket/MailServerClient;
Game/Socket/ServerClient;
Game/Socket/ServerClient/Account;
Game/Socket/ServerClient/Achivement;
Game/Socket/ServerClient/Auction;
Game/Socket/ServerClient/BlackList;
Game/Socket/ServerClient/Chat;
Game/Socket/ServerClient/Exchange;
Game/Socket/ServerClient/Friend;
Game/Socket/ServerClient/Guild;
Game/Socket/ServerClient/ItemProperty;
Game/Socket/ServerClient/LotteryTicket;
Game/Socket/ServerClient/Mail;
Game/Socket/ServerClient/Mission;
Game/Socket/ServerClient/NPC;
Game/Socket/ServerClient/OfferReward;
Game/Socket/ServerClient/PKArena;
Game/Socket/ServerClient/QQ;
Game/Socket/ServerClient/RedEnvelope;
Game/Socket/ServerClient/SecondaryPassword;
Game/Socket/ServerClient/ShabakeBattle;
Game/Socket/ServerClient/ShoppingMall;
Game/Socket/ServerClient/Team;
Game/Socket/ServerClient/TerritoryActivity;
Game/Socket/ServerClient/TreasureHouse;
Game/Socket/ServerClient/Warehouse;
Game/Socket/ServerClient/WorldBoss;
Game/Socket/ServerClient/WuJinJianTa;
Game/Socket/Struct;
Game/TeamManager;
Game/TimeTask;
Game/TreasureHouse;
Game/Utility;
visual studio 2022Templates;
visual studio 2022Templates/ItemTemplates;
visual studio 2022Templates/ProjectTemplates;
../MicroLegend_Server_Frame/Frame;
../MicroLegend_Server_Frame/Frame/Character;
../MicroLegend_Server_Frame/Frame/Character/Component;
../MicroLegend_Server_Frame/Frame/Character/Component/DecisionTree;
../MicroLegend_Server_Frame/Frame/Character/Component/StateMachine;
../MicroLegend_Server_Frame/Frame/ClassPool;
../MicroLegend_Server_Frame/Frame/ClassPool/ArrayPool;
../MicroLegend_Server_Frame/Frame/ClassPool/Pool;
../MicroLegend_Server_Frame/Frame/ClassPool/VectorPool;
../MicroLegend_Server_Frame/Frame/CommandSystem;
../MicroLegend_Server_Frame/Frame/CommandSystem/CmdCharacterManager;
../MicroLegend_Server_Frame/Frame/CommandSystem/CmdGlobal;
../MicroLegend_Server_Frame/Frame/CommandSystem/CmdMySQLManager;
../MicroLegend_Server_Frame/Frame/CommandSystem/CmdNetServer;
../MicroLegend_Server_Frame/Frame/Common;
../MicroLegend_Server_Frame/Frame/Component;
../MicroLegend_Server_Frame/Frame/Component/BaseComponent;
../MicroLegend_Server_Frame/Frame/Component/ComponentMovableObject;
../MicroLegend_Server_Frame/Frame/Config;
../MicroLegend_Server_Frame/Frame/DataBase;
../MicroLegend_Server_Frame/Frame/DataBase/Excel;
../MicroLegend_Server_Frame/Frame/DataBase/MySQL;
../MicroLegend_Server_Frame/Frame/DataBase/SQLite;
../MicroLegend_Server_Frame/Frame/Dependency;
../MicroLegend_Server_Frame/Frame/Dependency/lz4;
../MicroLegend_Server_Frame/Frame/Dependency/md5;
../MicroLegend_Server_Frame/Frame/Dependency/sqlite3;
../MicroLegend_Server_Frame/Frame/EventSystem;
../MicroLegend_Server_Frame/Frame/Factory;
../MicroLegend_Server_Frame/Frame/InputSystem;
../MicroLegend_Server_Frame/Frame/KeyframeManager;
../MicroLegend_Server_Frame/Frame/KeyframeManager/Curve;
../MicroLegend_Server_Frame/Frame/Log;
../MicroLegend_Server_Frame/Frame/Math;
../MicroLegend_Server_Frame/Frame/Memory;
../MicroLegend_Server_Frame/Frame/Net;
../MicroLegend_Server_Frame/Frame/Net/Http;
../MicroLegend_Server_Frame/Frame/Net/TCPClient;
../MicroLegend_Server_Frame/Frame/Net/TCPServer;
../MicroLegend_Server_Frame/Frame/Net/UDPServer;
../MicroLegend_Server_Frame/Frame/Net/WebSocketServer;
../MicroLegend_Server_Frame/Frame/Param;
../MicroLegend_Server_Frame/Frame/Serialize;
../MicroLegend_Server_Frame/Frame/ServerFramework;
../MicroLegend_Server_Frame/Frame/StateManager;
../MicroLegend_Server_Frame/Frame/STL;
../MicroLegend_Server_Frame/Frame/Thread;
../MicroLegend_Server_Frame/Frame/TimePointSystem;
../MicroLegend_Server_Frame/Frame/TimePointSystem/TimePoint;
../MicroLegend_Server_Frame/Frame/TimeTaskSystem;
../MicroLegend_Server_Frame/Frame/UnitTest;
../MicroLegend_Server_Frame/Frame/Utility;
../MicroLegend_Server_Frame/Frame/Utility/FileStruct;
../MicroLegend_Server_Frame/Frame/Utility/MathStruct;
../MicroLegend_Server_Frame/Frame/Utility/TypeTraits;
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
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lMicroLegend_Server_Frame)