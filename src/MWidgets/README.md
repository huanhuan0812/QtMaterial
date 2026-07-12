# MCard - Material Design 3 卡片控件

## 概述

MCard 是一个基于 Material Design 3 设计规范的现代化卡片控件，继承自 `MWidgetBase`，提供了丰富的交互效果和高度可定制的视觉样式。它支持阴影、圆角、悬停、点击、选中等多种交互状态，并自动适配主题颜色。

## 特性

- ✨ **Material Design 3 风格**：遵循最新的 Material Design 规范
- 🎨 **丰富的视觉样式**：支持填充、轮廓、悬浮等多种卡片样式
- 🖱️ **完整的交互反馈**：悬停、点击、选中、按压等状态动画
- 📐 **自适应布局**：根据内容自动调整尺寸，同时支持手动尺寸控制
- 🌓 **主题适配**：自动适配亮色/暗色主题
- 🔧 **高度可定制**：颜色、圆角、阴影、边距等均可自定义

## 快速开始

### 基本用法

```cpp
// 创建卡片
MCard *card = new MCard(this);
card->setTitle("卡片标题");
card->setSubtitle("卡片副标题");

// 设置内容
QWidget *content = new QWidget();
// ... 添加内容控件 ...
card->setContentWidget(content);
```

### 带标题的快捷构造

```cpp
MCard *card = new MCard("卡片标题", this);
card->setSubtitle("副标题");
```

## 核心功能

### 1. 内容管理

| 方法 | 描述 |
|------|------|
| `setTitle(const QString &)` | 设置卡片标题 |
| `setSubtitle(const QString &)` | 设置副标题 |
| `setContentWidget(QWidget *)` | 设置内容控件 |

### 2. 交互模式

| 方法 | 描述 |
|------|------|
| `setClickable(bool)` | 设置是否可点击，默认为 `true` |
| `setCheckable(bool)` | 设置是否可选中 |
| `setChecked(bool)` | 设置选中状态 |
| `isChecked()` | 获取选中状态 |

**信号：**
- `clicked()` - 点击时触发
- `pressed()` - 按压时触发
- `released()` - 释放时触发
- `checkedChanged(bool)` - 选中状态变化时触发
- `entered()` - 鼠标进入时触发
- `left()` - 鼠标离开时触发

### 3. 视觉样式

| 方法 | 描述 |
|------|------|
| `setOutlined(bool)` | 设置为轮廓样式（无阴影，带边框） |
| `setElevated(bool)` | 设置为悬浮样式（高阴影） |
| `setCornerRadius(qreal)` | 设置圆角半径，默认 12px |
| `setShadowEnabled(bool)` | 启用/禁用阴影 |

### 4. 尺寸控制

卡片支持两种尺寸管理模式：

**自动模式（默认）：**
```cpp
card->setAutoResize(true);  // 根据内容自动调整大小
```

**手动模式：**
```cpp
card->setFixedWidth(300);
card->setFixedHeight(200);
```

**尺寸限制：**
- 最小宽度：100px
- 最小高度：80px

所有尺寸设置方法都会自动应用最小尺寸限制，确保卡片始终可用。

## 示例

### 基础卡片

```cpp
MCard *card = new MCard("用户信息", this);
card->setSubtitle("点击查看详情");

QLabel *label = new QLabel("姓名：张三\n年龄：25岁");
label->setAlignment(Qt::AlignCenter);
card->setContentWidget(label);

connect(card, &MCard::clicked, []() {
    qDebug() << "卡片被点击";
});
```

### 可选择卡片

```cpp
MCard *card = new MCard("选择此项", this);
card->setCheckable(true);

connect(card, &MCard::checkedChanged, [](bool checked) {
    qDebug() << "选中状态：" << checked;
});
```

### 轮廓样式卡片

```cpp
MCard *card = new MCard("轮廓卡片", this);
card->setOutlined(true);  // 无阴影，带边框
card->setClickable(true);
```

### 悬浮样式卡片

```cpp
MCard *card = new MCard("悬浮卡片", this);
card->setElevated(true);  // 高阴影效果
```

## 架构设计

### 类继承关系

```
QWidget
    └── MWidgetBase (基类)
            └── MCard
```

### MCard 结构

```
┌Header (标题 + 副标题) ──────────┐
│  ┌──────────────────────────┐  │
│  │  内容容器                  │ │
│  │  (可自定义内容控件)         │ │
│  │                          │ │
│  └──────────────────────────┘ │
└──────────────────────────────┘
```

## 样式定制

### 颜色系统

卡片使用 `MWidgetBase` 的 `MColorScheme` 颜色系统：

| 颜色属性 | 用途 |
|---------|------|
| `primary` | 选中状态文字颜色 |
| `surface` | 卡片背景色 |
| `onSurface` | 标题文字颜色 |
| `onSurfaceVariant` | 副标题文字颜色 |
| `outline` | 轮廓边框颜色 |

### 交互状态视觉反馈

| 状态 | 效果 |
|------|------|
| 悬停 | 背景轻微变暗，阴影加深 |
| 按压 | 阴影压缩 |
| 选中 | 背景变为主题色（透明度 30），文字变为主题色 |
| 轮廓悬停 | 边框变为实线 |
| 轮廓选中 | 边框加粗并变为主题色 |

## 文件说明

| 文件 | 描述 |
|------|------|
| `MCard.h` | 卡片控件头文件 |
| `MCard.cpp` | 卡片控件实现 |
| `MWidgetBase.h` | 基类控件头文件 |
| `MWidgetBase.cpp` | 基类控件实现 |

## 依赖

- Qt 5.12+ 或 Qt 6.x
- C++11 或更高版本

## 注意事项

1. 卡片内容控件会自动设置为透明背景，请避免在内容控件上设置不透明背景
2. 使用 `setFixedHeight()` 时，高度不能低于 `MIN_CARD_HEIGHT`（80px）
3. 轮廓样式和悬浮样式互斥，设置其中一个会自动取消另一个
4. 阴影需要 `QGraphicsDropShadowEffect` 支持，确保 GPU 加速已启用

## 许可

本控件遵循项目整体许可协议。