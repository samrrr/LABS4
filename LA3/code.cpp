#include <windows.h>
#include <conio.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <queue>
#include <list>



using namespace std;

const int CR = 1;
const int CB = 0;



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



class NODE;

struct SP_EL
{
	NODE *lin;
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
			cout << p->lin;
			if (p->n)
				cout << " ";
		}
	}
	void killrep();
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
		spis->lin = el->lin;
		l = spis;
		el = el->n;

		last = l;

		for (; el; el = el->n)
		{
			p = new SP_EL;
			p->lin = el->lin;
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
		spis->lin = el->lin;
		l = spis;
		el = el->n;

		for (; el; el = el->n)
		{
			p = new SP_EL;
			p->lin = el->lin;
			l->n = p;
			l = p;
		}
		last = l;
		l->n = nullptr;

	}
	void addel(NODE *i)
	{
		SET2 temp;
		temp.spis = new SP_EL;
		temp.spis->n = nullptr;
		temp.spis->lin = i;
		*this = *this | temp;
	}
	void delel(NODE *i)
	{
		SET2 temp;
		temp.spis = new SP_EL;
		temp.spis->n = nullptr;
		temp.spis->lin = i;
		*this = *this & temp;
	}
	friend SET2 operator & (const SET2 &_a, const SET2 &_b);
	friend SET2 operator | (const SET2 &_a, const SET2 &_b);
	friend SET2 operator / (const SET2 &_a, const SET2 &_b);
	friend SET2 operator ^ (const SET2 &_a, const SET2 &_b)
	{
		SET2 c = (_a / _b) | (_b / _a);
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





class NODE
{

	int col; //����
	int num; //���������� ���������
	NODE * up;  // ������
	NODE * pl;	// ����� ���
	NODE * pr;	// ������ ���
public:
	int d;	 //��� ����
	NODE() : pl(nullptr), pr(nullptr), d(rand() % 20), col(rand() % 2), num(1) {} // ����������� ����
	NODE(int el, int _col, NODE* ol) : pl(nullptr), pr(nullptr), d(el), col(_col), up(ol), num(1) {} // ����������� ����
	NODE(int *ma, int n, int lev, NODE* _up)
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
				pl = new NODE(ma, nl, lev - 1, this);
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
	~NODE()
	{
		if (pl) delete pl; // ���������� (���������� ���������)
		if (pr) delete pr;
	}


	SET2 get()
	{
		SET2 res;
		if (pl)
			res.add_part(pl->get());
		SET2 temp;

		temp.addel(this);

		res.add_part(temp);
		if (pr)
			res.add_part(pr->get());

		return res;
	}


	friend class TREE;
	friend int put_node(NODE * n, TEXSEL **s, int x, int y);
	friend char mark_pr(NODE * n, char ch);
	friend char obh_sh(NODE * n);


	void swap_with(NODE* sww)
	{
		NODE *tu, *tl, *tr;
		tu = up;
		tl = pl;
		tr = pr;

		if (up)
		{
			if (up->pl == this)
				up->pl = sww;
			if (up->pr == this)
				up->pr = sww;
		}
		if (sww->up)
		{
			if (sww->up->pl == sww)
				sww->up->pl = this;
			if (sww->up->pr == sww)
				sww->up->pr = this;
		}

		if (pl)
			pl->up = sww;
		if (pr)
			pr->up = sww;

		if (sww->pl)
			sww->pl->up = this;
		if (sww->pr)
			sww->pr->up = this;

		up = sww->up;
		pl = sww->pl;
		pr = sww->pr;

		sww->up = tu;
		sww->pl = tl;
		sww->pr = tr;

		auto tcol = col;

		col = sww->col;
		sww->col = tcol;


		if (up == this)
		{
			up = sww;
			if (sww->pl == sww)
				sww->pl = this;
			if (sww->pr == sww)
				sww->pr = this;
		}
		if (sww->up == sww)
		{
			sww->up = this;
			if (this->pl == this)
				this->pl = sww;
			if (this->pr == this)
				this->pr = sww;
		}

	}

	int del_this()//0-ok 1-rootdel
	{
		while (pl || pr)
		{
			if (pr)
			{
				auto li = pr;
				while (li->pl)
					li = li->pl;

				swap_with(li);
			}
			if (pl)
			{
				auto li = pl;
				while (li->pr)
					li = li->pr;

				swap_with(li);
			}
		}
		if (!up)
		{
			return 1;

		}


		return 0;
	}

	void del()
	{
		if (up->pl == this)
			up->pl = nullptr;
		if (up->pr == this)
			up->pr = nullptr;
		delete this;
	}

	NODE* add_el(int el)
	{
		if (el < d)
		{
			if (pl == nullptr)
			{
				pl = new NODE(el, CR, this);
				auto li = pl;
				pl->add_fix();
				return li;
			}
			else
			{
				return pl->add_el(el);
			}
		}
		if (el > d)
		{
			if (pr == nullptr)
			{
				pr = new NODE(el, CR, this);
				auto li = pr;
				pr->add_fix();
				return li;
			}
			else
			{
				return pr->add_el(el);
			}
		}
		if (el == d)
		{
			num++;
			return this;
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
		return max(l, max(l1, l2));
	}

	void fix_nig_l()
	{
		int c_s;
		int c_vl;
		int c_vr;

		c_s = pr->col;
		c_vl = CB;
		c_vr = CB;
		if (pr->pl)
			c_vl = pr->pl->col;
		if (pr->pr)
			c_vr = pr->pr->col;

		///v1 
		if (c_s == CR)
		{
			col = CR;
			pr->col = CB;
			rotatel();

			c_s = pr->col;
			c_vl = CB;
			c_vr = CB;
			if (pr->pl)
				c_vl = pr->pl->col;
			if (pr->pr)
				c_vr = pr->pr->col;

		}

		if (c_vl == CB && c_vr == CB)
		{
			pr->col = CR;
			if (col == CB)
			{
				if (up)
				{
					if (up->pl == this)
						up->fix_nig_l();
					else
						if (up->pr == this)
							up->fix_nig_r();
				}
			}
			col = CB;
			return;
		}

		if (c_vl == CR && c_vr == CB)
		{
			pr->pl->col = CB;
			pr->col = CR;

			pr->rotater();

			c_s = pr->col;
			c_vl = CB;
			c_vr = CB;
			if (pr->pl)
				c_vl = pr->pl->col;
			if (pr->pr)
				c_vr = pr->pr->col;
		}

		if (c_vr == CR)
		{
			pr->pr->col = CB;
			pr->col = col;
			col = CB;
			rotatel();
		}

	}

	void fix_nig_r()
	{
		int c_s;
		int c_vl;
		int c_vr;

		c_s = pl->col;
		c_vl = CB;
		c_vr = CB;
		if (pl->pr)
			c_vl = pl->pr->col;
		if (pl->pl)
			c_vr = pl->pl->col;

		///v1 
		if (c_s == CR)
		{
			col = CR;
			pl->col = CB;
			rotater();

			c_s = pl->col;
			c_vl = CB;
			c_vr = CB;
			if (pl->pr)
				c_vl = pl->pr->col;
			if (pl->pl)
				c_vr = pl->pl->col;

		}

		if (c_vl == CB && c_vr == CB)
		{
			pl->col = CR;
			if (col == CB)
			{
				if (up)
				{
					if (up->pr == this)
						up->fix_nig_r();
					if (up->pl == this)
						up->fix_nig_l();
				}
			}
			col = CB;
		}

		if (c_vl == CR && c_vr == CB)
		{
			pl->pr->col = CB;
			pl->col = CR;

			pl->rotatel();

			c_s = pl->col;
			c_vl = CB;
			c_vr = CB;
			if (pl->pr)
				c_vl = pl->pr->col;
			if (pl->pl)
				c_vr = pl->pl->col;
		}

		if (c_vr == CR)
		{
			pl->pl->col = CB;
			pl->col = col;
			col = CB;
			rotater();
		}

	}
};

class TREE
{
	int N;
	list<NODE*> pr_list;
	NODE *root;
public:
	friend class NODE;
	TREE()
	{
		root = nullptr;
	}
	~TREE()
	{
		delete root;
	}
	void InTREE(char*);
	void add_last(int el);
	void OutTREE();

	void subst(vector<int> ma, int nom)
	{
		list<NODE*> ladd;
		auto itt = pr_list.begin();

		for (int i = 0; i < nom; i++)
			itt++;

		for (int i = 0; i < ma.size(); i++)
		{
			NODE* li;
			if (root != nullptr)
			{
				li = root->add_el(ma[i]);
				root_fix();
			}
			else
			{
				li = root = new NODE(ma[i], CB, nullptr);
			}
			ladd.push_back(li);
		}
		pr_list.splice(itt, ladd);
	}

	void merge(vector<int> ma)
	{
		list<NODE*> ladd;


		for (int i = 0; i < ma.size(); i++)
		{
			NODE* li;
			if (root != nullptr)
			{
				li = root->add_el(ma[i]);
				root_fix();
			}
			else
			{
				li = root = new NODE(ma[i], CB, nullptr);
			}
			ladd.push_back(li);
		}
		pr_list.merge(ladd, [](NODE*a, NODE*b){return a->d < b->d; });
	}

	void erase(int st, int en)
	{
		auto i = pr_list.begin();
		for (int r = 0; r < st; r++)
			i++;

		for (int r = st; r <= en; r++)
		{
			auto iter = i;
			auto lli = *i;
			i++;
			pr_list.erase(iter);
			if (lli->num > 1)
			{
				lli->num--;
			}
			else
			{
				int res = (lli)->del_this();
				root_fix();
				if (res == 0)
				{
					if (lli->up->pl == lli)
					{
						if (lli->col == CB)
						{
							(lli)->up->pl = nullptr;
							(lli)->up->fix_nig_l();
							root_fix();
							lli->del();
						}
						else
						{
							(lli)->up->pl = nullptr;
							lli->del();
						}
					}
					else
						if (lli->up->pr == lli)
						{
							if (lli->col == CB)
							{
								(lli)->up->pr = nullptr;
								(lli)->up->fix_nig_r();
								root_fix();
								lli->del();
							}
							else
							{
								(lli)->up->pr = nullptr;

								lli->del();
							}
						}

				}
				else
				{
					delete lli;
					root = nullptr;
				}

			}

		}

	}

	SET2 get_l()
	{
		SET2 res;
		if (root)
			res = root->get();
		return res;
	}

	TREE(TREE &&_a)
	{
		root = _a.root;
		_a.root = nullptr;

		pr_list = _a.pr_list;
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
			vec[i] = p->lin->d;
			p = p->n;
		}

		c.subst(vec, 0);
		return c;
	}

	friend TREE operator |(TREE &_a, TREE &_b)
	{
		SET2 la, lb;
		la = _a.get_l();
		lb = _b.get_l();
		SET2 lc = la | lb;
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
			vec[i] = p->lin->d;
			p = p->n;
		}

		c.subst(vec, 0);
		return c;
	}

	friend TREE operator /(TREE &_a, TREE &_b)
	{
		SET2 la, lb;
		la = _a.get_l();
		lb = _b.get_l();
		SET2 lc = la / lb;
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
			vec[i] = p->lin->d;
			p = p->n;
		}

		c.subst(vec, 0);
		return c;
	}

	friend TREE operator ^(TREE &_a, TREE &_b)
	{
		SET2 la, lb;
		la = _a.get_l();
		lb = _b.get_l();
		la.killrep();
		lb.killrep();
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
			vec[i] = p->lin->d;
			p = p->n;
		}

		c.subst(vec, 0);
		return c;
	}

	void root_fix()
	{
		while (root->up)
			root = root->up;
	}

};



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
		for (int i = x + ras1 / 2 + 1 - sizeout / 2; i <= x + ras1 + sizeout / 2 + ras2 / 2; i++)
			s[y + 1][i] = '.';
	}
	else
	{
		if (n->pl != nullptr)
		{
			for (int i = x + ras1 / 2 + 1 - sizeout / 2; i <= x + ras1 + ras2 / 2; i++)
				s[y + 1][i] = '.';
		}
		if (n->pr != nullptr)
		{
			for (int i = x + ras1 / 2 + 1 + 1; i <= x + ras1 + sizeout / 2 + ras2 / 2; i++)
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


void NODE::rotater()
{
	NODE* temp = pl->pr;
	pl->pr = this;
	pl->up = up;
	up = pl;
	pl = temp;

	if (pl)
		pl->up = this;
	if (up->pl)
		up->pl->up = up;
	if (up->up)
	{
		if (up->up->pl == this)
			up->up->pl = up;
		if (up->up->pr == this)
			up->up->pr = up;
	}
}
void NODE::rotatel()
{
	NODE* temp = pr->pl;
	pr->pl = this;
	pr->up = up;
	up = pr;
	pr = temp;

	if (pr)
		pr->up = this;
	if (up->pr)
		up->pr->up = up;
	if (up->up)
	{
		if (up->up->pr == this)
			up->up->pr = up;
		if (up->up->pl == this)
			up->up->pl = up;
	}
}


void NODE::add_fix()

{
	if (col == CB)
		return;
	//cout << "fix st for " << this->d << endl;


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
		//cout << "diada chern" << endl;
		if (up->pl == this && grandf->pr == up)
		{
			//cout << "need bit fixr " << up->d << endl;
			up->rotater();
			pr->add_fix();
			return;
		}
		else
			if (up->pr == this && grandf->pl == up)
			{
				//cout << "need bit fixl" << up->d << endl;
				up->rotatel();
				pl->add_fix();
				return;
			}
		up->col = CB;
		grandf->col = CR;
		if (diada)
			diada->col = CB;
		//grandf == root
		//if (grandf->up == nullptr)
		{

			//NODE* tr3;
			//tr3 = (NODE*)(((int)up->pl ^ (int)up->pr) ^ (int)this);
			if (grandf->pl == diada)
			{
				//cout << "roatl" << grandf->d << endl;
				grandf->rotatel();
			}
			else
			{
				//cout << "roatr" << grandf->d << endl;
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




void TREE::OutTREE()
{
	TEXSEL *s;
	TEXSEL **ss;
	int depth = 1;
	if (root)
		depth = root->get_lev();
	const int sizex = 79;

	s = new TEXSEL[sizex * (depth * 2 + 1) + 1];
	ss = new TEXSEL*[depth * 2 + 3];

	for (int i = 0; i < sizex * (depth * 2 + 1); i++)
		s[i] = ' ';

	for (int i = 0; i < depth * 2 + 1; i++)
	{
		s[(i + 1)*sizex - 1] = '\n';
		ss[i] = s + (i + 1)*sizex;
	}

	s[sizex * (depth * 2 + 1)] = 0;




	put_node(root, ss);





	//cout << N << endl;
	for (int i = 0; s[i].ch != '\0'; i++)
	{
		set_color(s[i].color, s[i].color_back);
		cout << s[i].ch;
	}

	set_color(15, 0);
	for (auto i = pr_list.begin(); i != pr_list.end(); i++)
		cout << (*i)->d << " ";
	cout << endl;

	delete[]ss;
	delete[]s;

}



void TREE::add_last(int el)
{

	NODE* li;
	if (root != nullptr)
	{
		li = root->add_el(el);
		root_fix();
	}
	else
	{
		li = root = new NODE(el, CB, nullptr);
	}

	pr_list.push_back(li);
}


int main()
{

	TREE t;


	TREE A, B, C, D, E;

	A.subst({ 0, 2,2, 3, 4, 6, 2,7, 8 }, 0);
	B.subst({ 2, 3, 4, 5, 6, 7 }, 0);
	C.subst({ 5, 6, 4, 2, 9 }, 0);
	D.subst({ 7, 50, 51, 6 }, 0);
	E.subst({ 3, 4, 9, 1 }, 0);
	
	cout << "A^B\n";
	(A^B).OutTREE();
	cout << "C&D\n";
	(C&D).OutTREE();
	cout << "(A^B)/(C&D)\n";
	((A^B) / (C&D)).OutTREE();
	cout << "(A^B)/(C&D)|E\n";
	( (A^B)/(C&D)|E ).OutTREE();
	system("pause");

	A.subst({0,1,0},2);
	A.OutTREE();

	system("pause");
}







SET2 operator & (const SET2 &_a, const SET2 &_b)
{
	SET2 c;

	SP_EL *p;

	for (SP_EL *p1 = _a.spis, *p2 = _b.spis; p1 != nullptr && p2 != nullptr;
		p1->lin->d > p2->lin->d ? p2 = p2->n : p1 = p1->n)
		if (p1->lin->d == p2->lin->d)
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
			p->lin = p1->lin;
		}



	return c;
}
SET2 operator | (const SET2 &_a, const SET2 &_b)
{
	SET2 c;

	SP_EL *p1 = _a.spis, *p2 = _b.spis;
	SP_EL *p = nullptr;

	c.spis = 0;
	c.last = 0;

	while (p1 || p2)
	{
		if (p1 == 0 || p2 != 0 && p1->lin->d > p2->lin->d)
		{
			if (p == nullptr || p2->lin->d != p->lin->d)
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
				p->lin = p2->lin;
			}
			p2 = p2->n;
		}
		else
		{
			if (p == nullptr || p1->lin->d != p->lin->d)
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
				p->lin = p1->lin;
			}
			p1 = p1->n;
		}
	}

	return c;
}
SET2 operator / (const SET2 &_a, const SET2 &_b)
{
	SET2 c(_a);

	SP_EL *p;

	for (SP_EL **p1 = &c.spis, *p2 = _b.spis; (*p1) && p2;)
	{
		if ((*p1)->lin->d == p2->lin->d)
		{
			p = (*p1)->n;
			delete (*p1);
			*p1 = p;

		}

		if (*p1)
			if ((*p1)->lin->d > p2->lin->d)
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
void SET2::killrep()
{
	auto pp = spis;

	if (!pp)
		return;

	while (pp->n)
	{
		if (pp->lin->d == pp->n->lin->d)
		{
			auto temp = pp->n;
			pp->n = pp->n->n;
			temp->n = nullptr;
			delete temp;
		}
		else
			pp = pp->n;
	}


}
