/*
Copyright (C) 2013  Sadika Sumanapala

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STEGYMAIN_H
#define STEGYMAIN_H

#include <QMainWindow>
#include <QSignalMapper>

namespace Ui {
class StegyMain;
}

class StegyMain : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit StegyMain(QWidget *parent = 0);
    ~StegyMain();

private slots:
    void setPage(QString action);
    void showMessage(QString message);
    void about();
    
private:
    Ui::StegyMain *ui;

    QSignalMapper *signalMapper;
};

#endif // STEGYMAIN_H
