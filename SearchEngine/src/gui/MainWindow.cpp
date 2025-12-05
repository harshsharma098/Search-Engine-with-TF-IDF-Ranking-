#include "gui/MainWindow.h"
#include <QFile>
#include <QTextStream>
#include <QScrollBar>
#include <QApplication>
#include <QHeaderView>
#include <sstream>
#include <iomanip>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    updateStatus("Ready. Load documents to begin searching.");
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    
    // Search bar section
    searchLayout = new QHBoxLayout();
    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("Enter search query...");
    searchInput->setMinimumHeight(35);
    searchButton = new QPushButton("Search", this);
    searchButton->setMinimumWidth(100);
    searchButton->setMinimumHeight(35);
    
    searchLayout->addWidget(searchInput);
    searchLayout->addWidget(searchButton);
    
    // Connect search functionality
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(searchInput, &QLineEdit::returnPressed, this, &MainWindow::onSearchClicked);
    
    mainLayout->addLayout(searchLayout);
    
    // Button section
    buttonLayout = new QHBoxLayout();
    loadFilesButton = new QPushButton("Load Files", this);
    clearIndexButton = new QPushButton("Clear Index", this);
    
    buttonLayout->addWidget(loadFilesButton);
    buttonLayout->addWidget(clearIndexButton);
    buttonLayout->addStretch();
    
    connect(loadFilesButton, &QPushButton::clicked, this, &MainWindow::onLoadFilesClicked);
    connect(clearIndexButton, &QPushButton::clicked, this, &MainWindow::onClearIndexClicked);
    
    mainLayout->addLayout(buttonLayout);
    
    // Results label
    resultsLabel = new QLabel("Results:", this);
    resultsLabel->setStyleSheet("font-weight: bold; font-size: 12pt;");
    mainLayout->addWidget(resultsLabel);
    
    // Results and preview section
    QHBoxLayout* contentLayout = new QHBoxLayout();
    
    // Results list
    resultsList = new QListWidget(this);
    resultsList->setMinimumWidth(400);
    resultsList->setMaximumWidth(500);
    connect(resultsList, &QListWidget::itemClicked, this, &MainWindow::onResultSelected);
    
    // Document preview
    documentPreview = new QTextEdit(this);
    documentPreview->setReadOnly(true);
    documentPreview->setPlaceholderText("Select a result to preview document content...");
    
    contentLayout->addWidget(resultsList);
    contentLayout->addWidget(documentPreview, 2); // Preview takes 2x space
    
    mainLayout->addLayout(contentLayout, 1);
    
    // Status bar
    statusLabel = new QLabel("", this);
    statusBar()->addWidget(statusLabel);
    
    progressBar = new QProgressBar(this);
    progressBar->setVisible(false);
    statusBar()->addPermanentWidget(progressBar);
    
    // Window properties
    setWindowTitle("C++ Search Engine - TF-IDF");
    resize(1000, 700);
    
    // Style
    setStyleSheet(
        "QMainWindow { background-color: #f5f5f5; }"
        "QPushButton { "
        "  background-color: #4CAF50; "
        "  color: white; "
        "  border: none; "
        "  padding: 8px 16px; "
        "  border-radius: 4px; "
        "  font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:pressed { background-color: #3d8b40; }"
        "QLineEdit { "
        "  padding: 8px; "
        "  border: 2px solid #ddd; "
        "  border-radius: 4px; "
        "  font-size: 12pt; "
        "}"
        "QLineEdit:focus { border: 2px solid #4CAF50; }"
        "QListWidget { "
        "  border: 1px solid #ddd; "
        "  border-radius: 4px; "
        "  background-color: white; "
        "}"
        "QListWidget::item { "
        "  padding: 8px; "
        "  border-bottom: 1px solid #eee; "
        "}"
        "QListWidget::item:hover { background-color: #f0f0f0; }"
        "QListWidget::item:selected { background-color: #e3f2fd; }"
        "QTextEdit { "
        "  border: 1px solid #ddd; "
        "  border-radius: 4px; "
        "  background-color: white; "
        "  padding: 10px; "
        "}"
    );
}

void MainWindow::onSearchClicked() {
    QString query = searchInput->text().trimmed();
    
    if (query.isEmpty()) {
        QMessageBox::information(this, "Search", "Please enter a search query.");
        return;
    }
    
    if (searchEngine.getDocumentCount() == 0) {
        QMessageBox::information(this, "Search", "No documents indexed. Please load files first.");
        return;
    }
    
    updateStatus("Searching...");
    QApplication::processEvents();
    
    std::vector<SearchResult> results = searchEngine.search(query.toStdString());
    
    displayResults(results);
    
    QString statusMsg = QString("Found %1 result(s) for '%2'")
                       .arg(results.size())
                       .arg(query);
    updateStatus(statusMsg);
}

void MainWindow::onLoadFilesClicked() {
    QStringList filePaths = QFileDialog::getOpenFileNames(
        this,
        "Select Text Files to Index",
        ".",
        "Text Files (*.txt);;All Files (*.*)"
    );
    
    if (filePaths.isEmpty()) {
        return;
    }
    
    progressBar->setVisible(true);
    progressBar->setMaximum(filePaths.size());
    progressBar->setValue(0);
    updateStatus("Indexing documents...");
    QApplication::processEvents();
    
    std::vector<std::string> paths;
    for (const QString& path : filePaths) {
        paths.push_back(path.toStdString());
    }
    
    int indexed = searchEngine.indexDocuments(paths);
    
    progressBar->setValue(filePaths.size());
    progressBar->setVisible(false);
    
    QString statusMsg = QString("Indexed %1 of %2 document(s) successfully.")
                       .arg(indexed)
                       .arg(filePaths.size());
    updateStatus(statusMsg);
    
    if (indexed < filePaths.size()) {
        QMessageBox::warning(this, "Indexing", 
                           QString("Some files could not be indexed. %1 of %2 succeeded.")
                           .arg(indexed)
                           .arg(filePaths.size()));
    }
}

void MainWindow::onClearIndexClicked() {
    if (searchEngine.getDocumentCount() == 0) {
        QMessageBox::information(this, "Clear Index", "Index is already empty.");
        return;
    }
    
    int ret = QMessageBox::question(this, "Clear Index",
                                   QString("Are you sure you want to clear %1 indexed document(s)?")
                                   .arg(searchEngine.getDocumentCount()),
                                   QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        searchEngine.clear();
        resultsList->clear();
        documentPreview->clear();
        updateStatus("Index cleared. Ready to load new documents.");
    }
}

void MainWindow::onResultSelected(QListWidgetItem* item) {
    if (!item) {
        return;
    }
    
    // Extract file path from item data
    QString filePath = item->data(Qt::UserRole).toString();
    if (!filePath.isEmpty()) {
        displayDocumentPreview(filePath);
    }
}

void MainWindow::updateStatus(const QString& message) {
    statusLabel->setText(message);
    statusBar()->showMessage(message, 5000);
}

void MainWindow::displayResults(const std::vector<SearchResult>& results) {
    resultsList->clear();
    documentPreview->clear();
    
    if (results.empty()) {
        resultsLabel->setText("Results: No matches found.");
        return;
    }
    
    resultsLabel->setText(QString("Results: %1 match(es)").arg(results.size()));
    
    for (const auto& result : results) {
        QString formatted = formatResult(result);
        QListWidgetItem* item = new QListWidgetItem(formatted, resultsList);
        item->setData(Qt::UserRole, QString::fromStdString(result.document->filePath));
        resultsList->addItem(item);
    }
}

QString MainWindow::formatResult(const SearchResult& result) {
    QString fileName = QString::fromStdString(result.document->fileName);
    QString score = QString::number(result.score, 'f', 4);
    QString filePath = QString::fromStdString(result.document->filePath);
    
    return QString("%1\nScore: %2\n%3")
           .arg(fileName)
           .arg(score)
           .arg(filePath);
}

void MainWindow::displayDocumentPreview(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        documentPreview->setPlainText("Error: Could not read file.");
        return;
    }
    
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    
    documentPreview->setPlainText(content);
    documentPreview->moveCursor(QTextCursor::Start);
}

