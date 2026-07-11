## API 文档


# Material Design Buttons for Qt

一套基于 Material Design 规范的 Qt 按钮组件，包含三种按钮类型：描边按钮、填充按钮和文本按钮。

## 目录

1. [类继承关系](#类继承关系)
2. [通用 API](#通用-api)
3. [MOutlinedButton - 描边按钮](#moutlinedbutton---描边按钮)
4. [MFilledButton - 填充按钮](#mfilledbutton---填充按钮)
5. [MTextButton - 文本按钮](#mtextbutton---文本按钮)
6. [使用示例](#使用示例)
7. [注意事项](#注意事项)

---

## 类继承关系

```
QPushButton
    └── MBaseButton (基类)
            ├── MOutlinedButton (描边按钮)
            ├── MFilledButton (填充按钮)
            └── MTextButton (文本按钮)
```

---

## 通用 API

所有按钮类型共享以下属性和方法。

### 颜色属性

#### `QColor primaryColor() const`
#### `void setPrimaryColor(const QColor &color)`

获取/设置主题色。

- **默认值**: 系统高亮色 `QPalette::Highlight`
- **用途**: 
  - `MOutlinedButton`: 边框颜色、文字颜色、状态层颜色
  - `MFilledButton`: 状态层颜色、水波纹颜色
  - `MTextButton`: 文字颜色、状态层颜色

#### `QColor rippleColor() const`
#### `void setRippleColor(const QColor &color)`

获取/设置水波纹颜色。

- **默认值**: 与 `primaryColor` 相同
- **用途**: 定义点击时水波纹效果的颜色

### 尺寸属性

#### `int cornerRadius() const`
#### `void setCornerRadius(int radius)`

获取/设置圆角半径（像素）。

- **默认值**: `20`
- **取值范围**: `0` ~ `min(width, height) / 2`
- **示例**: `button->setCornerRadius(8);`

### 视觉效果开关

#### `bool isRippleEnabled() const`
#### `void setRippleEnabled(bool enabled)`

启用/禁用水波纹效果。

- **默认值**: `true`
- **说明**: 禁用后点击按钮不会产生水波纹动画

#### `bool isStateLayerEnabled() const`
#### `void setStateLayerEnabled(bool enabled)`

启用/禁用状态层效果。

- **默认值**: `true`
- **说明**: 状态层在悬停和按下时显示半透明覆盖层
  - 悬停透明度: `0.08`
  - 按下透明度: `0.12`

#### `bool isFocusIndicatorEnabled() const`
#### `void setFocusIndicatorEnabled(bool enabled)`

启用/禁用焦点指示器。

- **默认值**: `true`
- **说明**: 当按钮获得键盘焦点时显示虚线框

#### `bool isHoverElevationEnabled() const`
#### `void setHoverElevationEnabled(bool enabled)`

启用/禁用悬停提升效果。

- **默认值**: `true`
- **说明**: 悬停时按钮下方显示阴影，产生"浮起"效果

### 动画属性

#### `int animationDuration() const`
#### `void setAnimationDuration(int ms)`

获取/设置动画持续时间（毫秒）。

- **默认值**: `300`
- **取值范围**: `0` ~ `1000`
- **说明**: 影响所有过渡动画（状态层、水波纹、悬停提升等）

---

## MOutlinedButton - 描边按钮

描边按钮具有透明背景和彩色边框，适用于次要操作。

### 构造函数

```cpp
MOutlinedButton(QWidget *parent = nullptr);
MOutlinedButton(const QString &text, QWidget *parent = nullptr);
MOutlinedButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
```

### 特有属性

#### `qreal borderWidth() const`
#### `void setBorderWidth(qreal width)`

获取/设置边框宽度（像素）。

- **默认值**: `1.0`
- **取值范围**: `0.5` ~ `5.0`
- **示例**: `button->setBorderWidth(2.0);`

### 外观说明

| 状态 | 边框颜色 | 文字颜色 | 背景 |
|------|---------|---------|------|
| 正常 | 12% 透明度 OnSurface | Primary Color | 透明 |
| 悬停 | Primary Color | Primary Color | 8% Primary Color |
| 按下 | Primary Color | Primary Color | 12% Primary Color |
| 禁用 | 38% 透明度 12% 黑 | 38% 透明度 | 透明 |

---

## MFilledButton - 填充按钮

填充按钮具有彩色背景和高对比度文字，适用于主要操作。

### 构造函数

```cpp
MFilledButton(QWidget *parent = nullptr);
MFilledButton(const QString &text, QWidget *parent = nullptr);
MFilledButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
```

### 特有属性

#### `QColor backgroundColor() const`
#### `void setBackgroundColor(const QColor &color)`

获取/设置背景颜色。

- **默认值**: `primaryColor`
- **说明**: 背景色会自动影响文字颜色（自动选择白色或深灰色以保证对比度）
- **示例**: `button->setBackgroundColor(QColor("#6200EE"));`

### 外观说明

| 状态 | 背景颜色 | 文字颜色 |
|------|---------|---------|
| 正常 | Background Color | 自动对比色 |
| 悬停 | Background Color + 8% 白色/黑色层 | 自动对比色 |
| 按下 | Background Color + 12% 白色/黑色层 | 自动对比色 |
| 禁用 | 12% 透明度 OnSurface | 38% 透明度 OnSurface |

---

## MTextButton - 文本按钮

文本按钮只有文字，无边框和背景，适用于最不突出的操作。

### 构造函数

```cpp
MTextButton(QWidget *parent = nullptr);
MTextButton(const QString &text, QWidget *parent = nullptr);
MTextButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
```

### 外观说明

| 状态 | 文字颜色 | 背景 |
|------|---------|------|
| 正常 | Primary Color (100%) | 透明 |
| 悬停 | Primary Color (80%) | 8% Primary Color |
| 按下 | Primary Color (50%) | 12% Primary Color |
| 禁用 | 38% 透明度 OnSurface | 透明 |

### 特性

- 无边框
- 无阴影效果（即使启用 `hoverElevationEnabled`）
- 最小视觉重量，适合用于对话框中的取消按钮或链接式操作

---

## 使用示例

### 基础示例

```cpp
#include "MOutlinedButton.h"
#include "MFilledButton.h"
#include "MTextButton.h"

// 创建填充按钮（主要操作）
MFilledButton *saveButton = new MFilledButton("保存", this);
saveButton->setBackgroundColor(QColor("#2196F3"));
saveButton->setCornerRadius(8);

// 创建描边按钮（次要操作）
MOutlinedButton *cancelButton = new MOutlinedButton("取消", this);
cancelButton->setPrimaryColor(QColor("#757575"));
cancelButton->setCornerRadius(8);
cancelButton->setBorderWidth(1.5);

// 创建文本按钮（最不突出的操作）
MTextButton *helpButton = new MTextButton("帮助", this);
helpButton->setPrimaryColor(QColor("#757575"));
```

### 带图标的按钮

```cpp
QIcon saveIcon(":/icons/save.svg");
MFilledButton *saveButton = new MFilledButton(saveIcon, "保存", this);

QIcon cancelIcon(":/icons/cancel.svg");
MOutlinedButton *cancelButton = new MOutlinedButton(cancelIcon, "取消", this);
```

### 自定义动画速度

```cpp
MOutlinedButton *button = new MOutlinedButton("动画", this);
button->setAnimationDuration(150);  // 更快的动画
button->setHoverElevationEnabled(true);
```

### 禁用特定效果

```cpp
MFilledButton *button = new MFilledButton("简洁模式", this);

// 禁用所有视觉效果，只保留基础功能
button->setRippleEnabled(false);
button->setStateLayerEnabled(false);
button->setFocusIndicatorEnabled(false);
button->setHoverElevationEnabled(false);
```

### 完整配置示例

```cpp
// 创建一个完全自定义的描边按钮
MOutlinedButton *customButton = new MOutlinedButton("自定义按钮", this);

// 颜色配置
customButton->setPrimaryColor(QColor("#FF5722"));
customButton->setRippleColor(QColor("#FF5722"));

// 尺寸配置
customButton->setCornerRadius(25);
customButton->setBorderWidth(2.0);

// 动画配置
customButton->setAnimationDuration(400);

// 效果配置
customButton->setRippleEnabled(true);
customButton->setStateLayerEnabled(true);
customButton->setFocusIndicatorEnabled(true);
customButton->setHoverElevationEnabled(true);

// 连接信号
connect(customButton, &QPushButton::clicked, this, &MyClass::onButtonClicked);
```

### 动态启用/禁用

```cpp
MFilledButton *submitButton = new MFilledButton("提交", this);

// 根据条件禁用
submitButton->setEnabled(false);

// 禁用状态下自动应用禁用样式
// - 背景变淡
// - 文字变淡
// - 不响应鼠标事件
```

### 样式建议

根据 Material Design 规范的建议使用场景：

```cpp
// 对话框按钮组合
MFilledButton *confirmButton = new MFilledButton("确认", this);      // 主要操作
MOutlinedButton *cancelButton = new MOutlinedButton("取消", this);   // 次要操作
MTextButton *learnMoreButton = new MTextButton("了解更多", this);    // 辅助操作

// 表单按钮
MFilledButton *loginButton = new MFilledButton("登录", this);        // 主要操作
MTextButton *forgotPasswordButton = new MTextButton("忘记密码？", this); // 辅助链接

// 工具栏按钮
MOutlinedButton *filterButton = new MOutlinedButton("筛选", this);   // 中等突出
MTextButton *clearButton = new MTextButton("清除", this);            // 低突出
```

---

## 注意事项

### 1. 性能考虑

- 所有按钮都启用了抗锯齿渲染，大量按钮可能影响性能
- 动画使用 Qt 属性动画系统，性能良好
- 建议在需要大量按钮的场景（如表格）中禁用不必要的效果

### 2. 颜色对比度

- `MFilledButton` 会自动计算文字颜色以保证可读性
- 对于 `MOutlinedButton` 和 `MTextButton`，建议使用符合 WCAG 标准的颜色

### 3. 焦点指示器

- 焦点指示器默认启用，适合键盘导航
- 在触摸屏应用中可以考虑禁用 `focusIndicatorEnabled`

### 4. 最小尺寸建议

```cpp
// 建议的最小尺寸
button->setMinimumSize(64, 36);  // 宽度64px，高度36px

// 对于只有图标的按钮
button->setMinimumSize(48, 48);
```

### 5. 样式表注意事项

```cpp
// 不要使用样式表覆盖内部绘制
button->setStyleSheet("background-color: red;");  // ❌ 会破坏自定义绘制

// 使用提供的 API
button->setBackgroundColor(QColor("red"));  // ✅ 正确方式
```

### 6. 已知限制

- 图标颜色目前只在 `MFilledButton` 中自动调整
- 不支持渐变背景
- 阴影效果较简单，不支持复杂阴影配置

### 7. 主题适配

```cpp
// 跟随系统主题变化的示例
void updateButtonTheme(bool isDarkMode) {
    if (isDarkMode) {
        saveButton->setBackgroundColor(QColor("#BB86FC"));
        cancelButton->setPrimaryColor(QColor("#CFCFCF"));
    } else {
        saveButton->setBackgroundColor(QColor("#6200EE"));
        cancelButton->setPrimaryColor(QColor("#000000"));
    }
}
```

---

## 版本历史

- **v1.0.0**: 初始版本
  - 实现 MBaseButton 基类
  - 实现 MOutlinedButton、MFilledButton、MTextButton
  - 支持水波纹、状态层、焦点指示器、悬停提升等效果
  - 完整的动画系统

---

## 许可证

本项目基于 MIT 许可证开源。

## 技术支持

如有问题或建议，请提交 Issue 或 Pull Request。
