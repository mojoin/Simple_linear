// graphicsview.cpp
#include "graphicsview.h"
#include <QPainter>
#include <QWheelEvent>
#include <QGraphicsScene>
#include <cmath>

GraphicsView::GraphicsView(QWidget* parent)
    : QGraphicsView(parent), hasRegressionLine(false), scaleFactor(1.0)
{
    // 创建场景
    QGraphicsScene* scene = new QGraphicsScene(this);
    setScene(scene);

    // 设置视图属性
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(AnchorUnderMouse);

    // 启用缓存提高性能
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);

    // 设置视口属性
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    viewport()->setAttribute(Qt::WA_OpaquePaintEvent, true);
    viewport()->setAttribute(Qt::WA_NoSystemBackground, true);

    // 设置初始视图中心
    viewCenter = QPointF(0, 0);

    setupCoordinateSystem();
}

QPointF GraphicsView::calculateDataCenter()
{
    if (dataPoints.isEmpty()) return QPointF(0, 0);

    QPointF sum(0, 0);
    for (const QPointF& point : dataPoints) {
        sum += point;
    }
    return sum / dataPoints.size();
}

void GraphicsView::setupCoordinateSystem()
{
    // 设置场景范围（以原点为中心）
    scene()->setSceneRect(-10000, -10000, 20000, 20000);
    centerOn(viewCenter);
}

void GraphicsView::setDataPoints(const QVector<QPointF> &points)
{
    //dataPoints = points;
    //因为实际图我们缩小了100倍，所以这里需要乘以100
    dataPoints.clear();
    int i=points.size();
    for (i = 0; i < points.size(); i++) {
        dataPoints.append(QPointF(points[i].x() * 100, - points[i].y() * 100));
    }
    scene()->update();
}

void GraphicsView::setRegressionLine(double a, double b, double r, double sse, int n)
{
    //regressionA = a;
    //regressionB = b;
    regressionA = -a;
    regressionB = -b * 100;
    hasRegressionLine = true;
    correlationR = r;
    sumSquaredErrors = sse;
    dataPointCount = n;
    scene()->update();
}

void GraphicsView::clearGraphics()
{
    dataPoints.clear();
    hasRegressionLine = false;
    scene()->update();
}

void GraphicsView::resetView()
{
    resetTransform();
    centerOn(0, 0);
    scaleFactor = 1.0;
}

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    // 绘制白色背景
    painter->fillRect(rect, Qt::white);

    // 绘制坐标系
    drawCoordinateSystem(painter);
}

void GraphicsView::drawForeground(QPainter *painter, const QRectF &rect)
{
    // 绘制数据点
    drawDataPoints(painter);

    // 绘制回归直线
    if (hasRegressionLine) {
        drawRegressionLine(painter);
    }
}

void GraphicsView::drawCoordinateSystem(QPainter *painter)
{
    painter->save();

    QPen gridPen(QColor(220, 220, 220), 1, Qt::DotLine);
    QPen axisPen(Qt::black, 2);

    QRectF sceneRect = scene()->sceneRect();

    // 绘制网格
    painter->setPen(gridPen);

    qreal left = std::floor(sceneRect.left() / 50) * 50;
    qreal right = std::ceil(sceneRect.right() / 50) * 50;
    qreal top = std::floor(sceneRect.top() / 50) * 50;
    qreal bottom = std::ceil(sceneRect.bottom() / 50) * 50;

    for (qreal x = left; x <= right; x += 50) {
        painter->drawLine(QPointF(x, sceneRect.top()), QPointF(x, sceneRect.bottom()));
    }
    for (qreal y = top; y <= bottom; y += 50) {
        painter->drawLine(QPointF(sceneRect.left(), y), QPointF(sceneRect.right(), y));
    }

    // 绘制坐标轴
    painter->setPen(axisPen);

    // X轴
    painter->drawLine(QPointF(sceneRect.left(), 0), QPointF(sceneRect.right(), 0));
    // Y轴
    painter->drawLine(QPointF(0, sceneRect.top()), QPointF(0, sceneRect.bottom()));

    // 绘制刻度
    QFont font = painter->font();
    font.setPointSize(8);
    painter->setFont(font);

    for (qreal x = left; x <= right; x += 50) {
        if (std::abs(x) > 25) { // 避免在原点处重叠
            painter->drawLine(QPointF(x, -5), QPointF(x, 5));
            painter->drawText(QRectF(x - 20, 10, 40, 20), Qt::AlignCenter, QString::number(x/100));
        }
    }

    for (qreal y = top; y <= bottom; y += 50) {
        if (std::abs(y) > 25) { // 避免在原点处重叠
            painter->drawLine(QPointF(-5, y), QPointF(5, y));
            painter->drawText(QRectF(-40, y - 10, 35, 20), Qt::AlignRight | Qt::AlignVCenter, QString::number(-y/100));
        }
    }

    // 绘制原点标记
    painter->drawText(QRectF(-30, -30, 60, 20), Qt::AlignCenter, "O(0,0)");

    painter->restore();
}

void GraphicsView::drawDataPoints(QPainter *painter)
{
    if (dataPoints.isEmpty()) return;

    painter->save();

    QPen pointPen(Qt::blue, 2);
    QBrush pointBrush(Qt::red);

    painter->setPen(pointPen);
    painter->setBrush(pointBrush);

    // 绘制数据点
    for (const QPointF &point : dataPoints) {
        painter->drawEllipse(point, 4, 4);

        // 显示坐标标签
        painter->drawText(point + QPointF(10, -10),
            QString("(%1, %2)").arg(point.x() / 100).arg(-point.y() / 100));
    }

    painter->restore();
}

void GraphicsView::drawRegressionLine(QPainter *painter)
{
    painter->save();

    QPen linePen(Qt::red, 2);
    painter->setPen(linePen);

    QRectF sceneRect = scene()->sceneRect();

    // 计算直线与场景边界的交点
    qreal x1 = sceneRect.left();
    qreal y1 = regressionA * x1 + regressionB;
    qreal x2 = sceneRect.right();
    qreal y2 = regressionA * x2 + regressionB;

    // 如果直线超出y范围，重新计算交点
    if (y1 < sceneRect.top() || y1 > sceneRect.bottom() ||
        y2 < sceneRect.top() || y2 > sceneRect.bottom()) {

        qreal y1_top = sceneRect.top();
        qreal x1_top = (y1_top - regressionB) / regressionA;

        qreal y2_bottom = sceneRect.bottom();
        qreal x2_bottom = (y2_bottom - regressionB) / regressionA;

        painter->drawLine(QPointF(x1_top, y1_top), QPointF(x2_bottom, y2_bottom));
    } else {
        painter->drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }

    // 计算数据点中心位置
    QPointF dataCenter = calculateDataCenter();

    // 设置文本字体和颜色
    QFont font = painter->font();
    font.setPointSize(10);
    font.setBold(true);
    painter->setFont(font);
    painter->setPen(Qt::darkBlue);

    // 在数据点中心下方显示回归方程
    QPointF textStartPos = dataCenter + QPointF(0, 80); // 中心点下方80像素

    // 显示回归方程
    QString equation = QString("回归方程: y = %1x + %2")
        .arg(-regressionA, 0, 'f', 4)
        .arg(-regressionB / 100, 0, 'f', 4);
    painter->drawText(textStartPos, equation);
    //QString equation = QString("y = %1x + %2").arg(regressionA, 0, 'f', 3).arg(-regressionB / 100, 0, 'f', 3);
    //painter->drawText(QPointF(50, 50), equation);

    // 设置较小字体显示其他参数
    font.setPointSize(9);
    font.setBold(false);
    painter->setFont(font);
    painter->setPen(Qt::darkGreen);

    // 显示其他相关参数
    QPointF paramPos = textStartPos + QPointF(0, 25);

    QString paramsText = QString("数据点数: %1\n相关系数: %2\n残差平方和: %3")
        .arg(dataPointCount)
        .arg(correlationR, 0, 'f', 4)
        .arg(sumSquaredErrors, 0, 'f', 4);

    // 绘制多行文本
    QStringList lines = paramsText.split('\n');
    for (int i = 0; i < lines.size(); ++i) {
        painter->drawText(paramPos + QPointF(0, i * 20), lines[i]);
    }

    painter->restore();
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    // 缩放控制
    double factor = (event->angleDelta().y() > 0) ? 1.1 : 0.9;
    scale(factor, factor);
    scaleFactor *= factor;

    event->accept();
}
