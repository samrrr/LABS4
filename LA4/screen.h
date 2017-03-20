//=== ���� screen.h - ��������� ������ � �������
#pragma once
#include <vector>
#include <iostream>

using namespace std;


class point // ����� �� ������ 
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
	inline void put_point_raw(int a, int b);// ����� ����� (3 ��������)
	void put_point(int a, int b);
	void put_point(point p);
	void put_line(int, int, int, int);// ����� ����� (2 ��������)
	void put_line(point a, point b);
	int on_screen(int a, int b);// �������� ��������� �� �����
};
