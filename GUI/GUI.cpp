#include "GUI.h"

//////////////////////////////////////////////////////////////////////////////////////////
GUI::GUI()
{
	pWind = new window(WindWidth + 15, WindHeight, 0, 0);
	pWind->ChangeTitle("The Restaurant");

	OrderCount = 0;

	//Set color for each order type
	OrdersClrs[TYPE_NORMAL] = DARKBLUE; //normal-order color
	OrdersClrs[TYPE_FROZEN] = VIOLET;   //Frozen-order color
	OrdersClrs[TYPE_VIP] = RED;			//VIP-order color

	Mode = PROGRAM_MODE(-1);
	ClearStatusBar();
	ClearDrawingArea(0);
	DrawRestArea();
}

//////////////////////////////////////////////////////////////////////////////////////////

GUI::~GUI()
{
	delete pWind;
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== PRIVATE FUNCTIONS ==================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::DrawOrders(bool animate, int time) const
{
	color penColor;
	int t = time % 24;
	if (t >= 4 && t < 11)
		penColor = ORANGERED;
	else if (t >= 11 && t < 19)
		penColor = DARKBLUE;
	else
		penColor = FLORALWHITE;

	pWind->SetPen(penColor);
	pWind->SetFont(25, ITALICIZED, ROMAN, "Arial");
	string s = "Day ";
	s += to_string(time / 24 + 1);
	pWind->DrawString(10, 50, s);

	string strTime = "";
	if (time % 24 < 10)
		strTime += "0";
	strTime += to_string(time % 24);
	strTime += " : ";
	strTime += "00";
	DrawString(WindWidth / 2 - 22, YHalfDrawingArea - 8, strTime);

	//Prepare counter for each region
	int RegionsCounts[REGION_COUNT] = {0}; //initlaize all counters to zero

	for (int i = 0; i < OrderCount; i++)
	{
		int orderRegion = OrdListForDrawing[i]->getRegion();
		RegionsCounts[orderRegion]++;
		DrawSingleOrder(OrdListForDrawing[i], RegionsCounts[orderRegion], animate, time);
		DrawStars(time, i % 8 < 4);
	}
}

void GUI::DrawSingleOrder(Order *pO, int RegionCount, bool animate, int time) const // It is a private function
{

	color clr = OrdersClrs[pO->getType()];
	if (pO->getType() == TYPE_NORMAL && (time % 24 >= 19 || time % 24 < 4))
		clr = color("3c829e"); //009bd8, 39b4e5, 4a9ebf, 3c829e[NIGHTNORMAL]?

	REGION Region = pO->getRegion();

	if (RegionCount > MaxRegionOrderCount)
		return; //no more orders can be drawn in this region

	int DrawDistance = RegionCount;
	int YPos = 1;
	if (RegionCount >= MaxHorizOrders) //max no. of orders to draw in one line
	{
		DrawDistance = (RegionCount - 1) % MaxHorizOrders + 1;
		YPos = (RegionCount - 1) / MaxHorizOrders + 1;
	}

	int x, y, refX, refY;
	//First calculate x,y position of the order on the output screen
	//It depends on the region and the order distance

	switch (Region)
	{
	case A_REGION:
		refX = (WindWidth / 2 - RestWidth / 2);
		refY = YHalfDrawingArea - OrderHeight;				 //
		x = refX - DrawDistance * OrderWidth - DrawDistance; //(Distance)
		y = refY - YPos * OrderHeight - YPos;				 // YPos
		break;
	case B_REGION:
		refX = (WindWidth / 2 + RestWidth / 2 + 15);
		refY = YHalfDrawingArea - OrderHeight;					   //
		x = refX + (DrawDistance - 1) * OrderWidth + DrawDistance; //(Distance)
		y = refY - YPos * OrderHeight - YPos;					   // YPos
		break;
	case C_REGION:
		refX = (WindWidth / 2 + RestWidth / 2 + 15);
		refY = YHalfDrawingArea + OrderHeight;					   //
		x = refX + (DrawDistance - 1) * OrderWidth + DrawDistance; //(Distance)
		y = refY + (YPos - 1) * OrderHeight + YPos;				   // YPos
		break;
	case D_REGION:
		refX = (WindWidth / 2 - RestWidth / 2);
		refY = YHalfDrawingArea + OrderHeight;				 //
		x = refX - DrawDistance * OrderWidth - DrawDistance; //(Distance)
		y = refY + (YPos - 1) * OrderHeight + YPos;			 // YPos
		break;
	default:
		break;
	}
	pO->setX(x);
	pO->setY(y);

	if (animate)
		Animate(x, y, pO->getID(), clr, Region, time);
	else
	{
		pWind->SetPen(clr);
		pWind->SetBrush(clr);
		pWind->SetFont(20, BOLD, MODERN);
		pWind->DrawInteger(x, y, pO->getID());
	}
}

void GUI::Animate(int x, int y, int id, color colr, REGION reg, int time) const
{
	color BG;
	int t = time % 24;
	if (t >= 4 && t < 11)
		BG = EARLYMORNING;
	else if (t >= 11 && t < 19)
		BG = color(255, 165 + 11 * 3, 10 + 2 * 11);
	else
		BG = NIGHT;

	// Drawing the Order
	for (int i = 120; i > 0; i -= 2)
	{
		pWind->SetPen(colr);
		pWind->SetBrush(colr);
		pWind->SetFont(20, BOLD, MODERN);
		if (reg == A_REGION || reg == D_REGION)
		{
			pWind->DrawInteger(x - i, y, id);
			Sleep(1);
			pWind->SetPen(BG);
			pWind->SetBrush(BG);
			pWind->DrawRectangle(x - i, y, x - i + OrderWidth - 10, y + 15);
		}
		else
		{
			pWind->DrawInteger(x + i, y, id);
			Sleep(1);
			pWind->SetPen(BG);
			pWind->SetBrush(BG);
			pWind->DrawRectangle(x + i, y, x + i + OrderWidth - 10, y + 15);
		}
	}

	pWind->SetPen(colr);
	pWind->SetBrush(colr);
	pWind->SetFont(20, BOLD, MODERN);
	pWind->DrawInteger(x, y, id);
}

void GUI::DestroyOrder(Order * pO, int time)
{
	int t = (time-1) % 24;
	int x = pO->getX(), y = pO->getY();
	if (t >= 4 && t < 11)
	{
		image img("bomb_explosion_morning.jpg");
		pWind->DrawImage(img,x-5,y-5);
	}

	else if (t >= 11 && t < 19)
	{
		image img("bomb_explosion_noon.jpg");
		pWind->DrawImage(img, x-5, y-5);
	}
	else {
		image img("bomb_explosion_night.jpg");
		pWind->DrawImage(img, x-5, y-5);
	}
	Sleep(2000);
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::DrawString(const int iX, const int iY, const string Text) const
{
	pWind->SetPen(WHITE);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");
	pWind->DrawString(iX, iY, Text);
}

void GUI::DrawRestArea() const
{
	int L = RestWidth / 2;

	// 1- Drawing the brown circle of the Rest
	color redDark = color(178, 7, 7);
	pWind->SetPen(redDark);
	pWind->SetBrush(redDark);
	pWind->DrawCircle((WindWidth / 2), YHalfDrawingArea, (WindWidth / 2) - RestStartX);

	// 2- Drawing the 2 brown crossed lines (for making 4 regions)
	pWind->SetPen(TEXTGREY, 3);
	pWind->DrawLine(0, YHalfDrawingArea, WindWidth, YHalfDrawingArea);
	pWind->DrawLine(WindWidth / 2, MenuBarHeight, WindWidth / 2, WindHeight - StatusBarHeight);

	// 3- Drawing the Timestep box
	pWind->SetPen(BLACK, 3);
	pWind->SetBrush(DARKRED);
	pWind->DrawRectangle(WindWidth / 2 - 30, YHalfDrawingArea - 20, WindWidth / 2 + 30, YHalfDrawingArea + 20);

	// 4- Writing the letter of each region (A, B, C, D)
	pWind->SetPen(BRIGHTYELLOW);
	pWind->SetFont(40, ITALICIZED, ROMAN, "Arial");
	pWind->DrawString(RestStartX + (int)(0.44 * L), RestStartY + 5 * L / 12, "A");
	pWind->DrawString(RestStartX + (int)(0.44 * L), YHalfDrawingArea + 5 * L / 20, "D");
	pWind->DrawString(RestStartX + L + (int)(0.36 * L), RestStartY + 5 * L / 12, "B");
	pWind->DrawString(RestStartX + L + (int)(0.36 * L), YHalfDrawingArea + 5 * L / 20, "C");
}

void GUI::DrawStars(int time, bool flip) const
{
	if (time % 24 >= 4 && time % 24 < 19)
		return;

	if (flip)
	{
		pWind->SetPen(DARKSTAR);
		pWind->SetBrush(DARKSTAR);
	}

	else
	{
		pWind->SetPen(WHITE);
		pWind->SetBrush(WHITE);
	}

	for (int i = MenuBarHeight + 25; i < WindHeight - StatusBarHeight; i += 30)
	{
		for (int j = (i / 10) % 2 ? 10 : 20; j < WindWidth; j += 30)
		{
			if ((j - (WindWidth) / 2) * (j - (WindWidth) / 2) + (i - YHalfDrawingArea) * (i - YHalfDrawingArea) > ((WindWidth / 2) - RestStartX) * ((WindWidth / 2) - RestStartX))
				pWind->DrawCircle(j, i, 1);
		}
	}
}

void GUI::ClearStatusBar() const
{
	pWind->SetPen(WHITE, 3);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, WindHeight - StatusBarHeight, WindWidth, WindHeight);

	pWind->SetPen(BROWN, 3);
	pWind->DrawLine(0, WindHeight - StatusBarHeight, WindWidth, WindHeight - StatusBarHeight);
}

void GUI::ClearDrawingArea(int time) const
{
	pWind->SetPen(WHITE);
	pWind->SetFont(20, ITALICIZED, ROMAN, "Arial");
	string s = "Day ";
	s += to_string(time / 24 + 1);
	pWind->DrawString(20, 40, s);

	int t = time % 24;
	// Clearing the Drawing area
	if (t >= 4 && t < 11)
	{
		pWind->SetPen(EARLYMORNING, 3);
		pWind->SetBrush(EARLYMORNING);
		pWind->DrawRectangle(0, 0, WindWidth + 10, WindHeight - StatusBarHeight);
		pWind->SetBrush(WHITESMOKE);
		pWind->SetPen(WHITESMOKE, 3);
		if (Mode == MODE_RAMADAN)
			pWind->DrawImage("Restaurant\\Ramadan\\decoration_early_morning.jpg", 0, 0);

		for (int i = 0; i < 2; i++)
		{
			pWind->DrawCircle(55 + t * i * 35 + t * 10, 70, 19);
			pWind->DrawCircle(70 + t * i * 35 + t * 10, 70, 25);
			pWind->DrawCircle(100 + t * i * 35 + t * 10, 70, 25);
			pWind->DrawCircle(115 + t * i * 35 + t * 10, 70, 19);
		}
		for (int i = 0; i < 2; i++)
		{
			pWind->DrawCircle(85 + t * i * 45 + t * 10 + WindWidth / 2, 70, 19);
			pWind->DrawCircle(100 + t * i * 45 + t * 10 + WindWidth / 2, 70, 25);
			pWind->DrawCircle(130 + t * i * 45 + t * 10 + WindWidth / 2, 70, 25);
			pWind->DrawCircle(145 + t * i * 45 + t * 10 + WindWidth / 2, 70, 19);
		}
	}

	else if (t >= 11 && t < 19)
	{
		color afterNoonGrad = AFTERNOON;
		for (int i = 0; i < 12; i++)
		{
			afterNoonGrad = color(255, 165 + i * 3, 10 + 2 * i);
			pWind->SetPen(afterNoonGrad, 3);
			pWind->SetBrush(afterNoonGrad);
			pWind->DrawRectangle(0, 10 * i, WindWidth + 10, 10 * i + 10);
		}
		pWind->SetPen(afterNoonGrad, 3);
		pWind->SetBrush(afterNoonGrad);
		pWind->DrawRectangle(0, 10 * 11 + 10, WindWidth + 10, WindHeight - StatusBarHeight);
		for (int i = 11; i >= 0; i--)
		{
			afterNoonGrad = color(255, 165 + i * 3, 10 + 2 * i);
			pWind->SetPen(afterNoonGrad, 3);
			pWind->SetBrush(afterNoonGrad);
			pWind->DrawRectangle(0, 300 + 10 * (11 - i), WindWidth + 10, WindHeight - StatusBarHeight - 10 * i);
		}
		if (Mode == MODE_RAMADAN)
			pWind->DrawImage("Restaurant\\Ramadan\\decoration_afternoon.jpg", 0, 0);
	}

	else
	{
		pWind->SetPen(NIGHT, 3);
		pWind->SetBrush(NIGHT);
		pWind->DrawRectangle(0, 0, WindWidth + 10, WindHeight - StatusBarHeight);
		if (Mode == MODE_RAMADAN)
			pWind->DrawImage("Restaurant\\Ramadan\\decoration_night.jpg", 0, 0);
		DrawStars(time);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== INPUT FUNCTIONS ====================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::waitForClick() const
{
	int x, y;
	pWind->WaitMouseClick(x, y); //Wait for mouse click
}

string GUI::GetString() const
{
	string Label;
	char Key;
	while (true)
	{
		pWind->WaitKeyPress(Key);
		if (Key == 27) //ESCAPE key is pressed
			return ""; //returns nothing as user has cancelled label
		if (Key == 13) //ENTER key is pressed
			return Label;
		if ((Key == 8) && (!Label.empty())) //BackSpace is pressed
			Label.resize(Label.size() - 1);
		else
			Label += Key;

		PrintMessage(Label);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== OUTPUT FUNCTIONS ===================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::PrintMessage(string msg) const //Prints a message on status bar
{
	ClearStatusBar(); //First clear the status bar

	pWind->SetPen(DARKRED);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");
	pWind->DrawString(10, WindHeight - StatusBarHeight + 10, msg);
}

void GUI::PrintTimestep(int time) const
{
	pWind->SetPen(BLACK, 4);
	pWind->SetBrush(DARKRED);
	pWind->DrawRectangle(WindWidth / 2 - 30, YHalfDrawingArea - 20, WindWidth / 2 + 30, YHalfDrawingArea + 20);
	string strTime = "";

	if (time % 24 < 10)
		strTime += "0";
	strTime += to_string(time % 24);
	strTime += " : ";
	strTime += "00";
	DrawString(WindWidth / 2 - 22, YHalfDrawingArea - 8, strTime);
}

void GUI::PrintRegions(string data[REGION_COUNT], string dataMotor[REGION_COUNT], string dataAssignedMotors[REGION_COUNT], string servedOrders[REGION_COUNT]) const
{
	ClearStatusBar(); //First clear the status bar
	pWind->SetPen(TEXTGREY, 3);
	pWind->DrawLine(0, WindHeight - StatusBarHeight / 2 - 5, WindWidth, WindHeight - StatusBarHeight / 2 - 5);
	pWind->DrawLine(WindWidth / 2, WindHeight - StatusBarHeight, WindWidth / 2, WindHeight);

	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		switch (reg)
		{
		case A_REGION:
			pWind->SetPen(TOMATO);
			pWind->SetFont(20, BOLD, BY_NAME, "Arial");
			pWind->DrawString(200, WindHeight - StatusBarHeight + 5, "A");
			pWind->SetPen(TEXTGREY);
			pWind->SetFont(16, PLAIN, BY_NAME, "Arial");
			pWind->DrawString(10, WindHeight - StatusBarHeight + 22, data[reg]);
			pWind->DrawString(10, WindHeight - StatusBarHeight + 39, dataMotor[reg]);
			pWind->DrawString(10, WindHeight - StatusBarHeight + 56, dataAssignedMotors[reg]);
			pWind->DrawString(10, WindHeight - StatusBarHeight + 73, servedOrders[reg]);
			break;

		case B_REGION:
			pWind->SetPen(TOMATO);
			pWind->SetFont(20, BOLD, BY_NAME, "Arial");
			pWind->DrawString(WindWidth / 2 + 200, WindHeight - StatusBarHeight + 5, "B");
			pWind->SetPen(TEXTGREY);
			pWind->SetFont(16, PLAIN, BY_NAME, "Arial");
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight + 22, data[reg]);
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight + 39, dataMotor[reg]);
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight + 56, dataAssignedMotors[reg]);
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight + 73, servedOrders[reg]);
			break;

		case C_REGION:
			pWind->SetPen(TOMATO);
			pWind->SetFont(20, BOLD, BY_NAME, "Arial");
			pWind->DrawString(WindWidth / 2 + 200, WindHeight - StatusBarHeight / 2, "C");
			pWind->SetPen(TEXTGREY);
			pWind->SetFont(16, PLAIN, BY_NAME, "Arial");
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight / 2 + 13, data[reg]);
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight / 2 + 31, dataMotor[reg]);
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight / 2 + 49, dataAssignedMotors[reg]);
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight / 2 + 68, servedOrders[reg]);
			break;

		case D_REGION:
			pWind->SetPen(TOMATO);
			pWind->SetFont(20, BOLD, BY_NAME, "Arial");
			pWind->DrawString(200, WindHeight - StatusBarHeight / 2, "D");
			pWind->SetPen(TEXTGREY);
			pWind->SetFont(16, PLAIN, BY_NAME, "Arial");
			pWind->DrawString(10, WindHeight - StatusBarHeight / 2 + 13, data[reg]);
			pWind->DrawString(10, WindHeight - StatusBarHeight / 2 + 31, dataMotor[reg]);
			pWind->DrawString(10, WindHeight - StatusBarHeight / 2 + 49, dataAssignedMotors[reg]);
			pWind->DrawString(10, WindHeight - StatusBarHeight / 2 + 68, servedOrders[reg]);
			break;

		default:
			return;
		}
	}
}

void GUI::OrderOut(int time)
{
	if (time <= 1)
		return;
	int t = time % 24;
	if (t >= 4 && t < 11)
	{
		image img("Restaurant\\delivery_man_morning.jpg");
		for (int i = 0; i < 300; i += 25)
		{
			drawImage(img, WindWidth - 350 + i, WindHeight - StatusBarHeight - 80);
			Sleep(50);
			pWind->SetPen(EARLYMORNING);
			pWind->SetBrush(EARLYMORNING);
			pWind->DrawRectangle(WindWidth - 350 + i, WindHeight - StatusBarHeight - 80, WindWidth - 350 + i + 80, WindHeight - StatusBarHeight);
		}
		pWind->DrawRectangle(WindWidth - 350, WindHeight - StatusBarHeight - 40, WindWidth + 5, WindHeight - StatusBarHeight);
	}

	else if (t >= 11 && t < 19)
	{
		image img("Restaurant\\delivery_man_afternoon.jpg");
		for (int i = 0; i < 300; i += 25)
		{
			drawImage(img, WindWidth - 350 + i, WindHeight - StatusBarHeight - 90);
			Sleep(50);
			color afterNoonGrad;
			afterNoonGrad = color(255, 165 + 3 * 5, 10 + 2 * 5);
			pWind->SetPen(afterNoonGrad);
			pWind->SetBrush(afterNoonGrad);
			pWind->DrawRectangle(WindWidth - 350 + i, WindHeight - StatusBarHeight - 90, WindWidth - 350 + i + 80, WindHeight - StatusBarHeight - 70);
			afterNoonGrad = color(255, 165 + 3 * 3, 10 + 2 * 3);
			pWind->SetPen(afterNoonGrad);
			pWind->SetBrush(afterNoonGrad);
			pWind->DrawRectangle(WindWidth - 350 + i, WindHeight - StatusBarHeight - 70, WindWidth - 350 + i + 80, WindHeight - StatusBarHeight - 50);
			afterNoonGrad = color(255, 165 + 3 * 2, 10 + 2 * 2);
			pWind->SetPen(afterNoonGrad);
			pWind->SetBrush(afterNoonGrad);
			pWind->DrawRectangle(WindWidth - 350 + i, WindHeight - StatusBarHeight - 50, WindWidth - 350 + i + 80, WindHeight - StatusBarHeight - 40);
			afterNoonGrad = color(255, 165, 10);
			pWind->SetPen(afterNoonGrad);
			pWind->SetBrush(afterNoonGrad);
			pWind->DrawRectangle(WindWidth - 350 + i, WindHeight - StatusBarHeight - 40, WindWidth - 350 + i + 80, WindHeight - StatusBarHeight);
		}
		pWind->DrawRectangle(WindWidth - 350, WindHeight - StatusBarHeight - 40, WindWidth + 5, WindHeight - StatusBarHeight);
	}
	else
	{
		image img("Restaurant\\delivery_man_night.jpg");
		for (int i = 0; i < 300; i += 25)
		{
			drawImage(img, WindWidth - 350 + i, WindHeight - StatusBarHeight - 80);
			Sleep(50);
			pWind->SetPen(NIGHT);
			pWind->SetBrush(NIGHT);
			pWind->DrawRectangle(WindWidth - 350 + i, WindHeight - StatusBarHeight - 80, WindWidth - 350 + i + 80, WindHeight - StatusBarHeight);
			bool flip = t % 2;
			if (!flip)
			{
				pWind->SetPen(DARKSTAR);
				pWind->SetBrush(DARKSTAR);
			}
			else
			{
				pWind->SetPen(WHITE);
				pWind->SetBrush(WHITE);
			}

			for (int j = WindHeight - StatusBarHeight - 75; j < WindHeight - StatusBarHeight; j += 30)
			{
				for (int k = (j / 10) % 2 ? 10 + WindWidth - 350 + i : 22 + WindWidth - 350 + i; k < WindWidth - 350 + i + 50; k += 30)
				{
					pWind->DrawCircle(k, j, 1);
				}
			}
		}
		//pWind->DrawRectangle(WindWidth - 350, WindHeight - StatusBarHeight - 40, WindWidth + 5, WindHeight - StatusBarHeight);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void GUI::UpdateInterface(bool animate, int time) const
{
	ClearDrawingArea(time);
	DrawRestArea();
	DrawOrders(animate, time);
}

void GUI::UpdateInterface(color newColor) const
{
	pWind->SetPen(newColor, 3);
	pWind->SetBrush(newColor);
	pWind->DrawRectangle(0, MenuBarHeight, WindWidth, WindHeight - StatusBarHeight);
	DrawRestArea();
}

void GUI::AddOrderForDrawing(Order *ptr)
{
	if (OrderCount < MaxPossibleOrdCnt)
		OrdListForDrawing[OrderCount++] = ptr;

	// Note that this function doesn't allocate any Order objects
	// It only makes the first free pointer in the array
	// points to the same Order pointed to by "ptr"
}

void GUI::ResetDrawingList()
{
	OrderCount = 0; //resets the orders count to be ready for next timestep updates
}

void GUI::drawImage(image img, int x, int y) const
{
	pWind->DrawImage(img, x, y);
}

PROGRAM_MODE GUI::getGUIMode()
{
	do
	{
		PrintMessage("Please select GUI mode: (1) Interactive, (2) StepByStep, (3) Silent , (4) Ramadan");
		string S = GetString();
		Mode = (PROGRAM_MODE)(stoi(S) - 1);
	} while (Mode < 0 || Mode >= MODE_COUNT);

	return Mode;
}
