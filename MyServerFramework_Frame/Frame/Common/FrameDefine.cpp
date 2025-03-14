﻿#include "FrameDefine.h"

const string FrameDefine::MEDIA_PATH = "../media/";
const string FrameDefine::CONFIG_PATH = MEDIA_PATH + "Config/";
const string FrameDefine::LOG_PATH = MEDIA_PATH + "Log/";
const string FrameDefine::SQLITE_PATH = FrameDefine::MEDIA_PATH + "DataBase/";
const string FrameDefine::EXCEL_PATH = FrameDefine::MEDIA_PATH + "Excel/";
const string FrameDefine::EMPTY = "";
byte FrameDefine::ENCRYPT_KEY[FrameDefine::ENCRYPT_KEY_LENGTH]
{
	0xE8, 0x89, 0xD1, 0x14, 0x8C, 0x67, 0xB1, 0xC8, 0x6E, 0x19, 0x1A, 0x5E, 0x50, 0x99, 0x8B, 0xF7,
	0x11, 0xA4, 0xAC, 0x10, 0x30, 0x10, 0x82, 0x92, 0x48, 0x8F, 0x0C, 0x85, 0xCE, 0x8A, 0xA9, 0xD3,
	0xC3, 0x62, 0xBC, 0xD7, 0xD0, 0x26, 0x2F, 0x34, 0xBC, 0xDF, 0x6E, 0xAA, 0x1A, 0x3E, 0x67, 0xB6,
	0xBA, 0xCF, 0xF1, 0xF5, 0xA5, 0xF3, 0xF4, 0x3F, 0x6E, 0xEA, 0xA4, 0xD3, 0x3F, 0x0A, 0xFA, 0xB4,
	0x79, 0x91, 0x6C, 0x7D, 0x52, 0x1C, 0x97, 0x60, 0xC6, 0x93, 0xC2, 0xEF, 0xAD, 0x72, 0xA1, 0x86,
	0x12, 0x63, 0x5C, 0x46, 0xAE, 0x44, 0xDE, 0xA1, 0xD5, 0x8A, 0xEE, 0x42, 0x07, 0x7D, 0x88, 0x65,
	0x37, 0xF1, 0xC5, 0xEB, 0xDF, 0xF9, 0x29, 0x9B, 0xDF, 0xEE, 0x2D, 0x99, 0x0D, 0x59, 0xED, 0x42,
	0xDE, 0x4C, 0x14, 0x65, 0xBC, 0xA8, 0xEA, 0xC6, 0x92, 0x02, 0xD5, 0xAC, 0x83, 0x83, 0x2F, 0x9B,
};