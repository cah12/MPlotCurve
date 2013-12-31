#ifndef POLYNOMIALFIT_H
#define POLYNOMIALFIT_H

#include <QObject>
#include <QVector>
#include <QString>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>

namespace ublas = boost::numeric::ublas;

class PolynomialFit
{
public:
    PolynomialFit(QVector<QPointF> & points)
    {
        _numOfKnownPoints = points.size();
        if(_numOfKnownPoints < 2)
            return;
        _B.resize(_numOfKnownPoints, 1);
        _X.resize(_numOfKnownPoints, 1);
        _A.resize(_numOfKnownPoints, _numOfKnownPoints);

        //Initialize B matrix
        for(int i=0; i < _B.size1(); ++i)
            _B(i,0) = points.at(i).y();

        //Initialize A matrix
        for(int i=0; i<_A.size1(); ++i)
            for(int j=0; j<_A.size2(); ++j)
                _A(i,j) = pow(points.at(i).x(), j);

        ublas::matrix<double> inverse(_numOfKnownPoints,_numOfKnownPoints);
        invertMatrix (_A, inverse);

        _X = ublas::prod(inverse, _B);

        for(int i=0; i<_numOfKnownPoints; ++i)
        {
            if(_X(i,0) != 0)
            {
                if(i==0)
                {
                    _fx += QString::number(_X(i,0));
                }
                else
                {
                     if(_X(i,0) > 0)
                     {
                        _fx += "+";
                     }
                     _fx += QString::number(_X(i,0));
                     _fx +=  "*X";
                     if(i > 1)
                     {
                         _fx +=  "^";
                         _fx +=  QString::number(i);
                     }
                }
            }
        }

    }

    double evalFx(double x)const
    {
        double result = 0;
        for(int i=0; i < _numOfKnownPoints; ++i)
        {
            result += (pow(x, i)*_X(i,0));
        }
        return result;
    }

    bool solve(double minX, double maxX, double stepX)
    {
        while(minX <= maxX)
        {
            _pointPairs.push_back(QPointF(minX, evalFx(minX)));
            minX += stepX;
        }
        if(_pointPairs.last().x() < maxX)
            _pointPairs.push_back(QPointF(maxX, evalFx(maxX)));
        if(_pointPairs.size() > 2)
            return true;
        return false;
    }

    QString fx()const{return _fx;}

    QVector<QPointF> pointPairs()const{return _pointPairs;}

private:
    template<class T>
    bool invertMatrix (const ublas::matrix<T>& input, ublas::matrix<T>& inverse);

    int                     _numOfKnownPoints;
    ublas::matrix<double>   _B;
    ublas::matrix<double>   _X;
    ublas::matrix<double>   _A;
    QString                 _fx;

    QVector<QPointF>        _pointPairs;

};

/* Matrix inversion routine.
    Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */
 template<class T>
 bool PolynomialFit::invertMatrix (const ublas::matrix<T>& input, ublas::matrix<T>& inverse)
 {
    using namespace boost::numeric::ublas;
    typedef permutation_matrix<std::size_t> pmatrix;
    // create a working copy of the input
    matrix<T> A(input);
    // create a permutation matrix for the LU-factorization
    pmatrix pm(A.size1());

    // perform LU-factorization
    int res = lu_factorize(A,pm);
        if( res != 0 ) return false;

    // create identity matrix of "inverse"
    inverse.assign(ublas::identity_matrix<T>(A.size1()));

    // backsubstitute to get the inverse
    lu_substitute(A, pm, inverse);
    return true;
 }

#endif // POLYNOMIALFIT_H
