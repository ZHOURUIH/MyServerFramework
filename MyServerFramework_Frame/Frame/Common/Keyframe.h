#pragma once

class MICRO_LEGEND_FRAME_API KEYFRAME
{
public:
	static constexpr int NONE = 0;
	static constexpr int LINEAR_ZERO_ONE = 1;
	static constexpr int LINEAR_ZERO_ONE_ZERO = 2;
	static constexpr int LINEAR_ONE_ZERO = 3;
	static constexpr int LINEAR_ONE_ZERO_ONE = 4;
	static constexpr int BACK_IN = 5;
	static constexpr int BACK_OUT = 6;
	static constexpr int BACK_IN_OUT = 7;
	static constexpr int BOUNCE_IN = 8;
	static constexpr int BOUNCE_OUT = 9;
	static constexpr int BOUNCE_IN_OUT = 10;
	static constexpr int CIRCLE_IN = 11;
	static constexpr int CIRCLE_OUT = 12;
	static constexpr int CIRCLE_IN_OUT = 13;
	static constexpr int CUBIC_IN = 14;
	static constexpr int CUBIC_OUT = 15;
	static constexpr int CUBIC_IN_OUT = 16;
	static constexpr int ELASTIC_IN = 17;
	static constexpr int ELASTIC_OUT = 18;
	static constexpr int ELASTIC_IN_OUT = 19;
	static constexpr int EXPO_IN = 20;
	static constexpr int EXPO_OUT = 21;
	static constexpr int EXPO_IN_OUT = 22;
	static constexpr int QUAD_IN = 23;
	static constexpr int QUAD_OUT = 24;
	static constexpr int QUAD_IN_OUT = 25;
	static constexpr int QUART_IN = 26;
	static constexpr int QUART_OUT = 27;
	static constexpr int QUART_IN_OUT = 28;
	static constexpr int QUINT_IN = 29;
	static constexpr int QUINT_OUT = 30;
	static constexpr int QUINT_IN_OUT = 31;
	static constexpr int SINE_IN = 32;
	static constexpr int SINE_OUT = 33;
	static constexpr int SINE_IN_OUT = 34;
};