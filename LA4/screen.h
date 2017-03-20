//=== Файл screen.h - поддержка работы с экраном
#pragma once
#include <vector>
#include <iostream>

using namespace std;


class point // Точка на экране 
{	
public:
	int x, y;
	point();
	point(int x, int y);
};

class screen
{
	vector<char>data;
	int x, y;
public:
	explicit screen();
	explicit screen(int x, int y);
	void draw();
	void clear();
	inline void put_point_raw(int a, int b);// Вывод точки (3 варианта)
	void put_point(int a, int b);
	void put_point(point p);
	void put_line(int, int, int, int);// Вывод линии (2 варианта)
	void put_line(point a, point b);
	int on_screen(int a, int b);// Проверка попадания на экран
};
