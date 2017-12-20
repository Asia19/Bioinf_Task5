#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
struct mypair
{
	int *a;
	int number;
};

void outputVec(int *a, int l)
{
	for (int i = 0; i < l; i++)
		cout << a[i] << "  ";
	cout << endl;
}
int* NextLeaf(int *a, int l, int k)
{
	l--;
	for (int i = l; i >= 0; i--)
	{
		if (a[i] < k - 1)  // (a[i] < k-1)  ????
		{
			a[i] = a[i] + 1;
			return a;
		}
		a[i] = 0;
	}
	return a;
}
void AllLeaves(int l, int k)
{
	int *a = new int[l];
	for (int i = 0; i < l; i++)
		a[i] = 0;   /// or 1?
	while (true)
	{
		outputVec(a, l);
		a = NextLeaf(a, l, k);
		int flag = 1;
		for (int i = 0; i < l; i++)
			if (a[i] != 1) flag = 0;
		if (flag) return;
	}
}
mypair Bypass(int *a, int i, int l, int k)
{
	mypair p;
	p.a = a;
	for (int j = i; j >= 0; j--)
		if (a[j] < k - 1)
		{
			a[j] = a[j] + 1;
			p.number = j;
			return p;
		}
	p.number = -1;
	return p;
}
mypair NextVertex(int *a, int i, int l, int k)
{
	mypair p;
	p.a = a;
	if (i < l)
	{
		a[i + 1] = 0;
		p.number = i + 1;
		return p;
	}
	else
	{		
		for (int j = l-1; j >= 0; j--)
			if (a[j] < k - 1)
			{
				a[j] = a[j] + 1;
				p.number = j;
				return p;
			}
	}
	p.number = -1;
	return p;
}
int Score(char **mas, int *s, int k, int m)
{	
	int score = 0;
	string bestMotifs = "";
	int *M = new int[k];
	for (int c = 0; c < k; c++)
	{
		int *letters = new int[4];
		for (int ii = 0; ii < 4; ii++) letters[ii] = 0;
		for (int r = 0; r < m; r++)
		{
			switch (mas[r][s[r] + c])
			{
			case 'A': letters[0]++; break;
			case 'C': letters[1]++; break;
			case 'G': letters[2]++; break;
			case 'T': letters[3]++; break;
			}
		}
		int max = letters[0];
		for (int ii = 1; ii < 4; ii++)
			if (letters[ii]>max) max = letters[ii];
		M[c] = max;
		delete[] letters;
	}
	/*cout << "score at first = " <<score << endl;*/
	for (int ii = 0; ii < k; ii++)
		score += M[ii];
	//delete[] M;
	/*cout << "I return score= " << score << " for start postions:";
	for (int ii = 0; ii < m; ii++) cout << s[ii] << " ";
	cout << endl;
	for (int v = 0; v < m; v++)
	{
		for (int w = 0; w < k; w++)
			bestMotifs += mas[v][s[v] + w];
		bestMotifs += '\n';
	}
	cout << "it's motif" << endl << bestMotifs << endl;*/
	return score;
}
int Score2(char **mas, int i, int k, int *s)
{
	int score = 0;
	int *M = new int[k];
	for (int c = 0; c < k; c++)
	{
		int *letters = new int[4];
		for (int ii = 0; ii < 4; ii++) letters[ii] = 0;
		for (int r = 0; r <i; r++)
		{
			switch (mas[r][s[r] + c])
			{
			case 'A': letters[0]++; break;
			case 'C': letters[1]++; break;
			case 'G': letters[2]++; break;
			case 'T': letters[3]++; break;
			}
		}
		int max = letters[0];
		for (int ii = 1; ii < 4; ii++)
			if (letters[ii]>max) max = letters[ii];
		M[c] = max;
		delete[] letters;
	}
	for (int ii = 0; ii < k; ii++)
		score += M[ii];
	delete[] M;
	//cout << "I return score= " << score << " for start postions:";
	//for (int ii = 0; ii <= i; ii++) cout << s[ii] << " ";
	//cout<<endl;
	return score;
}
string BaBMotifSearch(char **mas, int n, int m, int k)
{
	int row_pl = n - k + 1;
	string bestMotifs = "";
	int optimisticScore;
	int bestScore = 0;
	int *s = new int[m];
	for (int i = 0; i < n; i++)
		s[i] = 0;
	mypair pairS_i;
	pairS_i.a = s;
	pairS_i.number = 0;
	while (pairS_i.number >= 0)
	{
		if (pairS_i.number < m - 1)
		{
			optimisticScore = Score2(mas, pairS_i.number, k, pairS_i.a) + (m - pairS_i.number - 1)*n;    ////////????
			if (optimisticScore < bestScore)
				pairS_i = Bypass(pairS_i.a, pairS_i.number, n, row_pl);        /////// check it works?
			else pairS_i = NextVertex(pairS_i.a, pairS_i.number, m, row_pl);
		}
		else
		{
			int score = Score(mas, pairS_i.a, k, m);
			if (score >= bestScore)
			{
				bestScore = score;  //// ????
				int *bestS = new int[m];
				for (int ii = 0; ii < m; ii++)
					bestS[ii] = pairS_i.a[ii];
				bestMotifs = "";
				for (int ii = 0; ii < m; ii++)
				{
					for (int w = 0; w < k; w++)
						bestMotifs += mas[ii][bestS[ii] + w];
					bestMotifs += '\n';
				}
			}
			pairS_i = NextVertex(pairS_i.a, pairS_i.number, m, row_pl);
		}
	}

	return bestMotifs;
}


void main() {
	//считываем по строкам из файла  /////////

	string ss;
	ifstream fin("input.txt", ios_base::in);
	getline(fin, ss);
	int l1 = ss.length();
	int ii;
	for (ii = 0; ii < l1; ii++)
		if (ss[ii] == ' ') break;
	string kk = ss.substr(0, ii);
	string mm = ss.substr(ii + 1, l1);

	int k = stoi(kk);
	int m = stoi(mm);
	string *s = new string[m];
	for (int i = 0; i < m; i++)
	{
		getline(fin, s[i]);
	}
	int n = s[0].length();
	//////////////////////////////////////////
	char **dna = new char*[m];
	for (int i = 0; i < m; i++)
		dna[i] = new char[n];
	for (int i = 0; i < m; i++)
		for (int ii = 0; ii < n; ii++)
			dna[i][ii] = s[i][ii];
	for (int i = 0; i < m; i++)
	{
		for (int ii = 0; ii < n; ii++)
			cout << dna[i][ii] << " ";
		cout << endl;
	}
	cout << k << endl;
	cout << m << endl;

	string Result = "";
	//int l;
	Result = BaBMotifSearch(dna, n, m, k);

	cout << "Result: " << endl << Result << endl;

	//// записываем результат в файл   ////////////////////////////
	ofstream fout("output.txt", ios_base::out | ios_base::trunc);
	fout << Result;
	fout.close();

	delete[] s;


}