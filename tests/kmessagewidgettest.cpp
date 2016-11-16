/* This file is part of the KDE libraries
 *
 * Copyright 2012 Aurélien Gâteau <agateau@kde.org>
 *
 * Based on test program by Dominik Haumann <dhaumann@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QAction>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QCheckBox>
#include <QWhatsThis>

#include <kmessagewidget.h>

class Receiver : public QObject
{
    Q_OBJECT
public:
    Receiver(QObject *parent) : QObject(parent)
    {}
    virtual ~Receiver() {}

public Q_SLOTS:
    void showWhatsThis(const QString &text)
    {
        QWhatsThis::showText(QCursor::pos(), text);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QWidget *mainWindow = new QWidget();

    QVBoxLayout *l = new QVBoxLayout(mainWindow);

    KMessageWidget *mw = new KMessageWidget(mainWindow);
    mw->setWordWrap(true);
    mw->setText(
        QStringLiteral("Test KMessageWidget is properly sized when <a href=\"this is the contents\">word-wrap</a> is enabled by default.")
    );
    mw->setIcon(QIcon::fromTheme(QStringLiteral("kde")));
    KMessageWidget *mw2 = new KMessageWidget(mainWindow);
    mw2->setWordWrap(true);
    mw2->setText(QStringLiteral("A KMessageWidget with an icon and two additional buttons"));
    mw2->setIcon(QIcon::fromTheme(QStringLiteral("kde")));
    mw2->addAction(new QAction(QStringLiteral("Foo"), mw2));
    mw2->addAction(new QAction(QStringLiteral("Bar"), mw2));
    // A frame to materialize the end of the KMessageWidget
    QFrame *frame = new QFrame(mainWindow);
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QCheckBox *wordWrapCb = new QCheckBox(QStringLiteral("wordWrap"), mainWindow);
    wordWrapCb->setChecked(true);
    QObject::connect(wordWrapCb, &QAbstractButton::toggled, mw, &KMessageWidget::setWordWrap);
    QObject::connect(wordWrapCb, &QAbstractButton::toggled, mw2, &KMessageWidget::setWordWrap);

    QCheckBox *closeButtonCb = new QCheckBox(QStringLiteral("closeButton"), mainWindow);
    closeButtonCb->setChecked(true);
    QObject::connect(closeButtonCb, &QAbstractButton::toggled, mw, &KMessageWidget::setCloseButtonVisible);
    QObject::connect(closeButtonCb, &QAbstractButton::toggled, mw2, &KMessageWidget::setCloseButtonVisible);

    l->addWidget(wordWrapCb);
    l->addWidget(closeButtonCb);
    l->addWidget(mw);
    l->addWidget(mw2);
    l->addWidget(frame);

    mainWindow->resize(400, 300);
    mainWindow->show();

    // demonstrate linkActivated
    Receiver *info = new Receiver(mw);
    QObject::connect(mw, &KMessageWidget::linkActivated, info, &Receiver::showWhatsThis);

    return app.exec();
    delete mainWindow;
}

#include "kmessagewidgettest.moc"

// kate: replace-tabs on;
