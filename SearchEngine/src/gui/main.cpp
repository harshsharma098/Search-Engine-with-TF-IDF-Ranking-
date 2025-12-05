#include "gui/MainWindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("SearchEngine");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("SearchEngine");
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}

