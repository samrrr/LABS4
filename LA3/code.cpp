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


class NODE
{

	int col; //цвет
	int d;	 //тег узла
	int num; //количество элементов
	NODE * up;  // папаша
	NODE * pl;	// левый сын
	NODE * pr;	// правый сын
public:
	NODE() : pl(nullptr), pr(nullptr), d(rand() % 20), col(rand() % 2), num(1) {} // конструктор узла
	NODE(int el, int _col, NODE* ol) : pl(nullptr), pr(nullptr), d(el), col(_col), up(ol), num(1) {} // конструктор узла
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
		if (pl) delete pl; // деструктор (уничтожает поддерево)
		if (pr) delete pr;
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

		sww->up=tu;
		sww->pl=tl;
		sww->pr=tr;

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
			cout << "need bit fixr " << up->d << endl;
			up->rotater();
			pr->add_fix();
			return;
		}
		else
			if (up->pr == this && grandf->pl == up)
			{
				cout << "need bit fixl" << up->d << endl;
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
	for (auto i = pr_list.begin();i!=pr_list.end(); i++)
		cout << (*i)->d<<" ";
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


	t.subst({ 0, 2, 3, 4, 6, 7, 8 }, 0);
	t.OutTREE();
	system("pause");
	t.merge({ 1, 5, 9, 9, 9, 11, 12, 13 });
	t.OutTREE();
	system("pause");
	t.erase(1, 10);
	t.OutTREE();
	system("pause");
	
	for (int i = 0; i < 4; i++)
	{
		t.OutTREE();
		system("pause");
		int g = rand() % (30-i);
		g = 0;
		t.erase(g,g);
	}


	system("pause");
}
