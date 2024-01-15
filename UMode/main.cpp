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
	ULONG newProtect;
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
StructDll cshell;
StructDll ClientFx_x64;
HWND hWnd = NULL;
int status;

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

bool changeProtection(HANDLE pid, UINT_PTR address, ULONG size, ULONG newProtect) {
	NULL_MEMORY instr = { 0 };
	ULONG oldProtect;
	instr.changeProtect = true;
	instr.pid = pid;
	instr.addr = address;
	instr.size = size;
	instr.newProtect = newProtect;
	call_hook(instr);
	return true;
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
	changeProtection(pid, cshell.baseAddr + dwNoBugDamage, 19, PAGE_EXECUTE_READWRITE);
	BYTE writeBuffer[] = { 0x72,0x65,0x7A,0x5C,0x42,0x75,0x74,0x65,0x73,0x5C,0x42,0x46,0x30,0x33,0x35 };
	writeBytes(cshell.baseAddr + dwNoBugDamage, &writeBuffer, sizeof(writeBuffer));
	changeProtection(pid, cshell.baseAddr + dwNoBugDamage, 19, 0);
}

void NoFlash() {
	BYTE bytes[] = { 0x49,0x6d,0x70,0x6f,0x55,0x49,0x2f,0x4e,0x6f,0x69,0x73,0x65,0x53,0x63,0x72,0x65,0x65,0x6e,0x2f,0x4e,0x6f,0x69,0x73,0x65,0x46,0x69,0x6c,0x74,0x65,0x72,0x2e,0x64,0x74,0x78 };
	changeProtection(pid, cshell.baseAddr + dwNoFlash1, 34, PAGE_EXECUTE_READWRITE);
	changeProtection(pid, cshell.baseAddr + dwNoFlash2, 34, PAGE_EXECUTE_READWRITE);
	writeBytes(cshell.baseAddr + dwNoFlash1, &bytes, sizeof(bytes));
	writeBytes(cshell.baseAddr + dwNoFlash2, &bytes, sizeof(bytes));
	changeProtection(pid, cshell.baseAddr + dwNoFlash1, 34, 0);
	changeProtection(pid, cshell.baseAddr + dwNoFlash2, 34, 0);
}

void NoFallDamage() {
	uintptr_t base = read<uintptr_t>(cshell.baseAddr + dwNoFallDamage);
	write<float>(base + dwNoFallDamageOffset, 0.0f);
}

void AllBypass() {
	//Bypass Screen Capture
	BYTE bypassScreen[] = { 0xE9, 0x9E, 0x00, 0x00, 0x00, 0x90 };
	changeProtection(pid, cshell.baseAddr + dwBPSendScreen, 6, PAGE_EXECUTE_READWRITE);
	writeBytes(cshell.baseAddr + dwBPSendScreen, &bypassScreen, sizeof(bypassScreen));
	changeProtection(pid, cshell.baseAddr + dwBPSendScreen, 6, 0);

	//Bypass Detect Tool
	BYTE BPDetectTool[] = { 0xEB };
	changeProtection(pid, cshell.baseAddr + dwBPToolCheatDetect0, 2, PAGE_EXECUTE_READWRITE);
	changeProtection(pid, cshell.baseAddr + dwBPToolCheatDetect1, 2, PAGE_EXECUTE_READWRITE);
	writeBytes(cshell.baseAddr + dwBPToolCheatDetect0, &BPDetectTool, sizeof(BPDetectTool));
	writeBytes(cshell.baseAddr + dwBPToolCheatDetect1, &BPDetectTool, sizeof(BPDetectTool));
	changeProtection(pid, cshell.baseAddr + dwBPToolCheatDetect0, 2, 0);
	changeProtection(pid, cshell.baseAddr + dwBPToolCheatDetect1, 2, 0);
}

void Wall_Hack() {
	bool once_ingame = false;
	bool once_outgame = false;
	while (true) {
		int inGameStatus = read<int>(cshell.baseAddr + dwinGameStatus);
		if (inGameStatus == 0xB) {
			if (once_ingame == false) {
				Sleep(60000);
				write<int>(crossfire.baseAddr + dwWallhack + dwWallhackOffset, 256);
				once_ingame = true;
				once_outgame = false;
			}
		}

		else {
			if (once_outgame == false) {
				write<int>(crossfire.baseAddr + dwWallhack + dwWallhackOffset, 257);
				once_outgame = true;
				once_ingame = false;
			}
		}
	}
}

void GMChat() {
	changeProtection(pid, cshell.baseAddr + dwGMChat, 10, PAGE_EXECUTE_READWRITE);
	write<int>(cshell.baseAddr + dwGMChat, 257);
	changeProtection(pid, cshell.baseAddr + dwGMChat, 10, 0);
}

typedef int(*pDD_btn)(int btn);
typedef int(*pDD_movR)(int dx, int dy);

pDD_btn      DD_btn;          //Mouse button
pDD_movR   DD_movR;	     //VK to ddcode

void SupportFunction() {
	float oldx = 0, oldy = 0, oldz = 0;
	bool isSecondPerson = false;
	bool isNC_NR = false;
	bool isNoRecoilOn = false;
	bool isDrawCrosshair = false;
	bool isAimbot = false;
	int roomid = 0;

	HINSTANCE _hGDI = LoadLibraryA("gdi32.dll");
	uintptr_t thirdPerson_Base = read<uintptr_t>(cshell.baseAddr + dwThirdPerson_Base);
	uintptr_t copyRoom_base = read<uintptr_t>(cshell.baseAddr + dwCopyRoomBase);
	uintptr_t copyRoom_base1 = read<uintptr_t>(copyRoom_base + dwCopyRoom_Offset1);
	uintptr_t state_base = read<uintptr_t>(cshell.baseAddr + dwStateBase);
	while (true) {
		int inGameStatus = read<int>(cshell.baseAddr + dwinGameStatus);

		//Skill E
		if (GetAsyncKeyState(VK_SHIFT) & 1) {
			if (inGameStatus == 0xB) {
				uintptr_t skillE_Base = read<uintptr_t>(cshell.baseAddr + dwSkillE_Base);
				int skillE_read = read<int>(skillE_Base + dwSkillE_Offset);
				if (skillE_read == 2) {
					write<int>(skillE_Base + dwSkillE_Offset, 0);
				}
			}
		}

		//Checkpoint
		if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
			if (inGameStatus == 0xB) {
				Beep(500, 300);
				uintptr_t coordinate_base = read<uintptr_t>(cshell.baseAddr + dwCoordinate_Base);
				uintptr_t base = read<uintptr_t>(coordinate_base + dwCoordinate_Offset1);
				oldy = read<float>(base + dwCoordinate_Offset2 - 0x4);
				oldz = read<float>(base + dwCoordinate_Offset2);
				oldx = read<float>(base + dwCoordinate_Offset2 + 0x4);
			}
		}
		//Go to the last checkpoint
		if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
			if (inGameStatus == 0xB) {
				Beep(300, 300);
				uintptr_t coordinate_base = read<uintptr_t>(cshell.baseAddr + dwCoordinate_Base);
				uintptr_t base = read<uintptr_t>(coordinate_base + dwCoordinate_Offset1);
				write<float>(base + dwCoordinate_Offset2 - 0x4, oldy);
				write<float>(base + dwCoordinate_Offset2, oldz);
				write<float>(base + dwCoordinate_Offset2 + 0x4, oldx);
			}
		}

		//Third person
		if (GetAsyncKeyState('V') & 1) {
			isSecondPerson = !isSecondPerson;
			if (isSecondPerson) {
				write<int>(thirdPerson_Base + dwThirdPerson_Offset, 3);
			}
			else {
				write<int>(thirdPerson_Base + dwThirdPerson_Offset, 1);
			}
		}

		//Second person
		if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
			isSecondPerson = !isSecondPerson;
			if (isSecondPerson) {
				write<int>(thirdPerson_Base + dwThirdPerson_Offset, 2);
			}
			else {
				write<int>(thirdPerson_Base + dwThirdPerson_Offset, 1);
			}
		}

		//Copy room ID
		if (GetAsyncKeyState(VK_NUMPAD4) & 1) {
			Beep(500, 300);
			roomid = read<int>(copyRoom_base1 + dwCopyRoom_Offset2);
		}

		if (GetAsyncKeyState(VK_NUMPAD5) & 1) {
			Beep(300, 300);
			if (roomid != 0) {
				write<int>(copyRoom_base1 + dwCopyRoom_Offset2, roomid);
			}
		}
		
		//No Change and No Reload
		if (GetAsyncKeyState(VK_F2) & 1) {
			Beep(300, 500);
			isNC_NR = !isNC_NR;
		}
		if (isNC_NR) {
			uintptr_t nc_nrBase = read<uintptr_t>(cshell.baseAddr + dwNR_NCBase);
			write<int>(nc_nrBase + dwNR_NCOffset, 2);
		}

		//No Recoil
		if (GetAsyncKeyState(VK_F3) & 1) {
			Beep(300, 500);
			isNoRecoilOn = !isNoRecoilOn;
		}
		if (isNoRecoilOn) {
			int state = read<int>(state_base + dwStateOffset);
			int r = read<int>(thirdPerson_Base + dwThirdPerson_Offset);
			if (inGameStatus == 0xB) {
				if (state == 1 && r == 1) {
					Sleep(0.8);
					changeProtection(pid, cshell.baseAddr + dwNoRecoil, 0x1, PAGE_EXECUTE_READWRITE);
					write<float>(cshell.baseAddr + dwNoRecoil, 0.0f);
					changeProtection(pid, cshell.baseAddr + dwNoRecoil, 0x1, PAGE_EXECUTE_READWRITE);
				}
				else {
					Sleep(0.8);
					changeProtection(pid, cshell.baseAddr + dwNoRecoil, 0x1, PAGE_EXECUTE_READWRITE);
					write<float>(cshell.baseAddr + dwNoRecoil, -1.0f);
					changeProtection(pid, cshell.baseAddr + dwNoRecoil, 0x1, PAGE_EXECUTE_READWRITE);
				}
			}
		}
		else {
			changeProtection(pid, cshell.baseAddr + dwNoRecoil, 0x1, PAGE_EXECUTE_READWRITE);
			write<float>(cshell.baseAddr + dwNoRecoil, -1.0f);
			changeProtection(pid, cshell.baseAddr + dwNoRecoil, 0x1, PAGE_EXECUTE_READWRITE);
		}
		
		if (GetAsyncKeyState(VK_NUMPAD6) & 1) {
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
			draw_box((width / 2), (height / 2), 3, 3, 3, 255, 0, 0);
		}

		NoBugDamage();
	}
}


int main(void) {
	setup();
	Sleep(1000);
	if (!pid && !crossfire.baseAddr && !cshell.baseAddr && !ClientFx_x64.baseAddr) {
		pid = get_process_id(RGS("crossfire.dat"));
		crossfire = get_module_base_addr(RGS("crossfire.dat"));
		cshell = get_module_base_addr(RGS("cshell_x64.dll"));
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

	AllBypass(); 
	Sleep(1000);
	NoFallDamage();
	Sleep(1000);
	NoBugDamage();
	Sleep(1000);
	SupportFunction();
	system(RGS("pause"));
	return 0;
}

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