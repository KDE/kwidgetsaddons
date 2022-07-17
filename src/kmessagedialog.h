/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 2020 Ahmad Samir <a.samirh78@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KMESSAGEDIALOG_H
#define KMESSAGEDIALOG_H

#include <kwidgetsaddons_export.h>

#include <KStandardGuiItem>

#include <QDialog>
#include <QMessageBox>

#include <memory>

class QDialogButtonBox;

class KMessageDialogPrivate;
class KGuiItem;

/**
 * @class KMessageDialog kmessagedialog.h <KMessageDialog>
 *
 *
 * @brief KMessageDialog creates a message box similar to the ones you get from KMessageBox,
 * but that can be used asynchronously, i.e. you can show the dialog by using @c show()
 * or @c open().
 *
 * This class contructs a dialog similar to the dialogs the KMessageBox convenience functions
 * create. The main difference is that the KMessageBox methods typically use @c exec() to show
 * the dialogs; one of the main disadvantages of using @c exec(), is that it starts a nested
 * eventloop, which could lead to nasty crashes.
 *
 * Another difference is that the API is supposed to be slightly easier to use as it has
 * various methods to set up the dialog, e.g. @ref setCaption(), @ref setDetails() ...etc.
 *
 * By default, appropriate buttons based on the dialog type are added (since 5.85) (e.g. an
 * "OK" button is added to an Information dialog), you can set custom buttons by using the
 * @ref setButtons() method.
 *
 * The @ref QDialog::done() slot is called to set the result of the dialog, which will emit the
 * @ref QDialog::finished() signal with that result. The result is one of the
 * QDialogButtonBox::StandardButton enum. This is useful as you can tell exactly which button
 * was clicked by the user. E.g.:
 * - the "No" button having been clicked, in which case you may still want to save the status
 *   of the "Do not ask again" CheckBox
 * - the "Cancel" button having been clicked, in which case you ideally will ignore the status
 *   of the "Do not ask again" CheckBox
 *
 * For "warning" dialogs, i.e. dialogs with a potentially destructive action, the default
 * button is set to a button with the QDialogButtonBox::RejectRole, typically either button
 * "No" or "Cancel" depending on which one is shown in the dialog. If both of them are shown
 * (as is the case with KMessageDialog::WarningYesNoCancel dialog Type) then the "Cancel"
 * button will be set as the default one.
 *
 * This class intends to be very flexible with the buttons that can be used, since you can
 * call the @ref setButtons() method with a KGuiItem that has custom text/icon.
 *
 * Example:
 * @code
 * auto *dlg = new KMessageDialog(KMessageDialog::QuestionYesNoCancel,
 *                                QStringLiteral("Do you agree?"),
 *                                nullptr);
 *
 * dlg->setCaption(QStringLiteral("Window Title"));
 * dlg->setButtons(KStandardGuiItem::yes(), KStandardGuiItem::no(), KStandardGuiItem::cancel());
 * dlg->setListWidgetItems(QStringList{QStringLiteral("file1"), QStringLiteral("file2")});
 * dlg->setDetails(QStringLiteral("Some more details."));
 * dlg->setDontAskAgainText(QStringLiteral("Do not ask again"));
 * dlg->setDontAskAgainChecked(false);
 *
 *  // Delete the dialog when it's closed
 *  dlg->setAttribute(Qt::WA_DeleteOnClose);
 *  // Make the dialog window modal
 *  dlg->setWindowModality(Qt::WindowModal);
 *
 *  QObject::connect(dlg, &QDialog::finished, &app, [dlg](int result) {
 *      auto button = static_cast<QDialogButtonBox::StandardButton>(result);
 *      switch(button) {
 *      case QDialogButtonBox::Ok:
 *      case QDialogButtonBox::Yes:
 *          // The user clicked OK, handle the result...
 *          // save the "do not ask again" box status...
 *          break;
 *      case QDialogButtonBox::No:
 *          // The user clicked no, reject the changes...
 *          // save the "do not ask again" box status...
 *          break;
 *      case QDialogButtonBox::Cancel:
 *          // The user clicked cancel, reject the changes...
 *          break;
 *      default:
 *          break;
 *      }
 *  });
 *
 *  dlg->show();
 * @endcode
 *
 * @since 5.77
 */

class KWIDGETSADDONS_EXPORT KMessageDialog : public QDialog
{
    Q_OBJECT

public:
    enum Type {
        QuestionYesNo = 1,
        QuestionYesNoCancel = 2,
        WarningYesNo = 3,
        WarningYesNoCancel = 4,
        WarningContinueCancel = 5,
        Information = 6,
#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 97)
        Sorry ///< @deprecated Since 5.97, use Error.
            KWIDGETSADDONS_ENUMERATOR_DEPRECATED_VERSION(5, 97, "Use Error.") = 7,
#endif
        Error = 8,
    };

    /**
     * Constructs a KMessageDialog.
     *
     * @param type the dialog Type, one of KMessageDialog::Type enum
     * @param text the text message that is going to be displayed in the dialog
     * @param parent a QWidget* that will be set as the dialog parent
     */
    explicit KMessageDialog(KMessageDialog::Type type, const QString &text, QWidget *parent = nullptr);

    /**
     * This constructor takes the window Id of the parent window, instead of a QWidget*.
     *
     * @param type the dialog Type, one of KMessageDialog::Type enum
     * @param text the text message that is going to be displayed in the dialog
     * @param parent_id the native parent's window system identifier
     */
    explicit KMessageDialog(KMessageDialog::Type type, const QString &text, WId parent_id);
    /**
     * Destructor
     */
    ~KMessageDialog() override;

    /**
     * This can be used to set the title of the dialog window. If you pass an
     * empty QString(), a generic title will be used depending on the dialog
     * Type. E.g. for KMessageDialog::WarningYesNo, "Warning" will be used.
     */
    void setCaption(const QString &caption);

    /**
     * This can be used to set an icon that will be shown next to the main
     * text message. If you pass a null QIcon() a generic icon based on the dialog
     * Type will be used. E.g. for KMessageDialog::QuestionYesNo, QMessageBox::Question
     * will be used.
     */
    void setIcon(const QIcon &icon);

    /**
     * This will add a QListWidget to the dialog and populate it with @p strlist.
     * If @p strlist is empty, the list widget will not be shown.
     */
    void setListWidgetItems(const QStringList &strlist);

    /**
     * This will add a KCollapsibleGroupBox with a title "Details", as the class name
     * implies it is collapsible (and collapsed by default); you can use it to add a
     * more detailed explanation of what the dialog is trying to tell the user.
     *
     * If @p details is empty, the details widget will not be shown.
     */
    void setDetails(const QString &details);

    /**
     * This will add a "Do not ask again" checkbox to the dialog with the text
     * from @p dontAskAgainText. You can set the initial status of the checkbox
     * by using setDontAskAgainChecked().
     *
     * If @p dontAskAgainText is empty, no checkbox will be shown.
     *
     * Typical usage of this checkbox is for recurring questions, e.g. showing
     * a dialog to confirm moving files/directories to trash, the user can then
     * set the checkbox so as not to be asked about that again.
     *
     * You can get the state of the checkbox by using isDontAskAgainChecked().
     */
    void setDontAskAgainText(const QString &dontAskAgainText);

    /**
     * This can be used to set the initial status of the "Do not ask again" checkbox,
     * checked or unchecked, by setting @p isChecked to @c true or @c false
     * respectively.
     *
     * You need to call setDontAskAgainText() first to actually show a checkbox in
     * the dialog, otherwise calling this function will have no effect.
     */
    void setDontAskAgainChecked(bool isChecked);

    /**
     * This can be used to query the status of the "Do not ask again" checkbox;
     * returns @c true if the box is checked and @c false otherwise.
     *
     * @note This method will return @c false if a checkbox widget isn't shown in
     * the dialog. The dialog will not show a checkbox if setDontAskAgainText() was
     * not used previously to add a checkbox to begin with.
     */
    bool isDontAskAgainChecked() const;

    /**
     * Sets the text labels in the dialog to either allow opening external links or not.
     */
    void setOpenExternalLinks(bool isAllowed);

    /**
     * Since 5.85 buttons based on the dialog type are added by default, e.g. an OK
     * button to Information and Sorry dialogs; (before 5.85, if you didn't call this
     * method no buttons were added to the dialog).
     *
     * This will add a QDialogButtonBox populated with @p buttonAccept and @p buttonNo,
     * by default KStandardGuiItem::yes() and KStandardGuiItem::no() respectively. For
     * dialog Types that have a Cancel button, @p buttonCancel will be used.
     *
     * Using this method, you can customize the behavior based on your use-case, by
     * using a KGuiItem to get a button with custom text and icon.
     *
     * @note
     * - For WarningContinueCancel dialog Type, if buttonAccept has the same text as
     *   KStandardGuiItem::yes(), KStandardGuiItem::cont() will be used instead
     * - For dialog Types: Information, Sorry, and Error only one button
     *   (KStandardGuiItem::ok()) is added to the dialog.
     */
    void setButtons(const KGuiItem &buttonAccept = KStandardGuiItem::yes(),
                    const KGuiItem &buttonNo = KStandardGuiItem::no(),
                    const KGuiItem &buttonCancel = KStandardGuiItem::cancel());

private:
    std::unique_ptr<KMessageDialogPrivate> const d;
};

#endif // KMESSAGEDIALOG_H
