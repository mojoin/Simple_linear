#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化图形视图
    graphicsView = new GraphicsView(this);
    // 替换UI中的QGraphicsView
    QLayout *layout = ui->graphicsView->parentWidget()->layout();
    if (layout) {
        // 找到原来的graphicsView在布局中的位置
        int index = layout->indexOf(ui->graphicsView);
        if (index != -1) {
            // 移除原来的QGraphicsView
            QLayoutItem *item = layout->takeAt(index);
            delete item->widget();
            delete item;

            // 添加自定义的GraphicsView
            layout->addWidget(graphicsView);
        }
    } else {
        // 如果没有布局，直接替换
        ui->graphicsView->setParent(nullptr);
        delete ui->graphicsView;
        ui->graphicsView = nullptr;
        setCentralWidget(graphicsView);
    }

    // 连接视图控制按钮
    connect(ui->pushButton_fangda, &QPushButton::clicked, this, &MainWindow::on_zoomInButton_clicked);
    connect(ui->pushButton_suoxiao, &QPushButton::clicked, this, &MainWindow::on_zoomOutButton_clicked);
    connect(ui->pushButton_chongzhi, &QPushButton::clicked, this, &MainWindow::on_resetButton_clicked);

    //让表格列占满整个控件
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //构造函数声明,默认XY都存在误差
    currentMethod = XY_OBSERVED; 
    ui->radioButton_xy2->setChecked(true);

    // 连接单选按钮信号
    connect(ui->radioButton_x, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(ui->radioButton_xy1, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
    connect(ui->radioButton_xy1, &QRadioButton::toggled, this, &MainWindow::onRadioButtonToggled);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 视图控制按钮实现
void MainWindow::on_zoomInButton_clicked()
{
    graphicsView->scale(1.2, 1.2);
}

void MainWindow::on_zoomOutButton_clicked()
{
    graphicsView->scale(0.8, 0.8);
}

void MainWindow::on_resetButton_clicked()
{
    graphicsView->resetView();
}

void MainWindow::on_pushButton_shanchu_clicked()
{
    // 验证是否有选中的行
    if (!validateSelection()) {
        return;
    }

    // 执行删除操作
    deleteSelectedPoints();

    // 更新界面状态
    updateAfterDeletion();
}
// 验证是否有选中的行
bool MainWindow::validateSelection()
{
    QModelIndexList selectedIndexes = ui->tableWidget->selectionModel()->selectedIndexes();

    if (selectedIndexes.isEmpty()) 
    {
        // 没有选中任何行，显示提示
        QMessageBox::information(this,
            "提示",
            "请先选择要删除的数据点。\n\n点击表格中的行即可选中。",
            QMessageBox::Ok);
        return false;
    }
    return true;
}

void MainWindow::deleteSelectedPoints()
{
    // 获取所有选中的行（去重，因为选中单元格会返回多个索引）
    QSet<int> selectedRows;
    QModelIndexList selectedIndexes = ui->tableWidget->selectionModel()->selectedIndexes();

    for (const QModelIndex& index : selectedIndexes) 
    {
        selectedRows.insert(index.row());
    }

    // 从后往前删除，避免行索引变化导致的问题
    QList<int> rowsToDelete = selectedRows.values();
    // 倒序
    std::sort(rowsToDelete.begin(), rowsToDelete.end(), std::greater<int>());

    int deletedCount = 0;

    for (int row : rowsToDelete) 
    {
        if (row >= 0 && row < ui->tableWidget->rowCount()) 
        {
            // 从表格中删除行
            ui->tableWidget->removeRow(row);

            // 从数据点数组中删除对应点
            if (row < dataPoints.size()) 
            {
                dataPoints.removeAt(row);
            }

            deletedCount++;
        }
    }

    // 显示操作结果
    if (deletedCount > 0) {
        QString message = QString("成功删除 %1 个数据点").arg(deletedCount);
        if (ui->tableWidget->rowCount() > 0) {
            message += QString("，剩余 %1 个点").arg(ui->tableWidget->rowCount());
        }
        else {
            message += "，表格已空";
        }

        // 在状态栏显示短暂提示
        statusBar()->showMessage(message, 3000);
    }
}

void MainWindow::updateAfterDeletion()
{
    // 如果删除了所有点，清空图形显示
    if (ui->tableWidget->rowCount() <= 0) {
    //    clearGraphicsView();
        ui->label->setText("拟合结果：等待计算...");
        return;
    }

    // 如果有拟合结果，重新计算并更新显示
    if (ui->label->text() != "拟合结果：等待计算..." &&
        ui->label->text().contains("y =")) {

        // 自动重新计算（可选功能）
        // on_btnCalculate_clicked();

        // 或者只是清除之前的拟合结果
        ui->label->setText("拟合结果：数据已变更，请重新计算");
    }

    // 确保至少选中一行（提升用户体验）
    if (ui->tableWidget->rowCount() > 0 &&
        ui->tableWidget->selectionModel()->selectedIndexes().isEmpty()) {
        // 自动选中第一行
        ui->tableWidget->selectRow(0);
    }
}

void MainWindow::on_pushButton_tianjia_clicked()
{
    // 在表格末尾添加一行
    int newRow = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(newRow);

    // 可选：自动将焦点设置到新行的第一个单元格
    ui->tableWidget->setCurrentCell(newRow, 0);
    ui->tableWidget->edit(ui->tableWidget->currentIndex());

    // 更新数据点数组（添加一个默认点或空点）
    dataPoints.append(QPointF(0, 0));  // 或者 QPointF() 创建空点

    qDebug() << "添加新行，当前总行数:" << ui->tableWidget->rowCount();
}

void MainWindow::on_pushButton_daoru_clicked()
{
    // 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "选择数据文件",
        "",
        "数据文件 (*.csv *.txt);;所有文件 (*.*)"
    );

    if (fileName.isEmpty()) {
        return;  // 用户取消了选择
    }

    // 导入数据
    bool success = importDataFromFile(fileName);

    if (!success) {
        QMessageBox::critical(this, "导入失败", "数据导入失败，请检查文件格式和内容。");
    }
    else {
        QMessageBox::information(this, "导入成功",
            QString("成功导入 %1 个数据点").arg(ui->tableWidget->rowCount()));
    }

}

bool MainWindow::importDataFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法打开文件: " + fileName);
        return false;
    }

    QTextStream in(&file);

    // 清空现有数据
    clearTable();
    dataPoints.clear();

    int lineNumber = 0;
    int successCount = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        lineNumber++;

        // 跳过空行和注释行
        line = line.trimmed();//去除字符串两段的空白
        if (line.isEmpty() || line.startsWith('#') || line.startsWith("//")) {
            continue;
        }

        // 尝试不同的分隔符
        QStringList parts;
        if (line.contains(',')) {
            parts = line.split(',', Qt::SkipEmptyParts);
        }
        else if (line.contains('\t')) {
            parts = line.split('\t', Qt::SkipEmptyParts);
        }
        else if (line.contains(';')) {
            parts = line.split(';', Qt::SkipEmptyParts);
        }

        // 需要至少两列数据
        if (parts.size() < 2) {
            qDebug() << "第" << lineNumber << "行数据列不足，跳过";
            continue;
        }

        // 转换为数字
        bool okX, okY;
        double x = parts[0].trimmed().toDouble(&okX);
        double y = parts[1].trimmed().toDouble(&okY);

        if (okX && okY) {
            // 添加到表格
            addDataToTable(x, y);
            dataPoints.append(QPointF(x, y));
            successCount++;
        }
        else {
            qDebug() << "第" << lineNumber << "行数据格式错误，跳过";
        }
    }

    file.close();

    return successCount > 0;
}

void MainWindow::addDataToTable(double x, double y)
{
    int newRow = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(newRow);

    // 创建X坐标项
    QTableWidgetItem* xItem = new QTableWidgetItem(QString::number(x, 'f', 3));
    xItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(newRow, 0, xItem);

    // 创建Y坐标项
    QTableWidgetItem* yItem = new QTableWidgetItem(QString::number(y, 'f', 3));
    yItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(newRow, 1, yItem);
}

void MainWindow::clearTable()
{
    ui->tableWidget->setRowCount(0);
}

void MainWindow::onRadioButtonToggled()
{
    // 更新当前选择的平差方法
    if (ui->radioButton_x->isChecked()) {
        currentMethod = Y_OBSERVED;
    }
    else if (ui->radioButton_xy1->isChecked()) {
        currentMethod = X_OBSERVED;
    }
    else if (ui->radioButton_xy2->isChecked()) {
        currentMethod = XY_OBSERVED;
    }

    // 显示当前选择的方法
    QString methodName;
    switch (currentMethod) {
    case Y_OBSERVED:
        methodName = "Y观测（X无误差）";
        break;
    case X_OBSERVED:
        methodName = "X观测（Y无误差）";
        break;
    case XY_OBSERVED:
        methodName = "XY都观测";
        break;
    }

    statusBar()->showMessage(QString("当前平差方法: %1").arg(methodName), 2000);
}

void MainWindow::initializeSampleData()
{
    // 根据图片中的示例数据初始化
    dataPoints.clear();
    dataPoints << QPointF(1.0, 1.4) << QPointF(2.0, 2.0) << QPointF(3.0, 2.4);

    // 填充表格
    ui->tableWidget->setRowCount(dataPoints.size());
    for (int i = 0; i < dataPoints.size(); ++i) {
        QTableWidgetItem* xItem = new QTableWidgetItem(QString::number(dataPoints[i].x(), 'f', 1));
        QTableWidgetItem* yItem = new QTableWidgetItem(QString::number(dataPoints[i].y(), 'f', 1));

        xItem->setTextAlignment(Qt::AlignCenter);
        yItem->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(i, 0, xItem);
        ui->tableWidget->setItem(i, 1, yItem);
    }
}

void MainWindow::on_pushButton_clicked()
{
    // 检查数据点数量
    if (dataPoints.size() < 2) {
        QMessageBox::warning(this, "数据不足",
                             QString("至少需要2个数据点才能进行计算。当前只有%1个点。").arg(dataPoints.size()));
        return;
    }

    // 执行平差计算
    double a = 0.0, b = 0.0;
    bool success = false;
    QString methodName;

    // 根据选择的平差方法进行计算
    switch (currentMethod) {
    case Y_OBSERVED:
        success = calculateYObserved(a, b);
        methodName = "Y是观测值（X无误差）";
        break;
    case X_OBSERVED:
        success = calculateXY1Observed(a, b);
        methodName = "X是观测值（Y无误差）";
        break;
    case XY_OBSERVED:
        success = calculateXY2Observed(a, b);
        methodName = "XY都是观测值,都存在误差";
        break;
    }

    if (success) {
        // 计算相关系数和残差平方和
        double r = calculateCorrelationCoefficient();
        double sse = calculateSumSquaredErrors(a, b);

        // 显示拟合结果
        QString result = QString("拟合结果: y = %1x + %2\n方法: %3")
                             .arg(a, 0, 'f', 4)
                             .arg(b, 0, 'f', 4)
                             .arg(methodName);

        ui->label->setText(result);
        statusBar()->showMessage(QString("平差计算完成 - %1").arg(methodName), 3000);

        // 更新图形显示
        graphicsView->setDataPoints(dataPoints);
        graphicsView->setRegressionLine(a, b, r, sse, dataPoints.size());
    }
    else {
        QMessageBox::critical(this, "计算错误", "平差计算失败，请检查数据有效性。");
        ui->label->setText("拟合结果：计算失败");
    }
}

bool MainWindow::calculateYObserved(double& a, double& b)
{
    // Y观测（X无误差）的最小二乘法
    // 模型: y = ax + b

    if (dataPoints.size() < 2) {
        return false;
    }

    double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumX2 = 0.0;
    int n = dataPoints.size();

    // 计算各项和
    for (const QPointF& point : dataPoints) {
        double x = point.x();
        double y = point.y();
        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
    }

    // 计算均值
    double meanX = sumX / n;
    double meanY = sumY / n;

    // 计算离差平方和
    double Sxx = sumX2 - sumX * sumX / n;
    double Sxy = sumXY - sumX * sumY / n;

    // 检查数值稳定性
    if (fabs(Sxx) < 1e-10) {
        // 所有x值相同的情况 - 垂直线
        if (fabs(sumX2) > 1e-10) {
            a = std::numeric_limits<double>::infinity(); // 无穷大表示垂直线
            b = meanX; // 对于垂直线，b应该表示x的常数值
            return true;
        }
        return false; // 数据无效
    }

    // 计算斜率和截距（标准最小二乘公式）
    a = Sxy / Sxx;
    b = meanY - a * meanX;  // 正确的截距计算公式

    return true;
}

// 使用特征值分解的总体最小二乘法(TLS)
bool MainWindow::calculateXY1Observed(double& a, double& b)
{
    // 使用特征值分解的总体最小二乘法(TLS)
// 模型: y = ax + b

    if (dataPoints.size() < 2) return false;

    // 计算均值
    double meanX = 0.0, meanY = 0.0;
    for (const QPointF& point : dataPoints) {
        meanX += point.x();
        meanY += point.y();
    }
    meanX /= dataPoints.size();
    meanY /= dataPoints.size();

    // 构建协方差矩阵
    Eigen::Matrix2d covMatrix = Eigen::Matrix2d::Zero();
    for (const QPointF& point : dataPoints) {
        double dx = point.x() - meanX;
        double dy = point.y() - meanY;
        Eigen::Vector2d vec;
        vec << dx, dy;
        covMatrix += vec * vec.transpose();
    }
    covMatrix /= (dataPoints.size() - 1); // 无偏估计

    // 特征值分解
    Eigen::EigenSolver<Eigen::Matrix2d> solver(covMatrix);
    Eigen::Vector2d eigenvalues = solver.eigenvalues().real();
    Eigen::Matrix2d eigenvectors = solver.eigenvectors().real();

    // 找到最小特征值对应的特征向量
    int minIndex = 0;
    if (eigenvalues(1) < eigenvalues(0)) minIndex = 1;

    Eigen::Vector2d v = eigenvectors.col(minIndex);

    // 计算斜率 a = -v_x/v_y
    if (fabs(v(1)) < 1e-10) {
        // 垂直线情况
        a = std::numeric_limits<double>::infinity();
        b = meanX; // 对于垂直线，b表示x截距
        return true;
    }

    a = -v(0) / v(1);
    b = meanY - a * meanX;

    return true;
}

//奇异值分解的总体最小二乘法(TLS)
bool MainWindow::calculateXY2Observed(double& a, double& b)
{
    // 使用奇异值分解的总体最小二乘法(TLS)
    if (dataPoints.size() < 2) return false;

    int n = dataPoints.size();

    // 构建增广矩阵 [x, y, 1]
    Eigen::MatrixXd A(n, 3);
    for (int i = 0; i < n; ++i) {
        A(i, 0) = dataPoints[i].x();   // x
        A(i, 1) = dataPoints[i].y();  // y
        A(i, 2) = 1.0;               // 常数项
    }

    // 奇异值分解
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeFullV);
    Eigen::MatrixXd V = svd.matrixV();

    // 取最小奇异值对应的右奇异向量
    Eigen::Vector3d v = V.col(2);

    // 计算参数
    // 直线方程: v0*x + v1*y + v2 = 0
    // 转换为: y = a*x + b
    a = -v(0) / v(1);
    b = -v(2) / v(1);

    return true;
}

// 新增：计算相关系数
double MainWindow::calculateCorrelationCoefficient()
{
    if (dataPoints.size() < 2) return 0.0;

    double sumX = 0.0, sumY = 0.0, sumXY = 0.0;
    double sumX2 = 0.0, sumY2 = 0.0;
    int n = dataPoints.size();

    for (const QPointF& point : dataPoints) {
        double x = point.x();
        double y = point.y();
        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
        sumY2 += y * y;
    }

    double numerator = n * sumXY - sumX * sumY;
    double denominator = sqrt((n * sumX2 - sumX * sumX) * (n * sumY2 - sumY * sumY));

    if (fabs(denominator) < 1e-10) return 0.0;

    return numerator / denominator;
}

// 新增：计算残差平方和
double MainWindow::calculateSumSquaredErrors(double a, double b)
{
    double sse = 0.0;
    for (const QPointF& point : dataPoints) {
        double predictedY = a * point.x() + b;
        double residual = point.y() - predictedY;
        sse += residual * residual;
    }
    return sse;
}

void MainWindow::on_pushButton_2_clicked()
{
    // 检查是否有内容可导出
    if (dataPoints.isEmpty()) {
        QMessageBox::information(this, "导出提示",
                                 "当前没有数据点或回归线，无法导出图像。");
        return;
    }

    // 打开文件保存对话框
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "导出为PNG图像",
        QCoreApplication::applicationDirPath() + "/一元线性回归方程.png",
        "PNG图像 (*.png);;所有文件 (*.*)"
        );

    if (fileName.isEmpty()) {
        return;  // 用户取消了保存
    }

    // 确保文件扩展名是.png
    if (!fileName.endsWith(".png", Qt::CaseInsensitive)) {
        fileName += ".png";
    }

    // 执行导出操作
    bool success = exportToPng(fileName);

    if (success) {
        QMessageBox::information(this, "导出成功",
                                 QString("图像已成功导出到:\n%1").arg(fileName));
        statusBar()->showMessage(QString("图像已导出: %1").arg(fileName), 5000);
    }
    else {
        QMessageBox::critical(this, "导出失败",
                              "图像导出失败，请检查文件路径和权限。");
    }
}

bool MainWindow::exportToPng(const QString& fileName)
{
    // 创建QImage，使用与视图相同的大小
    QImage image(graphicsView->viewport()->size(), QImage::Format_ARGB32);
    image.fill(Qt::white);  // 白色背景

    // 创建QPainter来绘制图像
    QPainter painter(&image);
    // 启用抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 渲染整个GraphicsView到图像
    graphicsView->render(&painter);

    // 保存为PNG文件
    return image.save(fileName, "PNG");
}



