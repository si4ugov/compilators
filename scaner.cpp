#include "scaner.h"

Scaner::Scaner()
{
	keeper_last_char = '\0';

	in_main.open("main.txt", ios::in);

	if (!in_main.is_open())
	{
		cout << "Error opening file main.txt" << endl;
		system("pause");
		system("cls");
	}

}

Scaner::~Scaner()
{
	in_main.close();
}

string Scaner::get_lex()
{
	int state = BEGIN_STATE;
	string bufer, t_str;
	char c1, c2;
	//bool flag = true;
	c1 = keeper_last_char;

	//Пропуск пробелов табуляций и переносов строки преред лексемой

	do {
		if ((c1 == ' ') || (c1 == '\t') || (c1 == '\n'))
		{
			//flag = false;
			if (!in_main.eof())
				c1 = in_main.get();//Получение символа char
		}
		else
		{
			break;
		}
	} while (!in_main.eof());

	if (keeper_last_char == '\0')
	{
		if (!in_main.eof())
			c1 = in_main.get();//Получение символа char
	}

	if (!in_main.eof())
		c2 = in_main.get();//Получение символа char

	if ((c1 >= '0') && (c1 <= '9'))
		state = CONSTANT;

	while ((!in_main.eof()) || (c1 > 0))
	{
		//Начало коментария, если встречен с1 =/ с2 =/ Или с1 =/ с2 = *
		if (delete_coment(c1, c2))//Пропуск коментариев
		{
			//flag = true;
			if (!in_main.eof())
				c1 = in_main.get();
			//Пропуск пробелов табуляций и переносов строки преред лексемой после пропуска коментариев
			while (!in_main.eof())
			{

				if ((c1 == ' ') || (c1 == '\t') || (c1 == '\n'))
				{
					//flag = false;
					if (!in_main.eof())
						c1 = in_main.get();//Получение символа char
				}
				else
				{
					break;
				}
			}
			if (!in_main.eof())
				c2 = in_main.get();//Получение символа char2

			if ((c1 >= '0') && (c1 <= '9'))
				state = CONSTANT;
		}


		//получение лексемы
		t_str = c1;
		if (state == CONSTANT)
		{
			if ((c1 < '0') && (c1 > '9'))
			{
				//error
				t_str = "";
				Token A(t_str, ERROR);
				stream_of_token += A;
				return t_str;
			}

		}
		else
		{
			if (!(((c1 >= 'A') && (c1 <= 'Z')) || ((c1 >= 'a') && (c1 <= 'z')) || (table_of_dividers == t_str) || ((c1 >= '0') && (c1 <= '9'))))
			{
				//error
				t_str = "";
				Token A(t_str, ERROR);
				stream_of_token += A;
				return t_str;
			}
		}
		bufer += c1;
		t_str = c2;
		//Если с1 разделитель или с2 разделитель или пробел илитабуляция или перенос строки получить лексему
		if (table_of_dividers == bufer)//
		{
			keeper_last_char = c2;

			//Разделитель 
			Token A(bufer, DIVIDER);
			stream_of_token += A;

			return bufer;
		}
		if ((table_of_dividers == t_str) || (c2 == ' ') || (c2 == '\t') || (c2 == '\n'))
		{
			keeper_last_char = c2;
			//Константа или ID Или Ключевое слово
			if (state == CONSTANT)//Константа
			{

				Token A(bufer, CONSTANT);
				stream_of_token += A;
				if (!(table_of_constant == bufer))
				{
					Constant B(bufer);
					table_of_constant += B;
				}
			}
			else
			{
				if (table_of_keyWords == bufer)//Ключевое слово
				{
					Token A(bufer, KEYWORD);
					stream_of_token += A;
				}
				else//Идентификатор
				{
					Token A(bufer, IDENTIFIER);
					stream_of_token += A;

					if (!(table_of_identifier == bufer))
					{
						Identifier B(bufer);
						table_of_identifier += B;
					}
				}
			}

			return bufer;
		}

		c1 = c2;

		if (!in_main.eof())
			c2 = in_main.get();
	}

	bufer = "";//При возвращении пустой строки - конечное состояние
	Token A(bufer, END_OF_PROGRAMM);
	stream_of_token += A;

	return bufer;
}

bool Scaner::delete_coment(char c1, char c2)
{
	if (c1 == '/')
	{
		if (c2 == '/')//Однострочный коментарий
		{
			char c1;
			do {
				if (!in_main.eof())
					c1 = in_main.get();
			} while (c1 != '\n');
			return true;

		}
		else
		{
			if (c2 == '*')//многострочный коментарий
			{
				while (!in_main.eof())
				{
					if (!in_main.eof())
						c1 = in_main.get();
					if (c1 == '*')
					{
						if (in_main.eof())
							return false;
						if (!in_main.eof())
							c2 = in_main.get();

						if (c2 == '/')
							return true;
					}
				}
			}
		}
	}
	return false;
}
void Scaner::to_scan()
{
	while (get_lex() != "") {}
}

void Scaner::print_stream_of_lex()
{
	string type;
	cout << setw(10) << "№" << setw(20) << "Name of token" << setw(20) << " Type" << endl;
	for (int i = 0; i < stream_of_token.get_count() - 1; i++)
	{
		switch (stream_of_token[i].get_type())
		{
		case KEYWORD:
			type = "KEYWORD";
			break;
		case DIVIDER:
			type = "DIVIDER";
			break;
		case IDENTIFIER:
			type = "IDENTIFIER";
			break;
		case CONSTANT:
			type = "CONSTANT";
			break;
		case ERROR:
			//type = ERROR;
			cout << "ОШИБКА, невозможно обрабоать лексему " << endl;
			system("pause");
			exit(0);
			break;
		}
		cout << setw(10) << i << setw(20) << stream_of_token[i].get_name() << setw(20) << type << endl;
	}
}

void Scaner::print_another_table()
{
	int i = 0;
	cout << endl << endl;
	//Вывод таблицы ключвых слов
	cout << setw(10) << "№" << setw(20) << "Name of KeyWord" << endl;
	for (auto a : table_of_keyWords.get_vect_key_word())
	{
		cout << setw(10) << i << setw(20) << a << endl;
		i++;
	}

	cout << endl << endl;
	//Вывод таблицы разделителей
	cout << setw(10) << "№" << setw(20) << "Name of Divider" << endl;
	for (auto a : table_of_keyWords.get_vect_key_word())
	{
		cout << setw(10) << i << setw(20) << a << endl;
		i++;
	}

	cout << endl << endl;
	//Вывод таблицы констатнт
	cout << setw(10) << "№" << setw(20) << "Value of constant" << endl;
	for (i = 0; i < table_of_constant.get_count(); i++)
		cout << setw(10) << i << setw(20) << table_of_constant[i].get_name() << endl;

	cout << endl << endl;
	//Вывод таблицы Идинтефикаторов
	cout << setw(10) << "№" << setw(20) << "Name of Identifier" << endl;
	for (i = 0; i < table_of_identifier.get_count(); i++)
		cout << setw(10) << i << setw(20) << table_of_identifier[i].get_name() << endl;
	cout << endl << endl;
}
