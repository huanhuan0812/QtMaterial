#include "MTheme.h"
#include <QDebug>

// Static instance
MTheme* MTheme::s_instance = nullptr;

MTheme* MTheme::instance()
{
    if (!s_instance) {
        s_instance = new MTheme();
    }
    return s_instance;
}

MTheme::MTheme(QObject* parent) 
    : QObject(parent)
{
    initDefaultScheme();
    initTypography();
    initShape();
    initElevation();
    initState();
    initIconFont();
}

MTheme::~MTheme()
{
    s_instance = nullptr;
}

// ColorScheme to QMap conversion
QMap<QString, QColor> MTheme::ColorScheme::toMap() const
{
    QMap<QString, QColor> map;
    map["primary"] = primary;
    map["onPrimaryColor"] = onPrimaryColor;
    map["primaryContainer"] = primaryContainer;
    map["onPrimaryContainerColor"] = onPrimaryContainerColor;
    map["secondary"] = secondary;
    map["onSecondaryColor"] = onSecondaryColor;
    map["secondaryContainer"] = secondaryContainer;
    map["onSecondaryContainerColor"] = onSecondaryContainerColor;
    map["tertiary"] = tertiary;
    map["onTertiaryColor"] = onTertiaryColor;
    map["tertiaryContainer"] = tertiaryContainer;
    map["onTertiaryContainerColor"] = onTertiaryContainerColor;
    map["error"] = error;
    map["onErrorColor"] = onErrorColor;
    map["errorContainer"] = errorContainer;
    map["onErrorContainerColor"] = onErrorContainerColor;
    map["background"] = background;
    map["onBackgroundColor"] = onBackgroundColor;
    map["surface"] = surface;
    map["onSurfaceColor"] = onSurfaceColor;
    map["surfaceVariant"] = surfaceVariant;
    map["onSurfaceVariantColor"] = onSurfaceVariantColor;
    map["outline"] = outline;
    map["outlineVariant"] = outlineVariant;
    map["shadow"] = shadow;
    map["scrim"] = scrim;
    map["inverseSurface"] = inverseSurface;
    map["inverseOnSurface"] = inverseOnSurface;
    map["inversePrimary"] = inversePrimary;
    map["surfaceDim"] = surfaceDim;
    map["surfaceBright"] = surfaceBright;
    map["surfaceContainerLowest"] = surfaceContainerLowest;
    map["surfaceContainerLow"] = surfaceContainerLow;
    map["surfaceContainer"] = surfaceContainer;
    map["surfaceContainerHigh"] = surfaceContainerHigh;
    map["surfaceContainerHighest"] = surfaceContainerHighest;
    return map;
}

void MTheme::ColorScheme::fromMap(const QMap<QString, QColor>& map)
{
    primary = map.value("primary");
    onPrimaryColor = map.value("onPrimaryColor");
    primaryContainer = map.value("primaryContainer");
    onPrimaryContainerColor = map.value("onPrimaryContainerColor");
    secondary = map.value("secondary");
    onSecondaryColor = map.value("onSecondaryColor");
    secondaryContainer = map.value("secondaryContainer");
    onSecondaryContainerColor = map.value("onSecondaryContainerColor");
    tertiary = map.value("tertiary");
    onTertiaryColor = map.value("onTertiaryColor");
    tertiaryContainer = map.value("tertiaryContainer");
    onTertiaryContainerColor = map.value("onTertiaryContainerColor");
    error = map.value("error");
    onErrorColor = map.value("onErrorColor");
    errorContainer = map.value("errorContainer");
    onErrorContainerColor = map.value("onErrorContainerColor");
    background = map.value("background");
    onBackgroundColor = map.value("onBackgroundColor");
    surface = map.value("surface");
    onSurfaceColor = map.value("onSurfaceColor");
    surfaceVariant = map.value("surfaceVariant");
    onSurfaceVariantColor = map.value("onSurfaceVariantColor");
    outline = map.value("outline");
    outlineVariant = map.value("outlineVariant");
    shadow = map.value("shadow");
    scrim = map.value("scrim");
    inverseSurface = map.value("inverseSurface");
    inverseOnSurface = map.value("inverseOnSurface");
    inversePrimary = map.value("inversePrimary");
    surfaceDim = map.value("surfaceDim");
    surfaceBright = map.value("surfaceBright");
    surfaceContainerLowest = map.value("surfaceContainerLowest");
    surfaceContainerLow = map.value("surfaceContainerLow");
    surfaceContainer = map.value("surfaceContainer");
    surfaceContainerHigh = map.value("surfaceContainerHigh");
    surfaceContainerHighest = map.value("surfaceContainerHighest");
}

// Convert TextStyle to QFont
QFont MTheme::TextStyle::toFont() const
{
    QFont font(family);
    font.setWeight(static_cast<QFont::Weight>(weight));
    font.setPixelSize(size);
    // lineHeight and tracking would need custom handling
    // as QFont doesn't directly support these properties
    return font;
}

// Default color scheme (matching QML defaultScheme)
void MTheme::initDefaultScheme()
{
    m_color.primary = QColor("#6750A4");
    m_color.onPrimaryColor = QColor("#FFFFFF");
    m_color.primaryContainer = QColor("#EADDFF");
    m_color.onPrimaryContainerColor = QColor("#21005D");
    m_color.secondary = QColor("#625B71");
    m_color.onSecondaryColor = QColor("#FFFFFF");
    m_color.secondaryContainer = QColor("#E8DEF8");
    m_color.onSecondaryContainerColor = QColor("#1D192B");
    m_color.tertiary = QColor("#7D5260");
    m_color.onTertiaryColor = QColor("#FFFFFF");
    m_color.tertiaryContainer = QColor("#FFD8E4");
    m_color.onTertiaryContainerColor = QColor("#31111D");
    m_color.error = QColor("#B3261E");
    m_color.onErrorColor = QColor("#FFFFFF");
    m_color.errorContainer = QColor("#F9DEDC");
    m_color.onErrorContainerColor = QColor("#410E0B");
    m_color.background = QColor("#FFFBFE");
    m_color.onBackgroundColor = QColor("#1C1B1F");
    m_color.surface = QColor("#FFFBFE");
    m_color.onSurfaceColor = QColor("#1C1B1F");
    m_color.surfaceVariant = QColor("#E7E0EC");
    m_color.onSurfaceVariantColor = QColor("#49454F");
    m_color.outline = QColor("#79747E");
    m_color.outlineVariant = QColor("#CAC4D0");
    m_color.shadow = QColor("#000000");
    m_color.scrim = QColor("#000000");
    m_color.inverseSurface = QColor("#313033");
    m_color.inverseOnSurface = QColor("#F4EFF4");
    m_color.inversePrimary = QColor("#D0BCFF");
    m_color.surfaceDim = QColor("#DED8E1");
    m_color.surfaceBright = QColor("#FEF7FF");
    m_color.surfaceContainerLowest = QColor("#FFFFFF");
    m_color.surfaceContainerLow = QColor("#F7F2FA");
    m_color.surfaceContainer = QColor("#F3EDF7");
    m_color.surfaceContainerHigh = QColor("#ECE6F0");
    m_color.surfaceContainerHighest = QColor("#E6E0E9");
}

// Light scheme preset
MTheme::ColorScheme MTheme::lightScheme()
{
    ColorScheme scheme;
    scheme.primary = QColor("#6750A4");
    scheme.onPrimaryColor = QColor("#FFFFFF");
    scheme.primaryContainer = QColor("#EADDFF");
    scheme.onPrimaryContainerColor = QColor("#21005D");
    scheme.secondary = QColor("#625B71");
    scheme.onSecondaryColor = QColor("#FFFFFF");
    scheme.secondaryContainer = QColor("#E8DEF8");
    scheme.onSecondaryContainerColor = QColor("#1D192B");
    scheme.tertiary = QColor("#7D5260");
    scheme.onTertiaryColor = QColor("#FFFFFF");
    scheme.tertiaryContainer = QColor("#FFD8E4");
    scheme.onTertiaryContainerColor = QColor("#31111D");
    scheme.error = QColor("#B3261E");
    scheme.onErrorColor = QColor("#FFFFFF");
    scheme.errorContainer = QColor("#F9DEDC");
    scheme.onErrorContainerColor = QColor("#410E0B");
    scheme.background = QColor("#FFFBFE");
    scheme.onBackgroundColor = QColor("#1C1B1F");
    scheme.surface = QColor("#FFFBFE");
    scheme.onSurfaceColor = QColor("#1C1B1F");
    scheme.surfaceVariant = QColor("#E7E0EC");
    scheme.onSurfaceVariantColor = QColor("#49454F");
    scheme.outline = QColor("#79747E");
    scheme.outlineVariant = QColor("#CAC4D0");
    scheme.shadow = QColor("#000000");
    scheme.scrim = QColor("#000000");
    scheme.inverseSurface = QColor("#313033");
    scheme.inverseOnSurface = QColor("#F4EFF4");
    scheme.inversePrimary = QColor("#D0BCFF");
    scheme.surfaceDim = QColor("#DED8E1");
    scheme.surfaceBright = QColor("#FEF7FF");
    scheme.surfaceContainerLowest = QColor("#FFFFFF");
    scheme.surfaceContainerLow = QColor("#F7F2FA");
    scheme.surfaceContainer = QColor("#F3EDF7");
    scheme.surfaceContainerHigh = QColor("#ECE6F0");
    scheme.surfaceContainerHighest = QColor("#E6E0E9");
    return scheme;
}

// Dark scheme preset (inverse of light)
MTheme::ColorScheme MTheme::darkScheme()
{
    ColorScheme scheme;
    scheme.primary = QColor("#D0BCFF");
    scheme.onPrimaryColor = QColor("#381E72");
    scheme.primaryContainer = QColor("#4F378B");
    scheme.onPrimaryContainerColor = QColor("#EADDFF");
    scheme.secondary = QColor("#CCC2DC");
    scheme.onSecondaryColor = QColor("#332D41");
    scheme.secondaryContainer = QColor("#4A4458");
    scheme.onSecondaryContainerColor = QColor("#E8DEF8");
    scheme.tertiary = QColor("#EFB8C8");
    scheme.onTertiaryColor = QColor("#492532");
    scheme.tertiaryContainer = QColor("#633B48");
    scheme.onTertiaryContainerColor = QColor("#FFD8E4");
    scheme.error = QColor("#F2B8B5");
    scheme.onErrorColor = QColor("#601410");
    scheme.errorContainer = QColor("#8C1D18");
    scheme.onErrorContainerColor = QColor("#F9DEDC");
    scheme.background = QColor("#1C1B1F");
    scheme.onBackgroundColor = QColor("#E6E1E5");
    scheme.surface = QColor("#1C1B1F");
    scheme.onSurfaceColor = QColor("#E6E1E5");
    scheme.surfaceVariant = QColor("#49454F");
    scheme.onSurfaceVariantColor = QColor("#CAC4D0");
    scheme.outline = QColor("#938F99");
    scheme.outlineVariant = QColor("#49454F");
    scheme.shadow = QColor("#000000");
    scheme.scrim = QColor("#000000");
    scheme.inverseSurface = QColor("#E6E1E5");
    scheme.inverseOnSurface = QColor("#313033");
    scheme.inversePrimary = QColor("#6750A4");
    scheme.surfaceDim = QColor("#141316");
    scheme.surfaceBright = QColor("#3B383E");
    scheme.surfaceContainerLowest = QColor("#0F0E12");
    scheme.surfaceContainerLow = QColor("#1C1B1F");
    scheme.surfaceContainer = QColor("#211F24");
    scheme.surfaceContainerHigh = QColor("#2B292F");
    scheme.surfaceContainerHighest = QColor("#36343B");
    return scheme;
}

// Typography initialization (matching QML typography)
void MTheme::initTypography()
{
    // Display Large
    m_typography.displayLarge.family = "Roboto";
    m_typography.displayLarge.weight = QFont::Normal;
    m_typography.displayLarge.size = 57;
    m_typography.displayLarge.lineHeight = 64;
    m_typography.displayLarge.tracking = -0.25;
    
    // Display Medium
    m_typography.displayMedium.family = "Roboto";
    m_typography.displayMedium.weight = QFont::Normal;
    m_typography.displayMedium.size = 45;
    m_typography.displayMedium.lineHeight = 52;
    m_typography.displayMedium.tracking = 0;
    
    // Display Small
    m_typography.displaySmall.family = "Roboto";
    m_typography.displaySmall.weight = QFont::Normal;
    m_typography.displaySmall.size = 36;
    m_typography.displaySmall.lineHeight = 44;
    m_typography.displaySmall.tracking = 0;
    
    // Headline Large
    m_typography.headlineLarge.family = "Roboto";
    m_typography.headlineLarge.weight = QFont::Normal;
    m_typography.headlineLarge.size = 32;
    m_typography.headlineLarge.lineHeight = 40;
    m_typography.headlineLarge.tracking = 0;
    
    // Headline Medium
    m_typography.headlineMedium.family = "Roboto";
    m_typography.headlineMedium.weight = QFont::Normal;
    m_typography.headlineMedium.size = 28;
    m_typography.headlineMedium.lineHeight = 36;
    m_typography.headlineMedium.tracking = 0;
    
    // Headline Small
    m_typography.headlineSmall.family = "Roboto";
    m_typography.headlineSmall.weight = QFont::Normal;
    m_typography.headlineSmall.size = 24;
    m_typography.headlineSmall.lineHeight = 32;
    m_typography.headlineSmall.tracking = 0;
    
    // Title Large
    m_typography.titleLarge.family = "Roboto";
    m_typography.titleLarge.weight = QFont::Normal;
    m_typography.titleLarge.size = 22;
    m_typography.titleLarge.lineHeight = 28;
    m_typography.titleLarge.tracking = 0;
    
    // Title Medium
    m_typography.titleMedium.family = "Roboto";
    m_typography.titleMedium.weight = QFont::Medium;
    m_typography.titleMedium.size = 16;
    m_typography.titleMedium.lineHeight = 24;
    m_typography.titleMedium.tracking = 0.15;
    
    // Title Small
    m_typography.titleSmall.family = "Roboto";
    m_typography.titleSmall.weight = QFont::Medium;
    m_typography.titleSmall.size = 14;
    m_typography.titleSmall.lineHeight = 20;
    m_typography.titleSmall.tracking = 0.1;
    
    // Label Large
    m_typography.labelLarge.family = "Roboto";
    m_typography.labelLarge.weight = QFont::Medium;
    m_typography.labelLarge.size = 14;
    m_typography.labelLarge.lineHeight = 20;
    m_typography.labelLarge.tracking = 0.1;
    
    // Label Medium
    m_typography.labelMedium.family = "Roboto";
    m_typography.labelMedium.weight = QFont::Medium;
    m_typography.labelMedium.size = 12;
    m_typography.labelMedium.lineHeight = 16;
    m_typography.labelMedium.tracking = 0.5;
    
    // Label Small
    m_typography.labelSmall.family = "Roboto";
    m_typography.labelSmall.weight = QFont::Medium;
    m_typography.labelSmall.size = 11;
    m_typography.labelSmall.lineHeight = 16;
    m_typography.labelSmall.tracking = 0.5;
    
    // Body Large
    m_typography.bodyLarge.family = "Roboto";
    m_typography.bodyLarge.weight = QFont::Normal;
    m_typography.bodyLarge.size = 16;
    m_typography.bodyLarge.lineHeight = 24;
    m_typography.bodyLarge.tracking = 0.5;
    
    // Body Medium
    m_typography.bodyMedium.family = "Roboto";
    m_typography.bodyMedium.weight = QFont::Normal;
    m_typography.bodyMedium.size = 14;
    m_typography.bodyMedium.lineHeight = 20;
    m_typography.bodyMedium.tracking = 0.25;
    
    // Body Small
    m_typography.bodySmall.family = "Roboto";
    m_typography.bodySmall.weight = QFont::Normal;
    m_typography.bodySmall.size = 12;
    m_typography.bodySmall.lineHeight = 16;
    m_typography.bodySmall.tracking = 0.4;
}

// Shape initialization
void MTheme::initShape()
{
    m_shape.cornerNone = 0;
    m_shape.cornerExtraSmall = 4;
    m_shape.cornerSmall = 8;
    m_shape.cornerMedium = 12;
    m_shape.cornerLarge = 16;
    m_shape.cornerExtraLarge = 28;
    m_shape.cornerFull = 1000;
}

// Elevation initialization
void MTheme::initElevation()
{
    m_elevation.level0 = 0;
    m_elevation.level1 = 1;
    m_elevation.level2 = 3;
    m_elevation.level3 = 6;
    m_elevation.level4 = 8;
    m_elevation.level5 = 12;
}

// State opacities initialization
void MTheme::initState()
{
    m_state.draggedStateLayerOpacity = 0.16;
    m_state.focusStateLayerOpacity = 0.12;
    m_state.hoverStateLayerOpacity = 0.08;
    m_state.pressedStateLayerOpacity = 0.12;
}

// Icon font initialization
void MTheme::initIconFont()
{
    // Try to load the icon font from assets
    int fontId = QFontDatabase::addApplicationFont(":/assets/MaterialIconsRound-Regular.otf");
    if (fontId != -1) {
        QStringList families = QFontDatabase::applicationFontFamilies(fontId);
        if (!families.isEmpty()) {
            m_iconFont.setFamily(families.first());
        }
    } else {
        // Fallback to a system font if icon font not found
        m_iconFont.setFamily("Material Icons Round");
        m_iconFont.setHintingPreference(QFont::PreferNoHinting);
    }
    m_iconFont.setPixelSize(24); // Default icon size
}

// Set color scheme from ColorScheme struct
void MTheme::setColorScheme(const ColorScheme& scheme)
{
    m_color = scheme;
    emit colorSchemeChanged();
}

// Set color scheme from QMap
void MTheme::setColorScheme(const QMap<QString, QColor>& schemeMap)
{
    m_color.fromMap(schemeMap);
    emit colorSchemeChanged();
}

// Reset to default scheme
void MTheme::resetToDefaultScheme()
{
    initDefaultScheme();
    emit colorSchemeChanged();
}