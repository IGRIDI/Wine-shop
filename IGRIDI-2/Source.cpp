#include "stdafx.h"
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <iostream>

#pragma warning(disable : 4996)

using namespace std;

//количество записей на странице
#define notesOnPage 16
//код цвета обычного текста
#define textColor 15
//код цвета обычного фона
#define backgroundColor 3
//код цвета текста активного пункта меню или списка
#define activeTextColor 0
//код цвета фона активного пункта меню или списка
#define activeBackgroundColor 15

//имя стандартного дата-файла
#define binFile "BIN.DAT"
//имя стандартного текстового файла
#define txtFile "Table.txt"

//Перечисление для вывода кнопок MessageBox'a
enum ButtonsMessageBox {
	OneButton, TwoButton, ThreeButton
};

//Структура данных записей
struct NodeInfo
{
	int key;					//ключ
	int StoreNumber;			//Номер магазина
	int SectionNumber;			//Номер секции
	int CheckNumber;			//Номер чека
	char ProductName[31];		//Наименование продукта
	char ProductArticle[13];	//Артикул
	float Price;				//Цена
	int ProductAmount;			//Количество
	char DateOfSale[11];		//Дата продажи
};
//Структура узла списка
struct Node
{
	NodeInfo data;			//данные
	bool Visible;			//видимость (используется для реализации поиска)
	Node* pNext;			//указатель на следующий элемент списка
	Node* pPrev;			//указатель на предыдущий элемент списка
}*pHead, *pTail, *pTemp;	//Указатель на голову списка(pHead), на хвост списка(pTail) и указатель для временного хранения ссылки (pTemp)

							//Cтруктура узла списка обработанных данных
struct ProcessNode
{
	int StoreNumber;				//Номер магазина
	double Profit;					//Выручка
	ProcessNode *pNext, *pPrev;		//Указатели на следующий и предыдыущий элементы списка
};

//Количество записей в списке
int notesAmount = 0;
//Количество записей выводимых на экран
int notesForPrint = 0;

int PrintMainMenu();
int PrintProcessMenu();
void PrintAll();
bool PrintSortMenu();
void PrintDataOnScreen(NodeInfo* data);
int InitializePrint(int currentPage, int* pageAmount, Node** pBegin);
void PrintWindow(int width, int height, int x = -1, int y = -1);
int PrintSimpleMenu(char** menuItemsText, int menuItems, int width = -1, int height = -1, bool window = true, int x = -1, int y = -1);
int PrintMessageBox(ButtonsMessageBox btnMB, char* text, int width = -1, int height = -1, char* textFirstBtn = "ОК", char* textSecondBtn = "Отмена", char* textThirdBtn = "Выход");


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
	//Установка таблицы кодировки
	SetConsoleCP(866); SetConsoleOutputCP(866);
	//Обнуление указателей списка
	pTail = pHead = NULL;
	//Отключение курсора
	SetCursorVisibility(false);
	//Загрузка списка из файла
	ReadFromBinaryFile("\0");
	//Вывод списка
	PrintAll();
}

//Функция PrintMainMenu
//Выполняет прорисовку главного меню
//Возвращает номер вызванного пункта меню
int PrintMainMenu()
{
	//Количество элементов меню
	int menuItems = 10;
	//Массив пунктов меню
	//Размер массива больше количества элементов на один
	//т.к. 0-ой элемент массива - название меню
	char **menuText = new char*[menuItems + 1];
	menuText[0] = "";
	menuText[1] = "Закрыть ";
	menuText[2] = "Показать весь список";
	menuText[3] = "Добавить данные ";
	menuText[4] = "Поиск ";
	menuText[5] = "Сортировать ";
	menuText[6] = "Сохранить данные в файл ";
	menuText[7] = "Загрузить данные из файла ";
	menuText[8] = "Обработка ";
	menuText[9] = "Удалить все данные";
	menuText[10] = "Выйти ";
	//Вызов функции отрисовки меню и обработки нажимаемыхх клавиш
	return PrintSimpleMenu(menuText, menuItems, 34);
}
//Функция PrintProcessMenu
//Выполняет прорисовку меню обработки (меню при прорисовке таблицы обработки)
//Возвращает номер вызванного пункта меню
int PrintProcessMenu()
{
	//Количество элементов меню
	int menuItems = 3;
	//Массив пунктов меню
	//Размер массива больше количества элементов на один
	//т.к. 0-ой элемент массива - название меню
	char **menuText = new char*[menuItems + 1];
	menuText[0] = "";
	menuText[1] = "Закрыть ";
	menuText[2] = "Сохранить результаты в файл ";
	menuText[3] = "Показать список ";
	//Вызов функции отрисовки меню и обработки нажимаемыхх клавиш
	return PrintSimpleMenu(menuText, menuItems, 34);
}
//Функция PrintAll
//Осуществляет прорисовку таблицы данных
//и обработку нажимаемых клавиш
void PrintAll()
{
	//с - переменная для обработки нажатых клавиш, path - для сохранения вводимых путей к файлам
	char c, path[61];
	//page - номер текущей страницы, i - счетчик выведенных элементов списка, pageAmount - общее количества страниц
	int page = 1, i = 0, pageAmount = (notesAmount + notesOnPage - 1) / notesOnPage;
	//activeNote - номер активного элемента на странице, notesOnCurrentPage - количество записей на текущей странице
	//notesForPrint - количество записей для печати
	int activeNote = 0, notesOnCurrentPage = 0; notesForPrint = notesAmount;
	//pBegin - указатель на первый элемент выводимой страницы, pPrint - указатель для печати страницы, pActive - указатель на активный элемент
	Node *pBegin = pHead, *pPrint, *pActive = NULL;
	//цикл вывода и обработки
	int active = 1;
	while (true)
	{
		//Установка цвета текста и фона
		SetColor(textColor, backgroundColor);
		//l = active - (active % notesOnPage ? active % notesOnPage : notesOnPage) + 1;
		// = min(notesAmount, l + notesOnPage - 1);
		//Инициализация значенией указателей и переменных для верной отрисовки списка
		notesOnCurrentPage = InitializePrint(page, &pageAmount, &pBegin);
		pPrint = pBegin;
		i = 0;
		//Очистка экрана
		system("cls");
		//Печать шапки таблицы
		printf("                          Esc - для выхода в меню\n");
		printf("\n     Страница №%-3i   Запись №%04i           Страниц: %-3i    Записей: %-4i \n", page, 0, pageAmount, notesForPrint);
		printf("   ┌────────┬────────────────────────────────┬──────────┬──────┬────────────┐\n");
		printf("   │ Номер  │          Наименование          │   Цена   │Кол-во│    Дата    │\n");
		printf("   │магазина│             товара             │          │      │   продажи  │\n");
		printf("   ├────────┼────────────────────────────────┼──────────┼──────┼────────────┤\n");
		//Пока счетчик меньше требуемого количества элементов на экране выводим данные
		while (i < notesOnPage)
		{
			//Находим элемент для вывода на экран
			while (pPrint && !pPrint->Visible) {
				pPrint = pPrint->pNext;
			}
			//Если указатель не нулевой выводим данные, предварительно проверив является активным выводимый элемент или нет
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
				printf("   │ %-06.6i │ %-30.30s │ %8.2f │ %4i │ %10.10s │\n", pPrint->data.StoreNumber, pPrint->data.ProductName, pPrint->data.Price, pPrint->data.ProductAmount, pPrint->data.DateOfSale);
				pPrint = pPrint->pNext;
				if (activeNote == i) {
					SetColor(textColor, backgroundColor);
				}
			}
			//иначе выводим пустую строку таблицы
			else
				printf("   │ %6s │ %30s │ %8s │ %4s │ %10s │\n", "", "", "", "", "");
			//инкрементируем счетчик
			i++;
		}
		//печать окончания таблицы
		printf("   └────────┴────────────────────────────────┴──────────┴──────┴────────────┘\n");
		//записываем в c код символа нажатой клавиши
		c = _getch();
		switch (c)
		{ 
		case -32:
			c = _getch();
			//Нажатие стрелок - управление активным элементом и выводимыми страницами
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
				//нажатие кнопки F10 - Выход из программы
			case 'D': SetColor(textColor, backgroundColor);
				switch (PrintMessageBox(TwoButton, "Вы уверены, что хотите выйти?", -1, -1, "Да", "Нет"))
				{
				case 0: ClearList(); return;
				case 1: continue;
				default: break;
				}
				break;
			default: break;
			}
			break;
			//Нажатие кнопки Enter - Вывод данных активного элемента списка
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
			//Нажатие кнопки Esc - Вывод главного меню
		case 27:
			//Выбор возращаемого значения - кода выбранного пункта меню
			switch (PrintMainMenu())
			{
			case 0: break;
				//Показать весь список данных
			case 1: ShowAll(); break;
				//Добавление данных - ввод с клавиатуры
			case 2: AddNode(ReadDataFromScreen()); break;
				//Поиск
			case 3: Search(); activeNote = 0; page = 1; break;
				//Сортировка
			case 4:
				if (PrintSortMenu())
				{
					page = 1;
					activeNote = 0;
				}
				break;
				//Сохранение данных в файл
			case 5:
				//Вывод окошка с выбором типа файла
				switch (PrintMessageBox(ThreeButton, "Выберите тип файла для сохранения", 50, -1, "Дата-файл", "Текстовый файл"))
				{
					//Бинарный файл
				case 0:
					//чтение пути к файлу
					if (ReadFileName(path) == -1)
						continue;
					//если данные сохранены выводится окошко об успехе
					if (WriteIntoBinaryFile(path))
						PrintMessageBox(OneButton, "Данные успешно сохранены!");
					break;
					//Текстовый файл
				case 1:
					if (ReadFileName(path) == -1)
						continue;
					//если данные сохранены выводится окошко об успехе
					if (WriteIntoTextFile(path))
						PrintMessageBox(OneButton, "Данные успешно сохранены!");
					break;
					//Выход
				default: continue;
				}
				break;
				//Чтение данных из файла
			case 6:
				//Вывод окошка с выбором типа файла
				switch (PrintMessageBox(ThreeButton, "Выберите тип файла для чтения", 50, -1, "Дата-файл", "Текстовый файл"))
				{
					//Бинарный файл
				case 0:
					//чтение пути к файлу
					if (ReadFileName(path) == -1)
						continue;
					//вывод окна с вопросом об очистке списка
					if (pHead != NULL)
						switch (PrintMessageBox(ThreeButton, "Очистить список?", -1, -1, "Да", "Нет", "Выйти"))
						{
						case 0: ClearList();
						case 1: break;
						case 2: continue;
						default: break;
						}
					//если данные считаны выводится окошко об успехе
					if (ReadFromBinaryFile(path))
						PrintMessageBox(OneButton, "Данные успешно считаны!");
					break;
					//Бинарный файл
				case 1:
					//чтение пути к файлу
					if (ReadFileName(path) == -1)
						continue;
					//вывод окна с вопросом об очистке списка
					if (pHead != NULL)
						switch (PrintMessageBox(ThreeButton, "Очистить список?", -1, -1, "Да", "Нет", "Выйти"))
						{
						case 0: ClearList();
						case 1: break;
						case 2: continue;
						default: break;
						}
					//если данные считаны выводится окошко об успехе
					if (ReadFromTextFile(path))
						PrintMessageBox(OneButton, "Данные успешно считаны!");
					break;
				default: continue;
				}
				//Инициализация переменных списка после чтения
				pActive = NULL; activeNote = 0; page = 1;
				break;
				//Обработка данных
			case 7: ProcessList(); break;
				//Очистка списка
			case 8:
				//Окно подтверждения
				switch (PrintMessageBox(TwoButton, "Вы уверены, что хотите очистить список?", 50, -1, "Да", "Нет"))
				{
				case 0: ClearList();
				case 1: break;
				case 2: continue;
				default: break;
				}
				pActive = NULL; activeNote = 0; page = 1; break;
				//Выход из программы
			case 9:
				SetColor(textColor, backgroundColor);
				//Окно подтверждения
				switch (PrintMessageBox(TwoButton, "Вы уверены, что хотите выйти?", -1, -1, "Да", "Нет"))
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
//Функция PrintSortMenu
//Выводит меню сортировки и реализует сортировку данных списка по заданному полю
bool PrintSortMenu()
{
	int menuItems = 7;
	char** menuText = new char*[menuItems + 1];
	menuText[0] = "Выберите поле для сортировки";
	menuText[1] = "Номер записи";
	menuText[2] = "Номер магазина";
	menuText[3] = "Наименование";
	menuText[4] = "Цена";
	menuText[5] = "Количество";
	menuText[6] = "Дата продажи";
	menuText[7] = " Вернуться назад";
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
//Функция PrintDataOnScreen
//Выводит данные конкретного элемента списка и реализует возможность изменения и удаления элемента
void PrintDataOnScreen(NodeInfo* data)
{
	char c;
	//Параметры выводимого окна
	int width = 50, height = 16, x = (80 - width) / 2, y = 4;
	//Активный пункт
	int active = 2;
	//Режим редактирования
	bool edit = false;
	//Переменная для временного хранения измененных данных
	NodeInfo temp = *data;
	//Вывод рамки окна
	PrintWindow(width, height);
	//Вывод шапки
	GotoXY(x + 1, y + 4);  printf("  Номер магазина: ");
	GotoXY(x + 1, y + 5);  printf("  Номер секции: ");
	GotoXY(x + 1, y + 6);  printf("  Номер чека: ");
	GotoXY(x + 1, y + 7);  printf("  Наименование: ");
	GotoXY(x + 1, y + 8);  printf("  Артикул товара: ");
	GotoXY(x + 1, y + 9);  printf("  Цена: ");
	GotoXY(x + 1, y + 10); printf("  Количество: ");
	GotoXY(x + 1, y + 11); printf("  Дата продажи: ");
	//Вывод данных и обработка нажатых клавиш и выбранных пунктов меню
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
		//Вывод кнопок работы с элементом
		if (!edit)
		{
			GotoXY(x + 18, y + 2);  printf("Запись №%04i", temp.key);
			GotoXY(x + 5, y + 13);
			ActiveOrNot(active, 0);
			printf("Удалить");
			GotoXY(x + width / 2 - 4, y + 13);
			ActiveOrNot(active, 1);
			printf("Изменить");
			GotoXY(x + width - 13, y + 13);
			ActiveOrNot(active, 2);
			printf("Закрыть");
		}
		//Вывод кнопок при редактировании элемента
		else
		{
			GotoXY(x + 12, y + 2);  printf("Редактирование записи №%04i", temp.key);
			GotoXY(x + 5, y + 13);
			ActiveOrNot(active, 5);
			printf("  Сохранить  ");
			GotoXY(x + width - 18, y + 13);
			ActiveOrNot(active, 6);
			printf("Не  сохранять");
		}
		SetColor(textColor, backgroundColor);
		c = _getch();
		//Esc - или выход из режима редактирования или выход ил просмотра данных
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
		//Выбор элемента меню
		if (c == 13)
		{
			switch (active)
			{
				//Удаление данных
			case 0:
				//юсЁрсюЄър эр т√ыхЄ яЁш єфрыхэшш яюёыхфэхую ¤ыхьхэЄр ╩╬╤╥██████╦▄▄▄▄▄▄▄▄
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
				//Режим редактирования
			case 1:
				edit = true;
				temp = *data;
				active = 6;
				break;
				//Выход из просмотра
			case 2: return;
				//Выход из режима редактирования с сохранением отредактированных данных
			case 5: *data = temp; edit = false; active = 2; break;
				//Выход из режима редактирования без сохранения отредактированных данных
			case 6: edit = false; active = 2; temp = *data;  break;
				//11-18 - редактирование различных полей записи
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
		//Обработка нажатия стрелок
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
//Функция InitializePrint
//Реализует инициализацию переменных, отвечающих за прорисовку списка
//Принимает в качестве параметров номер текущей страницы, указатель на переменную с количеством страниц и указатель на указатель на головной элемент страницы\
//Возвращает количество записей на текущей странице
int InitializePrint(int currentPage, int* pageAmount, Node **pBegin)
{
	//Расчет общего количества страниц
	*pageAmount = (notesForPrint + notesOnPage - 1) / notesOnPage;
	*pBegin = pHead;
	//Расчет указателя на головной элемень страницы
	for (int i = 0; i < (currentPage - 1) * notesOnPage;)
	{
		*pBegin = (*pBegin)->pNext;
		if ((*pBegin)->Visible)
			i++;
	}
	//подсчет элементов на текущей странице
	//cout << notesForPrint << " " << notesOnPage << endl;
	if (notesForPrint - (notesOnPage* (currentPage - 1)) > notesOnPage)
		return notesOnPage;
	else
		return notesForPrint - (notesOnPage * (currentPage - 1));
}
//Функция PrintWindow
//Реализует вывод на экране оконной рамки заданного размера и в заданных координатах
//Принимает в качестве параметров ширину окна, высоту и координаты x и y на экране
void PrintWindow(int width, int height, int x, int y)
{
	//если не заданы координаты, расчет их исходя из ширины и высоты окна
	if (x == -1)
		x = (80 - width) / 2;
	if (y == -1)
		y = (25 - height) / 2;
	for (int i = 0; i < height; i++)
	{
		//Вывод верхней рамки окна
		GotoXY(x, i + y);
		if (i == 0)
		{
			printf("╔");
			for (int j = 0; j < width - 2; j++)
				printf("═");
			printf("╗");
		}
		else
		{
			//Вывод нижней рамки окна
			if (i == height - 1)
			{
				printf("╚");
				for (int j = 0; j < width - 2; j++)
					printf("═");
				printf("╝");
			}
			else
				//Вывод боковых рамок окна
				printf("║% *.*s║", width - 2, width - 2, " ");
		}
	}
}
//Функция PrintSimpleMenu
//Реализует вывод меню в виде окна на экран и обработку нажатия клавиш
//Принимает следующие параметры: menuItemsText - заголовки пунктов меню и 0-ой элемент - заголовок меню
//menuItems - количество элементов меню, width - ширина меню, height - высота меню , window - требуется ли рамка, x и y - координаты левого верхнего угла меню
//Вовзращает номер выбранного пункта меню (счет от 0)
int PrintSimpleMenu(char** menuItemsText, int menuItems, int width, int height, bool window, int x, int y)
{
	//Определение и инициализация параметров вывода
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
	//Если есть заголовок, то вывести
	if (menuItemsText[0] != "")
	{
		GotoXY(x + (width - strlen(menuItemsText[0])) / 2, y + 2);
		printf("%s", menuItemsText[0]);
	}
	//Вывод менб и обработка клавиш
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
//Функция PrintMessageBox
//Выводит на экран окошко заданного размера и заданным текстом и кнопками
//btnMB - количество кнопок (1 - 3), text - текст сообщения, width и height - ширина и высота окна соответственно
//остальные параметры текст выводимый на кнопках
//Возвращает номер нажатой кнопки (счет от 0)
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

//Функция ReadFileName
//Отрисоыывает окно ввода пути к файлу
//Принимает ссылку на массив char в который записывает считанный путь
//Возвращает число, символизирущее успешность, отмену  или сбой операции
int ReadFileName(char* path)
{
	int width = 70, height = 6;
	PrintWindow(width, height);
	GotoXY(8, 11);
	printf("Введите путь к файлу: (Например: E:\\BIN.dat или TEXT.txt)");
	GotoXY(11, 12);
	SetCursorVisibility(true);
	height = ReadFileNameFromScreen(path, 60);
	SetCursorVisibility(false);
	return height;
}
//Функция WriteIntoBinaryFile
//Выполняет запись данных в бинарный файл
//Принимает строку - путь к файлу
//Возвращает булевое значение, символизирущее успешность, отмену  или сбой операции
bool WriteIntoBinaryFile(char* path)
{
	//файловая переменная
	FILE *f;
	//Проверка на использование стандартного файла
	if (strcmp(path, "\0") == 0)
		f = fopen(binFile, "wb");
	else
		f = fopen(path, "wb");
	//Проверка открытия файла
	if (!f)
	{
		PrintMessageBox(OneButton, "Ошибка открытия файла");
		return false;
	}
	//Установка указателя в файле
	fseek(f, 0, SEEK_SET);
	//Определение размера структуры данных
	int sizeNote = sizeof(NodeInfo);
	//Запись данных в файлы
	pTemp = pHead;
	while (pTemp != NULL)
	{
		fwrite(&pTemp->data, sizeNote, 1, f);
		pTemp = pTemp->pNext;
	}
	//Закрытие файла
	fclose(f);
	return true;
}
//Функция ReadFromBinaryFile
//Выполняет чтение данных из бинарного файла
//Принимает строку - путь к файлу
//Возвращает булевое значение, символизирущее успешность, отмену  или сбой операции
bool ReadFromBinaryFile(char* path)
{
	//файловая переменная
	FILE *f;
	//структура данных
	NodeInfo data;
	//проверка использования стандартного файла
	if (strcmp(path, "\0") == 0)
		f = fopen(binFile, "rb");
	else
		f = fopen(path, "rb");
	//проверка успешности открытия файла
	if (!f)
	{
		PrintMessageBox(OneButton, "Ошибка открытия файла");
		return false;
	}
	//установление указателя на начала файла
	rewind(f);
	//определение размера структуры данных
	int sizeNote = sizeof(NodeInfo);
	//пока не конец файла чтение данных и добавление в список
	while (!feof(f))
	{
		if (fread(&data, sizeNote, 1, f) < 1)
			break;
		AddNode(data);
	}
	//закрытие файла
	fclose(f);
	return true;
}
//Функция WriteIntoTextFile
//Выполняет запись данных в текстовый файл
//Принимает строку - путь к файлу
//Возвращает булевое значение, символизирущее успешность, отмену  или сбой операции
bool WriteIntoTextFile(char* path)
{
	//файловая переменная
	FILE *f;
	//проверка использования стандартных файлов
	if (strcmp(path, "\0") == 0)
		f = fopen(txtFile, "w");
	else
		f = fopen(path, "w");
	//проверка успешности открытия файоа
	if (!f)
	{
		PrintMessageBox(OneButton, "Ошибка открытия файла");
		return false;
	}
	pTemp = pHead;
	//печать шапки таблицы
	fprintf(f, "┌────────┬──────┬────────────┬────────────────────────────────┬──────────────┬──────────┬──────┬────────────┐\n");
	fprintf(f, "│ Номер  │Номер │    Номер   │          Наименование          │   Артикул    │   Цена   │Кол-во│    Дата    │\n");
	fprintf(f, "│магазина│секции│    чека    │             товара             │    товара    │          │      │   продажи  │\n");
	fprintf(f, "├────────┼──────┼────────────┼────────────────────────────────┼──────────────┼──────────┼──────┼────────────┤\n");
	//пока не конец списка печатать данные в файл
	while (pTemp != NULL)
	{
		fprintf(f, "│ %-06.6i │ %-04.4i │  %-09.9i │ %-30.30s │ %-12.12s │ %8.2f │ %4i │ %10.10s │\n", pTemp->data.StoreNumber, pTemp->data.SectionNumber, pTemp->data.CheckNumber, pTemp->data.ProductName, pTemp->data.ProductArticle, pTemp->data.Price, pTemp->data.ProductAmount, pTemp->data.DateOfSale);
		pTemp = pTemp->pNext;
	}
	//печать окончания таблицы
	fprintf(f, "└────────┴──────┴────────────┴────────────────────────────────┴──────────────┴──────────┴──────┴────────────┘\n");
	//закрытие файла
	fclose(f);
	return true;
}
//Функция ReadFromTextFile
//Выполняет чтение данных из текстовогл файла
//Принимает строку - путь к файлу
//Возвращает булевое значение, символизирущее успешность, отмену  или сбой операции
bool ReadFromTextFile(char *path)
{
	//файловая переменная
	FILE *f;
	//структура данных
	NodeInfo data;
	//проверка на использование стандартных файлов
	if (strcmp(path, "\0") == 0)
		f = fopen(txtFile, "r");
	else
		f = fopen(path, "r");
	//проверка успешности открытия файла
	if (!f)
	{
		PrintMessageBox(OneButton, "Ошибка открытия файла");
		return false;
	}
	//переменные для хранения считываемого символа и обрабатываемого массива символов
	char c, tempStr[31];
	//флаг на наличие ошибочных данных в строке
	bool badStr = false;
	//переменные индексы, field - считываемого поля, k - индекс обрабатываемого массива символов
	int field = 0, k = 0;
	//пока не конец файла
	while (!feof(f))
	{
		//читать символ
		c = fgetc(f);
		//пропустить пробелы и символы таблицы
		while (c == '│' || c == '┬' || c == '┐' || c == '┌' || c == '─' || c == '└' || c == '┴' || c == '┘' || c == '┼' || c == '├' || c == '┤' || c == ' ' || c == 13 || c == 10)
			c = fgetc(f);
		//определить какое из полей считывается
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
				if (c == 9 || c == '│')
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
				if (c == 9 || c == '│')
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
				if (c == 9 || c == '│')
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
				if (c == 9 || c == '│')
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
				if (c == 9 || c == '│')
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
				if (k < 12 && ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= 'А' && c <= 'Я') || (c >= 'а' && c <= 'п') || (c >= 'р' && c <= 'я')))
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
				if (c == 9 || c == '│')
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
				if (c == 9 || c == '│')
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
				if (c == 9 || c == '│')
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
		//если строка с ошибками дочитать до конца строки, обнулить индексы
		if (badStr)
		{
			while (c != 10 && !feof(f))
				c = fgetc(f);
			badStr = false; field = 0; k = 0;
			continue;
		}
		//если данные считаны полностью добавить в список
		if (field == 8)
		{
			AddNode(data);
			field = 0;
		}
	}
	//закрыть файл
	fclose(f);
	return true;
}

//Функция AddNode
//Добавление данных в список
//Принимает добавляемые данные
void AddNode(NodeInfo elem)
{
	//Если ключ -1 то данные не добавлются
	if (elem.key == -1)
		return;
	//Если список пуст
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
	//иначе
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
	//элемент видим после добавления
	pTemp->Visible = true;
	pTemp = NULL;
	//инкремент количества выводимых записей и общего количества записей
	notesAmount++;
	notesForPrint++;
}
//Функция DeleteNode
//Удаляет элемент списка по ключу
//Принимает ключ в качестве параметра
void DeleteNode(int &key, int &active)
{
	if (pHead == NULL)
		return;
	Node * pDelete = pHead;
	//поиск удаляемого элемента
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
	//Изменение указателей
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
	//изменение ключей в списке
	while (pTemp != NULL)
		{
		if (pTemp->data.key > pDelete->data.key)
			pTemp->data.key -= 1;
		pTemp = pTemp->pNext;
	}
	delete(pDelete);
	notesAmount--;
	notesForPrint--;
	//освобождение памяти
	/* delete(pDelete);    //ъюёЄ√ы№ эр ёэюё фрээ√ї яю тшэфх
	notesAmount--;
	active--;
	key--;
	notesForPrint--;*/
}


//Функция DeleteNode
//Удаляет элемент списка по ссылке
//Принимает ключ в качестве параметра
/*void DeleteNode(Node * pDelete)
{
	if (pDelete == NULL)
		return;
	//Изменение указателей
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
	//изменение ключей в списке
	while (pTemp != NULL)
	{
		if (pTemp->data.key > pDelete->data.key)
			pTemp->data.key -= 1;
		pTemp = pTemp->pNext;
	}
	//осовбождение памяти
	delete(pDelete);
	notesAmount--;
	notesForPrint--;
}
//Функция ClearList
//Очистка всего списка
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
//Функция SortList
//Сортировка списка по заданному полю
//field - номер поля сортировки
void SortList(int field)
{
	//результат сравнения
	int res;
	if (pHead == NULL)
	{
		return;
	}
	//Пузырьковая сортировка
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
//Функция ProcessList
//Обработка данных списка
void ProcessList()
{
	//Указатели на список обработанных данных
	ProcessNode *pProcHead, *pProcTail, *pProcTemp;
	//общее количество записей и количество записей на странице
	int amount = 1, amountOnPage = 16;
	if (pHead == NULL)
	{
		PrintMessageBox(OneButton, "Список пуст!");
		return;
	}
	SortList(1);
	//Добавление головного элемента
	pProcHead = new ProcessNode;
	pProcHead->StoreNumber = pHead->data.StoreNumber;
	pProcHead->Profit = pHead->data.Price * pHead->data.ProductAmount;
	pProcHead->pNext = NULL;
	pProcTail = pProcHead;
	pTemp = pHead->pNext;
	//Обработка остальных данных
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
	//Вывод данных на экран
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
		printf("\n                          Страница №%-3i  из %-3i\n", page, pageAmount);
		printf("                     ┌────────┬──────────────────────┐\n");
		printf("                     │ Номер  │        Доход         │\n");
		printf("                     │магазина│       магазина       │\n");
		printf("                     ├────────┼──────────────────────┤\n");
		while (i < amountOnPage)
		{
			if (pPrint != NULL)
			{
				printf("                     │ %-06.6i │ %20.2f │\n", pPrint->StoreNumber, pPrint->Profit);
				pPrint = pPrint->pNext;
			}
			else
				printf("                     │ %6s │ %20s │\n", "", "");
			i++;
		}
		printf("                     └────────┴──────────────────────┘");
		c = _getch();
		//Обработка нажатия клавиш
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
					PrintMessageBox(OneButton, "Ошибка открытия файла");
					return;
				}
				pProcTemp = pProcHead;
				if (pProcTemp == NULL)
					break;
				fprintf(f, "┌────────┬──────────────────────┐\n");
				fprintf(f, "│ Номер  │        Доход         │\n");
				fprintf(f, "│магазина│       магазина       │\n");
				fprintf(f, "├────────┼──────────────────────┤\n");
				while (pProcTemp != NULL)
				{
					fprintf(f, "│ %-06.6i │ %20.2f │\n", pProcTemp->StoreNumber, pProcTemp->Profit);
					pProcTemp = pProcTemp->pNext;
				}
				fprintf(f, "└────────┴──────────────────────┘");
				fclose(f);
				break;
			case 2: SortList(0); return;
			default: break;
			}
		default: break;
		}
	}
}

//Функция Search
//Реализация поиска данных по списку
void Search()
{
	//Страница меню поиска, активный элемент меню, поле поиска и параметры поиска
	int page = 1, activeMenuItem = 0, menuItems = 2, fieldSearch = -1, paramSearch = -1;
	//массивы названий элементов меню, названий полей, названий параметров
	char c, **menuText, **fields = new char*[9], **params = new char*[9];
	fields[0] = "Номер записи"; fields[1] = "Номер магазина"; fields[2] = "Номер секции"; fields[3] = "Номер чека";
	fields[4] = "Наименование"; fields[5] = " Артикул";       fields[6] = "Цена";         fields[7] = "Количество"; fields[8] = "Дата продажи";
	params[0] = "Точное совпадение "; params[1] = "Меньше заданного";   params[2] = "Меньше или равно заданному"; params[3] = "Больше или равно заданному";
	params[4] = "Больше заданного";   params[5] = "Входит в диапазон "; params[6] = "Не входит в диапазон";       params[7] = "Наличие заданного ";
	SetColor(textColor, backgroundColor);
	//Вывод оконной рамки
	int width = 34, height = 16, x = (80 - width) / 2, y = (25 - height) / 2;
	PrintWindow(width, height);
	//Вывод меню
	while (true)
	{
		switch (page)
		{
		case 0:
			break;
			//Страница выбора поля поиска
		case 1:
			menuText = new char*[11];
			menuText[0] = "Выберите поле поиска";
			menuText[1] = "Номер записи";  menuText[2] = "Номер магазина"; menuText[3] = "Номер секции";  menuText[4] = "Номер чека";
			menuText[5] = "Наименование";  menuText[6] = " Артикул";	   menuText[7] = "Цена";		   menuText[8] = "Количество";
			menuText[9] = "Дата продажи";  menuText[10] = " Закрыть";
			fieldSearch = PrintSimpleMenu(menuText, 10, 34, 16);
			if (fieldSearch > 8 || fieldSearch < 0)
				return;
			page = 2;
			continue;
			//Страница выборп параметров поиска
		case 2:
			if (fieldSearch == 4 || fieldSearch == 5)
			{
				menuText = new char*[4];
				menuText[0] = "Выберите параметры поиска"; menuText[1] = "Точное совпадение";
				menuText[2] = "Наличие заданного";		   menuText[3] = "Назад";
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
				menuText[0] = "Выберите параметры поиска";  menuText[1] = "Точное совпадение ";
				menuText[2] = "Меньше заданного";			menuText[3] = "Меньше или равно заданному";
				menuText[4] = "Больше или равно заданному"; menuText[5] = "Больше заданного";
				menuText[6] = "Входит в диапазон ";			menuText[7] = "Не входит в диапазон";
				menuText[8] = "Назад ";
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
			//Страница ввода данных поиска и вызова требуемых функций для его реализации
		case 3:
			GotoXY(x + 4, y + 2);
			printf("Введите данные для поиска");
			for (int i = 0; i < height - 4; i++)
			{
				GotoXY(x + 1, y + 3 + i);
				printf("%*s", width - 2, " ");
			}
			GotoXY(x + 5, y + 4);
			printf("Поле поиска:");
			GotoXY(x + 1, y + 5);
			printf("%*s%s", (width - 2 - strlen(fields[fieldSearch])) / 2, " ", fields[fieldSearch]);
			GotoXY(x + 5, y + 6);
			printf("Параметр поиска:");
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
			//Если поиск не дал результатов
			if (pHead == NULL)
			{
				return;
			}
			if (notesForPrint == 0)
			{
				ShowAll();
				PrintMessageBox(OneButton, "Поиск результатов не дал!");
				page = 3;
				//continue;
				return;
			}
			return;
		default: break;
		}
	}
}
//Функции Search**
//Реализуют ввод данных с клавиатуры для заданном поля и поиск заданных данных
//Принимают параметр поиска и коордианаты окна ввода данных
void SearchKey(int paramSearch, int x, int y)
{
	int number = 0, number1 = 0, number2 = 0;
	bool res;
	SetCursorVisibility(true);
	if (paramSearch == 5 || paramSearch == 6)
	{
		GotoXY(x + 5, y + 9);
		printf("Значение 1: ");
		GotoXY(x + 5, y + 11);
		printf("Значение 2: ");
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
		printf("Значение: ");
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
		printf("Значение 1: ");
		GotoXY(x + 5, y + 11);
		printf("Значение 2: ");
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
		printf("Значение: ");
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
		printf("Значение 1: ");
		GotoXY(x + 5, y + 11);
		printf("Значение 2: ");
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
		printf("Значение: ");
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
		printf("Значение 1: ");
		GotoXY(x + 5, y + 11);
		printf("Значение 2: ");
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
		printf("Значение: ");
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
	printf("Значение: ");
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
	printf("Значение: ");
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
		printf("Значение 1: ");
		GotoXY(x + 5, y + 11);
		printf("Значение 2: ");
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
		printf("Значение: ");
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
		printf("Значение 1: ");
		GotoXY(x + 5, y + 11);
		printf("Значение 2: ");
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
		printf("Значение: ");
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
		printf("Значение 1: ");
		GotoXY(x + 5, y + 11);
		printf("Значение 2: ");
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
		printf("Значение: ");
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
//Функция ShowAll
//Изменяет видимость всех элементов списка на true
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

//Функция ReadDataFromScreen
//Выполняет чтение данных с клавиатуры
//Возвращает структуру данных
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
	printf("    Введите информацию о продаже продукта:");
	GotoXY(x + 1, y + 4);
	printf("  Номер магазина: ");
	if (ReadIntFromScreen(&data.StoreNumber, 6) == 0)
	{
		GotoXY(x + 19, y + 4);
		printf("%06.6i", data.StoreNumber);
		GotoXY(x + 1, y + 5);
		printf("  Номер секции: ");
		if (ReadIntFromScreen(&data.SectionNumber, 4) == 0)
		{
			GotoXY(x + 17, y + 5);
			printf("%04.4i", data.SectionNumber);
			GotoXY(x + 1, y + 6);
			printf("  Номер чека: ");
			if (ReadIntFromScreen(&data.CheckNumber, 9) == 0)
			{
				GotoXY(x + 15, y + 6);
				printf("%09.9i", data.CheckNumber);
				GotoXY(x + 1, y + 7);
				printf("  Наименование товара: ");
				if (ReadStringFromScreenWithoutRestrictions(data.ProductName, 30) == 0)
				{
					GotoXY(x + 1, y + 8);
					printf("  Артикул товара: ");
					if (ReadArticleFromScreen(data.ProductArticle, 12) == 0)
					{
						GotoXY(x + 1, y + 9);
						printf("  Цена: ");
						if (ReadFloatFromScreen(&data.Price, 5, 2) == 0)
						{
							GotoXY(x + 1, y + 10);
							printf("  Количество: ");
							if (ReadIntFromScreen(&data.ProductAmount, 4) == 0)
							{
								GotoXY(x + 1, y + 11);
								printf("  Дата продажи: ");
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
										printf("  Добавить  ");
										GotoXY(x + width - 18, y + 13);
										if (active == 1)
											SetColor(activeTextColor, activeBackgroundColor);
										else
											SetColor(textColor, backgroundColor);
										printf("Не  добавлять");
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
//Функции Read*FromScreen
//Выполняют чтение различных полей данных с клавиатуры с контролем ввода
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
			if ((c >= 'А' && c <= 'Я') || (c >= 'а' && c <= 'п') || (c >= 'р' && c <= 'я'))
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
		if (i < symbols && ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= 'А' && c <= 'Я') || (c >= 'а' && c <= 'п') || (c >= 'р' && c <= 'я')))
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

//Функция Swap
//Вспомогательная функция для перестановки двух элементов списка местами
//Принимает ссылки на элементы списка
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
//Функция CompareDate
//Выполняет сравнение дат
//Принимает 2 строки, содержащих даты
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
//Функция ActiveOrNot
//Выполняет сравнение номера активного элемента с номером текущего,
//если совпадают то цвет текста и фона меняется на активный, иначе меняется на обычный
//Примает номер активного элемента и текущего
void ActiveOrNot(int active, int current)
{
	if (active == current)
		SetColor(activeTextColor, activeBackgroundColor);
	else
		SetColor(textColor, backgroundColor);
}
//Функция SetColor
//Устанавливает цвет текста и фона
//Принимает код цвета текста и код цвета фона
void SetColor(int text, int background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
//Функция GotoXY
//Перемещает курсор в указанную позицию
//Принимает экранные координаты x и y
void GotoXY(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}
//Функция SetCursorVisibility
//Устанавливает заданную видимость курсора
//Принимает значение true/false для видимости курсора
void SetCursorVisibility(bool Visible)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hCon, &cci);
	cci.bVisible = Visible;
	SetConsoleCursorInfo(hCon, &cci);
}
