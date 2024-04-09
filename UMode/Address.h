#pragma once
//InGameStatus
// STring: ButtonGlobalQuickJoin
// 8b 15 ? ? ? ? ff 90 ? ? ? ? f6 05
//cshell_x64.WebView::CFWebView::operator=+56BECE - 8B 15 3089C301 - mov edx, [cshell_x64.dll + 276EF84] { [00000000] } <=== inGameStatus 0x276EF84
//cshell_x64.WebView::CFWebView::operator=+56BED4 - FF 90 B0000000 - call qword ptr[rax + 000000B0]
//cshell_x64.WebView::CFWebView::operator=+56BEDA - F6 05 E88DC301 01 - test byte ptr[cshell_x64.dll + 276F449], 01 { [0] }
//cshell_x64.WebView::CFWebView::operator=+56BEE1 - 74 31 - je cshell_x64.WebView::CFWebView::operator=+56BF14
//cshell_x64.WebView::CFWebView::operator=+56BEE3 - 48 8D 15 E6E14F01 - lea rdx, [cshell_x64.dll + 2034850] { ["ButtonGlobalQuickJoin"] }
//cshell_x64.WebView::CFWebView::operator=+56BEEA - 49 8B CE - mov rcx, r14
//cshell_x64.WebView::CFWebView::operator=+56BEED - E8 7E888100 - call cshell_x64.WebView::CFWebView::operator=+D84770
//cshell_x64.WebView::CFWebView::operator=+56BEF2 - 44 38 3D CF8DC301 - cmp[cshell_x64.dll + 276F448], r15l{ [00000000] }
//cshell_x64.WebView::CFWebView::operator=+56BEF9 - 74 19 - je cshell_x64.WebView::CFWebView::operator=+56BF14
#define dwinGameStatus 0x296AAA8

//GM Chat
// 00 01 00 00 00 00 00 00 E0 93 04
// cshell_x64.dll+267C960 - 00 01                 - add [rcx],al ==> cshell_x64.dll+267C960 == GM Chat
//write<int>(_rt.baseAddr + 0x267C960, 257);
#define dwGMChat 0x2705A08

////Address: 4C 8D 25 ?? ?? ?? ?? 48 8B F1 48 89 4C 24 28 B9 ?? ?? ?? ?? 41 8B F8 48 8B DA 4C 8B 08 42 8B 04 09 39 05 ?? ?? ?? ?? 0F 8F
//crossfire.WebView::CFWebView::operator=+167937 - 48 8D AC 24 F0FBFFFF - lea rbp, [rsp - 00000410]
//crossfire.WebView::CFWebView::operator=+16793F - 48 81 EC 10050000 - sub rsp, 00000510 { 1296 }
//crossfire.WebView::CFWebView::operator=+167946 - 48 8B 05 93A22D00 - mov rax, [crossfire.dat + 60B390] { ["cHA|u"] }
//crossfire.WebView::CFWebView::operator=+16794D - 48 33 C4 - skCrypt rax, rsp
//crossfire.WebView::CFWebView::operator=+167950 - 48 89 85 F0030000 - mov[rbp + 000003F0], rax
//crossfire.WebView::CFWebView::operator=+167957 - 65 48 8B 04 25 58000000 - mov rax, gs:[00000058] { 88 }
//crossfire.WebView::CFWebView::operator=+167960 - 4C 8D 25 B9D6DA00     - lea r12,[crossfire.dat+10DE7D0] { [00000000] }    ==> 0x10DE7D0 Wall hack
//crossfire.WebView::CFWebView::operator=+167967 - 48 8B F1 - mov rsi, rcx
//crossfire.WebView::CFWebView::operator=+16796A - 48 89 4C 24 28 - mov[rsp + 28], rcx
//crossfire.WebView::CFWebView::operator=+16796F - B9 04000000 - mov ecx, 00000004 { 4 }
//crossfire.WebView::CFWebView::operator=+167974 - 41 8B F8 - mov edi, r8d
//crossfire.WebView::CFWebView::operator=+167977 - 48 8B DA - mov rbx, rdx
#define dwWallhack 0x10B93E0

//Wall hack Offset: 46 0FB6 84 20 ?? ?? ?? ?? 41 FF 91 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? BA ?? ?? ?? ?? 48 8B 08 48 63 85
//crossfire.exe + 22293B - 46 0FB6 84 20 A7000000 - movzx r8d, byte ptr[rax + r12 + 000000A7] <============ A7 Offset
//crossfire.exe + 222944 - 41 FF 91 C8010000 - call qword ptr[r9 + 000001C8]
//crossfire.exe + 22294B - 48 8B 05 3EE21900 - mov rax, [crossfire.exe + 3C0B90]{ (16C40D60) }
//crossfire.exe + 222952 - BA 0E000000 - mov edx, 0000000E{ 14 }
//crossfire.exe + 222957 - 48 8B 08 - mov rcx, [rax]
//crossfire.exe + 22295A - 48 63 85 54010000 - movsxd  rax, dword ptr[rbp + 00000154]
//crossfire.exe + 222961 - 4C 8B 09 - mov r9, [rcx]
//crossfire.exe + 222964 - 46 0FB6 84 20 A4000000 - movzx r8d, byte ptr[rax + r12 + 000000A4]
//crossfire.exe + 22296D - 41 FF 91 C8010000 - call qword ptr[r9 + 000001C8]
//crossfire.exe + 222974 - 48 8B 05 15E21900 - mov rax, [crossfire.exe + 3C0B90]{ (16C40D60) }
//crossfire.exe + 22297B - BA 16000000 - mov edx, 00000016 { 22 }
//crossfire.exe + 222980 - 48 8B 08 - mov rcx, [rax]
//crossfire.exe + 222983 - 48 63 85 58010000 - movsxd  rax, dword ptr[rbp + 00000158]

#define dwWallhackOffset 0xA7
//SeeGhost 14 = on. 5 = Off
#define dwSeeGhost		 0xB8

//Zombie Bug
//4C 8B 1D ?? ?? ?? ?? 4D 85 DB 0F 84 ?? ?? ?? ?? 66 3B 35
//crossfire.WebView::CFWebViewCallback::RequestResource + 3181 - 4C 8B 1D C0DC0601 - mov r11, [crossfire.dat + 10CE6F8] { [7FFAE85D6960] } => 0x10CE6F8 Address
// Offset 1: 48 8B 9E ?? ?? ?? ?? 83 F8 12
//cshell_x64.WebView::CFWebView::operator=+E178E2 - 48 8B 9E 50020000 - mov rbx, [rsi + 00000250]										=> 0x250 Offset 1
//cshell_x64.WebView::CFWebView::operator=+E178E9 - 83 F8 12 - cmp eax, 12 { 18 }
//cshell_x64.WebView::CFWebView::operator=+E178EC - 77 05 - ja cshell_x64.WebView::CFWebView::operator=+E178F3
//cshell_x64.WebView::CFWebView::operator=+E178EE - 0FA3 C7 - bt edi, eax
//cshell_x64.WebView::CFWebView::operator=+E178F1 - 72 0D - jb cshell_x64.WebView::CFWebView::operator=+E17900
//cshell_x64.WebView::CFWebView::operator=+E178F3 - 83 F8 65 - cmp eax, 65 { 101 }
//cshell_x64.WebView::CFWebView::operator=+E178F6 - 74 08 - je cshell_x64.WebView::CFWebView::operator=+E17900
//cshell_x64.WebView::CFWebView::operator=+E178F8 - 83 C0 8D - add eax, -73 { 141 }
//cshell_x64.WebView::CFWebView::operator=+E178FB - 83 F8 01 - cmp eax, 01 { 1 }
//cshell_x64.WebView::CFWebView::operator=+E178FE - 77 13 - ja cshell_x64.WebView::CFWebView::operator=+E17913
//cshell_x64.WebView::CFWebView::operator=+E17900 - 48 8B 8B 70660000 - mov rcx, [rbx + 00006670]										=> 0x6670 Offset 2
//cshell_x64.WebView::CFWebView::operator=+E17907 - 41 8B D6 - mov edx, r14d
//cshell_x64.WebView::CFWebView::operator=+E1790A - 48 8B 01 - mov rax, [rcx]
//cshell_x64.WebView::CFWebView::operator=+E1790D - FF 90 A8000000 - call qword ptr[rax + 000000A8]
//83 BC 81 ?? ?? ?? ?? 00 0F 85 ?? ?? ?? ?? C3 CC CC CC 
//cshell_x64.WebView::CFWebView::operator=+C24DBA - 83 BC 81 00020000 00 - cmp dword ptr[rcx + rax * 4 + 00000200], 00 { 0 }			==> 0x200 Offset 3
#define a__BaseBugZombie				0x10CE6F8		// 4C 8B 1D ?? ?? ?? ?? 4D 85 DB 0F 84 ?? ?? ?? ?? 66 3B 35


#define a__OffsetBugZombie1				0x250			// 48 8B 9E ?? ?? ?? ?? 83 F8 12	
#define a__OffsetBugZombie2				0x6670
#define a__OffsetBugZombie3				0x200			// 83 BC 81 ?? ?? ?? ?? 00 0F 85 ?? ?? ?? ?? C3 CC CC CC 45 33 C0 



//No recoil
//00 00 80 BF ?? ?? ?? ?? ?? 99 99 BF ?? ?? ?? ?? 80 8A ?? ?? ?? ?? ?? BF ?? ?? ?? ??
//cshell_x64.dll + 21BF80C - A4 - movsb
//cshell_x64.dll + 21BF80D - 70 7D - jo cshell_x64.dll + 21BF88C
//cshell_x64.dll + 21BF80F - BF 77BE7FBF - mov edi, BF7FBE77{ -1.00 }
//cshell_x64.dll + 21BF814 - 00 00 - add[rax], al				==> No Recoil												==> 0x21BF814
//cshell_x64.dll + 21BF816 - 80 BF B81E85BF 9A - cmp byte ptr[rdi - 407AE148], -66 { 154 }
//cshell_x64.dll + 21BF81D - 99 - cdq
//cshell_x64.dll + 21BF81E - 99 - cdq

#define dwNoRecoil 0x236B77C


//No Fall Damage
// 48 8b 05 ? ? ? ? f3 0f 11 48 ? 48 8d 15 ? ? ? ? 48 8b 4d ? 48 8b 0c f1 e8 ? ? ? ? 48 8b d8
//cshell_x64.WebView::CFWebArgument::operator=+5AC - 48 8D 15 F5D7C901 - lea rdx, [cshell_x64.WebView::CFWebView::operator=+1810668] { ["DamagePerMeter"] }
//cshell_x64.WebView::CFWebArgument::operator=+5B3 - 48 8B 4D E8 - mov rcx, [rbp - 18]
//cshell_x64.WebView::CFWebArgument::operator=+5B7 - 48 8B 0C F1 - mov rcx, [rcx + rsi * 8]
//cshell_x64.WebView::CFWebArgument::operator=+5BB - E8 80648501 - call cshell_x64.WebView::CFWebView::operator=+13C9300
//cshell_x64.WebView::CFWebArgument::operator=+5C0 - 48 85 C0 - test rax, rax
//cshell_x64.WebView::CFWebArgument::operator=+5C3 - 74 25 - je cshell_x64.WebView::CFWebArgument::operator=+5EA
//cshell_x64.WebView::CFWebArgument::operator=+5C5 - 48 8B 40 08 - mov rax, [rax + 08]
//cshell_x64.WebView::CFWebArgument::operator=+5C9 - 48 8B 48 08 - mov rcx, [rax + 08]
//cshell_x64.WebView::CFWebArgument::operator=+5CD - 48 8B 49 08 - mov rcx, [rcx + 08]
//cshell_x64.WebView::CFWebArgument::operator=+5D1 - FF 15 2126B901 - call qword ptr[cshell_x64.WebView::CFWebView::operator=+17054B8]{ ->ucrtbase.atof }
//cshell_x64.WebView::CFWebArgument::operator=+5D7 - 0F57 C9 - xorps xmm1, xmm1
//cshell_x64.WebView::CFWebArgument::operator=+5DA - F2 0F5A C8 - cvtsd2ss xmm1, xmm0
//cshell_x64.WebView::CFWebArgument::operator=+5DE - 48 8B 05 AB136302 - mov rax, [cshell_x64.dll + 276E9D0] { [00000000] }			==> Base: 0x276E9D0
//cshell_x64.WebView::CFWebArgument::operator=+5E5 - F3 0F11 48 04 - movss[rax + 04], xmm1											==> Offset: 0x4
#define dwNoFallDamage 0x27FA2D0
#define dwNoFallDamageOffset 0x4

//No Bug Damage
// 48 8d 15 ? ? ? ? 48 8d 4c 24 ? e8 ? ? ? ? 4c 8d 44 24 ? ba ? ? ? ? 48 8d 8f
//cshell_x64.WebView::CFWebView::operator=+138075B - 48 8D 15 06118700 - lea rdx, [cshell_x64.dll + 21BBFE8] { ["rez\Butes\%s_DZ.ltc"] } => 0x21BBFE8

#define dwNoBugDamage 0x2367FF0

//48 8d 0d ? ? ? ? e8 ? ? ? ? 48 83 7b ? ? 48 8d 53 ? 75 ? 41 b8 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? 33 c0 48 83 c4 ? 5b c3 cc cc cc cc cc 40 57 48 83 ec ? 48 c7 44 24 ? ? ? ? ? 48 89 5c 24 ? 48 8b d9 33 ff 48 39 79 ? 74 ? 48 8d 0d ? ? ? ? ff 15 ? ? ? ? 48 8b 4b ? 48 85 c9 74 ? 48 89 7c 24 ? 48 8b 01 ff 50 ? 48 89 7b ? 48 8d 0d ? ? ? ? ff 15 ? ? ? ? 48 89 7c 24 ? 48 8b 4b ? 48 85 c9 74 ? 48 8b 01 ff 50 ? 48 89 7b ? 48 83 7b ? ? 74 ? 48 8d 0d ? ? ? ? ff 15 ? ? ? ? 48 8b 4b ? 48 85 c9 74 ? 48 89 7c 24 ? 48 8b 01 ff 50 ? 48 89 7b ? 48 8d 0d ? ? ? ? ff 15 ? ? ? ? 48 89 7c 24 ? 48 8b 4b ? 48 85 c9 74 ? 48 8b 01 ff 50 ? 48 89 7b ? 33 c0 48 8b 5c 24 ? 48 83 c4 ? 5f c3 cc cc cc cc cc cc cc cc cc 40 57 48 83 ec ? 48 c7 44 24 ? ? ? ? ? 48 89 5c 24 ? 48 8b d9
//48 8d 0d ? ? ? ? e8 ? ? ? ? 33 c0 48 83 c4 ? 5b c3 cc cc cc cc cc 40 57 48 83 ec ? 48 c7 44 24 ? ? ? ? ? 48 89 5c 24 ? 48 8b d9 33 ff 48 39 79 ? 74 ? 48 8d 0d ? ? ? ? ff 15 ? ? ? ? 48 8b 4b ? 48 85 c9 74 ? 48 89 7c 24 ? 48 8b 01 ff 50 ? 48 89 7b ? 48 8d 0d ? ? ? ? ff 15 ? ? ? ? 48 89 7c 24 ? 48 8b 4b ? 48 85 c9 74 ? 48 8b 01 ff 50 ? 48 89 7b ? 48 83 7b ? ? 74 ? 48 8d 0d ? ? ? ? ff 15 ? ? ? ? 48 8b 4b ? 48 85 c9 74 ? 48 89 7c 24 ? 48 8b 01 ff 50 ? 48 89 7b ? 48 8d 0d ? ? ? ? ff 15 ? ? ? ? 48 89 7c 24 ? 48 8b 4b ? 48 85 c9 74 ? 48 8b 01 ff 50 ? 48 89 7b ? 33 c0 48 8b 5c 24 ? 48 83 c4 ? 5f c3 cc cc cc cc cc cc cc cc cc 40 57 48 83 ec ? 48 c7 44 24 ? ? ? ? ? 48 89 5c 24 ? 48 8b d9
//cshell_X64.WebView::CFWebView::operator=+B97F3A - 48 8D 0D 1762F600 - lea rcx, [cshell_X64.dll + 20C88D8] { ["ImpoUI/NoiseScreen/FlashBangEfffect.dtx"] }  => 0x20C88D8
//cshell_X64.WebView::CFWebView::operator=+B97F57 - 48 8D 0D CA61F600 - lea rcx, [cshell_X64.dll + 20C88A8] { ["ImpoUI/NoiseScreen/FLASHBANGVVIPEFFFECT.dtx"] } => 0x20C88A8
//cshell_X64.WebView::CFWebView::operator=+C63051 - 48 8D 0D 7070EA00 - lea rcx, [cshell_X64.dll + 20D4848] { ["ImpoUI/NoiseScreen/NoiseFilter.dtx"] }
//cshell_X64.WebView::CFWebView::operator=+C62FFA - 48 8D 0D 9F70EA00 - lea rcx, [cshell_X64.dll + 20D4820] { ["ImpoUI/NoiseScreen/NoiseScreen1.dtx"] }
//cshell_X64.WebView::CFWebView::operator=+C63017 - 48 8D 0D 5A70EA00 - lea rcx, [cshell_X64.dll + 20D47F8] { ["ImpoUI/NoiseScreen/NoiseScreen2.dtx"] }
//cshell_X64.WebView::CFWebView::operator=+C63034 - 48 8D 0D B570EA00 - lea rcx, [cshell_X64.dll + 20D4870] { ["ImpoUI/NoiseScreen/NoiseScreen3.dtx"] }
#define dwNoFlash1 0x213BA88
#define dwNoFlash2 0x213BA58

//PlayerMgr
// 48 8b 1d ? ? ? ? f3 0f 11 4c 19 ? 4b 8b 34 fe 48 8d 15 ? ? ? ? 48 8b ce e8 ? ? ? ? 48 85 c0 74 ? 48 8b 40 ? 48 8b 48 ? 48 8b 49 ? ff 15 ? ? ? ? 0f 57 c9 f2 0f 5a c8 48 63 3d ? ? ? ? 48 69 cf ? ? ? ? 48 8b 1d ? ? ? ? f3 0f 11 4c 19 ? 4b 8b 34 fe 48 8d 15 ? ? ? ? 48 8b ce e8 ? ? ? ? 48 85 c0 74 ? 48 8b 40 ? 48 8b 48 ? 48 8b 49 ? ff 15 ? ? ? ? 0f 57 c9 f2 0f 5a c8 48 63 3d ? ? ? ? 48 69 cf ? ? ? ? 48 8b 1d ? ? ? ? f3 0f 11 4c 19 ? 48 63 c7
//cshell_x64.WebView::CFWebView::GetData + 2DE - 48 8D 15 BB22CB01 - lea rdx, [cshell_x64.WebView::CFWebView::operator=+180EA80] { ["MovementWalkRate"] }
//cshell_x64.WebView::CFWebView::GetData + 2E5 - 48 8B CE - mov rcx, rsi
//cshell_x64.WebView::CFWebView::GetData + 2E8 - E8 33CB8601 - call cshell_x64.WebView::CFWebView::operator=+13C9300
//cshell_x64.WebView::CFWebView::GetData + 2ED - 48 85 C0 - test rax, rax
//cshell_x64.WebView::CFWebView::GetData + 2F0 - 74 38 - je cshell_x64.WebView::CFWebView::GetData + 32A
//cshell_x64.WebView::CFWebView::GetData + 2F2 - 48 8B 40 08 - mov rax, [rax + 08]
//cshell_x64.WebView::CFWebView::GetData + 2F6 - 48 8B 48 08 - mov rcx, [rax + 08]
//cshell_x64.WebView::CFWebView::GetData + 2FA - 48 8B 49 08 - mov rcx, [rcx + 08]
//cshell_x64.WebView::CFWebView::GetData + 2FE - FF 15 D48CBA01 - call qword ptr[cshell_x64.WebView::CFWebView::operator=+17054B8]{ ->ucrtbase.atof }
//cshell_x64.WebView::CFWebView::GetData + 304 - 0F57 C9 - xorps xmm1, xmm1
//cshell_x64.WebView::CFWebView::GetData + 307 - F2 0F5A C8 - cvtsd2ss xmm1, xmm0
//cshell_x64.WebView::CFWebView::GetData + 30B - 48 63 3D 3A636402 - movsxd  rdi, dword ptr[cshell_x64.dll + 276D2AC]{ [00000000] }
//cshell_x64.WebView::CFWebView::GetData + 312 - 48 69 CF A8000000 - imul rcx, rdi, 000000A8{ 168 }
//cshell_x64.WebView::CFWebView::GetData + 319 - 48 8B 1D 38636402 - mov rbx, [cshell_x64.dll + 276D2B8] { [00000000] } => PlrMgr: 0x276D2B8
//cshell_x64.WebView::CFWebView::GetData + 320 - F3 0F11 4C 19 08 - movss[rcx + rbx + 08], xmm1							=> 0x08 Offset to MovementWalkRate
//cshell_x64.WebView::CFWebView::GetData + 326 - 4B 8B 34 FE - mov rsi, [r14 + r15 * 8]

#define dwPlayerMgr 0x276D2B8

//cshell_x64.WebView::CFWebView::operator=+E2E820 - 48 83 EC 58 - sub rsp, 58 { 88 }
//cshell_x64.WebView::CFWebView::operator=+E2E824 - 80 3D 707A3701 00 - cmp byte ptr[cshell_x64.dll + 2770A1B], 00 { [0] }
//cshell_x64.WebView::CFWebView::operator=+E2E82B - 74 36 - je cshell_x64.WebView::CFWebView::operator=+E2E863
//cshell_x64.WebView::CFWebView::operator=+E2E82D - C6 05 677A3701 00 - mov byte ptr[cshell_x64.dll + 2770A1B], 00 { [0] }
//cshell_x64.WebView::CFWebView::operator=+E2E834 - 83 FA 5F - cmp edx, 5F{ 95 }
//cshell_x64.WebView::CFWebView::operator=+E2E837 - 75 2A - jne cshell_x64.WebView::CFWebView::operator=+E2E863
//cshell_x64.WebView::CFWebView::operator=+E2E839 - 33 C0 - skCrypt eax, eax
//cshell_x64.WebView::CFWebView::operator=+E2E83B - 4C 8D 0D 6EA4CF00 - lea r9, [cshell_x64.dll + 20F3430] { ["vertex lock failed"] }
//cshell_x64.WebView::CFWebView::operator=+E2E842 - 48 89 44 24 38 - mov[rsp + 38], rax
//cshell_x64.WebView::CFWebView::operator=+E2E847 - 45 33 C0 - skCrypt r8d, r8d
//cshell_x64.WebView::CFWebView::operator=+E2E84A - 48 89 44 24 30 - mov[rsp + 30], rax
//cshell_x64.WebView::CFWebView::operator=+E2E84F - 48 89 44 24 28 - mov[rsp + 28], rax
//cshell_x64.WebView::CFWebView::operator=+E2E854 - 8D 50 77 - lea edx, [rax + 77]
//cshell_x64.WebView::CFWebView::operator=+E2E857 - 89 44 24 20 - mov[rsp + 20], eax
//cshell_x64.WebView::CFWebView::operator=+E2E85B - 8D 48 38 - lea ecx, [rax + 38]
//cshell_x64.WebView::CFWebView::operator=+E2E85E - E8 FD8E78FF - call cshell_x64.WebView::CFWebView::operator=+5B7760
//cshell_x64.WebView::CFWebView::operator=+E2E863 - 48 83 C4 58 - add rsp, 58 { 88 }
//cshell_x64.WebView::CFWebView::operator=+E2E867 - C3 - ret

//73 ? 8b c8 48 8b 05 ? ? ? ? 48 89 14 c8 8b 0d ? ? ? ? 48 8b 05 ? ? ? ? 48 8b 0c c8
//unpacked_crossfire.dat + 3C93CF - 73 30 - jae unpacked_crossfire.dat + 3C9401
//unpacked_crossfire.dat + 3C93D1 - 8B C8 - mov ecx, eax
//unpacked_crossfire.dat + 3C93D3 - 48 8B 05 FEC7CD00 - mov rax, [unpacked_crossfire.dat + 10A5BD8] { (0) }
//unpacked_crossfire.dat + 3C93DA - 48 89 14 C8 - mov[rax + rcx * 8], rdx
//unpacked_crossfire.dat + 3C93DE - 8B 0D FCC7CD00 - mov ecx, [unpacked_crossfire.dat + 10A5BE0] { (0) }
//unpacked_crossfire.dat + 3C93E4 - 48 8B 05 EDC7CD00 - mov rax, [unpacked_crossfire.dat + 10A5BD8] { (0) }
//unpacked_crossfire.dat + 3C93EB - 48 8B 0C C8 - mov rcx, [rax + rcx * 8]
//unpacked_crossfire.dat + 3C93EF - 48 C7 81 30030000 00000000 - mov qword ptr[rcx + 00000330], 00000000 { 0 }
//unpacked_crossfire.dat + 3C93FA - FF 05 E0C7CD00 - inc[unpacked_crossfire.dat + 10A5BE0]{ (0) }
//unpacked_crossfire.dat + 3C9400 - C3 - ret
//unpacked_crossfire.dat + 3C9401 - 4C 8B 05 00C8CD00 - mov r8, [unpacked_crossfire.dat + 10A5C08] { (0) }
//unpacked_crossfire.dat + 3C9408 - 48 8D 15 F1D61900 - lea rdx, [unpacked_crossfire.dat + 566B00] { ("Set '%s' overflowed") }

#define dwNoSmoke 0x3C93CF

//Z coordinate base: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 02 00 00 00 00 80 ?? ?? ?? ??
//cshell_x64.dll+2318040 - 00 00                 - add [rax],al
//cshell_x64.dll + 29F23F0 - 00 00 - add[rax], al
//Z coordinate offset 1: 8b 82 ? ? ? ? 41 89 40 ? 8b 82 ? ? ? ? 41 89 40 ? 33 c0 48 83 c4 ? c3 83 3d ? ? ? ? ? 74 ? ff 15 ? ? ? ? 83 3d ? ? ? ? ? 7c ? 4c 8d 0d ? ? ? ? 4c 8d 05 ? ? ? ? 48 8d 15 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? b8 ? ? ? ? 48 83 c4 ? c3 cc cc cc cc cc cc cc cc cc cc cc 48 83 ec ? 48 85 d2 74 ? 4d 85 c0 74 ? 8b 82 ? ? ? ? 41 89 00 8b 82 ? ? ? ? 41 89 40 ? 8b 82 ? ? ? ? 41 89 40 ? 8b 82 ? ? ? ? 41 89 40 ? 33 c0 48 83 c4 ? c3 83 3d ? ? ? ? ? 74 ? ff 15 ? ? ? ? 83 3d ? ? ? ? ? 7c ? 4c 8d 0d ? ? ? ? 4c 8d 05 ? ? ? ? 48 8d 15 ? ? ? ? 48 8d 0d ? ? ? ? e8 ? ? ? ? b8 ? ? ? ? 48 83 c4 ? c3 cc f3 0f 10 02 ~~ f3 44 0f 10 8f ? ? ? ? 41 0f 28 fa
//Z coordinate offset 2: 48 8B 05 ?? ?? ?? ?? 4C 8B B6 ?? ?? ?? ??
//coordinate base : cshell_x64.dll + 0x295A9D0
//offset 1 : 548
//offset 2 : 1A0
//#define dwCoordinate_Base 0x29F23F0
//#define dwCoordinate_Offset1 0x548
//#define dwCoordinate_Offset2 0x1A0

//48 8b 05 ? ? ? ? 44 0f 29 5c 24
//cshell_x64.dll+128474A - 48 8B 05 473D7201     - mov rax,[cshell_x64.dll+29A8498] { (7FFCD91907F0) }  ==> 29A8498
#define dwCoordinate_Base 0x296C550

//48 8b b0 ? ? ? ? c6 41
//cshell_x64.dll+1284757 - 48 8B B0 90000000     - mov rsi,[rax+00000090] ==> 90
#define dwCoordinate_Offset1 0x90

//4c 8b b6 ? ? ? ? 48 8b 18
//cshell_x64.dll+1284796 - 4C 8B B6 18050000     - mov r14,[rsi+00000518] ==> 0x518
#define dwCoordinate_Offset2 0x548


//f3 44 0f 10 8f ? ? ? ? 41 0f 28 fa
//crossfire.exe+8CEB6 - F3 44 0F10 8F A0010000  - movss xmm9,[rdi+000001A0] ==> 1A0
#define dwCoordinate_Offset3 0x1A0

//Room bot 1 :cshell_x64.dll+2ECEE90
//Offset 3: 44 89 81 ?? ?? ?? ?? 89 91 ?? ?? ?? ?? 41 8B ?? (
//Offset 2: 48 8B 0C 03 48 8B 01 FF 90 ?? ?? ?? ?? 48 83 C3 ?? 48 81 FB ?? ?? ?? ?? 7E DA 48 8B 87 ?? ?? ?? ?? (rbx)
//Offset 1: 48 8B 43 ?? 48 8B 08 48 85 C9 0F84 ?? ?? ?? ??
//Room bot 2 :cshell_x64.dll+2ECEE90
//Offset 3: 44 89 81 ?? ?? ?? ?? 89 91 ?? ?? ?? ?? 41 8B ??
//Offset 2: 48 8B 0C 03 48 8B 01 FF 90 ?? ?? ?? ?? 48 83 C3 ?? 48 81 FB ?? ?? ?? ?? 7E DA 48 8B 87 ?? ?? ?? ?? (rbx)
//Offset 1: 48 8B 43 ?? 48 8B 08 48 85 C9 0F84 ?? ?? ?? ?? + 0x4

#define dwRoomBot 0x2ECEE90
#define dwRoomBot_Offset1 0x20
#define dwRoomBot_Offset2 0x158
#define dwRoomBot_Offset3 0xCC
#define dwRoomBot_Offset4 dwRoomBot_Offset3 + 0x4

//Bunnyhop: 88 05 ?? ?? ?? ?? BE ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ??
//cshell_x64.dll+13D87BC - 88 05 52EB5F01        - mov [cshell_x64.dll+29D7314],al { (0) }      ==> 0x29D7314

#define dwBunnyhop 0x29D7314

//Skill E Base: 00 ?? ?? ?? ?? ?? ?? 00 ?? 00 00 00 00 00 00 00 02 00 00 00 00 80 BB ?? ?? ?? ?? ??
//Skill E Offset: 8B 81 ?? ?? ?? ?? 83 F8 ?? 75 03 B0 ?? C3
//Base: cshell_x64.dll+295A9D0 - 00 00                 - add [rax],al     ==> 0x295A9D0
//Offset: cshell_x64.dll+993F89 - 8B 81 DC080000        - mov eax,[rcx+000008DC]      ==> 0x8DC

#define dwSkillE_Base 0x29F23F0
#define dwSkillE_Offset 0x8DC

//Third Person Base: ?? ?? ?? ?? 00 00 00 00 01 01 00 00 ?? 00 00 00 00 00 00 00 00 00 20 43 00 00
//Third Person Offset: 8B 83 ?? ?? ?? ?? 48 83 C4 ?? 5B C3 CC CC CC CC CC CC CC CC CC CC 48 89 5C 24 ??
//Base: cshell_x64.dll+295A980 - 10 12                 - adc [rdx],dl      ==> 0x295A980
//Offset: cshell_x64.dll+1014CEA - 8B 83 98000000        - mov eax,[rbx+00000098]     ==> 0x98

#define dwThirdPerson_Base 0x2B632C0
#define dwThirdPerson_Offset 0x98

//Copy room ID
//48 8B ?? ?? ?? ?? ?? 48 ?? ?? 75 1F B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 89 44 24 ?? 48 8B C8 E8 ?? ?? ?? ?? 90 48 89 ?? ?? ?? ?? ?? 48 83 C4 ?? C3 CC CC CC 41 56
//Base: cshell_x64.dll+18956D - 48 8B 05 EC438402     - mov rax,[cshell_x64.dll+29CD960] { (0) } ==> 0x29CD960
// 48 8B 40 ?? 8B 88 ?? ?? ?? ?? 83 F9 ??
//Offset 1: cshell_x64.dll+955531 - 48 8B 40 10           - mov rax,[rax+10]					 ==> 0x10
// 48 89 8B ?? ?? ?? ?? 4C 8D BB ?? ?? ?? ?? 41 8B 3F
//Offset 2: cshell_x64.dll+16A026D - 48 89 8B 38010000     - mov [rbx+00000138],rcx				 ==> 0x138

#define dwCopyRoomBase 0x2A65410
#define dwCopyRoom_Offset1 0x10
#define dwCopyRoom_Offset2 0x138

//Bypass Send Screen
//0F85 ?? ?? ?? ?? 48 8B 4C 24 ?? 48 8B 01 FF 50 ?? 48 8B 4C 24 ?? BA ?? ?? ?? ?? 48 8B 01 FF 90 ?? ?? ?? ?? 48 8B 5C 24 ?? 48 8D 55 ?? 
// Ma Goc
//cshell_x64.dll+8F2B31 - 0F85 9D000000         - jne cshell_x64.dll+8F2BD4  ==> 0x8F2B31
// Ma jmp
//cshell_x64.dll + 8F2B31 - E9 9E000000 - jmp cshell_x64.dll + 8F2BD4
//cshell_x64.dll + 8F2B36 - 90 - nop

#define dwBPSendScreen 0x946801

//BypassDetectCheatTool[0]
//75 ? 48 8b 4c 24 ? 48 8b 01 ff 50 ? 48 8b 4c 24 ? 48 8b 01 ba ? ? ? ? ff 90 ? ? ? ? 48 8b 54 24 ? 48 8d 8d ? ? ? ? e8 ? ? ? ? 48 8b 05 ? ? ? ? 48 8b 18 48 8b 4c 24 ? 48 8b 01 ff 50 ? 48 8b d0 41 b8 ? ? ? ? 48 8b 0d ? ? ? ? ff 93 ? ? ? ? 48 8b 4c 24 ? 48 8b 01 ff 50 ? 41 b6
//cshell_x64.dll+8EE55A - 75 65                 - jne cshell_x64.dll+8EE5C1			==> 0x8EE55A
//BypassDetectCheatTool[1]
//75 ? 48 8b 4c 24 ? 48 8b 01 ff 50 ? 48 8b 4c 24 ? 48 8b 01 ba ? ? ? ? ff 90 ? ? ? ? 48 8b 54 24 ? 48 8d 4d ? e8 ? ? ? ? 48 8b 05 ? ? ? ? 48 8b 18 48 8b 4c 24 ? 48 8b 01 ff 50 ? 48 8b d0 41 b8 ? ? ? ? 48 8b 0d ? ? ? ? ff 93 ? ? ? ? 48 8b 4c 24 ? 48 8b 01 ff 50 ? 40 b6
//cshell_x64.dll+8EEC3A - 75 62                 - jne cshell_x64.dll+8EEC9E			==> 0x8EEC3A

#define dwBPToolCheatDetect0 0x94222A
#define dwBPToolCheatDetect1 0x94290A


//20 4d ? 40 01 00 00 00 00 8d
//crossfire.exe+10C77E8 - 20 4D 61              - and [rbp+61],cl
//m_ILTClientDefault crossfire.exe+10C77E8


//No Reload + No Change
//Base: 48 8b 05 ? ? ? ? ba ? ? ? ? 48 8b 88 ? ? ? ? 48 8b 01 ff 90 ? ? ? ? 40 0f b6 d7
//Offset 1: 48 8b 05 ? ? ? ? 48 8b 88 ? ? ? ? 48 85 c9 74 ? 48 8b 01 ff 90 ? ? ? ? 83 3d
//Offset 2: 41 8b 96 ? ? ? ? 49 8b ce ff 90 ? ? ? ? 84 c0 0f 84 ? ? ? ? 49 8b 06
//cshell_x64.dll+9A26EB - 41 8B 96 88080000     - mov edx,[r14+00000888]  ==> 0x888

#define dwNR_NCBase 0x296C550
#define dwNR_NCOffset1 0x90
#define dwNR_NCOffset2 0x888

//State Check
//Base: 48 8b 05 ? ? ? ? 48 8b e9 4c 8b b8
//cshell_x64.dll + 109095B - 48 8B 05 862B6D01 - mov rax, [cshell_x64.dll + 27634E8] { (7FF951A6A940) } ==> 0x27634E8
//Offset : 40 88 b7 ? ? ? ? 40 88 b7 ? ? ? ? 48 8b 87
//cshell_x64.dll + 14A4D0E - BA 10000000 - mov edx, 00000010 { 16 }
//cshell_x64.dll + 14A4D13 - FF 50 48 - call qword ptr[rax + 48]
//cshell_x64.dll + 14A4D16 - 40 88 B7 70020000 - mov[rdi + 00000270], sil ==> 0x270
//cshell_x64.dll + 14A4D1D - 40 88 B7 704A0400 - mov[rdi + 00044A70], sil
//cshell_x64.dll + 14A4D24 - 48 8B 87 50020000 - mov rax, [rdi + 00000250]
//cshell_x64.dll + 14A4D2B - 48 8B 88 90760000 - mov rcx, [rax + 00007690]
//cshell_x64.dll + 14A4D32 - 48 8B 01 - mov rax, [rcx]
//cshell_x64.dll + 14A4D35 - 33 D2 - skCrypt edx, edx

#define dwStateBase 0x27634E8
#define dwStateOffset 0x270


//31_9
//BA ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 8B 54 24 ? 48 8D 4C 24 ? E8 ? ? ? ? 48 8B 05 ? ? ? ? 48 8B 4C 24 ? 48 8B 18 48 8B 01 FF 50 38 48 8B 0D ? ? ? ? 41 B8 ? ? ? ? 48 8B D0 FF 93 ? ? ? ? 48 8B 4C 24 ? 48 8B 01 FF 50 10 48 8B 8D ? ? ? ? 48
//cshell_x64.dll + 8ECAA7 - 75 76 - jne cshell_x64.dll + 8ECB1F
//cshell_x64.dll + 8ECD36 - 75 76 - jne cshell_x64.dll + 8ECDAE
//cshell_x64.dll + 8ECF50 - 75 76 - jne cshell_x64.dll + 8ECFC8
//cshell_x64.dll + 8ED1F3 - 75 63 - jne cshell_x64.dll + 8ED258
//cshell_x64.dll + 8F2768 - 75 63 - jne cshell_x64.dll + 8F27CD

#define BP31_9_1 0x940777
#define BP31_9_2 0x940A60
#define BP31_9_3 0x940C20
#define BP31_9_4 0x940EC3
#define BP31_9_5 0x946438

//31_10
//75 ? 45 0f b6 c7
// cshell_x64.dll+13C86C0 - 75 0E                 - jne cshell_x64.dll+13C86D0
//41 B8 BA 07 00 00 + B8, + C0
// 75 ? 48 8b 4d ? 48 8b 01 ff 50 ? 48 8b 4d ? 48 8b 01 ba ? ? ? ? ff 90 ? ? ? ? 48 8b 55
//cshell_x64.dll+13C2440 - 75 5D                 - jne cshell_x64.dll+13C249F
//75 ? 48 8b 4c 24 ? 48 8b 01 ff 50 ? 48 8b 4c 24 ? 48 8b 01 ba ? ? ? ? ff 90 ? ? ? ? 48 8b 54 24 ? 48 8d 8d ? ? ? ? e8 ? ? ? ? 48 8b 05 ? ? ? ? 48 8b 18 48 8b 4c 24 ? 48 8b 01 ff 50 ? 48 8b d0 45 8b c5
//cshell_x64.dll+13C25C5 - 75 62                 - jne cshell_x64.dll+13C2629

#define BP31_10_1 0x142CDB0
#define BP31_10_2 0x142694F
#define BP31_10_3 0x1426ACA

//48 8b 05 ? ? ? ? 48 8b 98 ? ? ? ? 48 85 db 0f 84 ? ? ? ? 48 8b 03 48 8b cb ff 90 ? ? ? ? 84 c0 0f 84 ? ? ? ? 48 8b 03 48 8b cb ff 90 ? ? ? ? 8b 4e
//WTW Base: cshell_x64.dll+11C0C31 - 48 8B 05 B0285A01     - mov rax,[cshell_x64.dll+27634E8] { (7FFEC788A940) } ==> 0x27634E8
//41 F6 80 ?? ?? ?? ?? ?? 75 49 F3 41 0F10 88 ?? ?? ?? ??
//WTW Offset 1: unpacked_crossfire.dat+36ED2D - 41 F6 80 20010000 40  - test byte ptr [r8+00000120],40 { 64 } ==> 0x120
//48 8b 89 ? ? ? ? e8 ? ? ? ? 84 c0 0f 84
//WTW Offset 2: cshell_x64.dll+11A9930 - 48 8B 89 48050000     - mov rcx,[rcx+00000548]  ==> 0x548
//48 8b 8b ? ? ? ? 48 85 c9 74 ? 48 8b 01 48 83 c4 ? 5b 48 ff a0 ? ? ? ? 32 c0
//WTW Offset 3: cshell_x64.dll+13ED4AE - 48 8B 8B 90000000     - mov rcx,[rbx+00000090]	 ==> 0x90

#define dwWTWBase 0x27634E8
#define dwWTWBase_Offset1 0x120
#define dwWTWBase_Offset2 0x548
#define dwWTWBase_Offset3 0x90

//ButeType
//48 8b 35 ? ? ? ? 89 84 32 ? ? ? ? 49 8b 1f eb
//cshell_x64.WebView::CFWebView::GetData+1D6A - 48 8B 35 1F056D02     - mov rsi,[cshell_x64.dll+27F93D0] { (0) } ==> 0x27F93D0


//Walllist
//48 8d 15 ? ? ? ? e8 ? ? ? ? 48 85 c0 0f 84 ? ? ? ? 0f 57 c0 f3 0f 7f 44 24 ? 45 33 ff

//CharacterFunc
//48 8b 15 ? ? ? ? 48 23 0d
//cshell_x64.WebView::CFWebView::GetData+3FF9 - 48 8B 15 A86C9302     - mov rdx,[cshell_x64.dll+2A61DE8] { (260265BAA60) }  ==> 0x2A61DE8
//cshell_x64.WebView::CFWebView::GetData+4000 - 48 23 0D B96C9302     - and rcx,[cshell_x64.dll+2A61E00] { (7) }
//cshell_x64.WebView::CFWebView::GetData+4317 - 48 8B 15 CA699302     - mov rdx,[cshell_x64.dll+2A61E28] { (260265BACA0) }
//cshell_x64.WebView::CFWebView::GetData + 431E - 48 23 0D DB699302 - and rcx, [cshell_x64.dll + 2A61E40] { (7) }

#define dwCharacterFunc 0x2BD2CC8

//CFVN
//  //48 8b 05 ? ? ? ? ba ? ? ? ? 48 8b 88 ? ? ? ? 48 8b 01 ff 90 ? ? ? ? 40 0f b6 d7
//  /*
//	cshell_x64.dll+795D30 - 48 8B 05 61272102     - mov rax,[cshell_x64.dll+29A8498] { (7FF8064F07F0) }  ==> 0x29A8498
//	cshell_x64.dll+795D37 - BA 01000000           - mov edx,00000001 { 1 }
//	cshell_x64.dll+795D3C - 48 8B 88 90000000     - mov rcx,[rax+00000090]
//	cshell_x64.dll+795D43 - 48 8B 01              - mov rax,[rcx]
//	cshell_x64.dll+795D46 - FF 90 700E0000        - call qword ptr [rax+00000E70]
//	cshell_x64.dll+795D4C - 40 0FB6 D7            - movzx edx,dil
//	cshell_x64.dll+795D50 - 48 8B CB              - mov rcx,rbx
//	cshell_x64.dll+795D53 - E8 88FEFFFF           - call cshell_x64.dll+795BE0
//	cshell_x64.dll+795D58 - 48 8B 5C 24 50        - mov rbx,[rsp+50]
//	cshell_x64.dll+795D5D - 48 83 C4 40           - add rsp,40 { 64 }
//	cshell_x64.dll+795D61 - 5F                    - pop rdi
//
//  */
#define LTShell						0x296C550

//  //48 69 c8 ? ? ? ? 0f 29 b4 24 ? ? ? ? 48 8b 05
//
//  /*
//	cshell_x64.dll+14A839D - 48 69 C8 C80D0000     - imul rcx,rax,00000DC8 { 3528 }
//	cshell_x64.dll+14A83A4 - 0F29 B4 24 F0000000   - movaps [rsp+000000F0],xmm6
//	cshell_x64.dll+14A83AC - 48 8B 05 A5767401     - mov rax,[cshell_x64.dll+2BEFA58] { (12C12200) }
//	cshell_x64.dll+14A83B3 - 4C 8B BC 39 80020000  - mov r15,[rcx+rdi+00000280]    ==> 0x00000280
//	cshell_x64.dll+14A83BB - 48 85 C0              - test rax,rax
//
//  */
//  const DWORD ENT_BEGIN = 0x280;

#define ENT_BEGIN					0x288

//  //4d 85 c0 74 ? 41 8b 80 ? ? ? ? 89 02
//  /*
//	cshell_x64.dll+2E439C - 48 69 C8 C80D0000     - imul rcx,rax,00000DC8 { 3528 }    ==> 0x00000DC8
//	cshell_x64.dll+2E43A3 - 48 8B 05 EE406C02     - mov rax,[cshell_x64.dll+29A8498] { (7FF8064F07F0) }
//	cshell_x64.dll+2E43AA - 4C 8B 84 01 98020000  - mov r8,[rcx+rax+00000298]
//	cshell_x64.dll+2E43B2 - 4D 85 C0              - test r8,r8
//
//  */
//  const DWORD ENT_SIZE = 0xDC8;

#define ENT_SIZE					0x2080

//48 8b 05 ? ? ? ? 48 8b 88 ? ? ? ? 48 85 c9 74 ? 48 8b 01 ff 90 ? ? ? ? 83 3d
/*
	cshell_x64.dll+14B807E - 48 8B 05 13044F01     - mov rax,[cshell_x64.dll+29A8498] { (7FF8F19A07F0) }
	shell_x64.dll+14B8085 - 48 8B 88 90000000     - mov rcx,[rax+00000090]
	cshell_x64.dll+14B808C - 48 85 C9              - test rcx,rcx
*/

#define LOCAL_ENT					0x90

//4c 8b 35 ? ? ? ? 48 89 4d
//cshell_x64.dll + 12DC132 - 4C 8B 35 5FC36C01 - mov r14, [cshell_x64.dll + 29A8498] { (7FF8F19A07F0) }
//cshell_x64.dll + 12DC139 - 48 89 4D A0 - mov[rbp - 60], rcx
//cshell_x64.dll + 12DC13D - 41 0FB6 86 79020000 - movzx eax, byte ptr[r14 + 00000279]  ==> 0x279
//cshell_x64.dll + 12DC145 - 4C 69 F8 C80D0000 - imul r15, rax, 00000DC8{ 3528 }

#define LOCAL_ENT_INDEX				0x281


////CFO
////8b 15 ? ? ? ? ff 90 ? ? ? ? f6 05
////cshell_x64.dll+BDB053 - 8B 15 ABCEDC01        - mov edx,[cshell_x64.dll+29A7F04] { (0) }
//#define dwGameStatus				0x29A7F04
//
////00 00 80 BF ?? ?? ?? ?? ?? 99 99 BF ?? ?? ?? ?? 80 8A ?? ?? ?? ?? ?? BF ?? ?? ?? ??
////cshell_x64.dll + 23A1504 - 00 00 - add[rax], al
//#define dwRecoil					0x23A1504 
//
//
////  //48 8b 05 ? ? ? ? ba ? ? ? ? 48 8b 88 ? ? ? ? 48 8b 01 ff 90 ? ? ? ? 40 0f b6 d7
////  /*
////	cshell_x64.dll+795D30 - 48 8B 05 61272102     - mov rax,[cshell_x64.dll+29A8498] { (7FF8064F07F0) }  ==> 0x29A8498
////	cshell_x64.dll+795D37 - BA 01000000           - mov edx,00000001 { 1 }
////	cshell_x64.dll+795D3C - 48 8B 88 90000000     - mov rcx,[rax+00000090]
////	cshell_x64.dll+795D43 - 48 8B 01              - mov rax,[rcx]
////	cshell_x64.dll+795D46 - FF 90 700E0000        - call qword ptr [rax+00000E70]
////	cshell_x64.dll+795D4C - 40 0FB6 D7            - movzx edx,dil
////	cshell_x64.dll+795D50 - 48 8B CB              - mov rcx,rbx
////	cshell_x64.dll+795D53 - E8 88FEFFFF           - call cshell_x64.dll+795BE0
////	cshell_x64.dll+795D58 - 48 8B 5C 24 50        - mov rbx,[rsp+50]
////	cshell_x64.dll+795D5D - 48 83 C4 40           - add rsp,40 { 64 }
////	cshell_x64.dll+795D61 - 5F                    - pop rdi
////
////  */
//#define LTShell						0x29A8498
//
////  //48 69 c8 ? ? ? ? 0f 29 b4 24 ? ? ? ? 48 8b 05
////
////  /*
////	cshell_x64.dll+14A839D - 48 69 C8 C80D0000     - imul rcx,rax,00000DC8 { 3528 }
////	cshell_x64.dll+14A83A4 - 0F29 B4 24 F0000000   - movaps [rsp+000000F0],xmm6
////	cshell_x64.dll+14A83AC - 48 8B 05 A5767401     - mov rax,[cshell_x64.dll+2BEFA58] { (12C12200) }
////	cshell_x64.dll+14A83B3 - 4C 8B BC 39 80020000  - mov r15,[rcx+rdi+00000280]    ==> 0x00000280
////	cshell_x64.dll+14A83BB - 48 85 C0              - test rax,rax
////
////  */
////  const DWORD ENT_BEGIN = 0x280;
//
//#define ENT_BEGIN					0x280
//
////  //4d 85 c0 74 ? 41 8b 80 ? ? ? ? 89 02
////  /*
////	cshell_x64.dll+2E439C - 48 69 C8 C80D0000     - imul rcx,rax,00000DC8 { 3528 }    ==> 0x00000DC8
////	cshell_x64.dll+2E43A3 - 48 8B 05 EE406C02     - mov rax,[cshell_x64.dll+29A8498] { (7FF8064F07F0) }
////	cshell_x64.dll+2E43AA - 4C 8B 84 01 98020000  - mov r8,[rcx+rax+00000298]
////	cshell_x64.dll+2E43B2 - 4D 85 C0              - test r8,r8
////
////  */
////  const DWORD ENT_SIZE = 0xDC8;
//
//#define ENT_SIZE					0xDC8
//
//#define LOCAL_ENT					0x90
//
//#define LOCAL_ENT_INDEX				0x279


//class CPlayer
//{
//public:
//		
//		char ClientID;			0x8
//		char Team;				0x9
//		char _Name[12];			0xA
//		char pad_0x25[0x25]		0x25
//		int PlayerSlotTeam;		0x2F
//		int unkstruct1;			0x31
//		bool Has_C4;			0x33
//		int State;				0x34
//		int Rank;				0x38
//		int unkstruct2;			0x3C
//		int unkstruct3;			0x40
//		short HP;			0x44
//		short Kills;			0x48
//		__int32 Deaths;			0x4C
//		__int32 HeadShots;		0x4E
//		__int32 TeamID;			0x52
//		__int32 Ping;			0x56
//};

//48 8b 05 ? ? ? ? 48 85 c0 75 ? b9 ? ? ? ? e8 ? ? ? ? 45 33 c0
//unpacked_crossfire.WebView::CFWebView::GetData + 1A4 - 48 8B 05 3539F400 - mov rax, [unpacked_crossfire.dat + 10B2AE0] { (0) }  ==> 10B2AE0
//unpacked_crossfire.WebView::CFWebView::GetData + 1AB - 48 85 C0 - test rax, rax
//unpacked_crossfire.WebView::CFWebView::GetData + 1AE - 75 76 - jne unpacked_crossfire.WebView::CFWebView::GetData + 226

#define dwCFTTable 0x10B2AE0

//00 00 7a ? 00 00 7f
//cshell_x64.dll + 236B4EC - 00 00 - add[rax], al  ==> 236B4EC
//cshell_x64.dll + 236B4EE - 7A 44 - jp cshell_x64.dll + 236B534
//cshell_x64.dll + 236B4F0 - 00 00 - add[rax], al

#define dwFastGun 0x236B4EC

//48 8B 3D ?? ?? ?? ?? F3 0F 11 4C 3E 38 F3 0F 11 4C 3E 3C F3 0F 11 4C 3E 40
//CShell_x64.dll + 187586 - 48 8B 3D EB287E02 - mov rdi, [CShell_x64.dll + 2969E78] { (0) }
//CShell_x64.dll + 18758D - F3 0F11 4C 3E 38 - movss[rsi + rdi + 38], xmm1
//CShell_x64.dll + 187593 - F3 0F11 4C 3E 3C - movss[rsi + rdi + 3C], xmm1
//CShell_x64.dll + 187599 - F3 0F11 4C 3E 40 - movss[rsi + rdi + 40], xmm1
#define dwModelBute 0x2969E78

struct Netvars {
	DWORD64 InGameStatus;
	DWORD64 NoBugDamage;
	DWORD64 NoRecoil;
	DWORD64 ThirdPerson_Base;
	DWORD64 ThirdPerson_Offset;
	DWORD64 Coordinate_Base;
	DWORD64 Coordinate_Offset1;
	DWORD64 Coordinate_Offset2;
	DWORD64 Coordinate_Offset3;
	DWORD64 CharacterFunc;
	DWORD64 CFTTable;
	DWORD64 dwLTShell;
	DWORD64 dwENT_BEGIN;
	DWORD64 dwENT_SIZE;
	DWORD64 dwLOCAL_ENT_INDEX;
	DWORD64 FastGun;
	DWORD64 CopyRoomBase;
	DWORD64 CopyRoomOffset1;
	DWORD64 CopyRoomOffset2;
	DWORD64 SkillE_Base;
	DWORD64 SkillE_Offset;
	DWORD64 ModelBute;
	DWORD64 NR_NCBase;
	DWORD64 NR_NCOffset1;
	DWORD64 NR_NCOffset2;
	DWORD64 dwSpamRadio;
}nv;

//WeaponMgr
//48 8b 15 ? ? ? ? ff c3 49 8b c0 48 2b c2 8b cb 48 c1 f8 ? 48 3b c8 72 ? 0f b6 0d
