#pragma once

#include "stdafx.h"
#include <windows.h>
#include <cmath>
#include <C:\Program Files (x86)\Microsoft SDKs\Windows\v5.0\Include\psapi.h>
#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.1A\\Lib\\psapi.lib")
//#define WH_KEYBOARD_LL	13

class Pal5Q{
private:
	HWND hWnd;
	HWND hWnd2;
	HANDLE hProcess;
	DWORD processId;
	HANDLE hThread;
	bool thread;
	bool found;
	
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam){
		Pal5Q* pThis = (Pal5Q*)lParam;
		DWORD pid;
		GetWindowThreadProcessId(hwnd, &pid);
		
		if (pid == pThis->processId){
			char title[255];  
			GetWindowText(hwnd, title, 254);
			if(!strcmp(title, "Pal5Q")){
				pThis->hWnd2 = hwnd;
				return FALSE;
			}
		}
		return TRUE;
	}
	
	static DWORD WINAPI ScanProc (LPVOID lpParam){
		Pal5Q* p = (Pal5Q*)lpParam;
		while(p->thread){
			Sleep(100);
			if(p->FindPal5Q(false)){
				p->found = true;
			}else{
				p->found = false;
			}
		}
		return 0;
	}

	bool FindPal5Q(bool tips = true){
		DWORD pid[1024],cbNeeded;
		char pname[260];
		memset(pid, 0, sizeof(pid));
		memset(pname, 0, sizeof(pname));
		if (!EnumProcesses( pid, sizeof(pid), &cbNeeded)){
			if(tips)
				MessageBox(NULL, "枚举进程失败!", 0, MB_OK);
			found = false;
			return false;
		}
		int num = cbNeeded/sizeof(DWORD);
		for (int i = 0; i < num; i++) {
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, pid[i]);
			if (hProcess){
				HMODULE hMod;
				DWORD cbNeeded;
				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
					GetModuleBaseName(hProcess, hMod, pname,sizeof(pname));
				if (0 != strcmp(pname,"Pal5Q.exe")) {
					CloseHandle(hProcess);
				}else{
					this->hProcess=hProcess;
					this->processId=pid[i];
					
					hWnd2 = 0;
					EnumWindows((WNDENUMPROC)EnumWindowsProc, (DWORD)this);
					if(hWnd2 == 0){
						found = false;
						hWnd = 0;
						return false;
					}
					hWnd = hWnd2;
					found = true;
					return true;
				}
			}
		}
		if(tips)
			MessageBox(NULL, "没有找到游戏进程", 0, MB_OK);
		found = false;
		return false;
	}
public:
	Pal5Q():hWnd(0), hWnd2(0), hProcess(INVALID_HANDLE_VALUE), processId(0), 
		thread(false), hThread(INVALID_HANDLE_VALUE), found(false){}
	
	~Pal5Q(){stop();CloseHandle(hProcess);}
	
	bool privilegeUp(){
		TOKEN_PRIVILEGES Privileges;
		HANDLE hToken;
		if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken)){
			return false;
		}
		LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&Privileges.Privileges[0].Luid);
		Privileges.PrivilegeCount = 1;
		Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &Privileges, 0, (PTOKEN_PRIVILEGES)NULL, 0);
		return true;
	}
	
	bool Found(){
		return found;
	}
	
	void startScan(){
		if(hThread != INVALID_HANDLE_VALUE){
			return;
		}
		
		thread = true;
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ScanProc, this, 0, NULL);
	}
	
	void stop(){
		if(hThread != INVALID_HANDLE_VALUE){
			thread = false;
			WaitForSingleObject(hThread, 500);
			TerminateThread(hThread, 0);
			CloseHandle(hThread);
			hThread = INVALID_HANDLE_VALUE;
		}
	}
	
	DWORD GetPid(){return processId;}
	
	HWND GetHWND(){return hWnd;}
	
	HANDLE GetProcessHandle(){return hProcess;}
};

extern Pal5Q pal5q;

class AutoClick{
	HANDLE hThread;
	bool thread;
	
	static DWORD WINAPI ScanProc (LPVOID lpParam){
		AutoClick* p = (AutoClick*)lpParam;
		
		DWORD Addr=0x00AA9818;
		DWORD Addr2_base=0x00aa986c;
		DWORD offset = 0x1a9;
		DWORD Addr2;//0x16d9aef9;
		
		char fuck = 0;
		DWORD dwWriteBytes = 0;
		int key[5];
		
		ReadProcessMemory (pal5q.GetProcessHandle(), (LPVOID)Addr2_base, &Addr2, sizeof(Addr2), &dwWriteBytes);
		Addr2 += offset;
		
		while(p->thread){
			Sleep(50);
			ReadProcessMemory (pal5q.GetProcessHandle(), (LPVOID)Addr2, &fuck, sizeof(fuck), &dwWriteBytes);
			if(fuck == 0){
				continue;
			}
			ReadProcessMemory (pal5q.GetProcessHandle(), (LPVOID)Addr, key, sizeof(key), &dwWriteBytes);
			for(int i = 0; i < 5; i++){
				switch(key[i]){
				case 1:
					PostMessage(pal5q.GetHWND(), WM_LBUTTONDOWN, 1, 1);
					Sleep(10);
					PostMessage(pal5q.GetHWND(), WM_LBUTTONUP, 1, 1);
					break;
				case 2:
					PostMessage(pal5q.GetHWND(), WM_RBUTTONDOWN, 1, 1);
					Sleep(10);
					PostMessage(pal5q.GetHWND(), WM_RBUTTONUP, 1, 1);
					break;
				default:
					i = 10;
					break;
				}
			}
			
		}
		return 0;
	}
	
public:
	AutoClick():thread(false), hThread(INVALID_HANDLE_VALUE){}
	
	~AutoClick(){stop();}
	
	bool start(){
		stop();
		if(!pal5q.Found()){
			MessageBox(NULL, "游戏未开启", 0, MB_OK);
			return false;
		}
		thread = true;
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ScanProc, this, 0, NULL);
		return true;
	}
	
	void stop(){
		if(hThread != INVALID_HANDLE_VALUE){
			thread = false;
			WaitForSingleObject(hThread, 500);
			TerminateThread(hThread, 0);
			CloseHandle(hThread);
			hThread = INVALID_HANDLE_VALUE;
		}
	}
};

class FreeCamera{
private:
	
	
	BYTE fakeKbdHookProc[10];
	HHOOK hHook;
	
	DWORD ori_1, ori_2, ori_3;
	DWORD camTargetAddr;
	DWORD camPosAddr;
	DWORD camPos_1, camPos_2, camPos_3;
	DWORD camTar_1, camTar_2, camTar_3;
	
	
private:
	void WriteCamTarget(){
		WriteProcessMemory (pal5q.GetProcessHandle(), (LPVOID)(camTargetAddr), &camTar_1, sizeof(camTar_1), NULL);
		WriteProcessMemory (pal5q.GetProcessHandle(), (LPVOID)(camTargetAddr + 4), &camTar_2, sizeof(camTar_2), NULL);
		WriteProcessMemory (pal5q.GetProcessHandle(), (LPVOID)(camTargetAddr + 8), &camTar_3, sizeof(camTar_3), NULL);
	}
	
	LRESULT KbdHookProc(int code, WPARAM wParam, LPARAM lParam){
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
		HWND hTmp = GetForegroundWindow();
		
		if(hTmp != pal5q.GetHWND())
			return CallNextHookEx (hHook, code, wParam, lParam);
		
		if(wParam == WM_KEYDOWN){		
			ReadProcessMemory (pal5q.GetProcessHandle(), (LPVOID)camPosAddr, &camPos_1, sizeof(DWORD), NULL);
			ReadProcessMemory (pal5q.GetProcessHandle(), (LPVOID)(camPosAddr + 4), &camPos_2, sizeof(DWORD), NULL);
			ReadProcessMemory (pal5q.GetProcessHandle(), (LPVOID)(camPosAddr + 8), &camPos_3, sizeof(DWORD), NULL);
			
			ReadProcessMemory (pal5q.GetProcessHandle(), (LPVOID)camTargetAddr, &camTar_1, sizeof(DWORD), NULL);
			ReadProcessMemory (pal5q.GetProcessHandle(), (LPVOID)(camTargetAddr + 4), &camTar_2, sizeof(DWORD), NULL);
			ReadProcessMemory (pal5q.GetProcessHandle(), (LPVOID)(camTargetAddr + 8), &camTar_3, sizeof(DWORD), NULL);
			
			int towardsVector_1 = camPos_1 - camTar_1;
			//DWORD towardsVector_2 = camTar_2 - camPos_2;
			int towardsVector_3 = camPos_3 - camTar_3;
			
			double length = sqrt((double)towardsVector_1 * (double)towardsVector_1 + (double)towardsVector_3 * (double)towardsVector_3);
			double step = 40000.0 / (length + 1);
			int step_1 = (int)(step * towardsVector_1);
			int step_3 = (int)(step * towardsVector_3);
			
			switch (p->vkCode){
			case 'W':
				camTar_1 -= step_1;
				camTar_3 -= step_3;
				WriteCamTarget();
				return TRUE;
			case 'S':
				camTar_1 += step_1;
				camTar_3 += step_3;
				WriteCamTarget();
				return TRUE;
			case 'A':
				camTar_1 -= step_3;
				camTar_3 += step_1;
				WriteCamTarget();
				return TRUE;
			case 'D':
				camTar_1 += step_3;
				camTar_3 -= step_1;
				WriteCamTarget();
				return TRUE;
			case VK_SPACE:
				camTar_2 += 40000;
				WriteCamTarget();
				return TRUE;
			case VK_LCONTROL:
			case VK_RCONTROL:
				camTar_2 -= 40000;
				WriteCamTarget();
				return TRUE;
			}
		}
		
		return CallNextHookEx (hHook, code, wParam, lParam);
	}
	
	void storeOriPos(){
		ReadProcessMemory (pal5q.GetProcessHandle(), (LPVOID)camTargetAddr, &ori_1, sizeof(ori_1), NULL);
		ReadProcessMemory (pal5q.GetProcessHandle(), (LPVOID)(camTargetAddr + 4), &ori_2, sizeof(ori_2), NULL);
		ReadProcessMemory (pal5q.GetProcessHandle(), (LPVOID)(camTargetAddr + 8), &ori_3, sizeof(ori_3), NULL);
	}
	
	void restoreOriPos(){
		WriteProcessMemory (pal5q.GetProcessHandle(), (LPVOID)camTargetAddr, &ori_1, sizeof(ori_1),NULL);
		WriteProcessMemory (pal5q.GetProcessHandle(), (LPVOID)(camTargetAddr + 4), &ori_2, sizeof(ori_2), NULL);
		WriteProcessMemory (pal5q.GetProcessHandle(), (LPVOID)(camTargetAddr + 8), &ori_3, sizeof(ori_3), NULL);
	}
public:
	FreeCamera():hHook(NULL){
		union{
			long (FreeCamera::*p)(int,unsigned int,long);
			DWORD addr;
		}tmp;
		tmp.p = &FreeCamera::KbdHookProc;
		fakeKbdHookProc[0] = 0xb9;
		*((DWORD*)&fakeKbdHookProc[1]) = (DWORD)this;
		fakeKbdHookProc[5] = 0xe9;
		*((DWORD*)&fakeKbdHookProc[6]) = (DWORD)tmp.addr - (DWORD)fakeKbdHookProc - 10;
	}
	~FreeCamera(){stop();}
	
	bool start(){
		if(hHook != NULL)
			return true;
		if(!pal5q.Found()){
			MessageBox(0, "游戏未开启", 0, 0);
			return false;
		}
		
		DWORD Addr_base = 0x00964b2c;
		DWORD offset = 0x44;
		DWORD dwWriteBytes;
		
		ReadProcessMemory (pal5q.GetProcessHandle(), (LPVOID)Addr_base, &camTargetAddr, sizeof(camTargetAddr), &dwWriteBytes);
		camTargetAddr += offset;
		VirtualProtectEx(pal5q.GetProcessHandle(), (LPVOID)camTargetAddr, sizeof(DWORD) * 3, PAGE_READWRITE, NULL);
		storeOriPos();
		camPosAddr = 0x00a2a22c;
		
		
		hHook = SetWindowsHookEx(
            WH_KEYBOARD_LL,
            (HOOKPROC)((void*)fakeKbdHookProc),
            GetModuleHandle (NULL),
            0);
		if(hHook == NULL){
			MessageBox(NULL, "挂钩失败", 0, 0);
			return false;
		}
		return true;
	}
	
	void stop(){
		if(hHook){
			UnhookWindowsHookEx (hHook);
			hHook = NULL;
			restoreOriPos();
		}
	}
};

class PropertyModify{
public:
	enum {LXJYS, MONEY, NUM};
private:
	struct Property{
		DWORD addr;
		DWORD value;
		bool lock;
	};
	
	Property pro[NUM];
	HANDLE hThread;
	bool thread;
	
	static DWORD WINAPI ScanProc (LPVOID lpParam){
		PropertyModify* p = (PropertyModify*)lpParam;
		while(p->thread){
			Sleep(100);
			if(!pal5q.Found()){
				continue;
			}
			p->FillAddr();
			
			while(p->thread){
				Sleep(50);
				for(int i = 0; i < NUM; i++){
					if(p->pro[i].lock){
						WriteProcessMemory(pal5q.GetProcessHandle(), (void*)p->pro[i].addr, &p->pro[i].value, sizeof(p->pro[i].value), NULL);
					}else{
						ReadProcessMemory(pal5q.GetProcessHandle(), (void*)p->pro[i].addr, &p->pro[i].value, sizeof(p->pro[i].value), NULL);
					}
				}
			}
		}
		return 0;
	}
	
	
	void FillAddr(){
		pro[LXJYS].addr = 0x00964ac4;
		pro[MONEY].addr = 0x00964b64;
	}
	public:
		PropertyModify():thread(false), hThread(INVALID_HANDLE_VALUE){memset(pro, 0, sizeof(pro));}
		~PropertyModify(){stop();}
		
		void setLocked(bool lock, unsigned int value, int index){
			pro[index].lock = lock;
			pro[index].value = value;
		}
		
		void setValue(unsigned int value, int index){
			pro[index].value = value;
			WriteProcessMemory(pal5q.GetProcessHandle(), (void*)pro[index].addr, &value, sizeof(value), NULL);
		}
		
		DWORD getValue(int index){
			return pro[index].value;
		}

		bool start(){
			if(hThread != INVALID_HANDLE_VALUE){
				return true;
			}
			thread = true;
			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ScanProc, this, 0, NULL);
			return true;
		}
		
		void stop(){
			if(hThread != INVALID_HANDLE_VALUE){
				thread = false;
				WaitForSingleObject(hThread, 500);
				TerminateThread(hThread, 0);
				CloseHandle(hThread);
				hThread = INVALID_HANDLE_VALUE;
			}
		}
};