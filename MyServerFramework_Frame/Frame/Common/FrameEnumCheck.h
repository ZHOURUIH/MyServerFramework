#pragma once

#include "FrameEnum.h"

// auto generated file
class FrameEnumCheck
{
public:
	static constexpr bool checkEnum(const COMMAND_STATE value)
	{
		switch (value)
		{
		case COMMAND_STATE::NONE:break;
		case COMMAND_STATE::PUSHED:break;
		case COMMAND_STATE::EXECUTING:break;
		case COMMAND_STATE::EXECUTED:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const GROUP_MUTEX value)
	{
		switch (value)
		{
		case GROUP_MUTEX::COEXIST:break;
		case GROUP_MUTEX::REMOVE_OTHERS:break;
		case GROUP_MUTEX::NO_NEW:break;
		case GROUP_MUTEX::MUTEX_WITH_MAIN:break;
		case GROUP_MUTEX::MUTEX_WITH_MAIN_ONLY:break;
		case GROUP_MUTEX::MUTEX_INVERSE_MAIN:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const KEY_STATE value)
	{
		switch (value)
		{
		case KEY_STATE::KEEP_UP:break;
		case KEY_STATE::KEEP_DOWN:break;
		case KEY_STATE::CURRENT_UP:break;
		case KEY_STATE::CURRENT_DOWN:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const LOGIC_RELATION value)
	{
		switch (value)
		{
		case LOGIC_RELATION::NONE:break;
		case LOGIC_RELATION::AND:break;
		case LOGIC_RELATION::OR:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MOUSE_BUTTON value)
	{
		switch (value)
		{
		case MOUSE_BUTTON::NONE:break;
		case MOUSE_BUTTON::LEFT:break;
		case MOUSE_BUTTON::RIGHT:break;
		case MOUSE_BUTTON::MIDDLE:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MYSQL_ORDER value)
	{
		switch (value)
		{
		case MYSQL_ORDER::NONE:break;
		case MYSQL_ORDER::ASC:break;
		case MYSQL_ORDER::DESC:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const PARSE_RESULT value)
	{
		switch (value)
		{
		case PARSE_RESULT::SUCCESS:break;
		case PARSE_RESULT::NOT_ENOUGH:break;
		case PARSE_RESULT::DECOMPRESS_FAILED:break;
		case PARSE_RESULT::INVALID_PACKET_TYPE:break;
		case PARSE_RESULT::PACKET_PARSE_FAILED:break;
		case PARSE_RESULT::SEQUENCE_NUMBER_ERROR:break;
		case PARSE_RESULT::CRC_ERROR:break;
		case PARSE_RESULT::BODY_SIZE_CRC_ERROR:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const PLAY_STATE value)
	{
		switch (value)
		{
		case PLAY_STATE::NONE:break;
		case PLAY_STATE::PLAY:break;
		case PLAY_STATE::PAUSE:break;
		case PLAY_STATE::STOP:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const SERIALIZE_PARAM_LENGTH value)
	{
		switch (value)
		{
		case SERIALIZE_PARAM_LENGTH::NONE:break;
		case SERIALIZE_PARAM_LENGTH::SHORT:break;
		case SERIALIZE_PARAM_LENGTH::MIDDLE:break;
		case SERIALIZE_PARAM_LENGTH::LONG:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const SQLITE_DATATYPE value)
	{
		switch (value)
		{
		case SQLITE_DATATYPE::SQLITE_DATATYPE_INTEGER:break;
		case SQLITE_DATATYPE::SQLITE_DATATYPE_FLOAT:break;
		case SQLITE_DATATYPE::SQLITE_DATATYPE_TEXT:break;
		case SQLITE_DATATYPE::SQLITE_DATATYPE_BLOB:break;
		case SQLITE_DATATYPE::SQLITE_DATATYPE_NULL:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const STATE_MUTEX value)
	{
		switch (value)
		{
		case STATE_MUTEX::CAN_NOT_ADD_NEW:break;
		case STATE_MUTEX::REMOVE_OLD:break;
		case STATE_MUTEX::COEXIST:break;
		case STATE_MUTEX::KEEP_HIGH_PRIORITY:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const TASK_STATE value)
	{
		switch (value)
		{
		case TASK_STATE::NONE:break;
		case TASK_STATE::SUCCESS:break;
		case TASK_STATE::RUNNING:break;
		case TASK_STATE::FAILED:break;
		default:return false;
		}
		return true;
	}
};