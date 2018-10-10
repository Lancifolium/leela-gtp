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

#include "LeelaGTP.h"

#include <QtCore/QTimer>
#ifdef WIN32
#include <direct.h>
#endif
#include <QPainter>
#include <iostream>
#include <QTextStream>
#include "Game.h"

LeelaGTP::LeelaGTP(QApplication *app, QWidget *parent) :
        QMainWindow(parent),
        draw_imgbord(":/images/bord.png"),
        draw_imgw(":/images/movwhite.png"),
        draw_imgb(":/images/movblack.png"),
        draw_imgcur(":/images/cur_mov.png") {
    this->app = app;
    initial();

    this->setWindowTitle("里拉訓練GTP程序");
    //this->setWindowTitle("Leela AutoGTP (Local Version)");
    this->setFixedSize(900, 720);


    this->butt_run = new QPushButton("開始訓練", this);
    this->butt_run->setGeometry(660, 240, 84, 24);
    connect(butt_run, SIGNAL(clicked(bool)), this, SLOT(on_runbutt()));

    tmplabel1 = new QLabel("訓練時間（分鍾）：", this);
    tmplabel1->setGeometry(660, 306, 120, 24);
    this->butt_timeout = new QSpinBox(this);
    this->butt_timeout->setGeometry(660, 330, 84, 24);
    this->butt_timeout->setRange(0, 1000000);
    this->butt_timeout->setValue(config.run_timeout);
    this->butt_timeout->setSingleStep(10);
    this->butt_timeout->setSpecialValueText(tr("不限"));
    connect(butt_timeout, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) { config.run_timeout = val; });

    tmplabel2 = new QLabel("最多訓練局數：", this);
    tmplabel2->setGeometry(660, 366, 120, 24);
    this->butt_maxgames = new QSpinBox(this);
    this->butt_maxgames->setGeometry(660, 390, 84, 24);
    this->butt_maxgames->setRange(0, 1000000);
    this->butt_maxgames->setValue(config.run_maxgames);
    this->butt_maxgames->setSingleStep(10);
    this->butt_maxgames->setSpecialValueText(tr("不限"));
    connect(butt_maxgames, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) { config.run_maxgames = val; });

    this->tmplabel3 = new QLabel("單GPU上同時下棋局數：", this);
    this->tmplabel3->setGeometry(660, 426, 150, 24);
    this->butt_gpugames = new QSpinBox(this);
    this->butt_gpugames->setGeometry(660, 450, 84, 24);
    this->butt_gpugames->setRange(1, 8);
    this->butt_gpugames->setValue(config.gpu_games);
    this->butt_gpugames->setSingleStep(1);
    //connect(butt_gamesNum, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
    //        [=] (int val) { int_gamesNum = val; });
    connect(butt_gpugames, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) { config.gpu_games = val; });

    this->butt_configs = new QPushButton("更多设置...", this);
    this->butt_configs->setGeometry(660, 516, 84, 24);
    connect(butt_configs, SIGNAL(clicked(bool)), this, SLOT(on_furconfigs()));


    this->butt_zip = new QPushButton("訓練數據另存爲", this);
    this->butt_zip->setGeometry(660, 600, 108, 24);
    connect(butt_zip, SIGNAL(clicked(bool)), this, SLOT(on_zipbutt()));


    this->butt_keepSgf = new QRadioButton("是否保存棋譜文件", this);
    this->butt_keepSgf->setGeometry(60, 600, 600, 24);
    this->butt_keepSgf->setChecked(config.keepSgf);
    connect(butt_keepSgf, SIGNAL(toggled(bool)), this, SLOT(on_keepSgf()));
    this->butt_sgfpath = new QPushButton("打開路徑...", this);
    this->butt_sgfpath->setGeometry(60, 630, 84, 24);
    connect(butt_sgfpath, SIGNAL(clicked(bool)), this, SLOT(on_sgfpathbutt()));
    butt_sgfpath->setEnabled(config.keepSgf);
    this->show_sgfpath = new QLabel(this->config.sgf_path, this);
    this->show_sgfpath->setGeometry(160, 630, 600, 24);
    show_sgfpath->setEnabled(config.keepSgf);
    this->show_sgfpath->setText("默認路徑：" + config.sgf_path);

    this->butt_netfile = new QPushButton("打開自定义权重文件", this);
    this->butt_netfile->setGeometry(60, 660, 120, 24);
    connect(butt_netfile, SIGNAL(clicked(bool)), this, SLOT(on_netfilebutt()));
    this->show_netfile = new QLabel(this->config.net_file, this);
    this->show_netfile->setGeometry(196, 660, 600, 24);
    this->show_netfile->setText("默認文件：" + config.net_filepath);


    this->butt_clear = new QPushButton("淸理數據", this);
    this->butt_clear->setGeometry(660, 630, 84, 24);
    connect(butt_clear, SIGNAL(clicked(bool)), this, SLOT(on_clearbutt()));
}

LeelaGTP::~LeelaGTP() {
    delete butt_keepSgf;
    delete butt_sgfpath;
    delete show_sgfpath;
    delete butt_netfile;
    delete show_netfile;
    delete butt_timeout;
    delete butt_gpugames;
    delete butt_configs;
    delete butt_maxgames;
    delete butt_zip;
    delete butt_clear;
    delete butt_run;

    delete tmplabel1;
    delete tmplabel2;
    delete tmplabel3;
}

void LeelaGTP::initial() {
    config.initial();
    is_running = false;

    win_gap = 30;
    win_size = 600;
    win_xlb = 9;
    win_ylb = 9;

    draw_mov.init();
}

void LeelaGTP::on_keepSgf() {
    if (butt_keepSgf->isChecked()) {
        config.keepSgf = true;
    } else {
        config.keepSgf = false;
    }
    butt_sgfpath->setEnabled(config.keepSgf);
    show_sgfpath->setEnabled(config.keepSgf);
}

void LeelaGTP::on_sgfpathbutt() {
    config.sgf_path = QFileDialog::getExistingDirectory(this, tr("保存路徑"), ".");
    show_sgfpath->setText("另存为：" + config.sgf_path);
}

void LeelaGTP::on_netfilebutt() {
    config.net_filepath = QFileDialog::getOpenFileName(this, tr("权重文件"));
    QFileInfo file(config.net_filepath);
    config.net_file = file.fileName();
    show_netfile->setText(config.net_filepath);
}

void LeelaGTP::on_furconfigs() {
    QDialog *w = new GtpConfig(this, &config);
    w->setModal(true);
    w->show();
}

void LeelaGTP::on_zipbutt() {
#ifdef WIN32
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存訓練數據"),
                                                    ".", tr("压縮文件 (*.zip)"));
#else
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存訓練數據"),
                                                    ".", tr("压縮文件 (*.tar.gz)"));
#endif

#ifdef WIN32
    QString cmd("7z.exe -tZip a " + fileName + " data/ ");
    if (this->config.keepSgf) {
        cmd.append(this->config.sgf_path);
    }
    cmd.append(" -mx3");
    QProcess::execute(cmd);
#else
    QString cmd = "tar";
    cmd.append(" -czf " + fileName + " ./data/");
    QProcess::execute(cmd);
#endif
}

void LeelaGTP::on_clearbutt() {
    QDir dir("./data/");
    if (dir.exists()) {
        dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
        QFileInfoList filelist = dir.entryInfoList();
        foreach (QFileInfo file, filelist) {
            if (file.isFile())
                file.dir().remove(file.fileName());
        }
        QMessageBox::information(this, "淸除数据", "删除成功");
    } else
        QMessageBox::information(this, "淸除数据", "目录不存在");
}

void LeelaGTP::on_runbutt() {
    is_running = !is_running;
    if (is_running) {
        // Now running
        _enable_all_elements(false);
        butt_run->setText("中止訓練");
        _run();
    }
    else {
        // Now exit
        if (boss) {
            //QTextStream(stdout) << "sendquit in on runbutt\n";
            boss->sendQuit();
        }
        else {
            _enable_all_elements(true);
            butt_run->setText("開始訓練");
        }
    }
}

int LeelaGTP::_run() {
    if (config.keepSgf) {
        if (!QDir().mkpath(config.sgf_path)) {
            QMessageBox::information(this, "目錄創建失败", "创建棋譜保存目錄失敗！");
            on_bossexit();
            return EXIT_FAILURE;
        }
    }
    if (!QDir().mkpath("./data/")) {
        QMessageBox::information(this, "目錄创建失敗", "创建訓練數據保存目錄失敗！");
        on_bossexit();
        return EXIT_FAILURE;
    }
    //if (config.run_maxgames == 0)
    //    config.run_maxgames = -1;
    config.run_maxgames -= 1;
    QTextStream(stdout) << config.run_maxgames << " max ganesm\n";
    boss = new Management(1, config.gpu_games, QStringList(), AUTOGTP_VERSION,
                                      config.run_maxgames, false, config.keepSgf ? config.sgf_path : QString(),
#ifdef WIN32
                                      this->app->applicationDirPath(),
#endif
                                      "./data/", &config);

    //QTextStream(stdout) << "before 1st connect\n";
    QTimer *timer = new QTimer();
    Job *job = boss->giveAssignments();
    job->connect_sendmessage(this, SLOT(on_recvmove(int)));
    if (config.run_timeout > 0) {
        //QTextStream(stdout) << "before 2st connect\n";
        connect(timer, &QTimer::timeout, boss, &Management::storeGames);
        connect(timer, &QTimer::timeout, this, &LeelaGTP::on_bossexit);
        timer->start(config.run_timeout * 60000);
    } else {
        //QTextStream(stdout) << "before 3st connect\n";
        connect(boss, &Management::sendQuit, this, &LeelaGTP::on_bossexit);
    }
    return 0;
}

void LeelaGTP::on_bossexit() {
    is_running = false;
    _enable_all_elements(true);
    butt_run->setText("開始訓練");

    if (boss->terminate_leelaz())
        QMessageBox::information(this, "错误，中止训练",
                                 "没有找到leelaz执行文件或权重文件\n请您确认设置是否正确");

    delete boss;

    boss = nullptr;
    draw_mov.init();
    update();
    //QTextStream(stdout) << "finish on_bossexit\n";
}

void LeelaGTP::on_recvmove(int move) {
    int tmpmove;
    if (move == 0)
        return;
    /*
     * 210000: black pass
     * 220000: white pass
     * 310000: black resign
     * 320000: white resign
     *  10000: black move
     *  20000: white move
     */
    int cmd = move / 100000;
    switch (cmd) {
    case 0: // move
        tmpmove = (move % 10000) / 100 * 100 + 18 - (move % 100);
        draw_mov.configDropStone(move / 10000, tmpmove);
        break;
    case 2: // pass
        return;
    case 1: // init
    case 3: // resign
        draw_mov.init();
        QTextStream(stdout) << "SgfMov start a new game\n";
        break;
    default:
        return;
    }
    update();
}

void LeelaGTP::_enable_all_elements(bool cmd) {
    butt_timeout->setEnabled(cmd);
    butt_gpugames->setEnabled(cmd);
    butt_configs->setEnabled(cmd);
    butt_maxgames->setEnabled(cmd);
    butt_zip->setEnabled(cmd);
    butt_clear->setEnabled(cmd);
    butt_keepSgf->setEnabled(cmd);
    butt_netfile->setEnabled(cmd);
    if (config.keepSgf) {
        butt_sgfpath->setEnabled(cmd);
        show_sgfpath->setEnabled(cmd);
    }
}

void LeelaGTP::drawing_board() {
    QPainter pain(this);
    QRect target(win_xlb + win_gap / 2, win_ylb + win_gap / 2,
                 win_gap * 19, win_gap * 19);
    pain.drawImage(target, draw_imgbord);
    pain.setRenderHint(QPainter::Antialiasing, true); // 使得邊緣柔和

    int tmpi, tmpj;
    pain.setPen(Qt::black);
    //pain.setBrush(Qt::blue);
    for (tmpi = 0; tmpi < 19; tmpi++) { // 畫棋盤
        pain.drawLine(win_xlb + win_gap, win_ylb + win_gap + tmpi * win_gap,
                      win_xlb + win_gap + 18 * win_gap, win_ylb + win_gap + tmpi * win_gap);
        pain.drawLine(win_xlb + win_gap + tmpi * win_gap, win_ylb + win_gap,
                      win_xlb + win_gap + tmpi * win_gap, win_ylb + win_gap + 18 * win_gap);
    }
    int dotsize = win_gap / 15;
    for (tmpi = win_gap + 3 * win_gap - dotsize; tmpi < win_size; tmpi += 6 * win_gap) { // 畫星位
        for (tmpj = win_gap + 3 * win_gap - dotsize; tmpj < win_size; tmpj += 6 * win_gap) {
            pain.drawRect(win_xlb + tmpi, win_ylb + tmpj, dotsize * 2, dotsize * 2);
        }
    }

    //pain.setRenderHint(QPainter::Antialiasing, true); // 使得邊緣柔和

    for (tmpi = 0; tmpi < 19; tmpi++) {
        for (tmpj = 0; tmpj < 19; tmpj++) {
            if (draw_mov.ston[tmpi][tmpj] == 1) {
                QRect target(win_xlb + tmpi * win_gap + win_gap * 11 / 20,
                             win_ylb + tmpj * win_gap + win_gap * 11 / 20,
                             win_gap * 9 / 10,
                             win_gap * 9 / 10);
                pain.drawImage(target, draw_imgb);
            }
            else if (draw_mov.ston[tmpi][tmpj] == 2) {
                QRect target(win_xlb + tmpi * win_gap + win_gap * 11 / 20,
                             win_ylb + tmpj * win_gap + win_gap * 11 / 20,
                             win_gap * 9 / 10,
                             win_gap * 9 / 10);
                pain.drawImage(target, draw_imgw);
            }
        }
    } // finished for

    if (draw_mov.currmove >= 0) {
        QRect target(win_xlb + draw_mov.currmove / 100 * win_gap + win_gap,
                     win_ylb + draw_mov.currmove % 100 * win_gap + win_gap,
                     win_gap / 3, win_gap / 3);
        pain.drawImage(target, draw_imgcur);
    }
}

void LeelaGTP::paintEvent(QPaintEvent *) {
    drawing_board();
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    LeelaGTP w(&a);
    w.show();
    return a.exec();
}
