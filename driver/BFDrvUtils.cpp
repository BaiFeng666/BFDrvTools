/*
* BFDriver官方驱动群 410342663
* 周150 月450
* 免责声明及使用条款:
* 本驱动仅用于保护游戏软件和正规商业软件使用!
* 不得将本驱动用于,木马,病毒,外挂,恶意插件,等非法软件!
* 一切损失及涉及的法律责任自负,驱动开发者不负任何责任及费用!
* 驱动开发者有本驱动保护产品著作权,他人不得对本驱动保护产品进行修改,破解,买卖!
* 使用本驱动编写任何软件,软件版权归软件作者所有,一切软件商业活动均与驱动开发者无关!
*/
#include "BFDrvUtils.h"
#pragma warning (disable:4005)
#include "MemoryModule.h"
#include <cstdio>
#pragma comment(lib,"ntdll.lib")

#include <stdexcept>
#include <fstream>
#include "B_MemoryModule.c"
#include "BFDrv_Dynamic.c"

template<typename Ret, typename... Args>
using FunctionPtr = Ret(WINAPI*)(Args...);

using B_LoadDynamicLibraryFn = FunctionPtr<bool, HMODULE*, LPVOID>; B_LoadDynamicLibraryFn B_LoadDynamicLibraryPtr = nullptr;

using B_InitDrvFunc = FunctionPtr<B_STATUS, const char*, B_InstallMode, bool, bool, std::vector<const char*>>; B_InitDrvFunc B_InitDrvPtr = nullptr;
using B_AdjustPrivilegeFunc = FunctionPtr<bool>; B_AdjustPrivilegeFunc B_AdjustPrivilegePtr = nullptr;
using B_GetInitResultFunc = FunctionPtr<const char*>; B_GetInitResultFunc B_GetInitResultPtr = nullptr;
using B_GetExpirationFunc = FunctionPtr<const char*>; B_GetExpirationFunc B_GetExpirationPtr = nullptr;
using B_AttachProcessFunc = FunctionPtr<bool, int>; B_AttachProcessFunc B_AttachProcessPtr = nullptr;
using B_GetWindowsBuildNumberFunc = FunctionPtr<int>; B_GetWindowsBuildNumberFunc B_GetWindowsBuildNumberPtr = nullptr;
using B_GetMainModuleAddressFunc = FunctionPtr<ULONG64>; B_GetMainModuleAddressFunc B_GetMainModuleAddressPtr = nullptr;
using B_GetModuleBaseAddressFunc = FunctionPtr<ULONG64, const char*, ULONG*>; B_GetModuleBaseAddressFunc B_GetModuleBaseAddressPtr = nullptr;
using B_GetKernelModuleFunc = FunctionPtr<ULONG64, const char*, ULONG*>; B_GetKernelModuleFunc B_GetKernelModulePtr = nullptr;
using B_GetModuleBaseAddressNoAttachFunc = B_GetModuleBaseAddressFunc; B_GetModuleBaseAddressNoAttachFunc B_GetModuleBaseAddressNoAttachPtr = nullptr;
using B_ReadMemoryFunc = FunctionPtr<bool, ULONG64, void*, size_t, RWMode, ULONG64>; B_ReadMemoryFunc B_ReadMemoryPtr = nullptr;
using B_WriteMemoryFunc = FunctionPtr<bool, ULONG64, void*, size_t, RWMode, ULONG64>; B_WriteMemoryFunc B_WriteMemoryPtr = nullptr;
using B_AllocMemoryFunc = FunctionPtr<ULONG64, size_t>; B_AllocMemoryFunc B_AllocMemoryPtr = nullptr;
using B_FreeMemoryFunc = FunctionPtr<bool, ULONG64>; B_FreeMemoryFunc B_FreeMemoryPtr = nullptr;
using B_ProtectMemoryFunc = FunctionPtr<bool, ULONG64, DWORD, DWORD64>; B_ProtectMemoryFunc B_ProtectMemoryPtr = nullptr;
using B_MouseMoveFunc = FunctionPtr<void, int, int, MoveType>; B_MouseMoveFunc B_MouseMovePtr = nullptr;
using B_MouseCtlFunc = FunctionPtr<void, MouseKey, MouseStatus>; B_MouseCtlFunc B_MouseCtlPtr = nullptr;
using B_KeyCtlFunc = FunctionPtr<void, int, KeyStatus>; B_KeyCtlFunc B_KeyCtlPtr = nullptr;
using B_KeyCtlCharFunc = FunctionPtr<void, char, KeyStatus>; B_KeyCtlCharFunc B_KeyCtlCharPtr = nullptr;
using B_KeyClickFunc = FunctionPtr<void, int>; B_KeyClickFunc B_KeyClickPtr = nullptr;
using B_KeyClickCharFunc = FunctionPtr<void, char>; B_KeyClickCharFunc B_KeyClickCharPtr = nullptr;
using B_ProtectProcessFunc = FunctionPtr<bool, bool, int>; B_ProtectProcessFunc B_ProtectProcessPtr = nullptr;
using B_ProtectProcessV2Func = B_ProtectProcessFunc; B_ProtectProcessV2Func B_ProtectProcessV2Ptr = nullptr;
using B_HideProcessFunc = FunctionPtr<bool, bool, int>; B_HideProcessFunc B_HideProcessPtr = nullptr;
using B_HideWindowFunc = FunctionPtr<bool, ULONG64, HideWindowType>; B_HideWindowFunc B_HideWindowPtr = nullptr;
using B_GetModuleExportFuncAddressFunc = FunctionPtr<ULONG64, ULONG64, const char*>; B_GetModuleExportFuncAddressFunc B_GetModuleExportFuncAddressPtr = nullptr;
using B_DX11_HijackMapDllFunc = FunctionPtr<bool, const void*, bool>; B_DX11_HijackMapDllFunc B_DX11_HijackMapDllPtr = nullptr;
using B_DX12_HijackMapDllFunc = FunctionPtr<bool, const void*, bool>; B_DX12_HijackMapDllFunc B_DX12_HijackMapDllPtr = nullptr;
using B_ProtectWindowFunc = FunctionPtr<void, bool, ULONG, bool>; B_ProtectWindowFunc B_ProtectWindowPtr = nullptr;
using B_GetProcessRealCr3Func = FunctionPtr<ULONG64>; B_GetProcessRealCr3Func B_GetProcessRealCr3Ptr = nullptr;
using B_GetProcessRealCr3AttachFunc = FunctionPtr<ULONG64>; B_GetProcessRealCr3AttachFunc B_GetProcessRealCr3AttachPtr = nullptr;
using B_PhyReadMemoryWithCr3Func = FunctionPtr<bool, ULONG64, void*, size_t, ULONG64>; B_PhyReadMemoryWithCr3Func B_PhyReadMemoryWithCr3Ptr = nullptr;
using B_PhyWriteMemoryWithCr3Func = FunctionPtr<bool, ULONG64, void*, size_t, ULONG64>; B_PhyWriteMemoryWithCr3Func B_PhyWriteMemoryWithCr3Ptr = nullptr;
using B_ForceDeleteFileFunc = FunctionPtr<bool, const char*>; B_ForceDeleteFileFunc B_ForceDeleteFilePtr = nullptr;
using B_FindPatternV1Func = FunctionPtr<ULONG64, ULONG64, ULONG64, const char*, const char*, RWMode>; B_FindPatternV1Func B_FindPatternV1Ptr = nullptr;
using B_FindPatternV2Func = FunctionPtr<ULONG64, ULONG64, ULONG64, const char*, RWMode>; B_FindPatternV2Func B_FindPatternV2Ptr = nullptr;
using B_AOBScanV1Func = FunctionPtr<std::vector<ULONG64>, const char*, const char*, ULONG64, ULONG64, RWMode>; B_AOBScanV1Func B_AOBScanV1Ptr = nullptr;
using B_AOBScanV2Func = FunctionPtr<std::vector<ULONG64>, const char*, ULONG64, ULONG64, RWMode>; B_AOBScanV2Func B_AOBScanV2Ptr = nullptr;
using B_QueryMemoryFunc = FunctionPtr<bool, ULONG64, MEMORY_BASIC_INFORMATION*>; B_QueryMemoryFunc B_QueryMemoryPtr = nullptr;
using B_MapDLLFunc = FunctionPtr<bool, unsigned char*, size_t>; B_MapDLLFunc B_MapDLLPtr = nullptr;
using B_APCInjectDLLFunc = FunctionPtr<bool, unsigned char*, size_t, bool>; B_APCInjectDLLFunc B_APCInjectDLLPtr = nullptr;
using B_RipInjectDLLFunc = FunctionPtr<bool, unsigned char*, size_t, bool, bool>; B_RipInjectDLLFunc B_RipInjectDLLPtr = nullptr;
using B_RipInjectDLLV2Func = FunctionPtr<bool, unsigned char*, size_t, bool, bool>; B_RipInjectDLLV2Func B_RipInjectDLLV2Ptr = nullptr;
using B_DumpToFileFunc = FunctionPtr<bool, ULONG64, ULONG64, const char*, RWMode>; B_DumpToFileFunc B_DumpToFilePtr = nullptr;
using B_GetDriverBuildTimeFunc = FunctionPtr<std::string>; B_GetDriverBuildTimeFunc B_GetDriverBuildTimePtr = nullptr;
using B_RWKernelMemoryFunc = FunctionPtr<bool, ULONG64, void*, ULONG, int>; B_RWKernelMemoryFunc B_RWKernelMemoryPtr = nullptr;
using B_HideMemoryFunc = FunctionPtr<bool, ULONG64, ULONG64, HideMem>; B_HideMemoryFunc B_HideMemoryPtr = nullptr;
using B_DisableCallback_NMIFunc = FunctionPtr<bool>; B_DisableCallback_NMIFunc B_DisableCallback_NMIPtr = nullptr;
using B_DisablePrintFunc = FunctionPtr<void, bool>; B_DisablePrintFunc B_DisablePrintPtr = nullptr;
using B_TestModeFunc = FunctionPtr<void, bool>; B_TestModeFunc B_TestModePtr = nullptr;
using B_DisableCallbackFunc = FunctionPtr<void, const char*>; B_DisableCallbackFunc B_DisableCallbackPtr = nullptr;
using B_RestoreCallbackFunc = FunctionPtr<void>; B_RestoreCallbackFunc B_RestoreCallbackPtr = nullptr;

BFDrv::BFDrv()
{
	HMEMORYMODULE handle = MemoryLoadLibrary(B_MemoryModule, sizeof B_MemoryModule);
	if (!handle) throw std::runtime_error("Dll Load is NULL");

	HMODULE hModule = nullptr;
	B_LoadDynamicLibraryPtr = reinterpret_cast<B_LoadDynamicLibraryFn>(MemoryGetProcAddress(handle, "B_LoadDynamicLibrary"));
	if (!B_LoadDynamicLibraryPtr) throw std::runtime_error("LDL is NULL");

	if (!B_LoadDynamicLibraryPtr(&hModule, BFDrv_Dynamic) || !hModule)
		throw std::runtime_error("Failed to load DLL");

	auto setFunctionPtr = [&](auto& funcPtr, const char* funcName) {
		funcPtr = reinterpret_cast<std::decay_t<decltype(funcPtr)>>(GetProcAddress(hModule, funcName));
		return funcPtr != nullptr;
		};


	bool succeed = true;
	succeed &= setFunctionPtr(B_InitDrvPtr, "B_InitDrv");
	succeed &= setFunctionPtr(B_AdjustPrivilegePtr, "B_AdjustPrivilege");
	succeed &= setFunctionPtr(B_GetInitResultPtr, "B_GetInitResult");
	succeed &= setFunctionPtr(B_GetExpirationPtr, "B_GetExpiration");
	succeed &= setFunctionPtr(B_AttachProcessPtr, "B_AttachProcess");
	succeed &= setFunctionPtr(B_GetWindowsBuildNumberPtr, "B_GetWindowsBuildNumber");
	succeed &= setFunctionPtr(B_GetMainModuleAddressPtr, "B_GetMainModuleAddress");
	succeed &= setFunctionPtr(B_GetModuleBaseAddressPtr, "B_GetModuleBaseAddress");
	succeed &= setFunctionPtr(B_GetKernelModulePtr, "B_GetKernelModule");
	succeed &= setFunctionPtr(B_GetModuleBaseAddressNoAttachPtr, "B_GetModuleBaseAddressNoAttach");
	succeed &= setFunctionPtr(B_ReadMemoryPtr, "B_ReadMemory");
	succeed &= setFunctionPtr(B_WriteMemoryPtr, "B_WriteMemory");
	succeed &= setFunctionPtr(B_AllocMemoryPtr, "B_AllocMemory");
	succeed &= setFunctionPtr(B_FreeMemoryPtr, "B_FreeMemory");
	succeed &= setFunctionPtr(B_ProtectMemoryPtr, "B_ProtectMemory");
	succeed &= setFunctionPtr(B_MouseMovePtr, "B_MouseMove");
	succeed &= setFunctionPtr(B_MouseCtlPtr, "B_MouseCtl");
	succeed &= setFunctionPtr(B_KeyCtlPtr, "B_KeyCtl");
	succeed &= setFunctionPtr(B_KeyCtlCharPtr, "B_KeyCtlChar");
	succeed &= setFunctionPtr(B_KeyClickPtr, "B_KeyClick");
	succeed &= setFunctionPtr(B_KeyClickCharPtr, "B_KeyClickChar");
	succeed &= setFunctionPtr(B_ProtectProcessPtr, "B_ProtectProcess");
	succeed &= setFunctionPtr(B_ProtectProcessV2Ptr, "B_ProtectProcessV2");
	succeed &= setFunctionPtr(B_HideProcessPtr, "B_HideProcess");
	succeed &= setFunctionPtr(B_HideWindowPtr, "B_HideWindow");
	succeed &= setFunctionPtr(B_GetModuleExportFuncAddressPtr, "B_GetModuleExportFuncAddress");
	succeed &= setFunctionPtr(B_DX11_HijackMapDllPtr, "B_DX11_HijackMapDll");
	succeed &= setFunctionPtr(B_DX12_HijackMapDllPtr, "B_DX12_HijackMapDll");
	succeed &= setFunctionPtr(B_ProtectWindowPtr, "B_ProtectWindow");
	succeed &= setFunctionPtr(B_GetProcessRealCr3Ptr, "B_GetProcessRealCr3");
	succeed &= setFunctionPtr(B_GetProcessRealCr3AttachPtr, "B_GetProcessRealCr3Attach");
	succeed &= setFunctionPtr(B_PhyReadMemoryWithCr3Ptr, "B_PhyReadMemoryWithCr3");
	succeed &= setFunctionPtr(B_PhyWriteMemoryWithCr3Ptr, "B_PhyWriteMemoryWithCr3");
	succeed &= setFunctionPtr(B_ForceDeleteFilePtr, "B_ForceDeleteFile");
	succeed &= setFunctionPtr(B_FindPatternV1Ptr, "B_FindPatternV1");
	succeed &= setFunctionPtr(B_FindPatternV2Ptr, "B_FindPatternV2");
	succeed &= setFunctionPtr(B_AOBScanV1Ptr, "B_AOBScanV1");
	succeed &= setFunctionPtr(B_AOBScanV2Ptr, "B_AOBScanV2");
	succeed &= setFunctionPtr(B_DumpToFilePtr, "B_DumpToFile");
	succeed &= setFunctionPtr(B_QueryMemoryPtr, "B_QueryMemory");
	succeed &= setFunctionPtr(B_MapDLLPtr, "B_MapDLL");
	succeed &= setFunctionPtr(B_APCInjectDLLPtr, "B_APCInjectDLL");
	succeed &= setFunctionPtr(B_RipInjectDLLPtr, "B_RipInjectDLL");
	succeed &= setFunctionPtr(B_RipInjectDLLV2Ptr, "B_RipInjectDLLV2");
	succeed &= setFunctionPtr(B_GetDriverBuildTimePtr, "B_GetDriverBuildTime");
	succeed &= setFunctionPtr(B_RWKernelMemoryPtr, "B_RWKernelMemory");
	succeed &= setFunctionPtr(B_HideMemoryPtr, "B_HideMemory");
	succeed &= setFunctionPtr(B_DisableCallback_NMIPtr, "B_DisableCallback_NMI");
	succeed &= setFunctionPtr(B_DisablePrintPtr, "B_DisablePrint");
	succeed &= setFunctionPtr(B_TestModePtr, "B_TestMode");
	succeed &= setFunctionPtr(B_DisableCallbackPtr, "B_DisableCallback");
	succeed &= setFunctionPtr(B_RestoreCallbackPtr, "B_RestoreCallback");

	if (!succeed) throw std::runtime_error("Failed to set function");

	PMEMORYMODULE module = (PMEMORYMODULE)handle;
	ClearPEHeadersEx(reinterpret_cast<unsigned char*>(module->codeBase));
	ClearPEHeadersEx(reinterpret_cast<unsigned char*>(hModule));
#ifdef _WIN64
	UnlinkModule(reinterpret_cast<unsigned char*>(module->codeBase));
	UnlinkModule(reinterpret_cast<unsigned char*>(hModule));
#endif
}

bool BFDrv::B_AdjustPrivilege()
{
	return B_AdjustPrivilegePtr();
}

B_STATUS BFDrv::B_InitDrv(const char* key, B_InstallMode mode, bool ignorePdb, bool delectDrivers, std::vector<const char*> delectList)
{
	return B_InitDrvPtr(key, mode, ignorePdb, delectDrivers, delectList);
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

ULONG64 BFDrv::B_GetMainModuleAddress()
{
	return B_GetMainModuleAddressPtr();
}

ULONG64 BFDrv::B_GetModuleBaseAddress(const char* moduleName, ULONG* pSize)
{
	return B_GetModuleBaseAddressPtr(moduleName, pSize);
}

ULONG64 BFDrv::B_GetModuleBaseAddressNoAttach(const char* moduleName, ULONG* pSize)
{
	return B_GetModuleBaseAddressNoAttachPtr(moduleName, pSize);
}

ULONG64 BFDrv::B_GetModuleExportFuncAddress(ULONG64 ModuleAddr, const char* funcName)
{
	return B_GetModuleExportFuncAddressPtr(ModuleAddr, funcName);
}

bool BFDrv::B_ReadMemory(ULONG64 addr, void* buffer, size_t size, RWMode mode, ULONG64 cr3)
{
	if ((addr & 0xFFFF000000000000) != 0) return false;
	return B_ReadMemoryPtr(addr, buffer, size, mode, cr3);
}

bool BFDrv::B_WriteMemory(ULONG64 addr, void* buffer, size_t size, RWMode mode, ULONG64 cr3)
{
	if ((addr & 0xFFFF000000000000) != 0) return false;
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

bool BFDrv::B_MapDll(const char* dll_path, bool clear_pe)
{
	std::fstream file(dll_path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return false;
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return B_DX11_HijackMapDllPtr(str.data(), clear_pe);
}

bool BFDrv::B_MapDll(unsigned char* dll_data, bool clear_pe)
{
	return B_DX11_HijackMapDllPtr(dll_data, clear_pe);
}

bool BFDrv::B_MapDllV2(unsigned char* dll_data, size_t dll_size)
{
	return B_MapDLLPtr(dll_data, dll_size);
}

bool BFDrv::B_MapDllV2(const char* dll_path)
{
	std::fstream file(dll_path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return false;
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	return B_MapDLLPtr((unsigned char*)str.data(), str.size());
}

bool BFDrv::B_MapDllV3(unsigned char* dll_data, size_t dll_size, bool hide_mem)
{
	return B_APCInjectDLLPtr(dll_data, dll_size, hide_mem);
}

bool BFDrv::B_MapDllV3(const char* dll_path, bool hide_mem)
{
	std::fstream file(dll_path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return false;
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	return B_APCInjectDLLPtr((unsigned char*)str.data(), str.size(), hide_mem);
}

bool BFDrv::B_MapDllV4(unsigned char* dll_data, size_t dll_size, bool hide_mem, bool clear_shellcode)
{
	return B_RipInjectDLLPtr(dll_data, dll_size, hide_mem, clear_shellcode);
}

bool BFDrv::B_MapDllV4(const char* dll_path, bool hide_mem, bool clear_shellcode)
{
	std::fstream file(dll_path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return false;
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	return B_RipInjectDLLPtr((unsigned char*)str.data(), str.size(), hide_mem, clear_shellcode);
}

bool BFDrv::B_MapDllV5(unsigned char* dll_data, size_t dll_size, bool hide_mem, bool clear_shellcode)
{
	return B_RipInjectDLLV2Ptr(dll_data, dll_size, hide_mem, clear_shellcode);
}

bool BFDrv::B_MapDllV5(const char* dll_path, bool hide_mem, bool clear_shellcode)
{
	std::fstream file(dll_path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return false;
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	return B_RipInjectDLLV2Ptr((unsigned char*)str.data(), str.size(), hide_mem, clear_shellcode);
}

bool BFDrv::B_MapDllV6(const char* dll_path, bool clear_pe)
{
	std::fstream file(dll_path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return false;
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return B_DX12_HijackMapDllPtr(str.data(), clear_pe);
}

bool BFDrv::B_MapDllV6(unsigned char* dll_data, bool clear_pe)
{
	return B_DX12_HijackMapDllPtr(dll_data, clear_pe);
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

ULONG64 BFDrv::B_GetKernelModule(const char* moduleName, ULONG* pSize)
{
	return B_GetKernelModulePtr(moduleName, pSize);
}

bool BFDrv::B_RWKernelMemory(ULONG64 addr, void* buffer, ULONG size, int type)
{
	return B_RWKernelMemoryPtr(addr, buffer, size, type);
}

bool BFDrv::B_HideMemory(ULONG64 addr, ULONG64 size, HideMem attr)
{
	return B_HideMemoryPtr(addr, size, attr);
}

bool BFDrv::B_DisableCallback_NMI()
{
	return B_DisableCallback_NMIPtr();
}

void BFDrv::B_DisablePrint(bool value)
{
	return B_DisablePrintPtr(value);
}

void BFDrv::B_TestMode(bool value)
{
	return B_TestModePtr(value);
}

void BFDrv::B_DisableCallback(const char* driver_name)
{
	return B_DisableCallbackPtr(driver_name);
}

void BFDrv::B_RestoreCallback()
{
	return B_RestoreCallbackPtr();
}
