/*  Copyright 2020 Nicolas Fella <nicolas.fella@gmx.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef KRECENTFILESMENU_H
#define KRECENTFILESMENU_H

#include <kwidgetsaddons_export.h>

#include <QMenu>
#include <QUrl>

#include <memory> // for std::unique_ptr

class KRecentFilesMenuPrivate;

/**
 * A menu that offers a set of recent files.
 *
 * @since 5.74
 */
class KWIDGETSADDONS_EXPORT KRecentFilesMenu : public QMenu
{
    Q_OBJECT
public:
    explicit KRecentFilesMenu(const QString &title, QWidget *parent = nullptr);
    explicit KRecentFilesMenu(QWidget *parent = nullptr);
    ~KRecentFilesMenu() override;

    /**
     * The group the URLs are saved to/read from.
     * Unless a group is specified by setGroup "RecentFiles" is used.
     */
    QString group() const;

    /**
     * Specify a group for storing the URLs. This allows e.g. storing multiple
     * types of recent files.
     *
     * By default the group "RecentFiles" is used.
     *
     * @param group the name of the group.
     */
    void setGroup(const QString &group);

    /**
     *  Add URL to recent files list. This will enable this action.
     *
     *  @param url The URL of the file
     *  @param name The user visible pretty name that appears before the URL
     */
    void addUrl(const QUrl &url, const QString &name = QString());

    /**
     *  Remove a URL from the recent files list.
     *
     *  @param url The URL of the file
     */
    void removeUrl(const QUrl &url);

    /**
     * The maximum number of files this menu can hold.
     *
     * When the maximum url count is reached and a new URL is added the
     * oldest will be replaced.
     *
     * By default maximum 10 URLs are shown.
     */
    int maximumItems() const;

    /**
     * Set the maximum URL count.
     *
     * See \ref maximumItems
     */
    void setMaximumItems(size_t maximumItems);

Q_SIGNALS:
    /**
     * emitted when the user clicks on a file action.
     * Usually this should result in the specified URL being opened.
     *
     * @param url The url associated with the triggered action.
     */
    void urlTriggered(const QUrl &url);

private:
    void readFromFile();
    void writeToFile();
    void rebuildMenu();

    std::unique_ptr<KRecentFilesMenuPrivate> const d;
};

#endif
