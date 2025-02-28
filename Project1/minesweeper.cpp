#include "minesweeper.h"

void MineSweeper::setCell(int x, int y, bool to)
{
	cellsdata[(y * rowsize) + x] = to;
}

bool MineSweeper::getCell(int x, int y)
{
	return cellsdata[(y * rowsize) + x];
}

void MineSweeper::setFlag(int x, int y, bool to)
{
	flagsdata[(y * rowsize) + x] = to;
}

bool MineSweeper::getFlag(int x, int y)
{
	return flagsdata[(y * rowsize) + x];
}

void MineSweeper::setNeighborCount(int x, int y, int to)
{
	neighborcounts[(y * rowsize) + x] = to;
}

int MineSweeper::getNeighborCount(int x, int y)
{
	return neighborcounts[(y * rowsize) + x];
}

void MineSweeper::setHidden(int x, int y, bool to)
{
	hiddendata[(y * rowsize) + x] = to;
}

bool MineSweeper::getHidden(int x, int y)
{
	return hiddendata[(y * rowsize) + x];
}

void MineSweeper::initializeGrid()
{
	srand(time(0));

	minecount = 0;
	flagcount = 0;

	for (int i = 0;i<gridsize;i++) {
		if ((rand() % 12) <= 1) {
			cellsdata[i] = true;
			minecount++;
		}
		else
		{
			cellsdata[i] = false;
		}
		flagsdata[i] = false;
		hiddendata[i] = true;
	}

	for (int x = 0;x < rowsize;x++) {
		for (int y = 0;y < rowsize;y++) {
			setNeighborCount(x, y, getNeighborLandmineCount(x, y));
		}
	}

	gameOver = false;
	gameWon = false;
}

void MineSweeper::revealCell(int x, int y)
{
	if (getFlag(x, y)) return;

	setHidden(x, y, false);
	if (getNeighborCount(x, y) == 0 && !getCell(x, y)) {
		revealNeighbors(x, y);
	}

	if (getCell(x, y)) {
		gameOver = true;
		gameWon = false;
	}
	else if (allMinesFound()) {
		gameOver = true;
		gameWon = true;
	}
}

void MineSweeper::revealNeighbors(int x, int y)
{
	int x3 = 0, y3 = 0;
	for (int y2 = -1;y2 < 2;y2++) {
		for (int x2 = -1;x2 < 2;x2++) {
			if (x2 == 0 && y2 == 0) continue;
			x3 = x + x2;
			y3 = y + y2;
			if (x3 < 0 || x3 >= rowsize || y3 < 0 || y3 >= rowsize) continue;
			if (getHidden(x3, y3)) {
				revealCell(x3, y3);
			}
		}
	}
}

void MineSweeper::toggleFlag(int x, int y)
{
	if (!getHidden(x, y)) return;
	if (getFlag(x, y)) flagcount--;
	else flagcount++;
	setFlag(x, y, !getFlag(x, y));
}

bool MineSweeper::getNeighbor(int x, int y, int offx, int offy)
{
	x += offx;
	y += offy;

	if (x < 0 || x >= rowsize || y < 0 || y >= rowsize) {
		return false;
	}

	return getCell(x, y);
}

int MineSweeper::getNeighborLandmineCount(int x, int y)
{
	int count = 0;

	for (int y2 = -1;y2 < 2;y2++) {
		for (int x2 = -1;x2 < 2;x2++) {
			if (x2 == 0 && y2 == 0) continue;
			if (getNeighbor(x, y, x2, y2)) count++;
		}
	}

	return count;
}

bool MineSweeper::allMinesFound()
{
	for (int i = 0;i < gridsize;i++) {
		if (!cellsdata[i] && hiddendata[i]) {
			return false;
		}
	}
	return true;
}

void MineSweeper::drawGrid(HDC hdc,int winwidth,int winheight)
{
	Gdiplus::Graphics gf(hdc);

	Gdiplus::FontFamily fontFamily(L"Times New Roman");
	Gdiplus::Font font(&fontFamily, 20, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF pointF;

	Gdiplus::Color black = Gdiplus::Color(0, 0, 0);
	Gdiplus::Color darkgray = Gdiplus::Color(100, 100, 100);
	Gdiplus::Color gray = Gdiplus::Color(200, 200, 200);
	Gdiplus::Color white = Gdiplus::Color(255, 255, 255);
	Gdiplus::Color lightblue = Gdiplus::Color(240, 240, 255);
	Gdiplus::Color red = Gdiplus::Color(255, 0, 0);
	Gdiplus::Color orange = Gdiplus::Color(240, 165, 0);

	Gdiplus::SolidBrush brush(darkgray);
	Gdiplus::Pen pen(darkgray);
	pen.SetWidth(5);

	int xcoord, ycoord;

	for (int x = 0;x < rowsize;x++) {
		for (int y = 0;y < rowsize;y++) {
			xcoord = (x * cellsize) + offset;
			ycoord = (y * cellsize) + offset;

			gf.DrawRectangle(&pen, xcoord, ycoord, cellsize, cellsize);

			if (getHidden(x, y)) brush.SetColor(gray);
			else brush.SetColor((getNeighborCount(x, y) == 0) ? lightblue : white);
			if (getCell(x, y) && !getHidden(x, y)) brush.SetColor(red);
			if (getFlag(x, y)) brush.SetColor(orange);

			gf.FillRectangle(&brush, xcoord, ycoord, cellsize, cellsize);

			if (getHidden(x, y)) continue;
			if (getCell(x, y) && !getHidden(x, y)) continue;

			pointF.X = xcoord;
			pointF.Y = ycoord;
			brush.SetColor(black);

			const wchar_t* str{};
			switch (getNeighborCount(x, y)) {
			case 0: str = L"0"; break;
			case 1: str = L"1"; break;
			case 2: str = L"2"; break;
			case 3: str = L"3"; break;
			case 4: str = L"4"; break;
			case 5: str = L"5"; break;
			case 6: str = L"6"; break;
			case 7: str = L"7"; break;
			case 8: str = L"8"; break;
			}

			gf.DrawString(str, -1, &font, pointF, &brush); //Gdiplus text rendering is slow, doesnt appear to bake characters (ew)
		}
	}

	std::string str2 = "Mines: " + std::to_string(minecount);
	std::wstring widestr = std::wstring(str2.begin(), str2.end());
	const wchar_t* widecstr = widestr.c_str();
	pointF.X = pointF.X = getGridRenderSize() + 30 + offset;
	pointF.Y = 300;
	gf.DrawString(widecstr, -1, &font, pointF, &brush);

	str2 = "Flags: " + std::to_string(flagcount);
	widestr = std::wstring(str2.begin(), str2.end());
	widecstr = widestr.c_str();
	pointF.Y = 350;
	gf.DrawString(widecstr, -1, &font, pointF, &brush);

	if (gameOver) {
		pointF.X = getGridRenderSize() + 85 + offset;
		pointF.Y = cellsize * 2;
		gf.DrawString(L"click to reset", -1, &font, pointF, &brush);
		Gdiplus::Font font(&fontFamily, 30, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		pointF.X = getGridRenderSize() + 30;
		pointF.Y = cellsize;
		if (gameWon) gf.DrawString(L"Good job!", -1, &font, pointF, &brush); // I also don't like that the "string" needs to be a const wchar_t*, which is notably not a string, at least by modern sensibilities
		else gf.DrawString(L"You've exploded!", -1, &font, pointF, &brush);
	}
}

int MineSweeper::mouseCoordToGrid(int val)
{
	val -= offset;
	return (val - (val % cellsize)) / cellsize;
}

int MineSweeper::getGridRenderSize()
{
	return rowsize * cellsize;
}

void MineSweeper::lClickInput(int x, int y)
{
	if(!gameOver)
	{
		revealCell(mouseCoordToGrid(x), mouseCoordToGrid(y));
	}
	else
	{
		initializeGrid();
	}
}

void MineSweeper::rClickInput(int x, int y)
{
	if (!gameOver)
	{
		toggleFlag(mouseCoordToGrid(x), mouseCoordToGrid(y));
	}
	else
	{
		initializeGrid();
	}
}