#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QProgressBar>
#include <QThread>
#include <QStringList>
#include "core/SearchEngine.h"

/**
 * @brief Main window class for the search engine GUI application.
 * 
 * Provides a user-friendly interface for loading documents, performing searches,
 * and viewing results with TF-IDF relevance scores.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * 
     * @param parent Parent widget (nullptr for main window)
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Slot called when search button is clicked or Enter is pressed.
     */
    void onSearchClicked();

    /**
     * @brief Slot called when load files button is clicked.
     */
    void onLoadFilesClicked();

    /**
     * @brief Slot called when clear index button is clicked.
     */
    void onClearIndexClicked();

    /**
     * @brief Slot called when a result item is selected.
     * 
     * @param item The selected list item
     */
    void onResultSelected(QListWidgetItem* item);

private:
    // UI Components
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    QHBoxLayout* searchLayout;
    QHBoxLayout* buttonLayout;
    
    QLineEdit* searchInput;
    QPushButton* searchButton;
    QPushButton* loadFilesButton;
    QPushButton* clearIndexButton;
    
    QLabel* resultsLabel;
    QListWidget* resultsList;
    QTextEdit* documentPreview;
    
    QLabel* statusLabel;
    QProgressBar* progressBar;
    
    // Search Engine
    SearchEngine searchEngine;
    
    /**
     * @brief Sets up the user interface.
     */
    void setupUI();
    
    /**
     * @brief Updates the status message.
     * 
     * @param message The status message
     */
    void updateStatus(const QString& message);
    
    /**
     * @brief Updates the results display.
     * 
     * @param results Vector of search results
     */
    void displayResults(const std::vector<SearchResult>& results);
    
    /**
     * @brief Displays document content in the preview area.
     * 
     * @param filePath Path to the document file
     */
    void displayDocumentPreview(const QString& filePath);
    
    /**
     * @brief Formats a search result for display in the list.
     * 
     * @param result The search result
     * @return Formatted string
     */
    QString formatResult(const SearchResult& result);
};

#endif // MAINWINDOW_H

