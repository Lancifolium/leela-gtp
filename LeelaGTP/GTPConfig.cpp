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

#include "GTPConfig.h"
#include <QTime>
#include <QtGlobal>
#include <QTextStream>

GtpConfig::GtpConfig(QWidget *parent, GtpConfigElements *m_config) :
        QDialog (parent),
        main_config(m_config) {
    m_config->copy(&config);

    this->setWindowTitle("Leela GTP further configurations");
    this->setFixedSize(600, 600);

    this->tmplabel1 = new QLabel("前几步更随机：", this);
    this->tmplabel1->setGeometry(60, 66, 150, 24);
    this->butt_random = new QSpinBox(this);
    this->butt_random->setGeometry(60, 90, 84, 24);
    this->butt_random->setRange(0, 800);
    this->butt_random->setValue(config.random_num);
    this->butt_random->setSingleStep(10);
    connect(butt_random, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) { config.random_num = val; });

    this->butt_enablenoise = new QCheckBox("是否允许策略网络随机", this);
    this->butt_enablenoise->setGeometry(60, 120, 150, 24);
    connect(butt_enablenoise, SIGNAL(toggled(bool)), this, SLOT(on_noise()));
    this->butt_enablenoise->setChecked(config.enable_noise);


    this->tmplabel2 = new QLabel("设置循环遍历步数", this);
    this->tmplabel2->setGeometry(60, 156, 90, 24);
    this->butt_loopvisits = new QSpinBox(this);
    this->butt_loopvisits->setGeometry(60, 180, 84, 24);
    this->butt_loopvisits->setRange(100, 100000);
    this->butt_loopvisits->setValue(config.loop_visits);
    this->butt_loopvisits->setSingleStep(100);
    connect(butt_loopvisits, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) { config.loop_visits = val; });

    this->tmplabel3 = new QLabel("投子认输胜率", this);
    this->tmplabel3->setGeometry(60, 216, 90, 24);
    this->tmplabel4 = new QLabel("%", this);
    this->tmplabel4->setGeometry(100, 240, 24, 24);
    this->butt_resignpct = new QSpinBox(this);
    this->butt_resignpct->setGeometry(60, 240, 36, 24);
    this->butt_resignpct->setRange(1, 30);
    this->butt_resignpct->setValue(config.resignation_percent);
    this->butt_resignpct->setSingleStep(1);
    connect(butt_resignpct, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) { config.resignation_percent = val; });

    this->butt_heuristic = new QCheckBox("是否使用启发式智能弃权", this);
    this->butt_heuristic->setGeometry(60, 270, 150, 24);
    this->butt_heuristic->setChecked(config.heuristic);
    connect(butt_heuristic, SIGNAL(toggled(bool)), this, SLOT(on_heuristic()));

    this->tmplabel5 = new QLabel("设置附加 leelaz 选项:", this);
    this->tmplabel5->setGeometry(60, 306, 150, 24);
    this->edit_exlzparam = new QLineEdit(config.extral_lzparam, this);
    this->edit_exlzparam->setGeometry(60, 330, 400, 24);
    connect(edit_exlzparam, SIGNAL(editingFinished()), this, SLOT(on_exlzparam()));

    this->butt_validation = new QCheckBox("是否下权重对抗赛", this);
    this->butt_validation->setGeometry(60, 360, 120, 24);
    connect(butt_validation, SIGNAL(toggled(bool)), this, SLOT(on_validation()));
    this->butt_validation->setChecked(config.validation);
    this->butt_compnetfile = new QPushButton("打开对抗权重文件", this);
    this->butt_compnetfile->setGeometry(60, 390, 100, 24);
    connect(butt_compnetfile, SIGNAL(clicked(bool)), this, SLOT(on_compnetfile()));
    this->show_compnetfile =
        new QLabel("默認文件：" + config.net_component_filepath, this);
    this->show_compnetfile->setGeometry(176, 390, 400, 24);
    on_validation();

    this->butt_trainingdatapath = new QPushButton("打开训练文件目录", this);
    this->butt_trainingdatapath->setGeometry(60, 420, 100, 24);
    connect(butt_trainingdatapath, SIGNAL(clicked(bool)), this, SLOT(on_loadtrainingdata()));
    this->show_trainingdatapath =
            new QLabel("默認目录：" + config.training_data_path, this);
    this->show_trainingdatapath->setGeometry(176, 420, 360, 24);


    this->butt_okay = new QPushButton("确定", this);
    this->butt_okay->setGeometry(60, 510, 84, 24);
    connect(butt_okay, SIGNAL(clicked(bool)), this, SLOT(on_okay()));
    this->butt_cancel = new QPushButton("取消", this);
    this->butt_cancel->setGeometry(150, 510, 84, 24);
    connect(butt_cancel, SIGNAL(clicked(bool)), this, SLOT(on_cancel()));

}

GtpConfig::~GtpConfig() {
    delete butt_random;
    delete butt_enablenoise;
    delete butt_loopvisits;
    delete butt_resignpct;
    delete edit_exlzparam;
    delete butt_validation;
    delete butt_compnetfile;
    delete show_compnetfile;
    delete butt_heuristic;
    delete butt_okay;
    delete butt_cancel;

    delete tmplabel1;
    delete tmplabel2;
    delete tmplabel3;
    delete tmplabel4;
    delete tmplabel5;
}

void GtpConfig::on_noise() {
    if (this->butt_enablenoise->isChecked()) {
        config.enable_noise = true;
    } else {
        config.enable_noise = false;
    }
}

void GtpConfig::on_validation() {
    if (butt_validation->isChecked()) {
        config.validation = true;
        this->butt_compnetfile->setEnabled(true);
        this->show_compnetfile->setEnabled(true);
    } else {
        config.validation = false;
        this->butt_compnetfile->setEnabled(false);
        this->show_compnetfile->setEnabled(false);
    }
}

void GtpConfig::on_compnetfile() {
    config.net_component_filepath = QFileDialog::getOpenFileName(this, tr("权重文件"));
    QFileInfo file(config.net_component_filepath);
    config.net_component_file = file.fileName();
    this->show_compnetfile->setText(config.net_component_filepath);
}

void GtpConfig::on_loadtrainingdata() {
    config.training_data_path = QFileDialog::getExistingDirectory(this, tr("训练文件所在目录"), ".");
    show_trainingdatapath->setText(config.training_data_path);
}

void GtpConfig::on_heuristic() {
    if (this->butt_heuristic->isChecked()) {
        config.heuristic = true;
    } else {
        config.heuristic = false;
    }
}

void GtpConfig::on_exlzparam() {
    //QTextStream(stdout) << "leelaz extra params: " << config.extral_lzparam << "\n";
    config.extral_lzparam = edit_exlzparam->text();
}

void GtpConfig::on_okay() {
    config.copy(main_config);
    this->close();
    delete this;
}

void GtpConfig::on_cancel() {
    this->close();
    delete this;
}
