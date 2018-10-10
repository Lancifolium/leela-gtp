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

#ifndef GTPCONFIG_H
#define GTPCONFIG_H

#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>


struct GtpConfigElements {
    // Keep Sgf file
    bool keepSgf;
    QString sgf_path;

    // Weight file path and file name
    QString net_filepath;
    QString net_file;
    QString net_component_filepath;
    QString net_component_file;

    QString extral_lzparam;

    // load training data from directory
    QString training_data_path;

    // Save running games after the timeout (in minutes) is passed and then exit.
    int run_timeout;

    // Exit after the given number of games is completed.
    int run_maxgames;

    // Play more randomly the first x moves (-m =0).
    int random_num;

    // Enable policy network randomization (-n).
    bool enable_noise;

    // Set loop number of visits (-v [3200])
    int loop_visits;

    // Resign when winrate is less than x%. -1 uses 10% but scales for handicap.
    int resignation_percent;

    // Matches
    bool validation;

    // Don's use heuristic for smarter pass (-d)
    bool heuristic;

    // Play 'gpu_names' games on one GPU at the same time.
    int gpu_games;

    // Specify 'leelaz' path
    QString leelaz_path;

    void initial() {
        keepSgf = true;
        sgf_path = "./sgfs/";

        net_filepath = "./networks/weights.txt";
        net_file = "weights.txt";
        net_component_filepath = "./networks/component_weights.txt";
        net_component_file = "component_weights.txt";

        extral_lzparam = "--target-komi 0.5 --tg-auto-pn --neg";

        // Load training data
        training_data_path = "./data/";

        run_timeout = 0;
        run_maxgames = 0;
        random_num = 0;
        enable_noise = true;
        loop_visits = 3300;
        resignation_percent = 1;
        validation = false;

        heuristic = false;
        gpu_games = 1;
        leelaz_path = QString("./leelaz");
    }

    void copy(GtpConfigElements *c) {
        //c->keepSgf = keepSgf;
        //c->sgf_path = sgf_path;

        //c->net_path = net_path;
        //c->net_file = net_file;
        c->net_component_filepath = net_component_filepath;
        c->net_component_file = net_component_file;

        c->extral_lzparam = extral_lzparam;

        c->training_data_path = training_data_path;

        //c->run_timeout = run_timeout;
        //c->run_maxgames = run_maxgames;

        c->random_num = random_num;

        c->enable_noise = enable_noise;

        c->loop_visits = loop_visits;
        c->resignation_percent = resignation_percent;
        c->validation = validation;

        c->heuristic = heuristic;
        //c->gpu_games = gpu_games;
    }
};


class GtpConfig : public QDialog {
    Q_OBJECT
public:
    GtpConfig(QWidget *parent, GtpConfigElements *m_config);
    ~GtpConfig();

private:
    void drawwindow();

private slots:
    //void on_random();
    void on_noise();
    //void on_loopvisits();
    void on_validation();
    void on_compnetfile();
    void on_loadtrainingdata();
    void on_heuristic();
    void on_exlzparam();

    void on_okay();
    void on_cancel();

private:

    // Play more randomly the first x moves. (-m =0)
    QSpinBox *butt_random;

    // Enable policy network randomization
    QCheckBox *butt_enablenoise;

    // Set loop number of visits (-v [3200])
    QSpinBox *butt_loopvisits;

    // Set resign winrate (-r 1)
    QSpinBox *butt_resignpct;

    // Set extral leelaz parameters
    QLineEdit *edit_exlzparam;

    // Matches
    QCheckBox *butt_validation;
    QPushButton *butt_compnetfile;
    QLabel *show_compnetfile;

    // load training data from directory
    QPushButton *butt_trainingdatapath;
    QLabel *show_trainingdatapath;

    // Don's use heuristic for smarter pass (-d)
    QCheckBox *butt_heuristic;


    // OK, Cancel
    QPushButton *butt_okay;
    QPushButton *butt_cancel;

    //
    QLabel *tmplabel1, *tmplabel2, *tmplabel3, *tmplabel4, *tmplabel5;

    struct GtpConfigElements config;
    struct GtpConfigElements *main_config;
};


#endif // GTPCONFIG_H
