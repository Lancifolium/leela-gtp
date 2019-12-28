/*
    This file is part of Leela GTP.
    Copyright (C) 2018 Fierralin
    Copyright (C) 2017-2018 Marco Calignano

    Leela GTP and Leela Zero are free softwares: you can redistribute it
    and/or modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    Leela GTP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Leela GTP.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LEELAGTP_H
#define LEELAGTP_H

#include <QObject>
#include <QMainWindow>
#include <QApplication>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QLineEdit>
#include <QTextBrowser>
#include <QLabel>
#include <QResizeEvent>

#include <QString>

#include "Management.h"
#include "MovLancifolium.h"
#include "GTPConfig.h"

class LeelaGTP : public QMainWindow {
    Q_OBJECT
public:
    LeelaGTP(QApplication *app, QWidget *parent = nullptr);
    ~LeelaGTP();
    void drawwindow();

private:
    void initial();
    void _enable_all_elements(bool cmd);
    int _run();

    void drawing_board();
    void paintEvent(QPaintEvent *);

private slots:
    void on_keepSgf();
    void on_sgfpathbutt();
    void on_netfilebutt();
    void on_furconfigs();
    void on_zipbutt();
    void on_clearbutt();
    void on_runbutt();
    void on_bossexit();
    void on_recvmove(int);

private:
    QApplication *app;

    // Play 'gamesNum' games on one GPU at the same time.
    //QSpinBox *butt_gamesNum;

    // Save SGF files after each self-play game.
    QRadioButton *butt_keepSgf;
    QPushButton *butt_sgfpath;
    QLabel *show_sgfpath;

    // Set self defined network
    QPushButton *butt_netfile;
    QLabel *show_netfile;

    // Save running games after the timeout (in minutes) is passed and then exit.
    QSpinBox *butt_timeout;

    // Play 'gpu_names' games on one GPU at the same time.
    QSpinBox *butt_gpugames;

    // Further configurations
    QPushButton *butt_configs;

    // Exit after the given number of games is completed.
    QSpinBox *butt_maxgames;

    // Compress training data and save as a file
    QPushButton *butt_zip;

    // Clear old training data
    QPushButton *butt_clear;

    // Run
    QPushButton *butt_run;

private:
    GtpConfigElements config;

    QLabel *tmplabel1, *tmplabel2, *tmplabel3;
    bool is_running;
    Management *boss;

    MovLancifolium draw_mov;

    /* 繪圖 */
    QImage draw_imgbord; /* 棋盤 */
    QImage draw_imgw; /* 白子 */
    QImage draw_imgb; /* 黑子 */
    QImage draw_imgcur; /* 當前落子 */

    int win_size; /* 棋盤尺寸 */
    int win_gap; /* = win_size / 20 */
    int win_xlb; /* 棋盤左上角的橫坐標 */
    int win_ylb; /* 棋盤左上角的縱坐標 */
};

#endif // LEELAGTP_H
