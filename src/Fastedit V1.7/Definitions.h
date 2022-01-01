#pragma on

/////////////////////bool////////////////////////
bool shiftkey = false;
bool ctrlkey = false;
bool skey = false;
bool okey = false;
bool qkey = false;
extern bool nkey = false;
bool fkey = false;
bool ukey = false;
bool kkey = false;
bool rkey = false;
bool mkey = false;
bool ykey = false;
bool gkey = false;
bool uparrow = false;
bool downarrow = false;
bool alwaysontop = false;
bool minimodusbool = false;
bool saveminimode = false;
bool savealwaysontop = false;
bool wrapbool = false;
bool notsave = false;
bool searchupbool = false;
bool searchdownbool = false;
bool savecheckdownbool = false;
bool borderlessbool = false;
bool rainbowcolor = false;
bool switchrainbowcolor = false;
bool needsave = false;
bool neversave = true;
bool openrtfbool = false;
bool opentxtbool = false;
bool savetxtbool = false;
bool savertfbool = false;
bool txtselected = false;
bool isstarting = true;
bool save_open_sucess = false;
bool switchtoolbar = false;
bool switchstatusbar = false;
bool showToolBar = false;
bool showStatusBar = false;
bool displaylagPackBool = false;
bool autosavecheckbox = false;
bool autosavecheckboxsave = false;
//////////////ints//////////
int screenX = GetSystemMetrics(SM_CXSCREEN);
int screenY = GetSystemMetrics(SM_CYSCREEN);
int x;
int y;
int xpos;
int ypos;
int pos1 = 8;
int pos2 = 350;
int WordWrap = 0;
int xpossearchwnd;
int ypossearchwnd;
int rainbowcount = 0;
int fontsize = 18;
int selectionstart = 0;
int selectionend = 0;
int leftspaceofedit = 0;
int rightspaceofedit = 0;
int topspaceofedit = 0;
int bottomspaceofedit = 0;
int windowmode = 0;
int action_on_Dialog = 0;
int action_on_Dialog_2 = 0;
int lastfileextensionsaved = 0;
int lastfileextensionOpened = 0;
int DialogFontsize = 0;
int lastlangpacksopenedcount = 0;
int lastlangpackcount = 0;
int registered = 0; //0 = false 1 = true
int autosavetime = 1; //
int doautosave = 0;

//////////Font/////
HFONT GUITXTFont;

////////main
HWND tabCTRL;
static HWND hEdit;
HMENU hMenu;
HWND hToolv;
HWND hToolPopup;
HWND hMainWindow;
HWND hWindow;
HINSTANCE relib;
HMENU hPOPMenu;
HWND selectiontextStatic;
HWND hStatusBar;
HMENU hccpMenu;
HWND hFontcombobox;
HWND columnline;
HWND hwndTab;
HWND prefLanguageList;
HWND prefEditFontsizeGUI;
//Tabs
HWND TabBTN1;
HWND TabBTN2;
HWND TabBTN3;
HWND TabBTN4;
HWND TabBTN5;
///////////search
HWND searchedit;
HWND SearchWND;
HWND search_BTN_1;
HWND search_BTN_2;
///////////gotoline
HWND gotolineWND;
HWND gotoline_Edit;
HWND gotoline_BTN1;
////////////About Dialog///
HWND AboutWND;
HWND about_static;
HWND about_BTN_1;
/////////Save Dialog//
HWND hSaveWND;
HWND Save_BTN;
HWND Save_NotSave_BTN;
HWND Save_Cancel_BTN;
HWND Save_Text;
////////////////////////
///////Preferences Dialog///
HWND prefEditAutoSaveTime;
HWND prefWND;
HWND prefEditTop;
HWND prefEditLeft;
HWND prefEditRight;
HWND prefEditButtom;
HWND pref_Ok_BTN;
HWND pref_Cancel_BTN;
HWND pref_OpenLangPack_BTN;
HWND pref_static1;
HWND pref_static2;
HWND pref_static3;
HWND pref_static4;
HWND pref_static5;
HWND pref_static6;
HWND pref_static7;
HWND pref_static8;
HWND pref_static9;
HWND pref_static10;
HWND pref_static11;
HWND pref_static12;
HWND pref_static13;
HWND pref_CheckBox1;
//////OWN MessageBox////
HWND mb_dlg_WND;
////////////////////////
FINDTEXTEX findtext;
//////CHARS , chars , TCHARS

char open_save_file[10000] = { 0 };
TCHAR selstart;
TCHAR selend;
TCHAR ustring;

CHAR FindBuffer[MAX_PATH] = "";

char inipath[MAX_PATH];
//char inipath[_MAX_PATH] = { "C:\\Users\\Default\\AppData\\Roaming\\Fastedit\\FasteditV1.4.ini"};
char changelog[_MAX_PATH];
char wordwrapini[_MAX_PATH];
char lastOpenedPath[_MAX_PATH];
char lastSavedPath[_MAX_PATH];

char opengettext1[_MAX_PATH];
char selectedtext[_MAX_PATH];
char gettextsavertf[_MAX_PATH];
char gettexttocompare2[_MAX_PATH];
char *lastopenedpath;
char * texttoopen;
char langpack1[_MAX_PATH];
char langpack2[_MAX_PATH];
char langpack3[_MAX_PATH];
char langpack1_1[_MAX_PATH];
char langpack2_1[_MAX_PATH];
char langpack3_1[_MAX_PATH];

char projectfolderpath[_MAX_PATH];

char * openfile; // For Drag and Drop
bool rtfopen = false;
char SaveOpen_File_Extensions[5000];

char Abouttext[_MAX_PATH];

////////////////////////////////
char RecentFile1[_MAX_PATH];
char RecentFile2[_MAX_PATH];
char RecentFile3[_MAX_PATH];
char RecentFile4[_MAX_PATH];
char RecentFile5[_MAX_PATH];

//MessageBoxen / DialogBoxen
char softwareversion[50] = "V1.7";
char iniName[] = "Fastedit V1.7.ini";

char OpenFilefilter2[5000] = "Fastedit Document(.fed)\0 * .fed\0Richedit Datei(.rtf)\0 * .rtf\0Text Datei(.txt)\0 * .txt\0C - Code(.c)\0 * .c\0CPP - Code Code(.cpp)\0 * .cpp\0Headerdatei(.h)\0 * .h\0html - Code(.html)\0 * .html\0htm - Code(.htm)\0 * .htm\0Batchdatei(.bat)\0 * .bat\0Python Code(.py)\0 * .py\0Data Datei(.dat)\0 * .dat\0Log Datei(.log)\0 * .log";
char Open_Save_Filefilter[5000] = "Fastedit document(.fed)\0* .fed\0Richedit document(.rtf)\0* .rtf\0Text file(.txt)\0* .txt\0Windows batch file(.bat)\0 * .bat\0Windows registry file(.reg)\0 * .reg\0Log file(.log)\0*.log\0C++ source file(.cpp)\0* .cpp\0C header file(.h)\0* .h\0C source file(.c)\0* .c\0C# script(.cs)\0 * .cs\0Python source code(.py)\0 * .py\0Html file(.html)\0 * .html\0Htm file(.htm)\0 * .htm\0Java script file(.js)\0*.js\0Cascading Style Sheet(.css)\0*.css\0Java file(.java)\0*.java\0";

