package main

// Yeah, this is basically a copy paste
// From https://docs.fyne.io/extend/custom-theme/

import (
	"image/color"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/theme"
)

type myTheme struct {
	variant fyne.ThemeVariant
}

func CreateThemeVariant(variant fyne.ThemeVariant) fyne.Theme {
	return &myTheme{variant: variant}
}

func (m *myTheme) Color(name fyne.ThemeColorName, variant fyne.ThemeVariant) color.Color {
	if variant == 0 {
		variant = m.variant
	}

	if name == theme.ColorNamePrimary || name == theme.ColorNameSelection || name == theme.ColorNameButton || name == theme.ColorNameSeparator || name == theme.ColorNameInputBorder || name == theme.ColorNameShadow {
		return color.RGBA{136, 192, 208, 255}
	}

	if name == theme.ColorNameHover {
		return color.RGBA{94, 129, 172, 255}
	}

	return theme.DefaultTheme().Color(name, variant)
}

func (m *myTheme) Icon(name fyne.ThemeIconName) fyne.Resource {
	return theme.DefaultTheme().Icon(name)
}

func (m *myTheme) Font(style fyne.TextStyle) fyne.Resource {
	return theme.DefaultTheme().Font(style)
}

func (m *myTheme) Size(name fyne.ThemeSizeName) float32 {
	return theme.DefaultTheme().Size(name)
}
