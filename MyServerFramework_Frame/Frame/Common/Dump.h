/**********************************************************
说明:
	1.编译时用了 -O1/2/3优化的,backtrace信息将不准确,建议用-O0
	2.本模块基于backtrace,backtrace_symbols,line2addr实现
	3.动态库编译加 -g -rdynamic:
		gcc test.c -g -rdynamic -fPIC -shared -o libtest.so
		或者先.o再.so
		g++ test.c -g -rdynamic -fPIC -shared -o libtest.o
		g++ libtest.o -shared -funwind-tables -rdynamic  -o libtest.so
	4.执行文件编译需要加 -g
	5.为了兼容c/c++,打印方式使用printf
使用:
	1.基于线程,模块init :debug_backtrace_init();工作于当前线程
	  可以修改该函数(有些信号是测试的时候用的)
	2.指针校验:ASSERT(p) 当p为NULL时,调用dump_backtrace(),打印函数调用栈
	3.dump_backtrace()  打印函数调用栈
	4.检测段错误,数组越界等问题,发生时调自动用dump_backtrace()
***********************************************************/
#pragma once

/*本模块的调试开关*/
/*1 放开下面注释*/
//#define BT_MOD_DEBUG_ON
/*2 或者,编译时候加 -D BT_MOD_DEBUG_ON*/

#include "Utility.h"

/*内部结构体*/
typedef struct 
{
	char not_care[4][32];
	char library_path[128];
	char* offset_start;
	char* offset_end;
}library_maps_t;

typedef struct
{
	char exe[128];
	char* offset;
}bt_t;

/*模块init*/
class MICRO_LEGEND_FRAME_API Dump
{
public:
	static void init();
protected:
#ifdef LINUX
	static string output_addrline(bt_t* bt);
	static bool dump_library_maps();
	static void signal_exit(int dunno);
#endif
public:
	static const int MAX_LIB_NUM = 20;
	static int max_lib;
	static library_maps_t lib[Dump::MAX_LIB_NUM];
};