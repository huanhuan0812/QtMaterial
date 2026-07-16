#include "MMainWindow.h"
#include <QApplication>
#include <QWidget>

MMainWindow::MMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 应用当前主题
    applyTheme(m_themeMode);
    
    // 连接主题变化信号
    connectThemeSignals();
}

MMainWindow::~MMainWindow()
{
    // 无需额外操作
}

void MMainWindow::connectThemeSignals()
{
    MTheme* theme = MTheme::instance();
    if (theme) {
        // 当 MTheme 的颜色方案改变时，只刷新 UI，不重新应用主题
        connect(theme, &MTheme::colorSchemeChanged, this, [this]() {
            // 只更新颜色，避免循环调用 applyTheme
            updateColors();
        });
    }
}

void MMainWindow::setThemeMode(MThemeMode mode)
{
    if (m_themeMode == mode) {
        return;
    }
    m_themeMode = mode;
    applyTheme(mode);
    emit themeChanged(mode);
}

void MMainWindow::applyTheme(MThemeMode mode)
{
    MTheme* theme = MTheme::instance();
    if (!theme) {
        return;
    }
    
    // 根据模式设置颜色方案
    if (mode == MThemeMode::Light) {
        theme->setColorScheme(MTheme::lightScheme());
    } else {
        theme->setColorScheme(MTheme::darkScheme());
    }
    
    // 更新所有子控件的颜色
    updateColors();
}

void MMainWindow::updateColors()
{
    // 发出主题改变信号，子控件可以监听此信号来更新自己的颜色
    emit themeChanged(m_themeMode);
}

void MMainWindow::onThemeChanged(MThemeMode mode)
{
    setThemeMode(mode);
}

void MMainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
    // 窗口显示时确保主题正确应用
    applyTheme(m_themeMode);
}

// === 静态工具方法实现 ===

MTheme* MMainWindow::themeFor(QWidget* widget)
{
    if (!widget) {
        return MTheme::instance();  // 返回全局单例
    }
    
    // 向上遍历父控件链，查找 MMainWindow
    QWidget* parent = widget;
    while (parent) {
        MMainWindow* mainWindow = qobject_cast<MMainWindow*>(parent);
        if (mainWindow) {
            return mainWindow->theme();
        }
        parent = parent->parentWidget();
    }
    
    // 如果没有找到 MMainWindow，返回全局单例
    return MTheme::instance();
}

MThemeMode MMainWindow::themeModeFor(QWidget* widget)
{
    if (!widget) {
        return MThemeMode::Light;
    }
    
    QWidget* parent = widget;
    while (parent) {
        MMainWindow* mainWindow = qobject_cast<MMainWindow*>(parent);
        if (mainWindow) {
            return mainWindow->themeMode();
        }
        parent = parent->parentWidget();
    }
    
    return MThemeMode::Light;
}