#include <cassert>
#include <iostream>
#include "ringhandle/handle_table.hpp"

int main() {
    std::cout << "Running RingHandle unit tests...\n";

    auto handles = ringhandle::HandleScanner::snapshot_all_handles();
    assert(!handles.empty() && "Handle snapshot should return non-empty list");

    uint32_t current_pid = GetCurrentProcessId();
    auto self_handles = ringhandle::HandleScanner::filter_by_pid(current_pid);
    assert(!self_handles.empty() && "Current process must have active handles");

    std::cout << "All tests passed successfully! Total handles: " << handles.size()
              << ", self handles: " << self_handles.size() << "\n";
    return 0;
}
