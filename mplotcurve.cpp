#include "mplotcurve.h"
#include "qwt_plot.h"
#include "qwt_symbol.h"
#include "mcurvefitter.h"


MPlotCurve::MPlotCurve( const QString &title , QObject *parent):QObject(parent), QwtPlotCurve(title),
    _primaryCurve(0),_superimposed(true), _fitingType(No_Fitting), _prevType(No_Fitting), _currentSymbol(new QwtSymbol()), _fitter(new MCurveFitter_4)
{
    initPlotCurve();
}

MPlotCurve::MPlotCurve( const QwtText &title, QObject *parent ):QObject(parent), QwtPlotCurve(title),
    _primaryCurve(0),_superimposed(true), _fitingType(No_Fitting), _prevType(No_Fitting), _currentSymbol(new QwtSymbol()), _fitter(new MCurveFitter_4)
{
    initPlotCurve();
}

void MPlotCurve::initPlotCurve()
{
    ;
}

void MPlotCurve::setTranslate(const QPointF & offset)
{
    if(offset == _translate)
        return;
    QPointF pt(offset - _translate);
    QPolygonF poly(dynamic_cast<QwtPointSeriesData *>(this->data())->samples ());
    poly.translate(pt);
    this->setSamples(poly);
    _translate = offset;

    if(_primaryCurve)
    {
        QPolygonF poly(dynamic_cast<QwtPointSeriesData *>(_primaryCurve->data())->samples ());
        poly.translate(pt);
        _primaryCurve->setSamples(poly);
    }
    emit translated(this);
    this->plot()->replot();
}

QPen MPlotCurve::primaryCurvePen()
{
    if(!_superimposed)
        return QPen(Qt::NoPen);
    QPen p = this->pen();
    if(p.style() != Qt::DashLine)
        p.setStyle(Qt::DashLine);
    else
        p.setStyle(Qt::SolidLine);
    return p;
}

void MPlotCurve::setSuperimposed(bool on)
{
    if(on == _superimposed)
        return;
    _superimposed = on;
    if(_primaryCurve)
    {
        if(on == false)
        {
            _primaryCurve->setPen(QPen(Qt::NoPen));
        }
        else
        {
            _primaryCurve->setPen(primaryCurvePen());
        }
        this->plot()->replot();
    }
    emit superimposedChanged(_superimposed);
}

void MPlotCurve::setSymbol ( QwtSymbol *  symbol )
{
    if(_primaryCurve)
    {
       _primaryCurve->QwtPlotCurve::setSymbol(symbol);
        this->QwtPlotCurve::setSymbol(new QwtSymbol()); //Remove the symbols that acts as point markers
    }
    else
        QwtPlotCurve::setSymbol(symbol);
}

QwtSymbol * MPlotCurve::symbolCopy(const QwtSymbol * sym)
{
    if(sym)
    {
        return new QwtSymbol( sym->style(), sym->brush(), sym->pen(), sym->size());
    }
    return 0;
}

void MPlotCurve::buildAndAttchPrimaryCurve()
{
    if(!_primaryCurve)
    {
        _primaryCurve = new QwtPlotCurve;
        _primaryCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        _primaryCurve->setItemAttribute( QwtPlotItem::Legend, false);
        _primaryCurve->attach(this->plot());
        QPolygonF poly(rawData());
        poly.translate(_translate);
        _primaryCurve->setSamples(poly);
        QwtSymbol * sym = symbolCopy(this->symbol());
        _primaryCurve->setSymbol(sym);
        this->QwtPlotCurve::setSymbol(new QwtSymbol()); //Remove the symbols from the fitted curve
        if(_superimposed)
            _primaryCurve->setPen(primaryCurvePen());
        else
            _primaryCurve->setPen(QPen(Qt::NoPen));
    }
}

void MPlotCurve::removePrimaryCurve()
{
    if(_primaryCurve)
        delete _primaryCurve;
    _primaryCurve = 0;
}

void MPlotCurve::resetFitingType()
{
    QPolygonF poly(rawData());
    poly.translate(_translate);
    _primaryCurve->setSamples(poly);
    doSetFitingType(_fitingType);
}


void  MPlotCurve::itemChanged()
{
    if(_fitingType != No_Fitting && this->plot())
    {
        if(_primaryCurve)
        {
            //Check that pens are ok
            if(this->primaryCurvePen() != _primaryCurve->pen())
            {
                _primaryCurve->setPen(this->primaryCurvePen());
                this->plot()->replot();
            }
         }
    }
    QwtPlotCurve::itemChanged();
}


void MPlotCurve::setVisible( bool on)
{
    if(_primaryCurve)
        _primaryCurve->setVisible( on );
    QwtPlotItem::setVisible(on);
}

bool MPlotCurve::setFitingType(FitingType type)
{
    if(_fitingType==type /* No change ordered */|| !this->plot()/*Ensure a valid plot is attached*/ )
        return false;
    _prevType = _fitingType;
    doSetFitingType(type);
    emit fitingTypeChanged(_prevType, _fitingType);
    return true;
}

QString MPlotCurve::fittingTypeToCurveFitterId(FitingType type)
{
    if(type == Cubic_Spline_Natural_Fitting)
        return "NaturalSpline";
    else if(type == Cubic_Spline_Periodic_Fitting)
        return "PeriodicSpline";
    else if(type == Linear_Regression_Fitting)
        return "Regression";
    else if(type == Polynomial_Fitting)
        return "Polynomial";
    return "";
}

void MPlotCurve::doSetFitingType(FitingType type)
{
   if(type == No_Fitting)
    {
        QPolygonF poly(rawData());
        poly.translate(_translate);
        this->setSamples(poly);
        _fitingType=type;
        QwtSymbol * sym = symbolCopy(_primaryCurve->symbol());
        removePrimaryCurve();
        this->QwtPlotCurve::setSymbol(sym);
        this->plot()->replot();
        return;
    }
    buildAndAttchPrimaryCurve();
    QPolygonF polyPrimaryCurve(dynamic_cast<QwtPointSeriesData *>(_primaryCurve->data())->samples ());
    polyPrimaryCurve.translate(_translate);
    QPolygonF poly (_fitter->fit(rawData(), fittingTypeToCurveFitterId(type)));
    poly.translate(_translate);
    this->setSamples(poly);
    _fitingType=type;
    this->plot()->replot();

}
