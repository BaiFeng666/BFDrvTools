#include "BFDrvUtils.h"
/*
BFDriver官方驱动群 410342663
周100 月350
便宜吗？这还不买吗牢底？
禁止用于违法用途
*/
#pragma warning (disable:4005)
#include "MemoryModule.h"
#include <cstdio>
#pragma comment(lib,"ntdll.lib")

#include <stdexcept>
#include <fstream>
#include "B_MemoryModule.c"
#ifdef _WIN64
#include "BFDrv_Dynamic.c"
#else
#include "x86/BFDrv_Dynamic.c"
#endif // _WIN64

using B_LoadDynamicLibraryFn = bool(WINAPI*)(HMODULE*, LPVOID);
B_LoadDynamicLibraryFn B_LoadDynamicLibraryPtr = nullptr;


using B_InitDrvFunc = bool(WINAPI*)(const char*, B_InstallMode);
B_InitDrvFunc B_InitDrvPtr = nullptr;

using B_AdjustPrivilegeFunc = bool(WINAPI*)();
B_AdjustPrivilegeFunc B_AdjustPrivilegePtr = nullptr;

using B_GetInitResultFunc = const char* (WINAPI*)();
B_GetInitResultFunc B_GetInitResultPtr = nullptr;

using B_GetExpirationFunc = const char* (WINAPI*)();
B_GetExpirationFunc B_GetExpirationPtr = nullptr;

using B_AttachProcessFunc = bool(WINAPI*)(int);
B_AttachProcessFunc B_AttachProcessPtr = nullptr;

using B_GetWindowsBuildNumberFunc = int(WINAPI*)();
B_GetWindowsBuildNumberFunc B_GetWindowsBuildNumberPtr = nullptr;

using B_GetMoudleBaseAddressFunc = ULONG64(WINAPI*)(const char*, ULONG*);
B_GetMoudleBaseAddressFunc B_GetMoudleBaseAddressPtr = nullptr;

using B_GetMoudleBaseAddressNoAttachFunc = ULONG64(WINAPI*)(const char*, ULONG*);
B_GetMoudleBaseAddressNoAttachFunc B_GetMoudleBaseAddressNoAttachPtr = nullptr;

using B_ReadMemoryFunc = bool(WINAPI*)(ULONG64, void*, size_t, RWMode, ULONG64);
B_ReadMemoryFunc B_ReadMemoryPtr = nullptr;

using B_WriteMemoryFunc = bool(WINAPI*)(ULONG64, void*, size_t, RWMode, ULONG64);
B_WriteMemoryFunc B_WriteMemoryPtr = nullptr;

using B_AllocMemoryFunc = ULONG64(WINAPI*)(size_t);
B_AllocMemoryFunc B_AllocMemoryPtr = nullptr;

using B_FreeMemoryFunc = bool(WINAPI*)(ULONG64);
B_FreeMemoryFunc B_FreeMemoryPtr = nullptr;

using B_ProtectMemoryFunc = bool(WINAPI*)(ULONG64, DWORD, DWORD64);
B_ProtectMemoryFunc B_ProtectMemoryPtr = nullptr;

using B_MouseMoveFunc = void(WINAPI*)(int, int, MoveType);
B_MouseMoveFunc B_MouseMovePtr = nullptr;

using B_MouseCtlFunc = void(WINAPI*)(MouseKey, MouseStatus);
B_MouseCtlFunc B_MouseCtlPtr = nullptr;

using B_KeyCtlFunc = void(WINAPI*)(int, KeyStatus);
B_KeyCtlFunc B_KeyCtlPtr = nullptr;

using B_KeyCtlCharFunc = void(WINAPI*)(char, KeyStatus);
B_KeyCtlCharFunc B_KeyCtlCharPtr = nullptr;

using B_KeyClickFunc = void(WINAPI*)(int);
B_KeyClickFunc B_KeyClickPtr = nullptr;

using B_KeyClickCharFunc = void(WINAPI*)(char);
B_KeyClickCharFunc B_KeyClickCharPtr = nullptr;

using B_ProtectProcessFunc = bool(WINAPI*)(bool, int);
B_ProtectProcessFunc B_ProtectProcessPtr = nullptr;

using B_ProtectProcessV2Func = bool(WINAPI*)(bool, int);
B_ProtectProcessV2Func B_ProtectProcessV2Ptr = nullptr;

using B_HideProcessFunc = bool(WINAPI*)(bool, int);
B_HideProcessFunc B_HideProcessPtr = nullptr;

using B_HideWindowFunc = bool(WINAPI*)(ULONG64, HideWindowType);
B_HideWindowFunc B_HideWindowPtr = nullptr;

using B_GetMoudleExportFuncAddressFunc = ULONG64(WINAPI*)(ULONG64, const char*);
B_GetMoudleExportFuncAddressFunc B_GetMoudleExportFuncAddressPtr = nullptr;

using B_ManualMapDllMemoryFunc = bool(WINAPI*)(const void*, size_t);
B_ManualMapDllMemoryFunc B_ManualMapDllMemoryPtr = nullptr;

using B_ProtectWindowFunc = void(WINAPI*)(bool, ULONG, bool);
B_ProtectWindowFunc B_ProtectWindowPtr = nullptr;

using B_GetProcessRealCr3Func = ULONG64(WINAPI*)();
B_GetProcessRealCr3Func B_GetProcessRealCr3Ptr = nullptr;

using B_GetProcessRealCr3AttachFunc = ULONG64(WINAPI*)();
B_GetProcessRealCr3AttachFunc B_GetProcessRealCr3AttachPtr = nullptr;

using B_PhyReadMemoryWithCr3Func = bool(WINAPI*)(ULONG64, void*, size_t, ULONG64);
B_PhyReadMemoryWithCr3Func B_PhyReadMemoryWithCr3Ptr = nullptr;

using B_PhyWriteMemoryWithCr3Func = bool(WINAPI*)(ULONG64, void*, size_t, ULONG64);
B_PhyWriteMemoryWithCr3Func B_PhyWriteMemoryWithCr3Ptr = nullptr;

using B_ForceDeleteFileFunc = bool(WINAPI*)(const char*);
B_ForceDeleteFileFunc B_ForceDeleteFilePtr = nullptr;

using B_FindPatternV1Func = ULONG64(WINAPI*)(ULONG64, ULONG64, const char*, const char*, RWMode);
B_FindPatternV1Func B_FindPatternV1Ptr = nullptr;

using B_FindPatternV2Func = ULONG64(WINAPI*)(ULONG64, ULONG64, const char*, RWMode);
B_FindPatternV2Func B_FindPatternV2Ptr = nullptr;

using B_AOBScanV1Func = std::vector<ULONG64>(WINAPI*)(const char*, const char*, ULONG64, ULONG64, RWMode);
B_AOBScanV1Func B_AOBScanV1Ptr = nullptr;

using B_AOBScanV2Func = std::vector<ULONG64>(WINAPI*)(const char*, ULONG64, ULONG64, RWMode);
B_AOBScanV2Func B_AOBScanV2Ptr = nullptr;

using B_QueryMemoryFunc = bool(WINAPI*)(ULONG64, MEMORY_BASIC_INFORMATION*);
B_QueryMemoryFunc B_QueryMemoryPtr = nullptr;

using B_MapDLLFunc = bool(WINAPI*)(unsigned char*, size_t);
B_MapDLLFunc B_MapDLLPtr = nullptr;

using B_APCInjectDLLFunc = bool(WINAPI*)(unsigned char*, size_t, bool);
B_APCInjectDLLFunc B_APCInjectDLLPtr = nullptr;

using B_RipInjectDLLFunc = bool(WINAPI*)(unsigned char*, size_t, bool, bool);
B_RipInjectDLLFunc B_RipInjectDLLPtr = nullptr;

using B_RipInjectDLLV2Func = bool(WINAPI*)(unsigned char*, size_t, bool, bool);
B_RipInjectDLLV2Func B_RipInjectDLLV2Ptr = nullptr;

using B_DumpToFileFunc = bool(WINAPI*)(ULONG64, ULONG64, const char*, RWMode);
B_DumpToFileFunc B_DumpToFilePtr = nullptr;

using B_GetDriverBuildTimeFunc = std::string(WINAPI*)();
B_GetDriverBuildTimeFunc B_GetDriverBuildTimePtr = nullptr;

using B_RWKernelMemoryFunc = bool(WINAPI*)(ULONG64, void*, ULONG, int);
B_RWKernelMemoryFunc B_RWKernelMemoryPtr = nullptr;

using B_DisableCallback_NMIFunc = bool(WINAPI*)();
B_DisableCallback_NMIFunc B_DisableCallback_NMIPtr = nullptr;
BFDrv::BFDrv()
{
	HMEMORYMODULE handle = nullptr;
	HMODULE hModule = nullptr;
	handle = MemoryLoadLibrary(B_MemoryModule, sizeof B_MemoryModule);
	if (!handle) throw std::runtime_error("Dll Load is NULL");
	B_LoadDynamicLibraryPtr = (B_LoadDynamicLibraryFn)MemoryGetProcAddress(handle, "B_LoadDynamicLibrary");
	if (!B_LoadDynamicLibraryPtr) throw std::runtime_error("B_LoadDynamicLibraryPtr is NULL");
	if (!B_LoadDynamicLibraryPtr(&hModule, BFDrv_Dynamic)) throw std::runtime_error("Load Dll Failed");
	if (!hModule) throw std::runtime_error("hModule is NULL");
	B_InitDrvPtr = (B_InitDrvFunc)GetProcAddress(hModule, "B_InitDrv");
	if (!B_InitDrvPtr) throw std::runtime_error("B_InitDrvPtr is NULL");
	B_AdjustPrivilegePtr = (B_AdjustPrivilegeFunc)GetProcAddress(hModule, "B_AdjustPrivilege");
	if (!B_AdjustPrivilegePtr) throw std::runtime_error("B_AdjustPrivilegePtr is NULL");
	B_GetInitResultPtr = (B_GetInitResultFunc)GetProcAddress(hModule, "B_GetInitResult");
	if (!B_GetInitResultPtr) throw std::runtime_error("B_GetInitResultPtr is NULL");
	B_GetExpirationPtr = (B_GetExpirationFunc)GetProcAddress(hModule, "B_GetExpiration");
	if (!B_GetExpirationPtr) throw std::runtime_error("B_GetExpirationPtr is NULL");
	B_AttachProcessPtr = (B_AttachProcessFunc)GetProcAddress(hModule, "B_AttachProcess");
	if (!B_AttachProcessPtr) throw std::runtime_error("B_AttachProcessPtr is NULL");
	B_GetWindowsBuildNumberPtr = (B_GetWindowsBuildNumberFunc)GetProcAddress(hModule, "B_GetWindowsBuildNumber");
	if (!B_GetWindowsBuildNumberPtr) throw std::runtime_error("B_GetWindowsBuildNumberPtr is NULL");
	B_GetMoudleBaseAddressPtr = (B_GetMoudleBaseAddressFunc)GetProcAddress(hModule, "B_GetMoudleBaseAddress");
	if (!B_GetMoudleBaseAddressPtr) throw std::runtime_error("B_GetMoudleBaseAddressPtr is NULL");
	B_GetMoudleBaseAddressNoAttachPtr = (B_GetMoudleBaseAddressNoAttachFunc)GetProcAddress(hModule, "B_GetMoudleBaseAddressNoAttach");
	if (!B_GetMoudleBaseAddressNoAttachPtr) throw std::runtime_error("B_GetMoudleBaseAddressNoAttachPtr is NULL");
	B_ReadMemoryPtr = (B_ReadMemoryFunc)GetProcAddress(hModule, "B_ReadMemory");
	if (!B_ReadMemoryPtr) throw std::runtime_error("B_ReadMemoryPtr is NULL");
	B_WriteMemoryPtr = (B_WriteMemoryFunc)GetProcAddress(hModule, "B_WriteMemory");
	if (!B_WriteMemoryPtr) throw std::runtime_error("B_WriteMemoryPtr is NULL");
	B_AllocMemoryPtr = (B_AllocMemoryFunc)GetProcAddress(hModule, "B_AllocMemory");
	if (!B_AllocMemoryPtr) throw std::runtime_error("B_AllocMemoryPtr is NULL");
	B_FreeMemoryPtr = (B_FreeMemoryFunc)GetProcAddress(hModule, "B_FreeMemory");
	if (!B_FreeMemoryPtr) throw std::runtime_error("B_FreeMemoryPtr is NULL");
	B_ProtectMemoryPtr = (B_ProtectMemoryFunc)GetProcAddress(hModule, "B_ProtectMemory");
	if (!B_ProtectMemoryPtr) throw std::runtime_error("B_ProtectMemoryPtr is NULL");
	B_MouseMovePtr = (B_MouseMoveFunc)GetProcAddress(hModule, "B_MouseMove");
	if (!B_MouseMovePtr) throw std::runtime_error("B_MouseMovePtr is NULL");
	B_MouseCtlPtr = (B_MouseCtlFunc)GetProcAddress(hModule, "B_MouseCtl");
	if (!B_MouseCtlPtr) throw std::runtime_error("B_MouseCtlPtr is NULL");
	B_KeyCtlPtr = (B_KeyCtlFunc)GetProcAddress(hModule, "B_KeyCtl");
	if (!B_KeyCtlPtr) throw std::runtime_error("B_KeyCtlPtr is NULL");
	B_KeyCtlCharPtr = (B_KeyCtlCharFunc)GetProcAddress(hModule, "B_KeyCtlChar");
	if (!B_KeyCtlCharPtr) throw std::runtime_error("B_KeyCtlCharPtr is NULL");
	B_KeyClickPtr = (B_KeyClickFunc)GetProcAddress(hModule, "B_KeyClick");
	if (!B_KeyClickPtr) throw std::runtime_error("B_KeyClickPtr is NULL");
	B_KeyClickCharPtr = (B_KeyClickCharFunc)GetProcAddress(hModule, "B_KeyClickChar");
	if (!B_KeyClickCharPtr) throw std::runtime_error("B_KeyClickCharPtr is NULL");
	B_ProtectProcessPtr = (B_ProtectProcessFunc)GetProcAddress(hModule, "B_ProtectProcess");
	if (!B_ProtectProcessPtr) throw std::runtime_error("B_ProtectProcessPtr is NULL");
	B_ProtectProcessV2Ptr = (B_ProtectProcessV2Func)GetProcAddress(hModule, "B_ProtectProcessV2");
	if (!B_ProtectProcessV2Ptr) throw std::runtime_error("B_ProtectProcessV2Ptr is NULL");
	B_HideProcessPtr = (B_HideProcessFunc)GetProcAddress(hModule, "B_HideProcess");
	if (!B_HideProcessPtr) throw std::runtime_error("B_HideProcessPtr is NULL");
	B_HideWindowPtr = (B_HideWindowFunc)GetProcAddress(hModule, "B_HideWindow");
	if (!B_HideWindowPtr) throw std::runtime_error("B_HideWindowPtr is NULL");
	B_GetMoudleExportFuncAddressPtr = (B_GetMoudleExportFuncAddressFunc)GetProcAddress(hModule, "B_GetMoudleExportFuncAddress");
	if (!B_GetMoudleExportFuncAddressPtr) throw std::runtime_error("B_GetMoudleExportFuncAddressPtr is NULL");
	B_ManualMapDllMemoryPtr = (B_ManualMapDllMemoryFunc)GetProcAddress(hModule, "B_ManualMapDllMemory");
	if (!B_ManualMapDllMemoryPtr) throw std::runtime_error("B_ManualMapDllMemoryPtr is NULL");
	B_ProtectWindowPtr = (B_ProtectWindowFunc)GetProcAddress(hModule, "B_ProtectWindow");
	if (!B_ProtectWindowPtr) throw std::runtime_error("B_ProtectWindowPtr is NULL");
	B_GetProcessRealCr3Ptr = (B_GetProcessRealCr3Func)GetProcAddress(hModule, "B_GetProcessRealCr3");
	if (!B_GetProcessRealCr3Ptr) throw std::runtime_error("B_GetProcessRealCr3Ptr is NULL");
	B_GetProcessRealCr3AttachPtr = (B_GetProcessRealCr3AttachFunc)GetProcAddress(hModule, "B_GetProcessRealCr3Attach");
	if (!B_GetProcessRealCr3AttachPtr) throw std::runtime_error("B_GetProcessRealCr3AttachPtr is NULL");
	B_PhyReadMemoryWithCr3Ptr = (B_PhyReadMemoryWithCr3Func)GetProcAddress(hModule, "B_PhyReadMemoryWithCr3");
	if (!B_PhyReadMemoryWithCr3Ptr) throw std::runtime_error("B_PhyReadMemoryWithCr3Ptr is NULL");
	B_PhyWriteMemoryWithCr3Ptr = (B_PhyWriteMemoryWithCr3Func)GetProcAddress(hModule, "B_PhyWriteMemoryWithCr3");
	if (!B_PhyWriteMemoryWithCr3Ptr) throw std::runtime_error("B_PhyWriteMemoryWithCr3Ptr is NULL");
	B_ForceDeleteFilePtr = (B_ForceDeleteFileFunc)GetProcAddress(hModule, "B_ForceDeleteFile");
	if (!B_ForceDeleteFilePtr) throw std::runtime_error("B_ForceDeleteFilePtr is NULL");
	B_FindPatternV1Ptr = (B_FindPatternV1Func)GetProcAddress(hModule, "B_FindPatternV1");
	if (!B_FindPatternV1Ptr) throw std::runtime_error("B_FindPatternV1Ptr is NULL");
	B_FindPatternV2Ptr = (B_FindPatternV2Func)GetProcAddress(hModule, "B_FindPatternV2");
	if (!B_FindPatternV2Ptr) throw std::runtime_error("B_FindPatternV2Ptr is NULL");
	B_AOBScanV1Ptr = (B_AOBScanV1Func)GetProcAddress(hModule, "B_AOBScanV1");
	if (!B_AOBScanV1Ptr) throw std::runtime_error("B_AOBScanV1Ptr is NULL");
	B_AOBScanV2Ptr = (B_AOBScanV2Func)GetProcAddress(hModule, "B_AOBScanV2");
	if (!B_AOBScanV2Ptr) throw std::runtime_error("B_AOBScanV2Ptr is NULL");
	B_QueryMemoryPtr = (B_QueryMemoryFunc)GetProcAddress(hModule, "B_QueryMemory");
	if (!B_QueryMemoryPtr) throw std::runtime_error("B_QueryMemoryPtr is NULL");
	B_MapDLLPtr = (B_MapDLLFunc)GetProcAddress(hModule, "B_MapDLL");
	if (!B_MapDLLPtr) throw std::runtime_error("B_MapDLLPtr is NULL");
	B_APCInjectDLLPtr = (B_APCInjectDLLFunc)GetProcAddress(hModule, "B_APCInjectDLL");
	if (!B_APCInjectDLLPtr) throw std::runtime_error("B_APCInjectDLLPtr is NULL");
	B_DumpToFilePtr = (B_DumpToFileFunc)GetProcAddress(hModule, "B_DumpToFile");
	if (!B_DumpToFilePtr) throw std::runtime_error("B_DumpToFilePtr is NULL");
	B_RipInjectDLLPtr = (B_RipInjectDLLFunc)GetProcAddress(hModule, "B_RipInjectDLL");
	if (!B_RipInjectDLLPtr) throw std::runtime_error("B_RipInjectDLLPtr is NULL");
	B_RipInjectDLLV2Ptr = (B_RipInjectDLLV2Func)GetProcAddress(hModule, "B_RipInjectDLLV2");
	if (!B_RipInjectDLLV2Ptr) throw std::runtime_error("B_RipInjectDLLV2Ptr is NULL");
	B_GetDriverBuildTimePtr = (B_GetDriverBuildTimeFunc)GetProcAddress(hModule, "B_GetDriverBuildTime");
	if (!B_GetDriverBuildTimePtr) throw std::runtime_error("B_GetDriverBuildTimePtr is NULL");
	B_RWKernelMemoryPtr = (B_RWKernelMemoryFunc)GetProcAddress(hModule, "B_RWKernelMemory");
	if (!B_RWKernelMemoryPtr) throw std::runtime_error("B_RWKernelMemoryPtr is NULL");
	B_DisableCallback_NMIPtr = (B_DisableCallback_NMIFunc)GetProcAddress(hModule, "B_DisableCallback_NMI");
	if (!B_DisableCallback_NMIPtr) throw std::runtime_error("B_DisableCallback_NMIPtr is NULL");

	PMEMORYMODULE module = (PMEMORYMODULE)handle;
	ClearPEHeadersEx((unsigned char*)module->codeBase);
	ClearPEHeadersEx((unsigned char*)hModule);
#ifdef _WIN64
	UnlinkModule((unsigned char*)module->codeBase);
	UnlinkModule((unsigned char*)hModule);
#endif

	auto ClearDLL = [](unsigned char* base) -> void
		{
			PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)base;
			PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((PBYTE)pDosHeader + pDosHeader->e_lfanew);
			size_t size = pNtHeader->OptionalHeader.SizeOfImage;

			ULONG64 start = (ULONG64)base;
			ULONG64 end = (ULONG64)base + size;

			while (start < end)
			{
				MEMORY_BASIC_INFORMATION mbi;
				VirtualQuery((PVOID)start, &mbi, sizeof(mbi));
				if (!mbi.RegionSize)
					return;

				if (mbi.Protect != PAGE_READWRITE && mbi.Protect != PAGE_EXECUTE_READWRITE)
				{
					DWORD Protect;
					if (VirtualProtect((void*)start, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &Protect)) {
						RtlZeroMemory((void*)start, mbi.RegionSize);
						VirtualProtect((void*)start, mbi.RegionSize, Protect, &Protect);
					}
				}
				else {
					RtlZeroMemory((void*)start, mbi.RegionSize);
				}

				start += mbi.RegionSize;
			}
		};
}

bool BFDrv::B_AdjustPrivilege()
{
	return B_AdjustPrivilegePtr();
}

bool BFDrv::B_InitDrv(const char* key, B_InstallMode mode)
{
	return B_InitDrvPtr(key, mode);
}

const char* BFDrv::B_GetInitResult()
{
	return B_GetInitResultPtr();
}

bool BFDrv::B_AttachProcess(int pid)
{
	return B_AttachProcessPtr(pid);
}

const char* BFDrv::B_GetExpiration()
{
	return B_GetExpirationPtr();
}

int BFDrv::B_GetWindowsBuildNumber()
{
	return B_GetWindowsBuildNumberPtr();
}

std::string BFDrv::B_GetDriverBuildTime()
{
	return B_GetDriverBuildTimePtr();
}

ULONG64 BFDrv::B_GetMoudleBaseAddress(const char* moduleName, ULONG* pSize)
{
	return B_GetMoudleBaseAddressPtr(moduleName, pSize);
}

ULONG64 BFDrv::B_GetMoudleBaseAddressNoAttach(const char* moduleName, ULONG* pSize)
{
	return B_GetMoudleBaseAddressNoAttachPtr(moduleName, pSize);
}

ULONG64 BFDrv::B_GetMoudleExportFuncAddress(ULONG64 moudleAddr, const char* funcName)
{
	return B_GetMoudleExportFuncAddressPtr(moudleAddr, funcName);
}

bool BFDrv::B_ReadMemory(ULONG64 addr, void* buffer, size_t size, RWMode mode, ULONG64 cr3)
{
	return B_ReadMemoryPtr(addr, buffer, size, mode, cr3);
}

bool BFDrv::B_WriteMemory(ULONG64 addr, void* buffer, size_t size, RWMode mode, ULONG64 cr3)
{
	return B_WriteMemoryPtr(addr, buffer, size, mode, cr3);
}

ULONG64 BFDrv::B_AllocMemory(size_t size)
{
	return B_AllocMemoryPtr(size);
}

bool BFDrv::B_FreeMemory(ULONG64 addr)
{
	return B_FreeMemoryPtr(addr);
}

bool BFDrv::B_ProtectMemory(ULONG64 addr, DWORD newProtect, DWORD64 size)
{
	return B_ProtectMemoryPtr(addr, newProtect, size);
}

void BFDrv::B_MouseMove(int x, int y, MoveType type)
{
	return B_MouseMovePtr(x, y, type);
}

void BFDrv::B_MouseCtl(MouseKey key, MouseStatus status)
{
	return B_MouseCtlPtr(key, status);
}

void BFDrv::B_KeyCtl(int vKey, KeyStatus status)
{
	return B_KeyCtlPtr(vKey, status);
}

void BFDrv::B_KeyCtl(char key, KeyStatus status)
{
	return B_KeyCtlCharPtr(key, status);
}

void BFDrv::B_KeyClick(int vKey)
{
	return B_KeyClickPtr(vKey);
}

void BFDrv::B_KeyClick(char key)
{
	return B_KeyClickCharPtr(key);
}

bool BFDrv::B_ProtectProcess(bool protect, int pid)
{
	return B_ProtectProcessPtr(protect, pid);
}

bool BFDrv::B_ProtectProcessV2(bool protect, int pid)
{
	return B_ProtectProcessV2Ptr(protect, pid);
}

bool BFDrv::B_HideProcess(bool hide, int pid)
{
	return B_HideProcessPtr(hide, pid);
}

bool BFDrv::B_HideWindow(ULONG64 hWnd, HideWindowType type)
{
	return B_HideWindowPtr(hWnd, type);
}

bool BFDrv::B_MapDll(const char* dll_path)
{
	std::fstream file(dll_path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return false;
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return B_ManualMapDllMemoryPtr(str.data(), str.size());
}

bool BFDrv::B_MapDll(unsigned char* dll_data, size_t dll_size)
{
	return B_ManualMapDllMemoryPtr(dll_data, dll_size);
}

bool BFDrv::B_MapDLLV2(unsigned char* dll_data, size_t dll_size)
{
	return B_MapDLLPtr(dll_data, dll_size);
}

bool BFDrv::B_MapDLLV2(const char* dll_path)
{
	std::fstream file(dll_path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return false;
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	return B_MapDLLPtr((unsigned char*)str.data(), str.size());
}

bool BFDrv::B_MapDLLV3(unsigned char* dll_data, size_t dll_size, bool hide_mem)
{
	return B_APCInjectDLLPtr(dll_data, dll_size, hide_mem);
}

bool BFDrv::B_MapDLLV3(const char* dll_path, bool hide_mem)
{
	std::fstream file(dll_path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return false;
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	return B_APCInjectDLLPtr((unsigned char*)str.data(), str.size(), hide_mem);
}

bool BFDrv::B_MapDLLV4(unsigned char* dll_data, size_t dll_size, bool hide_mem, bool clear_shellcode)
{
	return B_RipInjectDLLPtr(dll_data, dll_size, hide_mem, clear_shellcode);
}

bool BFDrv::B_MapDLLV4(const char* dll_path, bool hide_mem, bool clear_shellcode)
{
	std::fstream file(dll_path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return false;
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	return B_RipInjectDLLPtr((unsigned char*)str.data(), str.size(), hide_mem, clear_shellcode);
}

bool BFDrv::B_MapDLLV5(unsigned char* dll_data, size_t dll_size, bool hide_mem, bool clear_shellcode)
{
	return B_RipInjectDLLV2Ptr(dll_data, dll_size, hide_mem, clear_shellcode);
}

bool BFDrv::B_MapDLLV5(const char* dll_path, bool hide_mem, bool clear_shellcode)
{
	std::fstream file(dll_path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return false;
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	return B_RipInjectDLLV2Ptr((unsigned char*)str.data(), str.size(), hide_mem, clear_shellcode);
}

ULONG64 BFDrv::B_GetProcessRealCr3()
{
	return B_GetProcessRealCr3Ptr();
}

ULONG64 BFDrv::B_GetProcessRealCr3Attach()
{
	return B_GetProcessRealCr3AttachPtr();
}

bool BFDrv::B_PhyReadMemoryWithCr3(ULONG64 addr, void* buffer, size_t size, ULONG64 cr3)
{
	return B_PhyReadMemoryWithCr3Ptr(addr, buffer, size, cr3);
}

bool BFDrv::B_PhyWriteMemoryWithCr3(ULONG64 addr, void* buffer, size_t size, ULONG64 cr3)
{
	return B_PhyWriteMemoryWithCr3Ptr(addr, buffer, size, cr3);
}

void BFDrv::B_ProtectWindow(bool protectWindow, ULONG protectWindowPid, bool protectGlobal)
{
	return B_ProtectWindowPtr(protectWindow, protectWindowPid, protectGlobal);
}

bool BFDrv::B_ForceDeleteFile(const char* filePath)
{
	return B_ForceDeleteFilePtr(filePath);
}

ULONG64 BFDrv::B_FindPatternV1(ULONG64 addr, ULONG64 size, const char* pattern, const char* mask, RWMode mode)
{
	return B_FindPatternV1Ptr(addr, size, pattern, mask, mode);
}

ULONG64 BFDrv::B_FindPatternV2(ULONG64 addr, ULONG64 size, const char* pattern, RWMode mode)
{
	return B_FindPatternV2Ptr(addr, size, pattern, mode);
}

std::vector<ULONG64> BFDrv::B_AOBScanV1(const char* pattern, const char* mask, ULONG64 addr, ULONG64 size, RWMode mode)
{
	return B_AOBScanV1Ptr(pattern, mask, addr, size, mode);
}

std::vector<ULONG64> BFDrv::B_AOBScanV2(const char* pattern, ULONG64 addr, ULONG64 size, RWMode mode)
{
	return B_AOBScanV2Ptr(pattern, addr, size, mode);
}

bool BFDrv::B_QueryMemory(ULONG64 addr, MEMORY_BASIC_INFORMATION* mbi)
{
	return B_QueryMemoryPtr(addr, mbi);
}

bool BFDrv::B_DumpToFile(ULONG64 moduleBase, ULONG64 moduleSize, const char* filePath, RWMode mode)
{
	return B_DumpToFilePtr(moduleBase, moduleSize, filePath, mode);
}

bool BFDrv::B_RWKernelMemory(ULONG64 addr, void* buffer, ULONG size, int type)
{
	return B_RWKernelMemoryPtr(addr, buffer, size, type);
}

bool BFDrv::B_DisableCallback_NMI()
{
	return B_DisableCallback_NMIPtr();
}
