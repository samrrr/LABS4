//Прикладная программа для работы с библиотекой фигур
// Прикладная программа: 
// пополнение и использование библиотеки фигур
#include <iostream>
using namespace std;
#include "screen.h"
#include "shape.h"
// Дополнительная "сборная" фигура
class myshape : public rectangle {//Моя фигура ЯВЛЯЕТСЯ прямоугольником
	line* l_eye; // левый глаз – моя фигура СОДЕРЖИТ линию
	line* r_eye; // правый глаз
	line* mouth; // рот
public:
	myshape(point, point);
	void draw(screen &sc);
	void move(int, int);
};
myshape::myshape(point a, point b) : rectangle(a, b)
{
	int ll = neast().x - swest().x + 1;
	int hh = neast().y - swest().y + 1;
	l_eye = new line(point(swest().x + 2, swest().y + hh * 3 / 4), 2);
	r_eye = new line(point(swest().x + ll - 4, swest().y + hh * 3 / 4), 2);
	mouth = new line(point(swest().x + 2, swest().y + hh / 4), ll - 4);
}

void myshape::draw(screen &sc)
{
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
	//== 1.Объявление набора фигур ==

	screen sc(200,40);

	rotatable* p1 = new rectangle(point(0, 0), point(14, 5));
	shape* p2 = new line(point(0, 15), 17);
	shape* p3 = new myshape(point(15, 10), point(27, 18));

	shape_draw(sc);

	system("pause"); //Смотреть исходный набор
	//== 2.Ориентация ==
	p1->rotate_right();
	shape_draw(sc);
	system("pause"); //Смотреть ориентацию
	//== 3.Сборка изображения ==
	p3->move(-10, -10);
	up(p2, p3);
	up(p1, p2);
	shape_draw(sc);
	system("pause"); //Смотреть результат
	//	screen_destroy( );
	return 0;
}



