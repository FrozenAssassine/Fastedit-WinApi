/////////////////////SprachPaket

char Main_Lang[400];
char Main_Version[400];

char deLanguagePackPath[5000];
char Menu_File[_MAX_PATH];
char Menu_File_New[_MAX_PATH];
char Menu_File_Open[_MAX_PATH];
char Menu_File_Save[_MAX_PATH];
char Menu_File_Save_As[_MAX_PATH];
char Menu_File_Close_File[_MAX_PATH];
char Menu_File_NewEditor[_MAX_PATH];
char Menu_File_Preferences[_MAX_PATH];
char Menu_File_Exit[_MAX_PATH];
char Menu_File_Save_As_Copy[_MAX_PATH];

char Menu_Edit[_MAX_PATH];
char Menu_Edit_Redo[_MAX_PATH];
char Menu_Edit_Undo[_MAX_PATH];
char Menu_Edit_Copy[_MAX_PATH];
char Menu_Edit_Paste[_MAX_PATH];
char Menu_Edit_Cut[_MAX_PATH];
char Menu_Edit_Delete[_MAX_PATH];
char Menu_Edit_Select_All[_MAX_PATH];

char Menu_Format[_MAX_PATH];
char Menu_Format_Colors[_MAX_PATH];
char Menu_Format_Colors_Black[_MAX_PATH];
char Menu_Format_Colors_Red[_MAX_PATH];
char Menu_Format_Colors_Green[_MAX_PATH];
char Menu_Format_Colors_Blue[_MAX_PATH];
char Menu_Format_Colors_Yellow[_MAX_PATH];
char Menu_Format_Colors_Rainbow[_MAX_PATH];
char Menu_Format_Colors_Color[_MAX_PATH];
char Menu_Format_Normal[_MAX_PATH];
char Menu_Format_Bold[_MAX_PATH];
char Menu_Format_Italic[_MAX_PATH];
char Menu_Format_Underlined[_MAX_PATH];
char Menu_Format_Underlined_Underlined[_MAX_PATH];
char Menu_Format_Underlined_Striketout[_MAX_PATH];
char Menu_Format_Wordwrap[_MAX_PATH];

char Menu_Search[_MAX_PATH];
char Menu_Search_Search[_MAX_PATH];
char Menu_Search_GoToLine[_MAX_PATH];
char Menu_Search_GoTo[_MAX_PATH];
char Menu_Search_GoTo_Begin[_MAX_PATH];
char Menu_Search_GoTo_End[_MAX_PATH];

char Menu_View[_MAX_PATH];
char Menu_View_AlwaysOnTop[_MAX_PATH];
char Menu_View_MiniMode[_MAX_PATH];

char Menu_Help[_MAX_PATH];
char Menu_Help_Shortcuts[_MAX_PATH];
char Menu_Help_Changelog[_MAX_PATH];
char Menu_Help_About[_MAX_PATH];

char MessageBox_Ask_To_Save[_MAX_PATH];
char MessageBox_Ask_To_Save_Title[_MAX_PATH];
char MessageBox_Btn_Save[_MAX_PATH];
char MessageBox_Btn_Not_Save[_MAX_PATH];
char MessageBox_Btn_Cancel[_MAX_PATH];
char MessageBox_Btn_Ok[_MAX_PATH];
char MessageBox_Title_Error[_MAX_PATH];
char MessageBox_Title_Info[_MAX_PATH];
char MessageBox_Title_Go_To[_MAX_PATH];
char MessageBox_Btn_Search[_MAX_PATH];

char Statusbar_Saved[_MAX_PATH];
char Statusbar_Not_Saved[_MAX_PATH];

char About_First[_MAX_PATH];
char About_Second[_MAX_PATH];
char About_Third[_MAX_PATH];

char TitleName_Untitled[_MAX_PATH];

char MessageBox_Pref_SpaceLeft_txt[_MAX_PATH];
char MessageBox_Pref_SpaceRight_txt[_MAX_PATH];
char MessageBox_Pref_SpaceTop_txt[_MAX_PATH];
char MessageBox_Pref_SpaceBottom_txt[_MAX_PATH];
char MessageBox_Pref_Message[_MAX_PATH];
char MessageBox_Pref_BTN_LoadLangPack[_MAX_PATH];

char MessageBox_pref_DialogFontsize_txt[_MAX_PATH];
char MessageBox_pref_AutoSave_txt[_MAX_PATH];
char MessageBox_pref_AutoSave_After_txt[_MAX_PATH];

char Error_Faild_to_Open[_MAX_PATH];
char Error_Fastedit_Error[_MAX_PATH];
char Error_Couldnt_load_richedit[_MAX_PATH];
char Error_Couldnt_Read_File[_MAX_PATH];
char Error_Couldnt_Write_File[_MAX_PATH];
char Error_Couldnt_Read[_MAX_PATH];
char Error_Couldnt_Write[_MAX_PATH];

char Open_File_DLG_Title[500];
char Save_File_DLG_Title[500];
char Open_felangFile_DLG_Title[500];

extern char langpack1[_MAX_PATH];
extern char softwareversion[50];
extern bool displaylagPackBool;

void OpenlangPack();
void readinifile();
void writeinifile();
void checkfelangversion();

void ErrorHandel() {
	bool ckecking = false;
	if (strcmp(Menu_Search_Search, "NULL") == 0 || strcmp(Menu_Format_Bold, "NULL") == 0 ||
		strcmp(Menu_Format_Colors_Black, "NULL") == 0 || strcmp(Menu_Edit_Cut, "NULL") == 0 ||
		strcmp(Menu_Edit, "NULL") == 0 || strcmp(Menu_File_NewEditor, "NULL") == 0 ||
		strcmp(Menu_File, "NULL") == 0 || strcmp(MessageBox_Btn_Search, "NULL") == 0 ||
		strcmp(TitleName_Untitled, "NULL") == 0 ||
		strcmp(MessageBox_Title_Go_To, "NULL") == 0 || strcmp(Menu_View_MiniMode, "NULL") == 0 ||
		strcmp(Error_Fastedit_Error, "NULL") == 0 || strcmp(MessageBox_Pref_SpaceLeft_txt, "NULL") == 0 ||
		strcmp(About_Second, "NULL") == 0 ||strcmp(Error_Faild_to_Open, "NULL") == 0)
	{
		char outtext[_MAX_PATH];
		wsprintf(outtext, "Unable to open Languagepack from '%s' Please load it again", deLanguagePackPath);
		MessageBox(NULL, outtext, "Error", MB_OK);
		OpenlangPack();
		if (displaylagPackBool == true) {
			writeinifile();
			MessageBox(NULL, "Languagepack Saved please start the Software again", "Error", MB_OK);
			PostQuitMessage(0);
		}
		else {
			PostQuitMessage(0);
		}
		

	}
}
void ReadLanguagenamefromPack(char*langpack) {
	GetPrivateProfileString("INIT", "Main_Lang", "NULL", Main_Lang, 400, langpack);

}
void ReadLanguagePacket(char * langpack) {
	wsprintf(deLanguagePackPath, langpack);

	GetPrivateProfileString("Menubar", "Menu_Help", "NULL", Menu_Help, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Help_Shortcuts", "NULL", Menu_Help_Shortcuts, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Help_Changelog", "NULL", Menu_Help_Changelog, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Help_About", "NULL", Menu_Help_About, 500, deLanguagePackPath);

	GetPrivateProfileString("Menubar", "Menu_View", "NULL", Menu_View, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_View_AlwaysOnTop", "NULL", Menu_View_AlwaysOnTop, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_View_MiniMode", "NULL", Menu_View_MiniMode, 500, deLanguagePackPath);

	GetPrivateProfileString("Menubar", "Menu_Search", "NULL", Menu_Search, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Search_Search", "NULL", Menu_Search_Search, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Search_GoToLine", "NULL", Menu_Search_GoToLine, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Search_GoTo", "NULL", Menu_Search_GoTo, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Search_GoTo_Begin", "NULL", Menu_Search_GoTo_Begin, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Search_GoTo_End", "NULL", Menu_Search_GoTo_End, 500, deLanguagePackPath);

	GetPrivateProfileString("Menubar", "Menu_Format", "NULL", Menu_Format, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Colors", "NULL", Menu_Format_Colors, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Colors_Black", "NULL", Menu_Format_Colors_Black, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Colors_Red", "NULL", Menu_Format_Colors_Red, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Colors_Green", "NULL", Menu_Format_Colors_Green, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Colors_Blue", "NULL", Menu_Format_Colors_Blue, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Colors_Yellow", "NULL", Menu_Format_Colors_Yellow, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Colors_Rainbow", "NULL", Menu_Format_Colors_Rainbow, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Colors_Color", "NULL", Menu_Format_Colors_Color, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Normal", "NULL", Menu_Format_Normal, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Bold", "NULL", Menu_Format_Bold, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Italic", "NULL", Menu_Format_Italic, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Underlined", "NULL", Menu_Format_Underlined, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Underlined_Underlined", "NULL", Menu_Format_Underlined_Underlined, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Underlined_Striketout", "NULL", Menu_Format_Underlined_Striketout, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Format_Wordwrap", "NULL", Menu_Format_Wordwrap, 500, deLanguagePackPath);

	GetPrivateProfileString("Menubar", "Menu_File", "NULL", Menu_File, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_File_New", "NULL", Menu_File_New, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_File_Open", "NULL", Menu_File_Open, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_File_Save", "NULL", Menu_File_Save, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_File_Save_As", "NULL", Menu_File_Save_As, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_File_Save_As_Copy", "NULL", Menu_File_Save_As_Copy, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_File_Close_File", "NULL", Menu_File_Close_File, 500, deLanguagePackPath);

	
	GetPrivateProfileString("Menubar", "Menu_File_NewEditor", "NULL", Menu_File_NewEditor, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_File_Preferences", "NULL", Menu_File_Preferences, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_File_Exit", "NULL", Menu_File_Exit, 500, deLanguagePackPath);

	GetPrivateProfileString("Menubar", "Menu_Edit", "NULL", Menu_Edit, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Edit_Redo", "NULL", Menu_Edit_Redo, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Edit_Undo", "NULL", Menu_Edit_Undo, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Edit_Copy", "NULL", Menu_Edit_Copy, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Edit_Paste", "NULL", Menu_Edit_Paste, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Edit_Cut", "NULL", Menu_Edit_Cut, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Edit_Delete", "NULL", Menu_Edit_Delete, 500, deLanguagePackPath);
	GetPrivateProfileString("Menubar", "Menu_Edit_Select_All", "NULL", Menu_Edit_Select_All, 500, deLanguagePackPath);

	GetPrivateProfileString("MessageBox", "MessageBox_Ask_To_Save", "NULL", MessageBox_Ask_To_Save, 500, deLanguagePackPath);
	GetPrivateProfileString("MessageBox", "MessageBox_Ask_To_Save_Title", "NULL", MessageBox_Ask_To_Save_Title, 500, deLanguagePackPath);
	GetPrivateProfileString("MessageBox", "MessageBox_Btn_Save", "NULL", MessageBox_Btn_Save, 500, deLanguagePackPath);
	GetPrivateProfileString("MessageBox", "MessageBox_Btn_Not_Save", "NULL", MessageBox_Btn_Not_Save, 500, deLanguagePackPath);
	GetPrivateProfileString("MessageBox", "MessageBox_Btn_Cancel", "NULL", MessageBox_Btn_Cancel, 500, deLanguagePackPath);
	GetPrivateProfileString("MessageBox", "MessageBox_Btn_Ok", "NULL", MessageBox_Btn_Ok, 500, deLanguagePackPath);
	GetPrivateProfileString("MessageBox", "MessageBox_Title_Error", "NULL", MessageBox_Title_Error, 500, deLanguagePackPath);
	GetPrivateProfileString("MessageBox", "MessageBox_Pref_Message", "NULL", MessageBox_Pref_Message, 500, deLanguagePackPath);
	GetPrivateProfileString("MessageBox", "MessageBox_Title_Info", "NULL", MessageBox_Title_Info, 500, deLanguagePackPath);

	GetPrivateProfileString("Statusbar", "Statusbar_Not_Saved", "NULL", Statusbar_Not_Saved, 500, deLanguagePackPath);
	GetPrivateProfileString("Statusbar", "Statusbar_Saved", "NULL", Statusbar_Saved, 500, deLanguagePackPath);
	
	GetPrivateProfileString("Other", "About_First", "NULL", About_First, 500, deLanguagePackPath);
	GetPrivateProfileString("Other", "About_Second", "NULL", About_Second, 500, deLanguagePackPath);
	GetPrivateProfileString("Other", "About_Third", "NULL", About_Third, 500, deLanguagePackPath);
	GetPrivateProfileString("Other", "TitleName_Untitled", "NULL", TitleName_Untitled, 500, deLanguagePackPath);
	GetPrivateProfileString("Other", "MessageBox_Pref_BTN_LoadLangPack", "NULL", MessageBox_Pref_BTN_LoadLangPack, 500, deLanguagePackPath);
	
	GetPrivateProfileString("Other", "MessageBox_Pref_SpaceLeft_txt", "NULL", MessageBox_Pref_SpaceLeft_txt, 500, deLanguagePackPath);
	GetPrivateProfileString("Other", "MessageBox_Pref_SpaceRight_txt", "NULL", MessageBox_Pref_SpaceRight_txt, 500, deLanguagePackPath);
	GetPrivateProfileString("Other", "MessageBox_Pref_SpaceTop_txt", "NULL", MessageBox_Pref_SpaceTop_txt, 500, deLanguagePackPath);
	GetPrivateProfileString("Other", "MessageBox_Pref_SpaceBottom_txt", "NULL", MessageBox_Pref_SpaceBottom_txt, 500, deLanguagePackPath);
	GetPrivateProfileString("Other", "MessageBox_Btn_Search", "NULL", MessageBox_Btn_Search, 500, deLanguagePackPath);
	GetPrivateProfileString("Other", "MessageBox_Title_Go_To", "NULL", MessageBox_Title_Go_To, 500, deLanguagePackPath);
	GetPrivateProfileString("Other", "MessageBox_pref_DialogFontsize_txt","NULL", MessageBox_pref_DialogFontsize_txt, 500, deLanguagePackPath);
	GetPrivateProfileString("Other", "MessageBox_pref_AutoSave_txt", "NULL", MessageBox_pref_AutoSave_txt, 500, deLanguagePackPath);
	GetPrivateProfileString("Other", "MessageBox_pref_AutoSave_After_txt", "NULL", MessageBox_pref_AutoSave_After_txt, 500, deLanguagePackPath);
	
	GetPrivateProfileString("Error", "Error_Faild_to_Open", "NULL", Error_Faild_to_Open, 500, deLanguagePackPath);
	GetPrivateProfileString("Error", "Error_Fastedit_Error", "NULL", Error_Fastedit_Error, 500, deLanguagePackPath);
	GetPrivateProfileString("Error", "Error_Couldnt_load_richedit", "NULL", Error_Couldnt_load_richedit, 500, deLanguagePackPath);
	GetPrivateProfileString("Error", "Error_Couldnt_Read_File", "NULL", Error_Couldnt_Read_File, 500, deLanguagePackPath);
	GetPrivateProfileString("Error", "Error_Couldnt_Write_File", "NULL", Error_Couldnt_Write_File, 500, deLanguagePackPath);
	GetPrivateProfileString("Error", "Error_Couldnt_Read", "NULL", Error_Couldnt_Read, 500, deLanguagePackPath);
	GetPrivateProfileString("Error", "Error_Couldnt_Write", "NULL", Error_Couldnt_Write, 500, deLanguagePackPath);

	GetPrivateProfileString("INIT", "Main_Lang", "NULL", Main_Lang, 400, deLanguagePackPath);
	GetPrivateProfileString("INIT", "Main_Version", "NULL", Main_Version, 400, deLanguagePackPath);

	GetPrivateProfileString("Dialog", "Open_File_DLG_Title", "NULL", Open_File_DLG_Title, 400, deLanguagePackPath);
	GetPrivateProfileString("Dialog", "Open_felangFile_DLG_Title", "NULL", Open_felangFile_DLG_Title, 400, deLanguagePackPath);
	GetPrivateProfileString("Dialog", "Save_File_DLG_Title", "NULL", Save_File_DLG_Title, 400, deLanguagePackPath);

	ErrorHandel();
	checkfelangversion();
}
void checkfelangversion() {

	if (strcmp(Main_Version, softwareversion) == 0) {
		return;
	}
	else {
		MessageBox(NULL, "Incorrect languagepack version please load another pack from the folder", "Error", MB_OK);
		OpenlangPack();
	}
}

