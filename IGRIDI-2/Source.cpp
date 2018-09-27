#include "stdafx.h"
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <iostream>

#pragma warning(disable : 4996)

using namespace std;

//������⢮ ����ᥩ �� ��࠭��
#define notesOnPage 16
//��� 梥� ���筮�� ⥪��
#define textColor 15
//��� 梥� ���筮�� 䮭�
#define backgroundColor 3
//��� 梥� ⥪�� ��⨢���� �㭪� ���� ��� ᯨ᪠
#define activeTextColor 0
//��� 梥� 䮭� ��⨢���� �㭪� ���� ��� ᯨ᪠
#define activeBackgroundColor 15

//��� �⠭���⭮�� ���-䠩��
#define binFile "BIN.DAT"
//��� �⠭���⭮�� ⥪�⮢��� 䠩��
#define txtFile "Table.txt"

//����᫥��� ��� �뢮�� ������ MessageBox'a
enum ButtonsMessageBox {
	OneButton, TwoButton, ThreeButton
};

//������� ������ ����ᥩ
struct NodeInfo
{
	int key;					//����
	int StoreNumber;			//����� ��������
	int SectionNumber;			//����� ᥪ樨
	int CheckNumber;			//����� 祪�
	char ProductName[31];		//������������ �த��
	char ProductArticle[13];	//��⨪�
	float Price;				//����
	int ProductAmount;			//������⢮
	char DateOfSale[11];		//��� �த���
};
//������� 㧫� ᯨ᪠
struct Node
{
	NodeInfo data;			//�����
	bool Visible;			//��������� (�ᯮ������ ��� ॠ����樨 ���᪠)
	Node* pNext;			//㪠��⥫� �� ᫥���騩 ����� ᯨ᪠
	Node* pPrev;			//㪠��⥫� �� �।��騩 ����� ᯨ᪠
}*pHead, *pTail, *pTemp;	//�����⥫� �� ������ ᯨ᪠(pHead), �� 墮�� ᯨ᪠(pTail) � 㪠��⥫� ��� �६������ �࠭���� ��뫪� (pTemp)

							//C������ 㧫� ᯨ᪠ ��ࠡ�⠭��� ������
struct ProcessNode
{
	int StoreNumber;				//����� ��������
	double Profit;					//����窠
	ProcessNode *pNext, *pPrev;		//�����⥫� �� ᫥���騩 � �।���騩 ������ ᯨ᪠
};

//������⢮ ����ᥩ � ᯨ᪥
int notesAmount = 0;
//������⢮ ����ᥩ �뢮����� �� �࠭
int notesForPrint = 0;

int PrintMainMenu();
int PrintProcessMenu();
void PrintAll();
bool PrintSortMenu();
void PrintDataOnScreen(NodeInfo* data);
int InitializePrint(int currentPage, int* pageAmount, Node** pBegin);
void PrintWindow(int width, int height, int x = -1, int y = -1);
int PrintSimpleMenu(char** menuItemsText, int menuItems, int width = -1, int height = -1, bool window = true, int x = -1, int y = -1);
int PrintMessageBox(ButtonsMessageBox btnMB, char* text, int width = -1, int height = -1, char* textFirstBtn = "��", char* textSecondBtn = "�⬥��", char* textThirdBtn = "��室");


int ReadFileName(char* path);
bool WriteIntoBinaryFile(char* path);
bool ReadFromBinaryFile(char* path);
bool WriteIntoTextFile(char* path);
bool ReadFromTextFile(char *path);

void AddNode(NodeInfo elem);
void DeleteNode(int &key,int &active);
void DeleteNode(Node* pDelete);
void ClearList();
void SortList(int field);
void ProcessList();

void Search();
void SearchKey(int paramSearch, int x, int y);
void SearchStoreNumber(int paramSearch, int x, int y);
void SearchSectionNumber(int paramSearch, int x, int y);
void SearchCheckNumber(int paramSearch, int x, int y);
void SearchProductName(int paramSearch, int x, int y);
void SearchProductArticle(int paramSearch, int x, int y);
void SearchPrice(int paramSearch, int x, int y);
void SearchProductAmount(int paramSearch, int x, int y);
void SearchDateOfSale(int paramSearch, int x, int y);
void ShowAll();

NodeInfo ReadDataFromScreen();
int ReadIntFromScreen(int* number, int digitAmount);
int ReadFloatFromScreen(float* number, int digitsBeforeComma, int digitsAfterComma);
int ReadFileNameFromScreen(char* str, int symbols);
int ReadStringFromScreenWithoutRestrictions(char* str, int symbols);
int ReadArticleFromScreen(char* str, int symbols);
int ReadDateFromScreen(char* str);


void Swap(Node *pN1, Node *pN2);
int CompareDate(char* date1, char* date2);
void ActiveOrNot(int active, int current);
void SetColor(int text, int background);
void GotoXY(int xPos, int yPos);
void SetCursorVisibility(bool Visible);


int _tmain(int argc, _TCHAR* argv[])
{
	system("mode 80,25");
	//��⠭���� ⠡���� ����஢��
	SetConsoleCP(866); SetConsoleOutputCP(866);
	//���㫥��� 㪠��⥫�� ᯨ᪠
	pTail = pHead = NULL;
	//�⪫�祭�� �����
	SetCursorVisibility(false);
	//����㧪� ᯨ᪠ �� 䠩��
	ReadFromBinaryFile("\0");
	//�뢮� ᯨ᪠
	PrintAll();
}

//�㭪�� PrintMainMenu
//�믮���� ���ᮢ�� �������� ����
//�����頥� ����� �맢������ �㭪� ����
int PrintMainMenu()
{
	//������⢮ ����⮢ ����
	int menuItems = 10;
	//���ᨢ �㭪⮢ ����
	//������ ���ᨢ� ����� ������⢠ ����⮢ �� ����
	//�.�. 0-�� ����� ���ᨢ� - �������� ����
	char **menuText = new char*[menuItems + 1];
	menuText[0] = "";
	menuText[1] = "������� ";
	menuText[2] = "�������� ���� ᯨ᮪";
	menuText[3] = "�������� ����� ";
	menuText[4] = "���� ";
	menuText[5] = "����஢��� ";
	menuText[6] = "���࠭��� ����� � 䠩� ";
	menuText[7] = "����㧨�� ����� �� 䠩�� ";
	menuText[8] = "��ࠡ�⪠ ";
	menuText[9] = "������� �� �����";
	menuText[10] = "��� ";
	//�맮� �㭪樨 ���ᮢ�� ���� � ��ࠡ�⪨ ����������� ������
	return PrintSimpleMenu(menuText, menuItems, 34);
}
//�㭪�� PrintProcessMenu
//�믮���� ���ᮢ�� ���� ��ࠡ�⪨ (���� �� ���ᮢ�� ⠡���� ��ࠡ�⪨)
//�����頥� ����� �맢������ �㭪� ����
int PrintProcessMenu()
{
	//������⢮ ����⮢ ����
	int menuItems = 3;
	//���ᨢ �㭪⮢ ����
	//������ ���ᨢ� ����� ������⢠ ����⮢ �� ����
	//�.�. 0-�� ����� ���ᨢ� - �������� ����
	char **menuText = new char*[menuItems + 1];
	menuText[0] = "";
	menuText[1] = "������� ";
	menuText[2] = "���࠭��� १����� � 䠩� ";
	menuText[3] = "�������� ᯨ᮪ ";
	//�맮� �㭪樨 ���ᮢ�� ���� � ��ࠡ�⪨ ����������� ������
	return PrintSimpleMenu(menuText, menuItems, 34);
}
//�㭪�� PrintAll
//�����⢫�� ���ᮢ�� ⠡���� ������
//� ��ࠡ��� ���������� ������
void PrintAll()
{
	//� - ��६����� ��� ��ࠡ�⪨ ������� ������, path - ��� ��࠭���� �������� ��⥩ � 䠩���
	char c, path[61];
	//page - ����� ⥪�饩 ��࠭���, i - ���稪 �뢥������ ����⮢ ᯨ᪠, pageAmount - ��饥 ������⢠ ��࠭��
	int page = 1, i = 0, pageAmount = (notesAmount + notesOnPage - 1) / notesOnPage;
	//activeNote - ����� ��⨢���� ����� �� ��࠭��, notesOnCurrentPage - ������⢮ ����ᥩ �� ⥪�饩 ��࠭��
	//notesForPrint - ������⢮ ����ᥩ ��� ����
	int activeNote = 0, notesOnCurrentPage = 0; notesForPrint = notesAmount;
	//pBegin - 㪠��⥫� �� ���� ����� �뢮����� ��࠭���, pPrint - 㪠��⥫� ��� ���� ��࠭���, pActive - 㪠��⥫� �� ��⨢�� �����
	Node *pBegin = pHead, *pPrint, *pActive = NULL;
	//横� �뢮�� � ��ࠡ�⪨
	int active = 1;
	while (true)
	{
		//��⠭���� 梥� ⥪�� � 䮭�
		SetColor(textColor, backgroundColor);
		//l = active - (active % notesOnPage ? active % notesOnPage : notesOnPage) + 1;
		// = min(notesAmount, l + notesOnPage - 1);
		//���樠������ ���祭��� 㪠��⥫�� � ��६����� ��� ��୮� ���ᮢ�� ᯨ᪠
		notesOnCurrentPage = InitializePrint(page, &pageAmount, &pBegin);
		pPrint = pBegin;
		i = 0;
		//���⪠ �࠭�
		system("cls");
		//����� 蠯�� ⠡����
		printf("                          Esc - ��� ��室� � ����\n");
		printf("\n     ��࠭�� �%-3i   ������ �%04i           ��࠭��: %-3i    ����ᥩ: %-4i \n", page, 0, pageAmount, notesForPrint);
		printf("   ������������������������������������������������������������������������Ŀ\n");
		printf("   � �����  �          ������������          �   ����   ����-���    ���    �\n");
		printf("   ����������             ⮢��             �          �      �   �த���  �\n");
		printf("   ������������������������������������������������������������������������Ĵ\n");
		//���� ���稪 ����� �ॡ㥬��� ������⢠ ����⮢ �� �࠭� �뢮��� �����
		while (i < notesOnPage)
		{
			//��室�� ����� ��� �뢮�� �� �࠭
			while (pPrint && !pPrint->Visible) {
				pPrint = pPrint->pNext;
			}
			//�᫨ 㪠��⥫� �� �㫥��� �뢮��� �����, �।���⥫쭮 �஢�ਢ ���� ��⨢�� �뢮���� ����� ��� ���
			if (pPrint != NULL)
			{
				if (activeNote == i)
				{
					pActive = pPrint;
					GotoXY(29, 2);
					printf("%04i", pActive->data.key);
					GotoXY(0, 7 + i);
					SetColor(activeTextColor, activeBackgroundColor);
				}
				printf("   � %-06.6i � %-30.30s � %8.2f � %4i � %10.10s �\n", pPrint->data.StoreNumber, pPrint->data.ProductName, pPrint->data.Price, pPrint->data.ProductAmount, pPrint->data.DateOfSale);
				pPrint = pPrint->pNext;
				if (activeNote == i) {
					SetColor(textColor, backgroundColor);
				}
			}
			//���� �뢮��� ������ ��ப� ⠡����
			else
				printf("   � %6s � %30s � %8s � %4s � %10s �\n", "", "", "", "", "");
			//���६����㥬 ���稪
			i++;
		}
		//����� ����砭�� ⠡����
		printf("   ��������������������������������������������������������������������������\n");
		//�����뢠�� � c ��� ᨬ���� ����⮩ ������
		c = _getch();
		switch (c)
		{ 
		case -32:
			c = _getch();
			//����⨥ ��५�� - �ࠢ����� ��⨢�� ����⮬ � �뢮���묨 ��࠭�栬�
			switch (c)
			{
			case 72:  if (notesAmount != 0) activeNote = (activeNote + notesOnCurrentPage - 1) % notesOnCurrentPage; break;
			case 75:  if (page > 1) { page--; activeNote = 0; } break;
			case 77:  if (page < pageAmount) { page++; activeNote = 0; }break;
			case 80:  if (notesAmount != 0) activeNote = (activeNote + 1) % notesOnCurrentPage; break;
			}
			break;
		case 0: c = _getch();
			switch (c)
			{
				//����⨥ ������ F10 - ��室 �� �ணࠬ��
			case 'D': SetColor(textColor, backgroundColor);
				switch (PrintMessageBox(TwoButton, "�� 㢥७�, �� ��� ���?", -1, -1, "��", "���"))
				{
				case 0: ClearList(); return;
				case 1: continue;
				default: break;
				}
				break;
			default: break;
			}
			break;
			//����⨥ ������ Enter - �뢮� ������ ��⨢���� ����� ᯨ᪠
		case 13: 
			if (notesForPrint) {		
				PrintDataOnScreen(&pActive->data);
				if (notesForPrint && page == pageAmount && activeNote >= notesForPrint % 16) {
					activeNote--;
					//notesForPrint--;
					if (activeNote < 0) {
						page--;
						//notesForPrint = notesAmount;
						activeNote = 15;
					}
				}
			}
			break;
			//����⨥ ������ Esc - �뢮� �������� ����
		case 27:
			//�롮� ����頥���� ���祭�� - ���� ��࠭���� �㭪� ����
			switch (PrintMainMenu())
			{
			case 0: break;
				//�������� ���� ᯨ᮪ ������
			case 1: ShowAll(); break;
				//���������� ������ - ���� � ����������
			case 2: AddNode(ReadDataFromScreen()); break;
				//����
			case 3: Search(); activeNote = 0; page = 1; break;
				//����஢��
			case 4:
				if (PrintSortMenu())
				{
					page = 1;
					activeNote = 0;
				}
				break;
				//���࠭���� ������ � 䠩�
			case 5:
				//�뢮� ���誠 � �롮஬ ⨯� 䠩��
				switch (PrintMessageBox(ThreeButton, "�롥�� ⨯ 䠩�� ��� ��࠭����", 50, -1, "���-䠩�", "����⮢� 䠩�"))
				{
					//������ 䠩�
				case 0:
					//�⥭�� ��� � 䠩��
					if (ReadFileName(path) == -1)
						continue;
					//�᫨ ����� ��࠭��� �뢮����� ���誮 �� �ᯥ�
					if (WriteIntoBinaryFile(path))
						PrintMessageBox(OneButton, "����� �ᯥ譮 ��࠭���!");
					break;
					//����⮢� 䠩�
				case 1:
					if (ReadFileName(path) == -1)
						continue;
					//�᫨ ����� ��࠭��� �뢮����� ���誮 �� �ᯥ�
					if (WriteIntoTextFile(path))
						PrintMessageBox(OneButton, "����� �ᯥ譮 ��࠭���!");
					break;
					//��室
				default: continue;
				}
				break;
				//�⥭�� ������ �� 䠩��
			case 6:
				//�뢮� ���誠 � �롮஬ ⨯� 䠩��
				switch (PrintMessageBox(ThreeButton, "�롥�� ⨯ 䠩�� ��� �⥭��", 50, -1, "���-䠩�", "����⮢� 䠩�"))
				{
					//������ 䠩�
				case 0:
					//�⥭�� ��� � 䠩��
					if (ReadFileName(path) == -1)
						continue;
					//�뢮� ���� � ����ᮬ �� ���⪥ ᯨ᪠
					if (pHead != NULL)
						switch (PrintMessageBox(ThreeButton, "������ ᯨ᮪?", -1, -1, "��", "���", "���"))
						{
						case 0: ClearList();
						case 1: break;
						case 2: continue;
						default: break;
						}
					//�᫨ ����� ��⠭� �뢮����� ���誮 �� �ᯥ�
					if (ReadFromBinaryFile(path))
						PrintMessageBox(OneButton, "����� �ᯥ譮 ��⠭�!");
					break;
					//������ 䠩�
				case 1:
					//�⥭�� ��� � 䠩��
					if (ReadFileName(path) == -1)
						continue;
					//�뢮� ���� � ����ᮬ �� ���⪥ ᯨ᪠
					if (pHead != NULL)
						switch (PrintMessageBox(ThreeButton, "������ ᯨ᮪?", -1, -1, "��", "���", "���"))
						{
						case 0: ClearList();
						case 1: break;
						case 2: continue;
						default: break;
						}
					//�᫨ ����� ��⠭� �뢮����� ���誮 �� �ᯥ�
					if (ReadFromTextFile(path))
						PrintMessageBox(OneButton, "����� �ᯥ譮 ��⠭�!");
					break;
				default: continue;
				}
				//���樠������ ��६����� ᯨ᪠ ��᫥ �⥭��
				pActive = NULL; activeNote = 0; page = 1;
				break;
				//��ࠡ�⪠ ������
			case 7: ProcessList(); break;
				//���⪠ ᯨ᪠
			case 8:
				//���� ���⢥ত����
				switch (PrintMessageBox(TwoButton, "�� 㢥७�, �� ��� ������ ᯨ᮪?", 50, -1, "��", "���"))
				{
				case 0: ClearList();
				case 1: break;
				case 2: continue;
				default: break;
				}
				pActive = NULL; activeNote = 0; page = 1; break;
				//��室 �� �ணࠬ��
			case 9:
				SetColor(textColor, backgroundColor);
				//���� ���⢥ত����
				switch (PrintMessageBox(TwoButton, "�� 㢥७�, �� ��� ���?", -1, -1, "��", "���"))
				{
				case 0: ClearList(); return;
				case 1: continue;
				default: break;
				}
			default: break;
			}
			break;
		default: break;
		}
	}
}
//�㭪�� PrintSortMenu
//�뢮��� ���� ���஢�� � ॠ����� ���஢�� ������ ᯨ᪠ �� ��������� ����
bool PrintSortMenu()
{
	int menuItems = 7;
	char** menuText = new char*[menuItems + 1];
	menuText[0] = "�롥�� ���� ��� ���஢��";
	menuText[1] = "����� �����";
	menuText[2] = "����� ��������";
	menuText[3] = "������������";
	menuText[4] = "����";
	menuText[5] = "������⢮";
	menuText[6] = "��� �த���";
	menuText[7] = " �������� �����";
	switch (PrintSimpleMenu(menuText, menuItems, 34))
	{
	case 0: SortList(0); return true; break;
	case 1: SortList(1); return true; break;
	case 2: SortList(4); return true; break;
	case 3: SortList(6); return true; break;
	case 4: SortList(7); return true; break;
	case 5: SortList(8); return true; break;
	default: return false;
	}
}
//�㭪�� PrintDataOnScreen
//�뢮��� ����� �����⭮�� ����� ᯨ᪠ � ॠ����� ����������� ��������� � 㤠����� �����
void PrintDataOnScreen(NodeInfo* data)
{
	char c;
	//��ࠬ���� �뢮������ ����
	int width = 50, height = 16, x = (80 - width) / 2, y = 4;
	//��⨢�� �㭪�
	int active = 2;
	//����� ।���஢����
	bool edit = false;
	//��६����� ��� �६������ �࠭���� ���������� ������
	NodeInfo temp = *data;
	//�뢮� ࠬ�� ����
	PrintWindow(width, height);
	//�뢮� 蠯��
	GotoXY(x + 1, y + 4);  printf("  ����� ��������: ");
	GotoXY(x + 1, y + 5);  printf("  ����� ᥪ樨: ");
	GotoXY(x + 1, y + 6);  printf("  ����� 祪�: ");
	GotoXY(x + 1, y + 7);  printf("  ������������: ");
	GotoXY(x + 1, y + 8);  printf("  ��⨪� ⮢��: ");
	GotoXY(x + 1, y + 9);  printf("  ����: ");
	GotoXY(x + 1, y + 10); printf("  ������⢮: ");
	GotoXY(x + 1, y + 11); printf("  ��� �த���: ");
	//�뢮� ������ � ��ࠡ�⪠ ������� ������ � ��࠭��� �㭪⮢ ����
	while (true)
	{
		ActiveOrNot(active, 11);
		GotoXY(x + 19, y + 4);  printf("%06.6i", temp.StoreNumber);
		ActiveOrNot(active, 12);
		GotoXY(x + 19, y + 5);  printf("%04.4i", temp.SectionNumber);
		ActiveOrNot(active, 13);
		GotoXY(x + 19, y + 6);  printf("%09.9i", temp.CheckNumber);
		ActiveOrNot(active, 14);
		GotoXY(x + 19, y + 7);  printf("%-30.30s", temp.ProductName);
		ActiveOrNot(active, 15);
		GotoXY(x + 19, y + 8);  printf("%-12.12s", temp.ProductArticle);
		ActiveOrNot(active, 16);
		GotoXY(x + 19, y + 9);   printf("%-8.2f", temp.Price);
		ActiveOrNot(active, 17);
		GotoXY(x + 19, y + 10); printf("%-4i", temp.ProductAmount);
		ActiveOrNot(active, 18);
		GotoXY(x + 19, y + 11); printf("%10.10s", temp.DateOfSale);
		GotoXY(x + 1, y + 13);
		ActiveOrNot(active, -1);
		printf("%*.*s", width - 2, width - 2, " ");
		GotoXY(x + 1, y + 2);
		printf("%*.*s", width - 2, width - 2, " ");
		//�뢮� ������ ࠡ��� � ����⮬
		if (!edit)
		{
			GotoXY(x + 18, y + 2);  printf("������ �%04i", temp.key);
			GotoXY(x + 5, y + 13);
			ActiveOrNot(active, 0);
			printf("�������");
			GotoXY(x + width / 2 - 4, y + 13);
			ActiveOrNot(active, 1);
			printf("��������");
			GotoXY(x + width - 13, y + 13);
			ActiveOrNot(active, 2);
			printf("�������");
		}
		//�뢮� ������ �� ।���஢���� �����
		else
		{
			GotoXY(x + 12, y + 2);  printf("������஢���� ����� �%04i", temp.key);
			GotoXY(x + 5, y + 13);
			ActiveOrNot(active, 5);
			printf("  ���࠭���  ");
			GotoXY(x + width - 18, y + 13);
			ActiveOrNot(active, 6);
			printf("��  ��࠭���");
		}
		SetColor(textColor, backgroundColor);
		c = _getch();
		//Esc - ��� ��室 �� ०��� ।���஢���� ��� ��室 �� ��ᬮ�� ������
		if (c == 27)
		{
			if (edit)
			{
				edit = false;
				active = 2;
			}
			else
				return;
		}
		//�롮� ����� ����
		if (c == 13)
		{
			switch (active)
			{
				//�������� ������
			case 0:
				//��������� �� ����� ��� �������� ���������� �������� �������������������
				/*							
				if (notesAmount > 16 && notesAmount % 16 == 1) {
					notesAmount--;
					notesForPrint--;
					PrintAll();
					notesForPrint++;
					notesAmount++;
				}*/
				DeleteNode((*data).key, active); //active--;
				return;
				//����� ।���஢����
			case 1:
				edit = true;
				temp = *data;
				active = 6;
				break;
				//��室 �� ��ᬮ��
			case 2: return;
				//��室 �� ०��� ।���஢���� � ��࠭����� ��।���஢����� ������
			case 5: *data = temp; edit = false; active = 2; break;
				//��室 �� ०��� ।���஢���� ��� ��࠭���� ��।���஢����� ������
			case 6: edit = false; active = 2; temp = *data;  break;
				//11-18 - ।���஢���� ࠧ����� ����� �����
			case 11:
				GotoXY(x + 19, y + 4); printf("%6.6s", " ");
				GotoXY(x + 19, y + 4);
				SetCursorVisibility(true);
				ReadIntFromScreen(&temp.StoreNumber, 6);
				SetCursorVisibility(false);
				break;
			case 12:
				GotoXY(x + 19, y + 5); printf("%4.4s", " ");
				GotoXY(x + 19, y + 5);
				SetCursorVisibility(true);
				ReadIntFromScreen(&temp.SectionNumber, 4);
				SetCursorVisibility(false);
				break;
			case 13:
				GotoXY(x + 19, y + 6); printf("%9.9s", " ");
				GotoXY(x + 19, y + 6);
				SetCursorVisibility(true);
				ReadIntFromScreen(&temp.CheckNumber, 9);
				SetCursorVisibility(false);
				break;
			case 14:
				GotoXY(x + 19, y + 7); printf("%30.30s", " ");
				GotoXY(x + 19, y + 7);
				SetCursorVisibility(true);
				ReadStringFromScreenWithoutRestrictions(temp.ProductName, 30);
				SetCursorVisibility(false);
				break;
			case 15:
				GotoXY(x + 19, y + 8); printf("%12.12s", " ");
				GotoXY(x + 19, y + 8);
				SetCursorVisibility(true);
				ReadArticleFromScreen(temp.ProductArticle, 12);
				SetCursorVisibility(false);
				break;
			case 16:
				GotoXY(x + 19, y + 9); printf("%8.8s", " ");
				GotoXY(x + 19, y + 9);
				SetCursorVisibility(true);
				ReadFloatFromScreen(&temp.Price, 5, 2);
				SetCursorVisibility(false);
				break;
			case 17:
				GotoXY(x + 19, y + 10); printf("%4.4s", " ");
				GotoXY(x + 19, y + 10);
				SetCursorVisibility(true);
				ReadIntFromScreen(&temp.ProductAmount, 4);
				SetCursorVisibility(false);
				break;
			case 18:
				GotoXY(x + 19, y + 11); printf("%10.10s", " ");
				GotoXY(x + 19, y + 11);
				SetCursorVisibility(true);
				ReadDateFromScreen(temp.DateOfSale);
				SetCursorVisibility(false);
				break;
			default: break;
			}
		}
		//��ࠡ�⪠ ������ ��५��
		if (c == -32)
		{
			c = _getch();
			switch (c)
			{
			case 75:
				if (!edit)
					active = (active + 2) % 3;
				else
				{
					if (active == 5 || active == 6)
						active = 5 + active % 2;
				}
				break;
			case 77:
				if (!edit)
					active = (active + 1) % 3;
				else
				{
					if (active == 5 || active == 6)
						active = 5 + active % 2;
				}
				break;
			case 72:
				if (edit)
				{
					if (active == 5 || active == 6)
						active = 18;
					else
						if (active == 11)
							active = 5;
						else
							active = 11 + (active + 4) % 8;
				}
				break;
			case 80:
				if (edit)
				{
					if (active == 5 || active == 6)
						active = 11;
					else
						if (active == 18)
							active = 5;
						else
							active = 11 + (active - 10) % 8;
				}
				break;
			default: break;
			}
		}
	}
}
//�㭪�� InitializePrint
//�������� ���樠������ ��६�����, �⢥���� �� ���ᮢ�� ᯨ᪠
//�ਭ����� � ����⢥ ��ࠬ��஢ ����� ⥪�饩 ��࠭���, 㪠��⥫� �� ��६����� � ������⢮� ��࠭�� � 㪠��⥫� �� 㪠��⥫� �� �������� ����� ��࠭���\
//�����頥� ������⢮ ����ᥩ �� ⥪�饩 ��࠭��
int InitializePrint(int currentPage, int* pageAmount, Node **pBegin)
{
	//����� ��饣� ������⢠ ��࠭��
	*pageAmount = (notesForPrint + notesOnPage - 1) / notesOnPage;
	*pBegin = pHead;
	//����� 㪠��⥫� �� �������� ����� ��࠭���
	for (int i = 0; i < (currentPage - 1) * notesOnPage;)
	{
		*pBegin = (*pBegin)->pNext;
		if ((*pBegin)->Visible)
			i++;
	}
	//������ ����⮢ �� ⥪�饩 ��࠭��
	//cout << notesForPrint << " " << notesOnPage << endl;
	if (notesForPrint - (notesOnPage* (currentPage - 1)) > notesOnPage)
		return notesOnPage;
	else
		return notesForPrint - (notesOnPage * (currentPage - 1));
}
//�㭪�� PrintWindow
//�������� �뢮� �� �࠭� ������� ࠬ�� ��������� ࠧ��� � � �������� ���न����
//�ਭ����� � ����⢥ ��ࠬ��஢ �ਭ� ����, ����� � ���न���� x � y �� �࠭�
void PrintWindow(int width, int height, int x, int y)
{
	//�᫨ �� ������ ���न����, ���� �� ��室� �� �ਭ� � ����� ����
	if (x == -1)
		x = (80 - width) / 2;
	if (y == -1)
		y = (25 - height) / 2;
	for (int i = 0; i < height; i++)
	{
		//�뢮� ���孥� ࠬ�� ����
		GotoXY(x, i + y);
		if (i == 0)
		{
			printf("�");
			for (int j = 0; j < width - 2; j++)
				printf("�");
			printf("�");
		}
		else
		{
			//�뢮� ������ ࠬ�� ����
			if (i == height - 1)
			{
				printf("�");
				for (int j = 0; j < width - 2; j++)
					printf("�");
				printf("�");
			}
			else
				//�뢮� ������� ࠬ�� ����
				printf("�% *.*s�", width - 2, width - 2, " ");
		}
	}
}
//�㭪�� PrintSimpleMenu
//�������� �뢮� ���� � ���� ���� �� �࠭ � ��ࠡ��� ������ ������
//�ਭ����� ᫥���騥 ��ࠬ����: menuItemsText - ��������� �㭪⮢ ���� � 0-�� ����� - ��������� ����
//menuItems - ������⢮ ����⮢ ����, width - �ਭ� ����, height - ���� ���� , window - �ॡ���� �� ࠬ��, x � y - ���न���� ������ ���孥�� 㣫� ����
//�����頥� ����� ��࠭���� �㭪� ���� (��� �� 0)
int PrintSimpleMenu(char** menuItemsText, int menuItems, int width, int height, bool window, int x, int y)
{
	//��।������ � ���樠������ ��ࠬ��஢ �뢮��
	int shiftY = 3;
	if (menuItemsText[0] == "")
		shiftY = 1;
	if (width == -1)
		width = 50;
	if (height == -1)
		height = menuItems + 3 + shiftY;
	if (x == -1)
		x = (80 - width) / 2;
	if (y == -1)
		y = (25 - height) / 2;
	char c;
	int activeMenuItem = 0;
	SetColor(textColor, backgroundColor);
	if (window)
		PrintWindow(width, height, x, y);
	//�᫨ ���� ���������, � �뢥��
	if (menuItemsText[0] != "")
	{
		GotoXY(x + (width - strlen(menuItemsText[0])) / 2, y + 2);
		printf("%s", menuItemsText[0]);
	}
	//�뢮� ���� � ��ࠡ�⪠ ������
	while (true)
	{
		for (int i = 1; i <= menuItems; i++)
		{
			GotoXY(x + 1, y + shiftY + i);
			ActiveOrNot(activeMenuItem, i - 1);
			printf("%-*s%-s%*s", (width - 2 - strlen(menuItemsText[i])) / 2, "[]", menuItemsText[i], (width - 2 - strlen(menuItemsText[i])) / 2, "[]");
		}
		c = _getch();
		switch (c)
		{
		case -32:
			c = _getch();
			switch (c)
			{
			case 72:  activeMenuItem = (activeMenuItem + menuItems - 1) % menuItems; break;
			case 80:  activeMenuItem = (activeMenuItem + 1) % menuItems; break;
			}
			break;
		case 13:
			if (activeMenuItem <= menuItems)
				return activeMenuItem;
			else
				return -2;
		case 27: return -1;
		default: break;
		}
	}
}
//�㭪�� PrintMessageBox
//�뢮��� �� �࠭ ���誮 ��������� ࠧ��� � ������� ⥪�⮬ � ��������
//btnMB - ������⢮ ������ (1 - 3), text - ⥪�� ᮮ�饭��, width � height - �ਭ� � ���� ���� ᮮ⢥��⢥���
//��⠫�� ��ࠬ���� ⥪�� �뢮���� �� �������
//�����頥� ����� ����⮩ ������ (��� �� 0)
int PrintMessageBox(ButtonsMessageBox btnMB, char* text, int width, int height, char* textFirstBtn, char* textSecondBtn, char* textThirdBtn)
{
	if (width == -1)
		width = 34;
	if (height == -1)
		height = 8;
	int	x = (80 - width) / 2;
	int y = (25 - height) / 2;
	PrintWindow(width, height, x, y);
	GotoXY(x + (width - strlen(text)) / 2, y + 3);
	printf("%s", text);
	char c;
	int active = 0, itemCount = btnMB;
	while (true)
	{
		switch (btnMB)
		{
		case OneButton:
			GotoXY(x + (width - strlen(textFirstBtn)) / 2, y + height - 3);
			SetColor(activeTextColor, activeBackgroundColor);
			printf("%s", textFirstBtn);
			break;
		case TwoButton:
			GotoXY(x + (width / 2 - strlen(textFirstBtn)) / 2, y + height - 3);
			ActiveOrNot(active, 0);
			printf("%s", textFirstBtn);
			GotoXY(x + width / 2 + (width / 2 - strlen(textSecondBtn)) / 2, y + height - 3);
			ActiveOrNot(active, 1);
			printf("%s", textSecondBtn);
			break;
		case ThreeButton:
			GotoXY(x + (width / 3 - strlen(textFirstBtn)) / 2, y + height - 3);
			ActiveOrNot(active, 0);
			printf("%s", textFirstBtn);
			GotoXY(x + width / 3 + (width / 3 - strlen(textSecondBtn)) / 2, y + height - 3);
			ActiveOrNot(active, 1);
			printf("%s", textSecondBtn);
			GotoXY(x + width * 2 / 3 + (width / 3 - strlen(textThirdBtn)) / 2, y + height - 3);
			ActiveOrNot(active, 2);
			printf("%s", textThirdBtn);
			break;
		}
		c = getch();

		switch (c)
		{
		case 27:
			SetColor(textColor, backgroundColor);
			return -1;
		case -32:
			c = _getch();
			switch (c)
			{
			case 75:  active = (active + itemCount) % (itemCount + 1); break; /////////////////////////////////////////////////////
			case 77:  active = (active + 1) % (itemCount + 1); break;
			}
			break;
		case 13:
			SetColor(textColor, backgroundColor);
			return active;
		}
	}
}

//�㭪�� ReadFileName
//�����뢠�� ���� ����� ��� � 䠩��
//�ਭ����� ��뫪� �� ���ᨢ char � ����� �����뢠�� ��⠭�� ����
//�����頥� �᫮, ᨬ��������饥 �ᯥ譮���, �⬥��  ��� ᡮ� ����樨
int ReadFileName(char* path)
{
	int width = 70, height = 6;
	PrintWindow(width, height);
	GotoXY(8, 11);
	printf("������ ���� � 䠩��: (���ਬ��: E:\\BIN.dat ��� TEXT.txt)");
	GotoXY(11, 12);
	SetCursorVisibility(true);
	height = ReadFileNameFromScreen(path, 60);
	SetCursorVisibility(false);
	return height;
}
//�㭪�� WriteIntoBinaryFile
//�믮���� ������ ������ � ������ 䠩�
//�ਭ����� ��ப� - ���� � 䠩��
//�����頥� �㫥��� ���祭��, ᨬ��������饥 �ᯥ譮���, �⬥��  ��� ᡮ� ����樨
bool WriteIntoBinaryFile(char* path)
{
	//䠩����� ��६�����
	FILE *f;
	//�஢�ઠ �� �ᯮ�짮����� �⠭���⭮�� 䠩��
	if (strcmp(path, "\0") == 0)
		f = fopen(binFile, "wb");
	else
		f = fopen(path, "wb");
	//�஢�ઠ ������ 䠩��
	if (!f)
	{
		PrintMessageBox(OneButton, "�訡�� ������ 䠩��");
		return false;
	}
	//��⠭���� 㪠��⥫� � 䠩��
	fseek(f, 0, SEEK_SET);
	//��।������ ࠧ��� �������� ������
	int sizeNote = sizeof(NodeInfo);
	//������ ������ � 䠩��
	pTemp = pHead;
	while (pTemp != NULL)
	{
		fwrite(&pTemp->data, sizeNote, 1, f);
		pTemp = pTemp->pNext;
	}
	//�����⨥ 䠩��
	fclose(f);
	return true;
}
//�㭪�� ReadFromBinaryFile
//�믮���� �⥭�� ������ �� ����୮�� 䠩��
//�ਭ����� ��ப� - ���� � 䠩��
//�����頥� �㫥��� ���祭��, ᨬ��������饥 �ᯥ譮���, �⬥��  ��� ᡮ� ����樨
bool ReadFromBinaryFile(char* path)
{
	//䠩����� ��६�����
	FILE *f;
	//������� ������
	NodeInfo data;
	//�஢�ઠ �ᯮ�짮����� �⠭���⭮�� 䠩��
	if (strcmp(path, "\0") == 0)
		f = fopen(binFile, "rb");
	else
		f = fopen(path, "rb");
	//�஢�ઠ �ᯥ譮�� ������ 䠩��
	if (!f)
	{
		PrintMessageBox(OneButton, "�訡�� ������ 䠩��");
		return false;
	}
	//��⠭������� 㪠��⥫� �� ��砫� 䠩��
	rewind(f);
	//��।������ ࠧ��� �������� ������
	int sizeNote = sizeof(NodeInfo);
	//���� �� ����� 䠩�� �⥭�� ������ � ���������� � ᯨ᮪
	while (!feof(f))
	{
		if (fread(&data, sizeNote, 1, f) < 1)
			break;
		AddNode(data);
	}
	//�����⨥ 䠩��
	fclose(f);
	return true;
}
//�㭪�� WriteIntoTextFile
//�믮���� ������ ������ � ⥪�⮢� 䠩�
//�ਭ����� ��ப� - ���� � 䠩��
//�����頥� �㫥��� ���祭��, ᨬ��������饥 �ᯥ譮���, �⬥��  ��� ᡮ� ����樨
bool WriteIntoTextFile(char* path)
{
	//䠩����� ��६�����
	FILE *f;
	//�஢�ઠ �ᯮ�짮����� �⠭������ 䠩���
	if (strcmp(path, "\0") == 0)
		f = fopen(txtFile, "w");
	else
		f = fopen(path, "w");
	//�஢�ઠ �ᯥ譮�� ������ 䠩��
	if (!f)
	{
		PrintMessageBox(OneButton, "�訡�� ������ 䠩��");
		return false;
	}
	pTemp = pHead;
	//����� 蠯�� ⠡����
	fprintf(f, "�����������������������������������������������������������������������������������������������������������Ŀ\n");
	fprintf(f, "� �����  ������ �    �����   �          ������������          �   ��⨪�    �   ����   ����-���    ���    �\n");
	fprintf(f, "����������ᥪ樨�    祪�    �             ⮢��             �    ⮢��    �          �      �   �த���  �\n");
	fprintf(f, "�����������������������������������������������������������������������������������������������������������Ĵ\n");
	//���� �� ����� ᯨ᪠ ������ ����� � 䠩�
	while (pTemp != NULL)
	{
		fprintf(f, "� %-06.6i � %-04.4i �  %-09.9i � %-30.30s � %-12.12s � %8.2f � %4i � %10.10s �\n", pTemp->data.StoreNumber, pTemp->data.SectionNumber, pTemp->data.CheckNumber, pTemp->data.ProductName, pTemp->data.ProductArticle, pTemp->data.Price, pTemp->data.ProductAmount, pTemp->data.DateOfSale);
		pTemp = pTemp->pNext;
	}
	//����� ����砭�� ⠡����
	fprintf(f, "�������������������������������������������������������������������������������������������������������������\n");
	//�����⨥ 䠩��
	fclose(f);
	return true;
}
//�㭪�� ReadFromTextFile
//�믮���� �⥭�� ������ �� ⥪�⮢��� 䠩��
//�ਭ����� ��ப� - ���� � 䠩��
//�����頥� �㫥��� ���祭��, ᨬ��������饥 �ᯥ譮���, �⬥��  ��� ᡮ� ����樨
bool ReadFromTextFile(char *path)
{
	//䠩����� ��६�����
	FILE *f;
	//������� ������
	NodeInfo data;
	//�஢�ઠ �� �ᯮ�짮����� �⠭������ 䠩���
	if (strcmp(path, "\0") == 0)
		f = fopen(txtFile, "r");
	else
		f = fopen(path, "r");
	//�஢�ઠ �ᯥ譮�� ������ 䠩��
	if (!f)
	{
		PrintMessageBox(OneButton, "�訡�� ������ 䠩��");
		return false;
	}
	//��६���� ��� �࠭���� ���뢠����� ᨬ���� � ��ࠡ��뢠����� ���ᨢ� ᨬ�����
	char c, tempStr[31];
	//䫠� �� ����稥 �訡���� ������ � ��ப�
	bool badStr = false;
	//��६���� �������, field - ���뢠����� ����, k - ������ ��ࠡ��뢠����� ���ᨢ� ᨬ�����
	int field = 0, k = 0;
	//���� �� ����� 䠩��
	while (!feof(f))
	{
		//���� ᨬ���
		c = fgetc(f);
		//�ய����� �஡��� � ᨬ���� ⠡����
		while (c == '�' || c == '�' || c == '�' || c == '�' || c == '�' || c == '�' || c == '�' || c == '�' || c == '�' || c == '�' || c == '�' || c == ' ' || c == 13 || c == 10)
			c = fgetc(f);
		//��।����� ����� �� ����� ���뢠����
		switch (field)
		{
		case 0:
			while (true)
			{
				if (feof(f) || (c == '\n'))
				{
					badStr = true;
					break;
				}
				if ((c >= '0' && c <= '9') && k < 6)
				{
					tempStr[k++] = c;
				}
				if (c == 9 || c == '�')
				{
					if (k == 0)
					{
						badStr = true;
						break;
					}
					tempStr[k] = '\0';
					data.StoreNumber = atoi(tempStr);
					field++; k = 0;
					break;
				}
				c = fgetc(f);
			}
			break;
		case 1:
			while (true)
			{
				if (feof(f) || (c == '\n'))
				{
					badStr = true;
					break;
				}
				if ((c >= '0' && c <= '9') && k < 4)
				{
					tempStr[k++] = c;
				}
				if (c == 9 || c == '�')
				{
					if (k == 0)
					{
						badStr = true;
						break;
					}
					tempStr[k] = '\0';
					data.SectionNumber = atoi(tempStr);
					field++; k = 0;
					break;
				}
				c = fgetc(f);
			}
			break;
		case 2:
			while (true)
			{
				if (feof(f) || (c == '\n'))
				{
					badStr = true;
					break;
				}
				if ((c >= '0' && c <= '9') && k < 9)
				{
					tempStr[k++] = c;
				}
				if (c == 9 || c == '�')
				{
					if (k == 0)
					{
						badStr = true;
						break;
					}
					tempStr[k] = '\0';
					data.CheckNumber = atoi(tempStr);
					field++; k = 0;
					break;
				}
				c = fgetc(f);
			}
			break;
		case 3:
			while (true)
			{
				if (feof(f) || (c == '\n'))
				{
					badStr = true;
					break;
				}
				if (c == 9 || c == '�')
				{
					if (k == 0)
					{
						badStr = true;
						break;
					}
					tempStr[k] = '\0';
					strcpy(data.ProductName, tempStr);
					field++; k = 0;
					break;
				}
				if (k < 30)
					tempStr[k++] = c;
				c = fgetc(f);
			}
			break;
		case 4:
			while (true)
			{
				if (feof(f) || (c == '\n'))
				{
					badStr = true;
					break;
				}
				if (c == 9 || c == '�')
				{
					if (k == 0)
					{
						badStr = true;
						break;
					}
					tempStr[k] = '\0';
					strcpy(data.ProductArticle, tempStr);
					field++; k = 0;
					break;
				}
				if (k < 12 && ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '�' && c <= '�') || (c >= '�' && c <= '�') || (c >= '�' && c <= '�')))
					tempStr[k++] = c;
				c = fgetc(f);
			}
			break;
		case 5:
			while (true)
			{
				if (feof(f) || (c == '\n'))
				{
					badStr = true;
					break;
				}
				if (k < 8 && ((c >= '0' && c <= '9') || (c == '.' || c == ',')))
				{
					tempStr[k++] = c;
				}
				if (c == 9 || c == '�')
				{
					if (k == 0)
					{
						badStr = true;
						break;
					}
					tempStr[k] = '\0';
					data.Price = atof(tempStr);
					field++; k = 0;
					break;
				}
				c = fgetc(f);
			}
			break;
		case 6:
			while (true)
			{
				if (feof(f) || (c == '\n'))
				{
					badStr = true;
					break;
				}
				if ((c >= '0' && c <= '9') && k < 4)
				{
					tempStr[k++] = c;
				}
				if (c == 9 || c == '�')
				{
					if (k == 0)
					{
						badStr = true;
						break;
					}
					tempStr[k] = '\0';
					data.ProductAmount = atoi(tempStr);
					field++; k = 0;
					break;
				}
				c = fgetc(f);
			}
			break;
		case 7:
			while (true)
			{
				if (feof(f) || (c == '\n'))
				{
					badStr = true;
					break;
				}
				if ((k != 2 && k != 5 && k < 10 && (c >= '0' && c <= '9')) || ((k == 2 || k == 5) && (c == '.' || c == ',')))
				{
					tempStr[k++] = c;
				}
				if (c == 9 || c == '�')
				{
					if (k == 0)
					{
						badStr = true;
						break;
					}
					tempStr[k] = '\0';
					strcpy(data.DateOfSale, tempStr);
					field++; k = 0;
					break;
				}
				c = fgetc(f);
			}
			break;
		}
		//�᫨ ��ப� � �訡���� ������ �� ���� ��ப�, ���㫨�� �������
		if (badStr)
		{
			while (c != 10 && !feof(f))
				c = fgetc(f);
			badStr = false; field = 0; k = 0;
			continue;
		}
		//�᫨ ����� ��⠭� ��������� �������� � ᯨ᮪
		if (field == 8)
		{
			AddNode(data);
			field = 0;
		}
	}
	//������� 䠩�
	fclose(f);
	return true;
}

//�㭪�� AddNode
//���������� ������ � ᯨ᮪
//�ਭ����� ������塞� �����
void AddNode(NodeInfo elem)
{
	//�᫨ ���� -1 � ����� �� ����������
	if (elem.key == -1)
		return;
	//�᫨ ᯨ᮪ ����
	if (pHead == NULL)
	{
		pHead = new Node;
		pHead->pNext = NULL;
		pHead->pPrev = NULL;
		pHead->data = elem;
		pHead->data.key = 1;
		pTail = pHead;
		pTemp = pHead;
	}
	//����
	else
	{
		pTemp = new Node;
		elem.key = notesAmount + 1;
		pTemp->data = elem;
		pTemp->pNext = NULL;
		pTemp->pPrev = pTail;
		pTail->pNext = pTemp;
		pTail = pTemp;
	}
	//����� ����� ��᫥ ����������
	pTemp->Visible = true;
	pTemp = NULL;
	//���६��� ������⢠ �뢮����� ����ᥩ � ��饣� ������⢠ ����ᥩ
	notesAmount++;
	notesForPrint++;
}
//�㭪�� DeleteNode
//������ ����� ᯨ᪠ �� �����
//�ਭ����� ���� � ����⢥ ��ࠬ���
void DeleteNode(int &key, int &active)
{
	if (pHead == NULL)
		return;
	Node * pDelete = pHead;
	//���� 㤠�塞��� �����
	for (int i = 1; i < key; i++)
	{
		pDelete = pDelete->pNext;
	}
	
	if (pDelete == pHead) {
		if (pHead == pTail) {
			pHead = pTail = NULL;
		}
		else {
			pHead = pHead->pNext;
			pHead->pPrev = NULL;
		}
	}
	else {
		if (pDelete == pTail) {
			pTail = pTail->pPrev;
			pTail->pNext = NULL;
		}
		else {
			pDelete->pNext->pPrev = pDelete->pPrev;
			pDelete->pPrev->pNext = pDelete->pNext;
		}
	}

/*	delete(pDelete);
	notesAmount--;
	notesForPrint--;
	//key--;
	//active--;
	*/
	/*
	if (pDelete == pTail)
	{
		pTail = pTail->pPrev;
		if (pTail) {
			pTail->pNext = 0;
		}
		key--;
		active = key;
		notesAmount--;
		notesForPrint--;
		delete(pDelete);
		key = active;
		return;
	}
	//��������� 㪠��⥫��
	if (pDelete == pHead)
	{
		if (pHead == pTail)
		{
			pHead = NULL;
			pTail = NULL;
		}
		else
		{
			pHead = pHead->pNext;
			pHead->pPrev = NULL;
		}
	}
	else
	{
		if (pDelete == pTail)
		{
			pTail = pTail->pPrev;
			pTail->pNext = NULL;
		}
		else
		{
			pDelete->pNext->pPrev = pDelete->pPrev;
			pDelete->pPrev->pNext = pDelete->pNext;
		}
	}
	*/
	pTemp = pHead;
	//��������� ���祩 � ᯨ᪥
	while (pTemp != NULL)
		{
		if (pTemp->data.key > pDelete->data.key)
			pTemp->data.key -= 1;
		pTemp = pTemp->pNext;
	}
	delete(pDelete);
	notesAmount--;
	notesForPrint--;
	//�᢮�������� �����
	/* delete(pDelete);    //������� �� ���� ������ �� �����
	notesAmount--;
	active--;
	key--;
	notesForPrint--;*/
}


//�㭪�� DeleteNode
//������ ����� ᯨ᪠ �� ��뫪�
//�ਭ����� ���� � ����⢥ ��ࠬ���
/*void DeleteNode(Node * pDelete)
{
	if (pDelete == NULL)
		return;
	//��������� 㪠��⥫��
	if (pDelete == pHead)
	{
		if (pHead == pTail)
		{
			pHead = NULL;
			pTail = NULL;
		}
		else
		{
			pHead = pHead->pNext;
			pHead->pPrev = NULL;
		}
	}
	else
	{
		if (pDelete == pTail)
		{
			pTail = pTail->pPrev;
			pTail->pNext = NULL;
		}
		else
		{
			pDelete->pNext->pPrev = pDelete->pPrev;
			pDelete->pPrev->pNext = pDelete->pNext;
		}
	}
	pTemp = pHead;
	//��������� ���祩 � ᯨ᪥
	while (pTemp != NULL)
	{
		if (pTemp->data.key > pDelete->data.key)
			pTemp->data.key -= 1;
		pTemp = pTemp->pNext;
	}
	//�ᮢ�������� �����
	delete(pDelete);
	notesAmount--;
	notesForPrint--;
}
//�㭪�� ClearList
//���⪠ �ᥣ� ᯨ᪠
*/
void ClearList()
{
	while (pHead != NULL)
	{
		pTemp = pHead;
		pHead = pHead->pNext;
		delete(pTemp);
	}
	pHead = pTemp = pTail = NULL;
	notesAmount = 0;
	notesForPrint = 0;
}
//�㭪�� SortList
//����஢�� ᯨ᪠ �� ��������� ����
//field - ����� ���� ���஢��
void SortList(int field)
{
	//१���� �ࠢ�����
	int res;
	if (pHead == NULL)
	{
		return;
	}
	//����쪮��� ���஢��
	for (Node *X = pHead; X->pNext; X = X->pNext)
		for (Node *pTemp = pTail; pTemp != X; pTemp = pTemp->pPrev)
		{
			switch (field)
			{
			case 0: res = pTemp->data.key < pTemp->pPrev->data.key; break;
			case 1:
				res = pTemp->data.StoreNumber < pTemp->pPrev->data.StoreNumber;
				if (pTemp->data.StoreNumber == pTemp->pPrev->data.StoreNumber)
					res = pTemp->data.key < pTemp->pPrev->data.key;
				break;
			case 2:
				res = pTemp->data.SectionNumber < pTemp->pPrev->data.SectionNumber;
				if (pTemp->data.SectionNumber == pTemp->pPrev->data.SectionNumber)
					res = pTemp->data.key < pTemp->pPrev->data.key;
				break;
			case 3:
				res = pTemp->data.CheckNumber < pTemp->pPrev->data.CheckNumber;
				if (pTemp->data.CheckNumber == pTemp->pPrev->data.CheckNumber)
					res = pTemp->data.key < pTemp->pPrev->data.key;
				break;
			case 4:
				res = strcmp(pTemp->pPrev->data.ProductName, pTemp->data.ProductName);
				if (res == 0)
					res = pTemp->data.key < pTemp->pPrev->data.key;
				break;
			case 5:
				res = strcmp(pTemp->pPrev->data.ProductArticle, pTemp->data.ProductArticle);
				if (res == 0)
					res = pTemp->data.key < pTemp->pPrev->data.key;
				break;
			case 6:
				res = pTemp->data.Price < pTemp->pPrev->data.Price;
				if (pTemp->data.Price == pTemp->pPrev->data.Price)
					res = pTemp->data.key < pTemp->pPrev->data.key;
				break;
			case 7:
				res = pTemp->data.ProductAmount < pTemp->pPrev->data.ProductAmount;
				if (pTemp->data.ProductAmount == pTemp->pPrev->data.ProductAmount)
					res = pTemp->data.key < pTemp->pPrev->data.key;
				break;
			case 8:
				res = CompareDate(pTemp->data.DateOfSale, pTemp->pPrev->data.DateOfSale);
				if (res == 0)
					res = pTemp->data.key < pTemp->pPrev->data.key;
				break;
			default: return;
			}
			if (res > 0)
				Swap(pTemp, pTemp->pPrev);
		}
}
//�㭪�� ProcessList
//��ࠡ�⪠ ������ ᯨ᪠
void ProcessList()
{
	//�����⥫� �� ᯨ᮪ ��ࠡ�⠭��� ������
	ProcessNode *pProcHead, *pProcTail, *pProcTemp;
	//��饥 ������⢮ ����ᥩ � ������⢮ ����ᥩ �� ��࠭��
	int amount = 1, amountOnPage = 16;
	if (pHead == NULL)
	{
		PrintMessageBox(OneButton, "���᮪ ����!");
		return;
	}
	SortList(1);
	//���������� ��������� �����
	pProcHead = new ProcessNode;
	pProcHead->StoreNumber = pHead->data.StoreNumber;
	pProcHead->Profit = pHead->data.Price * pHead->data.ProductAmount;
	pProcHead->pNext = NULL;
	pProcTail = pProcHead;
	pTemp = pHead->pNext;
	//��ࠡ�⪠ ��⠫��� ������
	while (pTemp != NULL)
	{
		if (pTemp->data.StoreNumber == pProcTail->StoreNumber)
			pProcTail->Profit += pTemp->data.Price * pTemp->data.ProductAmount;
		else
		{
			pProcTail->pNext = new ProcessNode;
			pProcTail = pProcTail->pNext;
			pProcTail->StoreNumber = pTemp->data.StoreNumber;
			pProcTail->Profit = pTemp->data.Price * pTemp->data.ProductAmount;
			pProcTail->pNext = NULL;
			amount++;
		}
		pTemp = pTemp->pNext;
	}
	//�뢮� ������ �� �࠭
	char c;
	int page = 1, pageAmount = (amount + amountOnPage - 1) / amountOnPage, i = 0;
	ProcessNode *pBegin = pProcHead, *pPrint;
	while (true)
	{
		SetColor(textColor, backgroundColor);
		pBegin = pProcHead; i = 0;
		while (i < (page - 1) * amountOnPage)
		{
			pBegin = pBegin->pNext;
			i++;
		}
		pPrint = pBegin;
		system("cls"); i = 0;
		printf("\n                          ��࠭�� �%-3i  �� %-3i\n", page, pageAmount);
		printf("                     �������������������������������Ŀ\n");
		printf("                     � �����  �        ��室         �\n");
		printf("                     ����������       ��������       �\n");
		printf("                     �������������������������������Ĵ\n");
		while (i < amountOnPage)
		{
			if (pPrint != NULL)
			{
				printf("                     � %-06.6i � %20.2f �\n", pPrint->StoreNumber, pPrint->Profit);
				pPrint = pPrint->pNext;
			}
			else
				printf("                     � %6s � %20s �\n", "", "");
			i++;
		}
		printf("                     ���������������������������������");
		c = _getch();
		//��ࠡ�⪠ ������ ������
		switch (c)
		{
		case -32:
			c = _getch();
			switch (c)
			{
			case 75:  if (page > 1) page--; break;
			case 77:  if (page < pageAmount) page++; break;
			}
			break;
		case 27:
			switch (PrintProcessMenu())
			{
			case 0: break;
			case 1:
				FILE* f;
				char path[61];
				if (ReadFileName(path) == -1)
					continue;
				f = fopen(path, "w");
				if (!f)
				{
					PrintMessageBox(OneButton, "�訡�� ������ 䠩��");
					return;
				}
				pProcTemp = pProcHead;
				if (pProcTemp == NULL)
					break;
				fprintf(f, "�������������������������������Ŀ\n");
				fprintf(f, "� �����  �        ��室         �\n");
				fprintf(f, "����������       ��������       �\n");
				fprintf(f, "�������������������������������Ĵ\n");
				while (pProcTemp != NULL)
				{
					fprintf(f, "� %-06.6i � %20.2f �\n", pProcTemp->StoreNumber, pProcTemp->Profit);
					pProcTemp = pProcTemp->pNext;
				}
				fprintf(f, "���������������������������������");
				fclose(f);
				break;
			case 2: SortList(0); return;
			default: break;
			}
		default: break;
		}
	}
}

//�㭪�� Search
//��������� ���᪠ ������ �� ᯨ��
void Search()
{
	//��࠭�� ���� ���᪠, ��⨢�� ����� ����, ���� ���᪠ � ��ࠬ���� ���᪠
	int page = 1, activeMenuItem = 0, menuItems = 2, fieldSearch = -1, paramSearch = -1;
	//���ᨢ� �������� ����⮢ ����, �������� �����, �������� ��ࠬ��஢
	char c, **menuText, **fields = new char*[9], **params = new char*[9];
	fields[0] = "����� �����"; fields[1] = "����� ��������"; fields[2] = "����� ᥪ樨"; fields[3] = "����� 祪�";
	fields[4] = "������������"; fields[5] = " ��⨪�";       fields[6] = "����";         fields[7] = "������⢮"; fields[8] = "��� �த���";
	params[0] = "��筮� ᮢ������� "; params[1] = "����� ���������";   params[2] = "����� ��� ࠢ�� ���������"; params[3] = "����� ��� ࠢ�� ���������";
	params[4] = "����� ���������";   params[5] = "�室�� � �������� "; params[6] = "�� �室�� � ��������";       params[7] = "����稥 ��������� ";
	SetColor(textColor, backgroundColor);
	//�뢮� ������� ࠬ��
	int width = 34, height = 16, x = (80 - width) / 2, y = (25 - height) / 2;
	PrintWindow(width, height);
	//�뢮� ����
	while (true)
	{
		switch (page)
		{
		case 0:
			break;
			//��࠭�� �롮� ���� ���᪠
		case 1:
			menuText = new char*[11];
			menuText[0] = "�롥�� ���� ���᪠";
			menuText[1] = "����� �����";  menuText[2] = "����� ��������"; menuText[3] = "����� ᥪ樨";  menuText[4] = "����� 祪�";
			menuText[5] = "������������";  menuText[6] = " ��⨪�";	   menuText[7] = "����";		   menuText[8] = "������⢮";
			menuText[9] = "��� �த���";  menuText[10] = " �������";
			fieldSearch = PrintSimpleMenu(menuText, 10, 34, 16);
			if (fieldSearch > 8 || fieldSearch < 0)
				return;
			page = 2;
			continue;
			//��࠭�� �롮� ��ࠬ��஢ ���᪠
		case 2:
			if (fieldSearch == 4 || fieldSearch == 5)
			{
				menuText = new char*[4];
				menuText[0] = "�롥�� ��ࠬ���� ���᪠"; menuText[1] = "��筮� ᮢ�������";
				menuText[2] = "����稥 ���������";		   menuText[3] = "�����";
				paramSearch = PrintSimpleMenu(menuText, 3, 34, 16);
				if (paramSearch > 2 || paramSearch < -1)
					return;
				if (paramSearch == 2)
					paramSearch = -1;
				if (paramSearch == 1)
					paramSearch = 7;
			}
			else
			{
				menuText = new char*[9];
				menuText[0] = "�롥�� ��ࠬ���� ���᪠";  menuText[1] = "��筮� ᮢ������� ";
				menuText[2] = "����� ���������";			menuText[3] = "����� ��� ࠢ�� ���������";
				menuText[4] = "����� ��� ࠢ�� ���������"; menuText[5] = "����� ���������";
				menuText[6] = "�室�� � �������� ";			menuText[7] = "�� �室�� � ��������";
				menuText[8] = "����� ";
				paramSearch = PrintSimpleMenu(menuText, 8, 34, 16);
				if (paramSearch == 7)
					paramSearch = -1;
				if (paramSearch > 7 || paramSearch < -1)
					return;
			}
			if (paramSearch == -1)
				page = 1;
			else
				page = 3;
			continue;
			//��࠭�� ����� ������ ���᪠ � �맮�� �ॡ㥬�� �㭪権 ��� ��� ॠ����樨
		case 3:
			GotoXY(x + 4, y + 2);
			printf("������ ����� ��� ���᪠");
			for (int i = 0; i < height - 4; i++)
			{
				GotoXY(x + 1, y + 3 + i);
				printf("%*s", width - 2, " ");
			}
			GotoXY(x + 5, y + 4);
			printf("���� ���᪠:");
			GotoXY(x + 1, y + 5);
			printf("%*s%s", (width - 2 - strlen(fields[fieldSearch])) / 2, " ", fields[fieldSearch]);
			GotoXY(x + 5, y + 6);
			printf("��ࠬ��� ���᪠:");
			GotoXY(x + 1, y + 7);
			printf("%*s%s", (width - 2 - strlen(params[paramSearch])) / 2, " ", params[paramSearch]);
			switch (fieldSearch)
			{
			case 0: SearchKey(paramSearch, x, y);
				break;
			case 1: SearchStoreNumber(paramSearch, x, y);
				break;
			case 2: SearchSectionNumber(paramSearch, x, y);
				break;
			case 3: SearchCheckNumber(paramSearch, x, y);
				break;
			case 4: SearchProductName(paramSearch, x, y);
				break;
			case 5: SearchProductArticle(paramSearch, x, y);
				break;
			case 6: SearchPrice(paramSearch, x, y);
				break;
			case 7: SearchProductAmount(paramSearch, x, y);
				break;
			case 8: SearchDateOfSale(paramSearch, x, y);
				break;
			default: break;
			}
			//�᫨ ���� �� ��� १���⮢
			if (pHead == NULL)
			{
				return;
			}
			if (notesForPrint == 0)
			{
				ShowAll();
				PrintMessageBox(OneButton, "���� १���⮢ �� ���!");
				page = 3;
				//continue;
				return;
			}
			return;
		default: break;
		}
	}
}
//�㭪樨 Search**
//��������� ���� ������ � ���������� ��� �������� ���� � ���� �������� ������
//�ਭ����� ��ࠬ��� ���᪠ � ���न����� ���� ����� ������
void SearchKey(int paramSearch, int x, int y)
{
	int number = 0, number1 = 0, number2 = 0;
	bool res;
	SetCursorVisibility(true);
	if (paramSearch == 5 || paramSearch == 6)
	{
		GotoXY(x + 5, y + 9);
		printf("���祭�� 1: ");
		GotoXY(x + 5, y + 11);
		printf("���祭�� 2: ");
		GotoXY(x + 2, y + 10);
		if (ReadIntFromScreen(&number1, 4) == -1)
			return;
		GotoXY(x + 2, y + 12);
		if (ReadIntFromScreen(&number2, 4) == -1)
			return;
		if (number1 > number2)
		{
			number = number1;
			number1 = number2;
			number2 = number;
		}
	}
	else
	{
		GotoXY(x + 5, y + 9);
		printf("���祭��: ");
		GotoXY(x + 2, y + 10);
		if (ReadIntFromScreen(&number, 4) == -1)
			return;
	}
	SetCursorVisibility(false);
	pTemp = pHead;
	notesForPrint = 0;
	while (pTemp != NULL)
	{
		if (pTemp->Visible)
		{
			switch (paramSearch)
			{
			case 0:	res = pTemp->data.key == number; break;
			case 1:	res = pTemp->data.key < number; break;
			case 2:	res = pTemp->data.key <= number; break;
			case 3:	res = pTemp->data.key >= number; break;
			case 4:	res = pTemp->data.key > number; break;
			case 5: res = (pTemp->data.key >= number1 && pTemp->data.key <= number2); break;
			case 6: res = (pTemp->data.key < number1 || pTemp->data.key > number2); break;
			default: break;
			}
			if (res)
				notesForPrint++;
			else
				pTemp->Visible = false;
		}
		pTemp = pTemp->pNext;
	}
}
void SearchStoreNumber(int paramSearch, int x, int y)
{
	int number, number1, number2;
	bool res;
	SetCursorVisibility(true);
	if (paramSearch == 5 || paramSearch == 6)
	{
		GotoXY(x + 5, y + 9);
		printf("���祭�� 1: ");
		GotoXY(x + 5, y + 11);
		printf("���祭�� 2: ");
		GotoXY(x + 2, y + 10);
		if (ReadIntFromScreen(&number1, 6) == -1)
			return;
		GotoXY(x + 2, y + 12);
		if (ReadIntFromScreen(&number2, 6) == -1)
			return;
		if (number1 > number2)
		{
			number = number1;
			number1 = number2;
			number2 = number;
		}
	}
	else
	{
		GotoXY(x + 5, y + 9);
		printf("���祭��: ");
		GotoXY(x + 2, y + 10);
		if (ReadIntFromScreen(&number, 6) == -1)
			return;
	}
	SetCursorVisibility(false);
	pTemp = pHead;
	notesForPrint = 0;
	while (pTemp != NULL)
	{
		if (pTemp->Visible)
		{
			switch (paramSearch)
			{
			case 0:	res = pTemp->data.StoreNumber == number; break;
			case 1:	res = pTemp->data.StoreNumber < number; break;
			case 2:	res = pTemp->data.StoreNumber <= number; break;
			case 3:	res = pTemp->data.StoreNumber >= number; break;
			case 4:	res = pTemp->data.StoreNumber > number; break;
			case 5: res = pTemp->data.StoreNumber >= number1 && pTemp->data.StoreNumber <= number2; break;
			case 6: res = pTemp->data.StoreNumber < number1 || pTemp->data.StoreNumber > number2; break;
			default: break;
			}
			if (res)
				notesForPrint++;
			else
				pTemp->Visible = false;
		}
		pTemp = pTemp->pNext;
	}
}
void SearchSectionNumber(int paramSearch, int x, int y)
{
	int number, number1, number2;
	bool res;
	SetCursorVisibility(true);
	if (paramSearch == 5 || paramSearch == 6)
	{
		GotoXY(x + 5, y + 9);
		printf("���祭�� 1: ");
		GotoXY(x + 5, y + 11);
		printf("���祭�� 2: ");
		GotoXY(x + 2, y + 10);
		if (ReadIntFromScreen(&number1, 4) == -1)
			return;
		GotoXY(x + 2, y + 12);
		if (ReadIntFromScreen(&number2, 4) == -1)
			return;
		if (number1 > number2)
		{
			number = number1;
			number1 = number2;
			number2 = number;
		}
	}
	else
	{
		GotoXY(x + 5, y + 9);
		printf("���祭��: ");
		GotoXY(x + 2, y + 10);
		if (ReadIntFromScreen(&number, 4) == -1)
			return;
	}
	if (number == -1 || number1 == -1 || number2 == -1)
		return;
	SetCursorVisibility(false);
	pTemp = pHead;
	notesForPrint = 0;
	while (pTemp != NULL)
	{
		if (pTemp->Visible)
		{
			switch (paramSearch)
			{
			case 0:	res = pTemp->data.SectionNumber == number; break;
			case 1:	res = pTemp->data.SectionNumber < number; break;
			case 2:	res = pTemp->data.SectionNumber <= number; break;
			case 3:	res = pTemp->data.SectionNumber >= number; break;
			case 4:	res = pTemp->data.SectionNumber > number; break;
			case 5: res = pTemp->data.SectionNumber >= number1 && pTemp->data.SectionNumber <= number2; break;
			case 6: res = pTemp->data.SectionNumber < number1 || pTemp->data.SectionNumber > number2; break;
			default: break;
			}
			if (res)
				notesForPrint++;
			else
				pTemp->Visible = false;
		}
		pTemp = pTemp->pNext;
	}
}
void SearchCheckNumber(int paramSearch, int x, int y)
{
	int number, number1, number2;
	bool res;
	SetCursorVisibility(true);
	if (paramSearch == 5 || paramSearch == 6)
	{
		GotoXY(x + 5, y + 9);
		printf("���祭�� 1: ");
		GotoXY(x + 5, y + 11);
		printf("���祭�� 2: ");
		GotoXY(x + 2, y + 10);
		if (ReadIntFromScreen(&number1, 9) == -1)
			return;
		GotoXY(x + 2, y + 12);
		if (ReadIntFromScreen(&number2, 9) == -1)
			return;
		if (number1 > number2)
		{
			number = number1;
			number1 = number2;
			number2 = number;
		}
	}
	else
	{
		GotoXY(x + 5, y + 9);
		printf("���祭��: ");
		GotoXY(x + 2, y + 10);
		if (ReadIntFromScreen(&number, 9) == -1)
			return;
	}
	SetCursorVisibility(false);
	pTemp = pHead;
	notesForPrint = 0;
	while (pTemp != NULL)
	{
		if (pTemp->Visible)
		{
			switch (paramSearch)
			{
			case 0:	res = pTemp->data.CheckNumber == number; break;
			case 1:	res = pTemp->data.CheckNumber < number; break;
			case 2:	res = pTemp->data.CheckNumber <= number; break;
			case 3:	res = pTemp->data.CheckNumber >= number; break;
			case 4:	res = pTemp->data.CheckNumber > number; break;
			case 5: res = pTemp->data.CheckNumber >= number1 && pTemp->data.CheckNumber <= number2; break;
			case 6: res = pTemp->data.CheckNumber < number1 || pTemp->data.CheckNumber > number2; break;
			default: break;
			}
			if (res)
				notesForPrint++;
			else
				pTemp->Visible = false;
		}
		pTemp = pTemp->pNext;
	}
}
void SearchProductName(int paramSearch, int x, int y)
{
	char str[31];
	bool res;
	SetCursorVisibility(true);
	GotoXY(x + 5, y + 9);
	printf("���祭��: ");
	GotoXY(x + 2, y + 10);
	if (ReadStringFromScreenWithoutRestrictions(str, 30) == -1)
		return;
	SetCursorVisibility(false);
	pTemp = pHead;
	notesForPrint = 0;
	while (pTemp != NULL)
	{
		switch (paramSearch)
		{
		case 0:	res = strcmp(pTemp->data.ProductName, str) == 0; break;
		case 7: res = strstr(pTemp->data.ProductName, str) != 0; break;
		default: break;
		}
		if (res)
			notesForPrint++;
		else
			pTemp->Visible = false;
		pTemp = pTemp->pNext;
	}
}
void SearchProductArticle(int paramSearch, int x, int y)
{
	char str[13];
	bool res;
	SetCursorVisibility(true);
	GotoXY(x + 5, y + 9);
	printf("���祭��: ");
	GotoXY(x + 2, y + 10);
	if (ReadArticleFromScreen(str, 12) == -1)
		return;
	SetCursorVisibility(false);
	pTemp = pHead;
	notesForPrint = 0;
	while (pTemp != NULL)
	{
		switch (paramSearch)
		{
		case 0:	res = strcmp(pTemp->data.ProductArticle, str) == 0; break;
		case 7: res = strstr(pTemp->data.ProductArticle, str) != 0; break;
		default: break;
		}
		if (res)
			notesForPrint++;
		else
			pTemp->Visible = false;
		pTemp = pTemp->pNext;
	}
}
void SearchPrice(int paramSearch, int x, int y)
{
	float number, number1, number2;
	bool res;
	SetCursorVisibility(true);
	if (paramSearch == 5 || paramSearch == 6)
	{
		GotoXY(x + 5, y + 9);
		printf("���祭�� 1: ");
		GotoXY(x + 5, y + 11);
		printf("���祭�� 2: ");
		GotoXY(x + 2, y + 10);
		if (ReadFloatFromScreen(&number1, 5, 2) == -1)
			return;
		GotoXY(x + 2, y + 12);
		if (ReadFloatFromScreen(&number2, 5, 2) == -1)
			return;
		if (number1 > number2)
		{
			number = number1;
			number1 = number2;
			number2 = number;
		}
	}
	else
	{
		GotoXY(x + 5, y + 9);
		printf("���祭��: ");
		GotoXY(x + 2, y + 10);
		if (ReadFloatFromScreen(&number, 5, 2) == -1)
			return;
	}
	SetCursorVisibility(false);
	pTemp = pHead;
	notesForPrint = 0;
	while (pTemp != NULL)
	{
		if (pTemp->Visible)
		{
			switch (paramSearch)
			{
			case 0:	res = pTemp->data.Price == number; break;
			case 1:	res = pTemp->data.Price < number; break;
			case 2:	res = pTemp->data.Price <= number; break;
			case 3:	res = pTemp->data.Price >= number; break;
			case 4:	res = pTemp->data.Price > number; break;
			case 5: res = pTemp->data.Price >= number1 && pTemp->data.Price <= number2; break;
			case 6: res = pTemp->data.Price < number1 || pTemp->data.Price > number2; break;
			default: break;
			}
			if (res)
				notesForPrint++;
			else
				pTemp->Visible = false;
		}
		pTemp = pTemp->pNext;
	}
}
void SearchProductAmount(int paramSearch, int x, int y)
{
	int number, number1, number2;
	bool res;
	SetCursorVisibility(true);
	if (paramSearch == 5 || paramSearch == 6)
	{
		GotoXY(x + 5, y + 9);
		printf("���祭�� 1: ");
		GotoXY(x + 5, y + 11);
		printf("���祭�� 2: ");
		GotoXY(x + 2, y + 10);
		if (ReadIntFromScreen(&number1, 4) == -1)
			return;
		GotoXY(x + 2, y + 12);
		if (ReadIntFromScreen(&number2, 4) == -1)
			return;
		if (number1 > number2)
		{
			number = number1;
			number1 = number2;
			number2 = number;
		}
	}
	else
	{
		GotoXY(x + 5, y + 9);
		printf("���祭��: ");
		GotoXY(x + 2, y + 10);
		if (ReadIntFromScreen(&number, 4) == -1)
			return;
	}
	SetCursorVisibility(false);
	pTemp = pHead;
	notesForPrint = 0;
	while (pTemp != NULL)
	{
		if (pTemp->Visible)
		{
			switch (paramSearch)
			{
			case 0:	res = pTemp->data.ProductAmount == number; break;
			case 1:	res = pTemp->data.ProductAmount < number; break;
			case 2:	res = pTemp->data.ProductAmount <= number; break;
			case 3:	res = pTemp->data.ProductAmount >= number; break;
			case 4:	res = pTemp->data.ProductAmount > number; break;
			case 5: res = pTemp->data.ProductAmount >= number1 && pTemp->data.ProductAmount <= number2; break;
			case 6: res = pTemp->data.ProductAmount < number1 || pTemp->data.ProductAmount > number2; break;
			default: break;
			}
			if (res)
				notesForPrint++;
			else
				pTemp->Visible = false;
		}
		pTemp = pTemp->pNext;
	}
}
void SearchDateOfSale(int paramSearch, int x, int y)
{
	char date[11], date1[11], date2[11];
	bool res;
	SetCursorVisibility(true);
	if (paramSearch == 5 || paramSearch == 6)
	{
		GotoXY(x + 5, y + 9);
		printf("���祭�� 1: ");
		GotoXY(x + 5, y + 11);
		printf("���祭�� 2: ");
		GotoXY(x + 2, y + 10);
		if (ReadDateFromScreen(date1) == -1)
			return;
		GotoXY(x + 2, y + 12);
		if (ReadDateFromScreen(date2) == -1)
			return;
		if (CompareDate(date1, date2) == -1)
		{
			strcpy(date, date1);
			strcpy(date1, date2);
			strcpy(date2, date);
		}
	}
	else
	{
		GotoXY(x + 5, y + 9);
		printf("���祭��: ");
		GotoXY(x + 2, y + 10);
		if (ReadDateFromScreen(date) == -1)
			return;
	}
	SetCursorVisibility(false);
	pTemp = pHead;
	notesForPrint = 0;
	while (pTemp != NULL)
	{
		if (pTemp->Visible)
		{
			switch (paramSearch)
			{
			case 0:	res = CompareDate(pTemp->data.DateOfSale, date) == 0; break;
			case 1:	res = CompareDate(pTemp->data.DateOfSale, date) > 0; break;
			case 2:	res = CompareDate(pTemp->data.DateOfSale, date) >= 0; break;
			case 3:	res = CompareDate(pTemp->data.DateOfSale, date) <= 0; break;
			case 4:	res = CompareDate(pTemp->data.DateOfSale, date) < 0; break;
			case 5: res = CompareDate(pTemp->data.DateOfSale, date1) <= 0 && CompareDate(pTemp->data.DateOfSale, date2) >= 0; break;
			case 6: res = CompareDate(pTemp->data.DateOfSale, date1) > 0 || CompareDate(pTemp->data.DateOfSale, date2) < 0; break;
			default: break;
			}
			if (res)
				notesForPrint++;
			else
				pTemp->Visible = false;
		}
		pTemp = pTemp->pNext;
	}
}
//�㭪�� ShowAll
//������� ��������� ��� ����⮢ ᯨ᪠ �� true
void ShowAll()
{
	pTemp = pHead;
	while (pTemp != NULL)
	{
		pTemp->Visible = true;
		pTemp = pTemp->pNext;
	}
	notesForPrint = notesAmount;
}

//�㭪�� ReadDataFromScreen
//�믮���� �⥭�� ������ � ����������
//�����頥� �������� ������
NodeInfo ReadDataFromScreen()
{
	NodeInfo data;
	data.key = 0;
	//system("cls");
	char c;
	SetCursorVisibility(true);
	int width = 50, height = 16, x = (80 - width) / 2, y = 4;
	int active = 0;
	PrintWindow(width, height, x, y);
	GotoXY(x + 1, y + 2);
	printf("    ������ ���ଠ�� � �த��� �த��:");
	GotoXY(x + 1, y + 4);
	printf("  ����� ��������: ");
	if (ReadIntFromScreen(&data.StoreNumber, 6) == 0)
	{
		GotoXY(x + 19, y + 4);
		printf("%06.6i", data.StoreNumber);
		GotoXY(x + 1, y + 5);
		printf("  ����� ᥪ樨: ");
		if (ReadIntFromScreen(&data.SectionNumber, 4) == 0)
		{
			GotoXY(x + 17, y + 5);
			printf("%04.4i", data.SectionNumber);
			GotoXY(x + 1, y + 6);
			printf("  ����� 祪�: ");
			if (ReadIntFromScreen(&data.CheckNumber, 9) == 0)
			{
				GotoXY(x + 15, y + 6);
				printf("%09.9i", data.CheckNumber);
				GotoXY(x + 1, y + 7);
				printf("  ������������ ⮢��: ");
				if (ReadStringFromScreenWithoutRestrictions(data.ProductName, 30) == 0)
				{
					GotoXY(x + 1, y + 8);
					printf("  ��⨪� ⮢��: ");
					if (ReadArticleFromScreen(data.ProductArticle, 12) == 0)
					{
						GotoXY(x + 1, y + 9);
						printf("  ����: ");
						if (ReadFloatFromScreen(&data.Price, 5, 2) == 0)
						{
							GotoXY(x + 1, y + 10);
							printf("  ������⢮: ");
							if (ReadIntFromScreen(&data.ProductAmount, 4) == 0)
							{
								GotoXY(x + 1, y + 11);
								printf("  ��� �த���: ");
								if (ReadDateFromScreen(data.DateOfSale) == 0)
								{
									SetCursorVisibility(false);
									while (true)
									{
										GotoXY(x + 5, y + 13);
										if (active == 0)
											SetColor(activeTextColor, activeBackgroundColor);
										else
											SetColor(textColor, backgroundColor);
										printf("  ��������  ");
										GotoXY(x + width - 18, y + 13);
										if (active == 1)
											SetColor(activeTextColor, activeBackgroundColor);
										else
											SetColor(textColor, backgroundColor);
										printf("��  ���������");
										c = _getch();
										if (c == 27)
										{
											data.key = -1;
											return data;
										}
										if (c == 13)
										{
											switch (active)
											{
											case 0: return data;
											case 1:
												data.key = -1;
												return data;
											default: break;
											}
										}
										if (c == -32)
										{
											c = _getch();
											switch (c)
											{
											case 75:  active = (active + 1) % 2; break;
											case 77:  active = (active + 1) % 2; break;
											default: break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	SetCursorVisibility(false);
	data.key = -1;
	return data;
}
//�㭪樨 Read*FromScreen
//�믮����� �⥭�� ࠧ����� ����� ������ � ���������� � ����஫�� �����
int ReadIntFromScreen(int* number, int digits)
{
	int i;
	char c, *str = new char[digits];
	for (i = 0; i < digits; i++)
		str[i] = ' ';
	i = 0;
	while (true)
	{
		c = _getch();
		if ((c == 13) && i > 0)
		{
			if (atoi(str) != 0)
				break;
			continue;
		}
		if (c == 27) return -1;
		if (c == 8 && i > 0)
		{
			_putch(c); _putch(' '); _putch(c);
			str[--i] = ' ';
			continue;
		}
		if (c >= '0' && c <= '9' && (i < digits))
		{
			str[i++] = c; _putch(c); continue;
		}
	}
	*number = atoi(str);
	return 0;
}
int ReadFloatFromScreen(float* number, int digitsBeforeComma, int digitsAfterComma)
{
	int i, j;
	char c, *str = new char[digitsBeforeComma + digitsAfterComma + 1];
	bool comma = false;
	for (i = 0; i < digitsBeforeComma + digitsAfterComma; i++)
		str[i] = ' ';
	i = 0; j = 0;
	while (true)
	{
		c = _getch();
		if (c == 13 && i > 0)
		{
			if (atof(str) != 0)
				break;
			continue;
		}
		if (c == 27) return -1;
		if (c == 8 && i > 0)
		{
			if (str[i - 1] == '.')
			{
				comma = false; j = 0;
			}
			_putch(c); _putch(' '); _putch(c);
			str[--i] = ' ';
			continue;
		}
		if ((c == '.' || c == ',') && !comma)
		{
			str[i++] = '.'; _putch(c);
			j = i; comma = true;
			continue;
		}
		if (i >= digitsBeforeComma && !comma) continue;
		if (((i - j) >= digitsAfterComma) && comma) continue;
		if (c >= '0' && c <= '9')
		{
			str[i++] = c; _putch(c); continue;
		}
	}
	*number = atof(str);
	return 0;
}
int ReadStringFromScreenWithoutRestrictions(char* str, int symbols)
{
	int i;
	char c;
	for (i = 0; i < symbols; i++)
		str[i] = ' ';
	i = 0;
	while (true)
	{
		c = _getch();
		if (c == 13 && i > 0) break;
		if (c == 27) return -1;
		if (c == 8)
		{
			if (i <= 0) continue;
			_putch(c); _putch(' '); _putch(c);
			str[--i] = ' ';
			continue;
		}
		if (i < symbols)
		{
			str[i++] = c; printf("%c", c); continue;
		}
	}
	str[i] = '\0';
	return 0;
}
int ReadFileNameFromScreen(char* str, int symbols)
{
	int i;
	char c;
	for (i = 0; i < symbols; i++)
		str[i] = ' ';
	i = 0;
	while (true)
	{
		c = _getch();
		if (c == 13 && i > 0) break;
		if (c == 27) return -1;
		if (c == 8)
		{
			if (i <= 0) continue;
			_putch(c); _putch(' '); _putch(c);
			str[--i] = ' ';
			continue;
		}
		if (i < symbols)
		{
			if ((c >= '�' && c <= '�') || (c >= '�' && c <= '�') || (c >= '�' && c <= '�'))
				continue;
			str[i++] = c; printf("%c", c); continue;
		}
	}
	str[i] = '\0';
	return 0;
}
int ReadArticleFromScreen(char* str, int symbols)
{
	int i;
	char c;
	for (i = 0; i < symbols; i++)
		str[i] = ' ';
	i = 0;
	while (true)
	{
		c = _getch();
		if (c == 13 && i > 0) break;
		if (c == 27) return -1;
		if (c == 8 && i > 0)
		{
			_putch(c); _putch(' '); _putch(c);
			str[--i] = ' ';
			continue;
		}
		if (i < symbols && ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '�' && c <= '�') || (c >= '�' && c <= '�') || (c >= '�' && c <= '�')))
		{
			str[i++] = c; printf("%c", c); continue;
		}
	}
	str[i] = '\0';
	return 0;
}
int ReadDateFromScreen(char* str)
{
	int i = 0;
	char c;
	bool date = false;
	for (; i < 10; i++)
		str[i] = ' ';
	str[10] = '\0';
	i = 0;
	while (true)
	{
		c = _getch();
		if (c == 13 && date) break;
		if (c == 27) return -1;
		if (c == 8 && i > 0)
		{
			if (i == 3 || i == 6)
			{
				_putch(c); _putch(' '); _putch(c);
				str[--i] = ' ';
			}
			_putch(c); _putch(' '); _putch(c);
			str[--i] = ' ';
			if (date)
				date = false;
			continue;
		}
		switch (i)
		{
		case 0:
			if (c >= '0' && c <= '3')
			{
				str[i++] = c; _putch(c);
			}
			break;
		case 1:
			if (str[0] == '3')
			{
				if (c < '0' || c > '1')
					continue;
			}
			else
			{
				if (c < '0' || c > '9')
					continue;
			}
			str[i++] = c; _putch(c);
			str[i++] = '.'; _putch('.');
			break;
		case 2: case 5:
			break;
		case 3:
			if (c >= '0' && c <= '1')
			{
				str[i++] = c; _putch(c);
			}
			break;
		case 4:
			if (str[0] == '3' && str[1] == '1')
			{
				if (str[3] == '0')
				{
					if (c != '1' && c != '3' && c != '5' && c != '7' && c != '8')
						continue;
				}
				else
				{
					if (c != '0' && c != '2')
						continue;
				}
			}
			else
			{
				if (str[0] == '3' && str[1] == '0')
				{
					if (str[3] == '0')
					{
						if (c < '0' || c > '9' || c == '2')
							continue;
					}
					else
					{
						if (c < '0' || c > '2')
							continue;
					}
				}
				else
				{
					if (str[3] == '0')
					{
						if (c < '0' || c > '9')
							continue;
					}
					else
					{
						if (c < '0' || c > '2')
							continue;
					}
				}
			}
			str[i++] = c; _putch(c);
			str[i++] = '.'; _putch('.');
			break;
		case 6: case 7: case 8:
			if (c >= '0' && c <= '9')
			{
				str[i++] = c; _putch(c);
			}
			break;
		case 9:
			if (str[0] == '2' && str[1] == '9' && str[3] == '0' && str[4] == '2')
			{
				if (c >= '0' && c <= '9')
				{
					char year[4];
					year[0] = str[6]; year[1] = str[7]; year[2] = str[8]; year[3] = c;
					if (atoi(year) % 4 == 0)
					{
						str[i++] = c; _putch(c);
						date = true;
					}
					continue;
				}
			}
			else
				if (c >= '0' && c <= '9')
				{
					str[i++] = c; _putch(c);
					date = true;
				}
			break;
		default: continue;
		}
		continue;
	}
	str[i] = '\0';
	return 0;
}

//�㭪�� Swap
//�ᯮ����⥫쭠� �㭪�� ��� ����⠭���� ���� ����⮢ ᯨ᪠ ���⠬�
//�ਭ����� ��뫪� �� ������ ᯨ᪠
void Swap(Node *pN1, Node *pN2)
{
	NodeInfo Temp;
	bool temp;
	temp = pN1->Visible;
	Temp = pN1->data;
	pN1->data = pN2->data;
	pN1->Visible = pN2->Visible;
	pN2->data = Temp;
	pN2->Visible = temp;
}
//�㭪�� CompareDate
//�믮���� �ࠢ����� ���
//�ਭ����� 2 ��ப�, ᮤ�ঠ�� ����
int CompareDate(char* date1, char* date2)
{
	int day1, day2, month1, month2, year1, year2;
	char str[4] = { ' ' };
	str[0] = date1[0]; str[1] = date1[1];
	day1 = atoi(str);
	str[0] = date2[0]; str[1] = date2[1];
	day2 = atoi(str);
	str[0] = date1[3]; str[1] = date1[4];
	month1 = atoi(str);
	str[0] = date2[3]; str[1] = date2[4];
	month2 = atoi(str);
	str[0] = date1[6]; str[1] = date1[7]; str[2] = date1[8]; str[3] = date1[9];
	year1 = atoi(str);
	str[0] = date2[6]; str[1] = date2[7]; str[2] = date2[8]; str[3] = date2[9];
	year2 = atoi(str);
	if (year1 < year2)
		return 1;
	if (year1 > year2)
		return -1;
	if (year1 == year2)
	{
		if (month1 < month2)
			return 1;
		if (month1 > month2)
			return -1;
		if (month1 == month2)
		{
			if (day1 < day2)
				return 1;
			if (day1 > day2)
				return -1;
			if (day1 == day2)
			{
				return 0;
			}
		}
	}
	return -2;
}
//�㭪�� ActiveOrNot
//�믮���� �ࠢ����� ����� ��⨢���� ����� � ����஬ ⥪�饣�,
//�᫨ ᮢ������ � 梥� ⥪�� � 䮭� ������� �� ��⨢��, ���� ������� �� �����
//�ਬ��� ����� ��⨢���� ����� � ⥪�饣�
void ActiveOrNot(int active, int current)
{
	if (active == current)
		SetColor(activeTextColor, activeBackgroundColor);
	else
		SetColor(textColor, backgroundColor);
}
//�㭪�� SetColor
//��⠭�������� 梥� ⥪�� � 䮭�
//�ਭ����� ��� 梥� ⥪�� � ��� 梥� 䮭�
void SetColor(int text, int background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
//�㭪�� GotoXY
//��६�頥� ����� � 㪠������ ������
//�ਭ����� �࠭�� ���न���� x � y
void GotoXY(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}
//�㭪�� SetCursorVisibility
//��⠭�������� �������� ��������� �����
//�ਭ����� ���祭�� true/false ��� �������� �����
void SetCursorVisibility(bool Visible)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hCon, &cci);
	cci.bVisible = Visible;
	SetConsoleCursorInfo(hCon, &cci);
}
