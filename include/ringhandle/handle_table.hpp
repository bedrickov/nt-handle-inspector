#pragma once
#include <windows.h>
#include <winternl.h>
#include <vector>
#include <string>
#include <cstdint>

#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#define SystemExtendedHandleInformation 64

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX {
    PVOID Object;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR HandleValue;
    ULONG GrantedAccess;
    USHORT CreatorBackTraceIndex;
    USHORT ObjectTypeIndex;
    ULONG HandleAttributes;
    ULONG Reserved;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX, *PSYSTEM_HANDLE_TABLE_ENTRY_INFO_EX;

typedef struct _SYSTEM_HANDLE_INFORMATION_EX {
    ULONG_PTR NumberOfHandles;
    ULONG_PTR Reserved;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX Handles[1];
} SYSTEM_HANDLE_INFORMATION_EX, *PSYSTEM_HANDLE_INFORMATION_EX;

namespace ringhandle {

struct HandleRecord {
    uint32_t pid;
    uintptr_t handle_value;
    uintptr_t object_address;
    uint32_t access_mask;
    uint16_t type_index;
};

class HandleScanner {
public:
    static std::vector<HandleRecord> snapshot_all_handles();
    static std::vector<HandleRecord> filter_by_pid(uint32_t pid);
};

}
