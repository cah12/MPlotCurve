MPlotCurve
==========

MPlotCurve is a subclass of QwtPlotCurve

When you apply a subclass of QwtCurveFitter to a QwtPlotCurve you achieve a desirable specific fitting. However, the points, in cartesian coordinates, that describe the fitted are no readily available for use. Thus, if you want to compute the area below the curve or the centroid you must do additional work. Moreover, there is no ready way to visually compare the fitted with unfitted curve. The designer of Qwt carefully and rightfully designed QwtPlotCurve (as well as many other Qwt classes) to facilitate subclassing. The virtual functions and virtual destructor is testimony to this.

MPlotCurve is an abstract class derive via multiple inheritance from QObject and QwtPlotCurve. Applications must subclass MPlotCurve and implement the pure virtual function rawData(). 

Create a project and run it as follow:-
    a) As is (i.e. with certain code lines commented out)
    b)



