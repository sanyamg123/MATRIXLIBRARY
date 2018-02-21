#include<iostream>
#include"../header_files/MATRIX.hpp"
using namespace std;
int main()
{
	// #ifndef ONLINE_JUDGE
 //    freopen("input.txt","r",stdin);
 //    freopen("output2.txt","w",stdout);
 //    #endif
	Matrix<int>  m0(2,2);
	int val;
	for ( int i = 0; i < 2; i ++)
		for ( int j = 0; j < 2; j ++)
			m0.putElement(i,j,1);

	Matrix< int > m1 (2,2);
	for ( int i = 0; i < 2; i ++)
		for ( int j = 0; j < 2; j ++)
			m1.putElement(i,j,2);

	Matrix< int > m2 (2,2);
	m2 = m0+m1+m1;
	for ( int i = 0; i < 2; i ++)
	{
		for ( int j = 0; j < 2; j ++)
			cout <<  m2.getElement(i,j) << " " ;
		cout << endl;
	}
}