#pragma once

#include "ClassBaseTypePoolThread.h"
#include "GameCommand.h"

class MICRO_LEGEND_FRAME_API CommandThreadPool : public ClassBaseTypePoolThread<GameCommand>
{};