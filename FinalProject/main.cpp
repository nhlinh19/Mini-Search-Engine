#undef UNICODE // Avoid VS compilation error

#include <iostream>
#include "Query.h"
#include "Base.h"
#include "Files.h"
#include "Operations.h"

void option(int posX, int posY);
bool search(baseData* data);

int main()
{
	resizeConsole(1200, 700);
	fixConsoleWindow();
	HidePointer();
	option(OPTION_POSX, OPTION_POSY);
	return 0;
}

bool search(baseData* data)
{
	drawBoard(38, 23, 3, 95, 13);
	goToXY(40, 24);  cout << "ENTER YOUR SEARCH: ";
	ShowPointer();
	string str;
	if (!getQuery(str, 12))
	{
		HidePointer();
		return false;
	}
	HidePointer();
	system("cls");
	printNote(NOTE_POSX, NOTE_POSY);
	goToXY(14, 3);
	textColor(0, 13); cout << "YOUR QUERY: ";
	textColor(0, 13); cout << shortenName(str, 110) << endl;
	drawBoard(10, 2, 2, 130, 13);
	queryData q1 = queryData(str, true);
	double processingTime = Operations::opWrapper(&q1, data, false);
	vector<pair<int, double> > scores = q1.getScores();
	vector<pair<int, double>> listFile;
	listFile = q1.getScores();

	drawBoard(FILES_POSX - 5, FILES_POSY - 2, FILES_CNT * 2 + 6, FILES_LEN + 10, 12);
	if (listFile.size() == 0)
	{
		textColor(0, 4);
		int x = FILES_POSX, y = FILES_POSY + 2;
		goToXY(x, y++); cout << "[INFO] No file matches your query!";
		goToXY(x, y++); cout << "[INFO] Please try again!";
		goToXY(x, y);	system("pause");
		return true;
	}
	else
	{
		FilesList filesList(listFile, data, &q1, processingTime);
		filesList.printList(0);
		while (true)
		{
			if (!filesList.moveFiles())
				return true;
		}
	}
}
void option(int posX, int posY)
{
	baseData data;
	int cur = 0;
	bool isArrow = false;
	int keyValue;
	system("cls");
	printNote(NOTE_POSX, NOTE_POSY);
	printHCMUS(40, 8);
	//data.theLoadFromCSV();
	printOptionList(cur, posX, posY);
	drawBoard(OPTION_POSX - 5, OPTION_POSY - 2, 12, 20, 13);
	while (true)
	{
		while (true)
		{
			if (_kbhit())
			{
				keyValue = toupper(_getch());
				if (keyValue == 224)
				{
					isArrow = true;
					continue;
				}
				if (isArrow)
				{
					isArrow = false;
					if (keyValue == 72 && cur > 0)
						printOptionList(--cur, posX, posY);
					if (keyValue == 80 && cur < 3)
						printOptionList(++cur, posX, posY);
					break;
				}
				if (keyValue == 13)
				{
					if (cur == 0)
					{
						textColor(0, 4);
						int x = 45, y = 22;
						data.clear();
						data.loadFromFiles("data", x, y);
						data.saveToFile(x, y);
						goToXY(x, y); system("pause");
					}
					if (cur == 1)
					{
						textColor(0, 4);
						int x = 45, y = 22;
						data.clear();
						data.readFromFile(x, y);
						goToXY(x, y); system("pause");
					}
					if (cur == 2)
					{
						bool ok = true;
						if (data.isEmpty)
						{
							ok = false;
							textColor(0, 4);
							int x = 45, y = 22;
							goToXY(x, y++); cout << "[ERROR] The data is empty, please try loading data!";
							goToXY(x, y); system("pause");
						}
						while (ok)
						{
							if (!search(&data))
								break;
							system("cls");
							printNote(NOTE_POSX, NOTE_POSY);
							printHCMUS(40, 8);
							printOptionList(cur, posX, posY);
							drawBoard(OPTION_POSX - 5, OPTION_POSY - 2, 12, 20, 13);
						}
					}
					if (cur == 3)
					{
						system("cls");
						data.clear();
						return;
					}
					system("cls");
					printNote(NOTE_POSX, NOTE_POSY);
					printHCMUS(40, 8);
					printOptionList(cur, posX, posY);
					drawBoard(OPTION_POSX - 5, OPTION_POSY - 2, 12, 20, 13);
					break;
				}
			}
		}
	}
}
