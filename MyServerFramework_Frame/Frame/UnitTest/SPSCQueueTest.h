#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API SPSCQueueTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void print_pass(const char* name);
	static bool test_empty_pop();
	static bool test_push_then_pop();
	static bool test_full_push_returns_false();
	static bool test_fifo_order();
	static bool test_wraparound_reuse();
	static bool test_large_sequential();
	static bool test_mt_order_and_integrity();
	static bool test_mt_tiny_queue_boundary();
	static bool test_mt_producer_leads_consumer();
	static bool test_mt_consumer_leads_producer();
	static bool test_two_element_batch();
	static bool test_nullptr_element();
	static bool test_non_pod_object();
	static bool test_various_capacities();
	static bool test_alternating_push_pop();
	static bool test_tiny_capacity_repeated_wraparound();
	static bool test_full_then_drain_then_full();
	static bool test_multi_type_same_capacity();
	static bool test_mt_burst_rounds();
	static bool test_mt_consumer_faster_than_producer();
	static bool test_mt_data_integrity_with_padding();
	static bool test_mt_repeated_queue_lifecycle();
	static bool test_mt_realistic_create_destroy();
#endif
};