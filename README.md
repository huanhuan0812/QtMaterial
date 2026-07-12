# QtMaterial

**一套基于 Material Design 3 设计规范的现代化 Qt 组件库**

[![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
[![C++](https://img.shields.io/badge/C++-11%2F14%2F17-blue.svg)](https://isocpp.org/)
[![Qt](https://img.shields.io/badge/Qt-5.12%2B%20%7C%206.x-brightgreen.svg)](https://www.qt.io/)

---

## 📖 简介

QtMaterial 是一个轻量级、跨平台的 Qt 组件库，严格遵循 **Material Design 3** 设计规范。它提供了一套开箱即用、高度可定制的现代化 UI 组件，帮助开发者快速构建美观、一致且交互流畅的桌面和移动应用。

### 设计理念

- **遵循规范**：严格按照 Material Design 3 指南实现颜色、间距、动效和交互模式
- **开箱即用**：提供完整的组件，无需额外配置即可获得符合规范的视觉风格
- **高度可定制**：每个组件都暴露丰富的 API，支持颜色、尺寸、动画等维度的自由调整
- **跨平台一致**：在 Windows、macOS、Linux、Android、iOS 上保持统一的外观和交互体验
- **性能优先**：基于 Qt 原生绘制，避免样式表开销，确保流畅的动画性能

---

## 🧩 组件概览

| 组件 | 描述 | 状态 |
|------|------|------|
| **MCard** | Material Design 3 卡片控件，支持阴影、圆角、悬停/点击/选中交互 | ✅ 稳定 |
| **QToast** | 轻量级吐司提示组件，支持鼠标穿透、淡入淡出动画、三种预设位置 | ✅ 稳定 |
| **MOutlinedButton** | 描边按钮（次要操作），透明背景 + 彩色边框 | ✅ 稳定 |
| **MFilledButton** | 填充按钮（主要操作），彩色背景 + 高对比度文字 | ✅ 稳定 |
| **MTextButton** | 文本按钮（最低突出操作），仅文字无边框 | ✅ 稳定 |

### 计划中的组件

- [ ] MCheckbox - 复选框
- [ ] MRadioButton - 单选按钮
- [ ] MSwitch - 开关
- [ ] MTextField - 文本输入框
- [ ] MDialog - 对话框
- [ ] MNavigationBar - 底部导航栏
- [ ] MAppBar - 应用栏

---

## 🚀 快速开始

### 环境要求

- **Qt**: 5.12+ 或 6.x
- **C++**: C++11 或更高版本
- **编译器**: 支持 C++11 的 MSVC、GCC、Clang

### 集成到项目

#### 方式一：源码集成（推荐）

```bash
# 克隆仓库
git clone https://github.com/yourusername/QtMaterial.git

# 将 src/ 目录复制到你的项目下
```

在 `.pro` 文件中添加：

```qmake
# 包含头文件路径
INCLUDEPATH += $$PWD/QtMaterial/src

# 添加源文件
HEADERS += $$PWD/QtMaterial/src/MCard.h \
           $$PWD/QtMaterial/src/MWidgetBase.h \
           $$PWD/QtMaterial/src/QToast.h \
           $$PWD/QtMaterial/src/MBaseButton.h \
           $$PWD/QtMaterial/src/MOutlinedButton.h \
           $$PWD/QtMaterial/src/MFilledButton.h \
           $$PWD/QtMaterial/src/MTextButton.h

SOURCES += $$PWD/QtMaterial/src/MCard.cpp \
           $$PWD/QtMaterial/src/MWidgetBase.cpp \
           $$PWD/QtMaterial/src/QToast.cpp \
           $$PWD/QtMaterial/src/MBaseButton.cpp \
           $$PWD/QtMaterial/src/MOutlinedButton.cpp \
           $$PWD/QtMaterial/src/MFilledButton.cpp \
           $$PWD/QtMaterial/src/MTextButton.cpp
```

#### 方式二：CMake 集成

```cmake
# 添加子目录
add_subdirectory(QtMaterial)

# 链接库
target_link_libraries(your_app QtMaterial)
```

### 第一个应用

```cpp
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>

#include "MCard.h"
#include "MFilledButton.h"
#include "MOutlinedButton.h"
#include "QToast.h"

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        QWidget *central = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(central);

        // 创建卡片
        MCard *card = new MCard("欢迎使用 QtMaterial", this);
        card->setSubtitle("一套现代化的 Material Design 3 组件库");

        // 创建按钮
        MFilledButton *primaryBtn = new MFilledButton("主要操作", this);
        MOutlinedButton *secondaryBtn = new MOutlinedButton("次要操作", this);

        // 连接信号
        connect(primaryBtn, &QPushButton::clicked, [this]() {
            QToast::makeToast(this, "主要操作已执行！", QToast::LENGTH_SHORT);
        });

        connect(secondaryBtn, &QPushButton::clicked, [this]() {
            QToast::makeToast(this, "次要操作已执行！", QToast::LENGTH_SHORT);
        });

        // 组装布局
        card->setContentWidget(primaryBtn);
        layout->addWidget(card);
        layout->addWidget(secondaryBtn);

        setCentralWidget(central);
        resize(400, 300);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
```

---

## 📚 组件详细文档

### MCard - 卡片控件

> 详细的 API 文档请参阅 [MCard/README.md](src/MCard/README.md)

```cpp
// 基本用法
MCard *card = new MCard("卡片标题", this);
card->setSubtitle("副标题");
card->setContentWidget(contentWidget);

// 交互模式
card->setClickable(true);   // 可点击（默认）
card->setCheckable(true);   // 可选中
card->setChecked(false);    // 设置选中状态

// 视觉样式
card->setOutlined(true);    // 轮廓样式（无阴影，带边框）
card->setElevated(true);    // 悬浮样式（高阴影）
card->setCornerRadius(16);  // 圆角半径
```

### QToast - 吐司提示

> 详细的 API 文档请参阅 [QToast/README.md](src/QToast/README.md)

```cpp
// 最简调用
QToast::makeToast(this, "操作成功！");

// 自定义时长和位置
QToast::makeToast(this, "文件已保存", QToast::LENGTH_LONG, QToast::POSITION_TOP);

// 高级定制
QToast *toast = new QToast(this);
toast->setText("下载完成");
toast->setDuration(2000);
toast->setPosition(QToast::POSITION_CENTER);
toast->setBackgroundColor(QColor(0, 150, 0, 220));
toast->setCornerRadius(30);
toast->show();
```

### Material Design 按钮

> 详细的 API 文档请参阅 [Buttons/README.md](src/Buttons/README.md)

```cpp
// 填充按钮 - 主要操作
MFilledButton *saveBtn = new MFilledButton("保存", this);
saveBtn->setBackgroundColor(QColor("#2196F3"));
saveBtn->setCornerRadius(8);

// 描边按钮 - 次要操作
MOutlinedButton *cancelBtn = new MOutlinedButton("取消", this);
cancelBtn->setPrimaryColor(QColor("#757575"));
cancelBtn->setBorderWidth(1.5);

// 文本按钮 - 最低突出操作
MTextButton *helpBtn = new MTextButton("帮助", this);
helpBtn->setPrimaryColor(QColor("#757575"));
```

---

## 🎨 颜色系统

QtMaterial 基于 Material Design 3 颜色系统设计，所有组件共享统一的颜色方案：

| 颜色角色 | 用途 |
|---------|------|
| `primary` | 品牌色，用于主要操作、选中状态、关键元素 |
| `onPrimary` | 在 primary 上显示的文字/图标颜色 |
| `surface` | 卡片、面板等表面背景色 |
| `onSurface` | 在 surface 上显示的文字颜色（高对比度） |
| `onSurfaceVariant` | 次要文字颜色（中等对比度） |
| `outline` | 轮廓、边框颜色 |
| `background` | 页面背景色 |

### 主题适配

所有组件自动适配亮色/暗色主题：

```cpp
// 在应用启动时设置主题
void setDarkMode(bool enabled) {
    if (enabled) {
        qApp->setPalette(darkPalette);
        // 组件自动响应 QPalette 变化
    } else {
        qApp->setPalette(lightPalette);
    }
}
```

---

## 🛠️ 架构设计

### 类继承关系

```
QWidget
    ├── MWidgetBase (基类 - 颜色系统、主题适配)
            └── MCard
    └──    QToast

QPushButton
    └── MBaseButton (基类 - 水波纹、状态层、动画)
            ├── MOutlinedButton
            ├── MFilledButton
            └── MTextButton
```

### 设计原则

- **组合优于继承**：基类只提供基础能力，具体组件通过组合实现功能
- **关注点分离**：绘制逻辑、交互逻辑、业务逻辑清晰分离
- **非侵入式**：组件可直接集成到现有项目，不影响已有代码
- **最小依赖**：仅依赖 Qt 核心模块（Core、Widgets、Gui）


---

## 🔧 构建与测试

### 构建示例

**待完成**

---

## 🌍 跨平台支持

| 平台 | 支持状态 | 备注 |
|------|---------|------|
| Windows 10/11 | ✅ 完全支持 | 原生事件循环，完美阴影效果 |
| macOS 10.15+ | ✅ 完全支持 | 原生窗口透明支持 |
| Linux (X11/Wayland) | ✅ 完全支持 | 支持主流桌面环境 |
| Android 5.0+ | ✅ 完全支持 | 触摸事件穿透支持 |
| iOS 12+ | ✅ 完全支持 | 触摸事件穿透支持 |

---

## 🤝 贡献指南

欢迎贡献代码、报告问题或提出改进建议！

1. **Fork** 本仓库
2. 创建你的特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交你的修改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 提交 **Pull Request**

### 编码规范

- 遵循 Qt 编码风格
- 使用 C++11 特性
- 所有公共 API 必须有完整的文档注释
- 新增组件必须包含示例代码

---

## 📄 许可证

本项目基于 **LGPL v3** 许可证开源。

---

## 📞 联系方式

- **GitHub Issues**: [https://github.com/huanhuan0812/QtMaterial/issues](https://github.com/huanhuan0812/QtMaterial/issues)
- **邮箱**: qdhuanhuan08@outlook.com

---

## 🙏 致谢

- [Material Design 3 设计规范](https://m3.material.io/)
- [Qt 框架](https://www.qt.io/)
- 所有贡献者和使用者

---

**用 QtMaterial，让您的应用更优雅。** ✨