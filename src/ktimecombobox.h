/*
    SPDX-FileCopyrightText: 2011 John Layt <john@layt.net>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KTIMECOMBOBOX_H
#define KTIMECOMBOBOX_H

#include <kwidgetsaddons_export.h>

#include <QComboBox>
#include <QLocale>
#include <memory>

/*!
 * \class KTimeComboBox
 * \inmodule KWidgetsAddons
 *
 * \brief A combobox for times.
 */
class KWIDGETSADDONS_EXPORT KTimeComboBox : public QComboBox
{
    Q_OBJECT

    /*!
     * \property KTimeComboBox::time
     */
    Q_PROPERTY(QTime time READ time WRITE setTime NOTIFY timeChanged USER true)

    /*!
     * \property KTimeComboBox::minimumTime
     */
    Q_PROPERTY(QTime minimumTime READ minimumTime WRITE setMinimumTime RESET resetMinimumTime)

    /*!
     * \property KTimeComboBox::maximumTime
     */
    Q_PROPERTY(QTime maximumTime READ maximumTime WRITE setMaximumTime RESET resetMaximumTime)

    /*!
     * \property KTimeComboBox::timeListInterval
     */
    Q_PROPERTY(int timeListInterval READ timeListInterval WRITE setTimeListInterval)

    /*!
     * \property KTimeComboBox::options
     */
    Q_PROPERTY(Options options READ options WRITE setOptions)

public:
    /*!
     * Options provided by the widget
     * \sa options
     * \sa setOptions
     *
     * \value EditTime Allow the user to manually edit the time in the combo line edit
     * \value SelectTime Allow the user to select the time from a drop-down menu
     * \value ForceTime Any set or entered time will be forced to one of the drop-down times
     * \value WarnOnInvalid Show a warning box on focus out if the user enters an invalid time
     */
    enum Option {
        EditTime = 0x0001,
        SelectTime = 0x0002,
        ForceTime = 0x0004,
        WarnOnInvalid = 0x0008,
    };
    Q_DECLARE_FLAGS(Options, Option)
    Q_FLAG(Options)

    /*!
     * Create a new KTimeComboBox widget
     */
    explicit KTimeComboBox(QWidget *parent = nullptr);

    ~KTimeComboBox() override;

    /*!
     * Return the currently selected time
     */
    QTime time() const;

    /*!
     * Return if the current user input is valid
     *
     * If the user input is null then it is not valid
     *
     * \sa isNull()
     */
    bool isValid() const;

    /*!
     * Return if the current user input is null
     *
     * \sa isValid()
     */
    bool isNull() const;

    /*!
     * Return the currently set widget options
     */
    Options options() const;

    /*!
     * Return the currently set time format
     *
     * By default this is the Short Format
     */
    QLocale::FormatType displayFormat() const;

    /*!
     * Return the current minimum time
     */
    QTime minimumTime() const;

    /*!
     * Reset the minimum time to the default of 00:00:00.000
     */
    void resetMinimumTime();

    /*!
     * Return the current maximum time
     */
    QTime maximumTime() const;

    /*!
     * Reset the maximum time to the default of 23:59:59.999
     */
    void resetMaximumTime();

    /*!
     * Set the minimum and maximum time range.
     *
     * If either time is invalid, or min > max then the range will not be set.
     *
     * \a minTime the minimum time
     *
     * \a maxTime the maximum time
     *
     * \a minWarnMsg the minimum warning message
     *
     * \a maxWarnMsg the maximum warning message
     */
    void setTimeRange(const QTime &minTime, const QTime &maxTime, const QString &minWarnMsg = QString(), const QString &maxWarnMsg = QString());

    /*!
     * Reset the minimum and maximum time to the default values.
     */
    void resetTimeRange();

    /*!
     * Return the interval in minutes between select time list entries if set by setTimeListInterval().
     *
     * Returns -1 if not set.
     *
     * \sa setTimeListInterval()
     */
    int timeListInterval() const;

    /*!
     * Return the list of times able to be selected in the drop-down.
     *
     * \sa setTimeList()
     * \sa timeListInterval()
     * \sa setTimeListInterval()
     */
    QList<QTime> timeList() const;

Q_SIGNALS:

    /*!
     * Signal if the time has been manually entered or selected by the user.
     *
     * The returned time may be invalid.
     *
     * \a time the new time
     */
    void timeEntered(const QTime &time);

    /*!
     * Signal if the time has been changed either manually by the user
     * or programmatically.
     *
     * The returned time may be invalid.
     *
     * \a time the new time
     */
    void timeChanged(const QTime &time);

    /*!
     * Signal if the time is being manually edited by the user.
     *
     * The returned time may be invalid.
     *
     * \a time the new time
     */
    void timeEdited(const QTime &time);

public Q_SLOTS:

    /*!
     * Set the currently selected time
     *
     * You can set an invalid time or a time outside the valid range, validity
     * checking is only done via isValid().
     *
     * \a time the new time
     */
    void setTime(const QTime &time);

    /*!
     * Set the new widget options
     *
     * \a options the new widget options
     */
    void setOptions(Options options);

    /*!
     * Sets the time format to display.
     *
     * By default is the Short Format.
     *
     * \a format the time format to use
     */
    void setDisplayFormat(QLocale::FormatType format);

    /*!
     * Set the minimum allowed time.
     *
     * If the time is invalid, or greater than current maximum,
     * then the minimum will not be set.
     *
     * \a minTime the minimum time
     *
     * \a minWarnMsg the minimum warning message
     *
     * \sa minimumTime()
     * \sa maximumTime()
     * \sa setMaximumTime()
     * \sa setTimeRange()
     */
    void setMinimumTime(const QTime &minTime, const QString &minWarnMsg = QString());

    /*!
     * Set the maximum allowed time.
     *
     * If the time is invalid, or less than current minimum,
     * then the maximum will not be set.
     *
     * \a maxTime the maximum time
     *
     * \a maxWarnMsg the maximum warning message
     *
     * \sa minimumTime()
     * \sa maximumTime()
     * \sa setMaximumTime()
     * \sa setTimeRange()
     */
    void setMaximumTime(const QTime &maxTime, const QString &maxWarnMsg = QString());

    /*!
     * Set the interval between times able to be selected from the drop-down.
     *
     * The combo drop-down will be populated with times every \a minutes
     * apart, starting from the minimumTime() and ending at maximumTime().
     *
     * If the ForceInterval option is set then any time manually typed into the
     * combo line edit will be forced to the nearest interval.
     *
     * This interval must be an exact divisor of the valid time range hours.
     * For example with the default 24 hour range \a interval must divide 1440
     * minutes exactly, meaning 1, 6 and 90 are valid but 7, 31 and 91 are not.
     *
     * Setting the time list interval will override any time list previously set
     * via setTimeList().
     *
     * \a minutes the time list interval to display
     *
     * \sa timeListInterval()
     */
    void setTimeListInterval(int minutes);

    /*!
     * Set the list of times able to be selected from the drop-down.
     *
     * Setting the time list will override any time interval previously set via
     * setTimeListInterval().
     *
     * Any invalid or duplicate times will be ignored, and the list will be
     * sorted.
     *
     * The minimum and maximum time will automatically be set to the earliest
     * and latest value in the list.
     *
     * \a timeList the list of times able to be selected
     *
     * \a minWarnMsg the minimum warning message
     *
     * \a maxWarnMsg the maximum warning message
     *
     * \sa timeList()
     */
    void setTimeList(QList<QTime> timeList, const QString &minWarnMsg = QString(), const QString &maxWarnMsg = QString());

protected:
    bool eventFilter(QObject *object, QEvent *event) override;
    void showPopup() override;
    void hidePopup() override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    /*!
     * Assign the time for the widget.
     *
     * Virtual to allow sub-classes to apply extra validation rules.
     *
     * \a time the new time
     */
    virtual void assignTime(const QTime &time);

private:
    friend class KTimeComboBoxPrivate;
    std::unique_ptr<class KTimeComboBoxPrivate> const d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(KTimeComboBox::Options)

#endif // KTIMECOMBOBOX_H
