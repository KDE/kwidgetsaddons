/*
    Copyright (C) 1997 Bernd Johannes Wuebben <wuebben@kde.org>
    Copyright (c) 1999 Preston Brown <pbrown@kde.org>
    Copyright (c) 1999 Mario Weilguni <mweilguni@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#ifndef K_FONT_CHOOSER_DIALOG_H
#define K_FONT_CHOOSER_DIALOG_H

#include <QDialog>

#include <kfontchooser.h>

#include <functional>

class QFont;
class QStringList;

class KFontChooserDialogPrivate;

/**
 * @class KFontChooserDialog kfontchooserdialog.h KFontChooserDialog
 *
 * @short A font selection dialog.
 *
 * The KFontChooserDialog provides a dialog for interactive font selection.
 * It is basically a thin wrapper around the KFontChooser widget (the latter
 * can also be used standalone). In most cases, the simplest use of this
 * class is the static method KFontChooserDialog::getFont(), which shows
 * the dialog, allows the user to select a font, and returns when the
 * dialog is closed.
 *
 * Features offered by KFontChooserDialog/KFontChooser:
 * - The ability to set decimal font sizes (e.g. "12.1")
 * - When selecting an initial font, if the styleName property of that font
 *   isn't set, the dialog will try and select the correct font style from the
 *   styles list
 * - The ability to set multiple fonts at once, for an example of this functionality
 *   see "Adjust All Fonts" in the Fonts KCM in Systemsettings; and you can change
 *   the font family, style or size separately
 * - Discarding the styleName property when closing the dialog for "Regular" font
 *   styles, since it doesn't make sense to set that property for such fonts and
 *   this allows setBold(true) to work correctly for more details see:
 *   https://bugreports.qt.io/browse/QTBUG-63792
 *   https://bugs.kde.org/show_bug.cgi?id=378523
 *
 * Example:
 *
 * \code
 *      QFont myFont;
 *      int result = KFontChooserDialog::getFont(myFont);
 *      if (result == QDialog::Accepted) {
 *            ...
 *      }
 * \endcode
 *
 * Example (async):
 *
 * \code
 *      KFontChooserDialog::getFont(oldFont, KFontChooser::NoDisplayFlags, window,
 *                                  this, [&](const QFont &selectedFont, int dialogCode) {
 *          if (dialogCode == QDialog::Accepted) {
 *              ...
 *          }
 *      });
 * \endcode
 *
 * \image html kfontchooserdialog.png "KFontChooserDialog"
 *
 * @author Preston Brown <pbrown@kde.org>, Bernd Wuebben <wuebben@kde.org>
 *
 * @since 5.69
 */
class KWIDGETSADDONS_EXPORT KFontChooserDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructs a font selection dialog.
     *
     * @param flags flags to define how the font chooser is displayed
     * @param parent parent widget of the dialog, if any, the dialog will be centered relative to it
     */
    explicit KFontChooserDialog(const KFontChooser::DisplayFlags &flags = KFontChooser::NoDisplayFlags,
                                QWidget *parent = nullptr);

    ~KFontChooserDialog();

    /**
     * Sets the currently selected font in the dialog.
     *
     * @param font the font to select
     * @param onlyFixed if @c true, the font list will show only fixed width (monospace)
     *        fonts, otherwise all available fonts are shown
     */
    void setFont(const QFont &font, bool onlyFixed = false);

    /**
     * @return the currently selected font in the dialog
     */
    QFont font() const;

    /**
     * Creates a modal font dialog, lets the user choose a font, and returns when
     * the dialog is closed.
     *
     * @param theFont a reference to the font to write the chosen font into
     * @param flags flags to define how the font chooser is displayed
     * @param parent parent widget of the dialog, if any, the dialog will be centered relative to it
     * @return QDialog::result()
     */
    static int getFont(QFont &theFont,
                       const KFontChooser::DisplayFlags &flags = KFontChooser::NoDisplayFlags,
                       QWidget *parent = nullptr);

    /**
     * Creates a modal font dialog, lets the user choose a font, and calls the slot or lambda when
     * the dialog is closed.
     *
     * @param font the font to set initially
     * @param flags flags to define how the font chooser is displayed
     * @param parent parent widget of the dialog, if any, the dialog will be centered relative to it
     * @param receiver receiver object of the slot @p slot or context object for the lambda @p slot
     * @param slot slot of the receiver object to call or lambda, signature should be compatible to
     *             @c signal(const QFont &font, int dialogCode)
     *
     * @since 5.78
     */
    template<class Receiver, class Func>
    static inline void getFont(const QFont &font,
                               KFontChooser::DisplayFlags flags,
                               QWidget *parent,
                               const Receiver *recvr, Func slot)
    {
        KFontChooserDialog *dialog = createInternal(font, flags, parent);
        QObject::connect(dialog, &KFontChooserDialog::gotFont, recvr, slot);
        dialog->open();
    }

    /**
     * Overload for getFont(const QFont &, KFontChooser::DisplayFlags flags, QWidget *parent, const Receiver *, Func), with @p flags set to @c KFontChooser::NoDisplayFlags.
     *
     * @since 5.78
     */
    template<class Receiver, class Func>
    static inline void getFont(const QFont &font,
                               QWidget *parent,
                               const Receiver *recvr, Func slot)
    {
        getFont<Receiver, Func>(font, KFontChooser::NoDisplayFlags, parent, recvr, slot);
    }

    /**
     * Creates a modal font difference dialog, lets the user choose a selection
     * of changes that should be made to a set of fonts, and returns when the
     * dialog is closed. Useful for choosing slight adjustments to the font set
     * when the user would otherwise have to manually edit a number of fonts.
     *
     * @param theFont a reference to the font to write the chosen font into
     * @param flags flags to define how the font chooser is displayed
     * @param diffFlags a reference to the integer bitmask into which the chosen
     *        difference selection bitmask should be written.
     *        Check the bitmask afterwards like:
     *        \code
     *        if ( diffFlags & KFontChooser::FontDiffFamily )  {
     *            [...]
     *        }
     *        if ( diffFlags & KFontChooser::FontDiffStyle ) {
     *            [...]
     *        }
     *        if ( diffFlags & KFontChooser::FontDiffSize ) {
     *            [...]
     *        }
     *        \endcode
     * @param parent parent widget of the dialog, if any, the dialog will be centered relative to it
     *
     * @returns QDialog::result()
     */
    static int getFontDiff(QFont &theFont,
                           KFontChooser::FontDiffFlags &diffFlags,
                           const KFontChooser::DisplayFlags &flags = KFontChooser::NoDisplayFlags,
                           QWidget *parent = nullptr);

    /**
     * Creates a modal font difference dialog, lets the user choose a selection
     * of changes that should be made to a set of fonts, and calls the slot or lambda
     * when the dialog is closed. Useful for choosing slight adjustments to the font set
     * when the user would otherwise have to manually edit a number of fonts.
     *
     * @param font the font to set initially
     * @param flags flags to define how the font chooser is displayed
     * @param parent parent widget of the dialog, if any, the dialog will be centered relative to it
     * @param receiver receiver object of the slot @p slot or context object for the lambda method @p slot
     * @param slot slot of the receiver object to call or lambda, signature should be compatible to
     *             @c signal(const QFont &font, KFontChooser::FontDiffFlags diffFlags, int dialogCode)
     *             where @p diffFlags is an integer bitmask holding the chosen difference selection bitmask.
     *             Check the bitmask like:
     *             \code
     *             if ( diffFlags & KFontChooser::FontDiffFamily )  {
     *                 [...]
     *             }
     *             if ( diffFlags & KFontChooser::FontDiffStyle ) {
     *                 [...]
     *             }
     *             if ( diffFlags & KFontChooser::FontDiffSize ) {
     *                 [...]
     *             }
     *             \endcode
     *
     * @since 5.78
     */
    template<class Receiver, class Func>
    static inline void getFontDiff(const QFont &font,
                                   KFontChooser::DisplayFlags flags,
                                   QWidget *parent,
                                   const Receiver *recvr, Func slot)
    {
        KFontChooserDialog *dialog = createInternalDiff(font, flags, parent);
        QObject::connect(dialog, &KFontChooserDialog::gotFontDiff, recvr, slot);
        dialog->open();
    }

    /**
     * Overload for getFontDiff(const QFont &, KFontChooser::DisplayFlags flags, QWidget *parent, const Receiver *, Func), with @p flags set to @c KFontChooser::NoDisplayFlags.
     *
     * @since 5.78
     */
    template<class Receiver, class Func>
    static inline void getFontDiff(const QFont &font,
                                   QWidget *parent,
                                   const Receiver *recvr, Func slot)
    {
        getFontDiff<Receiver, Func>(font, KFontChooser::NoDisplayFlags, parent, recvr, slot);
    }

Q_SIGNALS:
    /**
     * Emitted whenever the currently selected font changes.
     * Connect to this to monitor the font as it is selected if you are
     * not running modal.
     */
    void fontSelected(const QFont &font);

private:
    /**
     * @internal
     */
    static
    KFontChooserDialog * createInternal(const QFont &font, KFontChooser::DisplayFlags flags, QWidget *parent);

    /**
     * @internal
     */
    static
    KFontChooserDialog * createInternalDiff(const QFont &font,
                                           KFontChooser::DisplayFlags flags,
                                           QWidget *parent);

Q_SIGNALS:
    /**
     * @internal
     * Helper signal for getFont(..., const Receiver *recvr, Func slot),
     * to have Qt's connect do all the special handling needed to wire up to a lambda or slot
     */
    void gotFont(const QFont &font, int dialogCode, QPrivateSignal);
    /**
     * @internal
     * Helper signal for getFontDiff(..., const Receiver *recvr, Func slot),
     * to have Qt's connect do all the special handling needed to wire up to a lambda or slot
     */
    void gotFontDiff(const QFont &font, KFontChooser::FontDiffFlags diffFlags, int dialogCode, QPrivateSignal);

private:
    KFontChooserDialogPrivate *const d;

    Q_DISABLE_COPY(KFontChooserDialog)
};

#endif
