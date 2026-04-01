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
#include "BFDrv_Dynamic.c"

template<typename Ret, typename... Args>
using FunctionPtr = Ret(WINAPI*)(Args...);

#define DECLARE_FUNC_PTR(name, ret, ...) \
    using name##Func = FunctionPtr<ret, ##__VA_ARGS__>; \
    name##Func name##Ptr = nullptr;

DECLARE_FUNC_PTR(B_InitDrv, B_STATUS, const char*, B_InstallMode, ULONG)
DECLARE_FUNC_PTR(B_GetInitResult, const char*)
DECLARE_FUNC_PTR(B_GetExpiration, const char*)
DECLARE_FUNC_PTR(B_EnableComm2, bool)
DECLARE_FUNC_PTR(B_AttachProcess, bool, int)
DECLARE_FUNC_PTR(B_GetWindowsBuildNumber, int)
DECLARE_FUNC_PTR(B_GetMainModuleAddress, ULONG64)
DECLARE_FUNC_PTR(B_GetModuleBaseAddress, ULONG64, const char*, ULONG*)
DECLARE_FUNC_PTR(B_GetKernelModule, ULONG64, const char*, ULONG*)
DECLARE_FUNC_PTR(B_GetModuleBaseAddressNoAttach, ULONG64, const char*, ULONG*)
DECLARE_FUNC_PTR(B_ReadMemory, bool, ULONG64, void*, size_t, RWMode, ULONG64)
DECLARE_FUNC_PTR(B_WriteMemory, bool, ULONG64, void*, size_t, RWMode, ULONG64)
DECLARE_FUNC_PTR(B_AllocMemory, ULONG64, size_t)
DECLARE_FUNC_PTR(B_FreeMemory, bool, ULONG64)
DECLARE_FUNC_PTR(B_ProtectMemory, bool, ULONG64, DWORD, DWORD64)
DECLARE_FUNC_PTR(B_MouseMove, void, int, int, MoveType)
DECLARE_FUNC_PTR(B_MouseCtl, void, MouseKey, MouseStatus)
DECLARE_FUNC_PTR(B_KeyCtl, void, int, KeyStatus, bool)
DECLARE_FUNC_PTR(B_KeyCtlChar, void, char, KeyStatus, bool)
DECLARE_FUNC_PTR(B_KeyClick, void, int)
DECLARE_FUNC_PTR(B_KeyClickChar, void, char)
DECLARE_FUNC_PTR(B_ProtectProcess, bool, bool, int)
DECLARE_FUNC_PTR(B_ProtectProcessV2, bool, bool, int)
DECLARE_FUNC_PTR(B_HideProcess, bool, bool, int)
DECLARE_FUNC_PTR(B_HideWindow, bool, ULONG64, HideWindowType)
DECLARE_FUNC_PTR(B_GetModuleExportFuncAddress, ULONG64, ULONG64, const char*)
DECLARE_FUNC_PTR(B_InjectDll, std::pair<ULONG64, ULONG64>, unsigned char*, size_t, InjectType, bool, bool, bool)
DECLARE_FUNC_PTR(B_ProtectWindow, void, bool, ULONG, bool)
DECLARE_FUNC_PTR(B_GetProcessRealCr3, ULONG64)
DECLARE_FUNC_PTR(B_GetProcessRealCr3Attach, ULONG64)
DECLARE_FUNC_PTR(B_ForceDeleteFile, bool, const wchar_t*)
DECLARE_FUNC_PTR(B_FindPatternV1, ULONG64, ULONG64, ULONG64, const char*, const char*, RWMode)
DECLARE_FUNC_PTR(B_FindPatternV2, ULONG64, ULONG64, ULONG64, const char*, RWMode)
DECLARE_FUNC_PTR(B_AOBScanV1, std::vector<ULONG64>, const char*, const char*, ULONG64, ULONG64, RWMode)
DECLARE_FUNC_PTR(B_AOBScanV2, std::vector<ULONG64>, const char*, ULONG64, ULONG64, RWMode)
DECLARE_FUNC_PTR(B_QueryMemory, bool, ULONG64, MEMORY_BASIC_INFORMATION*)
DECLARE_FUNC_PTR(B_DumpToFile, bool, ULONG64, const char*, RWMode)
DECLARE_FUNC_PTR(B_RWKernelMemory, bool, ULONG64, void*, ULONG, int)
DECLARE_FUNC_PTR(B_HideMemory, bool, ULONG64, ULONG64, HideMem)
DECLARE_FUNC_PTR(B_DisableCallback_NMI, bool)
DECLARE_FUNC_PTR(B_DisablePrint, void, bool)
DECLARE_FUNC_PTR(B_FileOverride, void, bool)
DECLARE_FUNC_PTR(B_TestMode, void, bool)
DECLARE_FUNC_PTR(B_DisableCallback, void, const char*)
DECLARE_FUNC_PTR(B_RestoreCallback, void)
DECLARE_FUNC_PTR(B_RemoveVAD, void, bool)
DECLARE_FUNC_PTR(B_CheckCr3ValidWithPhy, void, bool)
DECLARE_FUNC_PTR(B_SuspendProcess, bool, bool, int)
DECLARE_FUNC_PTR(B_ProtectCE, bool, const wchar_t*, HWND)
DECLARE_FUNC_PTR(B_GetAllTeb, std::vector<ULONG64>)
DECLARE_FUNC_PTR(B_DSEHook, bool, bool)
DECLARE_FUNC_PTR(B_CustomSigLoader, void, std::vector<unsigned char>)
DECLARE_FUNC_PTR(B_KillProcess, bool, int)

DECLARE_FUNC_PTR(B_GetMemoryInfo__, bool, void*)
DECLARE_FUNC_PTR(B_ReadPhyMemoryDirect, bool, ULONG64, PVOID, ULONG)
DECLARE_FUNC_PTR(B_WritePhyMemoryDirect, bool, ULONG64, PVOID, ULONG)

BFDrv::BFDrv()
{
	auto handle = MemoryLoadLibrary(BFDrv_Dynamic, sizeof BFDrv_Dynamic);
	if (!handle)  {
		MessageBoxW(0, L"DynamicLibrary load failed", L"Init Error", MB_OK);
		exit(1);
	}

	ZeroMemory(BFDrv_Dynamic, sizeof BFDrv_Dynamic);

	auto setFunctionPtr = [&](auto& funcPtr, const char* funcName) {
		funcPtr = reinterpret_cast<std::decay_t<decltype(funcPtr)>>(MemoryGetProcAddress(handle, funcName));
		return funcPtr != nullptr;
		};

#define SET_FUNC_PTR(name) \
    succeed &= setFunctionPtr(name##Ptr, #name);

	bool succeed = true;
	SET_FUNC_PTR(B_InitDrv)
	SET_FUNC_PTR(B_GetInitResult)
	SET_FUNC_PTR(B_GetExpiration)
	SET_FUNC_PTR(B_EnableComm2)
	SET_FUNC_PTR(B_AttachProcess)
	SET_FUNC_PTR(B_GetWindowsBuildNumber)
	SET_FUNC_PTR(B_GetMainModuleAddress)
	SET_FUNC_PTR(B_GetModuleBaseAddress)
	SET_FUNC_PTR(B_GetKernelModule)
	SET_FUNC_PTR(B_GetModuleBaseAddressNoAttach)
	SET_FUNC_PTR(B_ReadMemory)
	SET_FUNC_PTR(B_WriteMemory)
	SET_FUNC_PTR(B_AllocMemory)
	SET_FUNC_PTR(B_FreeMemory)
	SET_FUNC_PTR(B_ProtectMemory)
	SET_FUNC_PTR(B_MouseMove)
	SET_FUNC_PTR(B_MouseCtl)
	SET_FUNC_PTR(B_KeyCtl)
	SET_FUNC_PTR(B_KeyCtlChar)
	SET_FUNC_PTR(B_KeyClick)
	SET_FUNC_PTR(B_KeyClickChar)
	SET_FUNC_PTR(B_ProtectProcess)
	SET_FUNC_PTR(B_ProtectProcessV2)
	SET_FUNC_PTR(B_HideProcess)
	SET_FUNC_PTR(B_HideWindow)
	SET_FUNC_PTR(B_GetModuleExportFuncAddress)
	SET_FUNC_PTR(B_InjectDll)
	SET_FUNC_PTR(B_ProtectWindow)
	SET_FUNC_PTR(B_GetProcessRealCr3)
	SET_FUNC_PTR(B_GetProcessRealCr3Attach)
	SET_FUNC_PTR(B_ForceDeleteFile)
	SET_FUNC_PTR(B_FindPatternV1)
	SET_FUNC_PTR(B_FindPatternV2)
	SET_FUNC_PTR(B_AOBScanV1)
	SET_FUNC_PTR(B_AOBScanV2)
	SET_FUNC_PTR(B_QueryMemory)
	SET_FUNC_PTR(B_DumpToFile)
	SET_FUNC_PTR(B_RWKernelMemory)
	SET_FUNC_PTR(B_HideMemory)
	SET_FUNC_PTR(B_DisableCallback_NMI)
	SET_FUNC_PTR(B_DisablePrint)
	SET_FUNC_PTR(B_FileOverride)
	SET_FUNC_PTR(B_TestMode)
	SET_FUNC_PTR(B_DisableCallback)
	SET_FUNC_PTR(B_RestoreCallback)
	SET_FUNC_PTR(B_RemoveVAD)
	SET_FUNC_PTR(B_CheckCr3ValidWithPhy)
	SET_FUNC_PTR(B_SuspendProcess)
	SET_FUNC_PTR(B_ProtectCE)
	SET_FUNC_PTR(B_GetAllTeb)
	SET_FUNC_PTR(B_DSEHook)
	SET_FUNC_PTR(B_CustomSigLoader)
	SET_FUNC_PTR(B_KillProcess)
	SET_FUNC_PTR(B_GetMemoryInfo__)
	SET_FUNC_PTR(B_ReadPhyMemoryDirect)
	SET_FUNC_PTR(B_WritePhyMemoryDirect)

	if (!succeed) {
		MessageBoxW(0, L"Failed to set function", L"Init Error", MB_OK);
		exit(1);
	}

	PMEMORYMODULE module = (PMEMORYMODULE)handle;
	ClearPEHeadersEx(reinterpret_cast<unsigned char*>(module->codeBase));
#ifdef _WIN64
	UnlinkModule(reinterpret_cast<unsigned char*>(module->codeBase));
#endif
}

B_STATUS BFDrv::B_InitDrv(const char* key, B_InstallMode mode, PdbMode pdbMode)
{
	return B_InitDrvPtr(key, mode, pdbMode);
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

bool BFDrv::B_EnableComm2()
{
	return B_EnableComm2Ptr();
}

int BFDrv::B_GetWindowsBuildNumber()
{
	return B_GetWindowsBuildNumberPtr();
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

void BFDrv::B_KeyCtl(int vKey, KeyStatus status, bool extendedKey)
{
	return B_KeyCtlPtr(vKey, status, extendedKey);
}

void BFDrv::B_KeyCtl(char key, KeyStatus status, bool extendedKey)
{
	return B_KeyCtlCharPtr(key, status, extendedKey);
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

std::pair<ULONG64, ULONG64> BFDrv::B_InjectDll(const char* dll_path, InjectType type, bool hide_mem, bool clear_pe, bool clear_shellcode)
{
	std::fstream file(dll_path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return { 0,0 };
	std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return B_InjectDll((unsigned char*)str.data(), str.size(), type, hide_mem, clear_pe, clear_shellcode);
}

std::pair<ULONG64, ULONG64> BFDrv::B_InjectDll(unsigned char* dll_data, ULONG64 dll_size, InjectType type, bool hide_mem, bool clear_pe, bool clear_shellcode)
{
	auto result = B_InjectDllPtr(dll_data, dll_size, type, hide_mem, clear_pe, clear_shellcode);
	if (result.first && result.second) {
		if (hide_mem && type != IT_APC) {//APC的隐藏在内核中进行
			//开始隐藏 可以根据需要修改内存的属性HideMem
			B_HideMemory(result.first, result.second, HideMem::HM_READONLY);
		}
	}
	return result;
}

ULONG64 BFDrv::B_GetProcessRealCr3()
{
	return B_GetProcessRealCr3Ptr();
}

ULONG64 BFDrv::B_GetProcessRealCr3Attach()
{
	return B_GetProcessRealCr3AttachPtr();
}

void BFDrv::B_ProtectWindow(bool protectWindow, ULONG protectWindowPid, bool protectGlobal)
{
	return B_ProtectWindowPtr(protectWindow, protectWindowPid, protectGlobal);
}

bool BFDrv::B_ForceDeleteFile(const wchar_t* filePath)
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

bool BFDrv::B_DumpToFile(ULONG64 moduleBase, const char* filePath, RWMode mode)
{
	return B_DumpToFilePtr(moduleBase, filePath, mode);
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

void BFDrv::B_FileOverride(bool enable)
{
	return B_FileOverridePtr(enable);
}

void BFDrv::B_DisableCallback(const char* driver_name)
{
	return B_DisableCallbackPtr(driver_name);
}

void BFDrv::B_RestoreCallback()
{
	return B_RestoreCallbackPtr();
}

void BFDrv::B_RemoveVAD(bool enable)
{
	return B_RemoveVADPtr(enable);
}

void BFDrv::B_CheckCr3ValidWithPhy(bool enable)
{
	return B_CheckCr3ValidWithPhyPtr(enable);
}

bool BFDrv::B_SuspendProcess(bool is_suspend, int pid)
{
	return B_SuspendProcessPtr(is_suspend, pid);
}

bool BFDrv::B_ProtectCE(const wchar_t* ce_process_name, HWND ce_hwnd)
{
	return B_ProtectCEPtr(ce_process_name, ce_hwnd);
}

std::vector<ULONG64> BFDrv::B_GetAllTeb()
{
	return B_GetAllTebPtr();
}

bool BFDrv::B_DSEHook(bool enable)
{
	return B_DSEHookPtr(enable);
}

void BFDrv::B_CustomSigLoader(std::vector<unsigned char> data)
{
	return B_CustomSigLoaderPtr(data);
}

bool BFDrv::B_KillProcess(int pid)
{
	return B_KillProcessPtr(pid);
}

bool BFDrv::B_GetMemoryInfo__(void* data)
{
	return B_GetMemoryInfo__Ptr(data);
}

bool BFDrv::B_ReadPhyMemoryDirect(ULONG64 phy_addr, PVOID buffer, ULONG size)
{
	return B_ReadPhyMemoryDirectPtr(phy_addr, buffer, size);
}

bool BFDrv::B_WritePhyMemoryDirect(ULONG64 phy_addr, PVOID buffer, ULONG size)
{
	return B_WritePhyMemoryDirectPtr(phy_addr, buffer, size);
}
