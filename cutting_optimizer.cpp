#include <QApplication>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QWidget>
#include <vector>
#include <algorithm>
#include <iostream>

// Data structure for storing stock profiles and cut list profiles
struct Profile {
    int length;
    int quantity;
};

// Cutting Optimizer class
class CuttingOptimizer : public QWidget {
    Q_OBJECT

public:
    CuttingOptimizer(QWidget *parent = nullptr);

private slots:
    void optimizeCuts();

private:
    QTableWidget *stockTable;
    QTableWidget *cutListTable;
    QPushButton *optimizeButton;

    std::vector<Profile> getStockProfiles();
    std::vector<Profile> getCutList();
    void showResult(const std::vector<std::pair<int, int>>& result, int totalWaste);
};

// Constructor for the Cutting Optimizer
CuttingOptimizer::CuttingOptimizer(QWidget *parent)
    : QWidget(parent) {

    // Set up layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Set up stock profile table
    stockTable = new QTableWidget(5, 2, this); // 5 rows, 2 columns (Length, Quantity)
    stockTable->setHorizontalHeaderLabels(QStringList() << "Length" << "Quantity");
    layout->addWidget(stockTable);

    // Set up cut list table
    cutListTable = new QTableWidget(5, 2, this); // 5 rows, 2 columns (Length, Quantity)
    cutListTable->setHorizontalHeaderLabels(QStringList() << "Length" << "Quantity");
    layout->addWidget(cutListTable);

    // Optimize button
    optimizeButton = new QPushButton("Optimize Cuts", this);
    layout->addWidget(optimizeButton);

    // Connect button to slot
    connect(optimizeButton, &QPushButton::clicked, this, &CuttingOptimizer::optimizeCuts);

    setLayout(layout);
}

// Helper function to retrieve stock profiles from the table
std::vector<Profile> CuttingOptimizer::getStockProfiles() {
    std::vector<Profile> stockProfiles;
    for (int i = 0; i < stockTable->rowCount(); ++i) {
        int length = stockTable->item(i, 0) ? stockTable->item(i, 0)->text().toInt() : 0;
        int quantity = stockTable->item(i, 1) ? stockTable->item(i, 1)->text().toInt() : 0;
        if (length > 0 && quantity > 0) {
            stockProfiles.push_back({length, quantity});
        }
    }
    return stockProfiles;
}

// Helper function to retrieve cut list from the table
std::vector<Profile> CuttingOptimizer::getCutList() {
    std::vector<Profile> cutList;
    for (int i = 0; i < cutListTable->rowCount(); ++i) {
        int length = cutListTable->item(i, 0) ? cutListTable->item(i, 0)->text().toInt() : 0;
        int quantity = cutListTable->item(i, 1) ? cutListTable->item(i, 1)->text().toInt() : 0;
        if (length > 0 && quantity > 0) {
            cutList.push_back({length, quantity});
        }
    }
    return cutList;
}

// Optimization algorithm: simple greedy method
void CuttingOptimizer::optimizeCuts() {
    std::vector<Profile> stock = getStockProfiles();
    std::vector<Profile> cuts = getCutList();

    std::vector<std::pair<int, int>> result;  // store <cut length, stock length>
    int totalWaste = 0;

    // Sort stock profiles by length (ascending order)
    std::sort(stock.begin(), stock.end(), [](const Profile &a, const Profile &b) {
        return a.length < b.length;
    });

    // Simple greedy algorithm
    for (const auto &cut : cuts) {
        int neededQuantity = cut.quantity;
        int cutLength = cut.length;

        for (auto &stockProfile : stock) {
            while (stockProfile.quantity > 0 && neededQuantity > 0) {
                if (stockProfile.length >= cutLength) {
                    // Use this stock piece for the cut
                    result.push_back({cutLength, stockProfile.length});
                    stockProfile.quantity--;
                    neededQuantity--;

                    // Calculate waste
                    totalWaste += stockProfile.length - cutLength;
                } else {
                    break;  // Move to next available stock
                }
            }
        }

        if (neededQuantity > 0) {
            QMessageBox::warning(this, "Optimization Error", "Not enough stock to fulfill the cut list.");
            return;
        }
    }

    // Show the result
    showResult(result, totalWaste);
}

// Display the optimization result in a message box
void CuttingOptimizer::showResult(const std::vector<std::pair<int, int>>& result, int totalWaste) {
    QString message;
    for (const auto &pair : result) {
        message += QString("Cut length %1 from stock length %2\n").arg(pair.first).arg(pair.second);
    }
    message += QString("\nTotal waste: %1").arg(totalWaste);

    QMessageBox::information(this, "Optimization Result", message);
}


#include "cutting_optimizer.moc"
