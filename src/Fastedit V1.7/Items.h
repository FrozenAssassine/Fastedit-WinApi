#pragma once
//////////////////////////////////
/////////////////Menubar//////////
/////////////File
#define MENU_NEW 200
#define MENU_OPEN 201
#define MENU_SAVE 202
#define MENU_SAVEAS 203
#define MENU_EXIT 204
#define MENU_SAVEDIFFRENT 205
#define MENU_NEWEDITOR 206
#define MENU_CLOSEOPENED 207
#define MENU_PREFERENCES 208
#define MENU_SAVEASCOPY 209
////////////Edit
#define MENU_COPY 210
#define MENU_PASTE 211
#define MENU_CUT 212
#define MENU_UNDO 213
#define MENU_SELALL 214
#define MENU_DELETE 21
#define MENU_REDO 217
/////////////search
#define MENU_GOTOLINE 280
#define MENU_SEARCH 281
#define MENU_GOTOTOP 282
#define MENU_GOTOBOTTOM 283
#define MENU_GOTOMIDDLE 284
/////////////Window
#define MENU_ALWAYSONTOP1 220
#define MENU_BORDERLESS 221
#define MENU_MINIMODUS 222
#define MENU_TOOLBAR 223
#define MENU_STATUSBAR 224
//////////Help
#define MENU_SHORTCUTS 230
#define MENU_ABOUT 231
#define MENU_CHANGELOG 232
//////////Format
#define MENU_COLORRED 250
#define MENU_COLORGREEN 251
#define MENU_COLORBLUE 252
#define MENU_TXTCOLOR 253
#define MENU_COLORBLACK 254
#define MENU_COLORYELLOW 255
#define MENU_RAINBOW 256
#define MENU_TXTBOLD 260
#define MENU_TXTKURSIV 261
#define MENU_TXT_UNDERLINE 262
#define MENU_TXT_NORMAL 263
#define MENU_TXT_DURCHGESTRICHEN 264
#define MENU_SUPERSCRIPT 265
#define MENU_WORDWRAP 270

///////////////////GUI//////////
//////////////////////////////////////////
#define STATUSBAR_ID_1 2000
#define COMBOBOX_FONTSIZE1 2010
#define IDC_TAB 2020
#define AUTO_SAVE_TIMER 2050
#define SAVE_CURSOR_TIMER 2051
//////////////////////////////////////////
///////////////////Popupmenu//////////////
#define POPUP_SELALL 400
#define POPUP_COPY 401
#define POPUP_UNDO 402
#define POPUP_PASTE 403
#define POPUP_CUT 404
//////////////////////////////////////////
///////////////Searchdlg//////////////////
#define SEARCH_CANCEL 1000
#define SEARCH_SEARCH 1001
#define SEARCH_DOWN 1002
#define SEARCH_UP 1003
/////////////////////////////////////////
///////////////Gotoline///////////1200-1299
#define GOTOLINE_GO 1200
#define GOTOLINE_EDIT 1201
/////////////////////////////////1300-1399
/////////////AboutDlg////////////////////
#define BTN_ABOUT_OK 1300
/////////////PreferenceDLG///////1100-1199
////////////////////////////////////////
#define PREF_BTN_OK 1100
#define PREF_BTN_CANCEL 1101
#define PREF_BTN_OPENLANGPACK 1102
#define PREF_CHECKBOX_AUTOSAVE 1120
///////////////MB_Custom DLG////////1500-1599
/////////////////////////////////////////
#define CUSTOM_MB_BTN1 1500
#define CUSTOM_MB_BTN2 1501
//////////////Save_Dlg//////////////////
/////////////////////////////////1400-1499
#define SAVE_BTN_CANCEL 1400
#define SAVE_BTN_NOTSAVE 1401
#define SAVE_BTN_SAVE 1402
//////////////////////////////////////////
////////////////font//////////////////////
HFONT MainFont = CreateFont(17, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, //charset geändert
	OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	DEFAULT_PITCH | FF_DONTCARE, TEXT("Consolas"));

HFONT OtherFont = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
	OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"));

