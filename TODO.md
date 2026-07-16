## **Qt Material 组件实现总表**

| 维度 | 控件名 | 实现策略 | 基类选择 | 核心绘制重写点 | 交互关键 | Qt特有技术要点 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **一、基础交互** | **MaterialButton** | **彻底重写** | `MaterialWidget` (自定义基类) | • 容器底色/圆角/禁用遮罩<br>• 涟漪（Ripple）半径动画 | • 精确点击热区 (`hitTest`)<br>• 状态优先级：禁用>按压>悬停>聚焦 | • 用`QVariantAnimation`驱动波纹半径（非`QPropertyAnimation`）<br>• 内嵌`RippleEffect`对象 |
| | **MaterialTextField** | **彻底重写** | `MaterialInputBase` | • 底部激活线（颜色过渡动画）<br>• 浮动占位符（Y轴位置动画）<br>• 辅助文本（Helper Text）换行计算 | • 输入防抖（`QTimer`控制信号频率）<br>• 聚焦/失焦状态切换 | • 在`resizeEvent`中计算辅助文本区域<br>• 接管`textChanged`信号，使用延迟发出 |
| | **MaterialSwitch** | **彻底重写** | `QAbstractButton` (非QCheckBox) | • 完全自绘滑块（Thumb）和轨道（Track）<br>• 忽略Qt原生`checkState`绘制 | • 仅在`mouseReleaseEvent`切换状态<br>• 不支持拖拽滑动（与iOS区分） | • 重写`hitTest`扩大点击区域（≥48dp） |
| | **MaterialSlider** | **彻底重写** | `QWidget` (非QSlider) | • 绘制分段刻度（Discrete Ticks小圆点）<br>• 数值Tooltip实时跟随 | • 鼠标X轴→数值映射算法<br>• 键盘方向键步进控制 | • 在`mouseMoveEvent`中持续更新Tooltip位置<br>• 重写`wheelEvent`实现精细调节 |
| **二、复合控件** | **MaterialAppBar** | **组合式重写** | `QFrame` + `QHBoxLayout` | • 底部1px阴影（Elevation 4）<br>• Title位置动态计算（左右按钮偏移）<br>• 滚动折叠动画（高度变化） | • 连接外部滚动条值变化实现折叠<br>• 返回箭头与菜单按钮自动适配 | • `setMaximumHeight`配合动画实现折叠<br>• 在`paintEvent`中绘制投影（性能优） |
| | **MaterialCard** | **组合式重写** | `QFrame` + GraphicsEffect | • **不建议**重写绘制阴影（性能差）<br>• 使用`QGraphicsDropShadowEffect`并缓存为`QPixmap` | • 按压时Elevation动态变化（2→8）<br>• 鼠标悬停背景微变 | • 在`mousePressEvent`中修改GraphicsEffect参数<br>• 启用`setCacheMode`提升重绘性能 |
| | **MaterialBottomNav** | **组合式重写** | `QWidget` + 自定义布局 | • 顶部分隔线（淡灰色，1px）<br>• 根据宽度自动切换显示模式（3个以下全Label，4个以上部分隐藏） | • 点击Item切换选中状态<br>• 选中指示器（下划线或颜色块）滑动动画 | • 重写`layoutSubviews`自行布局<br>• 用`QPropertyAnimation`驱动指示器移动 |
| **三、特有控件** | **FAB (浮动按钮)** | **从零创建** | `MaterialButtonBase` (固定圆形) | • `setClipPath`强制圆形裁剪<br>• 固定`sizeHint`为(56,56)/(40,40)<br>• 展开为菜单的旋转动画 | • 支持Mini/FAB两种尺寸<br>• 扩展动作菜单弹出逻辑 | • 用`QParallelAnimationGroup`组合旋转+透明度动画<br>• 重写`showEvent`实现展开效果 |
| | **Snackbar** | **从零创建** | `QFrame` (作为顶层子对象) | • 从底部滑入+淡入并行动画<br>• 自动消失定时器（鼠标悬停暂停） | • Action按钮点击信号转发<br>• 多Snackbar排队显示机制 | • 重写`eventFilter`拦截Action按钮事件<br>• 用`QPropertyAnimation`同时驱动位置和透明度 |
| | **BottomSheet** | **从零创建** | `QDialog` + `Qt::Popup` | • 绘制顶部拖拽手柄（横线）<br>• 三种状态：Peek/Expanded/Collapsed | • 鼠标拖拽高度变化（实时计算）<br>• ESC键先折叠再关闭（非直接`reject`） | • 重写`mouseMoveEvent`实现拖拽<br>• 重写`keyPressEvent`拦截ESC（调用折叠动画） |
| | **Chip (标签)** | **从零创建** | `QFrame` + 内部布局 | • 绘制胶囊背景（含边框）<br>• Hover状态背景色微变<br>• 选中状态高亮 | • 提供`setSelectable`接口<br>• 删除按钮点击移除Chip | • `enterEvent`/`leaveEvent`控制悬停状态<br>• Icon+Label+CloseButton组合布局 |
| | **DataTable** | **从零创建** | `QTableView` (完全重写paintEvent) | • 取消网格线→绘制水平分割线<br>• 表头底部1px下划线<br>• 行交替颜色（`alternatingRowColors`） | • 行悬停高亮<br>• 排序指示器（箭头）重绘 | • 重写`drawHeader`/`drawRow`<br>• 禁用Qt原生网格绘制（`setShowGrid(false)`） |
| **⚠️ 避免重写** | **QMenu / 下拉框** | 样式表+Native | `QMenu` + `setStyleSheet` | • 修改背景色和圆角<br>• **不重写paintEvent** | • 保留原生事件循环<br>• 保留焦点管理 | • 跨平台兼容性优先<br>• 自绘极易导致焦点丢失 |
| | **QScrollBar** | 部分重写 | 继承`QScrollBar` | • 仅重写`paintEvent`修改滑块颜色<br>• 保留滚动逻辑（`setSingleStep`） | • 自动隐藏（不重写滚轮事件） | • 保留Qt原生滚动算法 |

---

## **配套工程建议**

### **1. 状态绘制器（MaterialPainter）**
为避免重复代码，创建统一绘制工具类：
```cpp
class MaterialPainter {
public:
    static void drawRipple(QPainter* p, QPoint center, qreal radius, QColor color);
    static void drawDisabledOverlay(QPainter* p, QRect rect);   // 灰色遮罩 + 禁用图标
    static void drawFocusRing(QPainter* p, QRect rect, QColor color, int width=2);
    static void drawHoverBackground(QPainter* p, QRect rect, QColor color, qreal opacity=0.08);
};
```
- **调用位置**：所有控件的`paintEvent`末段调用
- **状态优先级统一**：`禁用 > 按压 > 悬停 > 聚焦 > 默认`

### **2. 颜色Token映射表（MaterialColorScheme）**
建议使用枚举+`QColor`映射，避免硬编码：
```cpp
enum class MaterialColorRole {
    Primary, OnPrimary, PrimaryContainer, 
    Secondary, OnSecondary,
    Surface, OnSurface, SurfaceVariant,
    Error, OnError,
    Outline, OutlineVariant
};
```
- **Button**使用`Primary`/`OnPrimary`
- **TextField**使用`Primary`（激活线）/`OnSurface`（占位符）
- **Card**使用`Surface`/`OnSurface`

### **3. 动画统一管理**
- 所有动画时长遵循Material规范（**300ms**标准，**150ms**快速）
- 使用`QAnimationGroup`组合复杂动画（FAB展开、BottomSheet拖拽）
- 动画曲线推荐：`QEasingCurve::OutCubic`（缓出）和`InOutCubic`（缓入缓出）