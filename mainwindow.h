#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>
#include <QButtonGroup>
#include "graphicsview.h"  // 添加图形视图头文件
#include <Eigen/Dense>
#include <QGraphicsLineItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //点数据
    QVector<QPointF> dataPoints;
    // 平差方法枚举
    enum AdjustmentMethod 
    {
        Y_OBSERVED,     // Y观测（X无误差）
        X_OBSERVED,     // X观测（Y无误差）  
        XY_OBSERVED     // XY都观测
    };
    AdjustmentMethod currentMethod;
    GraphicsView *graphicsView;  // 添加图形视图指针

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    //添加,删除,导入,导出,计算
    void on_pushButton_tianjia_clicked();
    void on_pushButton_shanchu_clicked();
    void on_pushButton_daoru_clicked();
    void on_pushButton_clicked();
    //计算前选择哪些值存在误差
    void onRadioButtonToggled();
    bool calculateYObserved(double& a, double& b);
    bool calculateXY1Observed(double& a, double& b);
    bool calculateXY2Observed(double& a, double& b);
    // 新增：视图控制按钮
    void on_zoomInButton_clicked();
    void on_zoomOutButton_clicked();
    void on_resetButton_clicked();
    // 导出为PNG
    bool exportToPng(const QString& fileName);
    

private slots:
    void on_pushButton_2_clicked();

private://函数
    void deleteSelectedPoints();            // 删除选中的点
    bool validateSelection();               // 验证选择是否有效
    void updateAfterDeletion();             // 删除后的更新操作

    // 导入相关方法
    bool importDataFromFile(const QString& fileName);
    void addDataToTable(double x, double y);
    void clearTable();
    void initializeSampleData();

    // 新增计算函数
    double calculateCorrelationCoefficient();
    double calculateSumSquaredErrors(double a, double b);

private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
