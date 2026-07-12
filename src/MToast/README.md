# QToast 吐司提示组件

轻量级、跨平台的 Qt 吐司（Toast）提示模块，提供非侵入式的短暂消息通知功能。**支持 Windows、macOS、Linux、Android、iOS** 五大平台。

## 功能特性

- **鼠标/触摸穿透**：默认启用输入穿透，点击/触摸 Toast 不会干扰底层界面操作
- **流畅动画**：内置淡入淡出动画效果
- **三种预设位置**：顶部 / 居中 / 底部
- **高度可定制**：背景色、文字色、圆角、字体、边距、最大宽度等均可自由配置
- **自动回收**：显示并淡出后自动销毁，无需手动管理内存
- **跨平台**：全面支持 Windows、macOS、Linux、Android、iOS

## 平台适配说明

| 平台 | 输入穿透实现方式 |
|------|------------------|
| **Windows** | `nativeEvent` 处理 `WM_NCHITTEST` 返回 `HTTRANSPARENT` |
| **macOS / iOS** | `Qt::WindowTransparentForInput` 窗口标志（Qt 5.15+） |
| **Linux** | `Qt::WindowTransparentForInput` 窗口标志（Qt 5.15+） |
| **Android** | `Qt::WindowTransparentForInput` 窗口标志 + `WA_TransparentForMouseEvents` |

> **注意**：在 Android 和 iOS 上，建议使用 Qt 5.15 或更高版本以获得最佳的触摸穿透体验。

## 使用方式

### 静态方法（最简调用）

```cpp
#include "QToast.h"

// 默认样式：底部显示 1.5 秒
QToast::makeToast(this, "操作成功！");

// 长提示 3 秒，位于顶部
QToast::makeToast(this, "文件已保存", QToast::LENGTH_LONG, QToast::POSITION_TOP);

// 完全自定义样式
QToast::makeToast(this, 
                  "自定义提示", 
                  QToast::LENGTH_SHORT, 
                  QToast::POSITION_CENTER, 
                  18,                                    // 字体大小
                  QColor(255, 100, 100, 200),            // 背景色（支持透明）
                  Qt::black);                            // 文字颜色
```

### 对象模式（高级定制）

```cpp
QToast *toast = new QToast(this);
toast->setText("下载完成");
toast->setDuration(2000);
toast->setPosition(QToast::POSITION_TOP);
toast->setBackgroundColor(QColor(0, 150, 0, 220));
toast->setCornerRadius(30);
toast->setMargins(50, 20);
toast->setFontSize(20);
toast->show();
```

## API 参考

| 方法 | 参数 | 说明 |
|------|------|------|
| `setText(const QString &)` | 文本内容 | 设置提示文字 |
| `setDuration(int ms)` | 毫秒数 | 显示时长，默认 1500ms |
| `setPosition(ToastPosition)` | `POSITION_BOTTOM` / `CENTER` / `TOP` | 显示位置 |
| `setBackgroundColor(const QColor&)` | 颜色值 | 背景色（支持透明度） |
| `setTextColor(const QColor&)` | 颜色值 | 文字颜色 |
| `setCornerRadius(int)` | 像素值 | 圆角半径 |
| `setMargins(int, int)` | 水平、垂直边距 | 内边距 |
| `setMaxWidth(int)` | 像素值 | 最大宽度限制 |
| `setFontSize(int)` | 像素值 | 字体大小 |
| `setBottomMarginRatio(qreal)` | 0.0~0.5 | 底部边距占屏幕高度比例（默认 0.08） |
| `setMinBottomMargin(int)` | 像素值 | 底部最小边距 |
| `show()` | 无 | 显示 Toast |

### 静态方法 `makeToast`

```cpp
// 简化版
static void makeToast(QWidget *parent, const QString &text, 
                      int duration = LENGTH_SHORT,
                      ToastPosition position = POSITION_BOTTOM);

// 完整版
static void makeToast(QWidget *parent, const QString &text, 
                      int duration, ToastPosition position,
                      int fontSize,
                      const QColor &backgroundColor = QColor(40, 40, 40, 230),
                      const QColor &textColor = Qt::white);
```

## 预定义常量

```cpp
// 显示时长
QToast::LENGTH_SHORT   // 1500ms
QToast::LENGTH_LONG    // 3000ms

// 显示位置
QToast::POSITION_BOTTOM  // 底部（默认）
QToast::POSITION_CENTER  // 居中
QToast::POSITION_TOP     // 顶部
```

## 注意事项

1. **输入穿透是设计特性**：Toast 默认不拦截任何输入事件，确保不干扰用户操作
2. **父对象可为空**：`parent` 参数传 `nullptr` 也可正常工作
3. **自动销毁**：Toast 淡出后自动调用 `deleteLater`，无需额外处理
4. **多屏支持**：自动适配主屏幕尺寸，位置计算基于 `primaryScreen()`
5. **移动端适配**：在 Android/iOS 上自动适配屏幕密度和触摸事件