#include "mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QPainter>
#include <QColor>
#include <algorithm>
#include <set>   // Add this line to include the std::set


// Custom Widget for Drawing the Cutting Scheme with Full Rectangle Bars
CuttingChartWidget::CuttingChartWidget(QWidget *parent) : QWidget(parent) {
}

// Set the cutting data for the chart and trigger a repaint
void CuttingChartWidget::setCuttingData(const std::map<QString, std::vector<double>> &data) {
    cuttingData = data;
    update(); // Request a repaint
}

// Paint the cutting chart using QPainter with colors
void CuttingChartWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int margin = 50;
    int maxHeight = height() - 2 * margin;
    int maxWidth = width() - 2 * margin;

    painter.setFont(QFont("Arial", 10));

    // Draw the axes
    painter.drawLine(margin, height() - margin, margin, margin);  // Y-axis
    painter.drawLine(margin, height() - margin, width() - margin, height() - margin);  // X-axis

    // Labels for axes
    painter.drawText(margin - 40, margin, "Length (m)");
    painter.drawText(width() - margin + 10, height() - margin + 10, "Profiles");

    // Define some colors for different profiles
    QList<QColor> colors = {Qt::red, Qt::green, Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow};
    int colorIndex = 0;

    // Draw the cutting bars with colors
    int profileIndex = 0;
    int profileSpacing = maxWidth / (cuttingData.size() + 1);

    for (const auto &entry : cuttingData) {
        QString profileName = entry.first;
        const std::vector<double> &cuts = entry.second;

        int xPos = margin + profileSpacing * (++profileIndex);
        painter.drawText(xPos - 20, height() - margin + 20, profileName);

        // Draw each cut as a full rectangle for this profile
        int yPosBase = height() - margin;
        int barWidth = 40;
        int currentYOffset = 0;

        QColor color = colors[colorIndex % colors.size()];
        colorIndex++;

        for (double cut : cuts) {
            int barHeight = static_cast<int>((cut / 10.0) * maxHeight);  // Scale length to fit chart
            QRect barRect(xPos - barWidth / 2, yPosBase - currentYOffset - barHeight, barWidth, barHeight);

            painter.setBrush(color);
            painter.setPen(color);
            painter.drawRect(barRect);

            // Label the cut length above the bar
            painter.setPen(Qt::black);
            painter.drawText(xPos - 10, yPosBase - currentYOffset - barHeight - 5, QString("%1 m").arg(cut, 0, 'f', 1));

            currentYOffset += barHeight; // Move up for next cut
        }
    }
}

// Main Window Implementation
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Profile Cutting Optimizer");
    setGeometry(100, 100, 1000, 800);

    // Central Widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Stock Table
    stockTable = new QTableWidget(0, 3, this);
    stockTable->setHorizontalHeaderLabels({"Profile Name", "Quantity", "Standard Length"});
    layout->addWidget(stockTable);

    // Add row button for Stock Table
    QPushButton *addStockButton = new QPushButton("Add Stock Row", this);
    connect(addStockButton, &QPushButton::clicked, this, &MainWindow::addStockRow);
    layout->addWidget(addStockButton);

    // Profile Table
    profileTable = new QTableWidget(0, 3, this);
    profileTable->setHorizontalHeaderLabels({"Profile Name", "Length to Cut", "Quantity"});
    layout->addWidget(profileTable);

    // Add row button for Profile Table
    QPushButton *addProfileButton = new QPushButton("Add Profile Row", this);
    connect(addProfileButton, &QPushButton::clicked, this, &MainWindow::addProfileRow);
    layout->addWidget(addProfileButton);

    // Result Area
    resultArea = new QTextEdit(this);
    resultArea->setReadOnly(true);
    layout->addWidget(resultArea);

    // Custom widget for displaying cutting scheme graphically
    chartWidget = new CuttingChartWidget(this);
    layout->addWidget(chartWidget);

    // "Optimize" button
    optimizeButton = new QPushButton("Optimize", this);
    connect(optimizeButton, &QPushButton::clicked, this, &MainWindow::optimizeCuts);
    layout->addWidget(optimizeButton);

    // Create the menu
    createMenu();
}

MainWindow::~MainWindow() {
    // Destructor
}

// Create the menu with "File" options for saving and loading
void MainWindow::createMenu() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *saveAction = new QAction(tr("Save Tables"), this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveTableData);
    fileMenu->addAction(saveAction);

    QAction *loadAction = new QAction(tr("Load Tables"), this);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadTableData);
    fileMenu->addAction(loadAction);
}

// Save the data from both stock and profile tables to a file
void MainWindow::saveTableData() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Table Data", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot save file: " + file.errorString());
        return;
    }

    QTextStream out(&file);

    // Save stock table data
    out << "Stock Table:\n";
    for (int row = 0; row < stockTable->rowCount(); ++row) {
        for (int col = 0; col < stockTable->columnCount(); ++col) {
            if (stockTable->item(row, col) != nullptr)
                out << stockTable->item(row, col)->text() << (col == 2 ? "\n" : ",");
        }
    }

    // Save profile table data
    out << "Profile Table:\n";
    for (int row = 0; row < profileTable->rowCount(); ++row) {
        for (int col = 0; col < profileTable->columnCount(); ++col) {
            if (profileTable->item(row, col) != nullptr)
                out << profileTable->item(row, col)->text() << (col == 2 ? "\n" : ",");
        }
    }

    file.close();
}

// Load the data from a file and populate the stock and profile tables
void MainWindow::loadTableData() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Table Data", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open file: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    QString line;
    bool loadingStock = true;
    int stockRow = 0;
    int profileRow = 0;

    stockTable->setRowCount(0);  // Clear existing data
    profileTable->setRowCount(0);  // Clear existing data

    while (!in.atEnd()) {
        line = in.readLine();
        if (line.startsWith("Stock Table:")) {
            loadingStock = true;
            continue;
        } else if (line.startsWith("Profile Table:")) {
            loadingStock = false;
            continue;
        }

        QStringList parts = line.split(",");
        if (parts.size() != 3) continue;  // Invalid format

        if (loadingStock) {
            stockTable->insertRow(stockRow);
            stockTable->setItem(stockRow, 0, new QTableWidgetItem(parts[0]));
            stockTable->setItem(stockRow, 1, new QTableWidgetItem(parts[1]));
            stockTable->setItem(stockRow, 2, new QTableWidgetItem(parts[2]));
            stockRow++;
        } else {
            profileTable->insertRow(profileRow);
            profileTable->setItem(profileRow, 0, new QTableWidgetItem(parts[0]));
            profileTable->setItem(profileRow, 1, new QTableWidgetItem(parts[1]));
            profileTable->setItem(profileRow, 2, new QTableWidgetItem(parts[2]));
            profileRow++;
        }
    }

    file.close();
}

void MainWindow::addStockRow() {
    int currentRow = stockTable->rowCount();
    stockTable->insertRow(currentRow);
    stockTable->setItem(currentRow, 0, new QTableWidgetItem(""));
    stockTable->setItem(currentRow, 1, new QTableWidgetItem("0"));
    stockTable->setItem(currentRow, 2, new QTableWidgetItem("0.0"));
}

void MainWindow::addProfileRow() {
    int currentRow = profileTable->rowCount();
    profileTable->insertRow(currentRow);
    profileTable->setItem(currentRow, 0, new QTableWidgetItem(""));
    profileTable->setItem(currentRow, 1, new QTableWidgetItem("0.0"));
    profileTable->setItem(currentRow, 2, new QTableWidgetItem("0"));
}

void MainWindow::optimizeCuts() {
    std::vector<std::tuple<QString, int, double>> stockData;
    std::vector<std::tuple<QString, double, int>> profileData;

    // Get data from stock table
    for (int row = 0; row < stockTable->rowCount(); ++row) {
        if (stockTable->item(row, 0) == nullptr || stockTable->item(row, 1) == nullptr || stockTable->item(row, 2) == nullptr) {
            continue;
        }

        QString profileName = stockTable->item(row, 0)->text();
        int quantity = stockTable->item(row, 1)->text().toInt();
        double length = stockTable->item(row, 2)->text().toDouble();
        stockData.emplace_back(profileName, quantity, length);
    }

    // Get data from profile table
    for (int row = 0; row < profileTable->rowCount(); ++row) {
        if (profileTable->item(row, 0) == nullptr || profileTable->item(row, 1) == nullptr || profileTable->item(row, 2) == nullptr) {
            continue;
        }

        QString profileName = profileTable->item(row, 0)->text();
        double lengthToCut = profileTable->item(row, 1)->text().toDouble();
        int quantity = profileTable->item(row, 2)->text().toInt();
        profileData.emplace_back(profileName, lengthToCut, quantity);
    }

    resultArea->clear();
    QString result = performCuttingOptimization(stockData, profileData);

    resultArea->append("Cutting Scheme:\n");
    resultArea->append(result);
}

void MainWindow::resetTables() {
    stockTable->setRowCount(0);
    profileTable->setRowCount(0);
    resultArea->clear();
    chartWidget->update();
}

// Perform cutting optimization
QString MainWindow::performCuttingOptimization(
    const std::vector<std::tuple<QString, int, double>> &stockData,
    const std::vector<std::tuple<QString, double, int>> &profileData) {

    QString result;
    std::map<QString, std::vector<double>> stockProfiles;
    std::map<QString, std::vector<double>> cutData;

    double totalStockLength = 0.0;  // Total length of available stock
    double usedStockLength = 0.0;   // Total length actually used in cutting

    // Group stock profiles by type and length
    for (const auto &stock : stockData) {
        QString profileName = std::get<0>(stock);
        int quantity = std::get<1>(stock);
        double length = std::get<2>(stock);

        for (int i = 0; i < quantity; ++i) {
            stockProfiles[profileName].push_back(length);
            totalStockLength += length;  // Adding total length of all stock profiles
        }
    }

    // Sort stock profiles by length in descending order to minimize waste
    for (auto &entry : stockProfiles) {
        std::sort(entry.second.begin(), entry.second.end(), std::greater<double>());
    }

    // Group profiles by name and sort by length to cut
    std::map<QString, std::vector<std::pair<double, int>>> groupedProfiles;
    for (const auto &profile : profileData) {
        QString profileName = std::get<0>(profile);
        double lengthToCut = std::get<1>(profile);
        int quantity = std::get<2>(profile);

        groupedProfiles[profileName].emplace_back(lengthToCut, quantity);
    }

    // Sort each profile's lengths in descending order to maximize stock usage
    for (auto &entry : groupedProfiles) {
        std::sort(entry.second.begin(), entry.second.end(), std::greater<>());
    }

    // Track identical cutting schemes
    std::map<QString, int> schemeCounts;

    // Process each profile group and try to minimize waste
    for (const auto &entry : groupedProfiles) {
        QString profileName = entry.first;
        const auto &lengthsToCut = entry.second;

        if (stockProfiles.find(profileName) == stockProfiles.end()) {
            result += QString("No stock available for profile: %1\n").arg(profileName);
            continue;
        }

        auto &availableStock = stockProfiles[profileName];

        for (auto &stockLength : availableStock) {
            bool hasCuts = false;
            double remainder = stockLength;
            QString cutsSummary = "";  // Store combined cuts in one line

            for (const auto &lengthAndQuantity : lengthsToCut) {
                double lengthToCut = lengthAndQuantity.first;
                int &quantityToCut = const_cast<int &>(lengthAndQuantity.second);  // Allow mutation

                // Cut as many pieces of this length as possible, until remainder is too small
                while (quantityToCut > 0 && remainder >= lengthToCut) {
                    remainder -= lengthToCut;
                    usedStockLength += lengthToCut;  // Track used length
                    cutsSummary += QString::number(lengthToCut) + " + ";
                    quantityToCut--;
                    hasCuts = true;
                }
            }

            // If stock was used, calculate the total used length and remainder
            if (!cutsSummary.isEmpty()) {
                cutsSummary.chop(3);  // Remove the last " + "
                QString combinedScheme = QString("%1 from stock profile %2, remainder: %3")
                                             .arg(cutsSummary)
                                             .arg(profileName)
                                             .arg(remainder);

                // Track the cutting scheme and count it
                auto it = schemeCounts.find(combinedScheme);
                if (it != schemeCounts.end()) {
                    schemeCounts[combinedScheme]++;
                } else {
                    schemeCounts[combinedScheme] = 1;
                }
            }

            // If stock profile was not cut, continue to the next profile
            if (!hasCuts) {
                continue;
            }
        }

        // Check if any profiles remain uncut
        for (const auto &lengthAndQuantity : lengthsToCut) {
            if (lengthAndQuantity.second > 0) {
                result += QString("Not enough stock for %1 (Need %2 more pieces of length %3)\n")
                .arg(profileName)
                    .arg(lengthAndQuantity.second)
                    .arg(lengthAndQuantity.first);
            }
        }
    }

    // Print grouped cutting schemes with counts
    for (const auto &scheme : schemeCounts) {
        if (scheme.second > 1) {
            result += QString("%1 x (%2)\n").arg(scheme.second).arg(scheme.first);
        } else {
            result += scheme.first + "\n";
        }
    }

    // **Calculate optimization percentage**: (usedStockLength / totalStockLength) * 100
    double optimizationPercent = 0.0;
    if (totalStockLength > 0) {
        optimizationPercent = (usedStockLength / totalStockLength) * 100;
    }

    // Display the optimization percentage in the result
    result += QString("\nOptimization Percentage: %1%\n").arg(optimizationPercent, 0, 'f', 2);

    // Update the cutting chart with the results
    chartWidget->setCuttingData(cutData);

    return result;
}
