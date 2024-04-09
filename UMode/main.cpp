#include "loadDrv.h"

typedef struct _NULL_MEMORY {
	//r/w variables
	void* buffer;
	UINT_PTR addr;
	HANDLE pid;

	//features boolean variables
	BOOLEAN getpid;
	BOOLEAN write;
	BOOLEAN read;
	BOOLEAN reg_base;
	BOOLEAN changeProtect;

	BOOLEAN draw_box;
	BOOLEAN draw_line;
	BOOLEAN fill_rect;
	BOOLEAN drawborder_box;
	BOOLEAN draw_text;

	//Draw Function
	int r, g, b, x, y, w, h, t;
	int     dx, dy;
	int     x_step, y_step;
	UINT string_length;
	LPCSTR lpString;

	//process char variables
	const char* moduleName;
	const char* processName;

	//DllStruct variables
	ULONG64 base_addr;
	ULONGLONG sizeDll;
	ULONG size;
	DWORD newProtect;
	DWORD oldProtect;
}NULL_MEMORY;

struct Dll {
	ULONG64 baseAddr;
	ULONGLONG sizeDll;
};

typedef struct Dll StructDll;

HANDLE pid = NULL;
static std::once_flag flag;
wchar_t currentDir[MAX_PATH];
StructDll crossfire;
StructDll CShell_x64;
StructDll ClientFx_x64;
uintptr_t CLT_SHELL;
StructDll libcef;
HWND hWnd = NULL;
DWORD CheckPoint = NULL;
DWORD ReturnCheckPoint = NULL;
DWORD SecondPerson = NULL;
DWORD CopyRoomID = NULL;
DWORD PasteRoomID = NULL;
DWORD DrawCrosshair = NULL;
DWORD ThirdPerson = NULL;
DWORD SkillE = NULL;
DWORD NoReload_NoChange = NULL;
DWORD NoRecoil = NULL;

template<typename ... A>
uint64_t call_hook(const A ... arguments)
{
	std::call_once(flag, [] { LoadLibraryA(RGS("user32.dll")); });
	void* control_function = GetProcAddress(LoadLibraryA(RGS("win32u.dll")), RGS("NtDxgkGetTrackedWorkloadStatistics"));
	const auto control = static_cast<uint64_t(__stdcall*)(A...)>(control_function);
	return control(arguments ...);
}


static StructDll get_module_base_addr(const char* moduleName) {
	StructDll _return;
	NULL_MEMORY instr = { 0 };
	instr.pid = pid;
	instr.reg_base = true;
	instr.moduleName = moduleName;
	call_hook(&instr);
	_return.baseAddr = instr.base_addr;
	_return.sizeDll = instr.sizeDll;
	return _return;
}

static HANDLE get_process_id(const char* process_name) {
	NULL_MEMORY instr = { 0 };
	instr.getpid = true;
	instr.processName = process_name;
	call_hook(&instr);

	return instr.pid;
}

template<class T>
T read(UINT_PTR readAddr) {
	T buffer{};
	NULL_MEMORY instr = { 0 };
	instr.pid = pid;
	instr.addr = readAddr;
	instr.read = true;
	instr.buffer = &buffer;
	instr.size = sizeof(T);
	call_hook(instr);
	return buffer;
}
template <class T>
bool read_raw(UINT_PTR addr, T* buffer, size_t size) {
	NULL_MEMORY instr = { 0 };
	instr.pid = pid;
	instr.addr = addr;
	instr.read = true;
	instr.buffer = buffer;
	instr.size = sizeof(T) * size;
	call_hook(instr);
	return true;
}

template <typename W>
bool write(UINT_PTR waddr, W value) {
	NULL_MEMORY instr = { 0 };
	instr.pid = pid;
	instr.addr = waddr;
	instr.write = true;
	instr.buffer = &value;
	instr.size = sizeof(W);
	call_hook(instr);
	return true;
}

bool writeBytes(UINT_PTR addr, PVOID buffer, SIZE_T bufferSize) {
	NULL_MEMORY instr = { 0 };
	instr.pid = pid;
	instr.addr = addr;
	instr.write = true;
	instr.buffer = buffer;
	instr.size = bufferSize;
	call_hook(instr);
	return true;
}

DWORD changeProtection(HANDLE pid, UINT_PTR address, ULONG size, DWORD newProtect) {
	NULL_MEMORY instr = { 0 };
	instr.changeProtect = true;
	instr.pid = pid;
	instr.addr = address;
	instr.size = size;
	instr.newProtect = newProtect;
	call_hook(&instr);
	return instr.oldProtect;
}	

bool writeByteProtect(HANDLE pid, UINT_PTR address, PVOID buffer, SIZE_T bufferSize, DWORD size, DWORD newProtect) {
	DWORD oldProt = changeProtection(pid, address, size, newProtect);
	writeBytes(address, buffer, bufferSize);
	DWORD newProt = changeProtection(pid, address, size, oldProt);
	return (newProt == newProtect);
}

bool draw_box(int x, int y, int w, int h, int t, int r, int g, int b) {
	NULL_MEMORY instr = { 0 };
	instr.draw_box = true;
	//Size
	instr.x = x;
	instr.y = y;
	instr.w = w;
	instr.h = h;
	instr.t = t;
	//Color
	instr.r = r;
	instr.g = g;
	instr.b = b;

	call_hook(&instr);
	return true;
}

bool draw_line(int x, int y, int dx, int dy, int t, int r, int g, int b)
{
	NULL_MEMORY instr = { 0 };
	instr.draw_line = true;

	instr.x = x;
	instr.y = y;
	instr.w = -1;
	instr.h = -1;
	instr.t = t;

	instr.r = r;
	instr.g = g;
	instr.b = b;

	instr.dx = dx;
	instr.dy = dy;

	instr.x_step = int(sqrt((float)((dx - x) * (dx - x) + (dy - y) * (dy - y))) / (dx - x));
	instr.y_step = int(sqrt((float)((dx - x) * (dx - x) + (dy - y) * (dy - y))) / (dy - y));

	call_hook(&instr);

	return true;
}

bool fill_rect(int x, int y, int w, int h, int r, int g, int b) {
	NULL_MEMORY instr = { 0 };
	instr.fill_rect = true;
	instr.x = x;
	instr.y = y;
	instr.w = w;
	instr.h = h;
	instr.r = r;
	instr.g = g;
	instr.b = b;
	call_hook(&instr);
	return true;
}

bool drawborder_box(int x, int y, int w, int h, int t, int r, int g, int b) {
	NULL_MEMORY instr = { 0 };
	instr.drawborder_box = true;
	instr.x = x;
	instr.y = y;
	instr.w = w;
	instr.h = h;
	instr.t = t;
	instr.r = r;
	instr.g = g;
	instr.b = b;
	call_hook(&instr);
	return true;
}

bool draw_text(int x, int y, LPCSTR string, UINT len) {
	NULL_MEMORY instr = { 0 };
	instr.draw_text = true;
	instr.x = x;
	instr.y = y;
	instr.lpString = string;
	instr.string_length = len;
	call_hook(&instr);
	return true;
}

//Game Function
void NoBugDamage() {
	changeProtection(pid, CShell_x64.baseAddr + nv.NoBugDamage, 19, PAGE_EXECUTE_READWRITE);
	BYTE writeBuffer[] = { 0x72,0x65,0x7A,0x5C,0x42,0x75,0x74,0x65,0x73,0x5C,0x42,0x46,0x30,0x33,0x35 };
	writeBytes(CShell_x64.baseAddr + nv.NoBugDamage, &writeBuffer, sizeof(writeBuffer));
	changeProtection(pid, CShell_x64.baseAddr + nv.NoBugDamage, 19, 0);
}

void SupportFunction() {
	float oldx = 0, oldy = 0, oldz = 0;
	bool isSecondPerson = false;
	int roomid = 0;

	uintptr_t thirdPerson_Base = read<uintptr_t>(CShell_x64.baseAddr + nv.ThirdPerson_Base);
	uintptr_t copyRoom_base = read<uintptr_t>(CShell_x64.baseAddr + nv.CopyRoomBase);
	uintptr_t copyRoom_base1 = read<uintptr_t>(copyRoom_base + nv.CopyRoomOffset1);
	while (true) {
		if (FindWindowA(0, RGS("CROSSFIRE"))) {
			int inGameStatus = read<int>(CShell_x64.baseAddr + nv.InGameStatus);
			if (SkillE) {
				//Skill E
				if (GetAsyncKeyState(SkillE) & 1) {
					if (inGameStatus == 0xB) {
						uintptr_t skillE_Base = read<uintptr_t>(CShell_x64.baseAddr + nv.SkillE_Base);
						int skillE_read = read<int>(skillE_Base + nv.SkillE_Offset);
						if (skillE_read == 2) {
							write<int>(skillE_Base + nv.SkillE_Offset, 0);
						}
					}
				}
			}


			if (CheckPoint) {
				//Checkpoint
				if (GetAsyncKeyState(CheckPoint) & 1) {
					if (inGameStatus == 11) {
						Beep(500, 300);
						uintptr_t coordinate_base = read<uintptr_t>(CShell_x64.baseAddr + nv.Coordinate_Base);
						uintptr_t base = read<uintptr_t>(coordinate_base + nv.Coordinate_Offset1);
						base = read<uintptr_t>(base + nv.Coordinate_Offset2);
						oldy = read<float>(base + nv.Coordinate_Offset3 - 0x4);
						oldz = read<float>(base + nv.Coordinate_Offset3);
						oldx = read<float>(base + nv.Coordinate_Offset3 + 0x4);
					}
				}
			}

			if (ReturnCheckPoint) {
				//Go to the last checkpoint
				if (GetAsyncKeyState(ReturnCheckPoint) & 1) {
					if (inGameStatus == 11) {
						Beep(300, 300);
						uintptr_t coordinate_base = read<uintptr_t>(CShell_x64.baseAddr + nv.Coordinate_Base);
						uintptr_t base = read<uintptr_t>(coordinate_base + nv.Coordinate_Offset1);
						base = read<uintptr_t>(base + nv.Coordinate_Offset2);
						write<float>(base + nv.Coordinate_Offset3 - 0x4, oldy);
						write<float>(base + nv.Coordinate_Offset3, oldz);
						write<float>(base + nv.Coordinate_Offset3 + 0x4, oldx);
					}
				}
			}

			if (ThirdPerson) {
				//Third person
				if (GetAsyncKeyState(ThirdPerson) & 1) {
					isSecondPerson = !isSecondPerson;
					if (isSecondPerson) {
						write<int>(thirdPerson_Base + nv.ThirdPerson_Offset, 3);
					}
					else {
						write<int>(thirdPerson_Base + nv.ThirdPerson_Offset, 1);
					}
				}
			}

			if (SecondPerson) {
				//Second person
				if (GetAsyncKeyState(SecondPerson) & 1) {
					isSecondPerson = !isSecondPerson;
					if (isSecondPerson) {
						write<int>(thirdPerson_Base + nv.ThirdPerson_Offset, 2);
					}
					else {
						write<int>(thirdPerson_Base + nv.ThirdPerson_Offset, 1);
					}
				}
			}

			if (CopyRoomID) {
				//Copy room ID
				if (GetAsyncKeyState(CopyRoomID) & 1) {
					Beep(500, 300);
					roomid = read<int>(copyRoom_base1 + nv.CopyRoomOffset2);
				}
			}

			if (PasteRoomID) {
				//Paste room ID
				if (GetAsyncKeyState(PasteRoomID) & 1) {
					Beep(300, 300);
					if (roomid != 0) {
						write<int>(copyRoom_base1 + nv.CopyRoomOffset2, roomid);
					}
				}
			}
			NoBugDamage();
		} else {
			Beep(300, 500);
			exit(0);
		}
		
	}
}

void pressK() {
	keybd_event(0x4B, 0x25, 0, 0);
	keybd_event(0x4B, 0x25, KEYEVENTF_KEYUP, 0);
}

#define msg(str) MessageBoxA(0, str, "Info", MB_OK);
#define __CODERDUC__ int main(void)
#define LOGGER //Uncomment this to use address logger

#ifdef LOGGER
	#define log(name, addr) cout << left << setw(40) << name << "0x" << setw(10) << hex << uppercase << addr << endl;
#else
	#define log(name, addr)
#endif // DEBUG

class FindPattern
{
public:
	FindPattern(DWORD64 start, DWORD64 len, BYTE* pMask, char* szMask) : Base(0), Offset(0)
	{
		BYTE* data = new BYTE[len];
		if (read_raw<BYTE>(start, data, len))
		{
			for (DWORD i = 0; i < len; i++)
			{
				if (DataCompare((const BYTE*)(data + i), (const BYTE*)pMask, szMask))
				{
					Base = (DWORD64)(start + i);
					Offset = i; 
					break;
				}
			}
		}
		delete[] data;
	}
	static HANDLE pHandle;
	DWORD64 Base;
	DWORD Offset;
private:
	bool DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask)
	{
		for (; *pszMask; ++pData, ++pMask, ++pszMask)
		{
			if (*pszMask == '0' && *pData != *pMask)
				return false;
		}
		return (*pszMask == NULL);
	}
};

bool isgetaddress = false;

bool Prepare() {
	if (!isgetaddress) {
		nv.InGameStatus = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x8b\x15\x00\x00\x00\x00\xff\x90\x00\x00\x00\x00\xf6\x05", "00????00????00").Base;
		if (nv.InGameStatus) {
			DWORD offset = read<DWORD>(nv.InGameStatus + 0x2);
			nv.InGameStatus = nv.InGameStatus + offset + 0x6;
			nv.InGameStatus -= CShell_x64.baseAddr;
			log(RGS("dwInGameStatus"), nv.InGameStatus);
		}

		nv.NoBugDamage = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x48\x8d\x15\x00\x00\x00\x00\x48\x8d\x4c\x24\x00\xe8\x00\x00\x00\x00\x4c\x8d\x44\x24\x00\xba\x00\x00\x00\x00\x48\x8d\x8f", "000????0000?0????0000?0????000").Base;
		if (nv.NoBugDamage) {
			DWORD offset = read<DWORD>(nv.NoBugDamage + 0x3);
			nv.NoBugDamage = nv.NoBugDamage + offset + 0x7;
			nv.NoBugDamage -= CShell_x64.baseAddr;
			log(RGS("dwNoBugDamage"), nv.NoBugDamage);
		}

		nv.NoRecoil = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x00\x00\x80\xBF\x00\x00\x00\x00\x00\x99\x99\xBF\x00\x00\x00\x00\x80\x8A\x00\x00\x00\x00\x00\xBF\x00\x00\x00\x00", "0000?????000????00?????0????").Base;
		if (nv.NoRecoil) {
			nv.NoRecoil -= CShell_x64.baseAddr;
			log(RGS("dwNoRecoil"), nv.NoRecoil);
		}

		
		nv.ThirdPerson_Base = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x00\x00\x00\x00\x00\x00\x00\x00\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x20\x43\x00\x00", "????00000000?0000000000000").Base;
		if (nv.ThirdPerson_Base) {
			nv.ThirdPerson_Base -= CShell_x64.baseAddr;
			log(RGS("dwThirdPerson_Base"), nv.ThirdPerson_Base);
		}
		
		nv.ThirdPerson_Offset = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x8B\x83\x00\x00\x00\x00\x48\x83\xC4\x00\x5B\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x89\x5C\x24\x00", "00????000?0000000000000000?").Base;
		if (nv.ThirdPerson_Offset) {
			nv.ThirdPerson_Offset = read<DWORD>(nv.ThirdPerson_Offset + 0x2);
			log(RGS("dwThirdPerson_Offset"), nv.ThirdPerson_Offset);
		}

		nv.Coordinate_Base = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x48\x8b\x05\x00\x00\x00\x00\x44\x0f\x29\x5c\x24", "000????00000").Base;
		if (nv.Coordinate_Base) {
			DWORD offset = read<DWORD>(nv.Coordinate_Base + 0x3);
			nv.Coordinate_Base = nv.Coordinate_Base + offset + 0x7;
			nv.Coordinate_Base -= CShell_x64.baseAddr;
			log(RGS("dwCoordinate_Base"), nv.Coordinate_Base);
		}

		nv.Coordinate_Offset1 = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x48\x8b\xb0\x00\x00\x00\x00\xc6\x41", "000????00").Base;
		if (nv.Coordinate_Offset1) {
			nv.Coordinate_Offset1 = read<DWORD>(nv.Coordinate_Offset1 + 0x3);
			log(RGS("dwCoordinate_Offset1"), nv.Coordinate_Offset1);
		}

		nv.Coordinate_Offset2 = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x4c\x8b\xb6\x00\x00\x00\x00\x48\x8b\x18", "000????000").Base;
		if (nv.Coordinate_Offset2) {
			nv.Coordinate_Offset2 = read<DWORD>(nv.Coordinate_Offset2 + 0x3);
			log(RGS("dwCoordinate_Offset2"), nv.Coordinate_Offset2);
		}

		nv.Coordinate_Offset3 = FindPattern::FindPattern((DWORD64)crossfire.baseAddr, (DWORD64)crossfire.sizeDll, (PBYTE)"\xf3\x44\x0f\x10\x8f\x00\x00\x00\x00\x41\x0f\x28\xfa", "00000????0000").Base;
		if (nv.Coordinate_Offset3) {
			nv.Coordinate_Offset3 = read<DWORD>(nv.Coordinate_Offset3 + 0x5);
			log(RGS("dwCoordinate_Offset3"), nv.Coordinate_Offset3);
		}
		
		nv.CharacterFunc = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x48\x8b\x15\x00\x00\x00\x00\x48\x23\x0d", "000????000").Base;
		if (nv.CharacterFunc) {
			DWORD offset = read<DWORD>(nv.CharacterFunc + 0x3);
			nv.CharacterFunc = nv.CharacterFunc + offset + 0x7;
			nv.CharacterFunc -= CShell_x64.baseAddr;
			log(RGS("dwCharacterFunc"), nv.CharacterFunc);
		}

		nv.CFTTable = FindPattern::FindPattern((DWORD64)crossfire.baseAddr, (DWORD64)crossfire.sizeDll, (PBYTE)"\x48\x8b\x05\x00\x00\x00\x00\x48\x85\xc0\x75\x00\xb9\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x45\x33\xc0", "000????0000?0????0????000").Base;
		if (nv.CFTTable) {
			DWORD offset = read<DWORD>(nv.CFTTable + 0x3);
			nv.CFTTable = nv.CFTTable + offset + 0x7;
			nv.CFTTable -= crossfire.baseAddr;
			log(RGS("dwCFTTable"), nv.CFTTable);
		}

		nv.dwLTShell = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x48\x8b\x05\x00\x00\x00\x00\xba\x00\x00\x00\x00\x48\x8b\x88\x00\x00\x00\x00\x48\x8b\x01\xff\x90\x00\x00\x00\x00\x40\x0f\xb6\xd7", "000????0????000????00000????0000").Base;
		if (nv.dwLTShell) {
			DWORD offset = read<DWORD>(nv.dwLTShell + 0x3);
			nv.dwLTShell = nv.dwLTShell + offset + 0x7;
			nv.dwLTShell -= CShell_x64.baseAddr;
			log(RGS("dwLTShell"), nv.dwLTShell);
		}

		nv.dwENT_BEGIN = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x4c\x8b\xbc\x39\x00\x00\x00\x00\x48\x85\xc0", "0000????000").Base;
		if (nv.dwENT_BEGIN) {
			nv.dwENT_BEGIN = read<DWORD>(nv.dwENT_BEGIN + 0x4);
			log(RGS("dwENT_BEGIN"), nv.dwENT_BEGIN);
		}

		nv.dwENT_SIZE = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x48\x69\xc8\x00\x00\x00\x00\x0f\x29\xb4\x24\x00\x00\x00\x00\x48\x8b\x05", "000????0000????000").Base;
		if (nv.dwENT_SIZE) {
			nv.dwENT_SIZE = read<DWORD>(nv.dwENT_SIZE + 0x3);
			log(RGS("dwENT_SIZE"), nv.dwENT_SIZE);
		}

		nv.dwLOCAL_ENT_INDEX = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x41\x0f\xb6\x86\x00\x00\x00\x00\x4c\x69\xf8", "0000????000").Base;
		if (nv.dwLOCAL_ENT_INDEX) {
			nv.dwLOCAL_ENT_INDEX = read<DWORD>(nv.dwLOCAL_ENT_INDEX + 0x4);
			log(RGS("dwLOCAL_ENT_INDEX"), nv.dwLOCAL_ENT_INDEX);
		}
		
		nv.FastGun = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x00\x00\x7a\x00\x00\x00\x7f", "000?000").Base;
		if (nv.FastGun) {
			nv.FastGun -= CShell_x64.baseAddr;
			log(RGS("dwFastGun"), nv.FastGun);
		}

		nv.CopyRoomBase = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x48\x8B\x00\x00\x00\x00\x00\x48\x00\x00\x75\x1F\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x00\x48\x8B\xC8\xE8\x00\x00\x00\x00\x90\x48\x89\x00\x00\x00\x00\x00\x48\x83\xC4\x00\xC3\xCC\xCC\xCC\x41\x56", "00?????0??000????0????0000?0000????000?????000?000000").Base;
		if (nv.CopyRoomBase) {
			DWORD offset = read<DWORD>(nv.CopyRoomBase + 0x3);
			nv.CopyRoomBase = nv.CopyRoomBase + offset + 0x7;
			nv.CopyRoomBase -= CShell_x64.baseAddr;
			log(RGS("dwCopyRoomBase"), nv.CopyRoomBase);
		}

		nv.CopyRoomOffset1 = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x48\x8b\x40\x00\x8b\x88\x00\x00\x00\x00\x83\xf9\x00\x75", "000?00????00?0").Base;
		if (nv.CopyRoomOffset1) {
			nv.CopyRoomOffset1 = read<BYTE>(nv.CopyRoomOffset1 + 0x3);
			log(RGS("dwCopyRoomOffset1"), nv.CopyRoomOffset1);
		}

		nv.CopyRoomOffset2 = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x48\x89\x8B\x00\x00\x00\x00\x4C\x8D\xBB\x00\x00\x00\x00\x41\x8B\x3F", "000????000????000").Base;
		if (nv.CopyRoomOffset2) {
			nv.CopyRoomOffset2 = read<DWORD>(nv.CopyRoomOffset2 + 0x3);
			log(RGS("dwCopyRoomOffset2"), nv.CopyRoomOffset2);
		}

		nv.SkillE_Base = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\x00\x00\x00\x80\xBB\x00\x00\x00\x00\x00", "0??????0?00000000000000?????").Base;
		if (nv.SkillE_Base) {
			nv.SkillE_Base -= CShell_x64.baseAddr;
			log(RGS("dwSkillE_Base"), nv.SkillE_Base);
		}

		nv.SkillE_Offset = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x8B\x81\x00\x00\x00\x00\x83\xF8\x00\x75\x03\xB0\x00\xC3", "00????00?000?0").Base;
		if (nv.SkillE_Offset) {
			nv.SkillE_Offset = read<DWORD>(nv.SkillE_Offset + 0x2);
			log(RGS("dwSkillE_Offset"), nv.SkillE_Offset);
		}

		nv.ModelBute = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x48\x8B\x3D\x00\x00\x00\x00\xF3\x0F\x11\x4C\x3E\x38\xF3\x0F\x11\x4C\x3E\x3C\xF3\x0F\x11\x4C\x3E\x40", "000????000000000000000000").Base;
		if (nv.ModelBute) {
			DWORD offset = read<DWORD>(nv.ModelBute + 0x3);
			nv.ModelBute = nv.ModelBute + offset + 0x7;
			nv.ModelBute -= CShell_x64.baseAddr;
			log(RGS("dwModelBute"), nv.ModelBute);
		}

		nv.NR_NCBase = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x48\x8b\x05\x00\x00\x00\x00\xba\x00\x00\x00\x00\x48\x8b\x88\x00\x00\x00\x00\x48\x8b\x01\xff\x90\x00\x00\x00\x00\x40\x0f\xb6\xd7", "000????0????000????00000????0000").Base;
		if (nv.NR_NCBase) {
			DWORD offset = read<DWORD>(nv.NR_NCBase + 0x3);
			nv.NR_NCBase = nv.NR_NCBase + offset + 0x7;
			nv.NR_NCBase -= CShell_x64.baseAddr;
			log(RGS("dwNR_NCBase"), nv.NR_NCBase);
		}

		nv.NR_NCOffset1 = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x48\x8b\xb0\x00\x00\x00\x00\xc6\x41", "000????00").Base;
		if (nv.NR_NCOffset1) {
			nv.NR_NCOffset1 = read<DWORD>(nv.NR_NCOffset1 + 0x3);
			log(RGS("dwNR_NCOffset1"), nv.NR_NCOffset1);
		}

		nv.NR_NCOffset2 = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x41\x8b\x96\x00\x00\x00\x00\x49\x8b\xce\xff\x90\x00\x00\x00\x00\x84\xc0\x0f\x84\x00\x00\x00\x00\x49\x8b\x06", "000????00000????0000????000").Base;
		if (nv.NR_NCOffset2) {
			nv.NR_NCOffset2 = read<DWORD>(nv.NR_NCOffset2 + 0x3);
			log(RGS("dwNR_NCOffset2"), nv.NR_NCOffset2);
		}

		nv.dwSpamRadio = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\x0f\xb6\x0d\x00\x00\x00\x00\x3d", "000????0").Base;
		if (nv.dwSpamRadio) {
			DWORD offset = read<DWORD>(nv.dwSpamRadio + 0x3);
			nv.dwSpamRadio = nv.dwSpamRadio + offset + 0x7;
			nv.dwSpamRadio -= CShell_x64.baseAddr;
			log(RGS("dwSpamRadio"), nv.dwSpamRadio);
		}

		nv.ShiftSpeedOffset = FindPattern::FindPattern((DWORD64)CShell_x64.baseAddr, (DWORD64)CShell_x64.sizeDll, (PBYTE)"\xf3\x0f\x10\x83\x00\x00\x00\x00\xf3\x0f\x59\x41\x00\x48\x8b\xcb\x48\x8b\x03\xf3\x0f\x11\x45\x00\xff\x90\x00\x00\x00\x00\x8b\xc8\xe8\x00\x00\x00\x00\x48\x85\xc0\x0f\x84\x00\x00\x00\x00\x83\xb8\x00\x00\x00\x00\x00\x0f\x85\x00\x00\x00\x00\xe8\x00\x00\x00\x00\xf3\x0f\x10\x4d\x00\xf3\x0f\x59\xc8\xf3\x0f\x11\x4d\x00\xe9\x00\x00\x00\x00\x48\x8b\x03\x48\x8b\xcb\xff\x90\x00\x00\x00\x00\x83\xf8\x00\x75\x00\x39\xbb\x00\x00\x00\x00\x75\x00\x40\x38\xbb\x00\x00\x00\x00\x74\x00\x40\x38\xbb\x00\x00\x00\x00\x74\x00\x8b\x8b\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x48\x8b\xcf\x48\x63\x90\x00\x00\x00\x00\x85\xd2\x78\x00\x8b\x05\x00\x00\x00\x00\xff\xc8\x3b\xd0\x7f\x00\x48\x69\xca\x00\x00\x00\x00\x48\x03\x0d\x00\x00\x00\x00\xf3\x0f\x10\x83\x00\x00\x00\x00\xf3\x0f\x59\x41\x00\xf3\x0f\x11\x45\x00\xeb\x00\x0f\x28\xc6", "0000????0000?0000000000?00????000????00000????00?????00????0????0000?00000000?0????00000000????00?0?00????0?000????0?000????0?00????0????000000????000?00????00000?000????000????0000????0000?0000?0?000").Base;
		if (nv.ShiftSpeedOffset) {
			nv.ShiftSpeedOffset = read<DWORD>(nv.ShiftSpeedOffset + 0x4);
			log(RGS("dwShiftSpeedOffset"), nv.ShiftSpeedOffset);
		}
		Beep(300, 500);
		isgetaddress = true;
	}
	return 1;
}


__CODERDUC__{
	setup();
	Sleep(1000);
	
	if (!pid && !crossfire.baseAddr && !CShell_x64.baseAddr && !ClientFx_x64.baseAddr) {
		pid = get_process_id(RGS("crossfire.exe"));
		crossfire = get_module_base_addr(RGS("crossfire.exe"));
		CShell_x64 = get_module_base_addr(RGS("cshell_x64.dll"));
		ClientFx_x64 = get_module_base_addr(RGS("ClientFx_x64.fxd"));
	}
	Prepare();
	////FreeConsole();

	//SkillE = VK_SHIFT;
	//CheckPoint = VK_NUMPAD1;
	//ReturnCheckPoint = VK_NUMPAD2;
	//CopyRoomID = VK_F3;
	//PasteRoomID = VK_F4;
	//SecondPerson = 'V';
	//SupportFunction();

	system(RGS("pause"));
	return 0;
}
