MPlotCurve
==========

MPlotCurve is a subclass of QwtPlotCurve

When you apply a subclass of QwtCurveFitter to a QwtPlotCurve you achieve a desirable specific fitting. However, the points, in cartesian coordinates, that describe the fitted are no readily available for use. Thus, if you want to compute the area below the curve or the centroid you must do additional work. Moreover, there is no ready way to visually compare the fitted with unfitted curve. The designer of Qwt carefully and rightfully designed QwtPlotCurve (as well as many other Qwt classes) to facilitate subclassing. The virtual functions and virtual destructor is testimony to this.

MPlotCurve is an abstract class derive via multiple inheritance from QObject and QwtPlotCurve. Applications must subclass MPlotCurve and implement the pure virtual function rawData(). 

Create a project and run it as follow: (a) As is (i.e. with certain code lines commented out); (b) Uncomment line 21 and run; (c) Uncomment line 28 and run; (d) Comment line 21, uncomment line 22 and run; ...get the picture... (e) Uncomment line 30 and 31 and run...setTranslate() receive, as argument, a QPointF tha represents the absolute translation.



