/*
    This file is part of the KDE frameworks
    SPDX-FileCopyrightText: 2014 Aurélien Gâteau <agateau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/
#ifndef KCOLUMNRESIZER_H
#define KCOLUMNRESIZER_H

#include <kwidgetsaddons_export.h>

#include <QFormLayout>
#include <QObject>

#include <memory>

class QEvent;
class QGridLayout;
class QLayout;
class QWidget;

/*!
 * \class KColumnResizer
 * \inmodule KWidgetsAddons
 *
 * \brief Maintains consistent column sizes across layouts.
 *
 * KColumnResizer is a helper class which can force columns of different layouts
 * to keep the same width. It is useful to keep label columns consistent.
 *
 * It works with QGridLayout and QFormLayout.
 *
 * \image kcolumnresizer.png "left: without KColumnResizer - right: with KColumnResizer"
 *
 * Here is a typical example:
 *
 * \code
 *  void Window::createWidgets()
 *  {
 *      QVBoxLayout *layout = new QVBoxLayout(this);
 *
 *      QGroupBox *box1 = new QGroupBox();
 *      // Fill box1
 *      // ...
 *      layout->addWidget(box1);
 *
 *      QGroupBox *box2 = new QGroupBox();
 *      // Fill box2
 *      // ...
 *      layout->addWidget(box2);
 *
 *      KColumnResizer *resizer = new KColumnResizer(this);
 *      resizer->addWidgetsFromLayout(box1->layout(), 0);
 *      resizer->addWidgetsFromLayout(box2->layout(), 0);
 *  }
 * \endcode
 *
 * In this example box1 and box2 children can be organized using QGridLayout or
 * QFormLayout, resizer will ensure the first columns of the two QGroupBox stay
 * the same width.
 *
 * \since 5.1
 */
class KWIDGETSADDONS_EXPORT KColumnResizer : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a KColumnResizer.
     */
    explicit KColumnResizer(QObject *parent = nullptr);

    ~KColumnResizer() override;

    /*!
     * Add all widgets from \a layout which are in column \a column to the list
     * of widgets to manage.
     *
     * \a layout The layout containing the widgets to add. KColumnResizer
     * supports QGridLayout and QFormLayout.
     *
     * \a column The column number which contains the widgets. If layout is
     * a QFormLayout, column should not be higher than QFormLayout::SpanningRole
     */
    void addWidgetsFromLayout(QLayout *layout, int column = 0);

    /*!
     * Add a single widget to the list of widgets whose width is monitored.
     *
     * It is more common to use addWidgetsFromLayout(), but adding single
     * widgets can be useful if you want to keep a single button the same width
     * as a column in a layout.
     *
     * \a widget The widget to add
     */
    void addWidget(QWidget *widget);

    /*!
     * Remove a widget previously added by addWidget or addWidgetsFromLayout.
     *
     * \a widget The widget to remove
     */
    void removeWidget(QWidget *widget);

protected:
    bool eventFilter(QObject *, QEvent *event) override;

private:
    std::unique_ptr<class KColumnResizerPrivate> const d;
    Q_DISABLE_COPY(KColumnResizer)
};

#endif /* KCOLUMNRESIZER_H */
