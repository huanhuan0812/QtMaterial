#ifndef MTHEME_H
#define MTHEME_H

#include <QObject>
#include <QColor>
#include <QFont>
#include <QMap>
#include <QString>
#include <QFontDatabase>

enum class MThemeMode {
    Light = 0,
    Dark = 1
};

class MTheme : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ColorScheme color READ color CONSTANT)
    Q_PROPERTY(Typography typography READ typography CONSTANT)
    Q_PROPERTY(Shape shape READ shape CONSTANT)
    Q_PROPERTY(Elevation elevation READ elevation CONSTANT)
    Q_PROPERTY(StateOpacity state READ state CONSTANT)

public:
    static MTheme* instance();
    
    // Color Scheme Structure
    struct ColorScheme {
        QColor primary;
        QColor onPrimaryColor;
        QColor primaryContainer;
        QColor onPrimaryContainerColor;
        QColor secondary;
        QColor onSecondaryColor;
        QColor secondaryContainer;
        QColor onSecondaryContainerColor;
        QColor tertiary;
        QColor onTertiaryColor;
        QColor tertiaryContainer;
        QColor onTertiaryContainerColor;
        QColor error;
        QColor onErrorColor;
        QColor errorContainer;
        QColor onErrorContainerColor;
        QColor background;
        QColor onBackgroundColor;
        QColor surface;
        QColor onSurfaceColor;
        QColor surfaceVariant;
        QColor onSurfaceVariantColor;
        QColor outline;
        QColor outlineVariant;
        QColor shadow;
        QColor scrim;
        QColor inverseSurface;
        QColor inverseOnSurface;
        QColor inversePrimary;
        QColor surfaceDim;
        QColor surfaceBright;
        QColor surfaceContainerLowest;
        QColor surfaceContainerLow;
        QColor surfaceContainer;
        QColor surfaceContainerHigh;
        QColor surfaceContainerHighest;
        
        // Convert to QMap for easy serialization
        QMap<QString, QColor> toMap() const;
        void fromMap(const QMap<QString, QColor>& map);
    };
    
    // Typography Structure
    struct TextStyle {
        QString family;
        int weight;
        int size;
        qreal lineHeight;
        qreal tracking;
        
        QFont toFont() const;
    };
    
    struct Typography {
        TextStyle displayLarge;
        TextStyle displayMedium;
        TextStyle displaySmall;
        TextStyle headlineLarge;
        TextStyle headlineMedium;
        TextStyle headlineSmall;
        TextStyle titleLarge;
        TextStyle titleMedium;
        TextStyle titleSmall;
        TextStyle labelLarge;
        TextStyle labelMedium;
        TextStyle labelSmall;
        TextStyle bodyLarge;
        TextStyle bodyMedium;
        TextStyle bodySmall;
    };
    
    // Shape Structure
    struct Shape {
        int cornerNone;
        int cornerExtraSmall;
        int cornerSmall;
        int cornerMedium;
        int cornerLarge;
        int cornerExtraLarge;
        int cornerFull;
    };
    
    // Elevation Structure
    struct Elevation {
        int level0;
        int level1;
        int level2;
        int level3;
        int level4;
        int level5;
    };
    
    // State Opacities Structure
    struct StateOpacity {
        qreal draggedStateLayerOpacity;
        qreal focusStateLayerOpacity;
        qreal hoverStateLayerOpacity;
        qreal pressedStateLayerOpacity;
    };

    // Getters
    ColorScheme color() const { return m_color; }
    Typography typography() const { return m_typography; }
    Shape shape() const { return m_shape; }
    Elevation elevation() const { return m_elevation; }
    StateOpacity state() const { return m_state; }
    
    // Icon Font
    QFont iconFont() const { return m_iconFont; }
    
    // Color scheme management
    void setColorScheme(const ColorScheme& scheme);
    void setColorScheme(const QMap<QString, QColor>& schemeMap);
    void resetToDefaultScheme();
    
    // Scheme presets
    static ColorScheme lightScheme();
    static ColorScheme darkScheme();
    
signals:
    void colorSchemeChanged();

private:
    explicit MTheme(QObject* parent = nullptr);
    ~MTheme();
    
    static MTheme* s_instance;
    
    // Initialize default values
    void initDefaultScheme();
    void initTypography();
    void initShape();
    void initElevation();
    void initState();
    void initIconFont();
    
    ColorScheme m_color;
    Typography m_typography;
    Shape m_shape;
    Elevation m_elevation;
    StateOpacity m_state;
    QFont m_iconFont;
};

#endif // MTHEME_H