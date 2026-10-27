#include "ringhandle/handle_table.hpp"

typedef NTSTATUS(NTAPI* pfnNtQuerySystemInformation)(
    ULONG SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

namespace ringhandle {

static pfnNtQuerySystemInformation get_nt_query_sys_info() {
    static auto fn = reinterpret_cast<pfnNtQuerySystemInformation>(
        GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQuerySystemInformation")
    );
    return fn;
}

std::vector<HandleRecord> HandleScanner::snapshot_all_handles() {
    std::vector<HandleRecord> records;
    auto query = get_nt_query_sys_info();
    if (!query) return records;

    ULONG size = 1024 * 1024;
    std::vector<uint8_t> buffer;
    NTSTATUS status = STATUS_INFO_LENGTH_MISMATCH;

    while (status == STATUS_INFO_LENGTH_MISMATCH) {
        size *= 2;
        buffer.resize(size);
        ULONG return_len = 0;
        status = query(SystemExtendedHandleInformation, buffer.data(), size, &return_len);
    }

    if (status != 0) { // STATUS_SUCCESS = 0
        return records;
    }

    auto info = reinterpret_cast<PSYSTEM_HANDLE_INFORMATION_EX>(buffer.data());
    records.reserve(info->NumberOfHandles);

    for (ULONG_PTR i = 0; i < info->NumberOfHandles; ++i) {
        const auto& entry = info->Handles[i];
        records.push_back(HandleRecord{
            static_cast<uint32_t>(entry.UniqueProcessId),
            reinterpret_cast<uintptr_t>(reinterpret_cast<void*>(entry.HandleValue)),
            reinterpret_cast<uintptr_t>(entry.Object),
            entry.GrantedAccess,
            entry.ObjectTypeIndex
        });
    }

    return records;
}

std::vector<HandleRecord> HandleScanner::filter_by_pid(uint32_t pid) {
    auto all = snapshot_all_handles();
    std::vector<HandleRecord> filtered;
    for (const auto& rec : all) {
        if (rec.pid == pid) {
            filtered.push_back(rec);
        }
    }
    return filtered;
}

}
