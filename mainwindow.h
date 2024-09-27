#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QWidget>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>

// Custom Widget to Draw the Full Rectangle Bars for Cutting Scheme
class CuttingChartWidget : public QWidget {
    Q_OBJECT

public:
    explicit CuttingChartWidget(QWidget *parent = nullptr);
    void setCuttingData(const std::map<QString, std::vector<double>> &data);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::map<QString, std::vector<double>> cuttingData; // Holds the cutting scheme data
};

// Main Window Class
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void optimizeCuts();       // Perform optimization when "Optimize" button is pressed
    void resetTables();        // Reset the input tables
    void addStockRow();        // Add a row to the stock table
    void addProfileRow();      // Add a row to the profile table
    void saveTableData();      // Save table data to file
    void loadTableData();      // Load table data from file

private:
    QTableWidget *stockTable;
    QTableWidget *profileTable;
    QTextEdit *resultArea;
    CuttingChartWidget *chartWidget;  // Custom widget for displaying cutting chart
    QPushButton *optimizeButton;

    void createMenu();  // Function to create the menu

    QString performCuttingOptimization(const std::vector<std::tuple<QString, int, double>> &stockData,
                                       const std::vector<std::tuple<QString, double, int>> &profileData);
};

#endif // MAINWINDOW_H
