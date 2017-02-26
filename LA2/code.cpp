#include <windows.h>
#include <conio.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <queue>



using namespace std;

const int CR = 1;
const int CB = 0;




struct SP_EL
{
	int ch;
	SP_EL *n;
};

class SET2
{
private:
public:
	SP_EL *spis;
	SP_EL *last;


	~SET2()
	{
		SP_EL *p = spis;
		while (p)
		{
			SP_EL *p1 = p;
			p = p->n;
			delete p1;
		}
	}

	SET2()
	{
		spis = nullptr;
		last = nullptr;
	}
	void put()
	{

		for (SP_EL *p = spis; p; p = p->n)
		{
			cout << p->ch;
			if (p->n)
				cout << " ";
		}
	}

	const SET2& operator= (const SET2 &_a)
	{
		SP_EL *p;

		p = spis;
		while (p)
		{
			SP_EL *p1 = p;
			p = p->n;
			delete p1;
		}
		if (_a.spis == nullptr)
		{
			spis = nullptr;
			last = nullptr;
			return *this;
		}

		SP_EL *l;
		SP_EL *el = _a.spis;

		spis = new SP_EL;
		spis->ch = el->ch;
		l = spis;
		el = el->n;

		last = l;

		for (; el; el = el->n)
		{
			p = new SP_EL;
			p->ch = el->ch;
			last = p;
			l->n = p;
			l = p;
		}

		l->n = nullptr;

		return *this;
	}
	SET2(SET2 &&_a)
	{
		spis = _a.spis;
		last = _a.last;
		_a.spis = nullptr;
		_a.last = nullptr;

	}
	SET2(const SET2 &_a)
	{
		if (_a.spis == nullptr)
		{
			spis = nullptr;
			return;
		}
		SP_EL *p;
		SP_EL *l;
		SP_EL *el = _a.spis;

		spis = new SP_EL;
		spis->ch = el->ch;
		l = spis;
		el = el->n;

		for (; el; el = el->n)
		{
			p = new SP_EL;
			p->ch = el->ch;
			l->n = p;
			l = p;
		}
		last = l;
		l->n = nullptr;

	}
	void addel(int i)
	{
		SET2 temp;
		temp.spis = new SP_EL;
		temp.spis->n = nullptr;
		temp.spis->ch = i;
		*this = *this | temp;
	}
	void delel(int i)
	{
		SET2 temp;
		temp.spis = new SP_EL;
		temp.spis->n = nullptr;
		temp.spis->ch = i;
		*this = *this & temp;
	}
	friend SET2 operator & (const SET2 &_a, const SET2 &_b)
	{
		SET2 c;

		SP_EL *p;

		for (SP_EL *p1 = _a.spis, *p2 = _b.spis; p1 != nullptr && p2 != nullptr;
			p1->ch > p2->ch ? p2 = p2->n : p1 = p1->n)
			if (p1->ch == p2->ch)
			{
				if (c.spis == nullptr)
				{
					p = new SP_EL;
					c.spis = p;
					c.last = p;
					p->n = nullptr;
				}
				else
				{
					p->n = new SP_EL;
					p = p->n;
					c.last = p;
					p->n = nullptr;
				}
				p->ch = p1->ch;
			}



		return c;
	}
	friend SET2 operator | (const SET2 &_a, const SET2 &_b)
	{
		SET2 c;

		SP_EL *p1 = _a.spis, *p2 = _b.spis;
		SP_EL *p = nullptr;

		c.spis = 0;
		c.last = 0;

		while (p1 || p2)
		{
			if (p1 == 0 || p2 != 0 && p1->ch > p2->ch)
			{
				if (p == nullptr || p2->ch != p->ch)
				{
					if (p == nullptr)
					{
						p = new SP_EL;
						c.spis = p;
					}
					else
					{
						p->n = new SP_EL;
						p = p->n;
					}
					c.last = p;
					p->n = nullptr;
					p->ch = p2->ch;
				}
				p2 = p2->n;
			}
			else
			{
				if (p == nullptr || p1->ch != p->ch)
				{
					if (p == nullptr)
					{
						p = new SP_EL;
						c.spis = p;
					}
					else
					{
						p->n = new SP_EL;
						p = p->n;
					}
					c.last = p;
					p->n = nullptr;
					p->ch = p1->ch;
				}
				p1 = p1->n;
			}
		}

		return c;
	}
	friend SET2 operator / (const SET2 &_a, const SET2 &_b)
	{
		SET2 c(_a);

		SP_EL *p;

		for (SP_EL **p1 = &c.spis, *p2 = _b.spis; (*p1) && p2;)
		{
			if ((*p1)->ch == p2->ch)
			{
				p = (*p1)->n;
				delete (*p1);
				*p1 = p;

			}

			if (*p1)
				if ((*p1)->ch > p2->ch)
				{
					p2 = p2->n;
				}
				else
				{
					p1 = &((*p1)->n);
				}

		}
		p = c.spis;
		if (p)
		while (p->n)
			p = p->n;
		c.last = p;

		return c;
	}
	friend SET2 operator ^ (const SET2 &_a, const SET2 &_b)
	{
		SET2 c=(_a / _b) | (_b / _a);
		return c;
	}



	void add_part(SET2 &_b)
	{
		if (_b.spis)
		{
			if (spis)
			{
				last->n = _b.spis;
				last = _b.last;

				_b.spis = nullptr;
				_b.last = nullptr;
			}
			else
			{
				spis = _b.spis;
				last = _b.last;

				_b.spis = nullptr;
				_b.last = nullptr;
			}
		}
	}

};



class TEXSEL
{
public:
	char ch;
	char color;
	char color_back;

	TEXSEL()
	{
		ch = ' ';
		color = 15;
		color_back = 0;
	}
	TEXSEL(char _ch)
	{
		ch = _ch;
		color = 0;
		color_back = 15;
	}
};


class NODE 
{
	
	int col; //цвет
	int d;	 //тег узла
	NODE * up;  // папаша
	NODE * pl;	// левый сын
	NODE * pr;	// правый сын
public:
	NODE() : pl(nullptr), pr(nullptr), d(rand() % 20), col(rand() % 2) {} // конструктор узла
	NODE(int el, int _col, NODE* ol) : pl(nullptr), pr(nullptr), d(el), col(_col), up(ol){} // конструктор узла
	NODE(int *ma, int n,int lev,NODE* _up)
	{
		up = _up;
		if (n == 1)
		{
			d = ma[0];
			col = CB;
			if (lev <= 0)
				col = CR;

			pl = nullptr;
			pr = nullptr;
		}
		else
		{
			int nl, nr;
			int mn = n / 2;
			nl = mn;
			nr = n - nl - 1;

			col = CB;
			if (lev <= 0)
				col = CR;

			d = ma[mn];

			if (nl > 0)
			{
				pl = new NODE(ma, nl, lev - 1,this);
			}
			else
			{
				pl = nullptr;
			}
			if (nr > 0)
			{
				pr = new NODE(ma + mn + 1, nr, lev - 1, this);
			}
			else
			{
				pr = nullptr;
			}
		}
	}
	SET2 get()
	{
		SET2 res;
		if (pl)
			res.add_part(pl->get());
		SET2 temp;

		temp.addel(d);
		
		res.add_part(temp);
		if (pr)
			res.add_part(pr->get());

		return res;
	}
	~NODE()
	{
		if (pl) delete pl; // деструктор (уничтожает поддерево)
		if (pr) delete pr;
	}
	friend class TREE;
	friend int put_node(NODE * n, TEXSEL **s, int x, int y);
	friend char mark_pr(NODE * n, char ch);
	friend char obh_sh(NODE * n);
	void add_el(int el)
	{
		if (el < d)
		{
			if (pl == nullptr)
			{
				pl = new NODE(el, CR, this);
				pl->add_fix();
			}
			else
			{
				pl->add_el(el);
			}
		}
		if (el > d)
		{
			if (pr == nullptr)
			{
				pr = new NODE(el, CR,this);
				pr->add_fix();
			}
			else
			{
				pr->add_el(el);
			}
		}
	}
	void rotater();
	void rotatel();
	void add_fix();

	int get_lev()
	{
		int l = 1;
		int l1 = 1;
		int l2 = 1;
		if (pl)
			l1 = pl->get_lev() + 1;
		if (pr)
			l2 = pr->get_lev() + 1;
		return max(l,max(l1,l2));
	}
};

class TREE
{
	int N, depth;

	NODE * root;
public:
	friend class NODE;
	TREE()
	{
		root = nullptr;
	}
	void InTREE(char*);
	void set(vector<int> ma);
	SET2 get_l()
	{
		SET2 res;
		if (root)
			res=root->get();
		return res;
	}
	void grand(int els)
	{
		int la = -1;
		vector<int> tr;
		tr.resize(els);
		for (int i = 0; i < els; i++)
		{
			la = la + 1 + rand() % 3;

			tr[i] = la;
		}
		this->set(tr);
	}
	void OutTREE();	
	void add_el(int el)
	{
		if (root != nullptr)
			root->add_el(el);
		else
			root = new NODE(el, CB,nullptr);
	}


	friend TREE operator &(TREE &_a, TREE &_b)
	{
		SET2 la, lb;
		la = _a.get_l();
		lb = _b.get_l();
		SET2 lc = la & lb;
		TREE c;

		int size = 0;
		auto p = lc.spis;
		while (p)
		{
			p = p->n;
			size++;
		}
		vector <int> vec;
		vec.resize(size);

		p = lc.spis;

		for (int i = 0; i < size; i++)
		{
			vec[i] = p->ch;
			p = p->n;
		}

		c.set(vec);
		return c;
	}

	friend TREE operator ^(TREE &_a, TREE &_b)
	{
		SET2 la, lb;
		la = _a.get_l();
		lb = _b.get_l();
		SET2 lc = la ^ lb;
		TREE c;

		int size = 0;
		auto p = lc.spis;
		while (p)
		{
			p = p->n;
			size++;
		}
		vector <int> vec;
		vec.resize(size);

		p = lc.spis;

		for (int i = 0; i < size; i++)
		{
			vec[i] = p->ch;
			p = p->n;
		}

		c.set(vec);
		return c;
	}

};

void NODE::rotater()
{
	auto val = d;
	d = pl->d;
	pl->d = val;

	NODE* temp = pl->pr;
	pl->pr = pr;
	pr = pl;
	pl = pr->pl;
	pr->pl = temp;
	if (pl != nullptr)
		pl->up = this;
	pr->up = this;
	if (pr->pl != nullptr)
		pr->pl->up = pr;
	if (pr->pr != nullptr)
		pr->pr->up = pr;

}
void NODE::rotatel()
{
	auto val = d;
	d = pr->d;
	pr->d = val;

	NODE* temp = pr->pl;
	pr->pl = pl;
	pl = pr;
	pr = pl->pr;
	pl->pr = temp;
	if (pr != nullptr)
		pr->up = this;
	pl->up = this;
	if (pl->pr != nullptr)
		pl->pr->up = pl;
	if (pl->pl != nullptr)
		pl->pl->up = pl;
	
	

}

void NODE::add_fix()

{
	if (col == CB)
		return;
	cout << "fix st for " << this->d << endl;


	NODE *diada, *grandf;
	if (!up)
	{
		col = CB;
		return;
	}
	if (up->col == CB)
		return;

	grandf = up->up;


	if (!grandf)
	{
		//cout << "tree error no grand father";
		//system("pause");
		//system("exit");
		up->col = CR;
		return;
	}

	diada = (NODE*)(((int)grandf->pl ^ (int)grandf->pr) ^ (int)up);


	if (diada == nullptr || diada->col == CB)
	{
		cout << "diada chern" << endl;
		if (up->pl == this && grandf->pr == up)
		{
			cout << "need bit fixr "<<up->d << endl;
			up->rotater();
			this->add_fix();
			return;
		}
		else
			if (up->pr == this && grandf->pl == up)
			{
				cout << "need bit fixl" << up->d << endl;
				up->rotatel();
				this->add_fix();
				return;
			}
		up->col = CR;
		//grandf == root
		//if (grandf->up == nullptr)
		{

			//NODE* tr3;
			//tr3 = (NODE*)(((int)up->pl ^ (int)up->pr) ^ (int)this);
			if (grandf->pl == diada)
			{
				cout << "roatl" << grandf->d << endl;
				grandf->rotatel();
			}
			else
			{
				cout << "roatr" << grandf->d << endl;
				grandf->rotater();
			}
		}
		grandf->add_fix();
		return;
	}

	if (diada->col == CR)
	{
		up->col = CB;
		grandf->col = CR;
		diada->col = CB;
		grandf->add_fix();
		return;
	}

}


void set_color(int text, int background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

int put_node(NODE * n, TEXSEL **s, int x = 0, int y = 0)
{
	const int sizeout = 3;
	if (n == nullptr)
		return sizeout;

	int ras, ras1, ras2;

	ras1 = put_node(n->pl, s, x, y + 2);
	ras = ras1;
	//if (n->pl == nullptr || n->pr == nullptr)
	//	ras -= 1+sizeout;
	ras2 = put_node(n->pr, s, x + ras + 1, y + 2);
	ras = ras + 1 + ras2;
	if (n->pl == nullptr && n->pr == nullptr)
		ras = sizeout;

	if (n->pl != nullptr)
		if (n->pl->up != n)
	{
			cout << "err at " << n->pl->d;
		system("pause");
	}
	if (n->pr != nullptr)
		if (n->pr->up != n)
	{
			cout << "err at " << n->pr->d;
		system("pause");
	}

	if (n->pl != nullptr && n->pr != nullptr)
	{
		for (int i = x + ras1 / 2 + 1 - sizeout / 2; i <= x + ras1 + sizeout / 2 + ras2 / 2 ; i++)
			s[y + 1][i] = '.';
	}
	else
	{
		if (n->pl != nullptr)
		{
			for (int i = x + ras1 / 2 + 1 - sizeout / 2; i <= x + ras1 + ras2 / 2; i++)
				s[y + 1][i] = '.';
		}
		if(n->pr != nullptr)
		{
			for (int i = x + ras1 / 2 + 1+1; i <= x + ras1 + sizeout / 2 + ras2 / 2; i++)
				s[y + 1][i] = '.';
		}
	}

	char buf[30];

	sprintf(buf, "%i", n->d);

	int c = 0;
	for (int i = 0; buf[i] != '\0'; i++)
		c++;

	for (int i = 0; buf[i] != '\0'; i++)
	{
		//cout <<n->d << " " << buf;
		*(s[y] + x + ras / 2 - (c - 1) / 2 + i) = buf[i];
		if (n->col == CR)
		{
			(s[y] + x + ras / 2 - (c - 1) / 2 + i)->color = 15;
			(s[y] + x + ras / 2 - (c - 1) / 2 + i)->color_back = 4;
		}
		if (n->col == CB)
		{
			(s[y] + x + ras / 2 - (c - 1) / 2 + i)->color = 15;
			(s[y] + x + ras / 2 - (c - 1) / 2 + i)->color_back = 0;
		}

	}
	return ras;

}


void TREE::OutTREE()
{
	TEXSEL *s;
	TEXSEL **ss;
	depth = 1;
	if (root)
		depth = root->get_lev();
	const int sizex=79;

	s = new TEXSEL[sizex * (depth * 2 + 1) + 1];
	ss = new TEXSEL*[depth * 2 + 3];

	for (int i = 0; i < sizex * (depth * 2 + 1); i++)
		s[i] = ' ';

	for (int i = 0; i < depth * 2 + 1; i++)
	{
		s[(i + 1)*sizex - 1] = '\n';
		ss[i] = s + (i+1)*sizex;
	}

	s[sizex * (depth * 2 + 1)] = 0;




	put_node(root, ss);





	//cout << N << endl;
	for (int i = 0; s[i].ch != '\0'; i++)
	{
		set_color(s[i].color,s[i].color_back);
		cout << s[i].ch;
	}

	set_color(15, 0);

	delete[]ss;
	delete[]s;

}

void TREE::InTREE(char *s)
{
	N = 0;
	depth = 0;

	NODE ** arr_n = new NODE*[100 + 1];
	int * arr_s = new int[100 + 1];//0-left 1-right

	int l = 0;
	bool b = 0;

	root = new NODE();
	arr_n[0] = root;
	arr_s[0] = 0;

	for (; b == 0;)
	{
		if (*s == 0)
			return;

		if (*(s++) == '1')
		{
			l++;
			if (l>depth)depth = l;
			if (arr_s[l - 1] == 0)
			{
				arr_n[l] = new NODE();
				arr_n[l - 1]->pl = arr_n[l];
				arr_s[l] = 0;
			}
			else
			{
				arr_n[l] = new NODE();
				arr_n[l - 1]->pr = arr_n[l];
				arr_s[l] = 0;
			}
		}
		else
		{
			if (arr_s[l] == 0)
			{
				arr_s[l] = 1;
			}
			else
			{
				if (arr_n[l]->pl == nullptr)
				{
					N++;
				}
				while (l >= 0 && arr_s[l] == 1)
					l--;
				if (l < 0)
				{
					b = 1;
				}
				else
				{
					arr_s[l] = 1;
				}
			}
		}
	}



	delete[]arr_n;
	delete[]arr_s;
}


void TREE::set(vector<int> ma)
{
	delete root;
	int ll = 0;
	int vv = 1;
	while (vv <= ma.size())
	{
		vv = vv << 1;
		ll++;
	}

	root = new NODE(&ma[0],ma.size(),ll-1,nullptr);
}


int main()
{
	TREE A,B,C,D,E;

	for (int i = 0; i < 30; i++)
	{
		//t.add_el(i);
		//t.OutTREE();
		//system("pause");
	}


	A.grand(27);
	B.grand(27);
	C.grand(27);
	D.grand(27);
	E.grand(27);

	cout << "A" << endl;
	A.OutTREE();
	cout << "B" << endl;
	B.OutTREE();
	cout << "C" << endl;
	C.OutTREE();
	cout << "D" << endl;
	D.OutTREE();
	cout << "E" << endl;
	E.OutTREE();

	cout << "A&B" << endl;
	(A&B).OutTREE();
	cout << "C&D&E" << endl;
	(C&D&E).OutTREE();
	cout << "A&B^C&D&E" << endl;
	((C&D&E)^(A&B)).OutTREE();

	
	cout << endl;
	system("pause");
}
