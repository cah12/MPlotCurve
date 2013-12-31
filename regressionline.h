#ifndef REGRESSIONLINE_H
#define REGRESSIONLINE_H

#include <QVector>
#include <QString>
#include <QPointF>
#include <map>

class RegressionLine
{
    public:

        typedef std::map<double, double> Points;

        //RegressionLine(QVector<QPointF> & pts);
        RegressionLine::RegressionLine(QVector<QPointF> & pts)
        {
            RegressionLine::Points points;

            for (int i = 0; i < pts.size(); i++)
            {
                points[pts[i].x()] =pts[i].y();
            }

            int n = points.size();
            if (n < 2)
                throw (QString("Must have at least two points"));


            double sumx=0,sumy=0,sumx2=0,sumy2=0,sumxy=0;
            double sxx,syy,sxy;

            // Conpute some things we need
            std::map<double, double>::const_iterator i;
            for (i = points.begin(); i != points.end(); i++)
            {
                double x = i->first;
                double y = i->second;

                sumx += x;
                sumy += y;
                sumx2 += (x * x);
                sumy2 += (y * y);
                sumxy += (x * y);
            }
            sxx = sumx2 - (sumx * sumx / n);
            syy = sumy2 - (sumy * sumy / n);
            sxy = sumxy - (sumx * sumy / n);

            // Infinite slope_, non existant yIntercept
            if (abs(sxx) == 0)
                throw (QString("Inifinite Slope"));

            // Calculate the slope_ and yIntercept
            slope_ = sxy / sxx;
            yIntercept_ = sumy / n - slope_ * sumx / n;

            // Compute the regression coefficient
            if (abs(syy) == 0)
                regressionCoefficient_ = 1;
            else
                regressionCoefficient_ = sxy / sqrt(sxx * syy);

            if(yIntercept_ !=0)
            {
                _fx += QString::number(yIntercept_);
            }
            if(slope_  !=0)
            {
                if(yIntercept_ !=0 && slope_ > 0)
                    _fx += "+";
                _fx += QString::number(slope_);
                _fx += "*";
                _fx += "x";
            }
        }



        virtual ~RegressionLine(){}

        const double slope() const;
        const double yIntercept() const;
        const double regressionCoefficient() const;

        QString function(){return _fx;}
        QVector<QPointF> pointPairs(){return _pointPairs;}

        bool solve(double minX, double maxX)
        {
            if(minX <= maxX)
            {
                _pointPairs.push_back(QPointF(minX, evalFx(minX)));
                _pointPairs.push_back(QPointF(maxX, evalFx(maxX)));
                return true;
            }            
            return false;
        }

    private:
        double evalFx(double x)
        {
            return yIntercept_ + x*slope_;
        }

        double slope_;
        double yIntercept_;
        double regressionCoefficient_;

        QString _fx;
        QVector<QPointF> _pointPairs;

};
