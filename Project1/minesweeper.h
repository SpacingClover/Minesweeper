#pragma once

#include <windows.h>
#include <gdiplus.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

class MineSweeper {

private:
	static const int rowsize = 15;
	static constexpr int gridsize = rowsize * rowsize;
	static const int cellsize = 30;
	static const int offset = 20;
	bool cellsdata[gridsize];
	bool flagsdata[gridsize];
	bool hiddendata[gridsize];
	int neighborcounts[gridsize]; //structure for saving number display data, so neighbors aren't parsed at draw time
	int minecount;
	int flagcount;
	bool gameOver = false;
	bool gameWon = false;

private:
	void setCell(int x, int y, bool to);
	bool getCell(int x, int y);
	void setFlag(int x, int y, bool to);
	bool getFlag(int x, int y);
	void setNeighborCount(int x, int y, int to);
	int getNeighborCount(int x, int y);
	void setHidden(int x, int y, bool to);
	bool getHidden(int x, int y);

public:
	void initializeGrid();
	void revealCell(int x, int y);
	void revealNeighbors(int x, int y);
	void toggleFlag(int x, int y);
	void drawGrid(HDC hdc,int winwidth, int winheight);
	void lClickInput(int x, int y);
	void rClickInput(int x, int y);

private:
	int mouseCoordToGrid(int val);
	int getGridRenderSize();
	bool getNeighbor(int x, int y, int offx, int offy);
	int getNeighborLandmineCount(int x, int y);
	bool allMinesFound();
};