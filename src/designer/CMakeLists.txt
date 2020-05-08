include(ECMAddQtDesignerPlugin)

ecm_qtdesignerplugin_widget(KActionSelector
    TOOLTIP "A widget for selecting and arranging actions/objects"
    GROUP "Views (KF5)"
)
ecm_qtdesignerplugin_widget(KAnimatedButton
    TOOLTIP "An extended version of QToolButton which can display an animated icon."
    GROUP "Display (KF5)"
)
ecm_qtdesignerplugin_widget(KBusyIndicatorWidget
    TOOLTIP "Busy Indicator (KF5)"
    GROUP "Display (KF5)"
)
ecm_qtdesignerplugin_widget(KCapacityBar
    TOOLTIP "Capacity Bar (KF5)"
    CONSTRUCTOR_ARGS_CODE "(KCapacityBar::DrawTextOutline, parent)"
    GROUP "Display (KF5)"
)
ecm_qtdesignerplugin_widget(KCharSelect
    TOOLTIP "Character Selection Widget (KF5)"
    WHATSTHIS "A widget that allows selection a character out ot a table"
    GROUP "Graphics (KF5)"
)
ecm_qtdesignerplugin_widget(KCollapsibleGroupBox
    TOOLTIP "A group box which can be expanded and collapsed"
    GROUP "Display (KF5)"
    CONTAINER
)
ecm_qtdesignerplugin_widget(KColorButton
    TOOLTIP "Color Chooser Button (KF5)"
    WHATSTHIS "A button that allows selecting a color"
    GROUP "Graphics (KF5)"
)
ecm_qtdesignerplugin_widget(KColorCombo
    TOOLTIP "Color Combo Box (KF5)"
    GROUP "Graphics (KF5)"
)
ecm_qtdesignerplugin_widget(KDatePicker
    TOOLTIP "A date selection widget (KF5)"
    WHATSTHIS "Provides a widget for calendar date input"
    GROUP "Date/Time (KF5)"
)
ecm_qtdesignerplugin_widget(KDateComboBox
    TOOLTIP "This widget can be used to display or edit the date. (KF5)"
    GROUP "Date/Time (KF5)"
)
ecm_qtdesignerplugin_widget(KDateTimeEdit
    TOOLTIP "This widget can be used to display or edit the date and time. (KF5)"
    GROUP "Date/Time (KF5)"
)
ecm_qtdesignerplugin_widget(KEditListWidget
    TOOLTIP "Fullfeatured edit box with buttons (KF5)"
    GROUP "Views (KF5)"
)
ecm_qtdesignerplugin_widget(KFontChooser
    ICON pics/kfontcombo.png
    TOOLTIP "Font Chooser (KF5)"
    WHATSTHIS "A font type, size and style selection widget complete with preview"
    GROUP "Input (KF5)"
)
ecm_qtdesignerplugin_widget(KFontRequester
    TOOLTIP "Font Requester (KF5)"
    WHATSTHIS "A compact font selection and preview widget"
    GROUP "Input (KF5)"
)
ecm_qtdesignerplugin_widget(KGradientSelector
    INCLUDE_FILE kselector.h
    TOOLTIP "Gradient Color Selector (KF5)"
    GROUP "Input (KF5)"
)
ecm_qtdesignerplugin_widget(KLed
    TOOLTIP "LED Widget (KF5)"
    WHATSTHIS "A widget showing a light emitter diode"
    GROUP "Display (KF5)"
)
ecm_qtdesignerplugin_widget(KMessageWidget
    TOOLTIP "Display an information or error message in an animated bubble."
    GROUP "Display (KF5)"
)
ecm_qtdesignerplugin_widget(KMimeTypeChooser
    TOOLTIP "Provides a checkable list of all available MIME types."
    CONSTRUCTOR_ARGS_CODE "(QString(), QStringList(), QString(), QStringList(), KMimeTypeChooser::Comments | KMimeTypeChooser::Patterns, parent)"
    GROUP "Input (KF5)"
)
ecm_qtdesignerplugin_widget(KMultiTabBar
    TOOLTIP "Multi Tab Bar Widget (KF5)"
    WHATSTHIS "A Widget for horizontal and vertical tabs."
    CONSTRUCTOR_ARGS_CODE "(KMultiTabBar::Left, parent)"
    GROUP "Display (KF5)"
)
ecm_qtdesignerplugin_widget(KNewPasswordWidget
    TOOLTIP "A password input widget"
    WHATSTHIS "This widget allows the user to enter a new password."
    GROUP "Input (KF5)"
)
ecm_qtdesignerplugin_widget(KPageView
    TOOLTIP "Paged View (KF5)"
    WHATSTHIS "A base class which can handle multiple pages."
    GROUP "Views (KF5)"
)
ecm_qtdesignerplugin_widget(KPageWidget
    TOOLTIP "Paged Widget (KF5)"
    WHATSTHIS "Page widget with many layouts (faces)."
    GROUP "Views (KF5)"
)
ecm_qtdesignerplugin_widget(KPasswordLineEdit
    TOOLTIP "A Password LineEdit (KF5)"
    WHATSTHIS "A Custom LineEdit which allows to show password"
    GROUP "Input (KF5)"
)
ecm_qtdesignerplugin_widget(KPixmapRegionSelectorWidget
    TOOLTIP "Pixmap Region Selector (KF5)"
    GROUP "Graphics (KF5)"
)
ecm_qtdesignerplugin_widget(KPixmapSequenceWidget
    TOOLTIP "Pixmap Sequence Widget (KF5)"
    WHATSTHIS "A simple widget showing a fixed size pixmap sequence."
    GROUP "Display (KF5)"
)
ecm_qtdesignerplugin_widget(KPopupFrame
    TOOLTIP "Frame with Popup Menu Behavior (KF5)"
    CONTAINER
    GROUP "Display (KF5)"
)
ecm_qtdesignerplugin_widget(KRatingWidget
    TOOLTIP "Rating value display (KF5)"
    WHATSTHIS "Displays a rating as a range of stars or other arbitrary pixmaps."
    GROUP "Display (KF5)"
)
ecm_qtdesignerplugin_widget(KRuler
    TOOLTIP "Measuring Ruler Widget (KF5)"
    WHATSTHIS "A measuring ruler widget as seen in KWord for page widths and heights"
    GROUP "Display (KF5)"
)
ecm_qtdesignerplugin_widget(KSeparator
    TOOLTIP "Standard horizontal or vertical separator."
    GROUP "Display (KF5)"
)
ecm_qtdesignerplugin_widget(KSqueezedTextLabel
    TOOLTIP "A QLabel that squeezes its text (KF5)"
    WHATSTHIS "If the text is too long to fit into the label it is divided into remaining left and right parts which are separated by three dots"
    CONSTRUCTOR_ARGS_CODE "(QStringLiteral(\"KSqueezedTextLabel\"), parent)"
    GROUP "Display (KF5)"
)
ecm_qtdesignerplugin_widget(KTimeComboBox
    TOOLTIP "This widget can be used to display or edit the time. (KF5)"
    GROUP "Date/Time (KF5)"
)
ecm_qtdesignerplugin_widget(KTitleWidget
    TOOLTIP "Title box with label and icon"
    WHATSTHIS "A styled frame to be used in title positions in dialogs and other widgets"
    GROUP "Display (KF5)"
)
ecm_qtdesignerplugin_widget(KUrlLabel
    TOOLTIP "URL Label (KF5)"
    CONSTRUCTOR_ARGS_CODE "(QStringLiteral(\"KUrlLabel\"), QString(), parent)"
    GROUP "Display (KF5)"
)

ecm_add_qtdesignerplugin(kwidgetsaddonswidgets
    NAME KWidgetsAddonsWidgets
    OUTPUT_NAME kwidgetsaddons5widgets
    WIDGETS
        KActionSelector
        KAnimatedButton
        KBusyIndicatorWidget
        KCapacityBar
        KCharSelect
        KCollapsibleGroupBox
        KColorButton
        KColorCombo
        KDateComboBox
        KDatePicker
        KDateTimeEdit
        KEditListWidget
        KFontChooser
        KFontRequester
        KGradientSelector
        KLed
        KMessageWidget
        KMimeTypeChooser
        KMultiTabBar
        KNewPasswordWidget
        KPageView
        KPageWidget
        KPasswordLineEdit
        KPixmapRegionSelectorWidget
        KPixmapSequenceWidget
        KPopupFrame
        KRatingWidget
        KRuler
        KSeparator
        KSqueezedTextLabel
        KTimeComboBox
        KTitleWidget
        KUrlLabel
    LINK_LIBRARIES
        KF5::WidgetsAddons
    INSTALL_DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/designer"
    COMPONENT Devel
)
