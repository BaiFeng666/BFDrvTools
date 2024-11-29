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

#include <Windows.h>
#include <iostream>
#include <string.h>
#include <TlHelp32.h>
#include <atlconv.h>
#include "../driver/BFDrvUtils.h"
#include "../driver/TestDLL.c"

DWORD GetProcessID(const char* ProcessName)
{
	PROCESSENTRY32 ProcessInfoPE;
	ProcessInfoPE.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(15, 0);
	Process32First(hSnapshot, &ProcessInfoPE);
	USES_CONVERSION;
	do {
		if (wcscmp(ProcessInfoPE.szExeFile, A2W(ProcessName)) == 0)
		{
			CloseHandle(hSnapshot);
			return ProcessInfoPE.th32ProcessID;
		}
	} while (Process32Next(hSnapshot, &ProcessInfoPE));
	CloseHandle(hSnapshot);
	return 0;
}

int main()
{
	BFDrv Drv;
	//Drv.B_DisablePrint(true);//禁用一些提示输出

	/*初始化驱动 输入卡密
	* 为了更多人能用上驱动 所以是自适应式计费法
	* 卡密计时方式： 时间 + 异地登录扣时
	* 初次登录绑定开发主机 开发主机上随意登录调试不会扣时间
	* 其他机器登录会扣2分钟（检测到其他机器有开发者行为的 一次扣十分钟 防止卡密分享）
	* 电脑开机后首次调用B_InitDrv()会安装驱动，所以需要管理员权限，后续调用B_InitDrv()无需管理员权限*/

	/*首次安装时可以检测一些驱动是否在运行 如果在运行就取消安装驱动的行为 安全性提高
	* 如果你无所谓 那么把B_InitDrv的参数delectDrivers填false即可忽视
	* 可以自定义修改 我这里随意写了几个 要检测哪些驱动正在运行 模糊匹配
	* 不需要的话该参数直接不填
	*/
	std::vector<const char*> delectDriverList = { "WeGame","ACE-", "AntiCheat", "BEDaisy" };

	auto result = Drv.B_InitDrv("", B_InstallMode::Normal, false, delectDriverList);

	std::cout << Drv.B_GetInitResult() << "\n";

	if (result != BF_OK) {
		std::cout << "初始化失败, 原因: 0x" << result << "\n";
		system("pause");
		return 0;
	}

	auto timeOver = Drv.B_GetExpiration();
	std::cout << "截至时间" << timeOver << "\n";

	auto winVer = Drv.B_GetWindowsBuildNumber();
	std::cout << "系统版本: " << winVer << "\n";

	auto lastBuildTime = Drv.B_GetDriverBuildTime();
	std::cout << "最后编译时间: " << lastBuildTime << "\n";

	int localPid = GetCurrentProcessId();
	HANDLE hProcess = GetCurrentProcess();

	/* Dump模块至文件
	int valorant = GetProcessID("VALORANT-Win64-Shipping.exe");
	if (valorant) {
		std::cout << "Dump模块至文件" << "\n";
		printf("VALORANT localPid: %d\n", valorant);
		Drv.B_AttachProcess(valorant);
		ULONG size = 0;
		auto base = Drv.B_GetModuleBaseAddress("VALORANT-Win64-Shipping.exe", &size);
		printf("valorant base: %llx, size: %lx\n", base, size);
		if (!Drv.B_DumpToFile(base, size, "D:\\桌面\\VALORANT_Dump.exe", RWMode::MmCopy)) {
			printf("DumpToFile 失败\n");
			system("pause");
		}
		printf("DumpToFile 成功\n");
		system("pause");
	}*/
	//操作进程前必须要附加
	Drv.B_AttachProcess(localPid);

	//获取模块基址大小
	ULONG size = 0;
	auto moduleBase = Drv.B_GetModuleBaseAddress("SimpleExample.exe", &size);
	auto moduleBase2 = Drv.B_GetMainModuleAddress();
	//auto moduleBase3 = Drv.B_GetModuleBaseAddressNoAttach("SimpleExample.exe");
	std::cout << "模块基址: " << std::hex << moduleBase << " 大小: " << size << "\n";
	std::cout << "模块基址2: " << std::hex << moduleBase2 << "\n";
	//std::cout << "模块基址3: " << std::hex << moduleBase3 << "\n";

	auto kernelDll = Drv.B_GetModuleBaseAddress("Kernel32.dll");
	auto IsBadReadPtr = Drv.B_GetModuleExportFuncAddress(kernelDll, "IsBadReadPtr");
	printf("获取模块导出函数 IsbadReadPtr: %llx\n", IsBadReadPtr);

	//读写测试
	uintptr_t readValue = 0;

	ReadProcessMemory(hProcess, (void*)moduleBase, &readValue, sizeof(readValue), 0);
	std::cout << "API read value: " << std::hex << readValue << "\n";

	readValue = 0;
	readValue = Drv.B_ReadMem<uintptr_t>(moduleBase, RWMode::Ke);
	std::cout << "Ke read value: " << std::hex << readValue << "\n";

	readValue = 0;
	readValue = Drv.B_ReadMem<uintptr_t>(moduleBase, RWMode::KeSafe);
	std::cout << "KeSafe read value: " << std::hex << readValue << "\n";

	readValue = 0;
	readValue = Drv.B_ReadMem<uintptr_t>(moduleBase, RWMode::Mdl);
	std::cout << "Mdl read value: " << std::hex << readValue << "\n";

	readValue = 0;
	readValue = Drv.B_ReadMem<uintptr_t>(moduleBase, RWMode::MmCopy);
	std::cout << "MmCopy read value: " << std::hex << readValue << "\n";

	readValue = 0;
	readValue = Drv.B_ReadMem<uintptr_t>(moduleBase, RWMode::Phy);
	//也可以这样
	//static ULONG64 cr3 = Drv.B_GetProcessRealCr3(); //执行一次即可 自己将返回值保存下来用于后续读取
	//readValue = Drv.B_ReadMem<uintptr_t>(moduleBase, RWMode::Phy, cr3);
	std::cout << "Phy read value: " << std::hex << readValue << "\n";

	//无视CR3加密读取数值
	readValue = 0;
	static ULONG64 cr3 = Drv.B_GetProcessRealCr3(); //执行一次即可 自己将返回值保存下来用于后续读取
	static ULONG64 cr3_attach = Drv.B_GetProcessRealCr3Attach();//同上
	std::cout << "-----> cr3 value: " << std::hex << cr3 << "\n";
	std::cout << "-----> read cr3 value: " << std::hex << cr3_attach << "\n";
	readValue = Drv.B_PhyReadMemWithCr3<uintptr_t>(moduleBase, cr3/*cr3_attach*/);
	std::cout << "Fuck CR3 read value: " << std::hex << readValue << "\n";

	int writeValue = 0;
	const int newValue = 100;

	WriteProcessMemory(hProcess, &writeValue, &newValue, sizeof(newValue), 0);
	std::cout << "API write value: " << std::dec << writeValue << "\n";

	writeValue = 0;
	Drv.B_WriteMem((ULONG64)&writeValue, &newValue, sizeof(newValue), RWMode::Phy);
	//也可以这样
	//ULONG64 cr3 = Drv.B_GetProcessRealCr3(); //执行一次即可 自己将返回值保存下来用于后续读取
	//Drv.B_WriteMem((ULONG64)&writeValue, &newValue, sizeof(newValue), RWMode::Phy, cr3);
	std::cout << "Phy write value: " << std::dec << writeValue << "\n";

	writeValue = 0;
	Drv.B_WriteMem((ULONG64)&writeValue, &newValue, sizeof(newValue), RWMode::Ke);
	std::cout << "Ke write value: " << std::dec << writeValue << "\n";

	writeValue = 0;
	Drv.B_WriteMem((ULONG64)&writeValue, &newValue, sizeof(newValue), RWMode::KeSafe);
	std::cout << "KeSafe write value: " << std::dec << writeValue << "\n";

	writeValue = 0;
	Drv.B_WriteMem((ULONG64)&writeValue, &newValue, sizeof(newValue), RWMode::Mdl);
	std::cout << "Mdl write value: " << std::dec << writeValue << "\n";

	writeValue = 0;
	Drv.B_WriteMem((ULONG64)&writeValue, &newValue, sizeof(newValue), RWMode::MmCopy);
	std::cout << "MmCopy write value: " << std::dec << writeValue << "\n";

	writeValue = 0;
	Drv.B_PhyWriteMemoryWithCr3((ULONG64)&writeValue, (void*)&newValue, sizeof(newValue), cr3/*read_cr3*/);
	std::cout << "Fuck CR3 write value: " << std::dec << writeValue << "\n";

	//内核键鼠模拟
	/*
	std::cout << "鼠标移动测试 向右移动十次 间隔200ms\n";
	//鼠标移动
	for (int i = 0; i < 10; i++) {
		Drv.B_MouseMove(20, 0, MoveType::Relative);//相对模式移动鼠标往右
		Sleep(200);
	}

	std::cout << "即将 鼠标左键点击五下 间隔500ms\n";
	for (int i = 0; i < 5; i++) {
		Drv.B_MouseCtl(MouseKey::LEFT, MouseStatus::DOWN);
		Drv.B_MouseCtl(MouseKey::LEFT, MouseStatus::UP);
		Sleep(500);
	}

	std::cout << "按键模拟测试 按五下a 间隔500ms\n";
	//按五下a
	for (int i = 0; i < 5; i++) {
		Drv.B_KeyClick('a');
		//Drv.B_KeyClick(0x61);
		Sleep(500);
	}*/

	std::cout << "即将 保护/隐藏自身 防止CE等调试工具附加程序 任务管理器也无法查看\n";
	system("pause");

	Drv.B_ProtectProcess(true, GetCurrentProcessId());
	Drv.B_HideProcess(true, GetCurrentProcessId());

	std::cout << "即将 取消保护/隐藏自身\n";
	system("pause");
	Drv.B_ProtectProcess(false, GetCurrentProcessId());
	Drv.B_HideProcess(false, GetCurrentProcessId());

	HWND hWnd = 0;//自己获取要反截图的窗口句柄
	if (hWnd) {
		std::cout << "HWND: " << hWnd << "\n";
		std::cout << "即将 内核反截图\n";
		system("pause");
		if (!Drv.B_HideWindow((ULONG64)hWnd, HideWindowType::Excludefromcapture)) {
			std::cout << "内核反截图失败\n";
		}

		std::cout << "即将 取消内核反截图\n";
		system("pause");
		Drv.B_HideWindow((ULONG64)hWnd, HideWindowType::None);
	}

	int notepadPid = GetProcessID("notepad.exe");
	if (notepadPid) {
		printf("notepad localPid: %d\n", notepadPid);
		Drv.B_AttachProcess(notepadPid);

		auto inject_result = Drv.B_InjectDll(TestDLL, sizeof TestDLL, IT_APC);//内存注入
		//Drv.B_InjectDll("C:\\TestDll.dll", IT_APC)	//路径注入
		if (inject_result.first != 0) {	
			printf("Dll 注入成功\n");
			printf("dll在目标进程中的基址：%llx，大小：%llx\n", inject_result.first, inject_result.second);
		}
		else {
			printf("Dll 注入失败\n");
		}

		ULONG size = 0;
		auto moduleBase = Drv.B_GetModuleBaseAddress("notepad.exe", &size);

		auto r3 = Drv.B_AOBScanV1("\xBF\x00\x00\x00\x00\x8B\x05", "x????xx",
			moduleBase, size);

		auto r4 = Drv.B_AOBScanV2("BF ? ? ? ? 8B 05", moduleBase, size);

		for (auto x : r3)
			printf("B_AOBScanV1 搜索结果: %llx\n", x);

		for (auto x : r4)
			printf("B_AOBScanV2 搜索结果: %llx\n", x);

		//保护窗口注意 写的是窗口的进程的PID
		//有时候会有多个同名进程 窗口各自属于的进程不相同 例如控制台 别写错PID了
		std::cout << "即将 保护窗口\n";
		system("pause");
		Drv.B_ProtectWindow(true, notepadPid);

		std::cout << "即将 取消保护窗口\n";
		system("pause");
		Drv.B_ProtectWindow(false, notepadPid);


		//保护、隐藏进程V2
		std::cout << "即将 保护进程V2\n";
		system("pause");
		Drv.B_ProtectProcessV2(true, notepadPid);

		std::cout << "即将 取消保护进程V2\n";
		system("pause");
		Drv.B_ProtectProcessV2(false, notepadPid); //在进程退出前建议恢复
	}

	Drv.B_AttachProcess(localPid);
	PVOID allocMem = VirtualAlloc(NULL, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (allocMem) {
		printf("即将隐藏内存 allocMem: %p\n", allocMem);
		system("pause");
		printf("内存改为只读属性\n");
		Drv.B_HideMemory((ULONG64)allocMem, 0x1000, HideMem::HM_READONLY);
		system("pause");
		printf("恢复内存可读可写可执行\n");
		Drv.B_HideMemory((ULONG64)allocMem, 0x1000, HideMem::HM_EXECUTE_READWRITE);
		system("pause");
	}


	ULONG ksize;
	auto kernelBase = Drv.B_GetKernelModule("ntoskrnl.exe", &ksize);
	std::cout << "ntoskrnl.exe模块基址: " << std::hex << kernelBase << " 大小: " << ksize << "\n";

	int kvalue;
	Drv.B_RWKernelMemory(kernelBase, &kvalue, 4, 0);
	std::cout << "read ntoskrnl.exe 4 bytes value: " << kvalue << "\n";

	//如果路径带中文 字符串要转成utf8编码
	//if (!Drv.B_ForceDeleteFile("C:\\Users\\18361\\Desktop\\test.txt")) {
	//	std::cout << "强删文件失败\n";
	//}

	//std::cout << "关闭NMI回调检测\n";
	//Drv.B_DisableCallback_NMI();
	//system("pause");

	//关闭指定驱动内核回调 可以同时关闭多个驱动 目前限制了最大关闭的回调数量是32个
	//注意：有的驱动会检测自身回调是否存在、修改，所以隔一段时间可能会被他恢复过去
	//Drv.B_DisableCallback("xxx.sys");
	//Drv.B_DisableCallback("xxx2.sys");
	//Drv.B_DisableCallback("xxx3.sys");
	//恢复所有关闭的内核回调
	//Drv.B_RestoreCallback();

	std::cout << "结束\n";
	system("pause");
}