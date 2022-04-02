/*
  SPDX-FileCopyrightText: 2004 Bram Schoenmakers <bramschoenmakers@kde.nl>
  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KDATEPICKERPOPUP_H
#define KDATEPICKERPOPUP_H

#include <kwidgetsaddons_export.h>

#include <QDate>
#include <QMenu>

#include <memory>

class KDatePicker;
class KDatePickerPopupPrivate;

/**
 * @short This menu helps the user to select a date quickly.
 *
 * This menu helps the user to select a date quickly. It offers various
 * modes of selecting, e.g. with a KDatePicker or with words like "Tomorrow".
 *
 * The available modes are:
 *
 * @li NoDate: A menu-item with "No Date". If chosen, the datepicker will emit
 *     a null QDate.
 * @li DatePicker: Shows a KDatePicker-widget.
 * @li Words: Shows items like "Today", "Tomorrow" or "Next Week".
 *
 * @author Bram Schoenmakers <bram_s@softhome.net>
 *
 * @since 5.94
 */
class KWIDGETSADDONS_EXPORT KDatePickerPopup : public QMenu
{
    Q_OBJECT
    Q_PROPERTY(Modes modes READ modes WRITE setModes)

public:
    /**
     * Describes the available selection modes.
     */
    enum Mode {
        NoDate = 1, ///< A menu-item with "No Date". Will always return an invalid date.
        DatePicker = 2, ///< A menu-item with a KDatePicker.
        Words = 4 ///< A menu-item with list of words that describe a date.
    };

    /**
     * Describes a set of combined modes.
     */
    Q_DECLARE_FLAGS(Modes, Mode)

    /**
     * Creates a new date picker popup.
     *
     * @param modes The selection modes that shall be offered
     * @param date The initial date of date picker widget.
     * @param parent The parent object.
     */
    explicit KDatePickerPopup(Modes modes = DatePicker, QDate date = QDate::currentDate(), QWidget *parent = nullptr);

    /**
     * Destroys the date picker popup.
     */
    ~KDatePickerPopup() override;

    /**
     * Returns the currently used selection modes.
     */
    Modes modes() const;

    /**
     * Set the selection modes to use.
     */
    void setModes(Modes modes);

    /**
     * Returns the used KDatePicker object.
     */
    Q_REQUIRED_RESULT KDatePicker *datePicker() const;

public Q_SLOTS:
    /**
     * Sets the current @p date.
     */
    void setDate(QDate date);

Q_SIGNALS:
    /**
     * This signal is emitted whenever the user has selected a new date.
     *
     * @param date The new date.
     */
    void dateChanged(const QDate &date);

private:
    //@cond PRIVATE
    std::unique_ptr<KDatePickerPopupPrivate> const d;
    //@endcond
};

Q_DECLARE_OPERATORS_FOR_FLAGS(KDatePickerPopup::Modes)

#endif
