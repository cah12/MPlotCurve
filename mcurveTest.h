#ifndef MCURVETEST_H
#define MCURVETEST_H

#include "mplotcurve.h"

extern QVector<QPointF> g_vec;

class MCurve: public MPlotCurve
{
    Q_OBJECT
public:
    explicit MCurve(const QString &title="" , QObject *parent =0):MPlotCurve(title , parent)
    {        
    }

    QVector<QPointF> rawData()
    {
        return g_vec;
    }
private:
    QVector<QPointF>    _vec;
};

#endif // MCURVETEST_H
