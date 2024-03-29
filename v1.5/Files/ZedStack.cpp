/*┌──────────────────────────────────────────────────────────────────────────────────┐
┌─┼─┐  Welcome to...                                                               ┌─┼─┐
│ └─┘                                                                              └─┘ │
│     ███████╗███████╗██████╗ ███████╗████████╗ █████╗  ██████╗██╗  ██╗   ██╗  ██╗     │
│     ╚══███╔╝██╔════╝██╔══██╗██╔════╝╚══██╔══╝██╔══██╗██╔════╝██║ ██╔╝   ██║  ██║     │
│       ███╔╝ █████╗  ██║  ██║███████╗   ██║   ███████║██║     █████╔╝    ███████║     │
│      ███╔╝  ██╔══╝  ██║  ██║╚════██║   ██║   ██╔══██║██║     ██╔═██╗    ██╔══██║     │
│     ███████╗███████╗██████╔╝███████║   ██║   ██║  ██║╚██████╗██║  ██╗██╗██║  ██║     │
│     ╚══════╝╚══════╝╚═════╝ ╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝     │
│ ┌─┐                                                                              ┌─┐ │
└─┼─┘                                 ...a simple library for Win32 GUI projects.  └─┼─┘
  └──────────────────────────────────────────────────────────────────────────────────┘*/
// V 1.5

// File: ZedStack.cpp

#pragma once

// Library that is being defined
#include "ZedStack.h"

// Needed libraries.
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <windowsx.h>
#include <queue>
#include <process.h>

// Extra libraries in the proyect.
#include "Struct.h"
#include "Static.h"
#include "Define.h"
#include "Enum.h"
#include "Draw.h"

LRESULT CALLBACK windowProcedure (HWND, UINT, WPARAM, LPARAM);

/**  G L O B A L   V A R I A B L E S  **/
HWND            hWnd;                               // Main screen.
HBITMAP         hBitmap;                            // Bitmap of the screen.
HDC             hDCMem = NULL;                      // Device context.
LPCSTR          title = "ZedStackLibrary Project";  // Title of the window.
std::queue<int> _key;                               // Key tail.
int             stdWidth = 720;                     // Standard width of the screen.
int             stdHeight = 480;                    // Standard height of the screen.
int             xMouseAxis;                         // Current x coord of the mouse.
int             yMouseAxis;                         // Current y coord of the mouse.
bool            mouseIsInScreen;                    // Returns true if the mouse is inside of the screen.
bool            lButton[5];                         // Left button of the mouse.
bool            rButton[5];                         // Right button of the mouse.
bool            mButton[5];                         // Middle button of the mouse.
char            szClassName[] = "ZedStackWindow";   // Name for the window class.
/**  G L O B A L   V A R I A B L E S  **/

VOID thread (PVOID pVoid) {
    Sleep (50);
    _main_ ();
}
void mayCallMain () {
    static bool started = false;

    if (!started) {
        _beginthread (thread, 0, NULL);
        started = true;
    }
}
void realFrame (int width, int height, int& retWidth, int& retHeight) {
    RECT frame = { 0, 0, width, height };
    AdjustWindowRect (&frame, WS_OVERLAPPEDWINDOW, FALSE);
    retWidth = frame.right - frame.left;
    retHeight = frame.bottom - frame.top;
}
void newMemDC (int width, int height) {
    if (hDCMem != NULL) {
		DeleteObject (hBitmap);
		DeleteDC (hDCMem);
	}
	HDC hDC = GetDC (hWnd);
	hDCMem = CreateCompatibleDC (hDC);
	hBitmap = CreateCompatibleBitmap (hDC, width, height);
	SelectObject (hDCMem, hBitmap);
	SetBkMode (hDCMem, TRANSPARENT);
}
LRESULT CALLBACK windowProcedure (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
    	case WM_SIZE: {
    		RECT rect;
    		GetClientRect (hWnd, &rect);
    		int width = rect.right - rect.left;
    		int height = rect.bottom - rect.top;

    		if (width == 0 && height == 0) break;

    		if (hDCMem == NULL || width != stdWidth || height != stdHeight) {
    			newMemDC (width, height);
    			mayCallMain ();
    		}
    		break;
    	}
    	case WM_SIZING: {
    		RECT* rect = (RECT*)lParam;

    		int width, height;
    		realFrame (stdWidth, stdHeight, width, height);

    		switch (wParam) {
        		case WMSZ_BOTTOM:		{ rect->bottom = rect->top + height; break; }
        		case WMSZ_TOP:			{ rect->top = rect->bottom - height; break; }
        		case WMSZ_RIGHT:		{ rect->right = rect->left + width;  break; }
        		case WMSZ_LEFT:			{ rect->left = rect->right - width;  break; }
        		case WMSZ_TOPLEFT:		{ rect->top = rect->bottom - height; rect->left = rect->right - width; break; }
        		case WMSZ_TOPRIGHT:		{ rect->top = rect->bottom - height; rect->left = rect->right + width; break; }
        		case WMSZ_BOTTOMLEFT:	{ rect->top = rect->bottom + height; rect->left = rect->right - width; break; }
        		case WMSZ_BOTTOMRIGHT:	{ rect->top = rect->bottom + height; rect->left = rect->right + width; break; }
    		}
    	}
    	case WM_PAINT: {
    		PAINTSTRUCT paintStruct;
    		HDC hDC = BeginPaint (hWnd, &paintStruct);
    		SelectObject (hDCMem, hBitmap);

    		if (hBitmap != NULL) BitBlt (hDC, 0, 0, stdWidth, stdHeight, hDCMem, 0, 0, SRCCOPY);
    		EndPaint (hWnd, &paintStruct);
    		break;
    	}
    	case WM_KEYDOWN: {
    		bool keyValue = false;

    		//! A - Z:
    		keyValue |= (wParam >= 65 && wParam <= 90);

    		//! Escape:
    		keyValue |= (wParam == VK_ESCAPE);

    		//! Space bar:
    		keyValue |= (wParam == VK_SPACE);

    		//! Return:
    		keyValue |= (wParam == VK_RETURN);

    		//! Arrows:
    		keyValue |= (wParam == VK_LEFT || wParam == VK_RIGHT || wParam == VK_UP || wParam == VK_DOWN);

    		//! F1 - F10:
    		for (unsigned int i = 0; i < 10; i++) keyValue |= (wParam == (VK_F1 + i));

    		//! 0 - 9:
    		keyValue |= (wParam >= 48 && wParam <= 57);

    		//! NP-0 - NP-9:
    		keyValue |= (wParam >= 96 && wParam <= 105);

    		if (keyValue) _key.push (wParam);
    		break;
    	}
    	case WM_MOUSEMOVE: {
    		TRACKMOUSEEVENT mouseEvent;
    		mouseEvent.cbSize = sizeof (TRACKMOUSEEVENT);
    		mouseEvent.hwndTrack = hWnd;
    		mouseEvent.dwFlags = TME_LEAVE; // TODO do some research about "TME_HOVER"
    		mouseEvent.dwHoverTime = HOVER_DEFAULT;
    		TrackMouseEvent (&mouseEvent);

    		MOUSE_INSIDE = true;

    		xMouseAxis = GET_X_LPARAM (lParam);
    		yMouseAxis = GET_Y_LPARAM (lParam);

    		LBUTTON[UP]		=  !(wParam & MK_LBUTTON);
    		LBUTTON[DOWN]	=    wParam & MK_LBUTTON;
    		RBUTTON[UP]		=  !(wParam & MK_RBUTTON);
    		RBUTTON[DOWN]	= !!(wParam & MK_RBUTTON);
    		MBUTTON[UP]		=  !(wParam & MK_MBUTTON);
    		MBUTTON[DOWN]	= !!(wParam & MK_MBUTTON);
    		break;
    	}
    	case WM_MOUSELEAVE: {
    		MOUSE_INSIDE = false;
    		break;
    	}
    	case WM_MBUTTONUP: {
    		MBUTTON[UP] = true;
    		if (MBUTTON[SNGCK_1]) MBUTTON[SNGCK_2] = true;

    		MBUTTON[DOWN] = false;
    		MBUTTON[DBLCK] = false;
    		break;
    	}
    	case WM_MBUTTONDOWN: {
    		MBUTTON[DOWN] = true;
    		MBUTTON[SNGCK_1] = true;

    		MBUTTON[UP] = false;
    		break;
    	}
    	case WM_MBUTTONDBLCLK: {
    		MBUTTON[DBLCK] = true;

    		MBUTTON[SNGCK_1] = false;
    		MBUTTON[SNGCK_2] = false;
    		break;
    	}
    	case WM_LBUTTONUP: {
    		LBUTTON[UP] = true;
    		if (LBUTTON[SNGCK_1]) LBUTTON[SNGCK_2] = true;

    		LBUTTON[DOWN] = false;
    		LBUTTON[DBLCK] = false;
    		break;
    	}
    	case WM_LBUTTONDOWN: {
    		LBUTTON[DOWN] = true;
    		LBUTTON[SNGCK_1] = true;

    		LBUTTON[UP] = false;
    		break;
    	}
    	case WM_LBUTTONDBLCLK: {
    		LBUTTON[DBLCK] = true;

    		LBUTTON[SNGCK_1] = false;
    		LBUTTON[SNGCK_2] = false;
    		break;
    	}
    	case WM_RBUTTONUP: {
    		RBUTTON[UP] = true;
    		if (RBUTTON[SNGCK_1]) RBUTTON[SNGCK_2] = true;

    		RBUTTON[DOWN] = false;
    		RBUTTON[DBLCK] = false;
    		break;
    	}
    	case WM_RBUTTONDOWN: {
    		RBUTTON[DOWN] = true;
    		RBUTTON[SNGCK_1] = true;

    		RBUTTON[UP] = false;
    		break;
    	}
    	case WM_RBUTTONDBLCLK: {
    		MBUTTON[DBLCK] = true;

    		MBUTTON[SNGCK_1] = false;
    		MBUTTON[SNGCK_2] = false;
    		break;
    	}
    	case WM_DESTROY: {
    		DeleteObject (hBitmap);
    		DeleteDC (hDCMem);
    		PostQuitMessage (0);
    		break;
    	}
    	default: {
    		return DefWindowProc (hWnd, message, wParam, lParam);
    	}
    	}
	return 0;
}
int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil) {
	static WNDCLASSEX wincl;

	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = windowProcedure;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof (WNDCLASSEX);
	wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	//wincl.hIcon = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON1));
	// TODO Create new logos, menus and toolbars for the application if necessary.
	wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
	//wincl.hIconSm = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_SMALL));
	//wincl.hIcon = LoadIcon(hThisInstance, MAKEINTRESOURCE(IDI_ICON1));
	// TODO Create new logos, menus and toolbars for the application if necessary.
	wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
	//wincl.hIconSm = LoadIcon(hThisInstance, MAKEINTRESOURCE(IDI_SMALL));
	wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = (HBRUSH)GetStockObject (BLACK_BRUSH);

	if (!RegisterClassEx (&wincl)) return 0;

	int width, height;
	realFrame (stdWidth, stdHeight, width, height);
	hWnd = CreateWindowEx (
		0,					//! Basic window style.
		szClassName,		//! Name of the class.
		title,				//! Title.
		WS_OVERLAPPEDWINDOW,//! Default window.
		CW_USEDEFAULT,		//! Windows decides the position -
		CW_USEDEFAULT,		//! - where the window ends up on the screen.
		width,				//! Window's width.
		height,				//! Window's height.
		HWND_DESKTOP,		//! The window is a child-window to desktop.
		NULL,				//! No menu.
		hThisInstance,		//! Program Instance handler.
		NULL);				//! No Window Creation data.
	hBitmap = NULL;
	ShowWindow (hWnd, nFunsterStil);
	MSG	messages;

	while (GetMessage (&messages, NULL, 0, 0)) {
		TranslateMessage (&messages);
		DispatchMessage (&messages);
	}
	return messages.wParam;
}

COLORREF stdColor = RGB (255, 255, 255);

namespace ZedStack {
    SCREEN::SCREEN () {
        this -> WIDTH = stdWidth;
        this -> HEIGHT = stdHeight;
    }
    SCREEN::SCREEN (int width, int height) {
        resize (width, height);
    }
    SCREEN::~SCREEN () {
        // Do nothing.
    }
    void SCREEN::getWidthAndHeight (int& width, int& height) {
        width = this -> WIDTH;
        height = this -> HEIGHT;
    }
    void SCREEN::resize (int newWidth, int newHeight) {
        this -> rewriteStandards (stdWidth, stdHeight, newWidth, newHeight);
        this -> WIDTH = newWidth;
        this -> HEIGHT = newHeight;

        int retWidth, retHeight;
        realFrame (this -> WIDTH, this -> HEIGHT, retWidth, retHeight);
        SetWindowPos (hWnd, NULL, 0, 0, retWidth, retHeight, SWP_NOMOVE);
        newMemDC (retWidth, retHeight);
    }
    void SCREEN::render () {
        InvalidateRect (hWnd, NULL, FALSE);
    }
    void SCREEN::clear () {
        RECT rect;
        HBRUSH hBrush = CreateSolidBrush (RGB (0, 0, 0));
        SetRect (&rect, 0, 0, this -> WIDTH, this -> HEIGHT);
        FillRect (hDCMem, &rect, hBrush);
        DeleteObject (hBrush);
    }
    void SCREEN::close () {
        PostMessage (hWnd, WM_CLOSE, 0, 0);
    }
    void SCREEN::rewriteStandards (int& oldWidth, int& oldHeight, int newWidth, int newHeight) {
        oldWidth = newWidth;
        oldHeight = newHeight;
    }
    int SCREEN::getWidth () {
        return this -> WIDTH;
    }
    int SCREEN::getHeight () {
        return this -> HEIGHT;
    }

    MOUSE::MOUSE () {
        this -> XAXIS = xMouseAxis;
        this -> YAXIS = yMouseAxis;
    }
    MOUSE::MOUSE (int& xAxis, int& yAxis) {
        MOUSE();
        xAxis = this -> XAXIS;
        yAxis = this -> YAXIS;
    }
    MOUSE::~MOUSE () {
        // Do nothing.
    }
    void MOUSE::getCoords (int& xAxis, int& yAxis) {
        xAxis = this -> XAXIS;
        yAxis = this -> YAXIS;
    }
    bool MOUSE::isInScreen () {
        return MOUSE_INSIDE;
    }
    bool MOUSE::checkAndGetCoords (int& xAxis, int& yAxis) {
        if (MOUSE_INSIDE) {
            xAxis = xMouseAxis;
            yAxis = yMouseAxis;
            return true;
        }
        else {
            xAxis = 0;
            yAxis = 0;
            return false;
        }
    }
    bool MOUSE::lButtonUp () {
        if (LBUTTON [UP]) return true;
        else return false;
    }
    bool MOUSE::lButtonDown () {
        if (LBUTTON [DOWN]) return true;
        else return false;
    }
    bool MOUSE::lButtonDoubleClick () {
        if (LBUTTON [DBLCK]) return true;
        else return false;
    }
    bool MOUSE::lButtonSingleClick () {
        if (LBUTTON [SNGCK_1] & LBUTTON [SNGCK_2]) {
            LBUTTON [SNGCK_1] = false;
            LBUTTON [SNGCK_2] = false;
            return true;
        }
        else return false;
    }
    bool MOUSE::rButtonUp () {
        if (RBUTTON [UP]) return true;
        else return false;
    }
    bool MOUSE::rButtonDown () {
        if (RBUTTON [DOWN]) return true;
        else return false;
    }
    bool MOUSE::rButtonDoubleClick () {
        if (RBUTTON [DBLCK]) return true;
        else return false;
    }
    bool MOUSE::rbuttonSingleClick () {
        if (RBUTTON [SNGCK_1] & RBUTTON [SNGCK_2]) {
            RBUTTON [SNGCK_1] = false;
            RBUTTON [SNGCK_2] = false;
            return true;
        }
        else return false;
    }
    bool MOUSE::mButtonUp () {
        if (MBUTTON [UP]) return true;
        else return false;
    }
    bool MOUSE::mButtonDown () {
        if (MBUTTON [DOWN]) return true;
        else return false;
    }
    bool MOUSE::mButtonDoubleClick () {
        if (MBUTTON [DBLCK]) return true;
        else return false;
    }
    bool MOUSE::mbuttonSingleClick () {
        if (MBUTTON [SNGCK_1] & MBUTTON [SNGCK_2]) {
            MBUTTON [SNGCK_1] = false;
            MBUTTON [SNGCK_2] = false;
            return true;
        }
        else return false;
    }
    int MOUSE::getX () {
        return xMouseAxis;
    }
    int MOUSE::getY () {
        return yMouseAxis;
    }

    GRID::GRID () {
        this -> WIDTH = 0;
        this -> HEIGHT = 0;
        this -> TILE_WIDTH = 0;
        this -> TILE_HEIGHT = 0;
        this -> TILE_AMOUNT = 0;
        this -> COLUMNS = 0;
        this -> ROWS = 0;
    }
    GRID::GRID (int rows, int columns, int tileSize) {
        this -> WIDTH           = tileSize * columns;
        this -> HEIGHT          = tileSize * rows;
        this -> TILE_WIDTH      = tileSize;
        this -> TILE_HEIGHT     = tileSize;
        this -> TILE_AMOUNT     = rows * columns;
        this -> COLUMNS         = columns;
        this -> ROWS            = rows;
    }
    GRID::GRID (int rows, int columns, int tileWidth, int tileHeight) {
        this -> WIDTH           = tileWidth * columns;
        this -> HEIGHT          = tileHeight * rows;
        this -> TILE_WIDTH      = tileWidth;
        this -> TILE_HEIGHT     = tileHeight;
        this -> TILE_AMOUNT     = rows * columns;
        this -> COLUMNS         = columns;
        this -> ROWS            = rows;
    }
    GRID::~GRID () {
        // Do nothing.
    }
    void GRID::setRows (int rows) {
        this -> ROWS = rows;
    }
    void GRID::setColumns (int columns) {
        this -> COLUMNS = columns;
    }
    void GRID::setRowsAndColumns (int rows, int columns) {
        setRows (rows);
        setColumns (columns);
    }
    void GRID::setTileSize (int tileWidth, int tileHeight) {
        this -> TILE_WIDTH = tileWidth;
        this -> TILE_HEIGHT = tileHeight;
    }
    void GRID::setTileSize (int tileSize) {
        setTileSize (tileSize, tileSize);
    }
    void GRID::load (ZS_COLORS C) {
        for (int y = 0; y < getHeight (); y += getTileHeight ()) {
            line (0, y, getWidth () + 1, y, 1, SOLID, C);
        }
        for (int x = 0; x < getWidth (); x += getTileWidth ()) {
            line (x, 0, x, getHeight (), 1, SOLID, C);
        }
    }
    void GRID::load () {
        load (CURRENT_COLOR);
    }
    void GRID::renderTile (int xTileAxis, int yTileAxis, ZS_COLORS C) {
        rectangle (
            RE_D_S,
            FILLED,
            xTileAxis * getTileWidth (),
            yTileAxis * getTileHeight (),
            getTileWidth (),
            getTileHeight (),
            0,
            C);
    }
    void GRID::renderTile (int xTileAxis, int yTileAxis) {
        renderTile (xTileAxis, yTileAxis, CURRENT_COLOR);
    }
    void GRID::renderTile (int tile, ZS_COLORS C) {
        renderTile (tile % getColumns (), tile * getRows (), C);
    }
    void GRID::renderTile (int tile) {
        renderTile (tile, CURRENT_COLOR);
    }
    // TODO: Finish this class. branch and create a "master" class "TABLE_BASIC" for "GRID", "PIXEL_GRID" and "TABLE".
    void GRID::writeTile(int xTileAxis, int yTileAxis, std::string line, ZS_COLORS C, ZS_TEXT_ALIGN TA) {
        int xAxis, yAxis;
        getCoordsFromTile (xTileAxis, yTileAxis, xAxis, yAxis);

        switch (TA) {
            case TOP_LEFT: { break; /* Do nothing... (by default) */ }
            case TOP_RIGHT: { xAxis += getTileWidth (); break; }
            case TOP_CENTER: { xAxis += getTileWidth () / 2; break; }

            case BOTTOM_LEFT: { yAxis += getTileHeight (); break; }
            case BOTTOM_RIGHT: { xAxis += getTileWidth (); yAxis += getTileHeight (); break; }
            case BOTTOM_CENTER: { xAxis += getTileWidth () / 2; yAxis += getTileHeight (); break; }

            case TOP: { break; /* Do nothing... (By default) */ }
            case LEFT: { break; /* Do nothing... (By default  ) */ }
            case BOTTOM: { yAxis += getTileHeight (); break; }
            case RIGHT: { xAxis += getTileWidth (); break; }
            case CENTER: { xAxis += getTileWidth () / 2; break; }
        }
		text (xAxis, yAxis, line);
    }
    void GRID::writeTile (int xTileAxis, int yTileAxis, std::string text, ZS_TEXT_ALIGN TA) {
        writeTile (xTileAxis, yTileAxis, text, CURRENT_COLOR, TA);
    }

    void GRID::getCoordsFromTile (int tile, int& xAxis, int& yAxis) {
        xAxis = (tile % getColumns ()) * getTileWidth ();
        yAxis = (tile / getColumns ()) * getTileHeight ();
    }
    void GRID::getCoordsFromTile (int tile, int& xStart, int& yStart, int& xFinal, int& yFinal) {
        getCoordsFromTile (tile, xStart, yStart);
        xFinal = xStart + getTileWidth ();
        yFinal = yStart + getTileHeight ();
    }
    void GRID::getCoordsFromTile (int xTileAxis, int yTileAxis, int& xAxis, int& yAxis) {
        getCoordsFromTile (getTileFromCoords (xTileAxis, yTileAxis), xAxis, yAxis);
    }
    void GRID::getCoordsFromTile (int xTileAxis, int yTileAxis, int& xStart, int& yStart, int& xFinal, int& yFinal) {
        getCoordsFromTile (xTileAxis, yTileAxis, xStart, yStart);
        xFinal = xStart + getTileWidth ();
        yFinal = yStart + getTileHeight ();
    }
    int GRID::getTileFromCoords (int xTileAxis, int yTileAxis) {
        return (yTileAxis * getColumns ()) + xTileAxis;
    }
    int GRID::getWidth () {
        return this -> WIDTH;
    }
    int GRID::getHeight () {
        return this -> HEIGHT;
    }
    int GRID::getTileWidth () {
        return this -> TILE_WIDTH;
    }
    int GRID::getTileHeight () {
        return this -> TILE_HEIGHT;
    }
    int GRID::getRows () {
        return this -> ROWS;
    }
    int GRID::getColumns () {
        return this -> COLUMNS;
    }

    void SQ_figureTypeSelector (SQ_FIGURE_TYPE figureType, COORDS& figure, int xAxis, int yAxis, int size, bool round, int wRound, int hRound) {
        if (round) {
            figure.wRound = wRound;
            figure.hRound = hRound;
        }
        switch (figureType) {
            case SQ_R: {
                figure.xStart = xAxis - size;
                figure.yStart = yAxis - size;
                figure.xFinal = xAxis + size;
                figure.yFinal = yAxis + size;
                break;
            }
            case SQ_D: {
                figure.xStart = xAxis - halfFloat (size);
                figure.yStart = yAxis - halfFloat (size);
                figure.xFinal = xAxis + halfFloat (size);
                figure.yFinal = yAxis + halfFloat (size);
                break;
            }
            case SQ_R_S: {
                figure.xStart = xAxis;
                figure.yStart = yAxis;
                figure.xFinal = xAxis + doubleFloat (size);
                figure.yFinal = yAxis + doubleFloat (size);
                break;
            }
            case SQ_R_F: {
                figure.xStart = xAxis - doubleFloat (size);
                figure.yStart = yAxis - doubleFloat (size);
                figure.xFinal = xAxis;
                figure.yFinal = yAxis;
                break;
            }
            case SQ_D_S: {
                figure.xStart = xAxis;
                figure.yStart = yAxis;
                figure.xFinal = xAxis + size;
                figure.yFinal = yAxis + size;
                break;
            }
            case SQ_D_F: {
                figure.xStart = xAxis - size;
                figure.yStart = yAxis - size;
                figure.xFinal = xAxis;
                figure.yFinal = yAxis;
                break;
        }
        }
    }
    void RE_figureTypeSelector (RE_FIGURE_TYPE figureType, COORDS& figure, int xAxis, int yAxis, int wSize, int hSize, bool round, int wRound, int hRound) {
        if (round) {
            figure.wRound = wRound;
            figure.hRound = hRound;
        }
        switch (figureType) {
            case ZedStack::RE_FREE: {
                figure.xStart = xAxis;
                figure.yStart = yAxis;
                figure.xFinal = wSize;
                figure.yFinal = hSize;
                break;
            }
            case ZedStack::RE_R: {
                figure.xStart = xAxis - wSize;
                figure.yStart = yAxis - hSize;
                figure.xFinal = xAxis + wSize;
                figure.yFinal = yAxis + hSize;
                break;
            }
            case ZedStack::RE_D: {
                figure.xStart = xAxis - halfFloat (wSize);
                figure.yStart = yAxis - halfFloat (hSize);
                figure.xFinal = xAxis + halfFloat (wSize);
                figure.yFinal = yAxis + halfFloat (hSize);
                break;
            }
            case ZedStack::RE_R_S: {
                figure.xStart = xAxis;
                figure.yStart = yAxis;
                figure.xFinal = xAxis + doubleFloat (wSize);
                figure.yFinal = yAxis + doubleFloat (hSize);
                break;
            }
            case ZedStack::RE_R_F: {
                figure.xStart = xAxis - doubleFloat (wSize);
                figure.yStart = yAxis - doubleFloat (hSize);
                figure.xFinal = xAxis;
                figure.yFinal = yAxis;
                break;
            }
            case ZedStack::RE_D_S: {
                figure.xStart = xAxis;
                figure.yStart = yAxis;
                figure.xFinal = xAxis + wSize;
                figure.yFinal = yAxis + hSize;
                break;
            }
            case ZedStack::RE_D_F: {
                figure.xStart = xAxis - wSize;
                figure.yStart = yAxis - hSize;
                figure.xFinal = wSize;
                figure.yFinal = hSize;
                break;
            }
        }
    }
    void CI_figureTypeSelector (CI_FIGURE_TYPE figureType, COORDS& figure, int xAxis, int yAxis, int size) {
        switch (figureType) {
            case ZedStack::CI_R: {
                figure.xAxis = xAxis;
                figure.yAxis = yAxis;
                figure.radius = size;
                break;
            }
            case ZedStack::CI_D: {
                figure.xAxis = xAxis;
                figure.yAxis = yAxis;
                figure.radius = halfFloat (size);
                break;
            }
            case ZedStack::CI_R_S: {
                figure.xAxis = xAxis + size;
                figure.yAxis = yAxis + size;
                figure.radius = size;
                break;
            }
            case ZedStack::CI_R_F: {
                figure.xAxis = xAxis - size;
                figure.yAxis = yAxis - size;
                figure.radius = size;
                break;
            }
            case ZedStack::CI_D_S: {
                figure.xAxis = xAxis + halfFloat (size);
                figure.yAxis = yAxis + halfFloat (size);
                figure.radius = halfFloat (size);
                break;
            }
            case ZedStack::CI_D_F: {
                figure.xAxis = xAxis - halfFloat (size);
                figure.yAxis = yAxis - halfFloat (size);
                figure.radius = halfFloat (size);
                break;
            }
        }
    }
    void EL_figureTypeSelector (RE_FIGURE_TYPE figureType, COORDS& figure, int xAxis, int yAxis, int wSize, int hSize) {
        switch (figureType) {
            case ZedStack::RE_FREE: {
                figure.xStart = xAxis;
                figure.yStart = yAxis;
                figure.xFinal = wSize;
                figure.yFinal = hSize;
                break;
            }
            case ZedStack::RE_R: {
                figure.xStart = xAxis - wSize;
                figure.yStart = yAxis - hSize;
                figure.xFinal = xAxis + wSize;
                figure.yFinal = yAxis + hSize;
                break;
            }
            case ZedStack::RE_D: {
                figure.xStart = xAxis - halfFloat (wSize);
                figure.yStart = yAxis - halfFloat (hSize);
                figure.xFinal = xAxis + halfFloat (wSize);
                figure.yFinal = yAxis + halfFloat (hSize);
                break;
            }
            case ZedStack::RE_R_S: {
                figure.xStart = xAxis;
                figure.yStart = yAxis;
                figure.xFinal = xAxis + doubleFloat (wSize);
                figure.yFinal = yAxis + doubleFloat (hSize);
                break;
            }
            case ZedStack::RE_R_F: {
                figure.xStart = xAxis - doubleFloat (wSize);
                figure.yStart = yAxis - doubleFloat (hSize);
                figure.xFinal = xAxis;
                figure.yFinal = yAxis;
                break;
            }
            case ZedStack::RE_D_S: {
                figure.xStart = xAxis;
                figure.yStart = yAxis;
                figure.xFinal = xAxis + wSize;
                figure.yFinal = yAxis + hSize;
                break;
            }
            case ZedStack::RE_D_F: {
                figure.xStart = xAxis - wSize;
                figure.yStart = yAxis - hSize;
                figure.xFinal = xAxis;
                figure.yFinal = yAxis;
                break;
            }
        }
    }
    void lineTypeSelector (ZS_LINE_TYPE lineType, HPEN& pen, int thickness, COLORREF color) {
		switch (lineType) {
    		case SOLID:
    		case BORDER: {
    			pen = CreatePen  (PS_SOLID, thickness, color);
    			break;
            }
    		case DOTTED: {
    			pen = CreatePen  (PS_DOT, thickness, color);
    			break;
            }
    		case DASHED: {
    			pen = CreatePen  (PS_DASH, thickness, color);
    			break;
            }
    		case DASHED_DOT: {
    			pen = CreatePen  (PS_DASHDOT, thickness, color);
    			break;
            }
    		case DASHED_DOT_DOT: {
    			pen = CreatePen  (PS_DASHDOTDOT, thickness, color);
    			break;
            }
            case FILLED: { break; }
		}
	}

    void square (SQ_FIGURE_TYPE figureType, ZS_LINE_TYPE lineType, int xAxis, int yAxis, int size,  int thickness) {
		COORDS figure;
		SQ_figureTypeSelector (figureType, figure, xAxis, yAxis, size, false, 0, 0);

		if (lineType != FILLED) {
			HPEN hPen;
			lineTypeSelector (lineType, hPen, thickness, stdColor);
			NORMAL_RECT (figure, hDCMem, hPen);
		}
		else FILLED_RECT (figure, hDCMem, stdColor);
	}
	void square (SQ_FIGURE_TYPE figureType, ZS_LINE_TYPE lineType, int xAxis, int yAxis, int size,  int thickness, ZS_COLORS C) {
		COORDS figure;
		SQ_figureTypeSelector (figureType, figure, xAxis, yAxis, size, false, 0, 0);

		if (lineType != FILLED) {
			HPEN hPen;
			lineTypeSelector (lineType, hPen, thickness, C);
			NORMAL_RECT (figure, hDCMem, hPen);
		}
		else FILLED_RECT (figure, hDCMem, stdColor);
	}
	void roundSquare (SQ_FIGURE_TYPE figureType, ZS_LINE_TYPE lineType, int xAxis, int yAxis, int size,  int thickness, int wRound, int hRound) {
		COORDS figure;
		SQ_figureTypeSelector (figureType, figure, xAxis, yAxis, size, true, wRound, hRound);

		figure.xFinal += 1;
		figure.yFinal += 1;

		if (lineType != FILLED) {
			HPEN hPen;
			lineTypeSelector (lineType, hPen, thickness, stdColor);
			NORMAL_ROUND_RECT (figure, hDCMem, hPen);
		}
		else FILLED_ROUND_RECT (figure, hDCMem, stdColor);
	}
	void roundSquare (SQ_FIGURE_TYPE figureType, ZS_LINE_TYPE lineType, int xAxis, int yAxis, int size,  int thickness, int wRound, int hRound, ZS_COLORS C) {
		COORDS figure;
		SQ_figureTypeSelector (figureType, figure, xAxis, yAxis, size, true, wRound, hRound);

		figure.xFinal += 1;
		figure.yFinal += 1;

		if (lineType != FILLED) {
			HPEN hPen;
			lineTypeSelector (lineType, hPen, thickness, C);
			NORMAL_ROUND_RECT (figure, hDCMem, hPen);
		}
		else FILLED_ROUND_RECT (figure, hDCMem, stdColor);
	}
	void circle (CI_FIGURE_TYPE figureType, ZS_LINE_TYPE lineType, int xAxis, int yAxis, int size,  int thickness) {
		COORDS figure;
		CI_figureTypeSelector (figureType, figure, xAxis, yAxis, size);

		if (lineType != FILLED) {
			HPEN hPen;
			lineTypeSelector (lineType, hPen, thickness, stdColor);
			NORMAL_CIRC (figure, hDCMem, hPen);
		}
		else FILLED_CIRC (figure, hDCMem, stdColor);
	}
	void circle (CI_FIGURE_TYPE figureType, ZS_LINE_TYPE lineType, int xAxis, int yAxis, int size,  int thickness, ZS_COLORS C) {
		COORDS figure;
		CI_figureTypeSelector (figureType, figure, xAxis, yAxis, size);

		if (lineType != FILLED) {
			HPEN hPen;
			lineTypeSelector (lineType, hPen, thickness, C);
			NORMAL_CIRC (figure, hDCMem, hPen);
		}
		else FILLED_CIRC (figure, hDCMem, stdColor);
	}
	void ellipse (RE_FIGURE_TYPE figureType, ZS_LINE_TYPE lineType, int xAxis, int yAxis, int wSize, int hSize, int thickness) {
		COORDS figure;
		EL_figureTypeSelector (figureType, figure, xAxis, yAxis, wSize, hSize);

		if (lineType != FILLED) {
			HPEN hPen;
			lineTypeSelector (lineType, hPen, thickness, stdColor);
			NORMAL_ELLI (figure, hDCMem, hPen);
		}
		else FILLED_ELLI (figure, hDCMem, stdColor);
	}
	void ellipse (RE_FIGURE_TYPE figureType, ZS_LINE_TYPE lineType, int xAxis, int yAxis, int wSize, int hSize, int thickness, ZS_COLORS C) {
		COORDS figure;
		EL_figureTypeSelector (figureType, figure, xAxis, yAxis, wSize, hSize);

		if (lineType != FILLED) {
			HPEN hPen;
			lineTypeSelector (lineType, hPen, thickness, C);
			NORMAL_ELLI (figure, hDCMem, hPen);
		}
		else FILLED_ELLI (figure, hDCMem, stdColor);
	}
	void rectangle (RE_FIGURE_TYPE figureType, ZS_LINE_TYPE lineType, int xAxis, int yAxis, int wSize, int hSize, int thickness) {
		COORDS figure;
		RE_figureTypeSelector (figureType, figure, xAxis, yAxis, wSize, hSize, false, 0, 0);

		if (lineType != FILLED) {
			HPEN hPen;
			lineTypeSelector (lineType, hPen, thickness, stdColor);
			NORMAL_RECT (figure, hDCMem, hPen);
		}
		else FILLED_RECT (figure, hDCMem, stdColor);
	}
	void rectangle (RE_FIGURE_TYPE figureType, ZS_LINE_TYPE lineType, int xAxis, int yAxis, int wSize, int hSize, int thickness, ZS_COLORS C) {
		COORDS figure;
		RE_figureTypeSelector (figureType, figure, xAxis, yAxis, wSize, hSize, false, 0, 0);

		if (lineType != FILLED) {
			HPEN hPen;
			lineTypeSelector (lineType, hPen, thickness, colours[C]);
			NORMAL_RECT (figure, hDCMem, hPen);
		}
		else FILLED_RECT (figure, hDCMem, colours[C]);
	}
	void roundRectangle (RE_FIGURE_TYPE figureType, ZS_LINE_TYPE lineType, int xAxis, int yAxis, int wSize, int hSize, int thickness, int wRound, int hRound) {
		COORDS figure;
		RE_figureTypeSelector (figureType, figure, xAxis, yAxis, wSize, hSize, true, wRound, hRound);

		figure.xFinal += 1;
		figure.yFinal += 1;

		if (lineType != FILLED) {
			HPEN hPen;
			lineTypeSelector (lineType, hPen, thickness, stdColor);
			NORMAL_ROUND_RECT (figure, hDCMem, hPen);
		}
		else FILLED_ROUND_RECT (figure, hDCMem, stdColor);
	}
	void roundRectangle (RE_FIGURE_TYPE figureType, ZS_LINE_TYPE lineType, int xAxis, int yAxis, int wSize, int hSize, int thickness, int wRound, int hRound, ZS_COLORS C) {
		COORDS figure;
		RE_figureTypeSelector (figureType, figure, xAxis, yAxis, wSize, hSize, true, wRound, hRound);

		figure.xFinal += 1;
		figure.yFinal += 1;

		if (lineType != FILLED) {
			HPEN hPen;
			lineTypeSelector (lineType, hPen, thickness, C);
			NORMAL_ROUND_RECT (figure, hDCMem, hPen);
		}
		else FILLED_ROUND_RECT (figure, hDCMem, stdColor);
	}

    void line (int xStart, int yStart, int xFinal, int yFinal, int thickness, ZS_LINE_TYPE lineType) {
		BeginPath (hDCMem);
		MoveToEx (hDCMem, int (xStart), int (yStart), NULL);
		LineTo (hDCMem, int (xFinal), int (yFinal));
		EndPath (hDCMem);
		//HPEN hPen = CreatePen (PS_DOT, thickness, stdColor);
		HPEN hPen;
		lineTypeSelector (lineType, hPen, thickness, stdColor);
		DELETE_LINE (hPen, hDCMem, hPen);
	}
	void line (int xStart, int yStart, int xFinal, int yFinal, int thickness, ZS_LINE_TYPE lineType, ZS_COLORS color) {
		BeginPath (hDCMem);
		MoveToEx (hDCMem, int (xStart), int (yStart), NULL);
		LineTo (hDCMem, int (xFinal), int (yFinal));
		EndPath (hDCMem);

		// HPEN hPen = CreatePen (PS_SOLID, thickness, colours[color]);
		HPEN hPen;
		lineTypeSelector (lineType, hPen, thickness, colours[color]);
		DELETE_LINE (hPen, hDCMem, hPen);
	}
	void point (int xAxis,  int yAxis) {
		SetPixel (hDCMem, int (xAxis), int (yAxis), stdColor);
	}
	void point (int xAxis,  int yAxis, ZS_COLORS color) {
		SetPixel (hDCMem, int (xAxis), int (yAxis), colours[color]);
	}
	void text (int xAxis,  int yAxis, const std::string line) {
		SetTextColor (hDCMem, stdColor);
		TextOut (hDCMem, int (xAxis), int (yAxis), line.c_str (), int (line.size ()));
	}
	void text (int xAxis,  int yAxis, const std::string line, ZS_COLORS color) {
		SetTextColor (hDCMem, colours[color]);
		TextOut (hDCMem, int (xAxis), int (yAxis), line.c_str (), int (line.size ()));
	}
	void text (int xAxis,  int yAxis, const std::string line, ZS_TEXT_ALIGN textAlign) {
		SetTextAlign (hDCMem, textAlignSates[textAlign]);
		SetTextColor (hDCMem, stdColor);
		TextOut (hDCMem, int (xAxis), int (yAxis), line.c_str (), int (line.size ()));
	}
	void text (int xAxis,  int yAxis, const std::string line, ZS_TEXT_ALIGN textAlign, ZS_COLORS color) {
		SetTextAlign (hDCMem, textAlignSates[textAlign]);
		SetTextColor (hDCMem, colours[color]);
		TextOut (hDCMem, int (xAxis), int (yAxis), line.c_str (), int (line.size ()));

	}
} /* ZedStack */
