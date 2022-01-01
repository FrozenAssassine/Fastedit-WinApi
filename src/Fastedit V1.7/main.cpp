//Fastedit WinAPI
//Created by Julius
#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <stdio.h>
#include <sys/stat.h>
#include <CommCtrl.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <sapi.h>
#include <limits>
#include <richedit.h>
#include <atlbase.h>
#include <direct.h>
#include <stdlib.h> 
#include <algorithm>
#include <string>
#include "ReadLanguagePack.h"
#include "Items.h"
#include "resource.h"
#include "definitions.h"

static LONG_PTR PrevWndProcEdit;

using namespace std;

void getscreenpossearchwnd();
void searchdlgclass(HINSTANCE hInst);
void writeinifile();
void gotolinedlgclass(HINSTANCE hInst);
void AboutDlgclass(HINSTANCE hInst);
void saveDLGclass(HINSTANCE hInst);
void optionsDLGclass(HINSTANCE hInst);
void setinipath();
void UpdateStatusBar(LPSTR lpszStatusString, WORD partNumber);
void ShowCursorPos();
void writeFilenameinTitle(char *file);
void getsettingsini();
void setpreftowindow();
void OpenlangPack();
void savedlg(HWND hWnd, char*MB_txt, char * headline);
void CustomMBDLGclass(HINSTANCE hInst);
void MB_Dialog(HWND hWnd, char*WindowText, char*BTN_1, char*BTN_2, int BTNS);
void readinifile();
void setfontofDialogs();
void writelangpackstoini();
//void registerfileextension();
void getprojectfolderpath();

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EditWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SearchProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK changelogDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK gotolineProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AboutProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SaveProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PrefProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MB_Procedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

HINSTANCE hinstance;

DWORD CALLBACK MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	std::ifstream* pFile = (ifstream*)dwCookie;
	pFile->read((char*)pbBuff, cb);
	return 0;
}
DWORD CALLBACK EditStreamOutCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	HANDLE hFile = (HANDLE)dwCookie;
	DWORD NumberOfBytesWritten;
	if (!WriteFile(hFile, pbBuff, cb, &NumberOfBytesWritten, NULL))
	{
		//handle errors
		return 1;
		// or perhaps return GetLastError();
	}
	*pcb = NumberOfBytesWritten;
	return 0;
}
DWORD CALLBACK SaveFileCallBack(DWORD dwCookie, LPBYTE lpBuffer, LONG lSize, LONG *plWriten)
{
	if (!lSize) return 1;

	// BCC 5.5 need (LPDWORD) conversion.
	if (!WriteFile((HANDLE)dwCookie, lpBuffer, lSize, (LPDWORD)plWriten, NULL))
		return -1;
	return 0;
}
DWORD CALLBACK LoadFileCallBack(DWORD dwCookie, LPBYTE lpBuffer, LONG lSize, LONG *plRead)
{
	if (!lSize) return 1;

	// BCC 5.5 needs (LPDWORD) conversion.
	if (!ReadFile((HANDLE)dwCookie, lpBuffer, lSize, (LPDWORD)plRead, NULL))
		return -1;
	return 0;
}

namespace IOstream {
	
	void SavetxtFile(char * path)
	{

		HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			char outfile[6000];
			wsprintf(outfile, "%s %s %s", Error_Couldnt_Write_File, path, Error_Couldnt_Write);
			MessageBox(hMainWindow, outfile, "Fastedit Error", MB_OK);
		}
		
		EDITSTREAM es = { 0 };
		es.dwCookie = (DWORD)hFile;
		es.dwError = 0;
		es.pfnCallback = SaveFileCallBack;
		SendMessage(hEdit, EM_STREAMOUT, SF_TEXT, (LPARAM)&es);
		CloseHandle(hFile);

		writeFilenameinTitle(path);

		neversave = false;
		needsave = false;
		openrtfbool = false;
		opentxtbool = false;
		savetxtbool = true;
		savertfbool = false;

	}

	void OpentxtFile(char * path)
	{
		HANDLE hFile;
		EDITSTREAM es;

		hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hMainWindow, "Couldn't open file for reading", "Fastedit io Error", MB_OK);
		}
		es.dwCookie = (DWORD)hFile;
		es.dwError = 0;
		es.pfnCallback = LoadFileCallBack;
		SendMessage(hEdit, EM_STREAMIN, SF_TEXT, (LPARAM)&es);

		CloseHandle(hFile);
		writeFilenameinTitle(path);
		neversave = false;
		needsave = false;
		openrtfbool = false;
		opentxtbool = true;
		savetxtbool = false;
		savertfbool = false;
		ShowCursorPos();
	}

	void savertf(char* path)
	{
		//LoadAccelerators(hInst,);
		HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hMainWindow, "Couldn't Open file for writing", "Fastedit io Error", MB_OK);
		}
		EDITSTREAM es = { 0 };
		es.dwCookie = (DWORD_PTR)hFile;
		es.pfnCallback = EditStreamOutCallback;
		SendMessage(hEdit, EM_STREAMOUT, SF_RTF, (LPARAM)&es);
		CloseHandle(hFile);

		writeFilenameinTitle(path);

		neversave = false;
		needsave = false;
		openrtfbool = false;
		opentxtbool = false;
		savetxtbool = false;
		savertfbool = true;
		ShowCursorPos();
	}

	void openrtf(char * path) {

		std::ifstream  File(path);
		EDITSTREAM es = { 0 };
		es.dwCookie = (DWORD)&File;
		es.pfnCallback = MyStreamInCallback;
		SendMessage(hEdit, EM_STREAMIN, SF_RTF, (LPARAM)&es);


		writeFilenameinTitle(path);
		neversave = false;
		needsave = false;
		openrtfbool = true;
		opentxtbool = false;
		savetxtbool = false;
		savertfbool = false;
		ShowCursorPos();

	}

	void save_file(HWND hWnd)
	{
		readinifile();
		OPENFILENAME sfn;

		ZeroMemory(&sfn, sizeof(OPENFILENAME));
		sfn.lStructSize = sizeof(OPENFILENAME);
		sfn.hwndOwner = hWnd;
		sfn.lpstrFile = open_save_file;
		sfn.nMaxFile = sizeof(open_save_file);
		sfn.lpstrFilter = Open_Save_Filefilter;
		sfn.nMaxFileTitle = 1;
		sfn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
		sfn.lpstrDefExt = "txt";
		sfn.lpstrInitialDir = lastSavedPath;
		sfn.nFilterIndex = lastfileextensionsaved;
		sfn.lpstrTitle = Save_File_DLG_Title;
		if (GetSaveFileName(&sfn) == TRUE)
		{
			wsprintf(lastSavedPath, sfn.lpstrFile); //Write LastFilePath to char

			lastfileextensionsaved = sfn.nFilterIndex; //Write Last File Extension to int
			writeinifile(); //Save that in .ini File

			save_open_sucess = true;
			if (sfn.nFilterIndex == 1 || sfn.nFilterIndex == 2) {
				savertf(sfn.lpstrFile);
			}
			if (sfn.nFilterIndex != 1 && sfn.nFilterIndex != 2) {
				SavetxtFile(sfn.lpstrFile);

			}
		}
		else {
			save_open_sucess = false;
		}
	}

	void open_file(HWND hWnd) {
		readinifile();
		OPENFILENAME ofn;       // common dialog box structure
								// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = open_save_file;
		ofn.nMaxFile = sizeof(open_save_file);
		ofn.lpstrFilter = Open_Save_Filefilter;
		ofn.nFilterIndex = lastfileextensionOpened;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
		ofn.lpstrInitialDir = lastOpenedPath;
		ofn.lpstrTitle = Open_File_DLG_Title;
		if (GetOpenFileName(&ofn) == TRUE)
		{
			wsprintf(lastOpenedPath, ofn.lpstrFile);//Write LastFilePath to char
			lastfileextensionOpened = ofn.nFilterIndex;//Write Last File Extension to int
			writeinifile();//Save that in .ini File

			save_open_sucess = true;

			if (ofn.nFilterIndex == 1 || ofn.nFilterIndex == 2) {
				openrtf(ofn.lpstrFile);
			}
			if (ofn.nFilterIndex != 1 && ofn.nFilterIndex != 2) {
				OpentxtFile(ofn.lpstrFile);
			}
		}
		else {
			save_open_sucess = false;
		}
	}

	void saveascopy(HWND hWnd) {

		if (neversave == true) {
			IOstream::save_file(hWnd);
			saveascopy(hWnd);
		}
		else {
			char originalfile[_MAX_PATH];
			char outfile[_MAX_PATH];
			char pathfromfile[_MAX_PATH];
			char extensionfromfile[_MAX_PATH];
			char filenamefromfile[_MAX_PATH];

			wsprintf(originalfile, open_save_file);

#if defined _WIN32
			const char *lastSeparator = strrchr(originalfile, '\\');
#else
			const char *lastSeparator = strrchr(originalfile, '/');
#endif
			const char *lastDot = strrchr(originalfile, '.');
			const char *endOfPath = originalfile + strlen(originalfile);
			const char *startOfName = lastSeparator ? lastSeparator + 1 : originalfile;
			const char *startOfExt = lastDot > startOfName ? lastDot : endOfPath;
			_snprintf(pathfromfile, _MAX_PATH, "%.*s", startOfName - originalfile, originalfile);
			_snprintf(filenamefromfile, _MAX_PATH, "%.*s", startOfExt - startOfName, startOfName);
			_snprintf(extensionfromfile, _MAX_PATH, "%s", startOfExt);

			wsprintf(outfile, "%s%s - Copy%s", pathfromfile, filenamefromfile, extensionfromfile);

			if (openrtfbool == true) {
				IOstream::savertf(outfile);
			}
			if (opentxtbool == true) {
				IOstream::SavetxtFile(outfile);
			}
			if (savetxtbool == true) {
				IOstream::SavetxtFile(outfile);
			}
			if (savertfbool == true) {
				IOstream::savertf(outfile);
			}
			save_open_sucess = true;
		}
		writeFilenameinTitle(lastopenedpath);
	}

}
namespace rich_edit {
	void replace_sel(HWND hWnd, const char* str) {
		SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)str);
	}
	void colorseltext(HWND hWnd, COLORREF clr) {
		CHARFORMAT cf;
		cf.cbSize = sizeof(cf);
		cf.dwMask = CFM_COLOR;
		cf.crTextColor = clr;
		cf.dwEffects = 0;
		SendMessage(hEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
	}
	void changefontsize(HWND hWnd, int size) {
		CHARFORMAT cf;
		cf.cbSize = sizeof(cf);
		cf.dwMask = CFM_SIZE;
		cf.dwEffects = 0;
		cf.yHeight = size;
		SendMessage(hEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
	}
	void txtBold() {
		CHARFORMAT boldfont;
		boldfont.cbSize = sizeof(CHARFORMAT);
#ifndef SET_OTHER_STUFF
		boldfont.dwMask = CFM_BOLD;
#else
		boldfont.dwMask = CFM_COLOR | CFM_SIZE | CFM_BOLD | CFM_CHARSET;
		boldfont.crTextColor = RGB(255, 0, 0);
		boldfont.yHeight = 300;
		boldfont.bCharSet = OEM_CHARSET;
#endif
		boldfont.dwEffects = CFE_BOLD;
		SendMessage(hEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&boldfont);
	}
	void txtUnderlined() {
		CHARFORMAT underlinedfont;
		underlinedfont.cbSize = sizeof(CHARFORMAT);
#ifndef SET_OTHER_STUFF
		underlinedfont.dwMask = CFM_UNDERLINE;
#else
		underlinedfont.dwMask = CFM_COLOR | CFM_SIZE | CFM_UNDERLINE | CFM_CHARSET;
		underlinedfont.crTextColor = RGB(0, 0, 0);
		underlinedfont.yHeight = 300;
		underlinedfont.bCharSet = OEM_CHARSET;
#endif
		underlinedfont.dwEffects = CFM_UNDERLINE;
		SendMessage(hEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&underlinedfont);
	}
	void txtNormal() {
		CHARFORMAT reset;
		reset.cbSize = sizeof(CHARFORMAT);
#ifndef SET_OTHER_STUFF
		reset.dwMask = CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFU_UNDERLINEWAVE;
#else
		reset.dwMask = CFM_COLOR | CFM_SIZE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFU_UNDERLINEWAVE | CFM_CHARSET;
		reset.crTextColor = RGB(0, 0, 0);
		reset.yHeight = 200;
		reset.bCharSet = ANSI_CHARSET;
#endif
		reset.dwEffects = 0;
		SendMessage(hEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&reset);
	}
	void txtKursiv() {
		CHARFORMAT Kursivfont;
		Kursivfont.cbSize = sizeof(CHARFORMAT);
#ifndef SET_OTHER_STUFF
		Kursivfont.dwMask = CFM_ITALIC;
#else
		Kursivfont.dwMask = CFM_COLOR | CFM_SIZE | CFM_ITALIC | CFM_CHARSET;
		Kursivfont.crTextColor = RGB(0, 0, 0);
		Kursivfont.yHeight = 300;
		Kursivfont.bCharSet = OEM_CHARSET;
#endif
		Kursivfont.dwEffects = CFM_ITALIC;
		SendMessage(hEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&Kursivfont);
	}
	void writerainbowcolor() {

		DWORD rgb1 = RGB(255, 0, 0);
		DWORD rgb2 = RGB(255, 100, 0);
		DWORD rgb3 = RGB(255, 255, 0);
		DWORD rgb4 = RGB(0, 255, 0);
		DWORD rgb5 = RGB(0, 255, 255);
		DWORD rgb6 = RGB(0, 0, 255);
		DWORD rgb7 = RGB(170, 0, 255);

		CHARFORMAT cf;
		cf.cbSize = sizeof(cf);
		cf.dwMask = CFM_COLOR;
		if (rainbowcount == 1) { cf.crTextColor = rgb1; }
		if (rainbowcount == 2) { cf.crTextColor = rgb2; }
		if (rainbowcount == 3) { cf.crTextColor = rgb3; }
		if (rainbowcount == 4) { cf.crTextColor = rgb4; }
		if (rainbowcount == 5) { cf.crTextColor = rgb5; }
		if (rainbowcount == 6) { cf.crTextColor = rgb6; }
		if (rainbowcount == 7) { cf.crTextColor = rgb7; }

		cf.dwEffects = 0;
		SendMessage(hEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

	}
	void txtdurchgestrichen() {
		CHARFORMAT underlinedfont;
		underlinedfont.cbSize = sizeof(CHARFORMAT);
#ifndef SET_OTHER_STUFF
		underlinedfont.dwMask = CFU_UNDERLINEWAVE;
#else
		underlinedfont.dwMask = CFM_COLOR | CFM_SIZE | CFU_UNDERLINEWAVE | CFM_CHARSET;
		underlinedfont.crTextColor = RGB(0, 0, 0);
		underlinedfont.yHeight = 300;
		underlinedfont.bCharSet = OEM_CHARSET;
#endif
		underlinedfont.dwEffects = CFU_UNDERLINEWAVE;
		SendMessage(hEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&underlinedfont);
	}
	void scroll_to(DWORD pos) {
		SendMessage(hEdit, WM_VSCROLL, pos, 0);
	}
}

int ReadINI_Int(const std::string &sSection, const std::string &sSub, int iDefaultValue)
{
	return GetPrivateProfileInt(sSection.c_str(), sSub.c_str(), iDefaultValue, inipath);
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	getprojectfolderpath();
	setinipath();
	readinifile();
	CustomMBDLGclass(hInst);
	searchdlgclass(hInst);
	gotolinedlgclass(hInst);
	AboutDlgclass(hInst);
	saveDLGclass(hInst);
	optionsDLGclass(hInst);
	getsettingsini();
	hinstance = hInst;
	texttoopen = args;

	WNDCLASSW wc = { 0 };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"MainWNDclass";
	wc.lpfnWndProc = WindowProcedure;
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));

	if (!RegisterClassW(&wc))
		return -1;
	hMainWindow = CreateWindowW(L"MainWNDclass", L"Fastedit", WS_OVERLAPPEDWINDOW | WS_VISIBLE, xpos, ypos, x, y,
		NULL, NULL, NULL, NULL);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

void SetCursor(LPCSTR cur) {
	HCURSOR cursor = LoadCursor(NULL, cur);
	SetCursor(cursor);
}

int AskSaveMB() {
	char mb_out_txt[_MAX_PATH];
	wsprintf(mb_out_txt, MessageBox_Ask_To_Save, lastopenedpath);

	int saveMB = MessageBox(hMainWindow, mb_out_txt, MessageBox_Ask_To_Save_Title, MB_YESNOCANCEL);
	if (saveMB == IDYES) {
		return 0;
	}
	if (saveMB == IDNO) {
		return 1;
	}
	if (saveMB == IDCANCEL) {
		return 2;
	}
}

////Savefunctions
void nonamefile() {
	char setfilenameto[_MAX_PATH];
	lastopenedpath = TitleName_Untitled;
	wsprintf(setfilenameto, "Fastedit - [%s]", TitleName_Untitled);

	SetWindowText(hMainWindow, setfilenameto);
}
void save_only(HWND hWnd) {

	if (neversave == true) {
		IOstream::save_file(hWnd);
	}
	if (neversave == false) {
		if (openrtfbool == true) {
			IOstream::savertf(open_save_file);
		}
		if (opentxtbool == true) {
			IOstream::SavetxtFile(open_save_file);
		}
		if (savetxtbool == true) {
			IOstream::SavetxtFile(open_save_file);
		}
		if (savertfbool == true) {
			IOstream::savertf(open_save_file);
		}
	}
	SetCursor(IDC_WAIT);
	SetTimer(hWnd, SAVE_CURSOR_TIMER, 200, (TIMERPROC)NULL);
	writeFilenameinTitle(lastopenedpath);
}
void exitsave(HWND hWnd) {

	if (needsave == true) {
		int res = AskSaveMB();
		if (res == 0) {
			save_only(hWnd);
			if (save_open_sucess == true) {
				PostQuitMessage(0);
			}
			else {
				DestroyWindow(hSaveWND);
				EnableWindow(hMainWindow, true);
				SetActiveWindow(hMainWindow);
			}
		}
		if (res == 1) {
			PostQuitMessage(0);
		}
	}
	else {
		PostQuitMessage(0);
	}
}
void newfile(HWND hWnd) {

	if (needsave == true) {
		int res = AskSaveMB();
		if (res == 1) {
			if (save_open_sucess) {
				SetWindowText(hEdit, "");

			}
		}
		if (res == 2) {

		}
	}
	else {
		SetWindowText(hEdit, "");
		nonamefile();
		neversave = true;
	}

}
void opennewfile(HWND hWnd) {

	if (needsave == true) {
		int res = AskSaveMB();
		if (res == 1) {

		}
		if (res == 2) {

		}
	}
	else {
		IOstream::open_file(hWnd);
	}
}
void Doopendragdrop() {
	if (rtfopen == true) {
		IOstream::openrtf(openfile);
		wsprintf(open_save_file, openfile);
	}
	if (rtfopen == false) {
		wsprintf(open_save_file, openfile);
		IOstream::OpentxtFile(open_save_file);
	}
}
void opendragdrop(HWND hWnd) {

	if (needsave == true) {
		int saveMBres = AskSaveMB();
		if (saveMBres == 0) { //yes
			if (save_open_sucess) {
				Doopendragdrop();
			}
		}
		if(saveMBres == 1){ //no
			Doopendragdrop();
		}
	}
	else {
		
	}
}
void openfileonopen(HWND hWnd, char * file) {

	string texttoconvert = texttoopen;

	std::replace(texttoconvert.begin(), texttoconvert.end(), '"', ' ');
	texttoconvert.erase(0, 1);

	wsprintf(texttoopen, texttoconvert.c_str());
	wsprintf(open_save_file, texttoconvert.c_str());

	if (strstr(texttoopen, ".fed") != NULL || strstr(texttoopen, ".rtf") != NULL)
	{
		IOstream::openrtf(open_save_file);
	}
	else {
		IOstream::OpentxtFile(open_save_file);
	}
	writeFilenameinTitle(texttoopen);
}
void closefile(HWND hWnd) {
	if (needsave == true) {
		int res = AskSaveMB();
		if (res == 1) {
			save_only(hWnd);
			if (save_open_sucess) {

			}
		}
		if (res == 2) {

		}
		SetWindowText(hWnd, "Fastedit");
		needsave = true;
		neversave = true;
		wsprintf(open_save_file, "");
	}
	else {
		SetWindowText(hEdit, "");
		SetWindowText(hWnd, "Fastedit");
		needsave = true;
		neversave = true;
		wsprintf(open_save_file, "");
	}
}
//AddGUI
void addListbox(HWND hWnd) {																																		
	prefLanguageList = CreateWindowEx(NULL, "LISTBOX", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE |ES_AUTOVSCROLL|WS_VSCROLL| WS_HSCROLL| ES_AUTOHSCROLL | LBS_NOTIFY, 320, 20, 250, 50, hWnd, NULL, GetModuleHandle(NULL), NULL);
}
void addStringListBox(HWND hWndListR, char*txt) {
	SendMessage(hWndListR, LB_ADDSTRING, NULL, (LPARAM)txt);
}
void addEditControl(HWND hWnd, int xposedit, int yposedit, int xedit, int yedit) {
	hEdit = CreateWindowEx(NULL, RICHEDIT_CLASS, "",
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | WS_HSCROLL,
		xposedit, yposedit, xedit, yedit, hWnd, NULL, NULL, NULL);

	PrevWndProcEdit = SetWindowLongPtr(hEdit, GWLP_WNDPROC,
		(LONG_PTR)EditWndProc);

	SendMessage(hEdit, EM_SETLIMITTEXT, -1, 0);
	SendMessage(hEdit, EM_SETEVENTMASK, 0, (LPARAM)(DWORD)ENM_DROPFILES);
	DragAcceptFiles(hEdit, true);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)MainFont, MAKELPARAM(TRUE, 0));
	SendMessage(hEdit, EM_SETZOOM, 3, 2);
}
void Menubar(HWND hWnd) {

	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hFileRecentMenu = CreateMenu();
	HMENU hWindowMenu = CreateMenu();
	HMENU hEditMenu = CreateMenu();
	HMENU hTextColorMenu = CreateMenu();
	HMENU hHelpMenu = CreateMenu();
	HMENU hSearchMenu = CreateMenu();
	HMENU hColorSubMenu = CreateMenu();
	HMENU hUnderlinedSubmenu = CreateMenu();
	HMENU hGotoMenu = CreateMenu();
	//MainMenu
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, Menu_File);
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, Menu_Edit);
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hTextColorMenu, Menu_Format);
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSearchMenu, Menu_Search);
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hWindowMenu,Menu_View);
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, Menu_Help);
	//FileMenu
	AppendMenu(hFileMenu, MF_STRING, MENU_NEW, Menu_File_New);
	AppendMenu(hFileMenu, MF_STRING, MENU_OPEN, Menu_File_Open);
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, MENU_CLOSEOPENED, Menu_File_Close_File);
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, MENU_SAVE, Menu_File_Save);
	AppendMenu(hFileMenu, MF_STRING, MENU_SAVEAS, Menu_File_Save_As);
	AppendMenu(hFileMenu, MF_STRING, MENU_SAVEASCOPY, Menu_File_Save_As_Copy);
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, MENU_NEWEDITOR, Menu_File_NewEditor);
	AppendMenu(hFileMenu, MF_STRING, MENU_PREFERENCES, Menu_File_Preferences);
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, MENU_EXIT, Menu_File_Exit);
	//EditMenu
	AppendMenu(hEditMenu, MF_STRING, MENU_REDO, Menu_Edit_Redo);
	AppendMenu(hEditMenu, MF_STRING, MENU_UNDO, Menu_Edit_Undo);
	AppendMenu(hEditMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hEditMenu, MF_STRING, MENU_COPY, Menu_Edit_Copy);
	AppendMenu(hEditMenu, MF_STRING, MENU_PASTE, Menu_Edit_Paste);
	AppendMenu(hEditMenu, MF_STRING, MENU_CUT, Menu_Edit_Cut);
	AppendMenu(hEditMenu, MF_STRING, MENU_DELETE, Menu_Edit_Delete);
	AppendMenu(hEditMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hEditMenu, MF_STRING, MENU_SELALL, Menu_Edit_Select_All);

	//SucheMenu
	AppendMenu(hSearchMenu, MF_STRING, MENU_SEARCH, Menu_Search_Search);
	AppendMenu(hSearchMenu, MF_STRING, MENU_GOTOLINE, Menu_Search_GoToLine);
	AppendMenu(hSearchMenu, MF_STRING | MF_POPUP, (UINT_PTR)hGotoMenu, Menu_Search_GoTo);
	AppendMenu(hGotoMenu, MF_STRING, MENU_GOTOTOP, Menu_Search_GoTo_Begin);
	AppendMenu(hGotoMenu, MF_STRING, MENU_GOTOBOTTOM, Menu_Search_GoTo_End);

	//FormatMenu
	AppendMenu(hTextColorMenu, MF_STRING | MF_POPUP, (UINT_PTR)hColorSubMenu, Menu_Format_Colors);
	AppendMenu(hColorSubMenu, MF_STRING, MENU_COLORBLACK, Menu_Format_Colors_Black);
	AppendMenu(hColorSubMenu, MF_STRING, MENU_COLORRED, Menu_Format_Colors_Red);
	AppendMenu(hColorSubMenu, MF_STRING, MENU_COLORGREEN, Menu_Format_Colors_Green);
	AppendMenu(hColorSubMenu, MF_STRING, MENU_COLORBLUE, Menu_Format_Colors_Blue);
	AppendMenu(hColorSubMenu, MF_STRING, MENU_COLORYELLOW, Menu_Format_Colors_Yellow);
	AppendMenu(hColorSubMenu, MF_STRING, MENU_RAINBOW, Menu_Format_Colors_Rainbow);
	AppendMenu(hTextColorMenu, MF_STRING, MENU_TXTCOLOR, Menu_Format_Colors_Color);
	AppendMenu(hTextColorMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hTextColorMenu, MF_STRING, MENU_TXT_NORMAL, Menu_Format_Normal);
	AppendMenu(hTextColorMenu, MF_STRING, MENU_TXTBOLD, Menu_Format_Bold);
	AppendMenu(hTextColorMenu, MF_STRING, MENU_TXTKURSIV, Menu_Format_Italic);
	AppendMenu(hTextColorMenu, MF_STRING | MF_POPUP, (UINT_PTR)hUnderlinedSubmenu, Menu_Format_Underlined);
	AppendMenu(hUnderlinedSubmenu, MF_STRING, MENU_TXT_UNDERLINE, Menu_Format_Underlined_Underlined);
	AppendMenu(hUnderlinedSubmenu, MF_STRING, MENU_TXT_DURCHGESTRICHEN, Menu_Format_Underlined_Striketout);
	AppendMenu(hTextColorMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hTextColorMenu, MF_STRING, MENU_WORDWRAP, Menu_Format_Wordwrap);

	CheckMenuItem(hMenu, MENU_WORDWRAP, MF_UNCHECKED);
	//WindowMenu
	//AppendMenu(hWindowMenu, MF_STRING, MENU_BORDERLESS, "Randlos");
	AppendMenu(hWindowMenu, MF_STRING, MENU_ALWAYSONTOP1, Menu_View_AlwaysOnTop);
	AppendMenu(hWindowMenu, MF_STRING, MENU_MINIMODUS, Menu_View_MiniMode);
	AppendMenu(hWindowMenu, MF_STRING, MENU_TOOLBAR, "Toolbar");
	AppendMenu(hWindowMenu, MF_STRING, MENU_STATUSBAR, "Statusbar");

	CheckMenuItem(hMenu, MENU_BORDERLESS, MF_UNCHECKED);

	//HelpMenu
	AppendMenu(hHelpMenu, MF_STRING, MENU_SHORTCUTS, Menu_Help_Shortcuts);
	AppendMenu(hHelpMenu, MF_STRING, MENU_CHANGELOG, Menu_Help_Changelog);
	AppendMenu(hHelpMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hHelpMenu, MF_STRING, MENU_ABOUT, Menu_Help_About);

	SetMenu(hWnd, hMenu);
}
void AddToolbarv(HWND hWnd) {

	TBBUTTON tbb[11];
	TBADDBITMAP tbab;

	hToolv = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBS_TOOLTIPS | WS_VISIBLE | TBSTYLE_FLAT, 0, 0, 0, 0,
		hWnd, NULL, NULL, NULL);

	SendMessage(hToolv, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hToolv, TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_HIDECLIPPEDBUTTONS);

	tbab.hInst = HINST_COMMCTRL;
	tbab.nID = IDB_STD_LARGE_COLOR;
	SendMessage(hToolv, TB_ADDBITMAP, 0, (LPARAM)&tbab);

	ZeroMemory(tbb, sizeof(tbb));
	tbb[0].iBitmap = STD_FILENEW;
	tbb[0].fsState = TBSTATE_ENABLED;
	tbb[0].fsStyle = TBSTYLE_BUTTON;
	tbb[0].idCommand = MENU_NEW;

	tbb[1].iBitmap = STD_FILEOPEN;
	tbb[1].fsState = TBSTATE_ENABLED;
	tbb[1].fsStyle = TBSTYLE_BUTTON;
	tbb[1].idCommand = MENU_OPEN;

	tbb[2].iBitmap = STD_FILESAVE;
	tbb[2].fsState = TBSTATE_ENABLED;
	tbb[2].fsStyle = TBSTYLE_BUTTON;
	tbb[2].idCommand = MENU_SAVE;

	tbb[3].iBitmap = STD_COPY;
	tbb[3].fsState = TBSTATE_ENABLED;
	tbb[3].fsStyle = TBSTYLE_BUTTON;
	tbb[3].idCommand = MENU_COPY;

	tbb[4].iBitmap = STD_PASTE;
	tbb[4].fsState = TBSTATE_ENABLED;
	tbb[4].fsStyle = TBSTYLE_BUTTON;
	tbb[4].idCommand = MENU_PASTE;

	tbb[5].iBitmap = STD_CUT;
	tbb[5].fsState = TBSTATE_ENABLED;
	tbb[5].fsStyle = TBSTYLE_BUTTON;
	tbb[5].idCommand = MENU_CUT;

	tbb[6].iBitmap = STD_DELETE;
	tbb[6].fsState = TBSTATE_ENABLED;
	tbb[6].fsStyle = TBSTYLE_BUTTON;
	tbb[6].idCommand = MENU_DELETE;

	tbb[7].iBitmap = STD_UNDO;
	tbb[7].fsState = TBSTATE_ENABLED;
	tbb[7].fsStyle = TBSTYLE_BUTTON;
	tbb[7].idCommand = MENU_UNDO;

	tbb[8].iBitmap = STD_REDOW;
	tbb[8].fsState = TBSTATE_ENABLED;
	tbb[8].fsStyle = TBSTYLE_BUTTON;
	tbb[8].idCommand = MENU_REDO;

	tbb[9].iBitmap = STD_FIND;
	tbb[9].fsState = TBSTATE_ENABLED;
	tbb[9].fsStyle = TBSTYLE_BUTTON;
	tbb[9].idCommand = MENU_SEARCH;

	tbb[10].iBitmap = STD_HELP;
	tbb[10].fsState = TBSTATE_ENABLED;
	tbb[10].fsStyle = TBSTYLE_BUTTON;
	tbb[10].idCommand = MENU_SHORTCUTS;

	SendMessage(hToolv, TB_SETBUTTONSIZE, (WPARAM)0, (LPARAM)MAKELONG(16, 16));
	SendMessage(hToolv, TB_ADDBUTTONS, sizeof(tbb) / sizeof(TBBUTTON), (LPARAM)&tbb);
	SendMessage(hToolv, TB_AUTOSIZE, 0, 0);

}
void addStatusbar(HWND hWnd) {

	hStatusBar = CreateWindowEx(
		0,
		STATUSCLASSNAME,
		(PCTSTR)NULL,
		SBARS_SIZEGRIP |
		WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0,
		hWnd,
		(HMENU)STATUSBAR_ID_1,
		NULL,
		NULL);
}
void PopuMenu(LPARAM lp) {
	POINT point;
	point.x = LOWORD(lp);
	point.y = HIWORD(lp);

	HMENU hPOPMenu = CreatePopupMenu();
	HMENU hColorSubMenu = CreatePopupMenu();
	HMENU hFormattxtMenu = CreateMenu();

	ClientToScreen(hEdit, &point);
	AppendMenu(hPOPMenu, MF_STRING, MENU_COPY, Menu_Edit_Copy);
	AppendMenu(hPOPMenu, MF_STRING, MENU_PASTE, Menu_Edit_Paste);
	AppendMenu(hPOPMenu, MF_STRING, MENU_CUT, Menu_Edit_Cut);
	AppendMenu(hPOPMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hPOPMenu, MF_STRING, MENU_DELETE, Menu_Edit_Delete);
	AppendMenu(hPOPMenu, MF_STRING, MENU_SELALL, Menu_Edit_Select_All);
	AppendMenu(hPOPMenu, MF_SEPARATOR, 0, NULL);

	AppendMenu(hPOPMenu, MF_POPUP, (UINT_PTR)hFormattxtMenu, Menu_Format);
	AppendMenu(hFormattxtMenu, MF_STRING, MENU_TXT_NORMAL, Menu_Format_Normal);
	AppendMenu(hFormattxtMenu, MF_STRING, MENU_TXTBOLD, Menu_Format_Bold);
	AppendMenu(hFormattxtMenu, MF_STRING, MENU_TXTKURSIV, Menu_Format_Italic);
	AppendMenu(hFormattxtMenu, MF_STRING, MENU_TXT_UNDERLINE, Menu_Format_Underlined_Underlined);
	AppendMenu(hFormattxtMenu, MF_STRING, MENU_TXT_DURCHGESTRICHEN, Menu_Format_Underlined_Striketout);


	AppendMenu(hPOPMenu, MF_POPUP, (UINT_PTR)hColorSubMenu, Menu_Format_Colors);
	AppendMenu(hColorSubMenu, MF_STRING, MENU_COLORRED, Menu_Format_Colors_Red);
	AppendMenu(hColorSubMenu, MF_STRING, MENU_COLORGREEN, Menu_Format_Colors_Green);
	AppendMenu(hColorSubMenu, MF_STRING, MENU_COLORBLUE, Menu_Format_Colors_Blue);
	AppendMenu(hColorSubMenu, MF_STRING, MENU_COLORYELLOW, Menu_Format_Colors_Yellow);
	AppendMenu(hColorSubMenu, MF_STRING, MENU_COLORBLACK, Menu_Format_Colors_Black);
	AppendMenu(hPOPMenu, MF_STRING, MENU_TXTCOLOR, Menu_Format_Colors_Color);

	TrackPopupMenu(hPOPMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hEdit, NULL);
	DestroyMenu(hPOPMenu);
}
//GetSet
void getscreensize(HWND hWnd) {
	RECT rect;
	GetWindowRect(hWnd, &rect);

	x = rect.right - rect.left;
	y = rect.bottom - rect.top;

	xpos = rect.left;
	ypos = rect.top;

}
void writeinifile() {

	std::stringstream xstring;
	xstring << x;
	std::string xsize = xstring.str();

	std::stringstream ystring;
	ystring << y;
	std::string ysize = ystring.str();

	std::stringstream xposstring;
	xposstring << xpos;
	std::string xposini = xposstring.str();

	std::stringstream yposstring;
	yposstring << ypos;
	std::string yposini = yposstring.str();

	std::stringstream searchposx;
	searchposx << xpossearchwnd;
	std::string xpossearch = searchposx.str();

	std::stringstream searchposy;
	searchposy << ypossearchwnd;
	std::string ypossearch = searchposy.str();
	
	std::stringstream lastfileextensionsavedstr;
	lastfileextensionsavedstr << lastfileextensionsaved;
	std::string lastfileextensionsavedout = lastfileextensionsavedstr.str();

	std::stringstream lastfileextensionOpenedstr;
	lastfileextensionOpenedstr << lastfileextensionOpened;
	std::string lastfileextensionOpenedout = lastfileextensionOpenedstr.str();
	
	std::stringstream lastlangpackcountstr;
	lastfileextensionOpenedstr << lastlangpackcount;
	std::string lastlangpackcountout = lastfileextensionOpenedstr.str();

	std::stringstream registeredstr;
	registeredstr << registered;
	std::string registeredout = registeredstr.str();

	WritePrivateProfileString("screen", "X", (LPCTSTR)xsize.c_str(), inipath); //size x
	WritePrivateProfileString("screen", "Y", (LPCTSTR)ysize.c_str(), inipath); //size y
	WritePrivateProfileString("screen", "XPOS", (LPCTSTR)xposini.c_str(), inipath); //pos x
	WritePrivateProfileString("screen", "YPOS", (LPCTSTR)yposini.c_str(), inipath); //pos y
	WritePrivateProfileString("search", "XPOS", (LPCTSTR)xpossearch.c_str(), inipath); //pos x searchwnd
	WritePrivateProfileString("search", "YPOS", (LPCTSTR)ypossearch.c_str(), inipath); //pos y searchwnd
	WritePrivateProfileString("Dialogs", "FileExtensionSave", (LPCTSTR)lastfileextensionsavedout.c_str(), inipath);
	WritePrivateProfileString("Dialogs", "FileExtensionOpen", (LPCTSTR)lastfileextensionOpenedout.c_str(), inipath);
	WritePrivateProfileString("Dialogs", "LastSavedPath", lastSavedPath, inipath);
	WritePrivateProfileString("Preferences", "Recentlangpackscount", lastlangpackcountout.c_str(), inipath);
	WritePrivateProfileString("screen", "registered", registeredout.c_str(), inipath);
}
void readinifile() {

	x = ReadINI_Int("screen", "X", NULL); //screensize x
	y = ReadINI_Int("screen", "Y", NULL); //screensize y
	xpos = ReadINI_Int("screen", "XPOS", NULL); //screenpos x
	ypos = ReadINI_Int("screen", "YPOS", NULL); //screenpos y
	xpossearchwnd = ReadINI_Int("search", "XPOS", NULL);
	ypossearchwnd = ReadINI_Int("search", "YPOS", NULL);
	lastfileextensionsaved = ReadINI_Int("Dialogs", "FileExtensionSave", 0);
	lastfileextensionOpened = ReadINI_Int("Dialogs", "FileExtensionOpen", 0);
	GetPrivateProfileString("Packs", "Language1", "NULL", langpack1, 500, inipath);
	GetPrivateProfileString("Dialogs", "LastOpenedPath", "NULL", lastOpenedPath, 500, inipath);
	GetPrivateProfileString("Dialogs", "LastSavedPath", "NULL", lastSavedPath, 500, inipath);
	lastlangpackcount = ReadINI_Int("Preferences", "Recentlangpackscount", 1);
	GetPrivateProfileString("Preferences", "Language1", "NULL", langpack1,500,inipath);
	GetPrivateProfileString("Preferences", "Language2", "NULL", langpack2, 500, inipath);
	GetPrivateProfileString("Preferences", "Language3", "NULL", langpack3, 500, inipath);

	autosavetime = ReadINI_Int("Preferences", "AutosaveTime", 60); //1min = 60sec = 60 000ms
	doautosave = ReadINI_Int("Preferences", "DoAutoSave", 0); //

	registered = ReadINI_Int("screen", "registered", 0);
	if (strcmp(langpack1, "NULL") == 0 || strcmp(langpack1, "") == 0) {
		MessageBox(NULL, "Pleas load a Language Pack", "Error", MB_OK);
		OpenlangPack();
	}

	////////////Init/////////////
	if (x < 1) { x = 300; }
	if (x > 0 && x < 136) { x = 300; }
	if (y < 1) { y = 300; }
	if (y > 0 && y < 40) { y = 300; }
	if (ypos > screenY - 50 || ypos < 0) { ypos = 50; }
	if (xpos > screenX - 50 || xpos < 0) { xpos = 50; }
	if (ypossearchwnd > screenY - 100 || ypossearchwnd <0) { ypossearchwnd = 50; }
	if (xpossearchwnd > screenX - 100 || xpossearchwnd <0) { xpossearchwnd = 50; }
	if (autosavetime > 59) {
		autosavetime = 59;
	}
}
void init(HWND hWnd) {
	ShowCursorPos();
}
void loadlibs() {
	relib = LoadLibrary("riched32.dll");
	if (relib == NULL)
		MessageBox(NULL, Error_Couldnt_load_richedit , "Error", MB_ICONEXCLAMATION);
}
void getscreenpos(HWND hWnd) {
	RECT rect;
	GetWindowRect(hWnd, &rect);
	xpos = rect.left;
	ypos = rect.top;
}
void getscreenpossearchwnd() {
	RECT rect;
	GetWindowRect(SearchWND, &rect);
	xpossearchwnd = rect.left;
	ypossearchwnd = rect.top;
}
//Windowmode
void minimodus(HWND hWnd, LPARAM lp) {
	getscreensize(hWnd);
	DestroyWindow(hToolv);
	DestroyWindow(hStatusBar);
	MoveWindow(hWnd, xpos, ypos, 400, 400, TRUE);
	MoveWindow(hEdit, 0, 0, 400, 400, TRUE);
	//MoveWindow(hToolv, 0, 0, 0, 0, TRUE);
	minimodusbool = true;
	CheckMenuItem(hMenu, MENU_MINIMODUS, MF_CHECKED);
	saveminimode = true;
}
void normalmodus(HWND hWnd, LPARAM lp) {
	getscreenpos(hWnd);
	AddToolbarv(hWnd);
	MoveWindow(hWnd, xpos, ypos, x, y, TRUE);
	MoveWindow(hEdit, 0, 36, x, y - 55, TRUE);
	addStatusbar(hWnd);
	MoveWindow(hStatusBar, 0, 0, LOWORD(lp), HIWORD(lp) - 23, TRUE);
	minimodusbool = false;
	CheckMenuItem(hMenu, MENU_MINIMODUS, MF_UNCHECKED);
	saveminimode = false;
}
void wordwrap(HWND hWnd) {
	if (wrapbool == true) {
		SendMessage(hEdit, EM_SETTARGETDEVICE, 0, 1);
		CheckMenuItem(hMenu, MENU_WORDWRAP, MF_UNCHECKED);
		wrapbool = false;
	}
	else {
		CheckMenuItem(hMenu, MENU_WORDWRAP, MF_CHECKED);
		SendMessage(hEdit, EM_SETTARGETDEVICE, 0, 0);
		wrapbool = true;
	}
}
void borderless(HWND hWnd, bool on, LPARAM lp) {
	if (on == true) {
		SetWindowLongPtr(hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
		CheckMenuItem(hMenu, MENU_BORDERLESS, MF_CHECKED);
	}
	else {
		SetWindowLongPtr(hWnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
		CheckMenuItem(hMenu, MENU_BORDERLESS, MF_UNCHECKED);
	}

}
void always_on_top(HWND hWnd) {
	if (alwaysontop == true) {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		alwaysontop = false;
		CheckMenuItem(hMenu, MENU_ALWAYSONTOP1, MF_UNCHECKED);
		savealwaysontop = true;
	}
	else {
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		alwaysontop = true;
		CheckMenuItem(hMenu, MENU_ALWAYSONTOP1, MF_CHECKED);
		savealwaysontop = false;
	}
}
void showToolbar(HWND hWnd, LPARAM lp) {

	getscreensize(hWnd);

	if(showToolBar == true){
		showToolBar = false;
		switchtoolbar = true;
		CheckMenuItem(hMenu, MENU_TOOLBAR, MF_UNCHECKED);
		AddToolbarv(hWnd);
		if (showStatusBar == true) {
			MoveWindow(hEdit, 0, 36, x, y - 60, TRUE);
		}
		else {
			MoveWindow(hEdit, 0, 36, x, y-36, TRUE);
		}
	}
	else {
		showToolBar = true;
		CheckMenuItem(hMenu, MENU_TOOLBAR, MF_CHECKED);
		DestroyWindow(hToolv);
		if (showStatusBar == true) {
			MoveWindow(hEdit, 0, 36, x, y - 60, TRUE);
		}
		else {
			MoveWindow(hEdit, 0, 0, x, y, TRUE);
		}
	}
}
void showStatusbar(HWND hWnd, LPARAM lp) {

	getscreensize(hWnd);

	if (showStatusBar == true) {

		showStatusBar = false;
		CheckMenuItem(hMenu, MENU_STATUSBAR, MF_UNCHECKED);
		addStatusbar(hWnd);

		if (showToolBar == true) {
			MoveWindow(hEdit, 0, 36, x, y - 60, TRUE);
		}
		else {
			MoveWindow(hEdit, 0, 36, x, y - 36, TRUE);
		}
	}
	else {

		showStatusBar = true;
		CheckMenuItem(hMenu, MENU_STATUSBAR, MF_CHECKED);
		DestroyWindow(hStatusBar);

		if (showToolBar == true) {
			MoveWindow(hEdit, 0, 36, x, y - 60, TRUE);
		}
		else {
			MoveWindow(hEdit, 0, 0, x, y, TRUE);
		}
	}
}
void WritePrivateProfileInt(char *main,char* sub, int onoff) {
	std::stringstream onoffstr;
	onoffstr << onoff;
	std::string onoffout = onoffstr.str();
	WritePrivateProfileString(main, sub, (LPCTSTR)onoffout.c_str(), inipath);
}
void Save_Wnd_Modes() {
	//1 = On 0 = Off
	if (saveminimode == false) {
		WritePrivateProfileInt("Main", "Minimode", 1);
	}
	else {
		WritePrivateProfileInt("Main", "Minimode", 0);
	}
	//1 = On 0 = Off
	if (savealwaysontop == true) {
		WritePrivateProfileInt("Main", "Alwaysontop", 1);
	}
	else {
		WritePrivateProfileInt("Main", "Alwaysontop", 0);
	}
}
void SetMinimode(HWND hWnd, LPARAM lp) {
	if (minimodusbool == true) {
		normalmodus(hWnd, lp);
		AddToolbarv(hWnd);
	}
	else {
		DestroyWindow(hToolv);
		MoveWindow(hEdit, 0, 0, LOWORD(lp), HIWORD(lp), TRUE);
		minimodus(hWnd, lp);
	}
}
void Get_Wnd_Modes(HWND hWnd, LPARAM lp) {
	int alwaysontopint = ReadINI_Int("Main", "Alwaysontop", 0);
	int minimodeint = ReadINI_Int("Main", "Minimode", 0);

	if (alwaysontopint == 1) {
		alwaysontop = true;
		always_on_top(hWnd);
	}
	else {
		alwaysontop = false;
		always_on_top(hWnd);
	}

	if (minimodeint == 1) {
		minimodusbool = true;
		SetMinimode(hWnd, lp);
	}
	else {
		minimodusbool = false;
		SetMinimode(hWnd, lp);
	}
}
//RegDLGclasses
void searchdlgclass(HINSTANCE hInst)
{
	WNDCLASSW search = { 0 };
	search.hbrBackground = (HBRUSH)COLOR_WINDOW;
	search.hCursor = LoadCursor(NULL, IDC_ARROW);
	search.hInstance = hInst;
	search.lpszClassName = L"searchDlg";
	search.lpfnWndProc = SearchProc;
	search.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassW(&search);
}
void gotolinedlgclass(HINSTANCE hInst)
{
	WNDCLASSW gotolineclass = { 0 };
	gotolineclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	gotolineclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	gotolineclass.hInstance = hInst;
	gotolineclass.lpszClassName = L"gotolinedlg";
	gotolineclass.lpfnWndProc = gotolineProc;
	gotolineclass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassW(&gotolineclass);
}
void AboutDlgclass(HINSTANCE hInst)
{
	WNDCLASSW aboutdlgclass = { 0 };
	aboutdlgclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	aboutdlgclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	aboutdlgclass.hInstance = hInst;
	aboutdlgclass.lpszClassName = L"aboutdlg";
	aboutdlgclass.lpfnWndProc = AboutProc;
	aboutdlgclass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassW(&aboutdlgclass);
}
void saveDLGclass(HINSTANCE hInst) {
	WNDCLASSW Savedlgclass = { 0 };
	Savedlgclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	Savedlgclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	Savedlgclass.hInstance = hInst;
	Savedlgclass.lpszClassName = L"SpeichernDLG";
	Savedlgclass.lpfnWndProc = SaveProc;
	Savedlgclass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassW(&Savedlgclass);
}
void optionsDLGclass(HINSTANCE hInst) {
	WNDCLASSW Optionsclass = { 0 };
	Optionsclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	Optionsclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	Optionsclass.hInstance = hInst;
	Optionsclass.lpszClassName = L"PrefDLG";
	Optionsclass.lpfnWndProc = PrefProc;
	Optionsclass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassW(&Optionsclass);
}
void CustomMBDLGclass(HINSTANCE hInst) {
	WNDCLASSW MB_Class = { 0 };
	MB_Class.hbrBackground = (HBRUSH)COLOR_WINDOW;
	MB_Class.hCursor = LoadCursor(NULL, IDC_ARROW);
	MB_Class.hInstance = hInst;
	MB_Class.lpszClassName = L"Mbclass";
	MB_Class.lpfnWndProc = MB_Procedure;
	MB_Class.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	RegisterClassW(&MB_Class);
}
//DialogGUI
void searchDlg(HWND hWnd) {
	EnableWindow(hMainWindow, false);
	SearchWND = CreateWindow("searchDlg", MessageBox_Btn_Search, WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, xpossearchwnd, ypossearchwnd, 300, 150, hWnd, NULL, NULL, NULL);
	search_BTN_1 =CreateWindow("Button", MessageBox_Btn_Search, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_FLAT, 200, 10, 80, 20, SearchWND, (HMENU)SEARCH_SEARCH, NULL, NULL);
	search_BTN_2=CreateWindow("Button", MessageBox_Btn_Cancel, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_FLAT, 200, 60, 80, 20, SearchWND, (HMENU)SEARCH_CANCEL, NULL, NULL);
	searchedit = CreateWindow("Edit", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 20, 10, 170, 20, SearchWND, NULL, NULL, NULL);
	setfontofDialogs();
}
void gotolinedlg() {
	char maxlinenumbertxt[500];

	EnableWindow(hMainWindow, false);
	gotolineWND = CreateWindow("gotolinedlg", MessageBox_Title_Go_To, WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 400, 400, 200, 130, NULL, NULL, NULL, NULL);
	gotoline_Edit = CreateWindow("Edit", "", WS_VISIBLE | WS_CHILD | ES_NUMBER | WS_BORDER, 20, 15, 160, 20, gotolineWND, (HMENU)GOTOLINE_EDIT, NULL, NULL);
	gotoline_BTN1 = CreateWindow("Button", MessageBox_Btn_Ok, WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON, 20, 55, 90, 20, gotolineWND, (HMENU)GOTOLINE_GO, NULL, NULL);
	
	CreateWindow("Static", maxlinenumbertxt, WS_VISIBLE | WS_CHILD, 60, 10, 80, 20, gotolineWND, NULL, NULL, NULL);
	setfontofDialogs();

}
void Aboutdlg() {
	wsprintf(Abouttext, "%s\n %s:Julius Kirsch\n %s:Julius Kirsch", About_First, About_Second,About_Third);
	EnableWindow(hMainWindow, false);
	AboutWND = CreateWindow("aboutdlg", "", WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, 300, 300, 240, 150, NULL, NULL, NULL, NULL);
	about_static = CreateWindow("Static", Abouttext, WS_VISIBLE | WS_CHILD, 20, 15, 270, 65, AboutWND, (HMENU)GOTOLINE_EDIT, NULL, NULL);
	about_BTN_1 = CreateWindow("Button", MessageBox_Btn_Ok, WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON, 130, 80, 70, 20, AboutWND, (HMENU)BTN_ABOUT_OK, NULL, NULL);
	setfontofDialogs();

}
void savedlg(HWND hWnd, char* MB_txt,char * headline) {
	EnableWindow(hMainWindow, false);
	DestroyWindow(hSaveWND);
	int lenght=strlen(MB_txt);
	int windowheight = 150;
	int btn_Pos_1 = 85;
	int btn_Pos_2 = 85;
	int btn_Pos_3 = 85;
	int txt_height = 100;

	if (lenght >= 80) {
		btn_Pos_1 = 115;
		btn_Pos_2 = 115;
		btn_Pos_3 = 115;
		windowheight = 180;
		txt_height = 130;
	}

	hSaveWND = CreateWindow("SpeichernDLG", "", WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 400, 400, 330, windowheight, hWnd, NULL, NULL, NULL);
	Save_Text = CreateWindow("Static", MB_txt, WS_CHILD | WS_VISIBLE, 20, 20, 270, txt_height, hSaveWND, NULL, NULL, NULL);

	Save_BTN = CreateWindow("Button", MessageBox_Btn_Save, WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON, 6, btn_Pos_1,90, 20, hSaveWND, (HMENU)SAVE_BTN_SAVE, NULL, NULL);
	Save_NotSave_BTN = CreateWindow("Button", MessageBox_Btn_Not_Save, WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON, 97, btn_Pos_2, 118, 20, hSaveWND, (HMENU)SAVE_BTN_NOTSAVE, NULL, NULL);
	Save_Cancel_BTN = CreateWindow("Button", MessageBox_Btn_Cancel, WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON, 216, btn_Pos_3, 90, 20, hSaveWND, (HMENU)SAVE_BTN_CANCEL, NULL, NULL);
	
	setfontofDialogs();

}
void Preferencedlg(HWND hWnd) {
	readinifile();
	EnableWindow(hMainWindow, false);
	getsettingsini();

	int movexspace=40;
	int moveyspace=0;
	int size =20;

	prefWND = CreateWindow("PrefDLG", "", WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 300, 300, 600, 250, hWnd, NULL, NULL, NULL);
	pref_Ok_BTN = CreateWindow("Button", MessageBox_Btn_Ok, WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON, 420, 180, 60, 20, prefWND, (HMENU)PREF_BTN_OK, NULL, NULL);
	pref_Cancel_BTN = CreateWindow("Button", MessageBox_Btn_Cancel, WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON, 490, 180, 90, 20, prefWND, (HMENU)PREF_BTN_CANCEL, NULL, NULL);
	////////////Fontborder Top
	pref_static1 = CreateWindow("Static", MessageBox_Pref_SpaceTop_txt, WS_VISIBLE | ES_RIGHT | WS_CHILD, 10, 20 + moveyspace, 120+ size, 20, prefWND, NULL, NULL, NULL);
	prefEditTop = CreateWindow("Edit", "0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_RIGHT, 150 + movexspace, 20 + moveyspace, 60, 20, prefWND, NULL, NULL, NULL);
	pref_static2 = CreateWindow("Static", "px", WS_VISIBLE | WS_CHILD, 220 + movexspace, 20 + moveyspace, 20, 20, prefWND, NULL, NULL, NULL);
	////////////Fontborder Bottom
	pref_static3 = CreateWindow("Static", MessageBox_Pref_SpaceBottom_txt, WS_VISIBLE | ES_RIGHT | WS_CHILD, 10, 40 + moveyspace, 120+ size, 20, prefWND, NULL, NULL, NULL);
	prefEditButtom = CreateWindow("Edit", "0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_RIGHT,		150 + movexspace, 40 + moveyspace, 60, 20, prefWND, NULL, NULL, NULL);
	pref_static4 = CreateWindow("Static", "px", WS_VISIBLE | WS_CHILD,					220 + movexspace, 40 + moveyspace, 20, 20, prefWND, NULL, NULL, NULL);
	////////////Fontborder Left
	pref_static5 = CreateWindow("Static", MessageBox_Pref_SpaceLeft_txt, WS_VISIBLE | ES_RIGHT | WS_CHILD, 10 , 60 + moveyspace, 120+ size, 20, prefWND, NULL, NULL, NULL);
	prefEditLeft = CreateWindow("Edit", "0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_RIGHT, 150 + movexspace, 60 + moveyspace, 60, 20, prefWND, NULL, NULL, NULL);
	pref_static6 = CreateWindow("Static", "px", WS_VISIBLE | WS_CHILD, 220 + movexspace, 60 + moveyspace, 20, 20, prefWND, NULL, NULL, NULL);
	////////////Fontborder Right
	pref_static7 = CreateWindow("Static", MessageBox_Pref_SpaceRight_txt, WS_VISIBLE |ES_RIGHT| WS_CHILD, 10, 80 + moveyspace, 130 + size, 20, prefWND, NULL, NULL, NULL);
	prefEditRight = CreateWindow("Edit", "0", WS_VISIBLE | WS_BORDER | WS_CHILD | ES_NUMBER | ES_RIGHT, 150 + movexspace, 80 + moveyspace, 60, 20, prefWND, NULL, NULL, NULL);
	pref_static8 = CreateWindow("Static", "px", WS_VISIBLE | WS_CHILD, 220 + movexspace, 80 + moveyspace, 20, 20, prefWND, NULL, NULL, NULL);
	
	pref_static11 = CreateWindow("Static", "Zuletzt:", WS_VISIBLE | WS_CHILD | ES_LEFT, 320, 2, 60, 20, prefWND, NULL, NULL, NULL);
	pref_OpenLangPack_BTN = CreateWindow("Button", MessageBox_Pref_BTN_LoadLangPack, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT, 370, 80, 150, 20, prefWND, (HMENU)PREF_BTN_OPENLANGPACK, NULL, NULL);
	
	pref_static9 = CreateWindow("Static", MessageBox_pref_DialogFontsize_txt, WS_VISIBLE | WS_CHILD | ES_RIGHT, 10 , 110 + moveyspace, 130 + size, 20, prefWND, NULL, NULL, NULL);
	prefEditFontsizeGUI = CreateWindow("Edit", "17", WS_VISIBLE | WS_CHILD | WS_BORDER|ES_NUMBER | ES_RIGHT, 150 + movexspace, 110 + moveyspace, 60, 20, prefWND, NULL, NULL, NULL);
	pref_static10 = CreateWindow("Static", "px", WS_VISIBLE | WS_CHILD, 220 + movexspace, 110 + moveyspace, 20, 20, prefWND, NULL, NULL, NULL);

	pref_static12 = CreateWindow("Static", MessageBox_pref_AutoSave_After_txt, WS_VISIBLE | ES_RIGHT | WS_CHILD, 10, 140 + moveyspace, 130 + size, 20, prefWND, NULL, NULL, NULL);
	prefEditAutoSaveTime = CreateWindow("Edit", "6", WS_VISIBLE | WS_BORDER | WS_CHILD | ES_NUMBER | ES_RIGHT, 150 + movexspace, 140 + moveyspace, 60, 20, prefWND, NULL, NULL, NULL);
	pref_static13 = CreateWindow("Static", "min", WS_VISIBLE | WS_CHILD, 220 + movexspace, 140 + moveyspace, 30, 20, prefWND, NULL, NULL, NULL);
	pref_CheckBox1 = CreateWindow("Button", MessageBox_pref_AutoSave_txt, WS_VISIBLE | WS_CHILD | BS_FLAT | BS_CHECKBOX, 320, 110, 200, 20, prefWND, (HMENU)PREF_CHECKBOX_AUTOSAVE, NULL, NULL);

	setfontofDialogs();
	addListbox(prefWND);
	ReadLanguagenamefromPack(langpack1);
	addStringListBox(prefLanguageList, Main_Lang);
	ReadLanguagenamefromPack(langpack2);
	addStringListBox(prefLanguageList, Main_Lang);
	ReadLanguagenamefromPack(langpack3);
	addStringListBox(prefLanguageList, Main_Lang);

	writelangpackstoini();
	setpreftowindow();
}

//DialogFunctions
void gotoline() {

	UINT LineNo = 0;
	CHARRANGE chrg;
	DWORD max;
	BOOL bSuccess;

	LineNo = GetDlgItemInt(gotolineWND, GOTOLINE_EDIT, &bSuccess, FALSE);

	max = SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);
	if (LineNo == NULL) {
		MessageBox(gotolineWND, "Please enter a number", "Error", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (max > LineNo)
	{
		LineNo = SendMessage(hEdit, EM_LINEINDEX, LineNo, 0);
		LineNo--;
		SendMessage(hEdit, EM_SETSEL, LineNo, LineNo);
		SetFocus(hEdit);
		ShowCursorPos();
		DestroyWindow(gotolineWND);
		EnableWindow(hMainWindow, true);
		SetActiveWindow(hMainWindow);
	}
	else {
		MessageBox(gotolineWND, "This line doen't exist", "Error", MB_OK | MB_ICONINFORMATION);
	}
}
void dosearch(HWND hWnd) {
	SendMessage(hEdit, EM_SETSEL, 0, 0);
	UINT uFlags;
	uFlags = 0;
	SendMessage(hEdit, EM_EXGETSEL, 0, (LPARAM)&findtext.chrg);
	GetWindowText(searchedit, (LPSTR)&FindBuffer, sizeof(FindBuffer));

	LONG t;
	uFlags = FR_DOWN;
	if (findtext.chrg.cpMin)
	{
		LONG t;
		t = findtext.chrg.cpMax;
		findtext.chrg.cpMin = t;
	}
	else findtext.chrg.cpMax = -1;

	uFlags = uFlags | FR_MATCHCASE;
	findtext.lpstrText = FindBuffer;

	if (-1 != SendMessage(hEdit, EM_FINDTEXTEX, uFlags, (LPARAM)(FINDTEXT FAR *) &findtext))
		SendMessage(hEdit, EM_EXSETSEL, 0, (LPARAM)(CHARRANGE FAR *) &findtext.chrgText);
	else {
		MessageBox(SearchWND, "Coulnt found!", "Error", MB_ICONINFORMATION | MB_OK);
	}
}

//Prefereces
void writesettingsini() {
	char spaceleftfromedit[500];
	char spacerightfromedit[500];
	char spacetopfromedit[500];
	char spacebuttomfromedit[500];
	char fontsizeint[4];
	char autosavetime[10];

	GetWindowText(prefEditLeft, spaceleftfromedit, 500);
	GetWindowText(prefEditRight, spacerightfromedit, 500);
	GetWindowText(prefEditTop, spacetopfromedit, 500);
	GetWindowText(prefEditButtom, spacebuttomfromedit, 500);
	GetWindowText(prefEditFontsizeGUI, fontsizeint, 4);
	GetWindowText(prefEditFontsizeGUI, fontsizeint, 4);
	GetWindowText(prefEditAutoSaveTime, autosavetime, 10);

	WritePrivateProfileString("Preferences", "LeftSpace", (LPCTSTR)spaceleftfromedit, inipath); //Linker Rand
	WritePrivateProfileString("Preferences", "RightSpace", (LPCTSTR)spacerightfromedit, inipath); //Rechter Rand
	WritePrivateProfileString("Preferences", "TopSpace", (LPCTSTR)spacetopfromedit, inipath); //Oberer Rand
	WritePrivateProfileString("Preferences", "BottomSpace", (LPCTSTR)spacebuttomfromedit, inipath); //Unterer Rand
	WritePrivateProfileString("Preferences", "DLGFontsize", (LPCTSTR)fontsizeint, inipath); //Schriftgröße der Dialogschrift
	WritePrivateProfileString("Preferences", "AutosaveTime", (LPCTSTR)autosavetime, inipath); //Autospeichern Zeit
	if (autosavecheckboxsave == true) { WritePrivateProfileInt("Preferences", "DoAutosave", 1); }
	if (autosavecheckboxsave == false) { WritePrivateProfileInt("Preferences", "DoAutosave", 0); }

}
void getsettingsini() {
	int autosaveint = 0;
	leftspaceofedit = ReadINI_Int("Preferences", "LeftSpace", NULL);
	rightspaceofedit = ReadINI_Int("Preferences", "RightSpace", NULL);
	topspaceofedit = ReadINI_Int("Preferences", "TopSpace", NULL);
	bottomspaceofedit = ReadINI_Int("Preferences", "BottomSpace", NULL);
	DialogFontsize = ReadINI_Int("Preferences", "DLGFontsize", 17);
	autosavetime = ReadINI_Int("Preferences", "AutosaveTime", 6);
	autosaveint = ReadINI_Int("Preferences", "doAutosave", 0);

	if (autosaveint == 1) {
		autosavecheckboxsave = true;
	}
	if (autosaveint == 0) {
		autosavecheckboxsave = false;
	}
	if (leftspaceofedit > 100) {
		leftspaceofedit = 100;
	}
	if (rightspaceofedit > 100) {
		rightspaceofedit = 100;
	}
	if (topspaceofedit > 100) {
		topspaceofedit = 100;
	}
	if (bottomspaceofedit > 100) {
		bottomspaceofedit = 100;
	}
	if (DialogFontsize < 10) {
		DialogFontsize = 10;
	}
	if (DialogFontsize > 20) {
		DialogFontsize = 20;
	}
	if (autosavetime > 59) {
		autosavetime = 59;
	}
	if (autosavetime < 1) {
		autosavetime = 1;
	}
}
void setpreftowindow() {
	char space1[_MAX_PATH];
	char space2[_MAX_PATH];
	char space3[_MAX_PATH];
	char space4[_MAX_PATH];
	char fontsizeofDLG[_MAX_PATH];
	char AutoSaveTime[_MAX_PATH];

	wsprintf(space1, "%d", leftspaceofedit);
	wsprintf(space2, "%d", rightspaceofedit);
	wsprintf(space3, "%d", topspaceofedit);
	wsprintf(space4, "%d", bottomspaceofedit);
	wsprintf(fontsizeofDLG, "%d", DialogFontsize);
	wsprintf(AutoSaveTime, "%d", autosavetime);

	SetWindowText(prefEditButtom, space4);
	SetWindowText(prefEditTop, space3);
	SetWindowText(prefEditRight, space2);
	SetWindowText(prefEditLeft, space1);
	SetWindowText(prefEditFontsizeGUI, fontsizeofDLG);
	SetWindowText(prefEditAutoSaveTime, AutoSaveTime);
	if (autosavecheckboxsave == true) {
		CheckDlgButton(prefWND, PREF_CHECKBOX_AUTOSAVE, MF_CHECKED);
	}
	else {
		CheckDlgButton(prefWND, PREF_CHECKBOX_AUTOSAVE, MF_UNCHECKED);
	}
}
void SetRectOfEdit() {
	RECT rc;
	SendMessage(hEdit, EM_GETRECT, 0, (LPARAM)&rc);
	rc.left += leftspaceofedit;
	rc.top += topspaceofedit;
	rc.right -= rightspaceofedit;
	rc.bottom -= bottomspaceofedit;
	SendMessage(hEdit, EM_SETRECT, 0, (LPARAM)&rc);
}
void writelangpackstoini() {
	WritePrivateProfileString("Preferences", "Language1", langpack1, inipath);
	WritePrivateProfileString("Preferences", "Language2", langpack2, inipath);
	WritePrivateProfileString("Preferences", "Language3", langpack3, inipath);
}
void displaylangPack(char * langpackin) {

	lastlangpackcount++;

	if (lastlangpackcount > 3) {
		lastlangpackcount = 1;
	}

	if (lastlangpackcount == 1) {
		wsprintf(langpack1, langpackin);
		ReadLanguagePacket(langpack1);
		wsprintf(langpack1_1, Main_Lang);
	}
	if (lastlangpackcount == 2) {
		wsprintf(langpack2, langpackin);
		ReadLanguagePacket(langpack2);
		wsprintf(langpack2_1, Main_Lang);
	}
	if (lastlangpackcount == 3) {
		wsprintf(langpack3, langpackin);
		ReadLanguagePacket(langpack3);
		wsprintf(langpack3_1, Main_Lang);
	}
	writelangpackstoini();
}
void OpenlangPack() {
	char LangPackiniPath[_MAX_PATH];
	wsprintf(LangPackiniPath,"%s\\felang", projectfolderpath);

	OPENFILENAME ofn;       // common dialog box structure
							// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hMainWindow;
	ofn.lpstrFile = langpack1;
	ofn.nMaxFile = sizeof(langpack1);
	ofn.lpstrFilter = "Fastedit language pack(.felang)\0 *.felang";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = LangPackiniPath;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrTitle = Open_felangFile_DLG_Title;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		displaylagPackBool = true;
		displaylangPack(ofn.lpstrFile);
	}
	else {
		displaylagPackBool = false;
		return;
	}

}

//Other...
void getprojectfolderpath() {

	GetModuleFileName(NULL, projectfolderpath, MAX_PATH);
	PathRemoveFileSpec(projectfolderpath);

}
void setinipath(){

	char filename[_MAX_PATH];
	wsprintf(filename, projectfolderpath);

	wsprintf(inipath, TEXT("%s\\%s"), filename, iniName);
}
void opennewEditor(HWND hWnd) {
	TCHAR pathofexe[_MAX_PATH];
	ypos = ypos + 10;
	xpos = xpos + 10;
	writeinifile();
	GetModuleFileName(NULL, pathofexe, MAX_PATH);
	WinExec(pathofexe, SW_SHOW);
	ypos = ypos - 10;
	xpos = xpos - 10;
	writeinifile();
}
void writeFilenameinTitle(char *file) {
	if (strcmp(file, "") != 0) {
		SetWindowText(hMainWindow, file);
		lastopenedpath = file;
	}
}
void Shortcuts() {
	char outtext[_MAX_PATH];
	char outpath[_MAX_PATH];
	wsprintf(outpath, "%s\\htm\\Shortcuts.htm", projectfolderpath);
	wsprintf(outtext, "%s %s", Error_Faild_to_Open, outpath);

	if ((int)ShellExecute(NULL, "open", outpath, NULL, NULL, SW_SHOWNORMAL) <= 32) {
		MessageBox(hMainWindow, outtext, Error_Fastedit_Error, MB_OK);
	}
	else {
		return;
	}
}
void Changelog() {
	char outtext[_MAX_PATH];
	char outpath[_MAX_PATH];
	wsprintf(outpath, "%s\\htm\\Changelog.htm", projectfolderpath);
	wsprintf(outtext, "%s %s", Error_Faild_to_Open, outpath);

	if ((int)ShellExecute(NULL, "Open", outpath, NULL, NULL, SW_SHOWNORMAL) <= 32) {
		
		MessageBox(hMainWindow, outtext, Error_Fastedit_Error, MB_OK);
	}
	else {
		return;
	}
	
}
void registerchangefont() {
	GUITXTFont = CreateFont(DialogFontsize, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"));
}

//Updates
void ShowCursorPos()
{
	CHARRANGE chrg;
	LRESULT Line;
	LRESULT Column;
	CHAR	line[30];
	char lineoutput[_MAX_PATH];
	int zoom = 0;
	//int EditZoom = hEditzoom();
	// Get the line number.
	SendMessage(hEdit, EM_EXGETSEL, 0, (LPARAM)&chrg);
	Line = SendMessage(hEdit, EM_EXLINEFROMCHAR, 0, (LPARAM)chrg.cpMin);

	// Get the column
	Column = chrg.cpMin - SendMessage(hEdit, EM_LINEINDEX, (WPARAM)Line, 0);
	LRESULT Selendout = SendMessage(hEdit, EM_LINELENGTH, 0,(LPARAM)chrg.cpMin);

	if (needsave == true) {
		wsprintf(line, "Zeile: %d Spalte: %d", Line + 1, Column + 1);
		wsprintf(lineoutput, "%s                |                %s                |                %d", line, Statusbar_Not_Saved, zoom);
	}
	if (needsave == false) {
		wsprintf(line, "Zeile: %d Spalte: %d", Line + 1, Column + 1);

		wsprintf(lineoutput, "%s                |                %s                |                %d", line, Statusbar_Saved, zoom);
	}

	UpdateStatusBar(lineoutput, 0);
} 
void UpdateStatusBar(LPSTR lpszStatusString, WORD partNumber){
	SendMessage(hStatusBar, SB_SETTEXT, partNumber | SBT_POPOUT,
		(LPARAM)lpszStatusString);
}
void setfontofDialogs() {
	registerchangefont();
	SendMessage(search_BTN_1, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(searchedit, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(search_BTN_2, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));

	SendMessage(gotoline_Edit, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(gotoline_BTN1, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));

	SendMessage(about_static, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(about_BTN_1, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));

	SendMessage(Save_Text, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(Save_BTN, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(Save_NotSave_BTN, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(Save_Cancel_BTN, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));


	SendMessage(pref_Cancel_BTN, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_Ok_BTN, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(prefEditTop, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(prefEditLeft, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(prefEditButtom, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(prefEditRight, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_static1, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_static2, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_static3, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_static4, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_static5, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_static6, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_static7, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_static8, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_static9, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_static10, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(prefEditFontsizeGUI, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_OpenLangPack_BTN, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_static11, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));	
	SendMessage(prefEditAutoSaveTime, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_static12, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_static13, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
	SendMessage(pref_CheckBox1, WM_SETFONT, (WPARAM)GUITXTFont, MAKELPARAM(TRUE, 0));
}

void autosave(HWND hWnd) {
	char outtext[2000];
	int outint = autosavetime * 60000;

	SetTimer(hWnd,
		AUTO_SAVE_TIMER,
		autosavetime * 60000, //  1 min / 60000 = 1 millisecond
		(TIMERPROC)NULL);

	wsprintf(outtext, "%d", outint);
}
void checktoAutosave(HWND hWnd) {
	if (autosavecheckboxsave == true) {
		autosave(hWnd);
	}
	else {
		KillTimer(hWnd, AUTO_SAVE_TIMER);
	}
}

//////////////////////////////////////////////////////
//////////////////////Test////////////////////////////


LRESULT CALLBACK PrefProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {

	case WM_CLOSE: {
		DestroyWindow(prefWND);
		EnableWindow(hMainWindow, true);
		SetActiveWindow(hMainWindow);
		break;
	}

	case WM_COMMAND: {
		switch (wp) {
		case PREF_BTN_CANCEL: {
			DestroyWindow(prefWND);
			EnableWindow(hMainWindow, true);
			SetActiveWindow(hMainWindow);
			break;
		}
		case PREF_BTN_OK: {
			MessageBox(hWnd, MessageBox_Pref_Message, MessageBox_Title_Info, MB_OK);
			writesettingsini();
			DestroyWindow(prefWND);
			EnableWindow(hMainWindow, true);
			SetActiveWindow(hMainWindow);
			break;
		}
		case PREF_BTN_OPENLANGPACK: {
			OpenlangPack();
			CreateWindow("Static", deLanguagePackPath, WS_VISIBLE | WS_CHILD, 20, 105, 380, 70, prefWND, NULL, NULL, NULL);
			break;
		}
		case PREF_CHECKBOX_AUTOSAVE: {
			if (autosavecheckbox == true) {
				CheckDlgButton(prefWND,PREF_CHECKBOX_AUTOSAVE,MF_UNCHECKED);
				autosavecheckbox = false;
				autosavecheckboxsave = false;
				KillTimer(hWnd, AUTO_SAVE_TIMER);
			}
			else {
				CheckDlgButton(prefWND, PREF_CHECKBOX_AUTOSAVE, MF_CHECKED);
				autosavecheckbox = true;
				autosavecheckboxsave = true;
				autosave(hWnd);
			}
			break;
		}
		}
		break;
	}
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}
LRESULT CALLBACK SaveProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {

	case WM_CLOSE: {
		DestroyWindow(hSaveWND);
		EnableWindow(hMainWindow, true);
		SetActiveWindow(hMainWindow);
		break;
	}
	case WM_COMMAND: {
		switch (wp) {
		case SAVE_BTN_SAVE: {

			if (action_on_Dialog == 2) {
				save_only(hWnd);

				if (save_open_sucess == true) {
					SetWindowText(hEdit, "");
					nonamefile();
					neversave = true;
				}
				else {
					DestroyWindow(hSaveWND);
					EnableWindow(hMainWindow, true);
					SetActiveWindow(hMainWindow);
				}

			} //New File
			if (action_on_Dialog == 3) { 
				save_only(hWnd); 
				if (save_open_sucess == true) {
					IOstream::open_file(hWnd);
				}
				else {
					DestroyWindow(hSaveWND);
					EnableWindow(hMainWindow, true);
					SetActiveWindow(hMainWindow);
				}
			
			} //Open
			//Drag and Drop
			if (action_on_Dialog == 4) {
				save_only(hWnd);
				if (save_open_sucess == true) {
					if (rtfopen == true) {
						IOstream::openrtf(openfile);
					}
					if (rtfopen == false) {
						IOstream::OpentxtFile(openfile);
					}
				}
				else {
					DestroyWindow(hSaveWND);
					EnableWindow(hMainWindow, true);
					SetActiveWindow(hMainWindow);
				}
				

			}	
			if (action_on_Dialog == 5) {
				save_only(hWnd);
				if (save_open_sucess == true) {
					SetWindowText(hEdit, "");
				}
				else {
					DestroyWindow(hSaveWND);
					EnableWindow(hMainWindow, true);
					SetActiveWindow(hMainWindow);
				}

			} //Close File

				DestroyWindow(hSaveWND);
				EnableWindow(hMainWindow, true);
				SetActiveWindow(hMainWindow);
			break;
		}
		case SAVE_BTN_NOTSAVE: {
			if (action_on_Dialog == 1) {}
			if (action_on_Dialog == 2) { SetWindowText(hEdit, ""); nonamefile(); neversave = true; } //New File
			if (action_on_Dialog == 3) { IOstream::open_file(hWnd); }//Open
			//Drag and Drop
			if (action_on_Dialog == 4) {
				if (rtfopen == true) {
					IOstream::openrtf(openfile);
				}
				if (rtfopen == false) {
					IOstream::OpentxtFile(openfile);
				}
			}
			if (action_on_Dialog == 5) { SetWindowText(hEdit, ""); }//Close File
			
			DestroyWindow(hSaveWND);
			EnableWindow(hMainWindow, true);
			SetActiveWindow(hMainWindow);
			break;
		}
		case SAVE_BTN_CANCEL: {
			DestroyWindow(hSaveWND);
			EnableWindow(hMainWindow, true);
			SetActiveWindow(hMainWindow);
			return 0;
		}
		}
		break;
	}

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}
LRESULT CALLBACK AboutProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_COMMAND: {
		switch (wp)
		{
		case BTN_ABOUT_OK: {
			DestroyWindow(AboutWND);
			EnableWindow(hMainWindow, true);
			SetActiveWindow(hMainWindow);
			break;
		}
		}
		break;
	}
	case WM_CLOSE: {
		DestroyWindow(AboutWND);
		EnableWindow(hMainWindow, true);
		SetActiveWindow(hMainWindow);
		break;
	}
	case WM_KEYDOWN: {
		if (wp == VK_ESCAPE) {
			DestroyWindow(AboutWND);
			EnableWindow(hMainWindow, true);
			SetActiveWindow(hMainWindow);
		}
	}
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}
LRESULT CALLBACK gotolineProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg)
	{
	case WM_COMMAND: {
		switch (wp)
		{
		case GOTOLINE_GO: {
			gotoline();

			break;
		}
		}
		break;
	}
	case WM_CLOSE: {
		DestroyWindow(gotolineWND);
		EnableWindow(hMainWindow, true);
		SetActiveWindow(hMainWindow);
		break;
	}
	case WM_KEYDOWN: {
		if (wp == VK_ESCAPE) {
			DestroyWindow(gotolineWND);
			EnableWindow(hMainWindow, true);
			SetActiveWindow(hMainWindow);
		}
		break;
	}
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}
LRESULT CALLBACK SearchProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND: {
		switch (wp)
		{
		case SEARCH_CANCEL: {
			getscreenpossearchwnd();
			DestroyWindow(hWnd);
			EnableWindow(hMainWindow, true);
			SetActiveWindow(hMainWindow);
			writeinifile();
			break;
		}
		case SEARCH_SEARCH: {
			dosearch(hWnd);
			getscreenpossearchwnd();
			DestroyWindow(hWnd);
			EnableWindow(hMainWindow, true);
			SetActiveWindow(hMainWindow);
			writeinifile();
			break;
		}
		}
		break;
	}
	case WM_KEYDOWN: {
		if (wp == VK_ESCAPE) {
			getscreenpossearchwnd();
			writeinifile();
			DestroyWindow(hWnd);
			EnableWindow(hMainWindow, true);
			SetActiveWindow(hMainWindow);
		}
		break;
	}
	case WM_CLOSE: {
		getscreenpossearchwnd();
		writeinifile();
		DestroyWindow(hWnd);
		EnableWindow(hMainWindow, true);
		SetActiveWindow(hMainWindow);
		break;
	}
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}
LRESULT CALLBACK EditWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp){
	
	switch (msg) {

	case WM_CHAR: {
		if (rainbowcolor == true) {
			rainbowcount = rainbowcount + 1;
			if (rainbowcount > 7) {
				rainbowcount = 1;
			}
			rich_edit::writerainbowcolor();
		}
		break;
	}
	case WM_RBUTTONDOWN: {
		PopuMenu(lp);
		break;
	}
	case WM_COMMAND: {
		switch (wp) {
		case MENU_COPY: {
			SendMessage(hEdit, WM_COPY, 0, 0);
			break;
		}
		case MENU_PASTE: {
			SendMessage(hEdit, WM_PASTE, 0, 0);
			break;
		}
		case MENU_CUT: {
			SendMessage(hEdit, WM_CUT, 0, 0);
			break;
		}
		case MENU_DELETE: {
			SendMessage(hEdit, EM_REPLACESEL, 0, 0);
			break;
		}
		case MENU_SELALL: {
			SendMessage(hEdit, EM_SETSEL, 0, -1);
			break;
		}
		case MENU_COLORBLACK: {
			rich_edit::colorseltext(hWnd, RGB(0, 0, 0));
			break;
		}
		case MENU_COLORBLUE: {
			rich_edit::colorseltext(hWnd, RGB(0, 0, 255));
			break;
		}
		case MENU_COLORGREEN: {
			rich_edit::colorseltext(hWnd, RGB(0, 255, 0));
			break;
		}
		case MENU_COLORRED: {
			rich_edit::colorseltext(hWnd, RGB(255, 0, 0));
			break;
		}
		case MENU_COLORYELLOW: {
			rich_edit::colorseltext(hWnd, RGB(255, 255, 0));
			break;
		}
		case MENU_TXTCOLOR: {
			CHOOSECOLOR cc;
			static COLORREF acrCustClr[32];
			HBRUSH hbrush;
			static DWORD rgbCurrent;


			ZeroMemory(&cc, sizeof(cc));
			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = hWnd;
			cc.lpCustColors = (LPDWORD)acrCustClr;
			cc.rgbResult = rgbCurrent;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;

			if (ChooseColor(&cc) == TRUE) {
				hbrush = CreateSolidBrush(cc.rgbResult);
				rgbCurrent = cc.rgbResult;
				rich_edit::colorseltext(hWnd, rgbCurrent);

			}
			break;
		}
		case MENU_TXTBOLD: {
			rich_edit::txtBold();
			break;
		}
		case MENU_TXTKURSIV: {
			rich_edit::txtKursiv();
			break;
		}
		case MENU_TXT_NORMAL: {
			rich_edit::txtNormal();
			break;
		}
		case MENU_TXT_UNDERLINE: {
			rich_edit::txtUnderlined();
			break;
		}
		case MENU_TXT_DURCHGESTRICHEN: {
			rich_edit::txtdurchgestrichen();
		}
		}
	}
	case WM_KEYDOWN: {
		if (neversave == true) {
			nonamefile();
		}

		if (wp == VK_SHIFT && shiftkey == false) {
			shiftkey = true;
		}
		if (wp == VK_CONTROL && ctrlkey == false) {
			ctrlkey = true;
		}
		if (wp == 'S' && ctrlkey == true) {
			save_only(hWnd);
			skey = true;
			ctrlkey = false;
		}
		if (wp == 'S' && ctrlkey == true && shiftkey == true) {
			opennewfile(hWnd);
			ctrlkey = false;
			skey = true;
		}
		if (wp == 'Q' && ctrlkey == true) {
			MessageBox(hWnd, " Shortcuts:\n Ctrl + C: Copy\n Ctrl + X: Cut\n Ctrl + V: Paste\n Ctrl + A: Select all\n Ctrl + Arrow Up: Zoom in\n Ctrl + Arrow Down: Zoom Out\n Ctrl + Q: Help\n Ctrl + Shift + Tab: Always on Top\n Ctrl + S: Save\n Ctrl + Shift + S: Save As\n Ctrl + O: Open\n Ctrl + N: New Document\n Ctrl + Z: Undo", "Help", MB_OK | MB_ICONINFORMATION);
			qkey = true;
			ctrlkey = false;
		}
		if (wp == 'N' && ctrlkey == true && shiftkey==false) {
			newfile(hWnd);
			nkey = true;
			ctrlkey = false;
		}
		if (wp == 'O' && ctrlkey == true) {
			opennewfile(hWnd);
			okey = true;
			ctrlkey = false;
		}
		if (wp == VK_UP) {
			ShowCursorPos();
		}
		if (wp == VK_DOWN) {
			ShowCursorPos();
		}
		if (wp == VK_LEFT) {
			ShowCursorPos();
		}
		if (wp == VK_RIGHT) {
			ShowCursorPos();
		}
		if (wp == 'F' && ctrlkey == true && shiftkey == true) {
			rich_edit::txtBold();
			fkey = true;
			ctrlkey = false;
		}
		if (wp == 'U' && ctrlkey == true && shiftkey == true) {
			rich_edit::txtUnderlined();
			ukey = true;
			ctrlkey = false;
		}
		if (wp == 'K' && ctrlkey == true && shiftkey == true) {
			rich_edit::txtKursiv();
			kkey = true;
			ctrlkey = false;
		}
		if (wp == 'R' && ctrlkey == true && shiftkey == true) {
			rich_edit::txtNormal();
			rkey = true;
			ctrlkey = false;
		}
		if (wp == 'F' && ctrlkey == true && shiftkey == false) {
			searchDlg(hWnd);
			ctrlkey = false;
			fkey = true;
		}
		if (wp == 'Y' && ctrlkey == true) {
			SendMessage(hEdit, EM_REDO, 0, 0);
			ctrlkey = false;
			ykey = true;
		}
		if (wp == VK_UP && ctrlkey == true) {
			rich_edit::scroll_to(SB_LINEUP);
			uparrow = true;
		}
		if (wp == VK_DOWN && ctrlkey == true) {
			rich_edit::scroll_to(SB_LINEDOWN);
			downarrow = true;
		}
		if (wp == 'G' && ctrlkey == true) {
			gotolinedlg();
			gkey = true;
			ctrlkey = true;
		}
		if (wp == 'N' && ctrlkey == true && shiftkey == true) {
			opennewEditor(hWnd);
			ctrlkey = false;
			nkey = true;
		}
		if (wp == VK_F5) {
			if (!neversave) {
				if (strcmp(open_save_file, ".fed") == 0 || strcmp(open_save_file, ".rtf") == 0) {
					IOstream::savertf(open_save_file);
					IOstream::openrtf(open_save_file);
				}
				else {
					IOstream::SavetxtFile(open_save_file);
					IOstream::OpentxtFile(open_save_file);
				}
			}
			
		}
		break;
	}
	case WM_KEYUP: {
		if (wp == VK_CONTROL && ctrlkey == true) {
			ctrlkey = false;
			break;
		}
		if (wp == VK_SHIFT && shiftkey == true) {
			shiftkey = false;
			ctrlkey = false;
		}
		if (wp == 'Q' && qkey == true) {
			qkey = false;
			ctrlkey = false;
		}
		if (wp == 'S' && skey == true) {
			skey = false;
			ctrlkey = false;
		}
		if (wp == 'N' && nkey == true) {
			nkey = false;
			ctrlkey = false;
		}
		if (wp == 'O' && okey == true) {

			okey = false;
			ctrlkey = false;
		}
		if (wp == 'F' && fkey == true) {
			fkey = false;
			ctrlkey = false;
		}
		if (wp == 'U' && ukey == true) {
			ukey = false;
			ctrlkey = false;
		}
		if (wp == 'K' && kkey == true) {
			kkey = false;
			ctrlkey = false;
		}
		if (wp == 'F' && rkey == true) {
			rkey = false;
			ctrlkey = false;
		}
		if (wp == 'Y' && ykey == true) {
			ykey = false;
			ctrlkey = false;
		}
		if (wp == VK_UP && uparrow == true) {
			uparrow = false;
		}
		if (wp == VK_DOWN && downarrow == true) {
			downarrow = false;
		}
		if (wp == 'G' && gkey == true) {
			ctrlkey = false;
			gkey = false;
		}
		if (wp == VK_UP) {
			ShowCursorPos();
		}
		if (wp == VK_DOWN) {
			ShowCursorPos();
		}
		if (wp == VK_LEFT) {
			ShowCursorPos();
		}
		if (wp == VK_RIGHT) {
			ShowCursorPos();
		}
		break;
	}
	case WM_LBUTTONUP: {
		ShowCursorPos();
	}
	case EN_SELCHANGE: {
		needsave = true;
		ShowCursorPos();
		break;
	}
	case WM_MOUSEMOVE: {
		SetFocus(hEdit);
		break;
	}
	}
	return CallWindowProc((WNDPROC)PrevWndProcEdit, hWnd, msg, wp, lp);
}
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	
	ShowCursorPos();

	switch (msg)
	{

	case WM_COMMAND: {
		switch (wp)
		{
		///////////File
		case MENU_EXIT: {
			getscreensize(hWnd);
			writeinifile();
			Save_Wnd_Modes();
			exitsave(hWnd);
			break;
		}
		case MENU_NEW: {
			newfile(hWnd);
			break;
		}
		case MENU_OPEN: {
			opennewfile(hWnd);
			break;
		}
		case MENU_SAVE: {
			save_only(hWnd);
			break;
		}
		case MENU_SAVEAS: {
			IOstream::save_file(hWnd);
			break;
		}
		case MENU_NEWEDITOR: {
			opennewEditor(hWnd);
			break;
		}
		case MENU_CLOSEOPENED: {
			closefile(hWnd);
			break;
		}		
		case MENU_PREFERENCES: {
			Preferencedlg(hWnd);
			break;
		}
		case MENU_SAVEASCOPY: {
			IOstream::saveascopy(hWnd);
			break;
		}
			///////////Edit
		case MENU_COPY: {
			SendMessage(hEdit, WM_COPY, 0, 0);
			break;
		}
		case MENU_PASTE: {
			//SendMessage(hEdit, EM_PASTESPECIAL, CF_TEXT, 0);
			SendMessage(hEdit, WM_PASTE, 0, 0);
			break;
		}
		case MENU_CUT: {
			SendMessage(hEdit, WM_CUT, 0, 0);
			break;
		}
		case MENU_DELETE: {
			SendMessage(hEdit, EM_REPLACESEL, 0, 0);
			break;
		}
		case MENU_SELALL: {
			SendMessage(hEdit, EM_SETSEL, 0, -1);
			break;
		}
		case MENU_UNDO: {
			SendMessage(hEdit, EM_UNDO, 0, 0);
			break;
		}
		case MENU_REDO: {
			SendMessage(hEdit, EM_REDO, 0, 0);
			break;
		}
						///////////Window
		case MENU_ALWAYSONTOP1: {
			always_on_top(hWnd);
			break;
		}
		case MENU_MINIMODUS: {
			if (minimodusbool == true) { normalmodus(hWnd, lp); AddToolbarv(hWnd); }
			else {
				DestroyWindow(hToolv);
				MoveWindow(hEdit, 0, 0, LOWORD(lp), HIWORD(lp), TRUE);
				minimodus(hWnd, lp);
			}
			windowmode = 1;
			break;
		}
		case MENU_BORDERLESS: {
			if (borderlessbool == true) {

				borderless(hWnd, false,lp);
				borderlessbool = false;
			}
			else {
				borderless(hWnd, true,lp);
				borderlessbool = true;
			}
			break;
		}
		case MENU_TOOLBAR: {
			showToolbar(hWnd,lp);
			break;
		}
		case MENU_STATUSBAR: {
			showStatusbar(hWnd, lp);
			break;
		}
							  //////////Format
		case MENU_COLORBLACK: {
			rich_edit::colorseltext(hWnd, RGB(0, 0, 0));
			break;
		}
		case MENU_COLORBLUE: {
			rich_edit::colorseltext(hWnd, RGB(0, 0, 255));
			break;
		}
		case MENU_COLORGREEN: {
			rich_edit::colorseltext(hWnd, RGB(0, 255, 0));
			break;
		}
		case MENU_COLORRED: {
			rich_edit::colorseltext(hWnd, RGB(255, 0, 0));
			break;
		}
		case MENU_COLORYELLOW: {
			rich_edit::colorseltext(hWnd, RGB(255, 255, 0));
			break;
		}
		case MENU_TXTCOLOR: {
			CHOOSECOLOR cc;
			static COLORREF acrCustClr[32];
			HBRUSH hbrush;
			static DWORD rgbCurrent;


			ZeroMemory(&cc, sizeof(cc));
			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = hWnd;
			cc.lpCustColors = (LPDWORD)acrCustClr;
			cc.rgbResult = rgbCurrent;
			cc.Flags = CC_FULLOPEN | CC_RGBINIT;

			if (ChooseColor(&cc) == TRUE) {
				hbrush = CreateSolidBrush(cc.rgbResult);
				rgbCurrent = cc.rgbResult;
				rich_edit::colorseltext(hWnd, rgbCurrent);

			}
			break;
		}
		case MENU_RAINBOW: {
			if (switchrainbowcolor == true) {
				rainbowcolor = false;
				switchrainbowcolor = false;
			}
			else
			{
				rich_edit::colorseltext(hWnd, RGB(0, 0, 0));
				rainbowcolor = true;
				switchrainbowcolor = true;
			}
			break;
		}
		case MENU_WORDWRAP: {
			wordwrap(hWnd);
			break;
		}
		case MENU_TXTBOLD: {
			rich_edit::txtBold();
			break;
		}
		case MENU_TXTKURSIV: {
			rich_edit::txtKursiv();
			break;
		}
		case MENU_TXT_UNDERLINE: {
			rich_edit::txtUnderlined();
			break;
		}
		case MENU_TXT_DURCHGESTRICHEN: {
			rich_edit::txtdurchgestrichen();
			break;
		}
		case MENU_TXT_NORMAL: {
			rich_edit::txtNormal();
			break;
		}
							  ///////////Help
		case MENU_ABOUT: {
			Aboutdlg();
			break;
		}
		case MENU_SHORTCUTS: {
			Shortcuts();
			break;
		}
		case MENU_CHANGELOG: {
			Changelog();
			break;
		}
							 ///////////Search
		case MENU_SEARCH: {
			searchDlg(hWnd);
			break;
		}
		case MENU_GOTOLINE: {
			gotolinedlg();
		}
		case MENU_GOTOBOTTOM: {
			rich_edit::scroll_to(SB_BOTTOM);

			LRESULT textlenght;
			textlenght = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
			textlenght = textlenght;
			SendMessage(hEdit, EM_SETSEL, textlenght, textlenght);
			ShowCursorPos();

			break;
		}
		case MENU_GOTOTOP: {
			rich_edit::scroll_to(SB_TOP);
			SendMessage(hEdit, EM_SETSEL, 0, 0);
			ShowCursorPos();

			break;
		}
		}
	}
	case WM_SETFOCUS: {
		SetFocus(hEdit);
		return 0;
	}
	case WM_SIZE: {
		ShowCursorPos();
		MoveWindow(hToolv, 0, 0, 0, HIWORD(lp), TRUE);
		MoveWindow(hEdit, 0, 36, LOWORD(lp), HIWORD(lp) - 60, TRUE);
		MoveWindow(hStatusBar, HIWORD(lp) - 23, 0, LOWORD(lp), 0, TRUE);

		return 0;
	}
	case WM_CREATE: {
		isstarting = true;
		ReadLanguagePacket(langpack1);
		init(hWnd);
		loadlibs();
		addEditControl(hWnd, 0, 36, x, y - 36);
		Menubar(hWnd);
		AddToolbarv(hWnd);
		SetRectOfEdit();
		addStatusbar(hWnd);
		registerchangefont();
		setfontofDialogs();
		checktoAutosave(hWnd);
		Get_Wnd_Modes(hWnd, lp);
		if (strcmp(texttoopen, "") != 0) {
			openfileonopen(hWnd, texttoopen);
		}
		isstarting = false;
		return 0;
	}
	case WM_CLOSE: {
		getscreensize(hWnd);
		Save_Wnd_Modes();
		writeinifile();
		exitsave(hWnd);
		break;
	}
	case WM_NOTIFY: {
		EDITSTREAM		editstream;
		fstream			fileio;
		NMHDR			*notify;
		ENDROPFILES		*dropfiles;
		HANDLE			hdrop;
		char			dropped[MAX_PATH] = { "" };

		ShowCursorPos();

		notify = (NMHDR*)lp;
		if (notify->hwndFrom == hEdit) {
			switch (notify->code)
			{
			case EN_SELCHANGE: {
				KillTimer(hWnd, SAVE_CURSOR_TIMER);
				break;
			}
			case EN_DROPFILES: {

				dropfiles = (ENDROPFILES FAR *)lp;
				hdrop = dropfiles->hDrop;
				DragQueryFile((HDROP)hdrop, 0, dropped, MAX_PATH);		//only first one will count
				DragFinish((HDROP)hdrop);
				for (int l = 0; l < strlen(dropped); l++)
					if (isupper(dropped[l]))
						dropped[l] = tolower(dropped[l]);
				if (strstr(dropped, ".fed") != NULL || strstr(dropped, ".rtf") != NULL)
				{
					rtfopen = true;
					openfile = dropped;
					opendragdrop(hWnd);

				}
				else {
					rtfopen = false;
					openfile = dropped;
					opendragdrop(hWnd);
				}
				break;
			}
			}
		}

		break;
	}
	case WM_TIMER: {

		switch (wp)
		{
		case AUTO_SAVE_TIMER: {
			if (neversave == true) {

				KillTimer(hWnd, AUTO_SAVE_TIMER);

				int autosavemb = MessageBox(hWnd, "Autosave could not be activated, because no filedirectory was specified", "Autosave", MB_OKCANCEL);
				
				if (autosavemb == IDOK) {
					IOstream::save_file(hWnd);
					if (save_open_sucess == true) {
						autosave(hWnd);
					}
					else {
						MessageBox(hWnd, "Autosave will be deactivated until softwarerestart", "Attention", MB_OK);
					}
				}
				if (autosavemb == IDCANCEL) {
					MessageBox(hWnd, "Autosave will be deactivated until softwarerestart", "Attention", MB_OK);
					return 0;
				}
			}
			else {
				save_only(hWnd);
				autosave(hWnd);
			}
			

			return 0;
		}
		case SAVE_CURSOR_TIMER: {
			HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
			SetCursor(cursor);
			break;
		}
		}
	}
	
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}