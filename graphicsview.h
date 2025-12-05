#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H


#include <QGraphicsView>
#include <QPointF>
#include <QVector>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphicsView(QWidget *parent = nullptr);
    void setDataPoints(const QVector<QPointF> &points);
    void setRegressionLine(double a, double b ,double r = 0, double sse = 0, int n = 0); // 直线方程 y = ax + b
    void clearGraphics();
    void resetView();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void setupCoordinateSystem();
    void drawCoordinateSystem(QPainter *painter);
    void drawDataPoints(QPainter *painter);
    void drawRegressionLine(QPainter *painter);
    QPointF calculateDataCenter(); // 新增：计算数据点中心

    QVector<QPointF> dataPoints;
    double regressionA, regressionB;
    bool hasRegressionLine;
    qreal scaleFactor;
    QPointF viewCenter;

    // 新增：相关参数
    double correlationR;
    double sumSquaredErrors;
    int dataPointCount;
};

#endif // GRAPHICSVIEW_H
