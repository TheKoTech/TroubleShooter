#pragma once
#include <wx/wx.h>

enum ids {
	MAIN_FRAME = wxID_HIGHEST + 1,
	SETTINGS_FRAME = MAIN_FRAME + 100,
	PING_THREAD = SETTINGS_FRAME + 100,
	MENU_ITEMS = PING_THREAD + 100,
};