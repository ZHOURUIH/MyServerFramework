#pragma once

// 解析消息的结果
enum class PARSE_RESULT : byte
{
	SUCCESS,				// 成功
	NOT_ENOUGH,				// 接收数据不足
	DECOMPRESS_FAILED,		// 解压错误
	INVALID_PACKET_TYPE,	// 无效的消息类型ID
	PACKET_PARSE_FAILED,	// 消息包体解析错误
	SEQUENCE_NUMBER_ERROR,	// 序列号错误
	CRC_ERROR,				// CRC校验错误
	BODY_SIZE_CRC_ERROR,	// 包体长度的CRC校验错误
};

// 播放状态
enum class PLAY_STATE : byte
{
	NONE,			// 无效值
	PLAY,			// 播放
	PAUSE,			// 暂停
	STOP,			// 停止
};

// 命令的状态
enum class COMMAND_STATE : byte
{
	NONE,			// 无效值
	PUSHED,			// 已压入命令系统
	EXECUTING,		// 正在执行
	EXECUTED,		// 执行完毕
};

// 按键状态
enum class KEY_STATE : byte
{
	KEEP_UP,		// 持续放开
	KEEP_DOWN,		// 持续按下
	CURRENT_UP,		// 当前放开
	CURRENT_DOWN,	// 当前按下
};

// 鼠标按钮
enum class MOUSE_BUTTON : byte
{
	NONE,
	LEFT,
	RIGHT,
	MIDDLE,
};

// SQLite数据类型
enum class SQLITE_DATATYPE : byte
{
	SQLITE_DATATYPE_INTEGER = SQLITE_INTEGER,
	SQLITE_DATATYPE_FLOAT = SQLITE_FLOAT,
	SQLITE_DATATYPE_TEXT = SQLITE_TEXT,
	SQLITE_DATATYPE_BLOB = SQLITE_BLOB,
	SQLITE_DATATYPE_NULL = SQLITE_NULL,
};

// 序列化数据的数组类型的字段长度类型
enum class SERIALIZE_PARAM_LENGTH : byte
{
	NONE,			// 无效值
	SHORT,			// 长度较短,最大长度为255
	MIDDLE,			// 长度中等,最大长度为ushort的最大值
	LONG,			// 长度较长,最大长度为int的最大值
};

// 数据库查询结果的排序方式
enum class MYSQL_ORDER : byte
{
	NONE,			// 不排序
	ASC,			// 升序排序
	DESC,			// 降序排序
};

// 逻辑关系类型
enum class LOGIC_RELATION : byte
{
	NONE,			// 无效值
	AND,			// 逻辑与
	OR,				// 逻辑或
};

// 行为树节点的执行结果
enum class TASK_STATE : byte
{
	NONE,			// 无效值
	SUCCESS,		// 节点执行成功
	RUNNING,		// 节点正在执行
	FAILED,			// 节点执行失败
};

// 添加同一状态时的操作选项
enum class STATE_MUTEX : byte
{
	CAN_NOT_ADD_NEW,		// 不可添加相同的新状态
	REMOVE_OLD,				// 添加新状态,移除互斥的旧状态
	COEXIST,				// 新旧状态可共存
	KEEP_HIGH_PRIORITY,		// 保留新旧状态中优先级最高的
};

// 同一状态组中的状态互斥选项
enum class GROUP_MUTEX : byte
{
	COEXIST,				// 各状态可完全共存
	REMOVE_OTHERS,			// 添加新状态时移除组中的其他所有状态
	NO_NEW,					// 状态组中有状态时不允许添加新状态
	MUTEX_WITH_MAIN,		// 仅与主状态互斥,添加主状态时移除其他所有状态,有主状态时不可添加其他状态,没有主状态时可任意添加其他状态
	MUTEX_WITH_MAIN_ONLY,   // 仅与主状态互斥,添加主状态时移除其他所有状态,无论是否有主状态都可以添加其他状态
	MUTEX_INVERSE_MAIN,		// 主状态反向互斥,有其他状态时,不允许添加主状态,添加其他状态时,立即将主状态移除
};