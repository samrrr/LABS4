#include "screen.h"

//== 2. ���������� ����� ==
struct shape { // ����������� ������� ����� "������"
	static shape* list;	// ������ ������ ����� (���� �� ��� ������!)
	shape* next;
	shape() { next = list; list = this; } //������ �������������� � ������
	virtual point north() const = 0;	//����� ��� ��������
	virtual point south() const = 0;
	virtual point east() const = 0;
	virtual point west() const = 0;
	virtual point neast() const = 0;
	virtual point seast() const = 0;
	virtual point nwest() const = 0;
	virtual point swest() const = 0;
	virtual void draw(screen &sc) = 0;	//���������
	virtual void move(int, int) = 0;	//�����������
};

shape * shape::list = nullptr;	//������������� ������ �����

class rotatable : public shape { //������, ��������� � �������� 
public:
	virtual void rotate_left() = 0;	//��������� �����
	virtual void rotate_right() = 0;	//��������� ������
};
class reflectable : public shape { // ������, ���������
	// � ����������� ���������
public:
	virtual void flip_horisontally() = 0;	// �������� �������������
	virtual void flip_vertically() = 0;	// �������� �����������
};
class line : public shape {
	/* ������� ������ ["w", "e" ].
	north( ) ���������� ����� "���� ������ ������� � ��� ������
	�� �����, ��� ����� ��� �������� �����", � �. �. */
	point w, e;
public:
	line(point a, point b) : w(a), e(b) { };
	line(point a, int L) : w(point(a.x + L - 1, a.y)), e(a) {  };
	point north() const { return point((w.x + e.x) / 2, e.y<w.y ? w.y : e.y); }
	point south() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
	point east() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
	point west() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
	point neast() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
	point seast() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
	point nwest() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
	point swest() const { return point((w.x + e.x) / 2, e.y<w.y ? e.y : w.y); }
	void move(int a, int b) 	{ w.x += a; w.y += b; e.x += a; e.y += b; }
	void draw(screen &sc) { sc.put_line(w, e); }
};
// �������������
class rectangle : public rotatable {
	/* nw ------ n ------ ne
	|		       |
	|		       |
	w	     c          e
	|		       |
	|		       |
	sw ------ s ------ se */
	point sw, ne;
public:
	rectangle(point, point);
	point north() const { return point((sw.x + ne.x) / 2, ne.y); }
	point south() const { return point((sw.x + ne.x) / 2, sw.y); }
	point east() const { return point(sw.x, (sw.y + ne.y) / 2); }
	point west() const { return point(ne.x, (sw.y + ne.y) / 2); }
	point neast() const { return ne; }
	point seast() const { return point(sw.x, ne.y); }
	point nwest() const { return point(ne.x, sw.y); }
	point swest() const { return sw; }
	void rotate_right() // ������� ������ ������������ se
	{
		int w = ne.x - sw.x, h = ne.y - sw.y;
		sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;
	}
	void rotate_left() // ������� ����� ������������ sw
	{
		int w = ne.x - sw.x, h = ne.y - sw.y;
		ne.x = sw.x + h * 2; ne.y = sw.y + w / 2;
	}
	void move(int a, int b)
	{
		sw.x += a; sw.y += b; ne.x += a; ne.y += b;
	}
	void draw(screen &sc);
};
rectangle::rectangle(point a, point b)
{
	if (a.x <= b.x) {
		if (a.y <= b.y) sw = a, ne = b;
		else sw = point(a.x, b.y), ne = point(b.x, a.y);
	}
	else {
		if (a.y <= b.y) sw = point(b.x, a.y), ne = point(a.x, b.y);
		else sw = b, ne = a;
	}
}
void rectangle::draw(screen &sc)
{
	point nw(sw.x, ne.y);
	point se(ne.x, sw.y);
	sc.put_line(nw, ne);
	sc.put_line(ne, se);
	sc.put_line(se, sw);
	sc.put_line(sw, nw);
}

void shape_draw(screen &sc) // ����������� ���� �����
{
	sc.clear();
	for (shape* p = shape::list; p; p = p->next) p->draw(sc);
	sc.draw();
}
void up(shape* p, const shape* q) // ��������� p ��� q
{	//��� ������� �������, � �� ���� ������!
	point n = q->north();
	point s = p->south();
	p->move(n.x - s.x, n.y - s.y + 1);
}
//��������������������������������