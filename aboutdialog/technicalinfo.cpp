/*
 * Copyright (c) LXQt contributors.
 *
 * This file is part of the LXQt project. <http://lxqt.org>
 * It is distributed under the LGPL 2.1 or later license.
 * Please refer to the LICENSE file for a copy of the license, and
 * the AUTHORS file for copyright and authorship information.
 */

#include "technicalinfo.h"
#include <LXQt/Translator>

#include <XdgDirs>

using namespace LXQt;

class TechInfoTable
{
public:
    TechInfoTable(const QString &title);
    void add(const QString &name, const QVariant &value);
    QString html() const;
    QString text(int nameFieldWidth) const;
    int maxNameLength() const;
private:
    QString mTitle;
    QList<QPair<QString,QString> > mRows;
};



TechInfoTable::TechInfoTable(const QString &title)
{
    mTitle = title;
}

void TechInfoTable::add(const QString &name, const QVariant &value)
{
    QPair<QString,QString> row;
    row.first = name;
    row.second = value.toString();
    mRows.append(row);
}

QString TechInfoTable::html() const
{
    QString res;

    res = "<style TYPE='text/css'> "
            ".techInfoKey { white-space: nowrap ; margin: 0 20px 0 16px; } "
          "</style>";

    res += QString("<b>%1</b>").arg(mTitle);
    res += "<table width='100%'>";
    QPair<QString,QString> row;
    foreach(row, mRows)
    {
        res += QString("<tr>"
                       "<td class=techInfoTd width='1%'>"
                            "<div class=techInfoKey>%1</div>"
                        "</td>"
                        "<td>%2</td>"
                       "</tr>").arg(row.first, row.second);
    }

    res += "</table>";
    return res;
}

QString TechInfoTable::text(int nameFieldWidth) const
{
    QString res;
    res += QString("%1\n").arg(mTitle);

    QPair<QString,QString> row;
    foreach(row, mRows)
    {
        res += QString("  %1  %2\n")
                .arg(row.first + ":", -nameFieldWidth)
                .arg(row.second);
    }
    return res;
}

int TechInfoTable::maxNameLength() const
{
    int res = 0;
    QPair<QString,QString> row;
    foreach (row, mRows)
        res = qMax(res, row.first.length());

    return res;
}


QString TechnicalInfo::html() const
{
    QString res;
    foreach (TechInfoTable* item, mItems)
    {
        res += item->html();
        res += "<br><br>";
    }
    return res;
}

QString TechnicalInfo::text() const
{
    int nameWidth = 0;
    foreach (TechInfoTable* item, mItems)
        nameWidth = qMax(nameWidth, item->maxNameLength());

    QString res;
    foreach (TechInfoTable* item, mItems)
    {
        res += item->text(nameWidth + 2);
        res += "\n\n";
    }
    return res;
}

TechInfoTable *TechnicalInfo::newTable(const QString &title)
{
    TechInfoTable *table = new TechInfoTable(title);
    mItems.append(table);
    return table;
}

TechnicalInfo::~TechnicalInfo()
{
    qDeleteAll(mItems);
}


TechnicalInfo::TechnicalInfo()
{
    TechInfoTable *table;

    // ******************************************
    table = newTable("LXQt Desktop Toolbox - Technical Info<p>");
#ifdef DEBUG
    QString buildType("Debug");
#else
    QString buildType("Release");
#endif

    table->add("Version",              LXQT_VERSION);
    table->add("Qt",                   qVersion());
    table->add("Build type",           buildType);
    table->add("System Configuration", LXQT_ETC_XDG_DIR);
    table->add("Share Directory",      LXQT_SHARE_DIR);
    table->add("Translations",         Translator::translationSearchPaths().join("<br>\n"));


    // ******************************************
    table = newTable("User Directories");
    XdgDirs xdgDirs;

    table->add("Xdg Data Home",        xdgDirs.dataHome(false));
    table->add("Xdg Config Home",      xdgDirs.configHome(false));
    table->add("Xdg Data Dirs",        xdgDirs.dataDirs().join(":"));
    table->add("Xdg Cache Home",       xdgDirs.cacheHome(false));
    table->add("Xdg Runtime Home",     xdgDirs.runtimeDir());
    table->add("Xdg Autostart Dirs",   xdgDirs.autostartDirs().join("<br>\n"));
    table->add("Xdg Autostart Home",   xdgDirs.autostartHome(false));

}

