#include "inc.h"

std::string username;
std::string password;
CURL* curl;
std::string readBuffer;

std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

const char* wstringToChar(const std::wstring& wideString) {
	static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	static std::string narrowString;

	narrowString = converter.to_bytes(wideString);
	return narrowString.c_str();
}

std::wstring get_files_directory()
{
	WCHAR system_dir[256];
	const char* s = RGS("\\SoftwareDistribution\\Download\\");
	GetWindowsDirectoryW(system_dir, 256);
	std::wstring sz_dir = (std::wstring(system_dir) + s2ws(s));
	return sz_dir;
}

std::wstring get_random_file_name_directory(std::wstring type_file)
{
	std::wstring sz_file = get_files_directory() + type_file;
	return sz_file;
}

void run_us_admin(std::wstring sz_exe, bool show)
{
	ShellExecuteW(NULL, L"runas", sz_exe.c_str(), NULL, NULL, show);
}

void run_us_admin_and_params(std::wstring sz_exe, std::wstring sz_params, bool show)
{
	ShellExecuteW(NULL, L"runas", sz_exe.c_str(), sz_params.c_str(), NULL, show);
}

bool drop_mapper(std::wstring path)
{
	HANDLE h_file;
	BOOLEAN b_status = FALSE;
	DWORD byte = 0;

	h_file = CreateFileW(path.c_str(), GENERIC_ALL, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (GetLastError() == ERROR_FILE_EXISTS)
		return true;

	if (h_file == INVALID_HANDLE_VALUE)
		return false;

	b_status = WriteFile(h_file, mapper_shellcode, sizeof(mapper_shellcode), &byte, nullptr);
	CloseHandle(h_file);

	if (!b_status)
		return false;

	return true;
}

bool drop_driver(std::wstring path)
{
	HANDLE h_file;
	BOOLEAN b_status = FALSE;
	DWORD byte = 0;

	h_file = CreateFileW(path.c_str(), GENERIC_ALL, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (GetLastError() == ERROR_FILE_EXISTS)
		return true;

	if (h_file == INVALID_HANDLE_VALUE)
		return false;

	b_status = WriteFile(h_file, driver_shellcode, sizeof(driver_shellcode), &byte, nullptr);
	CloseHandle(h_file);

	if (!b_status)
		return false;

	return true;
}

void loadDriver() {
	const char* drv = RGS("g4drv.sys");
	const char* mapper = RGS("ggwp.exe");
	
	std::wstring sz_driver = get_random_file_name_directory(s2ws(drv));
	std::wstring sz_mapper = get_random_file_name_directory(s2ws(mapper));
	std::wstring sz_params_map = sz_driver;

	drop_driver(sz_driver);
	drop_mapper(sz_mapper);

	run_us_admin_and_params(sz_mapper, sz_params_map, false);
	Sleep(1000);
	DeleteFileW(sz_driver.c_str());
	DeleteFileW(sz_mapper.c_str());
}

static BOOL SetConsoleSize(int cols, int rows) {
	HWND hWnd;
	HANDLE hConOut;
	CONSOLE_FONT_INFO fi;
	CONSOLE_SCREEN_BUFFER_INFO bi;
	int w, h, bw, bh;
	RECT rect = { 0, 0, 0, 0 };
	COORD coord = { 0, 0 };
	hWnd = GetConsoleWindow();
	if (hWnd) {
		hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hConOut && hConOut != (HANDLE)-1) {
			if (GetCurrentConsoleFont(hConOut, FALSE, &fi)) {
				if (GetClientRect(hWnd, &rect)) {
					w = rect.right - rect.left;
					h = rect.bottom - rect.top;
					if (GetWindowRect(hWnd, &rect)) {
						bw = rect.right - rect.left - w;
						bh = rect.bottom - rect.top - h;
						if (GetConsoleScreenBufferInfo(hConOut, &bi)) {
							coord.X = bi.dwSize.X;
							coord.Y = bi.dwSize.Y;
							if (coord.X < cols || coord.Y < rows) {
								if (coord.X < cols) {
									coord.X = cols;
								}
								if (coord.Y < rows) {
									coord.Y = rows;
								}
								if (!SetConsoleScreenBufferSize(hConOut, coord)) {
									return FALSE;
								}
							}
							return SetWindowPos(hWnd, NULL, rect.left, rect.top, cols * fi.dwFontSize.X + bw, rows * fi.dwFontSize.Y + bh, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
						}
					}
				}
			}
		}
	}
	return FALSE;
}

std::string gen_string(const int length)
{
	std::string GeneratedString;
	static const char Alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	srand((unsigned)time(NULL) * 5);
	for (int i = 0; i < length; i++)
		GeneratedString += Alphabet[rand() % (sizeof(Alphabet) - 1)];
	return GeneratedString;
}

size_t WriteCallback(char* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string takePasswdFromUser()
{
	char pin[15];
	int k = 0;
	while (pin[k - 1] != '\r') {
		pin[k] = _getch();
		if (pin[k - 1] != '\r') {
			std::cout << RGS("*");
		}
		k++;
	}
	pin[k - 1] = '\0';
	return pin;
}

void setup() {
	SetConsoleSize(90, 10);
	SetConsoleTitleA(gen_string(15).c_str());
	system(RGS("COLOR 0e"));
	system(RGS("cls"));
	SetConsoleCP(437);
	SetConsoleOutputCP(437);

	if (!FindWindowA(0, RGS("CROSSFIRE"))) {
		std::cout << RGS("[!] Please open the game first.") << std::endl;
		Sleep(2000);
		exit(1);
	}
	loadDriver();
	////Authorization
	//std::cout << RGS("Username: ");
	//std::cin >> username;
	//std::cout << RGS("Password: ");
	//password = takePasswdFromUser();
	//std::cout << "\n";
	//std::string data = RGS("username=") + username + RGS("&password=") + password;
	//const char* data2Send = data.c_str();

	//struct curl_slist* headers = NULL;

	//curl_global_init(CURL_GLOBAL_ALL);
	//curl = curl_easy_init();
	//if (curl) {
	//	headers = curl_slist_append(headers, RGS("X-Parse-Application-Id: aaaaaaahffb831460d02f0a4be23419e5cc3920d"));
	//	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	//	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data2Send);
	//	curl_easy_setopt(curl, CURLOPT_URL, RGS("https://apps.simbla.com/parse/login"));
	//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	//	curl_easy_perform(curl);
	//	if (readBuffer.find(RGS("objectId")) != std::string::npos) {
	//		std::cout << RGS("[+] Login successfully !") << std::endl;
	//		Sleep(5000);
	//		system("cls");
	//		int bar1 = 177, bar2 = 219;
	//		std::cout << RGS("\n\n\n\t\t\t\t   [>] Loading ...");
	//		std::cout << RGS("\n\n\n\t\t\t\t");
	//		for (int i = 0; i < 25; i++) {
	//			std::cout << (char)bar1;
	//		}
	//		std::cout << RGS("\r");
	//		std::cout << RGS("\t\t\t\t");
	//		for (int i = 0; i < 25; i++) {
	//			std::cout << (char)bar2;
	//			Sleep(150);
	//		}
	//		std::cout << "\n";
	//		loadDriver();
	//		system("cls");
	//		
	//	}
	//	else {
	//		std::cout << RGS("[!] Failed to log in !") << std::endl;
	//		Sleep(2000);
	//		exit(1);
	//	}
	//	curl_easy_cleanup(curl);
	//}
	//curl_slist_free_all(headers);
	//curl_global_cleanup();
}