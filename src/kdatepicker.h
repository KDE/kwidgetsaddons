/*  -*- C++ -*-
    This file is part of the KDE libraries
    SPDX-FileCopyrightText: 1997 Tim D. Gilman <tdgilman@best.org>
    SPDX-FileCopyrightText: 1998-2001 Mirko Boehm <mirko@kde.org>
    SPDX-FileCopyrightText: 2007 John Layt <john@layt.net>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KDATEPICKER_H
#define KDATEPICKER_H

#include <kwidgetsaddons_export.h>

#include <QDate>
#include <QFrame>
#include <memory>

class QLineEdit;
class KDateTable;

/**
 * @class KDatePicker kdatepicker.h KDatePicker
 *
 * @short A date selection widget.
 *
 * Provides a widget for calendar date input.
 *
 * Different from the previous versions, it now emits two types of signals,
 * either dateSelected() or dateEntered() (see documentation for both signals).
 *
 * A line edit has been added in the newer versions to allow the user
 * to select a date directly by entering numbers like 19990101
 * or 990101.
 *
 * \image html kdatepicker.png "KDatePicker Widget"
 *
 * @author Tim Gilman, Mirko Boehm
 */
class KWIDGETSADDONS_EXPORT KDatePicker : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged USER true)
    Q_PROPERTY(bool closeButton READ hasCloseButton WRITE setCloseButton)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)

public:
    /**
     * The constructor. The current date will be displayed initially.
     */
    explicit KDatePicker(QWidget *parent = nullptr);

    /**
     * The constructor. The given date will be displayed initially.
     */
    explicit KDatePicker(const QDate &dt, QWidget *parent = nullptr);

    /**
     * The destructor.
     */
    ~KDatePicker() override;

    /**
     * The size hint for date pickers. The size hint recommends the
     * minimum size of the widget so that all elements may be placed
     * without clipping. This sometimes looks ugly, so when using the
     * size hint, try adding 28 to each of the reported numbers of
     * pixels.
     */
    QSize sizeHint() const override;

    /**
     * Sets the date.
     *
     * @returns @p false and does not change anything if the date given is invalid.
     */
    bool setDate(const QDate &date);

    /**
     * @returns the selected date.
     */
    const QDate &date() const;

    /**
     * Sets the font size of the widgets elements.
     */
    void setFontSize(int);

    /**
     * Returns the font size of the widget elements.
     */
    int fontSize() const;

    /**
     * By calling this method with @p enable = true, KDatePicker will show
     * a little close-button in the upper button-row. Clicking the
     * close-button will cause the KDatePicker's topLevelWidget()'s close()
     * method being called. This is mostly useful for toplevel datepickers
     * without a window manager decoration.
     * @see hasCloseButton
     */
    void setCloseButton(bool enable);

    /**
     * @returns true if a KDatePicker shows a close-button.
     * @see setCloseButton
     */
    bool hasCloseButton() const;

    /**
     * Sets the range of dates that can be accepted.
     *
     * Invalid dates can be used to define open-ended ranges.
     * If both values are valid, the minimum date must be less than
     * or equal to the maximum date, otherwise the date range will
     * not be set.
     *
     * @param minDate the minimum date
     * @param maxDate the maximum date
     *
     * @since 6.12
     */
    void setDateRange(const QDate &minDate, const QDate &maxDate = QDate());

protected:
    /// to catch move keyEvents when QLineEdit has keyFocus
    bool eventFilter(QObject *o, QEvent *e) override;
    /// the resize event
    void resizeEvent(QResizeEvent *) override;
    void changeEvent(QEvent *event) override;

protected Q_SLOTS:
    void dateChangedSlot(const QDate &date);
    void tableClickedSlot();
    void monthForwardClicked();
    void monthBackwardClicked();
    void yearForwardClicked();
    void yearBackwardClicked();
    void selectMonthClicked();
    void selectYearClicked();
    void uncheckYearSelector();
    void lineEnterPressed();
    void todayButtonClicked();
    void weekSelected(int);

Q_SIGNALS:
    /**
     * This signal is emitted each time the selected date is changed.
     * Usually, this does not mean that the date has been entered,
     * since the date also changes, for example, when another month is
     * selected.
     * @see dateSelected
     */
    void dateChanged(const QDate &date);

    /**
     * This signal is emitted each time a day has been selected by
     * clicking on the table (hitting a day in the current month). It
     * has the same meaning as dateSelected() in older versions of
     * KDatePicker.
     */
    void dateSelected(const QDate &date);

    /**
     * This signal is emitted when enter is pressed and a VALID date
     * has been entered before into the line edit. Connect to both
     * dateEntered() and dateSelected() to receive all events where the
     * user really enters a date.
     */
    void dateEntered(const QDate &date);

    /**
     * This signal is emitted when the day has been selected by
     * clicking on it in the table.
     */
    void tableClicked();

private:
    KWIDGETSADDONS_NO_EXPORT KDateTable *dateTable() const;
    KWIDGETSADDONS_NO_EXPORT void initWidget(const QDate &date);

private:
    friend class KDatePickerPrivate;
    std::unique_ptr<class KDatePickerPrivate> const d;
};

#endif //  KDATEPICKER_H
