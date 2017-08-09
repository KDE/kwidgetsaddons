/* This file is part of the KDE libraries
   Copyright (C) 2000 Ronny Standtke <Ronny.Standtke@gmx.de>

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

#ifndef KSQUEEZEDTEXTLABEL_H
#define KSQUEEZEDTEXTLABEL_H

#include <kwidgetsaddons_export.h>
#include <QLabel>

class KSqueezedTextLabelPrivate;

/**
 * @class KSqueezedTextLabel ksqueezedtextlabel.h KSqueezedTextLabel
 *
 * @short A replacement for QLabel that squeezes its text into the label
 *
 * If the text is too long to fit into the label it is divided into
 * remaining left and right parts which are separated by three dots.
 * Hovering the mouse over the label shows the full text in a tooltip.
 *
 * Example:
 * http://www.kde.org/documentation/index.html could be squeezed to
 * http://www.kde...ion/index.html
 *
 * \image html ksqueezedtextlabel.png "KSqueezedTextLabel Widget"
 *
 * To change the position of the elision mark to the left or right end
 * of the text, use setTextElideMode().
 *
 * @anchor non-virtual-warning
 * @note Several functions of KSqueezedTextLabel (indicated by a warning
 * in their description) reimplement non-virtual functions of QLabel.
 * Therefore, you may need to cast the object to KSqueezedTextLabel in
 * some situations:
 * \Example
 * \code
 * KSqueezedTextLabel* squeezed = new KSqueezedTextLabel("text", parent);
 * QLabel* label = squeezed;
 * label->setText("new text");    // this will not work
 * squeezed->setText("new text"); // works as expected
 * static_cast<KSqueezedTextLabel*>(label)->setText("new text");  // works as expected
 * \endcode
 *
 * @author Ronny Standtke <Ronny.Standtke@gmx.de>
 */

/*
 * QLabel
 */
class KWIDGETSADDONS_EXPORT KSqueezedTextLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(Qt::TextElideMode textElideMode READ textElideMode WRITE setTextElideMode)

public:
    /**
     * Default constructor.
     * @param parent the label's parent object
     */
    explicit KSqueezedTextLabel(QWidget *parent = nullptr);

    /**
     * @param text the text that will be displayed
     * @param parent the label's parent object
     */
    explicit KSqueezedTextLabel(const QString &text, QWidget *parent = nullptr);

    virtual ~KSqueezedTextLabel();

    /**
     * @return the label's minimum size, where the horizontal component
     * will be -1 to indicate the label's ability to shrink its width
     * by squeezing the text
     */
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

    /**
     * @return the label's preferred size, which is wide enough
     * to display the text without squeezing it
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

    /**
     * Overridden for internal reasons; the API remains unaffected.
     */
    virtual void setAlignment(Qt::Alignment);

    /**
     *  @return the text elide mode
     */
    Qt::TextElideMode textElideMode() const;

    /**
     * Sets the text elide mode.
     * @param mode The text elide mode.
     */
    void setTextElideMode(Qt::TextElideMode mode);

    /**
     * @return the full text set via setText()
     *
     * @since 4.4
     */
    QString fullText() const;

    /**
     * @returns true if the text displayed is currently squeezed,
     * i.e. the original text does not fit inside the space available
     * and elide mode is set to a value other than Qt::ElideNone.
     *
     * @since 5.38
     */
    bool isSqueezed() const;

public Q_SLOTS:
    /**
     * Sets the text.
     * @param text The new text.
     *
     * Reimplementation of QLabel::setText().
     *
     * @warning The corresponding function in the base class is not virtual.
     * Therefore make sure to call this function on objects of type KSqueezedTextLabel,
     * as shown in the @ref non-virtual-warning "example in the class description".
     */
    void setText(const QString &text);

    /**
     * Clears the text.
     *
     * Reimplementation of QLabel::clear().
     *
     * @warning The corresponding function in the base class is not virtual.
     * Therefore make sure to call this function on objects of type KSqueezedTextLabel,
     * as shown in the @ref non-virtual-warning "example in the class description".
     */
    void clear();

protected:
    /**
     * \reimp
     */
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;

    /**
     * Called when widget is resized
     */
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

    /**
     * \reimp
     */
    void contextMenuEvent(QContextMenuEvent *) Q_DECL_OVERRIDE;

    /**
     * does the dirty work
     */
    void squeezeTextToLabel();

private:
    Q_PRIVATE_SLOT(d, void _k_copyFullText())
    KSqueezedTextLabelPrivate *const d;
};

#endif // KSQUEEZEDTEXTLABEL_H
