#pragma once

#include "FrameMacro.h"

// 创建一个行为树节点
#define DECISION(type, parent, debugString) static_cast<type*>(decisionTree->createNode<type>(parent, debugString))