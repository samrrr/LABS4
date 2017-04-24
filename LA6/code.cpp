#include <iostream>
#include <unordered_set>
#include <set>
#include <iterator>
#include <algorithm>

#include <windows.h>
using namespace std;

#include <intrin.h>  

#pragma intrinsic(__rdtsc)  


class my_t
{
public:
	int ttt;
	int val;

	bool operator <(const my_t &a)const
	{
		return val < a.val;
	}

	bool operator !=(const my_t &a)const = delete;
	bool operator ==(const my_t &a)const = delete;

};


template<class T>
bool ssp_cmp(const T* a, const T* b)
{
	return (*a) < (*b);
}

template<class T>
struct sp_t_less {
	bool operator()(const T* a, const T* b)
	{
		return (*a) < (*b);
		//if ((*a) < (*b))
		//	return 1;

		//if ((*b) < (*a))
		//	return 0;

		//return a < b;//ну надо же что-то делать...
	}
};


template<class T>
class supset//DANGER!!! ALL FUNCTIONS DESTROYS PARAMETERS! 
{
private:
	vector < T*> ar;
	multiset<T*, sp_t_less<T>> tr;
public:

	void gen(int size=10)
	{
		if (size <= 0)
			return;

		vector<T*> arrz,arrc;
		arrz.resize(size);

		int nn = rand() % size;
		for (int i = 0; i < arrz.size(); i++)
		{
			arrz[i] = new T();
			arrz[i]->val = nn;
			nn += rand() % 3;
		}
		
		arrc = arrz;

		for (int i = arrz.size()-1; i > 0; i--)
		{
			int pos = rand() % i;
			swap(arrc[i], arrc[pos]);
		}

		int n = arrz.size();

#ifdef _DEBUG
		cout << "A:" << endl;
		for (int i = 0; i < n; i++)
			cout << arrz[i]->val << ";";
		cout << endl;

		cout << "B:" << endl;
		for (int i = 0; i < n; i++)
			cout << arrc[i]->val << ";";
		cout << endl;
#endif		

		reset_by_arrs(arrz, arrc);
	}

	void reset_by_arrs(vector<T*> arrs, vector<T*> arrp)
	{
		ar = arrp;//50% готово
		tr.clear();
		tr.insert(arrs.begin(), arrs.end());		

#ifdef _DEBUG
		cout << "tr:" << endl;
		for (auto i = tr.begin(); i != tr.end(); i++)
			cout << (*i)->val << ";";
		cout << endl;
#endif		

	}



	void subst(const supset &b, int pos)
	{
		if (pos > b.ar.size())
			pos = b.ar.size();
		if (pos < 0)
			pos = 0;

		supset &a = *this;

		vector <T*> va_s(a.ar.size());
		vector <T*> vb_s(b.ar.size());
		vector <T*> vc_s;
		vc_s.reserve(a.ar.size() + b.ar.size());
		vector <T*> vc_u(a.ar.size() + b.ar.size());

		copy(a.tr.begin(), a.tr.end(), va_s.begin());
		copy(b.tr.begin(), b.tr.end(), vb_s.begin());

		std::merge(va_s.begin(), va_s.end(), vb_s.begin(), vb_s.end(), back_inserter(vc_s), ssp_cmp<T>);

		auto ita = a.ar.begin();
		auto itb = b.ar.begin();

		int i;

		for (i = 0; i < pos; i++, ita++)
			vc_u[i] = *ita;

		for (int r = 0; r < vb_s.size(); r++, i++, itb++)
			vc_u[i] = *itb;

		for (int r = pos + b.ar.size(); r < vc_u.size(); r++, i++, ita++)
			vc_u[i] = *ita;



#ifdef _DEBUG
		cout << "vc_u:" << endl;
		for (auto it = vc_u.begin(); it != vc_u.end(); it++)
			cout << (*it)->val << ";";
		cout << endl;

		cout << "vc_s:" << endl;
		for (auto it = vc_s.begin(); it != vc_s.end(); it++)
			cout << (*it)->val << ";";
		cout << endl;
#endif		

		reset_by_arrs(vc_s, vc_u);
	}

	void cut_copy(vector<T*> &a)
	{
		for (int i = 0; i < (int)a.size() - 1; i++)
		{
			if (a[i]->val == a[i + 1]->val)
			{
				a[i]->val = INT_MIN;
			}
		}

		int r = 0;
		for (int i = 0; i < a.size(); i++)
		{
			if (a[i]->val == INT_MIN)
			{
				delete a[i];
			}
			else
			{
				a[r] = a[i];
				r++;
			}
		}
		a.resize(r);
	}

	void erase(int st, int pos)
	{
		if (st < 0)
			st=0;
		if (pos >= ar.size()|| pos==-1)
			pos=ar.size()-1;
		if (pos<st)
			return;

		vector <T*> va_s(ar.size());
		copy(tr.begin(), tr.end(), va_s.begin());
		auto &va_u = ar;

		//va_s
		//va_u
		for (int i = st; i <= pos; i++)
		{
			va_u[i]->val = INT_MIN;
		}

		for (int i = pos + 1; i < va_u.size(); i++)
		{
			va_u[i - pos + st - 1] = va_u[i];
		}

		int r = 0;
		for (int i = 0; i < va_u.size(); i++)
		{
			if (va_s[i]->val == INT_MIN)
			{
				delete va_s[i];
			}
			else
			{
				va_s[r] = va_s[i];
				r++;
			}
		}

		va_s.resize(va_s.size() - pos + st - 1);

		va_u.resize(va_u.size() - pos + st - 1);

		reset_by_arrs(va_s, va_u);
	}

	supset& operator | (const supset &b)
	{
		auto &a = *this;

		vector <T*> va_s(a.ar.size());
		vector <T*> vb_s(b.ar.size());
		vector <T*> vc_s;
		vc_s.reserve(a.ar.size() + b.ar.size());

		copy(a.tr.begin(), a.tr.end(), va_s.begin());
		copy(b.tr.begin(), b.tr.end(), vb_s.begin());

		std::merge(va_s.begin(), va_s.end(), vb_s.begin(), vb_s.end(), back_inserter(vc_s), ssp_cmp<T>);

		for (int i = 0; i < (int)vc_s.size() - 1; i++)
		{
			if (vc_s[i]->val == vc_s[i + 1]->val)
			{
				vc_s[i]->val = INT_MIN;
			}
		}

		int r = 0;
		for (int i = 0; i < vc_s.size(); i++)
		{
			if (vc_s[i]->val == INT_MIN)
			{
				delete vc_s[i];
			}
			else
			{
				vc_s[r] = vc_s[i];
				r++;
			}
		}
		vc_s.resize(r);

		reset_by_arrs(vc_s, vc_s);
		return *this;
	}

	supset& operator & (const supset &b)
	{
		auto &a = *this;

		vector <T*> va_s(a.ar.size());
		vector <T*> vb_s(b.ar.size());
		vector <T*> vc_s;
		vc_s.reserve(a.ar.size() + b.ar.size());
		vector <T*> del_ray1;
		vector <T*> del_ray2;
		vector <T*> del_ray3;
		del_ray1.reserve(a.ar.size() + b.ar.size());
		del_ray2.reserve(a.ar.size() + b.ar.size());
		del_ray3.reserve(a.ar.size() + b.ar.size());

		copy(a.tr.begin(), a.tr.end(), va_s.begin());
		cut_copy(va_s);

		copy(b.tr.begin(), b.tr.end(), vb_s.begin());
		cut_copy(vb_s);

		std::set_intersection(va_s.begin(), va_s.end(), vb_s.begin(), vb_s.end(), back_inserter(vc_s), ssp_cmp<T>);
		std::set_difference(va_s.begin(), va_s.end(), vb_s.begin(), vb_s.end(), back_inserter(del_ray1), ssp_cmp<T>);
		std::set_difference(vb_s.begin(), vb_s.end(), va_s.begin(), va_s.end(), back_inserter(del_ray2), ssp_cmp<T>);

		std::merge(va_s.begin(), va_s.end(), vb_s.begin(), vb_s.end(), back_inserter(del_ray3), ssp_cmp<T>);

		int r = 0;
		for (int i = 0; i < del_ray1.size(); i++)
		{
			del_ray1[i]->val = INT_MIN;
		}
		r = 0;
		for (int i = 0; i < del_ray2.size(); i++)
		{
			del_ray2[i]->val = INT_MIN;
		}
		r = 0;
		for (int i = 0; i < vc_s.size(); i++)
		{
			vc_s[i]->val = INT_MIN;
		}
		
		r = 0;
		for (int i = 0; i < del_ray3.size(); i++)
		{
			if (del_ray3[i]->val == INT_MIN)
			{
				delete del_ray3[i];
			}
			else
			{
				del_ray3[r] = del_ray3[i];
				r++;
			}
		}
		del_ray3.resize(r);

		reset_by_arrs(del_ray3, del_ray3);
		return *this;
	}

	void merge(const supset &b)
	{
		auto &a = *this;

		vector <T*> va_s(a.ar.size());
		vector <T*> vb_s(b.ar.size());
		vector <T*> vc_s;
		vc_s.reserve(a.ar.size() + b.ar.size());

		copy(a.tr.begin(), a.tr.end(), va_s.begin());
		copy(b.tr.begin(), b.tr.end(), vb_s.begin());


#ifdef _DEBUG
		cout << "va_s:" << endl;
		for (auto it = va_s.begin(); it != va_s.end(); it++)
			cout << (*it)->val << ";";
		cout << endl;

		cout << "vb_s:" << endl;
		for (auto it = vb_s.begin(); it != vb_s.end(); it++)
			cout << (*it)->val << ";";
		cout << endl;
#endif		

		
		std::merge(va_s.begin(), va_s.end(), vb_s.begin(), vb_s.end(), back_inserter(vc_s), ssp_cmp<T>);

		reset_by_arrs(vc_s, vc_s);
	}


	void put()
	{
		cout << "P:" << endl;
		for (auto it = ar.begin(); it != ar.end(); it++)
			cout << (*it)->val << ";";
		cout << endl;

		cout << "D:" << endl;
		for (auto it = tr.begin(); it != tr.end(); it++)
			cout << (*it)->val << ";";
		cout << endl;

	}

};

int main()
{
	supset<my_t> A, B, C, D;

	float ddif = 0;
	for (int i = 1; i < 1000000;)
	{
		//i = 10000;

		int ss1 = __rdtsc();
		A.gen(i);
		B.gen(i);
		A = A & B;
		int ss2 = __rdtsc();

		ddif = ddif * 0.9 + 0.1 * (ss2 - ss1) / 100000.0 / i;
		cout << ddif << endl;

		i++;
		i = i * 1.2;

		A&D;
	}

	/*

	C.gen();

	cout << endl << "A" << endl;
	A.put();
	cout << endl << "B" << endl;
	B.put();
	cout << endl << "C" << endl;
	C.put();

	A.subst(B, 5);
	A.put();

	A.erase(4, 100);
	A.put();

	A=A&C;
	A.put();


	*/
	system("pause");
}
