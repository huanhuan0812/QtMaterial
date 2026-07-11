#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "MButtons/MFilledButton.h"
#include "MButtons/MOutlinedButton.h"
#include "MToast/QToast.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent)
    {
        auto* central = new QWidget(this);
        auto* layout = new QVBoxLayout(central);
        
        auto* title = new QLabel("QtMaterial Demo", this);
        title->setStyleSheet("font-size: 24px; font-weight: bold; margin: 20px;");
        layout->addWidget(title);
        
        auto* filledBtn = new MFilledButton("Filled Button", this);
        layout->addWidget(filledBtn);
        
        auto* outlinedBtn = new MOutlinedButton("Outlined Button", this);
        layout->addWidget(outlinedBtn);
        
        auto* toastBtn = new QPushButton("Show Toast", this);
        layout->addWidget(toastBtn);
        
        connect(toastBtn, &QPushButton::clicked, this, [this]() {
            QToast::showMessage("Hello from QtMaterial!", this);
        });
        
        layout->addStretch();
        setCentralWidget(central);
        resize(400, 300);
    }
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}

#include "main.moc"  // 添加这一行