#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "MToast/QToast.h"
#include "MButtons/MButton.h"
#include "MWidgets/MMainWindow.h"
#include "MWidgets/MCard.h"

class MainWindow : public MMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr) : MMainWindow(parent)
    {
        auto* central = new QWidget(this);
        auto* layout = new QVBoxLayout(central);
        
        auto* title = new QLabel("QtMaterial Demo", this);
        title->setStyleSheet("font-size: 24px; font-weight: bold; margin: 20px;");
        layout->addWidget(title);
        
        auto* filledBtn = new MButton(this);
        filledBtn->setText("Filled Button");
        filledBtn->setButtonType(MButton::Filled);
        layout->addWidget(filledBtn);
        
        auto* outlinedBtn = new MButton(this);
        outlinedBtn->setText("Outlined Button");
        outlinedBtn->setButtonType(MButton::Outlined);
        layout->addWidget(outlinedBtn);
        
        auto* toastBtn = new QPushButton("Show Toast", this);
        layout->addWidget(toastBtn);

        auto* card = new MCard(this);
        card->setType(MCard::Type::Outlined);
        layout->addWidget(card);

        QLabel* cardLabel = new QLabel("This is a card.", card);

        connect(toastBtn, &QPushButton::clicked, this, [this]() {
            QToast::makeToast(this, "This is a toast message!", 3000, QToast::POSITION_BOTTOM);
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