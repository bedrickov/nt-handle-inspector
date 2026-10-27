#include <iostream>
#include <iomanip>
#include "ringhandle/handle_table.hpp"

int main() {
    std::cout << "[RingHandle] Native NT System Extended Handle Table Inspector\n";
    std::cout << "Querying kernel handle table via NtQuerySystemInformation(64)...\n\n";

    auto handles = ringhandle::HandleScanner::snapshot_all_handles();
    std::cout << "Total system active handles captured: " << handles.size() << "\n\n";

    uint32_t my_pid = GetCurrentProcessId();
    auto my_handles = ringhandle::HandleScanner::filter_by_pid(my_pid);

    std::cout << "Self process (PID " << my_pid << ") open handles: " << my_handles.size() << "\n";
    std::cout << std::left << std::setw(12) << "HANDLE"
              << std::setw(20) << "KERNEL_OBJECT"
              << std::setw(12) << "ACCESS"
              << std::setw(10) << "TYPE_IDX" << "\n";
    std::cout << std::string(54, '-') << "\n";

    for (size_t i = 0; i < (my_handles.size() < 15 ? my_handles.size() : 15); ++i) {
        const auto& h = my_handles[i];
        std::cout << "0x" << std::hex << std::setw(10) << h.handle_value
                  << "0x" << std::setw(18) << h.object_address
                  << "0x" << std::setw(10) << h.access_mask
                  << std::dec << std::setw(10) << h.type_index << "\n";
    }

    return 0;
}
