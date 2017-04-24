//=== Файл screen.cpp - поддержка работы с экраном
#include "screen.h"

enum color { black = (char)219, white = '-' };

point::point() :x(0), y(0){};
point::point(int x, int y) : x(x), y(y){};

// Набор утилит для работы с экраном
vector<char>data;
int x, y;
screen::screen() :x(0), y(0){};
screen::screen(int x, int y) :x(x), y(y),data(x*y,white){};
void screen::draw()
{
	for (int r = 0; r < y; r++)
	{
		for (int i = 0; i < x; i++)
		{
			cout << data[i + (y - r - 1)*x];
		}
		cout << endl;
	}
}
inline void screen::put_point_raw(int _x, int _y)
{
	data[_x+_y*x] = black;
}
void screen::put_point(int x, int y)
{
	if (on_screen(x, y))
		put_point_raw(x, y);
}
void screen::put_point(point p)
{
	put_point(p.x, p.y);
}
void screen::put_line(int x0, int y0, int x1, int y1)
/*
Рисование отрезка прямой от (x0,y0) до (x1,y1).
Уравнение прямой: b(x-x0) + a(y-y0) = 0.
Минимизируется величина abs(eps),
где eps = 2*(b(x-x0)) + a(y-y0) (алгоритм Брезенхэма для прямой).
*/
{
	int dx = 1;
	int a = x1 - x0;
	if (a < 0) dx = -1, a = -a;
	int dy = 1;
	int b = y1 - y0;
	if (b < 0) dy = -1, b = -b;
	int two_a = 2 * a;
	int two_b = 2 * b;
	int xcrit = -b + two_a;
	int eps = 0;

	if (on_screen(x0, y0) && on_screen(x1, y1))
	{
		while (1)
		{
			put_point_raw(x0, y0);
			if (x0 == x1 && y0 == y1) break;
			if (eps <= xcrit) x0 += dx, eps += two_b;
			if (eps >= a || a < b) y0 += dy, eps -= two_a;
		}
	}
	else
		while (1)
		{
			put_point(x0, y0);
			if (x0 == x1 && y0 == y1) break;
			if (eps <= xcrit) x0 += dx, eps += two_b;
			if (eps >= a || a < b) y0 += dy, eps -= two_a;
		}
}

void put_circle(int x, int y, int r)
{

}


void screen::put_line(point a, point b)
{
	put_line(a.x,a.y,b.x,b.y);
}
int screen::on_screen(int a, int b)
{
	return 0 <= a && a < x && 0 <= b && b < y;
}
void screen::clear()
{
	data.assign(data.size(),white);
}


