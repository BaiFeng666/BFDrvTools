/*
BFDriver官方驱动群 410342663
周150 月450
便宜吗？这还不买吗牢底？
禁止用于违法用途
*/
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <vector>

enum B_InstallMode
{
	NtLoadDriver,
	Normal
};

enum class RWMode
{
	MmCopy,
	Mdl,
	Phy,
	Ke
};

enum class MoveType
{
	Relative,
	Absolute
};

enum class MouseKey
{
	LEFT,
	RIGHT,
	MIDDLE,
	X1,
	X2
};

enum class MouseStatus
{
	UP,
	DOWN
};

enum KeyStatus
{
	UP,
	DOWN
};

enum class HideWindowType
{
	None,
	Excludefromcapture,//透明
	Monitor//黑色
};


class BFDrv
{
public:
	BFDrv();

	//提升权限
	bool B_AdjustPrivilege();

	//初始化驱动
	//参数1 卡密
	//参数2 加载方式 NtLoadDriver更安全 Normal兼容性更好
	//参数3 如果电脑无法下载PDB符号文件 仍然可以加载驱动 （已知无pdb情况下无法使用B_ProtectProcessV2）
	bool B_InitDrv(const char* key, B_InstallMode mode = B_InstallMode::NtLoadDriver, bool ignorePdb = false);

	const char* B_GetInitResult();

	//附加你要操作的进程
	bool B_AttachProcess(int pid);

	//获取到期时间
	const char* B_GetExpiration();

	//获取系统版本
	int B_GetWindowsBuildNumber();

	//获取驱动最后编译的时间
	//有时候出问题了可以看看本地版本和云下发的驱动是否不对应 是不是要更新了
	std::string B_GetDriverBuildTime();

	//注意 不要在循环中疯狂调用 不然可能会走异常蓝屏
	//获取模块基址 大小
	//如果不需要获取模块大小 那么第二个参数填nullptr
	ULONG64 B_GetMoudleBaseAddress(const char* moduleName, ULONG* pSize = nullptr);

	//有bug 暂时不建议使用
	//无附加的获取模块基址 有时候会卡住 用有附加的也稳
	ULONG64 B_GetMoudleBaseAddressNoAttach(const char* moduleName, ULONG* pSize = nullptr);


	//获取模块导出函数
	//参数一 模块基址， 参数二 导出函数名
	ULONG64 B_GetMoudleExportFuncAddress(ULONG64 moudleAddr, const char* funcName);

	bool B_ReadMemory(ULONG64 addr, void* buffer, size_t size, RWMode mode, ULONG64 cr3 = 0);
	bool B_WriteMemory(ULONG64 addr, void* buffer, size_t size, RWMode mode, ULONG64 cr3 = 0);

	/*
	读写内存
	有5种读写模式 MDL MM KE PHY PHY (B_PhyReadMemoryWithCr3在下面)
	速度 KE > MDL > MM > PHY

	推荐使用PHY、MM

	由于目前是映射加载
	所以KE和MDL在读取长度过大的数据（例如字符串）或者不安全的地址时可能会蓝屏

	最后一个参数是cr3 仅当使用PHY时它才有用 可以忽略
	如果有的游戏有cr3加密 请用B_GetProcessRealCr3获取正确的cr3传入
	（EAC游戏建议用B_PhyReadMemoryWithCr3，因为这里的PHYCR3没有处理EAC的限速问题）
	*/
	template <typename type>
	type B_ReadMem(ULONG64 addr, RWMode mode, ULONG64 cr3 = 0)
	{
		type buff{};

		B_ReadMemory(addr, &buff, sizeof(type), mode, cr3);
		return buff;
	}

	template <typename type>
	type B_ReadMem(ULONG64 addr, size_t size, RWMode mode, ULONG64 cr3 = 0)
	{
		type buff{};

		B_ReadMemory(addr, &buff, size, mode, cr3);
		return buff;
	}

	template <typename Type>
	bool B_WriteMem(ULONG64 addr, Type* type, size_t size, RWMode mode, ULONG64 cr3 = 0)
	{
		return B_WriteMemory(addr, (void*)type, size, mode, cr3);
	}

	//分配内存
	ULONG64 B_AllocMemory(size_t size);

	//释放内存
	bool B_FreeMemory(ULONG64 addr);

	//保护内存
	bool B_ProtectMemory(ULONG64 addr, DWORD newProtect, DWORD64 size);

	//移动鼠标
	//有相对移动和绝对值移动
	void B_MouseMove(int x, int y, MoveType type);

	//鼠标控制
	//鼠标按键选项 按下/弹起
	void B_MouseCtl(MouseKey key, MouseStatus status);

	//VK虚拟键值
	void B_KeyCtl(int vKey, KeyStatus status);

	//例如
	//B_KeyCtl('a', KeyStatus::DOWN); //按下 a 键
	void B_KeyCtl(char key, KeyStatus status);

	//VK虚拟键值
	void B_KeyClick(int vKey);

	//
	void B_KeyClick(char key);


	/*
	内核级全局保护进程
	基本就Hook了OpenProcess
	可以防止CE、OD等调试工具打开句柄
	保护自身进程例子:
	ProtectProcess(true, GetCurrentProcessId());
	保护其他进程例子:
	ProtectProcess(true, 1234);

	注意：
	只保护最后一次传入的进程名
	例如连续调用：
	ProtectProcess(true, 123);
	ProtectProcess(true, 456);
	那么只会保护PID为456的进程

	该保护是全局保护 除非重启电脑 不然会一直保护着
	所以建议退出程序前取消保护 ProtectProcess(false, xxx);
	*/
	bool B_ProtectProcess(bool protect, int pid);

	/*
	断链保护+隐藏进程
	全局句柄表中抹除进程

	在被保护的进程推出前一定要恢复 不然会蓝屏

	支持多进程保护隐藏 至多支持32个进程
	*/
	bool B_ProtectProcessV2(bool protect, int pid);


	/*
	内核级全局隐藏进程
	任务管理器无法看到你
	隐藏自身进程例子:
	B_HideProcess(true, GetCurrentProcessId());
	保护其他进程例子:
	B_HideProcess(true, 1234);

	注意：
	只隐藏最后一次传入的进程名
	例如连续调用：
	B_HideProcess(true, 123);
	B_HideProcess(true, 456);
	那么只会保护PID为456的进程

	该隐藏是全局保护 除非重启电脑 不然会一直隐藏着
	所以建议退出程序前取消隐藏 HideProcess(false, xxx);
	*/
	bool B_HideProcess(bool hide, int pid);

	//内核反截图
	bool B_HideWindow(ULONG64 hWnd, HideWindowType type);

	//驱动映射注入 路径
	//可以注入主流dx11引擎的游戏
	bool B_MapDll(const char* dll_path);

	//驱动映射注入 内存
	//可以注入主流dx11引擎的游戏
	bool B_MapDll(unsigned char* dll_data, size_t dll_size);

	//通用驱动映射注入 支持x64/x32 内存
	bool B_MapDLLV2(unsigned char* dll_data, size_t dll_size);

	//通用驱动映射注入 支持x64/x32 路径
	bool B_MapDLLV2(const char* dll_path);

	//内核APC注入+隐藏内存 通杀各种游戏
	//hide_mem 部分游戏如果遇到玩个几十分钟崩溃的情况 hide_mem 填 false
	bool B_MapDLLV3(unsigned char* dll_data, size_t dll_size, bool hide_mem = true);

	//内核APC注入+隐藏内存 通杀各种游戏
	//hide_mem 部分游戏如果遇到玩个几十分钟崩溃的情况 hide_mem 填 false
	bool B_MapDLLV3(const char* dll_path, bool hide_mem = true);

	//内核RIP劫持注入 + 隐藏内存 内存
	//该注入Dll有要求，属性->代码生成-> 运行库 -> MT/MD，不然会蓝屏
	//如果返回false 可以把该系统的ntoskrnl.exe发给作者进行修复
	//clear_shellcode如果为true 该函数会延时5秒 用于清理shellcode痕迹
	bool B_MapDLLV4(unsigned char* dll_data, size_t dll_size, bool hide_mem = true, bool clear_shellcode = true);

	//内核RIP劫持注入 + 隐藏内存 路径
	//该注入Dll有要求，属性->代码生成-> 运行库 -> MT/MD，不然会蓝屏
	//如果返回false 可以把该系统的ntoskrnl.exe发给作者进行修复
	//hide_mem 部分游戏如果遇到玩个几十分钟崩溃的情况 hide_mem 填 false
	//clear_shellcode如果为true 该函数会延时5秒 用于清理shellcode痕迹
	bool B_MapDLLV4(const char* dll_path, bool hide_mem = true, bool clear_shellcode = true);

	//内核RIP劫持V2注入 + 隐藏内存 内存
	//如果返回false 可以把该系统的ntoskrnl.exe发给作者进行修复
	//hide_mem 部分游戏如果遇到玩个几十分钟崩溃的情况 hide_mem 填 false
	//clear_shellcode如果为true 该函数会延时5秒 用于清理shellcode痕迹
	bool B_MapDLLV5(unsigned char* dll_data, size_t dll_size, bool hide_mem = true, bool clear_shellcode = true);

	//内核RIP劫持V2注入 + 隐藏内存 路径
	//如果返回false 可以把该系统的ntoskrnl.exe发给作者进行修复
	//hide_mem 部分游戏如果遇到玩个几十分钟崩溃的情况 hide_mem 填 false
	//clear_shellcode如果为true 该函数会延时5秒 用于清理shellcode痕迹
	bool B_MapDLLV5(const char* dll_path, bool hide_mem = true, bool clear_shellcode = true);
	
	//调用之前先附加要读写的进程
	//获取真实进程CR3 主要是为了给B_PhyReadMemoryWithCr3()调用
	//注意 获取一次就行，你要是每次执行B_PhyReadMemoryWithCr3()都获取一次CR3那你就是傻逼 这个很吃效率
	ULONG64 B_GetProcessRealCr3();

	//依旧是读取一次即可
	//有时候当你第多次进入一个游戏时 B_GetProcessRealCr3()会读取老的CR3值 这会导致B_PhyReadMemoryWithCr3()失效
	//这时候你就可以使用B_GetProcessRealCr3Attach() 它能始终获取正确的CR3值 尽量少用
	ULONG64 B_GetProcessRealCr3Attach();

	//物理读写进阶版 无视EAC的CR3加密且不限速 最后一个参数需要调用B_GetProcessRealCr3()获取
	bool B_PhyReadMemoryWithCr3(ULONG64 addr, void* buffer, size_t size, ULONG64 cr3);

	bool B_PhyWriteMemoryWithCr3(ULONG64 addr, void* buffer, size_t size, ULONG64 cr3);

	//物理读写进阶版 无视EAC的CR3加密且不限速 最后一个参数需要调用B_GetProcessRealCr3()获取
	//多线程读写时记得加锁
	template <typename type>
	type B_PhyReadMemWithCr3(ULONG64 addr, ULONG64 cr3)
	{
		type buff{};

		B_PhyReadMemoryWithCr3(addr, &buff, sizeof(type), cr3);
		return buff;
	}

	//物理读写进阶版 无视EAC的CR3加密且不限速 最后一个参数需要调用B_GetProcessRealCr3()获取
	//多线程读写时记得加锁
	template <typename type>
	type B_PhyReadMemWithCr3(ULONG64 addr, size_t size, ULONG64 cr3)
	{
		type buff{};

		B_PhyReadMemoryWithCr3(addr, &buff, size, cr3);
		return buff;
	}

	/*
	内核保护窗口 内核hook多个重要函数 全面保护窗口
	protectWindow 保护开启/关闭
	protectWindowPid 保护的窗口的进程PID  注意是进程PID
	protectGlobal 全局保护窗口 任何进程都无法获取窗口句柄 包括你自己
	该选项为全局设置 建议退出程序时恢复 设置false

	如果有时候发现这个函数没有效果 就说明你的protectWindowPid传错了！
	*/
	void B_ProtectWindow(bool protectWindow, ULONG protectWindowPid, bool protectGlobal = false);

	//B_ForceDeleteFile("D:\\test.dll");
	//如果要输入中文 必须要把字符编码转为utf-8
	bool B_ForceDeleteFile(const char* filePath);

	/*特征码定位 掩码版本
	* 效率低 只能返回第一个匹配的地址 但不会漏搜索
	* addr: 搜索起始位置
	* size: 搜索范围大小
	* pattern: 特征码
	* mask: 掩码
	* mode: 读内存模式
	* 举例:
	* auto result = B_FindPatternV1(addr, size, "\xE8\x70\x02\x00\x00\x48\x8B\x4C\x24\x70", "xxx??xxxxx", RWMode::Phy);
	*/
	ULONG64 B_FindPatternV1(ULONG64 addr, ULONG64 size, const char* pattern, const char* mask, RWMode mode = RWMode::Phy);

	/*特征码定位
	* 效率低 只能返回第一个匹配的地址 但不会漏搜索
	* addr: 搜索起始位置
	* size: 搜索范围大小
	* pattern: 特征码 注意和V1的格式不一样
	* mode: 读内存模式
	* 举例:
	* auto result = B_FindPatternV1(addr, size, "E8 70 02 ? ? 48 8B 4C 24 70", RWMode::Phy);
	*/
	ULONG64 B_FindPatternV2(ULONG64 addr, ULONG64 size, const char* pattern, RWMode mode = RWMode::Phy);

	/*特征码定位
	* CE源码里CV过来的
	* 啥也别说了 用就完了
	* 举例:
	* auto result = B_AOBScanV1("\xE8\x70\x02\x00\x00\x48\x8B\x4C\x24\x70", "xxx??xxxxx", moduleBase, size, RWMode::Phy);
	*/
	std::vector<ULONG64> B_AOBScanV1(const char* pattern, const char* mask, ULONG64 addr, ULONG64 size, RWMode mode = RWMode::Phy);

	/*特征码定位
	* CE源码里CV过来的
	* 啥也别说了 用就完了
	* 举例:
	* auto result = B_AOBScanV2("E8 70 02 ? ? 48 8B 4C 24 70", moduleBase, size);
	*/
	std::vector<ULONG64> B_AOBScanV2(const char* pattern, ULONG64 addr, ULONG64 size, RWMode mode = RWMode::Phy);

	//查询内存
	//当作VirtualQueryEx来用
	bool B_QueryMemory(ULONG64 addr, MEMORY_BASIC_INFORMATION* mbi);

	//dump模块到文件
	//参数1、2 填模块基址和模块大小
	//filePath 填要保存的文件
	bool B_DumpToFile(ULONG64 moduleBase, ULONG64 moduleSize, const char* filePath, RWMode mode = RWMode::MmCopy);


	//读写内核空间 谨慎使用
	// type: 0 read / type: 1 write
	bool B_RWKernelMemory(ULONG64 addr, void* buffer, ULONG size, int type);


	//关闭NMI回调检测
	//如果返回值是false说明当前系统没有适配，可以把ntoskrnl.exe发给我进行适配
	bool B_DisableCallback_NMI();
};

