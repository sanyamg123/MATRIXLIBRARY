// EXPRESSION TEMPLATES IN C++ AND GENERIC LAMBDAS

/* NAME : SANYAM GARG
   INSTITUTION : DELHI TECHNOLOGICAL UNIVERSITY 
   Implemented Matrix Library which can compute algebraic expressions
   including + and *, += and *= involving matrices.  
*/
#ifndef _MATRIX_HPP
#define _MATRIX_HPP

#include <vector>
#include <cassert>
#include <iostream> 

using namespace std;

// #define NDEBUG

/* 
	To assert in case of errors in matrix 
	multiplication or addition
*/

#ifndef NDEBUG
#define ASSERT_EX(condition, statement) \
    do { \
        if (!(condition)) { statement; assert(condition); } \
    } while (false)
#else
#define ASSERT_EX(condition, statement) ((void)0)
#endif


template < typename ExpType > 
class Expression{
	public:
	inline const ExpType& self(void) const{
		return *static_cast<const ExpType*>(this);
	}
};

struct mul{
	template< typename dt>
	inline static vector<vector<dt> >  Map ( vector<vector<dt> > a,vector<vector<dt> >  b )
	{
		auto MUL = [](auto a, auto b)
		{
			ASSERT_EX(a[0].size()==b.size(),"Incompatible matrix multiplication");
			vector<vector<dt> >  m;
			m.resize(a.size());
			for ( int i = 0; i < a.size(); i ++)
			{
				m[i].resize(b[0].size());
				for ( int j = 0; j < b[0].size(); j ++ )
				{
					for ( int k = 0; k < a[0].size(); k ++)
					{
						m[i][j] = m[i][j] + (a[i][k]*b[k][j]);
					}
				}
			}
			return m;
		};
		return MUL(a,b);
	} 
};


struct add{
	template < typename dt> 
	inline static vector<vector<dt> >  Map ( vector<vector<dt> > a, vector<vector<dt> >  b )
	{
		auto ADD = [](auto a, auto b)
		{
			ASSERT_EX(a.size()==b.size() and a[0].size() == b[0].size(),"Incompatible matrix addition");
			vector<vector<dt> >  m;
			m.resize(a.size());
			for ( int i = 0; i < a.size(); i ++)
			{
				m[i].resize(a[0].size());
				for ( int j = 0; j < a[0].size(); j ++ )
				{
					m[i][j] = a[i][j] + b[i][j];
				}
			}
			return m;
		};
		return ADD(a,b);
	}	 
};

template <typename OP, typename LHS, typename RHS > 
struct BinaryMapExp : public Expression<BinaryMapExp<OP,LHS,RHS> >{
	const LHS& lhs;
	const RHS& rhs;
	BinaryMapExp( const LHS& l, const RHS& r)
		: lhs(l),rhs(r){}
	inline auto EVAL ( )
	const
	{
		return OP::Map(lhs.EVAL(),rhs.EVAL());
	}
};

template < typename dt > 
struct Matrix: public Expression<Matrix<dt> > {
	private:

	vector < vector < dt > > mat;
	size_t r,c;

	public:

	Matrix()
	{
		r =0 , c = 0;
	}
	Matrix(size_t rr, size_t cc)
	{
		r=rr , c=cc;
		mat.resize(r,vector < dt >(c));
	} 
	Matrix ( Matrix& M )
	{
		// update r and c
		r = M.rowsize();
		c = M.colsize();
		mat.resize(r,vector < dt >(c));
		for ( unsigned int i = 0; i < r; i ++ )
			for ( unsigned int j = 0; j < c; j ++)
				mat[i][j] = M.getElement(i,j);

	}
	size_t rowsize() const{
		return r;
	};
	size_t colsize() const{
		return c;
	};
	dt getElement(int x, int y ) const{
		return mat[x][y];	
	};
	void putElement(int x, int y, dt val)
	{
		ASSERT_EX ( x < r and x >=0  and y < c and y >= 0,
		std :: cerr << " Addition of a value to a non existing location" );
		mat[x][y] = val;
	}
	
	template < typename EType > 
	inline Matrix<dt>& operator=(const Expression<EType>& s_)
	{
		const EType &s = s_.self();
		// BUGG
		mat = s.EVAL();
		r = mat.size();
		c = mat[0].size();
		return (*this);
	} 

	template < typename EType > 
	inline Matrix<dt>& operator *=(const Expression<EType>& s_)
	{
		// dimensions of the matrix will change.
		const EType &s = s_.self();
		vector<vector<dt> > ss = s.EVAL();
		ASSERT_EX(c==ss.size(),cerr << "Incompatible multiplication amongst matrices.");
		vector<vector<dt> > m;
		m.resize(r);
		dt ret;
		for ( int i = 0; i < r; i ++)
		{
			m[i].resize(ss[0].size());
			for ( int j = 0; j < ss[0].size(); j ++)
			{
				ret = 0;
				for ( int k = 0; k < c; k ++)
				{
					ret += (mat[i][k]*ss[k][j]);
				}
				m[i][j] = ret;
			}
		}
		for ( int i = 0; i < r; i ++ )
		{
			mat[i].resize(ss[0].size());
			for ( int j = 0; j < ss.size(); j ++)
			{
				mat[i][j] = m[i][j];
			}
		}
		c = ss[0].size();
		return (*this);
	} 

	template < typename EType > 
	inline Matrix<dt>& operator +=(const Expression<EType>& s_)
	{
		const EType &s = s_.self();
		vector<vector<dt> > ss = s.EVAL();
		ASSERT_EX(r==ss.size() and c == ss[0].size(),"Incompatible addition amongst matrices.");
		for ( int i = 0; i < r; i ++)
		{
			for ( int j = 0; j < c; j ++)
			{
				mat[i][j] = mat[i][j]+ss[i][j];
			}
		}
		return (*this);
	} 
	inline vector<vector<dt> >  EVAL( )
	const
	{
		return mat;
	}
	
};

template <typename OP, typename LHS, typename RHS>
inline BinaryMapExp<OP,LHS,RHS>
operator_load(const Expression<LHS> &lhs,const Expression<RHS> &rhs)
{
	return BinaryMapExp<OP,LHS,RHS>(lhs.self(),rhs.self());
} 

template <typename LHS, typename RHS>
inline BinaryMapExp<mul,LHS,RHS>
operator *(const Expression<LHS> &lhs,const Expression<RHS> &rhs)
{
	return operator_load<mul>(lhs,rhs);
}

template <typename LHS, typename RHS>
inline BinaryMapExp<add,LHS,RHS>
operator +(const Expression<LHS> &lhs,const Expression<RHS> &rhs)
{
	return operator_load<add>(lhs,rhs);
}

#endif