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
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <vector>

enum B_STATUS : ULONG
{
	BF_OK = 0,				//成功
	BF_ERR_AUTH_INIT,		//连接服务器失败，国外无法连接服务器可以挂代理
	BF_ERR_LOGIN,			//登录失败，具体原因查看调用B_GetInitResult()
	BF_ERR_COMM,			//通信初始化失败
	BF_ERR_DETECT_DRIVER,	//检测到监管驱动在运行
	BF_ERR_INIT_DRIVER,		//初始化驱动失败，具体原因查看调用B_GetInitResult()
};

enum B_InstallMode
{
	NtLoadDriver,
	Normal
};

enum class RWMode
{
	MmCopy,	//无附加 调用内核Api
	Mdl,	//附加 MDL
	Phy,	//无附加 物理地址读写
	Ke,		//附加
	KeSafe,	//附加 相比Ke更安全 读取不安全地址不会蓝屏
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


enum HideMem : ULONG
{
	HM_READONLY = 1,
	HM_EXECUTE = 2,
	HM_EXECUTE_READ = 3,
	HM_READWRITE = 4,
	HM_WRITECOPY = 5,
	HM_EXECUTE_READWRITE = 6,
	HM_EXECUTE_WRITECOPY = 7,
	HM_NOCACHE = 0x8,
	HM_DECOMMIT = 0x10,
	HM_NOACCESS = 0x18,
};

enum InjectType : ULONG
{
	IT_NORMAL = 0,//支持x64/x32 通用驱动映射注入
	IT_APC,//支持x64 内核APC注入+隐藏内存 通杀各种游戏
	IT_RIP,//支持x64 内核RIP劫持注入 注入Dll有要求，属性->代码生成-> 运行库 -> MT/MD
	IT_RIPV2,//支持x64 内核RIP劫持注入
	IT_DX11,//支持x64 劫持dx11注入 可以注入主流dx11引擎的游戏
	IT_DX12//支持x64 劫持dx12注入 可以注入主流dx12引擎的游戏
};

class BFDrv
{
public:
	BFDrv();

	bool B_AdjustPrivilege();

	///初始化驱动
	///@param key 填写卡密
	///@param mode 加载方式 NtLoadDriver更安全 Normal兼容性更好
	///@param ignorePdb 忽略下载PDB 如果电脑无法下载PDB符号文件 仍然可以加载驱动 （已知无pdb情况下无法使用B_ProtectProcessV2）
	///@param delectList 加载时检测到关键驱动取消加载，不需要就不用填
	///@return 是否初始化成功
	B_STATUS B_InitDrv(const char* key, B_InstallMode mode = B_InstallMode::NtLoadDriver, bool ignorePdb = false, std::vector<const char*> delectList = {});

	/// <summary>
	/// 获取初始化结果
	/// </summary>
	/// <returns></returns>
	const char* B_GetInitResult();

	/// <summary>
	/// 选择要操作的进程
	/// </summary>
	/// <param name="pid">进程ProcessId</param>
	/// <returns></returns>
	bool B_AttachProcess(int pid);

	/// <summary>
	/// 卡密到期时间
	/// </summary>
	/// <returns>时间</returns>
	const char* B_GetExpiration();
	
	/// <summary>
	/// 获取系统版本
	/// </summary>
	/// <returns></returns>
	int B_GetWindowsBuildNumber();

	/// <summary>
	/// 驱动最后编译时间
	/// </summary>
	/// <returns>编译时间</returns>
	std::string B_GetDriverBuildTime();

	/// <summary>
	/// 获取主模块基址，何谓主模块？记事本里它是notepad.exe，CS2里它是cs2.exe，以此类推
	/// 无附加
	/// </summary>
	/// <returns></returns>
	ULONG64 B_GetMainModuleAddress();

	/// <summary>
	/// 获取模块基址、大小 注意不要在循环中疯狂调用 程序生命周期调用一次即可
	/// </summary>
	/// <param name="moduleName">模块名称</param>
	/// <param name="pSize">如果不需要获取模块大小 那么第二个参数填nullptr</param>
	/// <returns>模块基址</returns>
	ULONG64 B_GetModuleBaseAddress(const char* moduleName, ULONG* pSize = nullptr);

	/// <summary>
	/// 获取模块地址 无附加 有时失效？
	/// </summary>
	/// <param name="moduleName"></param>
	/// <param name="pSize"></param>
	/// <returns></returns>
	ULONG64 B_GetModuleBaseAddressNoAttach(const char* moduleName, ULONG* pSize = nullptr);

	/// <summary>
	/// 获取模块导出函数
	/// </summary>
	/// <param name="ModuleAddr">模块基址</param>
	/// <param name="funcName">导出函数名</param>
	/// <returns>函数地址</returns>
	ULONG64 B_GetModuleExportFuncAddress(ULONG64 ModuleAddr, const char* funcName);

	bool B_ReadMemory(ULONG64 addr, void* buffer, size_t size, RWMode mode, ULONG64 cr3 = 0);
	bool B_WriteMemory(ULONG64 addr, void* buffer, size_t size, RWMode mode, ULONG64 cr3 = 0);

	/*
	读写内存
	有5种读写模式 MDL MM KE PHY PHY (B_PhyReadMemoryWithCr3在下面)
	速度 KE > MDL > MM > PHY

	推荐使用PHY、MM

	*如果你不加载Pdb符号文件，那么KE和MDL在读取长度过大的数据（例如字符串）或者不安全的地址时可能会蓝屏*

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

	建议在被保护的进程推出前恢复
	（目前支持退出进程自动恢复，无需手动恢复，前提是退出进程底层函数需要调用NtTerminateProcess）

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

	/// <summary>
	/// 注入 传入路径
	/// </summary>
	/// <param name="dll_path">dll路径</param>
	/// <param name="type">注入类型</param>
	/// <param name="hide_mem">隐藏注入的dll内存，会影响一些游戏或者操作，需要动脑筋解决，例如：minhook无法hook（隐藏了内存它认为是无效地址）</param>
	/// <param name="clear_pe">清理PE痕迹</param>
	/// <param name="clear_shellcode">清理shellcode痕迹 针对rip注入</param>
	/// <returns>dll在目标进程中的基址、大小，失败则返回0，你可以手动调用B_HideMemory来改变注入dll的隐藏属性</returns>
	std::pair<ULONG64, ULONG64> B_InjectDll(const char* dll_path, InjectType type,
		bool hide_mem = false, bool clear_pe = false, bool clear_shellcode = false);
	
	/// <summary>
	/// 注入 传入内存
	/// </summary>
	/// <param name="dll_data">dll内存</param>
	/// <param name="dll_size">dll大小</param>
	/// <param name="type">注入类型</param>
	/// <param name="hide_mem">隐藏注入的dll内存，会影响一些游戏或者操作，需要动脑筋解决，例如：minhook无法hook（隐藏了内存它认为是无效地址）</param>
	/// <param name="clear_pe">清理PE痕迹</param>
	/// <param name="clear_shellcode">清理shellcode痕迹 针对rip注入</param>
	/// <returns>dll在目标进程中的基址、大小，失败则返回0，你可以手动调用B_HideMemory来改变注入dll的隐藏属性</returns>
	std::pair<ULONG64, ULONG64> B_InjectDll(unsigned char* dll_data, ULONG64 dll_size, InjectType type,
		bool hide_mem = false, bool clear_pe = false, bool clear_shellcode = false);

	//调用之前先附加要读写的进程
	//获取真实进程CR3 主要是为了给B_PhyReadMemoryWithCr3()调用
	//注意 获取一次就行，你要是每次执行B_PhyReadMemoryWithCr3()都获取一次CR3那你就是傻逼 这个很吃效率
	ULONG64 B_GetProcessRealCr3();

	//依旧是读取一次即可 但是会附加
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
	* CE源码里CV过来的 用就完了
	* 举例:
	* auto result = B_AOBScanV1("\xE8\x70\x02\x00\x00\x48\x8B\x4C\x24\x70", "xxx??xxxxx", moduleBase, size, RWMode::Phy);
	*/
	std::vector<ULONG64> B_AOBScanV1(const char* pattern, const char* mask, ULONG64 addr, ULONG64 size, RWMode mode = RWMode::Phy);

	/*特征码定位
	* CE源码里CV过来的 用就完了
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


	// 获取内核模块基址、大小
	ULONG64 B_GetKernelModule(const char* moduleName, ULONG* pSize = nullptr);

	//读写内核空间
	// type: 0 read / type: 1 write
	bool B_RWKernelMemory(ULONG64 addr, void* buffer, ULONG size, int type);

	//隐藏内存 注意隐藏的内存是以页面对齐(0x1000) 
	//比如改为只读：HM_READONLY 或者不可访问 HM_NOACCESS
	bool B_HideMemory(ULONG64 addr, ULONG64 size, HideMem attr);

	//关闭NMI回调检测
	bool B_DisableCallback_NMI();

	//关闭一些相关提示输出
	void B_DisablePrint(bool value);

	void B_TestMode(bool value);

	/// <summary>
	/// 关闭指定驱动的内核回调
	/// 目前支持关闭的回调有:
	/// LoadImageCallback,
	/// ProcessCreationCallback,
	/// ThreadCreationCallback,
	/// ProcessObjectCreationCallback,
	/// ThreadObjectCreationCallback,
	/// RegistryCallback,
	/// DriverVerificationCallback
	/// </summary>
	/// <param name="driver_name">指定要关闭回调的驱动</param>
	void B_DisableCallback(const char* driver_name);

	/// <summary>
	/// 恢复所有由B_DisableCallback关闭的内核回调
	/// </summary>
	void B_RestoreCallback();

	/// <summary>
	/// 在APC注入之前设置它
	/// 危险操作，如果你不知道你在做什么，不要调用它
	/// 当APC注入时，隐藏内存的同时将移除分配的内存VAD节点
	/// 已知被移除VAD节点的进程关闭时会蓝屏，不知道会不会触发PG
	/// </summary>
	void B_RemoveVAD(bool enable);
};

