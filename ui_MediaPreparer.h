/********************************************************************************
** Form generated from reading UI file 'MediaPreparer.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDIAPREPARER_H
#define UI_MEDIAPREPARER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MediaPreparer
{
public:
    QAction *actionSave;
    QAction *actionSave_as;
    QAction *actionLoad;
    QAction *actionExit;
    QAction *actionSuperEpicFuntime;
    QAction *actionAbout;
    QAction *actionHelp;
    QWidget *container_main;
    QWidget *layoutWidget;
    QGridLayout *container_directory;
    QSpacerItem *spacer2;
    QLineEdit *setting_directory;
    QPushButton *button_browse_directory;
    QPushButton *button_scan_directory;
    QLabel *label_directory;
    QLabel *label_fileCount;
    QSpacerItem *spacer1;
    QCheckBox *setting_subdirectories;
    QWidget *layoutWidget1;
    QGridLayout *container_settings;
    QComboBox *setting_preset;
    QSpacerItem *spacer3;
    QPushButton *button_savePreset;
    QLabel *label_settings;
    QTabWidget *container_settings_tabs;
    QWidget *container_settings_basic;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_dirOutput;
    QLineEdit *setting_dirOutput;
    QPushButton *button_browse_dirOutput;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label_vCodec;
    QComboBox *setting_vCodec;
    QLabel *label_aCodec;
    QComboBox *setting_aCodec;
    QWidget *formLayoutWidget_2;
    QFormLayout *formLayout_2;
    QSpinBox *setting_aQuality;
    QLabel *label_aQuality;
    QSpinBox *setting_vQuality;
    QLabel *label_vQuality;
    QWidget *formLayoutWidget_4;
    QFormLayout *formLayout_4;
    QLabel *label_container;
    QComboBox *setting_container;
    QLabel *label_subtitles;
    QComboBox *setting_subtitles;
    QWidget *container_settings_advanced;
    QWidget *formLayoutWidget_3;
    QFormLayout *formLayout_3;
    QLabel *label_threads;
    QLineEdit *setting_threads;
    QLabel *label_extraParams;
    QLineEdit *setting_extraParams;
    QWidget *container_fileList;
    QTableWidget *list_VideoLibrary;
    QWidget *container_encode;
    QProgressBar *progress_encode;
    QWidget *formLayoutWidget_5;
    QFormLayout *formLayout_5;
    QLabel *label_encode_file;
    QLineEdit *value_encode_file;
    QWidget *formLayoutWidget_6;
    QFormLayout *formLayout_6;
    QLabel *label_encode_file_vCodec;
    QLineEdit *value_encode_file_vCodec;
    QLineEdit *value_encode_file_aCodec;
    QLineEdit *value_encode_file_container;
    QLabel *label_encode_file_aCodec;
    QLabel *label_encode_file_container;
    QLineEdit *value_encode_file_subtitles;
    QLabel *label_encode_file_subtitles;
    QFrame *line_3;
    QWidget *formLayoutWidget_8;
    QFormLayout *formLayout_8;
    QLabel *label_encode_vQuality;
    QLineEdit *value_encode_vQuality;
    QLineEdit *value_encode_aQuality;
    QLineEdit *value_encode_subtitles;
    QLabel *label_encode_aQuality;
    QLabel *label_encode_subtitle;
    QLabel *label_encode_runtime;
    QLabel *value_encode_runtime;
    QWidget *formLayoutWidget_9;
    QFormLayout *formLayout_9;
    QLabel *label_encode_vCodec;
    QLineEdit *value_encode_vCodec;
    QLineEdit *value_encode_aCodec;
    QLineEdit *value_encode_container;
    QLabel *label_encode_aCodec;
    QLabel *label_encode_container;
    QLabel *labe_encode_lastTime;
    QLabel *value_encode_lastFile;
    QWidget *formLayoutWidget_10;
    QFormLayout *formLayout_10;
    QLabel *label_encode_count;
    QLabel *value_encode_count;
    QWidget *formLayoutWidget_7;
    QFormLayout *formLayout_7;
    QLineEdit *value_encode_file_duration;
    QLabel *label_encode_file_duration;
    QWidget *container_encode_fileList;
    QTableWidget *list_encode_VideoLibrary;
    QWidget *layoutWidget2;
    QGridLayout *container_actions;
    QPushButton *button_encode;
    QProgressBar *progress_worker;
    QFrame *line;
    QFrame *line_2;

    void setupUi(QWidget *MediaPreparer)
    {
        if (MediaPreparer->objectName().isEmpty())
            MediaPreparer->setObjectName(QStringLiteral("MediaPreparer"));
        MediaPreparer->setEnabled(true);
        MediaPreparer->resize(480, 480);
        MediaPreparer->setMinimumSize(QSize(480, 480));
        MediaPreparer->setMaximumSize(QSize(480, 480));
        QIcon icon;
        icon.addFile(QStringLiteral("seflogo.png"), QSize(), QIcon::Normal, QIcon::Off);
        MediaPreparer->setWindowIcon(icon);
        actionSave = new QAction(MediaPreparer);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave_as = new QAction(MediaPreparer);
        actionSave_as->setObjectName(QStringLiteral("actionSave_as"));
        actionLoad = new QAction(MediaPreparer);
        actionLoad->setObjectName(QStringLiteral("actionLoad"));
        actionExit = new QAction(MediaPreparer);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionSuperEpicFuntime = new QAction(MediaPreparer);
        actionSuperEpicFuntime->setObjectName(QStringLiteral("actionSuperEpicFuntime"));
        actionAbout = new QAction(MediaPreparer);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionHelp = new QAction(MediaPreparer);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        container_main = new QWidget(MediaPreparer);
        container_main->setObjectName(QStringLiteral("container_main"));
        container_main->setGeometry(QRect(0, 0, 480, 480));
        layoutWidget = new QWidget(container_main);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 461, 51));
        container_directory = new QGridLayout(layoutWidget);
        container_directory->setSpacing(6);
        container_directory->setContentsMargins(11, 11, 11, 11);
        container_directory->setObjectName(QStringLiteral("container_directory"));
        container_directory->setSizeConstraint(QLayout::SetNoConstraint);
        container_directory->setContentsMargins(0, 0, 0, 0);
        spacer2 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        container_directory->addItem(spacer2, 0, 1, 1, 1);

        setting_directory = new QLineEdit(layoutWidget);
        setting_directory->setObjectName(QStringLiteral("setting_directory"));

        container_directory->addWidget(setting_directory, 2, 0, 1, 5);

        button_browse_directory = new QPushButton(layoutWidget);
        button_browse_directory->setObjectName(QStringLiteral("button_browse_directory"));

        container_directory->addWidget(button_browse_directory, 2, 5, 1, 1);

        button_scan_directory = new QPushButton(layoutWidget);
        button_scan_directory->setObjectName(QStringLiteral("button_scan_directory"));

        container_directory->addWidget(button_scan_directory, 2, 6, 1, 1);

        label_directory = new QLabel(layoutWidget);
        label_directory->setObjectName(QStringLiteral("label_directory"));

        container_directory->addWidget(label_directory, 0, 0, 1, 1);

        label_fileCount = new QLabel(layoutWidget);
        label_fileCount->setObjectName(QStringLiteral("label_fileCount"));

        container_directory->addWidget(label_fileCount, 0, 2, 1, 1);

        spacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        container_directory->addItem(spacer1, 0, 3, 1, 1);

        setting_subdirectories = new QCheckBox(layoutWidget);
        setting_subdirectories->setObjectName(QStringLiteral("setting_subdirectories"));
        setting_subdirectories->setChecked(true);

        container_directory->addWidget(setting_subdirectories, 0, 4, 1, 1);

        layoutWidget1 = new QWidget(container_main);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 80, 461, 331));
        container_settings = new QGridLayout(layoutWidget1);
        container_settings->setSpacing(6);
        container_settings->setContentsMargins(11, 11, 11, 11);
        container_settings->setObjectName(QStringLiteral("container_settings"));
        container_settings->setContentsMargins(0, 0, 0, 0);
        setting_preset = new QComboBox(layoutWidget1);
        setting_preset->setObjectName(QStringLiteral("setting_preset"));

        container_settings->addWidget(setting_preset, 0, 2, 1, 1);

        spacer3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        container_settings->addItem(spacer3, 0, 1, 1, 1);

        button_savePreset = new QPushButton(layoutWidget1);
        button_savePreset->setObjectName(QStringLiteral("button_savePreset"));

        container_settings->addWidget(button_savePreset, 0, 3, 1, 1);

        label_settings = new QLabel(layoutWidget1);
        label_settings->setObjectName(QStringLiteral("label_settings"));

        container_settings->addWidget(label_settings, 0, 0, 1, 1);

        container_settings_tabs = new QTabWidget(layoutWidget1);
        container_settings_tabs->setObjectName(QStringLiteral("container_settings_tabs"));
        container_settings_tabs->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(20);
        sizePolicy.setHeightForWidth(container_settings_tabs->sizePolicy().hasHeightForWidth());
        container_settings_tabs->setSizePolicy(sizePolicy);
        container_settings_basic = new QWidget();
        container_settings_basic->setObjectName(QStringLiteral("container_settings_basic"));
        horizontalLayoutWidget = new QWidget(container_settings_basic);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 100, 432, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_dirOutput = new QLabel(horizontalLayoutWidget);
        label_dirOutput->setObjectName(QStringLiteral("label_dirOutput"));

        horizontalLayout->addWidget(label_dirOutput);

        setting_dirOutput = new QLineEdit(horizontalLayoutWidget);
        setting_dirOutput->setObjectName(QStringLiteral("setting_dirOutput"));

        horizontalLayout->addWidget(setting_dirOutput);

        button_browse_dirOutput = new QPushButton(horizontalLayoutWidget);
        button_browse_dirOutput->setObjectName(QStringLiteral("button_browse_dirOutput"));

        horizontalLayout->addWidget(button_browse_dirOutput);

        formLayoutWidget = new QWidget(container_settings_basic);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 10, 151, 71));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_vCodec = new QLabel(formLayoutWidget);
        label_vCodec->setObjectName(QStringLiteral("label_vCodec"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_vCodec);

        setting_vCodec = new QComboBox(formLayoutWidget);
        setting_vCodec->setObjectName(QStringLiteral("setting_vCodec"));

        formLayout->setWidget(0, QFormLayout::FieldRole, setting_vCodec);

        label_aCodec = new QLabel(formLayoutWidget);
        label_aCodec->setObjectName(QStringLiteral("label_aCodec"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_aCodec);

        setting_aCodec = new QComboBox(formLayoutWidget);
        setting_aCodec->setObjectName(QStringLiteral("setting_aCodec"));

        formLayout->setWidget(1, QFormLayout::FieldRole, setting_aCodec);

        formLayoutWidget_2 = new QWidget(container_settings_basic);
        formLayoutWidget_2->setObjectName(QStringLiteral("formLayoutWidget_2"));
        formLayoutWidget_2->setGeometry(QRect(170, 10, 111, 71));
        formLayout_2 = new QFormLayout(formLayoutWidget_2);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        setting_aQuality = new QSpinBox(formLayoutWidget_2);
        setting_aQuality->setObjectName(QStringLiteral("setting_aQuality"));
        setting_aQuality->setMinimum(20);
        setting_aQuality->setMaximum(1024);
        setting_aQuality->setValue(384);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, setting_aQuality);

        label_aQuality = new QLabel(formLayoutWidget_2);
        label_aQuality->setObjectName(QStringLiteral("label_aQuality"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_aQuality);

        setting_vQuality = new QSpinBox(formLayoutWidget_2);
        setting_vQuality->setObjectName(QStringLiteral("setting_vQuality"));
        setting_vQuality->setMaximum(51);
        setting_vQuality->setValue(24);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, setting_vQuality);

        label_vQuality = new QLabel(formLayoutWidget_2);
        label_vQuality->setObjectName(QStringLiteral("label_vQuality"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_vQuality);

        formLayoutWidget_4 = new QWidget(container_settings_basic);
        formLayoutWidget_4->setObjectName(QStringLiteral("formLayoutWidget_4"));
        formLayoutWidget_4->setGeometry(QRect(290, 10, 151, 71));
        formLayout_4 = new QFormLayout(formLayoutWidget_4);
        formLayout_4->setSpacing(6);
        formLayout_4->setContentsMargins(11, 11, 11, 11);
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        formLayout_4->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_4->setContentsMargins(0, 0, 0, 0);
        label_container = new QLabel(formLayoutWidget_4);
        label_container->setObjectName(QStringLiteral("label_container"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_container);

        setting_container = new QComboBox(formLayoutWidget_4);
        setting_container->setObjectName(QStringLiteral("setting_container"));

        formLayout_4->setWidget(0, QFormLayout::FieldRole, setting_container);

        label_subtitles = new QLabel(formLayoutWidget_4);
        label_subtitles->setObjectName(QStringLiteral("label_subtitles"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_subtitles);

        setting_subtitles = new QComboBox(formLayoutWidget_4);
        setting_subtitles->addItem(QString());
        setting_subtitles->addItem(QString());
        setting_subtitles->addItem(QString());
        setting_subtitles->setObjectName(QStringLiteral("setting_subtitles"));

        formLayout_4->setWidget(1, QFormLayout::FieldRole, setting_subtitles);

        container_settings_tabs->addTab(container_settings_basic, QString());
        container_settings_advanced = new QWidget();
        container_settings_advanced->setObjectName(QStringLiteral("container_settings_advanced"));
        formLayoutWidget_3 = new QWidget(container_settings_advanced);
        formLayoutWidget_3->setObjectName(QStringLiteral("formLayoutWidget_3"));
        formLayoutWidget_3->setGeometry(QRect(10, 10, 431, 71));
        formLayout_3 = new QFormLayout(formLayoutWidget_3);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_3->setContentsMargins(0, 0, 0, 0);
        label_threads = new QLabel(formLayoutWidget_3);
        label_threads->setObjectName(QStringLiteral("label_threads"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_threads);

        setting_threads = new QLineEdit(formLayoutWidget_3);
        setting_threads->setObjectName(QStringLiteral("setting_threads"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, setting_threads);

        label_extraParams = new QLabel(formLayoutWidget_3);
        label_extraParams->setObjectName(QStringLiteral("label_extraParams"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_extraParams);

        setting_extraParams = new QLineEdit(formLayoutWidget_3);
        setting_extraParams->setObjectName(QStringLiteral("setting_extraParams"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, setting_extraParams);

        container_settings_tabs->addTab(container_settings_advanced, QString());
        container_fileList = new QWidget();
        container_fileList->setObjectName(QStringLiteral("container_fileList"));
        list_VideoLibrary = new QTableWidget(container_fileList);
        if (list_VideoLibrary->columnCount() < 5)
            list_VideoLibrary->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        list_VideoLibrary->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        list_VideoLibrary->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        list_VideoLibrary->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        list_VideoLibrary->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        list_VideoLibrary->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        list_VideoLibrary->setObjectName(QStringLiteral("list_VideoLibrary"));
        list_VideoLibrary->setEnabled(true);
        list_VideoLibrary->setGeometry(QRect(0, 0, 455, 276));
        list_VideoLibrary->setFrameShape(QFrame::NoFrame);
        list_VideoLibrary->setFrameShadow(QFrame::Plain);
        list_VideoLibrary->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        list_VideoLibrary->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        list_VideoLibrary->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        list_VideoLibrary->setDragDropOverwriteMode(false);
        list_VideoLibrary->setSelectionBehavior(QAbstractItemView::SelectRows);
        list_VideoLibrary->setSortingEnabled(true);
        list_VideoLibrary->setWordWrap(false);
        list_VideoLibrary->horizontalHeader()->setCascadingSectionResizes(false);
        list_VideoLibrary->horizontalHeader()->setDefaultSectionSize(74);
        list_VideoLibrary->horizontalHeader()->setHighlightSections(false);
        list_VideoLibrary->horizontalHeader()->setMinimumSectionSize(25);
        list_VideoLibrary->verticalHeader()->setVisible(false);
        list_VideoLibrary->verticalHeader()->setDefaultSectionSize(25);
        list_VideoLibrary->verticalHeader()->setHighlightSections(false);
        list_VideoLibrary->verticalHeader()->setMinimumSectionSize(20);
        list_VideoLibrary->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        container_settings_tabs->addTab(container_fileList, QString());
        container_encode = new QWidget();
        container_encode->setObjectName(QStringLiteral("container_encode"));
        container_encode->setEnabled(true);
        progress_encode = new QProgressBar(container_encode);
        progress_encode->setObjectName(QStringLiteral("progress_encode"));
        progress_encode->setEnabled(true);
        progress_encode->setGeometry(QRect(180, 130, 261, 20));
        progress_encode->setMaximum(1000);
        progress_encode->setValue(0);
        progress_encode->setAlignment(Qt::AlignCenter);
        formLayoutWidget_5 = new QWidget(container_encode);
        formLayoutWidget_5->setObjectName(QStringLiteral("formLayoutWidget_5"));
        formLayoutWidget_5->setGeometry(QRect(10, 10, 431, 41));
        formLayout_5 = new QFormLayout(formLayoutWidget_5);
        formLayout_5->setSpacing(6);
        formLayout_5->setContentsMargins(11, 11, 11, 11);
        formLayout_5->setObjectName(QStringLiteral("formLayout_5"));
        formLayout_5->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_5->setContentsMargins(0, 0, 0, 0);
        label_encode_file = new QLabel(formLayoutWidget_5);
        label_encode_file->setObjectName(QStringLiteral("label_encode_file"));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, label_encode_file);

        value_encode_file = new QLineEdit(formLayoutWidget_5);
        value_encode_file->setObjectName(QStringLiteral("value_encode_file"));
        value_encode_file->setEnabled(false);

        formLayout_5->setWidget(0, QFormLayout::FieldRole, value_encode_file);

        formLayoutWidget_6 = new QWidget(container_encode);
        formLayoutWidget_6->setObjectName(QStringLiteral("formLayoutWidget_6"));
        formLayoutWidget_6->setGeometry(QRect(10, 50, 160, 101));
        formLayout_6 = new QFormLayout(formLayoutWidget_6);
        formLayout_6->setSpacing(6);
        formLayout_6->setContentsMargins(11, 11, 11, 11);
        formLayout_6->setObjectName(QStringLiteral("formLayout_6"));
        formLayout_6->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_6->setContentsMargins(0, 0, 0, 0);
        label_encode_file_vCodec = new QLabel(formLayoutWidget_6);
        label_encode_file_vCodec->setObjectName(QStringLiteral("label_encode_file_vCodec"));

        formLayout_6->setWidget(0, QFormLayout::LabelRole, label_encode_file_vCodec);

        value_encode_file_vCodec = new QLineEdit(formLayoutWidget_6);
        value_encode_file_vCodec->setObjectName(QStringLiteral("value_encode_file_vCodec"));
        value_encode_file_vCodec->setEnabled(false);

        formLayout_6->setWidget(0, QFormLayout::FieldRole, value_encode_file_vCodec);

        value_encode_file_aCodec = new QLineEdit(formLayoutWidget_6);
        value_encode_file_aCodec->setObjectName(QStringLiteral("value_encode_file_aCodec"));
        value_encode_file_aCodec->setEnabled(false);

        formLayout_6->setWidget(1, QFormLayout::FieldRole, value_encode_file_aCodec);

        value_encode_file_container = new QLineEdit(formLayoutWidget_6);
        value_encode_file_container->setObjectName(QStringLiteral("value_encode_file_container"));
        value_encode_file_container->setEnabled(false);

        formLayout_6->setWidget(2, QFormLayout::FieldRole, value_encode_file_container);

        label_encode_file_aCodec = new QLabel(formLayoutWidget_6);
        label_encode_file_aCodec->setObjectName(QStringLiteral("label_encode_file_aCodec"));

        formLayout_6->setWidget(1, QFormLayout::LabelRole, label_encode_file_aCodec);

        label_encode_file_container = new QLabel(formLayoutWidget_6);
        label_encode_file_container->setObjectName(QStringLiteral("label_encode_file_container"));

        formLayout_6->setWidget(2, QFormLayout::LabelRole, label_encode_file_container);

        value_encode_file_subtitles = new QLineEdit(formLayoutWidget_6);
        value_encode_file_subtitles->setObjectName(QStringLiteral("value_encode_file_subtitles"));
        value_encode_file_subtitles->setEnabled(false);

        formLayout_6->setWidget(3, QFormLayout::FieldRole, value_encode_file_subtitles);

        label_encode_file_subtitles = new QLabel(formLayoutWidget_6);
        label_encode_file_subtitles->setObjectName(QStringLiteral("label_encode_file_subtitles"));

        formLayout_6->setWidget(3, QFormLayout::LabelRole, label_encode_file_subtitles);

        line_3 = new QFrame(container_encode);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(10, 159, 431, 5));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        formLayoutWidget_8 = new QWidget(container_encode);
        formLayoutWidget_8->setObjectName(QStringLiteral("formLayoutWidget_8"));
        formLayoutWidget_8->setGeometry(QRect(280, 169, 160, 101));
        formLayout_8 = new QFormLayout(formLayoutWidget_8);
        formLayout_8->setSpacing(6);
        formLayout_8->setContentsMargins(11, 11, 11, 11);
        formLayout_8->setObjectName(QStringLiteral("formLayout_8"));
        formLayout_8->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_8->setContentsMargins(0, 0, 0, 0);
        label_encode_vQuality = new QLabel(formLayoutWidget_8);
        label_encode_vQuality->setObjectName(QStringLiteral("label_encode_vQuality"));

        formLayout_8->setWidget(0, QFormLayout::LabelRole, label_encode_vQuality);

        value_encode_vQuality = new QLineEdit(formLayoutWidget_8);
        value_encode_vQuality->setObjectName(QStringLiteral("value_encode_vQuality"));
        value_encode_vQuality->setEnabled(false);

        formLayout_8->setWidget(0, QFormLayout::FieldRole, value_encode_vQuality);

        value_encode_aQuality = new QLineEdit(formLayoutWidget_8);
        value_encode_aQuality->setObjectName(QStringLiteral("value_encode_aQuality"));
        value_encode_aQuality->setEnabled(false);

        formLayout_8->setWidget(1, QFormLayout::FieldRole, value_encode_aQuality);

        value_encode_subtitles = new QLineEdit(formLayoutWidget_8);
        value_encode_subtitles->setObjectName(QStringLiteral("value_encode_subtitles"));
        value_encode_subtitles->setEnabled(false);

        formLayout_8->setWidget(2, QFormLayout::FieldRole, value_encode_subtitles);

        label_encode_aQuality = new QLabel(formLayoutWidget_8);
        label_encode_aQuality->setObjectName(QStringLiteral("label_encode_aQuality"));

        formLayout_8->setWidget(1, QFormLayout::LabelRole, label_encode_aQuality);

        label_encode_subtitle = new QLabel(formLayoutWidget_8);
        label_encode_subtitle->setObjectName(QStringLiteral("label_encode_subtitle"));

        formLayout_8->setWidget(2, QFormLayout::LabelRole, label_encode_subtitle);

        label_encode_runtime = new QLabel(formLayoutWidget_8);
        label_encode_runtime->setObjectName(QStringLiteral("label_encode_runtime"));

        formLayout_8->setWidget(3, QFormLayout::LabelRole, label_encode_runtime);

        value_encode_runtime = new QLabel(formLayoutWidget_8);
        value_encode_runtime->setObjectName(QStringLiteral("value_encode_runtime"));
        value_encode_runtime->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_8->setWidget(3, QFormLayout::FieldRole, value_encode_runtime);

        formLayoutWidget_9 = new QWidget(container_encode);
        formLayoutWidget_9->setObjectName(QStringLiteral("formLayoutWidget_9"));
        formLayoutWidget_9->setGeometry(QRect(10, 169, 160, 101));
        formLayout_9 = new QFormLayout(formLayoutWidget_9);
        formLayout_9->setSpacing(6);
        formLayout_9->setContentsMargins(11, 11, 11, 11);
        formLayout_9->setObjectName(QStringLiteral("formLayout_9"));
        formLayout_9->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_9->setContentsMargins(0, 0, 0, 0);
        label_encode_vCodec = new QLabel(formLayoutWidget_9);
        label_encode_vCodec->setObjectName(QStringLiteral("label_encode_vCodec"));

        formLayout_9->setWidget(0, QFormLayout::LabelRole, label_encode_vCodec);

        value_encode_vCodec = new QLineEdit(formLayoutWidget_9);
        value_encode_vCodec->setObjectName(QStringLiteral("value_encode_vCodec"));
        value_encode_vCodec->setEnabled(false);

        formLayout_9->setWidget(0, QFormLayout::FieldRole, value_encode_vCodec);

        value_encode_aCodec = new QLineEdit(formLayoutWidget_9);
        value_encode_aCodec->setObjectName(QStringLiteral("value_encode_aCodec"));
        value_encode_aCodec->setEnabled(false);

        formLayout_9->setWidget(1, QFormLayout::FieldRole, value_encode_aCodec);

        value_encode_container = new QLineEdit(formLayoutWidget_9);
        value_encode_container->setObjectName(QStringLiteral("value_encode_container"));
        value_encode_container->setEnabled(false);

        formLayout_9->setWidget(2, QFormLayout::FieldRole, value_encode_container);

        label_encode_aCodec = new QLabel(formLayoutWidget_9);
        label_encode_aCodec->setObjectName(QStringLiteral("label_encode_aCodec"));

        formLayout_9->setWidget(1, QFormLayout::LabelRole, label_encode_aCodec);

        label_encode_container = new QLabel(formLayoutWidget_9);
        label_encode_container->setObjectName(QStringLiteral("label_encode_container"));

        formLayout_9->setWidget(2, QFormLayout::LabelRole, label_encode_container);

        labe_encode_lastTime = new QLabel(formLayoutWidget_9);
        labe_encode_lastTime->setObjectName(QStringLiteral("labe_encode_lastTime"));

        formLayout_9->setWidget(3, QFormLayout::LabelRole, labe_encode_lastTime);

        value_encode_lastFile = new QLabel(formLayoutWidget_9);
        value_encode_lastFile->setObjectName(QStringLiteral("value_encode_lastFile"));
        value_encode_lastFile->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_9->setWidget(3, QFormLayout::FieldRole, value_encode_lastFile);

        formLayoutWidget_10 = new QWidget(container_encode);
        formLayoutWidget_10->setObjectName(QStringLiteral("formLayoutWidget_10"));
        formLayoutWidget_10->setGeometry(QRect(180, 169, 91, 101));
        formLayout_10 = new QFormLayout(formLayoutWidget_10);
        formLayout_10->setSpacing(6);
        formLayout_10->setContentsMargins(11, 11, 11, 11);
        formLayout_10->setObjectName(QStringLiteral("formLayout_10"));
        formLayout_10->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_10->setContentsMargins(0, 0, 0, 0);
        label_encode_count = new QLabel(formLayoutWidget_10);
        label_encode_count->setObjectName(QStringLiteral("label_encode_count"));
        label_encode_count->setAlignment(Qt::AlignCenter);

        formLayout_10->setWidget(0, QFormLayout::SpanningRole, label_encode_count);

        value_encode_count = new QLabel(formLayoutWidget_10);
        value_encode_count->setObjectName(QStringLiteral("value_encode_count"));
        value_encode_count->setAlignment(Qt::AlignCenter);

        formLayout_10->setWidget(1, QFormLayout::SpanningRole, value_encode_count);

        formLayoutWidget_7 = new QWidget(container_encode);
        formLayoutWidget_7->setObjectName(QStringLiteral("formLayoutWidget_7"));
        formLayoutWidget_7->setGeometry(QRect(180, 50, 261, 71));
        formLayout_7 = new QFormLayout(formLayoutWidget_7);
        formLayout_7->setSpacing(6);
        formLayout_7->setContentsMargins(11, 11, 11, 11);
        formLayout_7->setObjectName(QStringLiteral("formLayout_7"));
        formLayout_7->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout_7->setContentsMargins(0, 0, 0, 0);
        value_encode_file_duration = new QLineEdit(formLayoutWidget_7);
        value_encode_file_duration->setObjectName(QStringLiteral("value_encode_file_duration"));
        value_encode_file_duration->setEnabled(false);

        formLayout_7->setWidget(0, QFormLayout::FieldRole, value_encode_file_duration);

        label_encode_file_duration = new QLabel(formLayoutWidget_7);
        label_encode_file_duration->setObjectName(QStringLiteral("label_encode_file_duration"));

        formLayout_7->setWidget(0, QFormLayout::LabelRole, label_encode_file_duration);

        container_settings_tabs->addTab(container_encode, QString());
        container_encode_fileList = new QWidget();
        container_encode_fileList->setObjectName(QStringLiteral("container_encode_fileList"));
        list_encode_VideoLibrary = new QTableWidget(container_encode_fileList);
        if (list_encode_VideoLibrary->columnCount() < 5)
            list_encode_VideoLibrary->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        list_encode_VideoLibrary->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        list_encode_VideoLibrary->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        list_encode_VideoLibrary->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        list_encode_VideoLibrary->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        list_encode_VideoLibrary->setHorizontalHeaderItem(4, __qtablewidgetitem9);
        list_encode_VideoLibrary->setObjectName(QStringLiteral("list_encode_VideoLibrary"));
        list_encode_VideoLibrary->setEnabled(true);
        list_encode_VideoLibrary->setGeometry(QRect(0, 0, 455, 276));
        list_encode_VideoLibrary->setFrameShape(QFrame::NoFrame);
        list_encode_VideoLibrary->setFrameShadow(QFrame::Plain);
        list_encode_VideoLibrary->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        list_encode_VideoLibrary->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        list_encode_VideoLibrary->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        list_encode_VideoLibrary->setDragDropOverwriteMode(false);
        list_encode_VideoLibrary->setSelectionMode(QAbstractItemView::ExtendedSelection);
        list_encode_VideoLibrary->setSelectionBehavior(QAbstractItemView::SelectRows);
        list_encode_VideoLibrary->setSortingEnabled(true);
        list_encode_VideoLibrary->setWordWrap(false);
        list_encode_VideoLibrary->horizontalHeader()->setCascadingSectionResizes(false);
        list_encode_VideoLibrary->horizontalHeader()->setDefaultSectionSize(88);
        list_encode_VideoLibrary->horizontalHeader()->setHighlightSections(false);
        list_encode_VideoLibrary->horizontalHeader()->setMinimumSectionSize(25);
        list_encode_VideoLibrary->verticalHeader()->setVisible(false);
        list_encode_VideoLibrary->verticalHeader()->setDefaultSectionSize(25);
        list_encode_VideoLibrary->verticalHeader()->setHighlightSections(false);
        list_encode_VideoLibrary->verticalHeader()->setMinimumSectionSize(20);
        list_encode_VideoLibrary->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        container_settings_tabs->addTab(container_encode_fileList, QString());

        container_settings->addWidget(container_settings_tabs, 1, 0, 1, 4);

        layoutWidget2 = new QWidget(container_main);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 430, 461, 41));
        container_actions = new QGridLayout(layoutWidget2);
        container_actions->setSpacing(6);
        container_actions->setContentsMargins(11, 11, 11, 11);
        container_actions->setObjectName(QStringLiteral("container_actions"));
        container_actions->setContentsMargins(0, 0, 0, 0);
        button_encode = new QPushButton(layoutWidget2);
        button_encode->setObjectName(QStringLiteral("button_encode"));
        button_encode->setEnabled(false);
        button_encode->setMinimumSize(QSize(0, 32));

        container_actions->addWidget(button_encode, 0, 1, 1, 1);

        progress_worker = new QProgressBar(layoutWidget2);
        progress_worker->setObjectName(QStringLiteral("progress_worker"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(progress_worker->sizePolicy().hasHeightForWidth());
        progress_worker->setSizePolicy(sizePolicy1);
        progress_worker->setMinimumSize(QSize(0, 30));
        progress_worker->setMaximum(1000);
        progress_worker->setValue(0);
        progress_worker->setAlignment(Qt::AlignCenter);
        progress_worker->setTextVisible(true);
        progress_worker->setInvertedAppearance(false);
        progress_worker->setTextDirection(QProgressBar::TopToBottom);

        container_actions->addWidget(progress_worker, 0, 0, 1, 1);

        line = new QFrame(container_main);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(10, 69, 461, 5));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(container_main);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(10, 419, 461, 5));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        retranslateUi(MediaPreparer);

        container_settings_tabs->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MediaPreparer);
    } // setupUi

    void retranslateUi(QWidget *MediaPreparer)
    {
        MediaPreparer->setWindowTitle(QApplication::translate("MediaPreparer", "SEF Media Preparer", nullptr));
        actionSave->setText(QApplication::translate("MediaPreparer", "Save", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSave->setShortcut(QApplication::translate("MediaPreparer", "Ctrl+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionSave_as->setText(QApplication::translate("MediaPreparer", "Save as...", nullptr));
        actionLoad->setText(QApplication::translate("MediaPreparer", "Load...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionLoad->setShortcut(QApplication::translate("MediaPreparer", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionExit->setText(QApplication::translate("MediaPreparer", "Exit", nullptr));
        actionSuperEpicFuntime->setText(QApplication::translate("MediaPreparer", "SuperEpicFuntime", nullptr));
        actionAbout->setText(QApplication::translate("MediaPreparer", "About", nullptr));
        actionHelp->setText(QApplication::translate("MediaPreparer", "Help", nullptr));
#ifndef QT_NO_SHORTCUT
        actionHelp->setShortcut(QApplication::translate("MediaPreparer", "F1", nullptr));
#endif // QT_NO_SHORTCUT
        button_browse_directory->setText(QApplication::translate("MediaPreparer", "Browse", nullptr));
        button_scan_directory->setText(QApplication::translate("MediaPreparer", "Scan", nullptr));
        label_directory->setText(QApplication::translate("MediaPreparer", "<html><head/><body><p>Directory:</p></body></html>", nullptr));
        label_fileCount->setText(QApplication::translate("MediaPreparer", "<html><head/><body><p>0 file(s) found</p></body></html>", nullptr));
        setting_subdirectories->setText(QApplication::translate("MediaPreparer", "Subdirectories", nullptr));
        setting_preset->setCurrentText(QString());
        button_savePreset->setText(QApplication::translate("MediaPreparer", "Save", nullptr));
        label_settings->setText(QApplication::translate("MediaPreparer", "<html><head/><body><p>Settings:</p></body></html>", nullptr));
        label_dirOutput->setText(QApplication::translate("MediaPreparer", "Output Directory:", nullptr));
        button_browse_dirOutput->setText(QApplication::translate("MediaPreparer", "Browse", nullptr));
        label_vCodec->setText(QApplication::translate("MediaPreparer", "Video Encoder:", nullptr));
        label_aCodec->setText(QApplication::translate("MediaPreparer", "Audio Encoder:", nullptr));
        setting_aQuality->setSuffix(QApplication::translate("MediaPreparer", "k", nullptr));
        label_aQuality->setText(QApplication::translate("MediaPreparer", "Bitrate:", nullptr));
        label_vQuality->setText(QApplication::translate("MediaPreparer", "CRF:", nullptr));
        label_container->setText(QApplication::translate("MediaPreparer", "File Container:", nullptr));
        label_subtitles->setText(QApplication::translate("MediaPreparer", "Subtitles:", nullptr));
        setting_subtitles->setItemText(0, QApplication::translate("MediaPreparer", "Embed", nullptr));
        setting_subtitles->setItemText(1, QApplication::translate("MediaPreparer", "Ignore", nullptr));
        setting_subtitles->setItemText(2, QApplication::translate("MediaPreparer", "Remove", nullptr));

        container_settings_tabs->setTabText(container_settings_tabs->indexOf(container_settings_basic), QApplication::translate("MediaPreparer", "General", nullptr));
        label_threads->setText(QApplication::translate("MediaPreparer", "Threads:", nullptr));
        setting_threads->setText(QString());
        label_extraParams->setText(QApplication::translate("MediaPreparer", "Extra Parameters:", nullptr));
        container_settings_tabs->setTabText(container_settings_tabs->indexOf(container_settings_advanced), QApplication::translate("MediaPreparer", "Advanced", nullptr));
        QTableWidgetItem *___qtablewidgetitem = list_VideoLibrary->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MediaPreparer", "File", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = list_VideoLibrary->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MediaPreparer", "Video", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = list_VideoLibrary->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MediaPreparer", "Audio", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = list_VideoLibrary->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MediaPreparer", "Container", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = list_VideoLibrary->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("MediaPreparer", "Subtitle", nullptr));
        container_settings_tabs->setTabText(container_settings_tabs->indexOf(container_fileList), QApplication::translate("MediaPreparer", "Files", nullptr));
        progress_encode->setFormat(QString());
        label_encode_file->setText(QApplication::translate("MediaPreparer", "File:", nullptr));
        label_encode_file_vCodec->setText(QApplication::translate("MediaPreparer", "Video Codec:", nullptr));
        label_encode_file_aCodec->setText(QApplication::translate("MediaPreparer", "Audio Codec:", nullptr));
        label_encode_file_container->setText(QApplication::translate("MediaPreparer", "Container:", nullptr));
        label_encode_file_subtitles->setText(QApplication::translate("MediaPreparer", "Subtitles:", nullptr));
        label_encode_vQuality->setText(QApplication::translate("MediaPreparer", "CRF:", nullptr));
        label_encode_aQuality->setText(QApplication::translate("MediaPreparer", "Audio Bitrate:", nullptr));
        label_encode_subtitle->setText(QApplication::translate("MediaPreparer", "Subtitles:", nullptr));
        label_encode_runtime->setText(QApplication::translate("MediaPreparer", "Total Runtime:", nullptr));
        value_encode_runtime->setText(QApplication::translate("MediaPreparer", "00:00:00.00", nullptr));
        label_encode_vCodec->setText(QApplication::translate("MediaPreparer", "Video Encoder:", nullptr));
        label_encode_aCodec->setText(QApplication::translate("MediaPreparer", "Audio Encoder:", nullptr));
        label_encode_container->setText(QApplication::translate("MediaPreparer", "Container:", nullptr));
        labe_encode_lastTime->setText(QApplication::translate("MediaPreparer", "Last File Took:", nullptr));
        value_encode_lastFile->setText(QApplication::translate("MediaPreparer", "00:00:00.00", nullptr));
        label_encode_count->setText(QApplication::translate("MediaPreparer", "Encoding File", nullptr));
        value_encode_count->setText(QApplication::translate("MediaPreparer", "0 / 0", nullptr));
        label_encode_file_duration->setText(QApplication::translate("MediaPreparer", "Duration:", nullptr));
        container_settings_tabs->setTabText(container_settings_tabs->indexOf(container_encode), QApplication::translate("MediaPreparer", "Encode", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = list_encode_VideoLibrary->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QApplication::translate("MediaPreparer", "File", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = list_encode_VideoLibrary->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QApplication::translate("MediaPreparer", "Video Codec", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = list_encode_VideoLibrary->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QApplication::translate("MediaPreparer", "Audio Codec", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = list_encode_VideoLibrary->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QApplication::translate("MediaPreparer", "Container", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = list_encode_VideoLibrary->horizontalHeaderItem(4);
        ___qtablewidgetitem9->setText(QApplication::translate("MediaPreparer", "Subtitle", nullptr));
        container_settings_tabs->setTabText(container_settings_tabs->indexOf(container_encode_fileList), QApplication::translate("MediaPreparer", "Queue", nullptr));
        button_encode->setText(QApplication::translate("MediaPreparer", "Encode [0]", nullptr));
        progress_worker->setFormat(QString());
    } // retranslateUi

};

namespace Ui {
    class MediaPreparer: public Ui_MediaPreparer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDIAPREPARER_H
