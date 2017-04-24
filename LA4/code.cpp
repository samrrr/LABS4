//���������� ��������� ��� ������ � ����������� �����
// ���������� ���������: 
// ���������� � ������������� ���������� �����
#include <iostream>
using namespace std;
#include "screen.h"
#include "shape.h"

class circle : public shape {//��� ������ �������� ���������������
private:
	point w;
	int r;
public:
	circle(point, int);
	point north() const { return point(w.x, w.y + r); }
	point south() const { return point(w.x, w.y - r); }
	point east() const { return point(w.x+r, w.y); }
	point west() const { return point(w.x-r, w.y); }
	point neast() const { return point(w.x+r, w.y+r); }
	point seast() const { return point(w.x+r, w.y-r); }
	point nwest() const { return point(w.x-r, w.y+r); }
	point swest() const { return point(w.x-r, w.y-r); }
	void draw(screen &sc);
	void move(int, int);
};
void circle::move(int a, int b) 	
{
	w.x += a;
	w.y += b; 
}
circle::circle(point a, int r) :w(a), r(r){}

void circle::draw(screen &sc)
{
	for (int i = 0; i < 20; i++)
	{
		point p1, p2;
		p1.x = w.x + r*sin(i*3.1415926 * 2 / 20);
		p1.y = w.y + r*cos(i*3.1415926 * 2 / 20);
		p2.x = w.x + r*sin((i - 1)*3.1415926 * 2 / 20);
		p2.y = w.y + r*cos((i - 1)*3.1415926 * 2 / 20);
		sc.put_line(p1, p2);
	}
}

// �������������� "�������" ������
class myshape : public rectangle {//��� ������ �������� ���������������
	circle* l_eye; // ����� ���� � ��� ������ �������� �����
	circle* r_eye; // ������ ����
	line* mouth; // ���
public:
	myshape(point, point);
	void draw(screen &sc);
	void move(int, int);
};
myshape::myshape(point a, point b) : rectangle(a, b)
{
	int ll = neast().x - swest().x + 1;
	int hh = neast().y - swest().y + 1;
	l_eye = new circle(point(swest().x + 2, swest().y + hh * 3 / 4), hh/4);
	r_eye = new circle(point(swest().x + ll - 4, swest().y + hh * 3 / 4), hh/4);
	mouth = new line(point(swest().x + 2, swest().y + hh / 4), ll - 4);
}
void myshape::draw(screen &sc)
{
	rectangle::draw(sc);
	rectangle::draw(sc);
	int a = (swest().x + neast().x) / 2;
	int b = (swest().y + neast().y) / 2;
	sc.put_point(point(a, b));
}
void myshape::move(int a, int b)
{
	rectangle::move(a, b);
	l_eye->move(a, b);
	r_eye->move(a, b);
	mouth->move(a, b);
}


int main()
{
	//== 1.���������� ������ ����� ==

	screen sc(200,40);

	rotatable* p1 = new rectangle(point(0, 0), point(14, 5));
	shape* p4 = new circle(point(0, 0), 4);
	shape* p2 = new line(point(0, 15), 17);
	shape* p3 = new myshape(point(15, 10), point(27, 18));

	shape_draw(sc);

	system("pause"); //�������� �������� �����
	//== 2.���������� ==
	p1->rotate_right();
	shape_draw(sc);
	system("pause"); //�������� ����������
	//== 3.������ ����������� ==
	p3->move(-10, -10);
	up(p2, p3);
	up(p1, p2);
	up(p4, p2);
	shape_draw(sc);
	system("pause"); //�������� ���������
	//	screen_destroy( );
	return 0;
}



