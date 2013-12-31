#ifndef MCURVEFITTER_H
#define MCURVEFITTER_H

#include <qwt_spline.h>
#include <QString>
#include "RegressionLine.h"
#include "polynomialfit.h"

class QPolygon;

class MAbstractCurveFitter
{    
public:
    MAbstractCurveFitter(){}
    virtual QPolygonF fit(const QPolygonF & points, const QString & id) = 0;
    virtual bool isFitterTypeSupported(const QString & id) const =0;
};

class MCurveFitter_1 : public MAbstractCurveFitter //NaturalSpline
{
public:
    MCurveFitter_1(){}
    virtual QPolygonF  fit(const QPolygonF & points, const QString & id)
    {
        if(id == fitterType())
        {
           return fitSpline( points, QwtSpline::Natural);
        }
        return points;
    }

    virtual bool isFitterTypeSupported(const QString & id)const
    {
        if(id == "NaturalSpline")
            return true;
        return false;
    }

    QString fitterType()const
    {
        return "NaturalSpline";
    }
protected:\
   QPolygonF  fitSpline(const QPolygonF & points, QwtSpline::SplineType type)
   {
       int numValues = 250;
        QwtSpline spline;
        spline.setSplineType(type);
        if ( !spline.setPoints(points) )
            return points;
        QPolygonF interpolatedPoints(numValues);
        const double delta =  (points[points.size() - 1].x() - points[0].x()) / (numValues-1);
        for(int i = 0; i < numValues; i++)  // interpolate
        {
            const double x = points[0].x() + i * delta;
            interpolatedPoints[i].setX(x);
            interpolatedPoints[i].setY(spline.value(x));
        }
        return interpolatedPoints;
   }
};


class MCurveFitter_2 : public MCurveFitter_1 //PeriodicSpline
{
public:
    MCurveFitter_2(){}
    virtual QPolygonF  fit(const QPolygonF & points, const QString & id)
    {
        if(id == fitterType())
        {
            return fitSpline( points, QwtSpline::Periodic);
        }
        return MCurveFitter_1::fit(points, id);
    }

    virtual bool isFitterTypeSupported(const QString & id)const
    {
        if(id == "PeriodicSpline")
            return true;
        return MCurveFitter_1::isFitterTypeSupported(id);
    }

    QString fitterType()const
    {
        return "PeriodicSpline";
    }
};

class MCurveFitter_3 : public MCurveFitter_2 //Regression
{
public:
    MCurveFitter_3(){}
    virtual QPolygonF  fit(const QPolygonF & points, const QString & id)
    {
        if(id == fitterType())
        {
            return regress(points);
        }
        return MCurveFitter_2::fit(points, id);
    }

    virtual bool isFitterTypeSupported(const QString & id)const
    {
        if(id == "Regression")
            return true;
        return MCurveFitter_2::isFitterTypeSupported(id);
    }

    QString fitterType()const
    {
        return "Regression";
    }
protected:
    QPolygonF regress(QPolygonF  points)
    {
        RegressionLine rl(points);
        if(rl.solve(points[0].x(), points[points.size()-1].x()))
            return rl.pointPairs();
        return points;
    }
};

class MCurveFitter_4 : public MCurveFitter_3 //Polynomial
{
public:
    MCurveFitter_4(){}
    virtual QPolygonF  fit(const QPolygonF & points, const QString & id)
    {
        if(id == fitterType())
        {
            return  polyFit(points);
        }
        return MCurveFitter_3::fit(points, id);
    }

    virtual bool isFitterTypeSupported(const QString & id)const
    {
        if(id == "Polynomial")
            return true;
        return MCurveFitter_3::isFitterTypeSupported(id);
    }

    QString fitterType()const
    {
        return "Polynomial";
    }
protected:
    QPolygonF polyFit(QPolygonF points)
    {
        PolynomialFit pf(points);
        double xStart = points[0].x();
        double xEnd = points[points.size()-1].x();
        if(pf.solve(xStart, xEnd, (xEnd - xStart) / 250))
            return pf.pointPairs();
        return points;
    }
};


#endif // MCURVEFITTER_H
