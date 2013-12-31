#include <QApplication>
#include <qwt_plot.h>
#include <qwt_symbol.h>
#include "mcurveTest.h"


QVector<QPointF> g_vec;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    g_vec << QPointF(0, 0) << QPointF(1, 4.5) << QPointF(2,4.5) << QPointF(3,9);
    QwtPlot p;
    p.resize(QSize(500, 400));
    MCurve curve("My Curve");
    curve.setRenderHint( QwtPlotItem::RenderAntialiased, true );
    curve.attach(&p);
    curve.setSamples(g_vec);

    //curve.setFitingType(MPlotCurve::Cubic_Spline_Natural_Fitting);
    //curve.setFitingType(MPlotCurve::Cubic_Spline_Periodic_Fitting);
    //curve.setFitingType(MPlotCurve::Polynomial_Fitting);
    //curve.setFitingType(MPlotCurve::Linear_Regression_Fitting);

    curve.setSymbol( new QwtSymbol( QwtSymbol::Ellipse,  QBrush(Qt::red), QPen( Qt::red ), QSize( 10, 10 ) ) );

    //curve.setSuperimposed(false); //Only when one of the above Fitting is set, will see the effect of this line setSuperimposed(false) be seen.

    //curve.setTranslate(QPointF(20.0, 30.0)); //translation is absolute. Two successive calls is equivalent to the last call.
    //curve.setTranslate(QPointF(28.0, 38.0));
    
    //QPolygonF poly(dynamic_cast<QwtPointSeriesData *>(curve.data())->samples ());

    p.show();
    return a.exec();
}
