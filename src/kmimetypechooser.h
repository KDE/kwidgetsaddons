/* This file is part of the KDE libraries
   Copyright (C) 2001 - 2004 Anders Lund <anders@alweb.dk>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KMIMETYPE_CHOOSER_H
#define KMIMETYPE_CHOOSER_H

#include <kwidgetsaddons_export.h>
#include <QDialog>
#include <QWidget>

/**
 * @class KMimeTypeChooser kmimetypechooser.h KMimeTypeChooser
 *
 * This widget provides a checkable list of all available mimetypes, presented
 * as a treeview, with the mimetype comment and glob patterns as individual columns.
 * It allows users to edit a mimetype by launching the KDE mimetype editor (if it's
 * available on the system).
 *
 * When the user clicks the OK button, a list of the selected mimetype names
 * and associated glob patterns can be retrieved respectively; those lists can
 * be used to populate a text line edit, or set a configuration entry... etc.
 *
 * @author Anders Lund (anders at alweb dk), jan 23, 2002
 */
class KWIDGETSADDONS_EXPORT KMimeTypeChooser : public QWidget
{
    Q_OBJECT

public:
    /**
     * Buttons and data for display.
     */
    enum Visuals {
        Comments = 1, ///< Show the MimeType comment (e.g. "HTML Document") in a column
        Patterns = 2, ///< Show the Mimetypes glob patterns (e.g. "*.html;*.htm") in a column
        EditButton = 4 ///< Show the "Edit" button, allowing to edit the selected type
    };
    /**
     * Create a new KMimeTypeChooser.
     *
     * @param text A plain text line to display above the list
     * @param selectedMimeTypes A list of mimetype names, these will be initially selected
     *        in the list (provided they exist)
     * @param visuals OR'd KMimetypeChooser::Visuals enum values to to set whether to
     *        show the mimetype comment and glob patterns columns and an Edit button,
     *        respectively, or not
     * @param defaultGroup The group (e.g. "text") to expand in the treeview when no groups
     *        are selected. If not provided, no group is expanded by default.
     *        If @p groupsToShow is provided and it doesn't include @p defaultGroup, this
     *        parameter is ignored
     * @param groupsToShow a list of mimetype groups to show. If empty, all groups are shown
     * @param parent The parent widget to use
     */
    explicit
    KMimeTypeChooser(const QString &text = QString(),
                     const QStringList &selectedMimeTypes = QStringList(),
                     const QString &defaultGroup = QString(),
                     const QStringList &groupsToShow = QStringList(),
                     int visuals = Comments | Patterns | EditButton,
                     QWidget *parent = nullptr);
    ~KMimeTypeChooser();

    /**
     * @return a list of all selected mimetype names
     */
    QStringList mimeTypes() const;
    /**
     * @return a list of the filename glob patterns associated with all selected mimetypes
     */
    QStringList patterns() const;

private:
    class KMimeTypeChooserPrivate *d;

    Q_PRIVATE_SLOT(d, void _k_slotCurrentChanged(const QModelIndex &index))
    Q_PRIVATE_SLOT(d, void _k_slotSycocaDatabaseChanged(QStringList))
};

/**
  * @class KMimeTypeChooserDialog kmimetypechooser.h KMimeTypeChooserDialog
  *
  * @short A Dialog to select mimetypes from the list of available ones on the system
  *
  * This dialog embeds KMimeTypeChooser widget, presenting a checkable tree list of
  * mimetypes, each with its associated icon, and optinally associated glob patterns
  * (displayed in a separate column); also an optional Edit button to lauch the KDE
  * mimetype editor to edit the selected mimetype.
  *
  * Here is an example, using the dialog to set the text of two line edits with the
  * list of mimetype names and glob patterns, respectively, of the mimetypes that
  * the user has selected:
  *
  * @code
  *    QLineEdit *leMimetypes = new QLineEdit();
  *    QLineEdit *leGlobPatterns = new QLineEdit();
  *    [...]
  *    QString textLine = i18n("Select MimeTypes");
  *    QStringList mimeList = QStringList::split(QRegularExpression("\\s*;\\s*"), leMimetypes->text());
  *    KMimeTypeChooserDialog dlg(i18n("Select Mime Types"), textLine, mimeList, "text", this);
  *    if (dlg.exec() == QDialog::Accepted) {
  *        leMimetypes->setText(dlg.chooser()->mimeTypes().join(";"));
  *        leGlobPatterns->setText(dlg.chooser()->patterns().join(";"));
  *    }
  * @endcode
  *
  * \image html kmimetypechooserdialog.png "KMimeTypeChooserDialog in action"
  *
  * @author Anders Lund (anders at alweb dk) dec 19, 2001
  */
class KWIDGETSADDONS_EXPORT KMimeTypeChooserDialog : public QDialog
{
    Q_OBJECT
public:
    /**
     * Create a KMimeTypeChooser dialog.
     *
     * @param title The title of the dialog
     * @param text A plain text line to display above the list
     * @param selectedMimeTypes A list of mimetype names, these will be initially selected
     *        in the list, provided they exist
     * @param visuals OR'd KMimetypeChooser::Visuals enum values to to set whether to
     *        show the mimetype comment and glob patterns columns and an Edit button,
     *        respectively, or not
     * @param defaultGroup The group (e.g. "text") to expand in the treeview when no
     *        groups are selected. If not provided, no group is expanded by default
     *        If @p groupsToShow is provided and it doesn't include @p defaultGroup,
     *        this parameter is ignored
     * @param groupsToShow A list of mimetype groups to show. If empty, all groups are shown
     * @param parent The parent widget to use
     */
    explicit
    KMimeTypeChooserDialog(const QString &title = QString(),
                           const QString &text = QString(),
                           const QStringList &selectedMimeTypes = QStringList(),
                           const QString &defaultGroup = QString(),
                           const QStringList &groupsToShow = QStringList(),
                           int visuals = KMimeTypeChooser::Comments | KMimeTypeChooser::Patterns | KMimeTypeChooser::EditButton,
                           QWidget *parent = nullptr);

    /**
     * @overload
     */
    KMimeTypeChooserDialog(const QString &title,
                           const QString &text,
                           const QStringList &selectedMimeTypes,
                           const QString &defaultGroup,
                           QWidget *parent = nullptr);

    ~KMimeTypeChooserDialog();

    /**
     * @return a pointer to the KMimeTypeChooser widget
     */
    KMimeTypeChooser *chooser();

    QSize sizeHint() const override;

private:
    class Private;
    Private *const d;
};
#endif // _KMIMETYPE_CHOOSER_H_
