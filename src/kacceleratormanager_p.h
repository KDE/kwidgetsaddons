/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2002 Matthias Hölzer-Klüpfel <mhk@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KACCELERATORMANAGER_PRIVATE_H
#define KACCELERATORMANAGER_PRIVATE_H

#include <QList>
#include <QObject>
#include <QString>

class QStackedWidget;
class QMenu;
class QMenuBar;
class QTabBar;
class QDockWidget;

/*!
 * \internal
 * A string class handling accelerators.
 *
 * This class contains a string and knowledge about accelerators.
 * It keeps a list weights, telling how valuable each character
 * would be as an accelerator.
 */
class KAccelString
{
public:
    KAccelString()
        : m_pureText()
        , m_accel(-1)
        , m_orig_accel(-1)
    {
    }
    explicit KAccelString(const QString &input, int initalWeight = -1);

    void calculateWeights(int initialWeight);

    const QString &pure() const
    {
        return m_pureText;
    }
    QString accelerated() const;

    int accel() const
    {
        return m_accel;
    }
    void setAccel(int accel)
    {
        m_accel = accel;
    }

    int originalAccel() const
    {
        return m_orig_accel;
    }
    QString originalText() const
    {
        return m_origText;
    }

    QChar accelerator() const;

    int maxWeight(int &index, const QString &used) const;

    bool operator==(const KAccelString &c) const
    {
        return m_pureText == c.m_pureText && m_accel == c.m_accel && m_orig_accel == c.m_orig_accel;
    }

private:
    int stripAccelerator(QString &input);

    void dump();

    QString m_pureText;
    QString m_origText;
    int m_accel;
    int m_orig_accel;
    QList<int> m_weight;
};

Q_DECLARE_TYPEINFO(KAccelString, Q_RELOCATABLE_TYPE);

typedef QList<KAccelString> KAccelStringList;

/*!
 * \internal
 * This class encapsulates the algorithm finding the 'best'
 * distribution of accelerators in a hierarchy of widgets.
 */
class KAccelManagerAlgorithm
{
public:
    enum {
        // Default control weight
        DEFAULT_WEIGHT = 50,
        // Additional weight for first character in string
        FIRST_CHARACTER_EXTRA_WEIGHT = 50,
        // Additional weight for the beginning of a word
        WORD_BEGINNING_EXTRA_WEIGHT = 50,
        // Additional weight for the dialog buttons (large, we basically never want these reassigned)
        DIALOG_BUTTON_EXTRA_WEIGHT = 300,
        // Additional weight for a 'wanted' accelerator
        WANTED_ACCEL_EXTRA_WEIGHT = 150,
        // Default weight for an 'action' widget (ie, pushbuttons)
        ACTION_ELEMENT_WEIGHT = 50,
        // Default weight for group boxes (lowest priority)
        GROUP_BOX_WEIGHT = -2000,
        // Default weight for checkable group boxes (low priority)
        CHECKABLE_GROUP_BOX_WEIGHT = 20,
        // Default weight for menu titles
        MENU_TITLE_WEIGHT = 250,
        // Additional weight for KDE standard accelerators
        STANDARD_ACCEL = 300,
    };

    static void findAccelerators(KAccelStringList &result, QString &used);
};

/*!
 * \internal
 * This class manages a popup menu. It will notice if entries have been
 * added or changed, and will recalculate the accelerators accordingly.
 *
 * This is necessary for dynamic menus like for example in kicker.
 */
class KPopupAccelManager : public QObject
{
    Q_OBJECT

public:
    static void manage(QMenu *popup);

protected:
    KPopupAccelManager(QMenu *popup);

private Q_SLOTS:

    void aboutToShow();

private:
    void calculateAccelerators();

    void findMenuEntries(KAccelStringList &list);
    void setMenuEntries(const KAccelStringList &list);

    QMenu *m_popup;
    KAccelStringList m_entries;
    int m_count;
};

class QWidgetStackAccelManager : public QObject
{
    Q_OBJECT

public:
    static void manage(QStackedWidget *popup);

protected:
    QWidgetStackAccelManager(QStackedWidget *popup);

private Q_SLOTS:

    void currentChanged(int child);
    bool eventFilter(QObject *watched, QEvent *e) override;

private:
    void calculateAccelerators();

    QStackedWidget *m_stack;
    KAccelStringList m_entries;
};

/*********************************************************************

 class KAcceleratorManagerPrivate - internal helper class

 This class does all the work to find accelerators for a hierarchy of
 widgets.

 *********************************************************************/

class KAcceleratorManagerPrivate
{
public:
    static void manage(QWidget *widget);
    static bool programmers_mode;
    static void addStandardActionNames(const QStringList &strList);
    static bool standardName(const QString &str);

    static bool checkChange(const KAccelString &as)
    {
        QString t2 = as.accelerated();
        QString t1 = as.originalText();
        if (t1 != t2) {
            if (as.accel() == -1) {
                removed_string += QLatin1String("<tr><td>") + t1.toHtmlEscaped() + QLatin1String("</td></tr>");
            } else if (as.originalAccel() == -1) {
                added_string += QLatin1String("<tr><td>") + t2.toHtmlEscaped() + QLatin1String("</td></tr>");
            } else {
                changed_string += QLatin1String("<tr><td>") + t1.toHtmlEscaped() + QLatin1String("</td>");
                changed_string += QLatin1String("<td>") + t2.toHtmlEscaped() + QLatin1String("</td></tr>");
            }
            return true;
        }
        return false;
    }
    static QString changed_string;
    static QString added_string;
    static QString removed_string;
    static QMap<QWidget *, int> ignored_widgets;
    static QStringList standardNames;

private:
    class Item;

public:
    typedef QList<Item *> ItemList;

private:
    static void traverseChildren(QWidget *widget, Item *item, QString &used);

    static void manageWidget(QWidget *widget, Item *item, QString &used);
    static void manageMenuBar(QMenuBar *mbar, Item *item);
    static void manageTabBar(QTabBar *bar, Item *item);
    static void manageDockWidget(QDockWidget *dock, Item *item);

    static void calculateAccelerators(Item *item, QString &used);

    class Item
    {
    public:
        Item()
            : m_widget(nullptr)
            , m_children(nullptr)
            , m_index(-1)
        {
        }
        ~Item();

        Item(const Item &) = delete;
        Item &operator=(const Item &) = delete;

        void addChild(Item *item);

        QWidget *m_widget;
        KAccelString m_content;
        ItemList *m_children;
        int m_index;
    };
};

#endif // KACCELERATORMANAGER_PRIVATE_H
