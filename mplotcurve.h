#ifndef MPLOTCURVE_H
#define MPLOTCURVE_H

#include "qwt_plot_curve.h"
#include "qwt_spline.h"

class  MCurveFitter_1;


class MPlotCurve : public QObject, public QwtPlotCurve
{
    Q_OBJECT

    Q_PROPERTY(QPen Pen READ pen WRITE setPen)
    Q_PROPERTY(QPointF Translate READ translate WRITE setTranslate)   
    Q_PROPERTY(FitingType Fiting_Type READ fitingType WRITE setFitingType)
    Q_PROPERTY(bool Superimposed READ isSuperimposed WRITE setSuperimposed)

    Q_ENUMS(FitingType)

public:
    enum FitingType {No_Fitting, Cubic_Spline_Natural_Fitting,
                     Cubic_Spline_Periodic_Fitting,
                     Polynomial_Fitting,
                    Linear_Regression_Fitting};

    explicit MPlotCurve( const QString &title = QString::null  , QObject *parent=0);
    explicit MPlotCurve( const QwtText &title , QObject *parent=0 );

    virtual QVector<QPointF> rawData()=0;

    FitingType fitingType() const{return _fitingType;}
    virtual bool setFitingType(FitingType type);

    void setVisible( bool on);
    void itemChanged();
    //Hide the base class method setSymbol ()
    void setSymbol  ( QwtSymbol *  symbol );

    QPointF translate()const{return _translate;}
    void setTranslate(const QPointF & offset);

    bool isSuperimposed()const{return _superimposed;}
    void setSuperimposed(bool on);

signals:   
    void translated(QwtPlotCurve*);
    void superimposedChanged(bool);
    void fitingTypeChanged(MPlotCurve::FitingType prevType, MPlotCurve::FitingType newType);
public slots:


protected:
    void initPlotCurve();
    QString fittingTypeToCurveFitterId(FitingType type);
    QwtSymbol * symbolCopy(const QwtSymbol * sym);
    QPen primaryCurvePen();
    void buildAndAttchPrimaryCurve();
    void removePrimaryCurve();
    void doSetFitingType(FitingType type);
    void resetFitingType();

    QPointF             _translate;
    FitingType          _fitingType;
    bool                _superimposed;
    QwtPlotCurve*       _primaryCurve;
    FitingType          _prevType;
    QwtSymbol *         _currentSymbol;
    MCurveFitter_1*     _fitter;

};

#endif // MPLOTCURVE_H
