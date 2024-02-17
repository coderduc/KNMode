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
	void* control_function = GetProcAddress(LoadLibraryA(RGS("win32u.dll")), RGS("NtOpenCompositionSurfaceSectionInfo"));
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
	changeProtection(pid,CShell_x64.baseAddr + dwNoBugDamage, 19, PAGE_EXECUTE_READWRITE);
	BYTE writeBuffer[] = { 0x72,0x65,0x7A,0x5C,0x42,0x75,0x74,0x65,0x73,0x5C,0x42,0x46,0x30,0x33,0x35 };
	writeBytes(CShell_x64.baseAddr + dwNoBugDamage, &writeBuffer, sizeof(writeBuffer));
	changeProtection(pid, CShell_x64.baseAddr + dwNoBugDamage, 19, 0);
}

void NoFlash() {
	BYTE bytes[] = { 0x49,0x6d,0x70,0x6f,0x55,0x49,0x2f,0x4e,0x6f,0x69,0x73,0x65,0x53,0x63,0x72,0x65,0x65,0x6e,0x2f,0x4e,0x6f,0x69,0x73,0x65,0x46,0x69,0x6c,0x74,0x65,0x72,0x2e,0x64,0x74,0x78 };
	changeProtection(pid, CShell_x64.baseAddr + dwNoFlash1, 34, PAGE_EXECUTE_READWRITE);
	changeProtection(pid, CShell_x64.baseAddr + dwNoFlash2, 34, PAGE_EXECUTE_READWRITE);
	writeBytes(CShell_x64.baseAddr + dwNoFlash1, &bytes, sizeof(bytes));
	writeBytes(CShell_x64.baseAddr + dwNoFlash2, &bytes, sizeof(bytes));
	changeProtection(pid, CShell_x64.baseAddr + dwNoFlash1, 34, 0);
	changeProtection(pid, CShell_x64.baseAddr + dwNoFlash2, 34, 0);
}

void NoFallDamage() {
	uintptr_t base = read<uintptr_t>(CShell_x64.baseAddr + dwNoFallDamage);
	write<float>(base + dwNoFallDamageOffset, 0.0f);
}

class Player {
public:
	DWORD64 baseEntity;

	Player(DWORD64 baseEnt) {
		baseEntity = baseEnt;
	}

	void getName(char* buffer, size_t len) {
		read_raw<char>(baseEntity + 0xA, buffer, len); //Name
	}

	signed getTeam() {
		return read<BYTE>(baseEntity + 0x9); // Team
	}

	int getHealth() {
		return read<int>(baseEntity + 0x44); // Health
	}

	int getKill() {
		return read<int>(baseEntity + 0x48); //Kill
	}

	void setTeam(BYTE id) {
		write<BYTE>(baseEntity + 0x9, id);
	}
};

DWORD64 GetPlayerByIndex(int i) {
	return ((CLT_SHELL + ENT_BEGIN + (i * ENT_SIZE)));
}

bool InGame(DWORD64 clt) {
	return clt && read<BYTE>(clt + LOCAL_ENT);
}

void SupportFunction() {
	float oldx = 0, oldy = 0, oldz = 0;
	bool isSecondPerson = false;
	bool isNC_NR = false;
	bool isDrawCrosshair = false;
	bool isAimbot = false;
	bool isBPOn = false;
	int roomid = 0;

	uintptr_t thirdPerson_Base = read<uintptr_t>(CShell_x64.baseAddr + dwThirdPerson_Base);
	uintptr_t copyRoom_base = read<uintptr_t>(CShell_x64.baseAddr + dwCopyRoomBase);
	uintptr_t copyRoom_base1 = read<uintptr_t>(copyRoom_base + dwCopyRoom_Offset1);
	while (true) {
		int inGameStatus = read<int>(CShell_x64.baseAddr + dwinGameStatus);
		if (SkillE) {
			//Skill E
			if (GetAsyncKeyState(SkillE) & 1) {
				if (inGameStatus == 0xB) {
					uintptr_t skillE_Base = read<uintptr_t>(CShell_x64.baseAddr + dwSkillE_Base);
					int skillE_read = read<int>(skillE_Base + dwSkillE_Offset);
					if (skillE_read == 2) {
						write<int>(skillE_Base + dwSkillE_Offset, 0);
					}
				}
			}
		}
		
		
		if (CheckPoint) {
			//Checkpoint
			if (GetAsyncKeyState(CheckPoint) & 1) {
				if (inGameStatus == 0xB) {
					Beep(500, 300);
					uintptr_t coordinate_base = read<uintptr_t>(CShell_x64.baseAddr + dwCoordinate_Base);
					uintptr_t base = read<uintptr_t>(coordinate_base + dwCoordinate_Offset1);
					oldy = read<float>(base + dwCoordinate_Offset2 - 0x4);
					oldz = read<float>(base + dwCoordinate_Offset2);
					oldx = read<float>(base + dwCoordinate_Offset2 + 0x4);
				}
			}
		}
		
		if (ReturnCheckPoint) {
			//Go to the last checkpoint
			if (GetAsyncKeyState(ReturnCheckPoint) & 1) {
				if (inGameStatus == 0xB) {
					Beep(300, 300);
					uintptr_t coordinate_base = read<uintptr_t>(CShell_x64.baseAddr + dwCoordinate_Base);
					uintptr_t base = read<uintptr_t>(coordinate_base + dwCoordinate_Offset1);
					write<float>(base + dwCoordinate_Offset2 - 0x4, oldy);
					write<float>(base + dwCoordinate_Offset2, oldz);
					write<float>(base + dwCoordinate_Offset2 + 0x4, oldx);
				}
			}
		}
		
		if (ThirdPerson) {
			//Third person
			if (GetAsyncKeyState(ThirdPerson) & 1) {
				isSecondPerson = !isSecondPerson;
				if (isSecondPerson) {
					write<int>(thirdPerson_Base + dwThirdPerson_Offset, 3);
				}
				else {
					write<int>(thirdPerson_Base + dwThirdPerson_Offset, 1);
				}
			}
		}
		
		if (SecondPerson) {
			//Second person
			if (GetAsyncKeyState(SecondPerson) & 1) {
				isSecondPerson = !isSecondPerson;
				if (isSecondPerson) {
					write<int>(thirdPerson_Base + dwThirdPerson_Offset, 2);
				}
				else {
					write<int>(thirdPerson_Base + dwThirdPerson_Offset, 1);
				}
			}
		}
		
		if (CopyRoomID) {
			//Copy room ID
			if (GetAsyncKeyState(CopyRoomID) & 1) {
				Beep(500, 300);
				roomid = read<int>(copyRoom_base1 + dwCopyRoom_Offset2);
			}
		}
		
		if (PasteRoomID) {
			//Paste room ID
			if (GetAsyncKeyState(PasteRoomID) & 1) {
				Beep(300, 300);
				if (roomid != 0) {
					write<int>(copyRoom_base1 + dwCopyRoom_Offset2, roomid);
				}
			}
		}
		
		
		if (inGameStatus == 11) {
			if (GetAsyncKeyState(VK_LBUTTON)) {
				DWORD oldProt = changeProtection(pid, CShell_x64.baseAddr + dwNoRecoil, 1, PAGE_EXECUTE_READWRITE);
				write<float>(CShell_x64.baseAddr + dwNoRecoil, 0.0f);
				changeProtection(pid, CShell_x64.baseAddr + dwNoRecoil, 1, oldProt);
			}
			else {
				DWORD oldProt = changeProtection(pid, CShell_x64.baseAddr + dwNoRecoil, 1, PAGE_EXECUTE_READWRITE);
				write<float>(CShell_x64.baseAddr + dwNoRecoil, -1.0f);
				changeProtection(pid, CShell_x64.baseAddr + dwNoRecoil, 1, oldProt);
			}
		}

		if (DrawCrosshair) {
			//Draw crosshair
			if (GetAsyncKeyState(DrawCrosshair) & 1) {
				Beep(300, 500);
				isDrawCrosshair = !isDrawCrosshair;
			}
			if (isDrawCrosshair) {
				int width = 0, height = 0;
				RECT rect;
				if (GetWindowRect(FindWindowA(0, RGS("CROSSFIRE")), &rect))
				{
					width = rect.right - rect.left;
					height = rect.bottom - rect.top;
				}
				draw_box(width / 2,  height / 2, 3, 3, 3, 255, 0, 0);
			}
		}
		NoBugDamage();

		CLT_SHELL = read<uintptr_t>(CShell_x64.baseAddr + LTShell);
		if (InGame(CLT_SHELL)) {
			signed meIndex = read<BYTE>(CLT_SHELL + LOCAL_ENT_INDEX);
			Player localPlayer(GetPlayerByIndex(meIndex));
			if (localPlayer.getTeam() == 0) {
				if (GetAsyncKeyState(VK_XBUTTON1)) {
					for (int i = 0; i < 17; i++) {
						Player curP(GetPlayerByIndex(i));
						if (meIndex == i) continue;
						curP.setTeam(0);
					}
				}
				else {
					for (int i = 0; i < 17; i++) {
						Player curP(GetPlayerByIndex(i));
						if (meIndex == i) continue;
						curP.setTeam(1);
					}
				}
			}
			else {
				if (GetAsyncKeyState(VK_XBUTTON1)) {
					for (int i = 0; i < 17; i++) {
						Player curP(GetPlayerByIndex(i));
						if (meIndex == i) continue;
						curP.setTeam(1);
					}
				}
				else {
					for (int i = 0; i < 17; i++) {
						Player curP(GetPlayerByIndex(i));
						if (meIndex == i) continue;
						curP.setTeam(0);
					}
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

void pressK() {
	keybd_event(0x4B, 0x25, 0, 0);
	keybd_event(0x4B, 0x25, KEYEVENTF_KEYUP, 0);
}

DWORD WINAPI NameEsp_Auto(LPVOID) {
	while (1) {
		Sleep(1);
		CLT_SHELL = read<uintptr_t>(CShell_x64.baseAddr + LTShell);
		if (!CLT_SHELL) continue;
		signed meIndex = read<BYTE>(CLT_SHELL + LOCAL_ENT_INDEX);
		if (meIndex == 255) continue;
		Player localPlayer(GetPlayerByIndex(meIndex));
		signed CheckTeam = localPlayer.getTeam();
		if (InGame(CLT_SHELL)) {
			if (localPlayer.getHealth() > 0) {
				if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
					for (int i = 0; i < 32; i++) {
						Player curP(GetPlayerByIndex(i));
						if (i != meIndex)
							curP.setTeam(2);
					}
					pressK();
				}
				else {
					for (int i = 0; i < 32; i++) {
						Player curP(GetPlayerByIndex(i));
						if (i != meIndex)
							curP.setTeam(CheckTeam);
					}
				}
			}
			else {
				for (int i = 0; i < 32; i++) {
					Player curP(GetPlayerByIndex(i));
					if (i != meIndex)
						curP.setTeam(CheckTeam);
				}
			}
		}
	}
	return 0;
}

DWORD WINAPI STW(LPVOID) {
	vector<bool> ESE_default = { 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1 };
	vector<bool> WS_default = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 };
	vector<bool> PS_default = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 };

	bool isOnOnce = false;
	bool isOffOnce = false;
	while (1) {
		Sleep(1);
		CLT_SHELL = read<uintptr_t>(CShell_x64.baseAddr + LTShell);
		if (InGame(CLT_SHELL)) {
			//Turn on STW
			if (!isOnOnce) {
				Beep(500, 500);
				uintptr_t WallPointer = read<uintptr_t>(CShell_x64.baseAddr + 0x27FA2D8);
				for (int i = 0; i < 20; i++) {
					write<bool>(WallPointer + (i * 0x82C) + 0x7DC, true);
					write<bool>(WallPointer + (i * 0x82C) + 0x7E0, true);
					write<bool>(WallPointer + (i * 0x82C) + 0x7E4, true);
				}
				isOnOnce = true;
				isOffOnce = false;
			}
		}
		else {
			//Backup default value
			if (!isOffOnce) {
				Beep(300, 500);
				uintptr_t WallPointer = read<uintptr_t>(CShell_x64.baseAddr + 0x27FA2D8);
				for (int i = 0; i < 20; i++) {
					write<bool>(WallPointer + (i * 0x82C) + 0x7DC, ESE_default[i]);
					write<bool>(WallPointer + (i * 0x82C) + 0x7E0, WS_default[i]);
					write<bool>(WallPointer + (i * 0x82C) + 0x7E4, PS_default[i]);
				}
				isOffOnce = true;
				isOnOnce = false;
			}
		}
	}
	return 0;
}


#define msg(str) MessageBoxA(0, str, "Info", MB_OK);
#define __CODERDUC__ int main(void)

__CODERDUC__{
	//setup();
	//Sleep(1000);

	if (!pid && !crossfire.baseAddr && !CShell_x64.baseAddr && !ClientFx_x64.baseAddr) {
		pid = get_process_id(RGS("crossfire.dat"));
		crossfire = get_module_base_addr(RGS("crossfire.dat"));
		CShell_x64 = get_module_base_addr(RGS("cshell_x64.dll"));
		ClientFx_x64 = get_module_base_addr(RGS("ClientFx_x64.fxd"));
	}
	std::cout << R"(
  ________    _____     ________.__  .__  __         .__
 /  _____/   /  |  |   /  _____/|  | |__|/  |_  ____ |  |__   ___________
/   \  ___  /   |  |_ /   \  ___|  | |  \   __\/ ___\|  |  \_/ __ \_  __ \
\    \_\  \/    ^   / \    \_\  \  |_|  ||  | \  \___|   Y  \  ___/|  | \/
 \______  /\____   |   \______  /____/__||__|  \___  >___|  /\___  >__|
		\/      |__|          \/                   \/     \/     \/
)" << '\n';
	
	//CFO
	//cshell_x64.WebView::CFWebArgument::operator=+344E - 49 69 D5 2C080000     - imul rdx,r13,0000082C { 2092 } ==> size

	//cshell_x64.WebView::CFWebArgument::operator=+3455 - 48 8B 0D 9C348102     - mov rcx,[cshell_x64.dll+29A7958] { (0) }  => base

	//cshell_x64.WebView::CFWebArgument::operator=+345C - 89 84 0A DC070000     - mov [rdx+rcx+000007DC],eax ==> EdgeShotEnabled

	//cshell_x64.WebView::CFWebArgument::operator=+349B - 89 84 0A E0070000     - mov [rdx+rcx+000007E0],eax ==> Wallshot

	//cshell_x64.WebView::CFWebArgument::operator=+34DA - 89 84 0A E4070000     - mov [rdx+rcx+000007E4],eax ==> PerfectShot

	CreateThread(0, 0, NameEsp_Auto, 0, 0, 0);
	CreateThread(0, 0, STW, 0, 0, 0);

	while (true) {
		HWND CF = FindWindowA(0, skCrypt("CROSSFIRE"));
		if (CF) {
			if ((GetAsyncKeyState(VK_DELETE) & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
				Beep(300, 500);
				exit(1);
			}
		}
		else {
			Beep(300, 500);
			exit(0);
		}
		Sleep(1);
	}
	//CFVN
	// 49 69 d5 ? ? ? ? 48 8b 0d ? ? ? ? 89 84 0a ? ? ? ? 4b 8b 1c f4 48 8d 15 ? ? ? ? 48 8b cb e8 ? ? ? ? 48 85 c0 74 ? 48 8b 40 ? 48 8b 48 ? 48 8b 49 ? ff 15 ? ? ? ? 49 69 d5 ? ? ? ? 48 8b 0d ? ? ? ? 89 84 0a ? ? ? ? 4b 8b 1c f4
	//cshell_x64.WebView::CFWebArgument::operator=+344E - 49 69 D5 2C080000     - imul rdx,r13,0000082C { 2092 }
	// 48 8b 0d ? ? ? ? 89 84 0a ? ? ? ? 4b 8b 1c f4 48 8d 15 ? ? ? ? 48 8b cb e8 ? ? ? ? 48 85 c0 74 ? 48 8b 40 ? 48 8b 48 ? 48 8b 49 ? ff 15 ? ? ? ? 49 69 d5 ? ? ? ? 48 8b 0d ? ? ? ? 89 84 0a ? ? ? ? 4b 8b 1c f4
	//cshell_x64.WebView::CFWebArgument::operator=+3455 - 48 8B 0D 3C9C6B02     - mov rcx,[cshell_x64.dll+27FA2D8] { (0) }
	//89 84 0a ? ? ? ? 4b 8b 1c f4 48 8d 15 ? ? ? ? 48 8b cb e8 ? ? ? ? 48 85 c0 74 ? 48 8b 40 ? 48 8b 48 ? 48 8b 49 ? ff 15 ? ? ? ? 49 69 d5 ? ? ? ? 48 8b 0d ? ? ? ? 89 84 0a ? ? ? ? 4b 8b 1c f4
	//cshell_x64.WebView::CFWebArgument::operator=+345C - 89 84 0A DC070000     - mov [rdx+rcx+000007DC],eax
	//89 84 0a ? ? ? ? 4b 8b 1c f4 48 8d 15 ? ? ? ? 48 8b cb e8 ? ? ? ? 48 85 c0 74 ? 48 8b 40 ? 48 8b 48 ? 48 8b 49 ? ff 15 ? ? ? ? 49 69 d5 ? ? ? ? 48 8b 0d ? ? ? ? 89 84 0a ? ? ? ? ff 05
	//cshell_x64.WebView::CFWebArgument::operator=+349B - 89 84 0A E0070000     - mov [rdx+rcx+000007E0],eax
	//89 84 0a ? ? ? ? ff 05
	//cshell_x64.WebView::CFWebArgument::operator=+34DA - 89 84 0A E4070000     - mov [rdx+rcx+000007E4],eax
	

	

	system(RGS("pause"));
	return 0;
}


////ModelBute (Damage Factor)
	//uintptr_t base = read<uintptr_t>(crossfire.baseAddr + 0x109FE60);
	//uintptr_t p = read<uintptr_t>(base + 0x40);
	//uintptr_t p1 = read<uintptr_t>(p + 0x10);

	//for (int i = 0; i < 2839; i++) {
	//	uintptr_t p2 = read<uintptr_t>(p1 + (i * 8));
	//	float r = read<float>(p2 + 0x38);
	//	cout << i << " - " << r << endl;
	//}

//Dimensions Log
//ofstream fileout("1.txt");
//fileout << "{ ";
//for (int i = 1; i <= 2915; i++) {
//
//	float x = read<float>(dDimensionBase + (i - 1) * 0xA4 + 0x38);
//	float y = read<float>(dDimensionBase + (i - 1) * 0xA4 + 0x3C);
//	float z = read<float>(dDimensionBase + (i - 1) * 0xA4 + 0x40);
//	if (x == 99.0 && y == 99.0 && z == 99.0)
//		fileout << "{" << i << ", {" << x << "," << y << "," << z << "} }, ";
//}
//fileout << "}";
//fileout.close();

////Room bot
		//if (GetAsyncKeyState(VK_F4) & 1) {
		//	Beep(500, 500);
		//	if (inGameStatus == 8) {
		//		uintptr_t roomBotBase = read<uintptr_t>(cshell.baseAddr + dwRoomBot);
		//		int room = read<int>(read<uintptr_t>(read<uintptr_t>(roomBotBase + dwRoomBot_Offset1) + dwRoomBot_Offset2) + dwRoomBot_Offset3);
		//		if (room != 1) {
		//			write<int>(read<uintptr_t>(read<uintptr_t>(roomBotBase + dwRoomBot_Offset1) + dwRoomBot_Offset2) + dwRoomBot_Offset3, 1);
		//			write<int>(read<uintptr_t>(read<uintptr_t>(roomBotBase + dwRoomBot_Offset1) + dwRoomBot_Offset2) + dwRoomBot_Offset4, 1);
		//		}
		//	}
		//}

/*vector<pair<int, vector<float>>> DimensionsMod = { {1139, {150, 200, 150}}, {31, {99,99,99} }, {46, {99,99,99} }, {72, {99,99,99} }, {88, {99,99,99} }, {103, {99,99,99} }, {118, {99,99,99} }, {140, {99,99,99} }, {159, {99,99,99} }, {160, {99,99,99} }, {189, {99,99,99} }, {204, {99,99,99} }, {264, {99,99,99} }, {284, {99,99,99} }, {312, {99,99,99} }, {328, {99,99,99} }, {345, {99,99,99} }, {362, {99,99,99} }, {377, {99,99,99} }, {393, {99,99,99} }, {431, {99,99,99} }, {446, {99,99,99} }, {468, {99,99,99} }, {489, {99,99,99} }, {530, {99,99,99} }, {545, {99,99,99} }, {571, {99,99,99} }, {604, {99,99,99} }, {619, {99,99,99} }, {632, {99,99,99} }, {634, {99,99,99} }, {654, {99,99,99} }, {675, {99,99,99} }, {706, {99,99,99} }, {717, {99,99,99} }, {727, {99,99,99} }, {735, {99,99,99} }, {743, {99,99,99} }, {761, {99,99,99} }, {771, {99,99,99} }, {782, {99,99,99} }, {792, {99,99,99} }, {800, {99,99,99} }, {811, {99,99,99} }, {821, {99,99,99} }, {829, {99,99,99} }, {837, {99,99,99} }, {855, {99,99,99} }, {865, {99,99,99} }, {876, {99,99,99} }, {886, {99,99,99} }, {910, {99,99,99} }, {923, {99,99,99} }, {938, {99,99,99} }, {953, {99,99,99} }, {999, {99,99,99} }, {1026, {99,99,99} }, {1036, {99,99,99} }, {1051, {99,99,99} }, {1062, {99,99,99} }, {1063, {99,99,99} }, {1079, {99,99,99} }, {1090, {99,99,99} }, {1105, {99,99,99} }, {1140, {99,99,99} }, {1158, {99,99,99} }, {1173, {99,99,99} }, {1188, {99,99,99} }, {1213, {99,99,99} }, {1228, {99,99,99} }, {1248, {99,99,99} }, {1249, {99,99,99} }, {1250, {99,99,99} }, {1266, {99,99,99} }, {1282, {99,99,99} }, {1283, {99,99,99} }, {1312, {99,99,99} }, {1329, {99,99,99} }, {1353, {99,99,99} }, {1378, {99,99,99} }, {1406, {99,99,99} }, {1416, {99,99,99} }, {1424, {99,99,99} }, {1442, {99,99,99} }, {1465, {99,99,99} }, {1480, {99,99,99} }, {1506, {99,99,99} }, {1514, {99,99,99} }, {1532, {99,99,99} }, {1547, {99,99,99} }, {1592, {99,99,99} }, {1602, {99,99,99} }, {1619, {99,99,99} }, {1629, {99,99,99} }, {1640, {99,99,99} }, {1648, {99,99,99} }, {1649, {99,99,99} }, {1678, {99,99,99} }, {1696, {99,99,99} }, {1709, {99,99,99} }, {1725, {99,99,99} }, {1739, {99,99,99} }, {1768, {99,99,99} }, {1784, {99,99,99} }, {1797, {99,99,99} }, {1799, {99,99,99} }, {1826, {99,99,99} }, {1850, {99,99,99} }, {1882, {99,99,99} }, {1897, {99,99,99} }, {1916, {99,99,99} }, {1941, {99,99,99} }, {1953, {99,99,99} }, {1979, {99,99,99} }, {1980, {99,99,99} }, {1991, {99,99,99} }, {2007, {99,99,99} }, {2034, {99,99,99} }, {2071, {99,99,99} }, {2072, {99,99,99} }, {2101, {99,99,99} }, {2116, {99,99,99} }, {2181, {99,99,99} }, {2224, {99,99,99} }, {2248, {99,99,99} }, {2272, {99,99,99} }, {2294, {99,99,99} }, {2313, {99,99,99} }, {2328, {99,99,99} }, {2342, {99,99,99} }, {2343, {99,99,99} }, {2372, {99,99,99} }, {2387, {99,99,99} }, {2388, {99,99,99} }, {2417, {99,99,99} }, {2441, {99,99,99} }, {2442, {99,99,99} }, {2471, {99,99,99} }, {2508, {99,99,99} }, {2509, {99,99,99} }, {2546, {99,99,99} }, {2567, {99,99,99} }, {2568, {99,99,99} }, {2597, {99,99,99} }, {2613, {99,99,99} }, {2628, {99,99,99} }, {2629, {99,99,99} }, {2694, {99,99,99} }, {2710, {99,99,99} }, {2725, {99,99,99} }, {2744, {99,99,99} }, {2783, {99,99,99} }, {2800, {99,99,99} }, {2820, {99,99,99} }, {2840, {99,99,99} }, {2866, {99,99,99} }, {2877, {99,99,99} }, {2903, {99,99,99} }, {2904, {99,99,99} } };
	uintptr_t dDimensionBase = read<uintptr_t>(CShell_x64.baseAddr + 0x2771908);

	for (int i = 1; i <= 2915; i++) {
		for (int j = 0; j < DimensionsMod.size(); j++) {
			if (i == DimensionsMod[j].first) {
				write<float>(dDimensionBase + (i - 1) * 0xA4 + 0x38, DimensionsMod[j].second[0]);
				write<float>(dDimensionBase + (i - 1) * 0xA4 + 0x3C, DimensionsMod[j].second[1]);
				write<float>(dDimensionBase + (i - 1) * 0xA4 + 0x40, DimensionsMod[j].second[2]);
				break;
			}
		}
	}*/

////Bunnyhop
//if (GetAsyncKeyState(VK_F4) & 1) {
//	Beep(300, 500);
//	isBunny = !isBunny;
//}
//if (isBunny) {
//	write<int>(cshell.baseAddr + dwBunnyhop, 0);
//}

//void NoSmoke() {
//	BYTE noSmoke[] = { 0xEB };
//	changeProtection(pid, crossfire.baseAddr + dwCrossfireWebView + dwNoSmoke, 1, PAGE_EXECUTE_READWRITE);
//	writeBytes(crossfire.baseAddr + dwCrossfireWebView + dwNoSmoke, &noSmoke, sizeof(noSmoke));
//	changeProtection(pid, crossfire.baseAddr + dwCrossfireWebView + dwNoSmoke, 1, 0);
//}

//Increase hit box
//while (true) {
	//	int inGameStatus = read<int>(_rt.baseAddr + 0x273A174);
	//	uintptr_t t = read<uintptr_t>(_rt.baseAddr + 0x2738CB0);
	//	if (inGameStatus == 0xD) {//Ngoai sanh
	//		for (int x = 1; x <= 244; x++) {
	//			r = read<int>(t + (0xD14 + ((0xEEC * x) - 0xEEC)));
	//			Backup[x] = r;
	//			write<int>(t + (0xD14 + ((0xEEC * x) - 0xEEC)), 0xB1);
	//		}
	//	}
	//	else if (inGameStatus == 0xB) {//Trong game
	//		for (int x = 1; x <= 244; x++) {
	//			write<int>(t + (0xD14 + ((0xEEC * x) - 0xEEC)), Backup[x]);
	//		}
	//	}
	//}

//Bug Zombie
//static uintptr_t BaseZombie, GameUI, NanoZombie;
//BaseZombie = read<uintptr_t>(crossfire.baseAddr + a__BaseBugZombie);
//if (BaseZombie) {
//	GameUI = read<uintptr_t>(BaseZombie + a__OffsetBugZombie1);
//	if (GameUI) {
//		NanoZombie = read<uintptr_t>(GameUI + a__OffsetBugZombie2);
//		if (NanoZombie) {
//			for (int x = 0; x < 7; x++) {
//				changeProtection(pid, NanoZombie + a__OffsetBugZombie3 + (4 * x), 1, PAGE_EXECUTE_READWRITE);
//				write<int>(NanoZombie + a__OffsetBugZombie3 + (4 * x), 1);
//				std::cout << "Zombie Done" << std::endl;
//			}
//		}
//	}
//}
