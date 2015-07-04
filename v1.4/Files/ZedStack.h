#pragma once

// Library: ZedStack.h

// Needed libraries.
#include <iostream>
#include <windows.h>

// Extra libraries in the project.
#include "Enum.h"

#define main _main_
int _main_();

namespace ZedStack {
	class SCREEN {
	private:
		int WIDTH;
		int HEIGHT;
		void rewriteStandards(int& oldWidth, int& oldHeight, int newWidth, int newHeight);
	public:
		SCREEN();
		SCREEN(int width, int height);
		~SCREEN();

		void getWidthAndHeight(int& retWidth, int& retHeight);
		void resize(int newWdth, int newHeight);
		void render();
		void close();
		void clear();
		int getWidth();
		int getHeight();
	};
	class MOUSE{
	private:
		int XAXIS;
		int YAXIS;
	public:
		MOUSE();
		MOUSE(int& retXAxis, int& retYAxis);
		~MOUSE();

		void getCoords(int& retXAxis, int& retYAxis);
		bool checkAndGetCoords(int& retXAxis, int& retYAxis);
		bool isInScreen();
		bool lButtonUp();
		bool lButtonDown();
		bool lButtonDoubleClick();
		bool lButtonSingleClick();
		bool rButtonUp();
		bool rButtonDown();
		bool rButtonDoubleClick();
		bool rbuttonSingleClick();
		bool mButtonUp();
		bool mButtonDown();
		bool mButtonDoubleClick();
		bool mbuttonSingleClick();
		int getX();
		int getY();
	};
	class GRID{
	private:
		int WIDTH;
		int HEIGHT;
		int TILE_WIDTH;
		int TILE_HEIGHT;
		int TILE_AMOUNT;
		int COLUMS_AMOUNT;
		int ROWS_AMOUNT;
	public:
		GRID();
		GRID(int rows, int colums, int tileSize);
		GRID(int rows, int colums, int tileWidth, int tileHeight);
		~GRID();

		void setRows(int rows);
		void setColums(int colums);
		void setRowsAndColums(int rows, int colums);
		void setTileSize(int tWidth, int tHeight);
		void setTileSize(int tSize);
		void load();
		void load(ZS_COLORS C);
		void renderTile(int xAxis, int yAxis);
		void renderTile(int xAxis, int yAxis, ZS_COLORS C);
		void renderTile(int tile);
		void renderTile(int tile, ZS_COLORS C);
		void clear();
		void clearAll();
		void clearAll(ZS_COLORS C);
		void getCoordsFromTile(int tile, int& xAxis, int& yAxis);
		void getCoordsFromTile(int tile, int& xStart, int& yStart, int& xFinal, int& yFinal);
		int getTileFromCoords(int xAxis, int yAxis);
		int getWidth();
		int getHeight();
		int getTileWidth();
		int getTileHeight();
		int getRows();
		int getColums();
	};

	void square			(SQ_FIGURE_TYPE FG, ZS_LINE_TYPE LT, int xAxis, int yAxis, int size,  int thickness);
	void square			(SQ_FIGURE_TYPE FG, ZS_LINE_TYPE LT, int xAxis, int yAxis, int size,  int thickness, ZS_COLORS C);
	void roundSquare	(SQ_FIGURE_TYPE FG, ZS_LINE_TYPE LT, int xAxis, int yAxis, int size,  int thickness, int wRound, int hRound);
	void roundSquare	(SQ_FIGURE_TYPE FG, ZS_LINE_TYPE LT, int xAxis, int yAxis, int size,  int thickness, int wRound, int hRound, ZS_COLORS C);
	void circle			(CI_FIGURE_TYPE FG, ZS_LINE_TYPE LT, int xAxis, int yAxis, int size,  int thickness);
	void circle			(CI_FIGURE_TYPE FG, ZS_LINE_TYPE LT, int xAxis, int yAxis, int size,  int thickness, ZS_COLORS C);
	void ellipse		(RE_FIGURE_TYPE FG, ZS_LINE_TYPE LT, int xAxis, int yAxis, int wSize, int hSize, int thickness);
	void ellipse		(RE_FIGURE_TYPE FG, ZS_LINE_TYPE LT, int xAxis, int yAxis, int wSize, int hSize, int thickness, ZS_COLORS C);
	void rectangle		(RE_FIGURE_TYPE FG, ZS_LINE_TYPE LT, int xAxis, int yAxis, int wSize, int hSize, int thickness);
	void rectangle		(RE_FIGURE_TYPE FG, ZS_LINE_TYPE LT, int xAxis, int yAxis, int wSize, int hSize, int thickness, ZS_COLORS C);
	void roundRectangle	(RE_FIGURE_TYPE FG, ZS_LINE_TYPE LT, int xAxis, int yAxis, int wSize, int hSize, int thickness, int wRound, int hRound);
	void roundRectangle	(RE_FIGURE_TYPE FG, ZS_LINE_TYPE LT, int xAxis, int yAxis, int wSize, int hSize, int thickness, int wRound, int hRound, ZS_COLORS C);

	void line	(int xAxis,  int yAxis,  int size,   int angle,  int thickness,  ZS_LINE_TYPE LT);
	void line	(int xAxis,  int yAxis,  int size,   int angle,  int thickness,  ZS_LINE_TYPE LT, ZS_COLORS C);
	void line	(int xStart, int yStart, int xFinal, int yFinal, int thickness, ZS_LINE_TYPE LT);
	void line	(int xStart, int yStart, int xFinal, int yFinal, int thickness, ZS_LINE_TYPE LT, ZS_COLORS C);
	void text	(int xAXis,  int yAxis,  const std::string line);
	void text	(int xAXis,  int yAxis,  const std::string line, ZS_COLORS C);
	void text	(int xAXis,  int yAxis,  const std::string line, ZS_TEXT_ALIGN TA);
	void text	(int xAXis,  int yAxis,  const std::string line, ZS_TEXT_ALIGN TA, ZS_COLORS C);
	void point	(int xAxis,  int yAxis);
	void point	(int xAxis,  int yAxis, ZS_COLORS C);

	int getPreviousColor();
	int getCurrentColor();
	void setColor	(ZS_COLORS C);
	void setRGBColor(short red, short green, short blue);
	void getRGBValue(short& red, short& green, short& blue);
	void getRGBValueFromColor(ZS_COLORS C, short& red, short& green, short& blue);
	short getRedValue	();
	short getGreenValue	();
	short getBlueValue	();
	short getRedValueFromColor	(ZS_COLORS color);
	short getGreenValueFromColor(ZS_COLORS color);
	short getBlueValueFromColor	(ZS_COLORS color);

	void waitFor(int value, ZS_TIME T);

	void setWindowTitle(LPCSTR newTitle);
	const std::string getWindowTitle();

	int setKey();
}
namespace ZedStack_cplx {

}
namespace ZedStack_LogicGates { // TODO test every definition on this namespace
#define isHiger(a, b)			if (a > b)
#define isHigerOrEqual(a, b)	if (a >= b)
#define isLower(a, b)			if (a < b)
#define isLowerOrEqual(a, b)	if (a <= b)
#define isEqual(a, b)			if (a = b)
#define isDiferent(a, b)		if (a != b)

#define NOT(a) !a

#define AND_2(a, b)		a & b
#define OR_2(a, b)		a | b
#define NAND_2(a, b)	!(a & b)
#define NOR_2(a, b)		!(a | b)
#define XOR_2(a, b)		(a + b) % 2
#define XNOR_2(a, b)	!((a + b) % 2)

#define AND_3(a, b, c)	a & b & c
#define OR_3(a, b, c)	a | b | c
#define NAND_3(a, b, c)	!(a & b & c)
#define NOR_3(a, b, c)	!(a | b | c)
#define XOR_3(a, b, c)	(a + b + c) % 2
#define XNOR_3(a, b, c)	!((a + b + c) % 2)
}
