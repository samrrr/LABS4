#include <windows.h>
#include <conio.h>
#include <time.h>
#include <iostream>
#include <vector>



using namespace std;

struct SP_EL
{
	int ch;
	SP_EL *n;
};

class SET2
{
private:
	SP_EL *spis;
public:


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
		spis = 0;
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
			return *this;
		}

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
		l->n = nullptr;

		return *this;
	}
	SET2(SET2 &&_a)
	{
		spis = _a.spis;
		_a.spis = nullptr;

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
					p->n = nullptr;
				}
				else
				{
					p->n = new SP_EL;
					p = p->n;
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

		return c;
	}

};



class HA_TA
{
public:
	vector <SET2> a;
	HA_TA()
	{
		a.resize(2);
	}
	void set_size(int n)
	{
		if (n <= 1)
			n = 2;
		a.resize(0);
		a.resize(n);
	}
	int h_f(int id)
	{
		const int pr1 = 27644437;
		const int pr2 = 11;
		return (id*pr1) % a.size();
	}
	void addel(int id)
	{
		a[h_f(id)].addel(id);
	}
	void delel(int id)
	{
		a[h_f(id)].delel(id);
	}

	friend HA_TA operator&(const HA_TA&_a, const HA_TA&_b)
	{
		HA_TA res;
		res.set_size(_a.a.size());
		for (int i = 0; i < _a.a.size(); i++)
		{
			res.a[i] = _a.a[i] & _b.a[i];
		}
		return res;
	}
	friend HA_TA operator|(const HA_TA&_a, const HA_TA&_b)
	{
		HA_TA res;
		res.set_size(_a.a.size());
		for (int i = 0; i < _a.a.size(); i++)
		{
			res.a[i] = _a.a[i] | _b.a[i];
		}
		return res;
	}
	friend HA_TA operator/(const HA_TA&_a, const HA_TA&_b)
	{
		HA_TA res;
		_a.a.size();
		res.set_size(_a.a.size());
		for (int i = 0; i < _a.a.size(); i++)
		{
			res.a[i] = _a.a[i] / _b.a[i];
		}
		return res;
	}
	friend HA_TA operator^(const HA_TA&_a, const HA_TA&_b)
	{
		HA_TA res;
		_a.a.size();
		res.set_size(_a.a.size());
		for (int i = 0; i < _a.a.size(); i++)
		{
			res.a[i] = (_a.a[i] / _b.a[i]) | (_b.a[i] / _a.a[i]);
		}
		return res;
	}
	void put()
	{
		for (int i = 0; i < a.size(); i++)
		{
			cout << i<<":{";
			a[i].put();
			cout << "}\n";
		}		
	}
};




int main()
{
	HA_TA A,B,C,D,E;

	E.set_size(26);
	A = B = C = D = E; 

	for (int i = 0; i < 100; i++)
	{
		A.addel(i);
	}
	for (int i = 0; i < 28; i++)
	{
		A.addel(rand() % 100);
		B.addel(rand() % 100);
		C.addel(rand() % 100);
		D.addel(rand() % 100);
		E.addel(rand() % 100);
	}

	A.put();
	cout <<endl;
	B.put();
	cout << endl;
	C.put();
	cout << endl;
	D.put();
	cout << endl;
	E.put();
	cout << endl;
	cout << endl;
	cout << endl;

	(A&B).put();
	cout << endl;
	(C&D&E).put();
	cout << endl;
	(A&B^C&D&E).put();


	system("pause");
}