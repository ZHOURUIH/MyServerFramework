#include "FrameHeader.h"

// 全局 backtrace state
#ifdef LINUX
struct backtrace_state* CrashHandler::bt_state = nullptr;
#endif

void CrashHandler::init()
{
#ifdef LINUX
    // 初始化 libbacktrace
    bt_state = backtrace_create_state(nullptr, 0, nullptr, nullptr);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = crash_handler;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGABRT, &sa, nullptr);
    sigaction(SIGFPE, &sa, nullptr);
    sigaction(SIGILL, &sa, nullptr);
    sigaction(SIGBUS, &sa, nullptr);
    // 先创建Crash目录
    createDir(FrameDefine::CRASH_PATH);
#endif
}

#ifdef LINUX
void CrashHandler::safe_write(int fd, const char* str)
{
    write(fd, str, strlen(str));
}

// 打印 backtrace 回调
int CrashHandler::on_backtrace(void* data, uintptr_t pc, const char* filename, int lineno, const char* function)
{
    int fd = *(int*)data;
    char buf[1024];
    snprintf(buf, sizeof(buf), "0x%lx: %s at %s:%d\n",
        pc,
        function ? function : "??",
        filename ? filename : "??",
        lineno);
    safe_write(fd, buf);
    return 0;
}

void CrashHandler::on_backtrace_error(void* data, const char* msg, int errnum)
{
    const int fd = *(int*)data;
    char buf[512]{0};
    snprintf(buf, sizeof(buf), "Backtrace error: %s\n", msg);
    safe_write(fd, buf);
}

// 崩溃处理函数
void CrashHandler::crash_handler(int sig, siginfo_t* info, void* ucontext)
{
    int fd = open((FrameDefine::CRASH_PATH + getTime() + ".txt").c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
    char buf[512]{0};

    snprintf(buf, sizeof(buf), "\n=== CRASH ===\nSignal: %d, si_errno: %d, si_code: %d, si_addr: %p\n", sig, info->si_errno, info->si_code, info->si_addr);
    safe_write(fd, buf);

    safe_write(fd, "Backtrace:\n");
    if (bt_state != nullptr)
    {
        backtrace_full(bt_state, 0, on_backtrace, on_backtrace_error, &fd);
    }

    _exit(128 + sig);
}
#endif