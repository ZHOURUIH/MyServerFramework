#ifdef WINDOWS
#include "FrameHeader.h"

// ============================================================
// StackTest — Stack<T> 全方法深度测试
// legacy STL aggregate test 中 Stack 只有极少量 push/pop 测试（legacy STL aggregate test 不含 Stack）
// ContainerTest 中也无 Stack 测试
// Stack 接口：push / pop / top / size
// 注意：框架 Stack<T> 是 std::stack<T> 的薄包装，无 isEmpty/clear
// ============================================================

#define STE_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("StackTest FAILED: ") + (msg)); }
#define STE_EQ(a, b, msg) \
    if ((a) != (b)) { ERROR(string("StackTest FAILED: ") + (msg) + \
        " | got=" + IToS((int)(a))); }

// ============================================================
// push / size
// ============================================================
void StackTest::testStack_PushSize_Int()
{
    Stack<int> s;
    STE_EQ(s.size(), 0, "init size==0");
    s.push(10);
    STE_EQ(s.size(), 1, "push 1 size==1");
    s.push(20);
    STE_EQ(s.size(), 2, "push 2 size==2");
    s.push(30);
    STE_EQ(s.size(), 3, "push 3 size==3");
}

void StackTest::testStack_PushSize_String()
{
    Stack<string> s;
    s.push("alpha");
    s.push("beta");
    s.push("gamma");
    STE_EQ(s.size(), 3, "string push 3 size==3");
}

void StackTest::testStack_PushMany()
{
    Stack<int> s;
    for (int i = 0; i < 100; ++i)
    {
        s.push(i);
    }
    STE_EQ(s.size(), 100, "push 100 size==100");
}

// ============================================================
// pop — LIFO 顺序
// ============================================================
void StackTest::testStack_PopOrder_Int()
{
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    STE_EQ(s.pop(), 3, "pop 1st==3");
    STE_EQ(s.pop(), 2, "pop 2nd==2");
    STE_EQ(s.pop(), 1, "pop 3rd==1");
    STE_EQ(s.size(), 0, "pop all size==0");
}

void StackTest::testStack_PopOrder_String()
{
    Stack<string> s;
    s.push("first");
    s.push("second");
    s.push("third");
    STE_ASSERT(s.pop() == "third", "pop string 1st==third");
    STE_ASSERT(s.pop() == "second", "pop string 2nd==second");
    STE_ASSERT(s.pop() == "first", "pop string 3rd==first");
}

void StackTest::testStack_PopOrder_Float()
{
    Stack<float> s;
    s.push(1.0f);
    s.push(2.5f);
    s.push(3.14f);
    float v = s.pop();
    STE_ASSERT(fabsf(v - 3.14f) < 1e-4f, "pop float 1st==3.14");
    v = s.pop();
    STE_ASSERT(fabsf(v - 2.5f) < 1e-4f, "pop float 2nd==2.5");
    v = s.pop();
    STE_ASSERT(fabsf(v - 1.0f) < 1e-4f, "pop float 3rd==1.0");
}

void StackTest::testStack_PopLarge()
{
    Stack<int> s;
    const int N = 500;
    for (int i = 0; i < N; ++i)
    {
        s.push(i);
    }
    FOR_INVERSE(N)
    {
        STE_EQ(s.pop(), i, "pop large LIFO order");
    }
    STE_EQ(s.size(), 0, "pop large all gone size==0");
}

// ============================================================
// top — 查看栈顶不弹出
// ============================================================
void StackTest::testStack_Top_Int()
{
    Stack<int> s;
    s.push(100);
    s.push(200);
    STE_EQ(s.top(), 200, "top==200");
    STE_EQ(s.size(), 2, "top doesn't change size");
    STE_EQ(s.top(), 200, "top again==200");
}

void StackTest::testStack_Top_AfterPush()
{
    Stack<int> s;
    s.push(1);
    STE_EQ(s.top(), 1, "top after push 1==1");
    s.push(2);
    STE_EQ(s.top(), 2, "top after push 2==2");
    s.push(3);
    STE_EQ(s.top(), 3, "top after push 3==3");
}

void StackTest::testStack_Top_AfterPop()
{
    Stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);
    s.pop();
    STE_EQ(s.top(), 20, "top after pop==20");
    s.pop();
    STE_EQ(s.top(), 10, "top after 2nd pop==10");
}

void StackTest::testStack_Top_Modify()
{
    Stack<int> s;
    s.push(5);
    s.top() = 99;  // top() 返回引用，可以修改
    STE_EQ(s.pop(), 99, "top() modify then pop==99");
}

void StackTest::testStack_Top_String()
{
    Stack<string> s;
    s.push("hello");
    STE_ASSERT(s.top() == "hello", "top string==hello");
    s.top() = "modified";
    STE_ASSERT(s.top() == "modified", "top modify string");
}

// ============================================================
// size — 追踪
// ============================================================
void StackTest::testStack_SizeTracking()
{
    Stack<int> s;
    STE_EQ(s.size(), 0, "size init==0");
    s.push(1);
    STE_EQ(s.size(), 1, "size after push 1");
    s.push(2);
    STE_EQ(s.size(), 2, "size after push 2");
    s.pop();
    STE_EQ(s.size(), 1, "size after pop");
    s.pop();
    STE_EQ(s.size(), 0, "size after pop all");
}

void StackTest::testStack_SizeTracking_PushPopMix()
{
    Stack<int> s;
    for (int i = 0; i < 10; ++i)
    {
        s.push(i);
    }
    STE_EQ(s.size(), 10, "pushPopMix size==10");
    for (int i = 0; i < 5; ++i)
    {
        s.pop();
    }
    STE_EQ(s.size(), 5, "pushPopMix size==5");
    for (int i = 0; i < 10; ++i)
    {
        s.push(i);
    }
    STE_EQ(s.size(), 15, "pushPopMix size==15");
}

// ============================================================
// 综合场景：模拟括号匹配
// ============================================================
void StackTest::testStack_BracketMatch()
{
    Stack<char> s;
    const char* expr = "(a+(b*c))";
    int len = (int)strlen(expr);
    for (int i = 0; i < len; ++i)
    {
        if (expr[i] == '(')
        {
            s.push('(');
        }
        else if (expr[i] == ')')
        {
            STE_ASSERT(s.size() > 0, "bracketMatch non-empty on ')'");
            s.pop();
        }
    }
    STE_EQ(s.size(), 0, "bracketMatch fully matched");
}

void StackTest::testStack_BracketMatch_Nested()
{
    Stack<char> s;
    const char* expr = "((()))";
    int len = (int)strlen(expr);
    for (int i = 0; i < len; ++i)
    {
        if (expr[i] == '(')
        {
            s.push('(');
        }
        else if (expr[i] == ')')
        {
            s.pop();
        }
    }
    STE_EQ(s.size(), 0, "nested bracket match");
}

// ============================================================
// 综合场景：模拟路径解析（目录栈）
// ============================================================
void StackTest::testStack_DirectoryNavigation()
{
    Stack<string> dirStack;
    dirStack.push("root");
    dirStack.push("home");
    dirStack.push("user");
    dirStack.push("documents");
    STE_EQ(dirStack.size(), 4, "dirNav size==4");
    STE_ASSERT(dirStack.top() == "documents", "dirNav top==documents");
    dirStack.pop();
    STE_ASSERT(dirStack.top() == "user", "dirNav after pop top==user");
    dirStack.pop();
    STE_ASSERT(dirStack.top() == "home", "dirNav 2nd pop top==home");
    dirStack.pop();
    dirStack.pop();
    STE_EQ(dirStack.size(), 0, "dirNav all popped");
}

// ============================================================
// 综合场景：逆序输出
// ============================================================
void StackTest::testStack_Reverse()
{
    Stack<int> s;
    const int data[] = { 1, 2, 3, 4, 5 };
    for (int i = 0; i < 5; ++i)
    {
        s.push(data[i]);
    }
    // 逐个弹出，得到逆序
    FOR_INVERSE(5)
    {
        STE_EQ(s.pop(), data[i], "reverse pop");
    }
}

// ============================================================
// 综合场景：交替 push/pop
// ============================================================
void StackTest::testStack_AlternatePushPop()
{
    Stack<int> s;
    // 交替 push/pop，始终保持 1 个元素
    for (int i = 0; i < 50; ++i)
    {
        s.push(i);
        STE_EQ(s.size(), 1, "alternate push size==1");
        STE_EQ(s.top(), i, "alternate top==i");
        STE_EQ(s.pop(), i, "alternate pop==i");
        STE_EQ(s.size(), 0, "alternate pop size==0");
    }
}

// ============================================================
// 不同类型：llong / Vector2 / struct
// ============================================================
void StackTest::testStack_LLong()
{
    Stack<llong> s;
    s.push(1000000000LL);
    s.push(-999999999LL);
    s.push(0LL);
    STE_EQ(s.pop(), 0LL, "llong pop 0");
    STE_EQ(s.pop(), -999999999LL, "llong pop -999999999");
    STE_EQ(s.pop(), 1000000000LL, "llong pop 1000000000");
}

void StackTest::testStack_Float_Precision()
{
    Stack<float> s;
    s.push(0.1f);
    s.push(0.2f);
    s.push(0.3f);
    float v1 = s.pop();
    float v2 = s.pop();
    float v3 = s.pop();
    STE_ASSERT(fabsf(v1 - 0.3f) < 1e-4f, "float stack pop 0.3");
    STE_ASSERT(fabsf(v2 - 0.2f) < 1e-4f, "float stack pop 0.2");
    STE_ASSERT(fabsf(v3 - 0.1f) < 1e-4f, "float stack pop 0.1");
}

void StackTest::testStack_Vector2()
{
    Stack<Vector2> s;
    s.push(Vector2(1.0f, 2.0f));
    s.push(Vector2(3.0f, 4.0f));
    s.push(Vector2(5.0f, 6.0f));
    Vector2 v = s.pop();
    STE_ASSERT(fabsf(v.x - 5.0f) < 1e-4f, "Vector2 pop x==5");
    STE_ASSERT(fabsf(v.y - 6.0f) < 1e-4f, "Vector2 pop y==6");
    STE_EQ(s.size(), 2, "Vector2 pop size==2");
}

// ============================================================
// 边界场景：单个元素
// ============================================================
void StackTest::testStack_SingleElement()
{
    Stack<int> s;
    s.push(42);
    STE_EQ(s.size(), 1, "single size==1");
    STE_EQ(s.top(), 42, "single top==42");
    STE_EQ(s.pop(), 42, "single pop==42");
    STE_EQ(s.size(), 0, "single pop size==0");
}

void StackTest::testStack_SingleElement_Reuse()
{
    Stack<int> s;
    for (int val = 100; val < 110; ++val)
    {
        s.push(val);
        STE_EQ(s.top(), val, "single reuse top==val");
        STE_EQ(s.pop(), val, "single reuse pop==val");
    }
    STE_EQ(s.size(), 0, "single reuse all empty");
}

// ============================================================
// 场景：用栈实现简单计算器（后缀表达式求值）
// ============================================================
void StackTest::testStack_PostfixCalculator()
{
    // 计算 "3 4 +" = 7
    Stack<int> s;
    s.push(3);
    s.push(4);
    int b = s.pop();
    int a = s.pop();
    s.push(a + b);
    STE_EQ(s.pop(), 7, "postfix 3+4==7");
}

void StackTest::testStack_PostfixCalculator_Complex()
{
    // 计算 "5 3 2 * +" = 5 + 3*2 = 11
    Stack<int> s;
    s.push(5);
    s.push(3);
    s.push(2);
    int b = s.pop();
    int a = s.pop();
    s.push(a * b);  // 3*2=6
    b = s.pop();
    a = s.pop();
    s.push(a + b);  // 5+6=11
    STE_EQ(s.pop(), 11, "postfix 5+3*2==11");
}

// ============================================================
// 场景：深度遍历栈（模拟图遍历）
// ============================================================
void StackTest::testStack_DepthFirstTraversal()
{
    // 模拟简单图的 DFS 遍历
    // 节点 0 -> {1, 2}
    // 节点 1 -> {3, 4}
    // 节点 2 -> {}
    Stack<int> dfsStack;
    Vector<bool> visited;
    visited.resize(5);
    for (int i = 0; i < 5; ++i)
    {
        visited[i] = false;
    }

    dfsStack.push(0);
    Vector<int> order;
    while (dfsStack.size() > 0)
    {
        int node = dfsStack.pop();
        if (visited[node])
        {
            continue;
        }
        visited[node] = true;
        order.add(node);
        if (node == 0)
        {
            dfsStack.push(2);
            dfsStack.push(1);
        }
        else if (node == 1)
        {
            dfsStack.push(4);
            dfsStack.push(3);
        }
    }
    STE_EQ(order.size(), 5, "DFS visits 5 nodes");
    STE_EQ(order[0], 0, "DFS start at 0");
}

// ============================================================
// 场景：嵌套调用帧模拟
// ============================================================
void StackTest::testStack_CallFrameSimulation()
{
    Stack<string> callStack;
    // 模拟函数调用
    callStack.push("main");
    callStack.push("init");
    callStack.push("loadConfig");
    STE_EQ(callStack.size(), 3, "call stack depth 3");
    STE_ASSERT(callStack.top() == "loadConfig", "top frame==loadConfig");
    callStack.pop();  // return from loadConfig
    STE_ASSERT(callStack.top() == "init", "back to init");
    callStack.push("parseData");
    STE_EQ(callStack.size(), 3, "call stack 3 again");
    callStack.pop();
    callStack.pop();
    STE_ASSERT(callStack.top() == "main", "final frame==main");
    callStack.pop();
    STE_EQ(callStack.size(), 0, "call stack empty");
}

// ============================================================
// 压力测试
// ============================================================
void StackTest::testStack_StressTest()
{
    Stack<int> s;
    const int N = 10000;
    for (int i = 0; i < N; ++i)
    {
        s.push(i);
    }
    STE_EQ(s.size(), N, "stress push size==10000");
    int sum = 0;
    for (int i = 0; i < N; ++i)
    {
        sum += s.pop();
    }
    STE_EQ(sum, N * (N - 1) / 2, "stress sum correct");
    STE_EQ(s.size(), 0, "stress all popped");
}

void StackTest::testStack_StressAlternate()
{
    Stack<int> s;
    // 每次 push 3 个，pop 2 个，最终累积 N 个
    const int CYCLES = 100;
    int expected = 0;
    for (int c = 0; c < CYCLES; ++c)
    {
        for (int i = 0; i < 3; ++i)
        {
            s.push(c * 3 + i);
            ++expected;
        }
        for (int i = 0; i < 2; ++i)
        {
            s.pop();
            --expected;
        }
    }
    STE_EQ(s.size(), expected, "stress alternate final size");
}

// ============================================================
// 主入口
// ============================================================
void StackTest::test()
{
    testStack_PushSize_Int();
    testStack_PushSize_String();
    testStack_PushMany();
    testStack_PopOrder_Int();
    testStack_PopOrder_String();
    testStack_PopOrder_Float();
    testStack_PopLarge();
    testStack_Top_Int();
    testStack_Top_AfterPush();
    testStack_Top_AfterPop();
    testStack_Top_Modify();
    testStack_Top_String();
    testStack_SizeTracking();
    testStack_SizeTracking_PushPopMix();
    testStack_BracketMatch();
    testStack_BracketMatch_Nested();
    testStack_DirectoryNavigation();
    testStack_Reverse();
    testStack_AlternatePushPop();
    testStack_LLong();
    testStack_Float_Precision();
    testStack_Vector2();
    testStack_SingleElement();
    testStack_SingleElement_Reuse();
    testStack_PostfixCalculator();
    testStack_PostfixCalculator_Complex();
    testStack_DepthFirstTraversal();
    testStack_CallFrameSimulation();
    testStack_StressTest();
    testStack_StressAlternate();
	testSTLBasic();
}

// ============================================================
// Consolidated from legacy STL aggregate test::testStack
// ============================================================
#ifndef STACKTEST_STL_ASSERT
#define STACKTEST_STL_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("StackTest FAIL: ") + (msg)); } } while(0)
#endif
#ifndef STACKTEST_STL_ASSERT_EQ
#define STACKTEST_STL_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StackTest FAIL: ") + (msg) + " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#endif
#ifndef STACKTEST_STL_ASSERT_STR
#define STACKTEST_STL_ASSERT_STR(a, b, msg) \
	do { if (string(a) != string(b)) { ERROR(string("StackTest FAIL: ") + (msg)); } } while(0)
#endif

void StackTest::testSTLBasic()
{
	Stack<int> st;
	STACKTEST_STL_ASSERT_EQ(st.size(), 0, "Stack default size=0");

	// push
	st.push(1);
	st.push(2);
	st.push(3);
	STACKTEST_STL_ASSERT_EQ(st.size(), 3, "Stack size=3 after push 3");

	// top
	STACKTEST_STL_ASSERT_EQ(st.top(), 3, "Stack top=3 (LIFO)");

	// pop (LIFO order)
	const int a = st.pop();
	STACKTEST_STL_ASSERT_EQ(a, 3, "Stack pop=3");
	STACKTEST_STL_ASSERT_EQ(st.size(), 2, "Stack size=2");

	const int b = st.pop();
	STACKTEST_STL_ASSERT_EQ(b, 2, "Stack pop=2");

	const int c = st.pop();
	STACKTEST_STL_ASSERT_EQ(c, 1, "Stack pop=1");
	STACKTEST_STL_ASSERT_EQ(st.size(), 0, "Stack empty after 3 pops");

	// multiple types: string stack
	Stack<string> strSt;
	strSt.push("hello");
	strSt.push("world");
	STACKTEST_STL_ASSERT_EQ(strSt.size(), 2, "StrStack size=2");
	STACKTEST_STL_ASSERT(strSt.top() == "world", "StrStack top=world");
	const string top = strSt.pop();
	STACKTEST_STL_ASSERT(top == "world", "StrStack pop=world");
	STACKTEST_STL_ASSERT(strSt.top() == "hello", "StrStack top=hello after pop");

	// stress: push 100, pop 100 in LIFO order
	Stack<int> stress;
	FOR(100)
	{
		stress.push(i);
	}
	STACKTEST_STL_ASSERT_EQ(stress.size(), 100, "Stress stack size=100");
	FOR_INVERSE(100)
	{
		const int val = stress.pop();
		STACKTEST_STL_ASSERT_EQ(val, i, "Stress stack LIFO order");
	}
	STACKTEST_STL_ASSERT_EQ(stress.size(), 0, "Stress stack empty");
}
#endif
