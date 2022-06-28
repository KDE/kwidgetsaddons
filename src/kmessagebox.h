/*
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 1999 Waldo Bastian <bastian@kde.org>

    SPDX-License-Identifier: LGPL-2.0-only
*/

// krazy:excludeall=dpointer

#ifndef KMESSAGEBOX_H
#define KMESSAGEBOX_H

#include <QDialogButtonBox>
#include <QMessageBox>
#include <QStringList>

#include <kguiitem.h>
#include <kstandardguiitem.h>

#include <kwidgetsaddons_export.h>

class KMessageBoxDontAskAgainInterface;
class KMessageBoxNotifyInterface;
class QDialog;
class QDialogButtonBox;
class QWidget;
class KConfig;

/**
 * Easy message dialog box.
 *
 * Provides convenience functions for some i18n'ed standard dialogs,
 * as well as audible notification via @ref KNotification
 *
 * The text in message boxes is wrapped automatically. The text may either
 * be plaintext or richtext. If the text is plaintext, a newline-character
 * may be used to indicate the end of a paragraph.
 *
 * \image html kmessagebox.png "An information dialog box"
 *
 * @author Waldo Bastian (bastian@kde.org)
 */
namespace KMessageBox
{
/**
 * Button types.
 */
enum ButtonCode {
    Ok = 1,
    Cancel = 2,
    PrimaryAction = 3, ///< @since 5.100
    SecondaryAction = 4, ///< @since 5.100
#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
    /// @deprecated Since 5.100, use PrimaryAction.
    Yes KWIDGETSADDONS_ENUMERATOR_DEPRECATED_VERSION(5, 100, "Use PrimaryAction.") = PrimaryAction,
    /// @deprecated Since 5.100, use SecondaryAction.
    No KWIDGETSADDONS_ENUMERATOR_DEPRECATED_VERSION(5, 100, "Use SecondaryAction.") = SecondaryAction,
#endif
    Continue = 5,
};

enum DialogType {
    QuestionTwoActions = 1, ///< @since 5.100
    WarningTwoActions = 2, ///< @since 5.100
    WarningContinueCancel = 3,
    WarningTwoActionsCancel = 4, ///< @since 5.100
    Information = 5,
// Reserved for: SSLMessageBox = 6
#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 97)
    Sorry ///< @deprecated Since 5.97, use Error.
        KWIDGETSADDONS_ENUMERATOR_DEPRECATED_VERSION(5, 97, "Use Error.") = 7,
#endif
    Error = 8,
    QuestionTwoActionsCancel = 9, ///< @since 5.100
#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
    /// @deprecated Since 5.100, use QuestionTwoActions.
    QuestionYesNo KWIDGETSADDONS_ENUMERATOR_DEPRECATED_VERSION(5, 100, "Use QuestionTwoActions.") = QuestionTwoActions,
    /// @deprecated Since 5.100, use WarningTwoActions.
    WarningYesNo KWIDGETSADDONS_ENUMERATOR_DEPRECATED_VERSION(5, 100, "Use WarningTwoActions.") = WarningTwoActions,
    /// @deprecated Since 5.100, use WarningTwoActionsCancel.
    WarningYesNoCancel KWIDGETSADDONS_ENUMERATOR_DEPRECATED_VERSION(5, 100, "Use WarningTwoActionsCancel.") = WarningTwoActionsCancel,
    /// @deprecated Since 5.100, use QuestionTwoActionsCancel.
    QuestionYesNoCancel KWIDGETSADDONS_ENUMERATOR_DEPRECATED_VERSION(5, 100, "Use QuestionTwoActionsCancel.") = QuestionTwoActionsCancel,
#endif
};

/**
 * @see Options
 */
enum Option {
    Notify = 1, ///< Emit a KNotify event
    AllowLink = 2, ///< The message may contain links.
    Dangerous = 4, ///< The action to be confirmed by the dialog is a potentially destructive one. The default button will be set to Cancel or No, depending on
                   ///< which is available.
#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 97)
    PlainCaption ///< @deprecated Since 5.0 this option is ignored.
        KWIDGETSADDONS_ENUMERATOR_DEPRECATED_VERSION_BELATED(5, 97, 5, 0, "This option is ignored..") = 8,
#endif
    NoExec = 16, ///< Do not call exec() in createKMessageBox()
    WindowModal = 32, ///< The window is to be modal relative to its parent. By default, it is application modal.
};

/**
 * Stores a combination of #Option values.
 */
Q_DECLARE_FLAGS(Options, Option)

// This declaration must be defined before first Option is used in method signatures
Q_DECLARE_OPERATORS_FOR_FLAGS(Options)

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * Display a simple "question" dialog.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Question").
 * @param buttonYes The text for the first button.
 *                  The default is KStandardGuiItem::yes().
 * @param buttonNo  The text for the second button.
 *                  The default is KStandardGuiItem::no().
 * @param dontAskAgainName If provided, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options  see Option
 *
 * @return  'Yes' is returned if the Yes-button is pressed. 'No' is returned
 *          if the No-button is pressed.
 *
 * To be used for questions like "Do you have a printer?"
 *
 * The default button is "Yes". Pressing "Esc" selects "No".
 *
 * @deprecated Since 5.100, use questionTwoActions()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use questionTwoActions()")
ButtonCode questionYesNo(QWidget *parent,
                         const QString &text,
                         const QString &title = QString(),
                         const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                         const KGuiItem &buttonNo = KStandardGuiItem::no(),
                         const QString &dontAskAgainName = QString(),
                         Options options = Notify);
#endif

/**
 * Display a "question" dialog with two action buttons.
 *
 * To be used for questions like "Do you want to save the message for later or discard it?".
 *
 * The default button is the primary button. Pressing "Esc" triggers the secondary button.
 *
 * @param parent  the parent widget
 * @param text    the message string
 * @param title   the message box title. If an empty string, defaults to i18n("Question").
 * @param primaryAction the action for the primary button
 * @param secondaryAction the action for the secondary button
 * @param dontAskAgainName If not an empty string, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options see Option
 *
 * @returns @c PrimaryAction if the primary button is triggered, @c SecondaryAction
 *          if the secondary button is triggered.
 *
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode questionTwoActions(QWidget *parent,
                              const QString &text,
                              const QString &title,
                              const KGuiItem &primaryAction,
                              const KGuiItem &secondaryAction,
                              const QString &dontAskAgainName = QString(),
                              Options options = Notify);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * Display a simple "question" dialog.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Question").
 * @param buttonYes The text for the first button.
 *                  The default is KStandardGuiItem::yes().
 * @param buttonNo  The text for the second button.
 *                  The default is KStandardGuiItem::no().
 * @param buttonCancel  The text for the third button.
 *                  The default is KStandardGuiItem::cancel().
 * @param dontAskAgainName If provided, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options  see Options
 *
 * @return  'Yes' is returned if the Yes-button is pressed. 'No' is returned
 *          if the No-button is pressed.
 *
 * To be used for questions like "Do you want to discard the message or save it for later?",
 *
 * The default button is "Yes". Pressing "Esc" selects "Cancel".
 *
 * @deprecated Since 5.100, use questionTwoActionsCancel()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use questionTwoActionsCancel()")
ButtonCode questionYesNoCancel(QWidget *parent,
                               const QString &text,
                               const QString &title = QString(),
                               const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                               const KGuiItem &buttonNo = KStandardGuiItem::no(),
                               const KGuiItem &buttonCancel = KStandardGuiItem::cancel(),
                               const QString &dontAskAgainName = QString(),
                               Options options = Notify);
#endif

/**
 * Display a "question" dialog with two action buttons and a cancel button.
 *
 * To be used for questions like "Do you want to save the message for later or discard it?".
 *
 * The default button is the primary button. Pressing "Esc" triggers the cancel button.
 *
 * @param parent  the parent widget
 * @param text    the message string
 * @param title   the message box title. If an empty string, defaults to i18n("Question").
 * @param primaryAction the action for the primary button
 * @param secondaryAction the action for the secondary button
 * @param cancelAction the action for the cancel button
 * @param dontAskAgainName If not an empty string, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options  see Option
 *
 * @returns @c PrimaryAction if the primary button is triggered, @c SecondaryAction
 *          if the secondary button is triggered. @c Cancel if the cancel button is triggered.
 *
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode questionTwoActionsCancel(QWidget *parent,
                                    const QString &text,
                                    const QString &title,
                                    const KGuiItem &primaryAction,
                                    const KGuiItem &secondaryAction,
                                    const KGuiItem &cancelAction = KStandardGuiItem::cancel(),
                                    const QString &dontAskAgainName = QString(),
                                    Options options = Notify);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * Display a "question" dialog with a listbox to show information to the user
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param strlist List of strings to be written in the listbox. If the list is
 *                empty, it doesn't show any listbox, working as questionYesNo.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Question").
 * @param buttonYes The text for the first button.
 *                  The default is KStandardGuiItem::yes().
 * @param buttonNo  The text for the second button.
 *                  The default is KStandardGuiItem::no().
 * @param dontAskAgainName If provided, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options  see Options
 *
 * @return  'Yes' is returned if the Yes-button is pressed. 'No' is returned
 *          if the No-button is pressed.
 *
 * To be used for questions like "Do you really want to delete these files?"
 * And show the user exactly which files are going to be deleted in case
 * he presses "Yes"
 *
 * The default button is "Yes". Pressing "Esc" selects "No".
 *
 * @deprecated Since 5.100, use questionTwoActionsList()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use questionTwoActionsList()")
ButtonCode questionYesNoList(QWidget *parent,
                             const QString &text,
                             const QStringList &strlist,
                             const QString &title = QString(),
                             const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                             const KGuiItem &buttonNo = KStandardGuiItem::no(),
                             const QString &dontAskAgainName = QString(),
                             Options options = Notify);
#endif

/**
 * Display a "question" dialog with a listbox to show information to the user
 * and two action buttons.
 *
 * To be used for questions like "Do you really want to delete these files?"
 * and show the user exactly which files are going to be deleted in case.
 *
 * The default button is the primary button. Pressing "Esc" triggers the secondary button.
 *
 * @param parent  the parent widget
 * @param text    the message string
 * @param strlist List of strings to be written in the listbox. If the list is
 *                empty, it doesn't show any listbox, working as questionTwoActions().
 * @param title   the message box title. If an empty string, defaults to i18n("Question").
 * @param primaryAction the action for the primary button
 * @param secondaryAction the action for the secondary button
 * @param dontAskAgainName If not an empty string, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options  see Option
 *
 * @returns @c PrimaryAction if the primary button is triggered, @c SecondaryAction
 *          if the secondary button is triggered.
 *
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode questionTwoActionsList(QWidget *parent,
                                  const QString &text,
                                  const QStringList &strlist,
                                  const QString &title,
                                  const KGuiItem &primaryAction,
                                  const KGuiItem &secondaryAction,
                                  const QString &dontAskAgainName = QString(),
                                  Options options = Notify);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * Display a "warning" dialog.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Warning").
 * @param buttonYes The text for the first button.
 *                  The default is KStandardGuiItem::yes().
 * @param buttonNo  The text for the second button.
 *                  The default is KStandardGuiItem::no().
 * @param dontAskAgainName If provided, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options  see Options
 *
 * @return  @p Yes is returned if the Yes-button is pressed. @p No is returned
 *          if the No-button is pressed.
 *
 * To be used for questions "Shall I update your configuration?"
 * The text should explain the implication of both options.
 *
 * The default button is "No". Pressing "Esc" selects "No".
 *
 * @deprecated Since 5.100, use warningTwoActions()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use warningTwoActions()")
ButtonCode warningYesNo(QWidget *parent,
                        const QString &text,
                        const QString &title = QString(),
                        const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                        const KGuiItem &buttonNo = KStandardGuiItem::no(),
                        const QString &dontAskAgainName = QString(),
                        Options options = Options(Notify | Dangerous));
#endif

/**
 * Display a "warning" dialog with two action buttons.
 *
 * To be used for questions like "Shall I update your configuration?".
 * The text should explain the implication of both actions.
 *
 * The default button is the secondary button. Pressing "Esc" triggers the secondary button.
 *
 * @param parent  the parent widget
 * @param text    the message string
 * @param title   the message box title. If an empty string, defaults to i18n("Warning").
 * @param primaryAction the action for the primary button
 * @param secondaryAction the action for the secondary button
 * @param dontAskAgainName If not an empty string, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options see Options
 *
 * @returns @c PrimaryAction if the primary button is triggered, @c SecondaryAction
 *          if the secondary button is triggered.
 *
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode warningTwoActions(QWidget *parent,
                             const QString &text,
                             const QString &title,
                             const KGuiItem &primaryAction,
                             const KGuiItem &secondaryAction,
                             const QString &dontAskAgainName = QString(),
                             Options options = Options(Notify | Dangerous));

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * Display a "warning" dialog with a listbox to show information to the user
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param strlist List of strings to be written in the listbox. If the list is
 *                empty, it doesn't show any listbox, working as questionYesNo.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Question").
 * @param buttonYes The text for the first button.
 *                  The default is KStandardGuiItem::yes().
 * @param buttonNo  The text for the second button.
 *                  The default is KStandardGuiItem::no().
 * @param dontAskAgainName If provided, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options  see Options
 *
 * @return  'Yes' is returned if the Yes-button is pressed. 'No' is returned
 *          if the No-button is pressed.
 *
 * To be used for questions like "Do you really want to delete these files?"
 * And show the user exactly which files are going to be deleted in case
 * he presses "Yes"
 *
 * The default button is "No". Pressing "Esc" selects "No".
 *
 * @deprecated Since 5.100, use warningTwoActionsList()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use warningTwoActionsList()")
ButtonCode warningYesNoList(QWidget *parent,
                            const QString &text,
                            const QStringList &strlist,
                            const QString &title = QString(),
                            const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                            const KGuiItem &buttonNo = KStandardGuiItem::no(),
                            const QString &dontAskAgainName = QString(),
                            Options options = Options(Notify | Dangerous));
#endif

/**
 * Display a "warning" dialog with a listbox to show information to the user
 * and two action buttons.
 *
 * To be used for questions like "Shall I update your configuration?".
 * The text should explain the implication of both actions.
 *
 * The default button is the secondary button. Pressing "Esc" triggers the secondary button.
 *
 * @param parent  the parent widget
 * @param text    the message string
 * @param strlist List of strings to be written in the listbox. If the list is
 *                empty, it doesn't show any listbox, working as questionYesNo.
 * @param title   the message box title. If an empty string, defaults to i18n("Warning").
 * @param primaryAction the action for the primary button
 * @param secondaryAction the action for the secondary button
 * @param dontAskAgainName If not an empty string, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options see Options
 *
 * @returns @c PrimaryAction if the primary button is triggered, @c SecondaryAction
 *          if the secondary button is triggered.
 *
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode warningTwoActionsList(QWidget *parent,
                                 const QString &text,
                                 const QStringList &strlist,
                                 const QString &title,
                                 const KGuiItem &primaryAction,
                                 const KGuiItem &secondaryAction,
                                 const QString &dontAskAgainName = QString(),
                                 Options options = Options(Notify | Dangerous));

/**
 * Display a "warning" dialog.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Warning").
 * @param buttonContinue The text for the first button.
 *                       The default is KStandardGuiItem::cont().
 * @param buttonCancel The text for the second button.
 *                     The default is KStandardGuiItem::cancel().
 * @param dontAskAgainName If provided, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options  see Options
 *
 * @return  @p Continue is returned if the Continue-button is pressed.
 *          @p Cancel is returned if the Cancel-button is pressed.
 *
 * To be used for questions like "You are about to Print. Are you sure?"
 * the continueButton should then be labeled "Print".
 *
 * The default button is buttonContinue. Pressing "Esc" selects "Cancel".
 */
KWIDGETSADDONS_EXPORT ButtonCode warningContinueCancel(QWidget *parent,
                                                       const QString &text,
                                                       const QString &title = QString(),
                                                       const KGuiItem &buttonContinue = KStandardGuiItem::cont(),
                                                       const KGuiItem &buttonCancel = KStandardGuiItem::cancel(),
                                                       const QString &dontAskAgainName = QString(),
                                                       Options options = Notify);

/**
 * Display a "warning" dialog with a collapsible "Details" section.
 *
 * @since 5.61
 */
KWIDGETSADDONS_EXPORT ButtonCode warningContinueCancelDetailed(QWidget *parent,
                                                               const QString &text,
                                                               const QString &title = QString(),
                                                               const KGuiItem &buttonContinue = KStandardGuiItem::cont(),
                                                               const KGuiItem &buttonCancel = KStandardGuiItem::cancel(),
                                                               const QString &dontAskAgainName = QString(),
                                                               Options options = Notify,
                                                               const QString &details = QString());

/**
 * Display a "warning" dialog with a listbox to show information to the user.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param strlist List of strings to be written in the listbox. If the
 *                list is empty, it doesn't show any listbox, working
 *                as warningContinueCancel.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Warning").
 * @param buttonContinue The text for the first button.
 *                       The default is KStandardGuiItem::cont().
 * @param buttonCancel The text for the second button.
 *                     The default is KStandardGuiItem::cancel().
 * @param dontAskAgainName If provided, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 *
 * @param options  see Options
 *
 * @return  @p Continue is returned if the Continue-button is pressed.
 *          @p Cancel is returned if the Cancel-button is pressed.
 *
 * To be used for questions like "You are about to Print. Are you sure?"
 * the continueButton should then be labeled "Print".
 *
 * The default button is buttonContinue. Pressing "Esc" selects "Cancel".
 */
KWIDGETSADDONS_EXPORT ButtonCode warningContinueCancelList(QWidget *parent,
                                                           const QString &text,
                                                           const QStringList &strlist,
                                                           const QString &title = QString(),
                                                           const KGuiItem &buttonContinue = KStandardGuiItem::cont(),
                                                           const KGuiItem &buttonCancel = KStandardGuiItem::cancel(),
                                                           const QString &dontAskAgainName = QString(),
                                                           Options options = Notify);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * Display a Yes/No/Cancel "warning" dialog.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Warning").
 * @param buttonYes The text for the first button.
 *                  The default is KStandardGuiItem::yes().
 * @param buttonNo  The text for the second button.
 *                  The default is KStandardGuiItem::no().
 * @param buttonCancel  The text for the third button.
 *                  The default is KStandardGuiItem::cancel().
 * @param dontAskAgainName If provided, a checkbox is added with which
 *                further questions can be turned off. If turned off
 *                all questions will be automatically answered with the
 *                last answer (either Yes or No).
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options  see Options
 *
 * @return  @p Yes is returned if the Yes-button is pressed. @p No is returned
 *          if the No-button is pressed. @p Cancel is retunred if the Cancel-
 *          button is pressed.
 *
 * To be used for questions "Do you want to save your changes?"
 * The text should explain the implication of choosing 'No'.
 *
 * The default button is "Yes". Pressing "Esc" selects "Cancel"
 *
 * @deprecated Since 5.100, use warningTwoActionsCancel()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use warningTwoActionsCancel()")
ButtonCode warningYesNoCancel(QWidget *parent,
                              const QString &text,
                              const QString &title = QString(),
                              const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                              const KGuiItem &buttonNo = KStandardGuiItem::no(),
                              const KGuiItem &buttonCancel = KStandardGuiItem::cancel(),
                              const QString &dontAskAgainName = QString(),
                              Options options = Notify);
#endif

/**
 * Display a "warning" dialog with two action buttons and a cancel button.
 *
 * To be used for questions like "Shall I update your configuration?".
 * The text should explain the implication of both actions.
 *
 * The default button is the cancel button. Pressing "Esc" triggers the cancel button.
 *
 * @param parent  the parent widget
 * @param text    the message string
 * @param title   the message box title. If an empty string, defaults to i18n("Warning").
 * @param primaryAction the action for the primary button
 * @param secondaryAction the action for the secondary button
 * @param cancelAction the action for the cancel button
 * @param dontAskAgainName If not an empty string, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options see Options
 *
 * @returns @c PrimaryAction if the primary button is triggered, @c SecondaryAction
 *          if the secondary button is triggered. @c Cancel if the cancel button is triggered.
 *
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode warningTwoActionsCancel(QWidget *parent,
                                   const QString &text,
                                   const QString &title,
                                   const KGuiItem &primaryAction,
                                   const KGuiItem &secondaryAction,
                                   const KGuiItem &cancelAction = KStandardGuiItem::cancel(),
                                   const QString &dontAskAgainName = QString(),
                                   Options options = Options(Notify | Dangerous));

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * Display a Yes/No/Cancel "warning" dialog with a listbox to show information
 * to the user.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param strlist List of strings to be written in the listbox. If the
 *                list is empty, it doesn't show any listbox, working
 *                as warningYesNoCancel.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Warning").
 * @param buttonYes The text for the first button.
 *                  The default is KStandardGuiItem::yes().
 * @param buttonNo  The text for the second button.
 *                  The default is KStandardGuiItem::no().
 * @param buttonCancel  The text for the third button.
 *                  The default is KStandardGuiItem::cancel().
 * @param dontAskAgainName If provided, a checkbox is added with which
 *                further questions can be turned off. If turned off
 *                all questions will be automatically answered with the
 *                last answer (either Yes or No).
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options  see Options
 *
 * @return  @p Yes is returned if the Yes-button is pressed. @p No is returned
 *          if the No-button is pressed. @p Cancel is retunred if the Cancel-
 *          button is pressed.
 *
 * To be used for questions "Do you want to save your changes?"
 * The text should explain the implication of choosing 'No'.
 *
 * The default button is "Yes". Pressing "Esc" selects "Cancel"
 *
 * @deprecated Since 5.100, use warningTwoActionsCancelList()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use warningTwoActionsCancelList()")
ButtonCode warningYesNoCancelList(QWidget *parent,
                                  const QString &text,
                                  const QStringList &strlist,
                                  const QString &title = QString(),
                                  const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                                  const KGuiItem &buttonNo = KStandardGuiItem::no(),
                                  const KGuiItem &buttonCancel = KStandardGuiItem::cancel(),
                                  const QString &dontAskAgainName = QString(),
                                  Options options = Notify);
#endif

/**
 * Display a "warning" dialog with a listbox to show information
 * to the user, two action buttons and a cancel button.
 *
 * To be used for questions like "Shall I update your configuration?".
 * The text should explain the implication of both actions.
 *
 * The default button is the cancel button. Pressing "Esc" triggers the cancel button.
 *
 * @param parent  the parent widget
 * @param text    the message string
 * @param strlist a List of strings to be written in the listbox. If the
 *                list is empty, it doesn't show any listbox, working
 *                as warningTwoActionsCancel().
 * @param title   the message box title. If an empty string, defaults to i18n("Warning").
 * @param primaryAction the action for the primary button
 * @param secondaryAction the action for the secondary button
 * @param cancelAction the action for the cancel button
 * @param dontAskAgainName If not an empty string, a checkbox is added with which
 *                further confirmation can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 *                If @p dontAskAgainName starts with a ':' then the setting
 *                is stored in the global config file.
 * @param options see Options
 *
 * @returns @c PrimaryAction if the primary button is triggered, @c SecondaryAction
 *          if the secondary button is triggered. @c Cancel if the cancel button is triggered.
 *
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode warningTwoActionsCancelList(QWidget *parent,
                                       const QString &text,
                                       const QStringList &strlist,
                                       const QString &title,
                                       const KGuiItem &primaryAction,
                                       const KGuiItem &secondaryAction,
                                       const KGuiItem &cancelAction = KStandardGuiItem::cancel(),
                                       const QString &dontAskAgainName = QString(),
                                       Options options = Options(Notify | Dangerous));

/**
 * Display an "Error" dialog.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Error").
 * @param options  see Options
 *
 * Your program messed up and now it's time to inform the user.
 * To be used for important things like "Sorry, I deleted your hard disk."
 *
 * The default button is "&OK". Pressing "Esc" selects the OK-button.
 *
 * @note The OK button will always have the i18n'ed text '&OK'.
 */
KWIDGETSADDONS_EXPORT void error(QWidget *parent, const QString &text, const QString &title = QString(), Options options = Notify);

/**
 * Display an "Error" dialog.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Error").
 * @param buttonOk The text for the only button.
 *                 The default is KStandardGuiItem::ok().
 * @param options  see Options
 *
 * There is only one button, therefore it's the default button, and pressing "Esc" selects it as well.
 *
 * @since 5.97
 */
KWIDGETSADDONS_EXPORT
void error(QWidget *parent,
           const QString &text,
           const QString &title /*= QString()*/,
           const KGuiItem &buttonOk /*= KStandardGuiItem::ok()*/,
           Options options = Notify); // TODO KF6 merge with previous overload

/**
 * Display an "Error" dialog with a listbox.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param strlist List of strings to be written in the listbox. If the
 *                list is empty, it doesn't show any listbox, working
 *                as error().
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Error").
 * @param options  see Options
 *
 * Your program messed up and now it's time to inform the user.
 * To be used for important things like "Sorry, I deleted your hard disk."
 *
 * The default button is "&OK". Pressing "Esc" selects the OK-button.
 *
 * @note The OK button will always have the i18n'ed text '&OK'.
 */
KWIDGETSADDONS_EXPORT void
errorList(QWidget *parent, const QString &text, const QStringList &strlist, const QString &title = QString(), Options options = Notify);

/**
 * Displays an "Error" dialog with a "Details >>" button.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param details Detailed message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Error").
 * @param options  see Options
 *
 * Your program messed up and now it's time to inform the user.
 * To be used for important things like "Sorry, I deleted your hard disk."
 *
 * The @p details message can contain additional information about
 * the problem and can be shown on request to advanced/interested users.
 *
 * The default button is "&OK". Pressing "Esc" selects the OK-button.
 *
 * @note The OK button will always have the i18n'ed text '&OK'.
 */
KWIDGETSADDONS_EXPORT void
detailedError(QWidget *parent, const QString &text, const QString &details, const QString &title = QString(), Options options = Notify);

/**
 * Displays an "Error" dialog with a "Details >>" button.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param details Detailed message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Error").
 * @param buttonOk The text for the only button.
 *                 The default is KStandardGuiItem::ok().
 * @param options  see Options
 *
 * Your program messed up and now it's time to inform the user.
 * To be used for important things like "Sorry, I deleted your hard disk."
 *
 * The @p details message can contain additional information about
 * the problem and can be shown on request to advanced/interested users.
 *
 * There is only one button, therefore it's the default button, and pressing "Esc" selects it as well.
 *
 * @since 5.97
 */
KWIDGETSADDONS_EXPORT
void detailedError(QWidget *parent,
                   const QString &text,
                   const QString &details,
                   const QString &title /*= QString()*/,
                   const KGuiItem &buttonOk /*= KStandardGuiItem::ok()*/,
                   Options options = Notify); // TODO KF6 merge with previous overload

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 97)
/**
 * Display a "Sorry" dialog.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Sorry").
 * @param options  see OptionsType
 *
 * Either your program messed up and asks for understanding
 * or your user did something stupid.
 *
 * To be used for small problems like
 * "Sorry, I can't find the file you specified."
 *
 * The default button is "&OK". Pressing "Esc" selects the OK-button.
 *
 * @note The OK button will always have the i18n'ed text '&OK'.
 * See the overload with a KGuiItem argument to change that.
 *
 * @deprecated Since 5.97, use error().
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 97, "Use error().")
void sorry(QWidget *parent, const QString &text, const QString &title = QString(), Options options = Notify);
#endif

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 97)
/**
 * Display a "Sorry" dialog.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Sorry").
 * @param buttonOk The text for the only button.
 *                 The default is KStandardGuiItem::ok().
 * @param options  see OptionsType
 *
 * Either your program messed up and asks for understanding
 * or your user did something stupid.
 *
 * To be used for small problems like
 * "Sorry, I can't find the file you specified."
 *
 * The default button is "&OK". Pressing "Esc" selects the OK-button.
 *
 * There is only one button, therefore it's the default button, and pressing "Esc" selects it as well.
 * @since 5.63
 *
 * @deprecated Since 5.97 use error().
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 97, "Use error().")
void sorry(QWidget *parent,
           const QString &text,
           const QString &title /*= QString()*/,
           const KGuiItem &buttonOk /*= KStandardGuiItem::ok()*/,
           Options options = Notify);
#endif

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 97)
/**
 * Displays a "Sorry" dialog with a "Details >>" button.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param details Detailed message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Sorry").
 * @param options  see Options
 *
 * Either your program messed up and asks for understanding
 * or your user did something stupid.
 *
 * To be used for small problems like
 * "Sorry, I can't find the file you specified."
 *
 * And then @p details can contain something like
 * "foobar.txt was not found in any of
 *  the following directories:
 *  /usr/bin,/usr/local/bin,/usr/sbin"
 *
 * The default button is "&OK". Pressing "Esc" selects the OK-button.
 *
 * @note The OK button will always have the i18n'ed text '&OK'.
 * See the overload with a KGuiItem argument to change that.
 *
 * @deprecated Since 5.97 use detailedError().
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 97, "Use detailedError().")
void detailedSorry(QWidget *parent, const QString &text, const QString &details, const QString &title = QString(), Options options = Notify);
#endif

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 97)
/**
 * Displays a "Sorry" dialog with a "Details >>" button.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param details Detailed message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Sorry").
 * @param buttonOk The text for the only button.
 *                 The default is KStandardGuiItem::ok().
 * @param options  see Options
 *
 * Either your program messed up and asks for understanding
 * or your user did something stupid.
 *
 * To be used for small problems like
 * "Sorry, I can't find the file you specified."
 *
 * And then @p details can contain something like
 * "foobar.txt was not found in any of
 *  the following directories:
 *  /usr/bin,/usr/local/bin,/usr/sbin"
 *
 * There is only one button, therefore it's the default button, and pressing "Esc" selects it as well.
 * @since 5.63
 *
 * @deprecated Since 5.97 use detailedError().
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 97, "Use detailedError().")
void detailedSorry(QWidget *parent,
                   const QString &text,
                   const QString &details,
                   const QString &title /* = QString() */,
                   const KGuiItem &buttonOk /*= KStandardGuiItem::ok()*/,
                   Options options = Notify);
#endif

/**
 * Display an "Information" dialog.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Information").
 * @param dontShowAgainName If provided, a checkbox is added with which
 *                further notifications can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 * @param options  see Options
 *
 *
 * Your program wants to tell the user something.
 * To be used for things like:
 * "Your bookmarks have been rearranged."
 *
 * The default button is "&OK". Pressing "Esc" selects the OK-button.
 *
 *  @note The OK button will always have the i18n'ed text '&OK'.
 */
KWIDGETSADDONS_EXPORT void
information(QWidget *parent, const QString &text, const QString &title = QString(), const QString &dontShowAgainName = QString(), Options options = Notify);

/**
 * Display an "Information" dialog with a listbox.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param strlist List of strings to be written in the listbox. If the
 *                list is empty, it doesn't show any listbox, working
 *                as information.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("Information").
 * @param dontShowAgainName If provided, a checkbox is added with which
 *                further notifications can be turned off.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 *                The setting is stored in the "Notification Messages" group.
 * @param options  see Options
 *
 *
 * Your program wants to tell the user something.
 * To be used for things like:
 * "The following bookmarks have been rearranged:"
 *
 * The default button is "&OK". Pressing "Esc" selects the OK-button.
 *
 *  @note The OK button will always have the i18n'ed text '&OK'.
 */
KWIDGETSADDONS_EXPORT void informationList(QWidget *parent,
                                           const QString &text,
                                           const QStringList &strlist,
                                           const QString &title = QString(),
                                           const QString &dontShowAgainName = QString(),
                                           Options options = Notify);

/**
 * Enable all messages which have been turned off with the
 * @p dontShowAgainName feature.
 */
KWIDGETSADDONS_EXPORT void enableAllMessages();

/**
 * Re-enable a specific @p dontShowAgainName messages that had
 * previously been turned off.
 * @see saveDontShowAgainTwoActions()
 * @see saveDontShowAgainContinue()
 */
KWIDGETSADDONS_EXPORT void enableMessage(const QString &dontShowAgainName);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 97)
/**
 * Display an "About" dialog.
 *
 * @param parent  Parent widget.
 * @param text    Message string.
 * @param title   Message box title. The application name is added to
 *                the title. The default title is i18n("About \<appname\>").
 * @param options  see Options
 *
 *
 * Your program wants to show some general information about the application
 * like the authors's names and email addresses.
 *
 * The default button is "&OK".
 *
 *  @note The OK button will always have the i18n'ed text '&OK'.
 *
 * @deprecated Since 5.0, use QMessageBox::about(). No known users of the extre features here.
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION_BELATED(5, 97, 5, 0, "Use QMessageBox::about()")
void about(QWidget *parent, const QString &text, const QString &title = QString(), Options options = Notify);
#endif

/**
 * Alternate method to show a messagebox:
 *
 * @param parent Parent widget.
 * @param type type of message box: QuestionTwoActions, WarningTwoActions, WarningContinueCancel...
 * @param text Message string.
 * @param title Message box title.
 * @param primaryAction The KGuiItem for the first button.
 *                      The default (deprecated since 5.100) is KStandardGuiItem::yes().
 * @param secondaryAction The KGuiItem for the second button.
 *                         The default (deprecated since 5.100) is KStandardGuiItem::no().
 * @param cancelAction The text for the third button.
 *                     The default is KStandardGuiItem::cancel().
 * @param dontShowAskAgainName If provided, a checkbox is added with which
 *                further questions/information can be turned off. If turned off
 *                all questions will be automatically answered with the
 *                last answer (either Yes or No), if the message box needs an answer.
 *                The string is used to lookup and store the setting
 *                in the applications config file.
 * @param options  see Options
 * Note: for ContinueCancel, buttonYes is the continue button and buttonNo is unused.
 *       and for Information, none is used.
 * @return a button code, as defined in KMessageBox.
 */
KWIDGETSADDONS_EXPORT
ButtonCode messageBox(QWidget *parent,
                      DialogType type,
                      const QString &text,
#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
                      const QString &title = QString(),
                      const KGuiItem &primaryAction = KStandardGuiItem::yes(),
                      const KGuiItem &secondaryAction = KStandardGuiItem::no(),
#else
                      const QString &title,
                      const KGuiItem &primaryAction,
                      const KGuiItem &secondaryAction,
#endif
                      const KGuiItem &cancelAction = KStandardGuiItem::cancel(),
                      const QString &dontShowAskAgainName = QString(),
                      Options options = Notify);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * @return true if the corresponding yes/no message box should be shown.
 * @param dontShowAgainName the name that identify the message box. If
 * empty, true is always returned.
 * @param result is set to the result (Yes or No) that was chosen the last
 * time the message box was shown. Only meaningful, if the message box
 * should not be shown.
 *
 * @deprecated Since 5.100, use shouldBeShownTwoActions()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use shouldBeShownTwoActions()")
bool shouldBeShownYesNo(const QString &dontShowAgainName, ButtonCode &result);
#endif

/**
 * @param dontShowAgainName the name that identifies the message box.
 *                          If empty, @c true is always returned.
 * @param result reference to a variable to be set to the choice (@c PrimaryAction or @c SecondaryAction)
 *               that was chosen the last time the message box was shown.
 *               Only meaningful if the message box should not be shown.
 * @returns @c true if the corresponding two actions message box should be shown, @c false otherwise.
 *
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
bool shouldBeShownTwoActions(const QString &dontShowAgainName, ButtonCode &result);

/**
 * @return true if the corresponding continue/cancel message box should be
 * shown.
 * @param dontShowAgainName the name that identify the message box. If
 * empty, true is always returned.
 */
KWIDGETSADDONS_EXPORT bool shouldBeShownContinue(const QString &dontShowAgainName);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * Save the fact that the yes/no message box should not be shown again.
 * @param dontShowAgainName the name that identify the message box. If
 * empty, this method does nothing.
 * @param result the value (Yes or No) that should be used as the result
 * for the message box.
 *
 * @deprecated Since 5.100, use saveDontShowAgainTwoActions()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use saveDontShowAgainTwoActions()")
void saveDontShowAgainYesNo(const QString &dontShowAgainName, ButtonCode result);
#endif

/**
 * Save the fact that a two actions message box should not be shown again.
 *
 * @param dontShowAgainName the name that identifies the message box.
 *                          If empty, this method does nothing.
 * @param result the value (@c PrimaryAction or @c SecondaryAction) that should be used
 *               as the result for the message box.
 *
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
void saveDontShowAgainTwoActions(const QString &dontShowAgainName, ButtonCode result);

/**
 * Save the fact that the continue/cancel message box should not be shown
 * again.
 * @param dontShowAgainName the name that identify the message box. If
 * empty, this method does nothing.
 */
KWIDGETSADDONS_EXPORT void saveDontShowAgainContinue(const QString &dontShowAgainName);

/**
 * Use @p cfg for all settings related to the dontShowAgainName feature.
 * If @p cfg is 0 (default) KGlobal::config() will be used.
 */
KWIDGETSADDONS_EXPORT void setDontShowAgainConfig(KConfig *cfg);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 0)
/**
 * @deprecated since 5.0, use setDontShowAgainConfig()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 0, "Use KMessageBox::setDontShowAgainConfig(KConfig*)")
inline void setDontShowAskAgainConfig(KConfig *cfg)
{
    setDontShowAgainConfig(cfg);
}
#endif

/**
 * Use @p dontAskAgainInterface for all settings related to the dontShowAgain feature.
 * This method does not take ownership of @p dontAskAgainInterface.
 *
 * @since 5.0
 */
KWIDGETSADDONS_EXPORT void setDontShowAgainInterface(KMessageBoxDontAskAgainInterface *dontAskAgainInterface);

/**
 * Use @p notifyInterface to send notifications.
 * This method does not take ownership of @p notifyInterface.
 *
 * @since 5.0
 */
KWIDGETSADDONS_EXPORT void setNotifyInterface(KMessageBoxNotifyInterface *notifyInterface);

/**
 * Create content and layout of a standard dialog
 *
 * @param dialog  The parent dialog base
 * @param buttons a QDialogButtonBox instance. This function will take care of connecting to it.
 * @param icon    Which predefined icon the message box shall show.
 * @param text    Message string.
 * @param strlist List of strings to be written in the listbox.
 *                If the list is empty, it doesn't show any listbox
 * @param ask     The text of the checkbox. If empty none will be shown.
 * @param checkboxReturn The result of the checkbox. If it's initially
 *                true then the checkbox will be checked by default.
 *                May be a null pointer. Incompatible with NoExec.
 * @param options  see Options
 * @param details Detailed message string.
 * @return A QDialogButtonBox::StandardButton button code, not a KMessageBox
 *         button code, based on the buttonmask given to the constructor of
 *         the @p dialog (ie. will return QDialogButtonBox::Yes instead of
 *         KMessageBox::PrimaryAction). Will return QDialogButtonBox::NoButton if the
 *         message box is queued for display instead of exec()ed immediately
 *         or if the option NoExec is set.
 * @note   Unless NoExec is used,
 *         the @p dialog that is passed in is deleted by this
 *         function. Do not delete it yourself.
 */
KWIDGETSADDONS_EXPORT QDialogButtonBox::StandardButton createKMessageBox(QDialog *dialog,
                                                                         QDialogButtonBox *buttons,
                                                                         QMessageBox::Icon icon, // krazy:exclude=qclasses
                                                                         const QString &text,
                                                                         const QStringList &strlist,
                                                                         const QString &ask,
                                                                         bool *checkboxReturn,
                                                                         Options options,
                                                                         const QString &details = QString());

/**
 * Create content and layout of a standard dialog
 *
 * @param dialog  The parent dialog base
 * @param buttons a QDialogButtonBox instance. This function will take care of connecting to it.
 * @param icon    A QPixmap containing the icon to be displayed in the
 *                dialog next to the text.
 * @param text    Message string.
 * @param strlist List of strings to be written in the listbox.
 *                If the list is empty, it doesn't show any listbox
 * @param ask     The text of the checkbox. If empty none will be shown.
 * @param checkboxReturn The result of the checkbox. If it's initially
 *                true then the checkbox will be checked by default.
 *                May be a null pointer. Incompatible with NoExec.
 * @param options  see Options
 * @param details Detailed message string.
 * @param notifyType The type of notification to send when this message
 *                is presentend.
 * @return A QDialogButtonBox::StandardButton button code, not a KMessageBox
 *         button code, based on the buttonmask given to the constructor of
 *         the @p dialog (ie. will return QDialogButtonBox::Yes instead of
 *         KMessageBox::PrimaryAction). Will return QDialogButtonBox::NoButton if the
 *         message box is queued for display instead of exec()ed immediately
 *         or if the option NoExec is set.
 * @note   Unless NoExec is used,
 *         the @p dialog that is passed in is deleted by this
 *         function. Do not delete it yourself.
 */
KWIDGETSADDONS_EXPORT QDialogButtonBox::StandardButton createKMessageBox(QDialog *dialog,
                                                                         QDialogButtonBox *buttons,
                                                                         const QIcon &icon,
                                                                         const QString &text,
                                                                         const QStringList &strlist,
                                                                         const QString &ask,
                                                                         bool *checkboxReturn,
                                                                         Options options,
                                                                         const QString &details = QString(),
                                                                         QMessageBox::Icon notifyType = QMessageBox::Information); // krazy:exclude=qclasses

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @deprecated Since 5.100, use questionTwoActionsWId()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use questionTwoActionsWId()")
ButtonCode questionYesNoWId(WId parent_id,
                            const QString &text,
                            const QString &title = QString(),
                            const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                            const KGuiItem &buttonNo = KStandardGuiItem::no(),
                            const QString &dontAskAgainName = QString(),
                            Options options = Notify);
#endif

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @see questionTwoActions()
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode questionTwoActionsWId(WId parent_id,
                                 const QString &text,
                                 const QString &title,
                                 const KGuiItem &primaryAction,
                                 const KGuiItem &secondaryAction,
                                 const QString &dontAskAgainName = QString(),
                                 Options options = Notify);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @deprecated Since 5.100, use questionTwoActionsCancelWId()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use questionTwoActionsCancelWId()")
ButtonCode questionYesNoCancelWId(WId parent_id,
                                  const QString &text,
                                  const QString &title = QString(),
                                  const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                                  const KGuiItem &buttonNo = KStandardGuiItem::no(),
                                  const KGuiItem &buttonCancel = KStandardGuiItem::cancel(),
                                  const QString &dontAskAgainName = QString(),
                                  Options options = Notify);
#endif

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @see questionTwoActionsCancel()
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode questionTwoActionsCancelWId(WId parent_id,
                                       const QString &text,
                                       const QString &title,
                                       const KGuiItem &primaryAction,
                                       const KGuiItem &secondaryAction,
                                       const KGuiItem &cancelAction = KStandardGuiItem::cancel(),
                                       const QString &dontAskAgainName = QString(),
                                       Options options = Notify);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @deprecated Since 5.100, use questionTwoActionsListWId()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use questionTwoActionsListWId()")
ButtonCode questionYesNoListWId(WId parent_id,
                                const QString &text,
                                const QStringList &strlist,
                                const QString &title = QString(),
                                const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                                const KGuiItem &buttonNo = KStandardGuiItem::no(),
                                const QString &dontAskAgainName = QString(),
                                Options options = Notify);
#endif

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @see questionTwoActionsList()
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode questionTwoActionsListWId(WId parent_id,
                                     const QString &text,
                                     const QStringList &strlist,
                                     const QString &title,
                                     const KGuiItem &primaryAction,
                                     const KGuiItem &secondaryAction,
                                     const QString &dontAskAgainName = QString(),
                                     Options options = Notify);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @deprecated Since 5.100, use warningTwoActionsWId()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use warningTwoActionsWId()")
ButtonCode warningYesNoWId(WId parent_id,
                           const QString &text,
                           const QString &title = QString(),
                           const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                           const KGuiItem &buttonNo = KStandardGuiItem::no(),
                           const QString &dontAskAgainName = QString(),
                           Options options = Options(Notify | Dangerous));
#endif

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @see warningTwoActions()
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode warningTwoActionsWId(WId parent_id,
                                const QString &text,
                                const QString &title,
                                const KGuiItem &primaryAction,
                                const KGuiItem &secondaryAction,
                                const QString &dontAskAgainName = QString(),
                                Options options = Options(Notify | Dangerous));

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @deprecated Since 5.100, use warningTwoActionsListWId()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use warningTwoActionsListWId()")
ButtonCode warningYesNoListWId(WId parent_id,
                               const QString &text,
                               const QStringList &strlist,
                               const QString &title = QString(),
                               const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                               const KGuiItem &buttonNo = KStandardGuiItem::no(),
                               const QString &dontAskAgainName = QString(),
                               Options options = Options(Notify | Dangerous));
#endif

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @see warningTwoActionsList()
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode warningTwoActionsListWId(WId parent_id,
                                    const QString &text,
                                    const QStringList &strlist,
                                    const QString &title,
                                    const KGuiItem &primaryAction,
                                    const KGuiItem &secondaryAction,
                                    const QString &dontAskAgainName = QString(),
                                    Options options = Options(Notify | Dangerous));

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 */
KWIDGETSADDONS_EXPORT ButtonCode warningContinueCancelWId(WId parent_id,
                                                          const QString &text,
                                                          const QString &title = QString(),
                                                          const KGuiItem &buttonContinue = KStandardGuiItem::cont(),
                                                          const KGuiItem &buttonCancel = KStandardGuiItem::cancel(),
                                                          const QString &dontAskAgainName = QString(),
                                                          Options options = Notify);

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 */
KWIDGETSADDONS_EXPORT ButtonCode warningContinueCancelListWId(WId parent_id,
                                                              const QString &text,
                                                              const QStringList &strlist,
                                                              const QString &title = QString(),
                                                              const KGuiItem &buttonContinue = KStandardGuiItem::cont(),
                                                              const KGuiItem &buttonCancel = KStandardGuiItem::cancel(),
                                                              const QString &dontAskAgainName = QString(),
                                                              Options options = Notify);

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @deprecated Since 5.100, use warningTwoActionsCancelWId()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use warningTwoActionsCancelWId()")
ButtonCode warningYesNoCancelWId(WId parent_id,
                                 const QString &text,
                                 const QString &title = QString(),
                                 const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                                 const KGuiItem &buttonNo = KStandardGuiItem::no(),
                                 const KGuiItem &buttonCancel = KStandardGuiItem::cancel(),
                                 const QString &dontAskAgainName = QString(),
                                 Options options = Notify);
#endif

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @see warningTwoActionsCancel()
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode warningTwoActionsCancelWId(WId parent_id,
                                      const QString &text,
                                      const QString &title,
                                      const KGuiItem &primaryAction,
                                      const KGuiItem &secondaryAction,
                                      const KGuiItem &cancelAction = KStandardGuiItem::cancel(),
                                      const QString &dontAskAgainName = QString(),
                                      Options options = Options(Notify | Dangerous));

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @deprecated Since 5.100, use warningTwoActionsCancelListWId()
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 100, "Use warningTwoActionsCancelListWId()")
ButtonCode warningYesNoCancelListWId(WId parent_id,
                                     const QString &text,
                                     const QStringList &strlist,
                                     const QString &title = QString(),
                                     const KGuiItem &buttonYes = KStandardGuiItem::yes(),
                                     const KGuiItem &buttonNo = KStandardGuiItem::no(),
                                     const KGuiItem &buttonCancel = KStandardGuiItem::cancel(),
                                     const QString &dontAskAgainName = QString(),
                                     Options options = Notify);
#endif

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 *
 * @see warningTwoActionsCancelList()
 * @since 5.100
 */
KWIDGETSADDONS_EXPORT
ButtonCode warningTwoActionsCancelListWId(WId parent_id,
                                          const QString &text,
                                          const QStringList &strlist,
                                          const QString &title,
                                          const KGuiItem &primaryAction,
                                          const KGuiItem &secondaryAction,
                                          const KGuiItem &cancelAction = KStandardGuiItem::cancel(),
                                          const QString &dontAskAgainName = QString(),
                                          Options options = Options(Notify | Dangerous));

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 */
KWIDGETSADDONS_EXPORT void errorWId(WId parent_id, const QString &text, const QString &title = QString(), Options options = Notify);

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 */
KWIDGETSADDONS_EXPORT void
errorListWId(WId parent_id, const QString &text, const QStringList &strlist, const QString &title = QString(), Options options = Notify);

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 */
KWIDGETSADDONS_EXPORT void
detailedErrorWId(WId parent_id, const QString &text, const QString &details, const QString &title = QString(), Options options = Notify);

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 * @since 5.97
 */
KWIDGETSADDONS_EXPORT
void detailedErrorWId(WId parent_id,
                      const QString &text,
                      const QString &details,
                      const QString &title /*= QString()*/,
                      const KGuiItem &buttonOk /*= KStandardGuiItem::ok()*/,
                      Options options = Notify); // TODO KF6 merge with previous overload

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 97)
/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 * @deprecated Since 5.97 use errorWId().
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 97, "Use errorWId().")
void sorryWId(WId parent_id, const QString &text, const QString &title = QString(), Options options = Notify);
#endif

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 97)
/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 * @deprecated Since 5.97 use detailedErrorWId().
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 97, "Use detailedErrorWId().")
void detailedSorryWId(WId parent_id, const QString &text, const QString &details, const QString &title = QString(), Options options = Notify);
#endif

#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 97)
/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 * @since 5.63
 * @deprecated Since 5.97 use detailedErrorWId().
 */
KWIDGETSADDONS_EXPORT
KWIDGETSADDONS_DEPRECATED_VERSION(5, 97, "Use detailedErrorWId().")
void detailedSorryWId(WId parent_id,
                      const QString &text,
                      const QString &details,
                      const QString &title /*= QString()*/,
                      const KGuiItem &buttonOk /*= KStandardGuiItem::ok()*/,
                      Options options = Notify);
#endif

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 */
KWIDGETSADDONS_EXPORT void
informationWId(WId parent_id, const QString &text, const QString &title = QString(), const QString &dontShowAgainName = QString(), Options options = Notify);

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 */
KWIDGETSADDONS_EXPORT void informationListWId(WId parent_id,
                                              const QString &text,
                                              const QStringList &strlist,
                                              const QString &title = QString(),
                                              const QString &dontShowAgainName = QString(),
                                              Options options = Notify);

/**
 * This function accepts the window id of the parent window, instead
 * of QWidget*. It should be used only when necessary.
 */
KWIDGETSADDONS_EXPORT
ButtonCode messageBoxWId(WId parent_id,
                         DialogType type,
                         const QString &text,
#if KWIDGETSADDONS_ENABLE_DEPRECATED_SINCE(5, 100)
                         const QString &title = QString(),
                         const KGuiItem &primaryAction = KStandardGuiItem::yes(),
                         const KGuiItem &secondaryAction = KStandardGuiItem::no(),
#else
                         const QString &title,
                         const KGuiItem &primaryAction,
                         const KGuiItem &secondaryAction,
#endif
                         const KGuiItem &cancelAction = KStandardGuiItem::cancel(),
                         const QString &dontShowAskAgainName = QString(),
                         Options options = Notify);
}

#endif
