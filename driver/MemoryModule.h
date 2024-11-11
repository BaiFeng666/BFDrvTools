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
#ifndef __MEMORY_MODULE_HEADER
#define __MEMORY_MODULE_HEADER

#include <windows.h>

typedef void* HMEMORYMODULE;

typedef void* HMEMORYRSRC;

typedef void* HCUSTOMMODULE;

#ifdef __cplusplus
extern "C" {
#endif

    typedef LPVOID(*CustomAllocFunc)(LPVOID, SIZE_T, DWORD, DWORD, void*);
    typedef BOOL(*CustomFreeFunc)(LPVOID, SIZE_T, DWORD, void*);
    typedef HCUSTOMMODULE(*CustomLoadLibraryFunc)(LPCSTR, void*);
    typedef FARPROC(*CustomGetProcAddressFunc)(HCUSTOMMODULE, LPCSTR, void*);
    typedef void (*CustomFreeLibraryFunc)(HCUSTOMMODULE, void*);

    /**
     * Load EXE/DLL from memory location with the given size.
     *
     * All dependencies are resolved using default LoadLibrary/GetProcAddress
     * calls through the Windows API.
     */
    HMEMORYMODULE MemoryLoadLibrary(const void*, size_t);

    /**
     * Load EXE/DLL from memory location with the given size using custom dependency
     * resolvers.
     *
     * Dependencies will be resolved using passed callback methods.
     */
    HMEMORYMODULE MemoryLoadLibraryEx(const void*, size_t,
        CustomAllocFunc,
        CustomFreeFunc,
        CustomLoadLibraryFunc,
        CustomGetProcAddressFunc,
        CustomFreeLibraryFunc,
        void*);

    /**
     * Get address of exported method. Supports loading both by name and by
     * ordinal value.
     */
    FARPROC MemoryGetProcAddress(HMEMORYMODULE, LPCSTR);

    /**
     * Free previously loaded EXE/DLL.
     */
    void MemoryFreeLibrary(HMEMORYMODULE);

    void ClearPEHeadersEx(unsigned char*);

    void UnlinkModule(unsigned char*);

    /**
     * Execute entry point (EXE only). The entry point can only be executed
     * if the EXE has been loaded to the correct base address or it could
     * be relocated (i.e. relocation information have not been stripped by
     * the linker).
     *
     * Important: calling this function will not return, i.e. once the loaded
     * EXE finished running, the process will terminate.
     *
     * Returns a negative value if the entry point could not be executed.
     */
    int MemoryCallEntryPoint(HMEMORYMODULE);

    /**
     * Find the location of a resource with the specified type and name.
     */
    HMEMORYRSRC MemoryFindResource(HMEMORYMODULE, LPCTSTR, LPCTSTR);

    /**
     * Find the location of a resource with the specified type, name and language.
     */
    HMEMORYRSRC MemoryFindResourceEx(HMEMORYMODULE, LPCTSTR, LPCTSTR, WORD);

    /**
     * Get the size of the resource in bytes.
     */
    DWORD MemorySizeofResource(HMEMORYMODULE, HMEMORYRSRC);

    /**
     * Get a pointer to the contents of the resource.
     */
    LPVOID MemoryLoadResource(HMEMORYMODULE, HMEMORYRSRC);

    /**
     * Load a string resource.
     */
    int MemoryLoadString(HMEMORYMODULE, UINT, LPTSTR, int);

    /**
     * Load a string resource with a given language.
     */
    int MemoryLoadStringEx(HMEMORYMODULE, UINT, LPTSTR, int, WORD);

    /**
    * Default implementation of CustomAllocFunc that calls VirtualAlloc
    * internally to allocate memory for a library
    *
    * This is the default as used by MemoryLoadLibrary.
    */
    LPVOID MemoryDefaultAlloc(LPVOID, SIZE_T, DWORD, DWORD, void*);

    /**
    * Default implementation of CustomFreeFunc that calls VirtualFree
    * internally to free the memory used by a library
    *
    * This is the default as used by MemoryLoadLibrary.
    */
    BOOL MemoryDefaultFree(LPVOID, SIZE_T, DWORD, void*);

    /**
     * Default implementation of CustomLoadLibraryFunc that calls LoadLibraryA
     * internally to load an additional libary.
     *
     * This is the default as used by MemoryLoadLibrary.
     */
    HCUSTOMMODULE MemoryDefaultLoadLibrary(LPCSTR, void*);

    /**
     * Default implementation of CustomGetProcAddressFunc that calls GetProcAddress
     * internally to get the address of an exported function.
     *
     * This is the default as used by MemoryLoadLibrary.
     */
    FARPROC MemoryDefaultGetProcAddress(HCUSTOMMODULE, LPCSTR, void*);

    /**
     * Default implementation of CustomFreeLibraryFunc that calls FreeLibrary
     * internally to release an additional libary.
     *
     * This is the default as used by MemoryLoadLibrary.
     */
    void MemoryDefaultFreeLibrary(HCUSTOMMODULE, void*);

#ifdef __cplusplus
}
#endif

#endif  // __MEMORY_MODULE_HEADER


struct ExportNameEntry {
    LPCSTR name;
    WORD idx;
};

typedef BOOL(WINAPI* DllEntryProc)(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);
typedef int (WINAPI* ExeEntryProc)(void);

#ifdef _WIN64
typedef struct POINTER_LIST {
    struct POINTER_LIST* next;
    void* address;
} POINTER_LIST;
#endif

typedef struct {
    PIMAGE_NT_HEADERS headers;
    unsigned char* codeBase;
    HCUSTOMMODULE* modules;
    int numModules;
    BOOL initialized;
    BOOL isDLL;
    BOOL isRelocated;
    CustomAllocFunc alloc;
    CustomFreeFunc free;
    CustomLoadLibraryFunc loadLibrary;
    CustomGetProcAddressFunc getProcAddress;
    CustomFreeLibraryFunc freeLibrary;
    struct ExportNameEntry* nameExportsTable;
    void* userdata;
    ExeEntryProc exeEntry;
    DWORD pageSize;
#ifdef _WIN64
    POINTER_LIST* blockedMemory;
#endif
} MEMORYMODULE, * PMEMORYMODULE;

typedef struct {
    LPVOID address;
    LPVOID alignedAddress;
    SIZE_T size;
    DWORD characteristics;
    BOOL last;
} SECTIONFINALIZEDATA, * PSECTIONFINALIZEDATA;