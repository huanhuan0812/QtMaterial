#pragma once
#include <QMainWindow>
#include <utils/MTheme.h>
#include <QPointer>

#ifdef QTMATERIAL_LIBRARY
    #ifdef _WIN32
        #ifdef QTMATERIAL_SHARED
            #define QTMATERIAL_EXPORT __declspec(dllexport)
        #else 
            #define QTMATERIAL_EXPORT __declspec(dllimport)
        #endif
    #else
        #define QTMATERIAL_EXPORT __attribute__((visibility("default")))
    #endif
#else
    #define QTMATERIAL_EXPORT
#endif

class QTMATERIAL_EXPORT MMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MMainWindow(QWidget *parent = nullptr);
    virtual ~MMainWindow();

    // === 主题获取 ===
    // 获取当前主题实例（直接返回单例）
    MTheme* theme() const { return MTheme::instance(); }
    
    // 获取当前主题模式
    MThemeMode themeMode() const { return m_themeMode; }
    void setThemeMode(MThemeMode mode);
    
    // === 主题切换 ===
    virtual void applyTheme(MThemeMode mode);
    virtual void updateColors();
    
    // === 静态工具方法 ===
    // 获取任意控件的主题实例（从父控件链查找 MMainWindow，如果没有则返回全局单例）
    static MTheme* themeFor(QWidget* widget);
    // 获取任意控件的主题模式（从父控件链查找 MMainWindow，如果没有则返回 Light）
    static MThemeMode themeModeFor(QWidget* widget);
    
public slots:
    void onThemeChanged(MThemeMode mode);
    
signals:
    void themeChanged(MThemeMode mode);

protected:
    MThemeMode m_themeMode = MThemeMode::Light;
    
    virtual void showEvent(QShowEvent* event) override;
    
private:
    // 连接 MTheme 的信号
    void connectThemeSignals();
};