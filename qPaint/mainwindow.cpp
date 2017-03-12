#include "mainwindow.h"
#include "paintarea.h"
#include "enum.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent}
{
    initializeUi();
}

void MainWindow::initializeUi()
{
    brush.setStyle(Qt::NoBrush);
    setCentralWidget(centralWidget);
    centralWidgetLayout->setContentsMargins(8,8,8,8);
    setWindowTitle("qPaint 0.1");
    resize(1152, 864);
    
    initializeMenus();
    initializeStatusBar();
    initializeToolBar();
    createNewTabPage(QString());
    initializeOptionsTab();
    initializeInfoBox();

    addToolBar(Qt::TopToolBarArea, toolBar);
    centralWidgetLayout->addWidget(documentTab);
    centralWidgetLayout->addLayout(optionsTabLayout);

    //Konekcije za menije
    connect(action_New, SIGNAL(triggered(bool)), this, SLOT(on_action_newFile_triggered()));
    connect(action_Open, SIGNAL(triggered(bool)), this, SLOT(on_action_openFile_triggered()));
    connect(action_Save, SIGNAL(triggered(bool)), this, SLOT(on_action_save_triggered()));
    connect(action_SaveAs, SIGNAL(triggered(bool)), this, SLOT(on_action_saveFileAs_triggered()));
    connect(action_Exit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(action_Undo, SIGNAL(triggered(bool)), this, SLOT(on_action_Undo_triggered()));
    connect(action_Redo, SIGNAL(triggered(bool)), this, SLOT(on_action_Redo_triggered()));
    connect(action_Copy, SIGNAL(triggered(bool)), this, SLOT(on_action_copy_triggered()));
    connect(action_Paste, SIGNAL(triggered(bool)), this, SLOT(on_action_paste_triggered()));
    connect(action_Delete, SIGNAL(triggered(bool)), this, SLOT(on_action_delete_triggered()));
    connect(action_ToggleAntiAliasing, SIGNAL(triggered(bool)), this, SLOT(on_action_ToggleAntiAliasing_triggered()));
    connect(action_ToggleToolBar, SIGNAL(triggered(bool)), this, SLOT(on_action_ToggleToolBar_triggered()));
    connect(action_ToggleInfoBox, SIGNAL(triggered(bool)), this, SLOT(on_action_ToggleInfoBox_triggered()));
    connect(action_ResizeImage, SIGNAL(triggered(bool)), this, SLOT(on_action_resizeImage_triggered()));
    connect(action_About, SIGNAL(triggered(bool)), this, SLOT(on_action_About_triggered()));
    //Konekcije alatnog doka
    connect(actiongroup_Tools, SIGNAL(triggered(QAction*)), this, SLOT(on_actiongroup_Tools_triggered(QAction*)));
    //Konekcije za postavke olovke i kista
    connect(slider_strokecolor_red, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(slider_strokecolor_green, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(slider_strokecolor_blue, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(slider_fillcolor_red, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(slider_fillcolor_green, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(slider_fillcolor_blue, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(button_strokeColor, SIGNAL(clicked(bool)), colorMapper, SLOT(map()));
    connect(spinbox_strokecolor_red, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(spinbox_strokecolor_green, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(spinbox_strokecolor_blue, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(spinbox_fillcolor_red, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(spinbox_fillcolor_green, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(spinbox_fillcolor_blue, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(button_fillColor, SIGNAL(clicked(bool)), colorMapper, SLOT(map()));
    connect(slider_stroke_opacity, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(slider_fill_opacity, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(slider_stroke_width, SIGNAL(valueChanged(int)), colorMapper, SLOT(map()));
    connect(colorMapper, SIGNAL(mapped(QString)), this, SLOT(on_colorChanged(QString)));
    connect(buttongroup_capstyle, SIGNAL(buttonClicked(int)), this, SLOT(on_buttonGroup_capType_buttonClicked(int)));
    connect(combobox_penstyle, SIGNAL(activated(int)), this, SLOT(on_combobox_penStyle_activated(int)));
    connect(combobox_brushstyle, SIGNAL(activated(int)), this, SLOT(on_combobox_brushStyle_activated(int)));
    //Konekcije za listu oblika
    connect(shapeListView, SIGNAL(shapeChanged()), this, SLOT(update_paintArea()));
    connect(shapeListView->model(), SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(update_paintArea()));
    connect(shapeListView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), getCurrentPaintArea(), SLOT(selectShape(QItemSelection,QItemSelection)));
    //Ostale konekcije
    connect(documentTab, SIGNAL(currentChanged(int)), this, SLOT(on_documentTab_currentChanged(int)));
}
void MainWindow::initializeMenus()
{
    action_New->setStatusTip("Kreiraj novu datoteku");
    action_New->setShortcut(Qt::CTRL+Qt::Key_N);
    //
    action_Open->setStatusTip("Otvori postojeću datoteku");
    action_Open->setShortcut(Qt::CTRL+Qt::Key_O);
    //
    action_Save->setStatusTip("Spremi datoteku na disk");
    action_Save->setShortcut(Qt::CTRL+Qt::Key_S);
    //
    action_SaveAs->setStatusTip("Spremi pod drugim imenom ili formatom");
    action_SaveAs->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_S);
    //
    action_Exit->setStatusTip("Izađi iz programa");
    //
    action_ToggleAntiAliasing->setStatusTip("Ublaži rubove oblika (anti-aliasing)");
    action_ToggleAntiAliasing->setCheckable(true);
    //
    action_ToggleToolBar->setStatusTip("Prikaži ili sakrij alatnu traku");
    action_ToggleToolBar->setCheckable(true);
    action_ToggleToolBar->setChecked(true);
    action_ToggleToolBar->setShortcut(Qt::Key_T);
    //
    action_ToggleInfoBox->setStatusTip("Prikaži ili sakrij informacije o obliku");
    action_ToggleInfoBox->setCheckable(true);
    action_ToggleInfoBox->setChecked(true);
    action_ToggleInfoBox->setShortcut(Qt::Key_I);
    //
    action_Undo->setStatusTip("Poništi posljednju radnju");
    action_Undo->setShortcut(Qt::CTRL + Qt::Key_Z);
    //
    action_Redo->setStatusTip("Ponovi posljednju poništenu radnju");
    action_Redo->setShortcut(Qt::CTRL + Qt::Key_Y);
    //
    action_Copy->setStatusTip("Kopiraj trenutnu sliku");
    action_Copy->setShortcut(Qt::CTRL + Qt::Key_C);
    //
    action_Paste->setStatusTip("Zalijepi sadržaj iz clipboarda");
    action_Paste->setShortcut(Qt::CTRL + Qt::Key_V);
    //
    action_Delete->setStatusTip("Obriši sadržaj slike");
    action_Delete->setShortcut(Qt::Key_Delete);
    //
    action_ResizeImage->setStatusTip("Povećaj ili smanji sliku");
    //
    action_About->setStatusTip("Informacije o programu");
    
    menu_File->addAction(action_New);
    menu_File->addAction(action_Open);
    menu_File->addAction(action_Save);
    menu_File->addAction(action_SaveAs);
    menu_File->addSeparator();
    menu_File->addAction(action_Exit);
    
    menu_Edit->addAction(action_Undo);
    menu_Edit->addAction(action_Redo);
    menu_Edit->addSeparator();
    menu_Edit->addAction(action_Copy);
    menu_Edit->addAction(action_Paste);
    menu_Edit->addAction(action_Delete);
    menu_Edit->addSeparator();
    menu_Edit->addAction(action_ResizeImage);
    
    menu_Options->addAction(action_ToggleAntiAliasing);
    menu_Options->addAction(action_ToggleToolBar);
    menu_Options->addAction(action_ToggleInfoBox);
    
    menu_Help->addAction(action_About);
}
void MainWindow::initializeToolBar()
{
    tool_Select->setObjectName("selectTool");
    tool_Select->setShortcut(Qt::Key_1);
    tool_Select->setCheckable(true);
    tool_Select->setData(0);

    tool_Freehand->setShortcut(Qt::Key_2);
    tool_Freehand->setCheckable(true);
    tool_Freehand->setChecked(true);
    tool_Freehand->setData(1);

    tool_Line->setShortcut(Qt::Key_3);
    tool_Line->setCheckable(true);
    tool_Line->setData(2);

    tool_Rectangle->setShortcut(Qt::Key_4);
    tool_Rectangle->setCheckable(true);
    tool_Rectangle->setData(3);

    tool_Ellipse->setShortcut(Qt::Key_5);
    tool_Ellipse->setCheckable(true);
    tool_Ellipse->setData(4);

    tool_Polygon->setShortcut(Qt::Key_6);
    tool_Polygon->setCheckable(true);
    tool_Polygon->setData(5);

    toolBar->addAction(tool_Select);
    toolBar->addAction(tool_Freehand);
    toolBar->insertSeparator(tool_Freehand);
    toolBar->addAction(tool_Line);
    toolBar->addAction(tool_Rectangle);
    toolBar->addAction(tool_Ellipse);
    toolBar->addAction(tool_Polygon);

    actiongroup_Tools->setExclusive(true);
}
void MainWindow::initializeStatusBar()
{
    setStatusBar(statusBar);
}

void MainWindow::initializeOptionsTab()
{   
    //Ova funkcija izvršava početni postav desnog taba s opcijama
    optionsTabLayout->setContentsMargins(0,0,0,0);
    optionsTabLayout->setSpacing(8);
    optionsTabLayout->addWidget(optionsTab);
    optionsTab->setFixedWidth(200);

    //Stranica: okvir (strokeOptionsTab)//
    optionsTab->setDocumentMode(false);
    optionsTab->addTab(strokeOptionsPage, "Okvir");

    layout_v_tabpage_Stroke->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    QGroupBox* colorGroupBox = new QGroupBox("Boja linije");
    QVBoxLayout* colorGroupVLayout = new QVBoxLayout;
    QGridLayout* colorGroupGridLayout = new QGridLayout;
    colorGroupBox->setLayout(colorGroupVLayout);

    button_strokeColor->setObjectName("button_strokeColor");
    button_strokeColor->setFixedSize(50, 50);
    button_strokeColor->setStyleSheet(QString("background-color: %1").arg(pen.color().name()));
    button_strokeColor->setToolTip("Boja okvira");

    colorGroupVLayout->addWidget(button_strokeColor);
    colorGroupVLayout->addWidget(label_strokecolor, 0, Qt::AlignCenter);
    colorGroupVLayout->setAlignment(button_strokeColor, Qt::AlignCenter);
    colorGroupVLayout->addLayout(colorGroupGridLayout);

    slider_strokecolor_red->setMaximum(255);
    slider_strokecolor_red->setMinimum(0);
    slider_strokecolor_green->setMaximum(255);
    slider_strokecolor_green->setMinimum(0);
    slider_strokecolor_blue->setMaximum(255);
    slider_strokecolor_blue->setMinimum(0);

    spinbox_strokecolor_red->setMaximum(255);
    spinbox_strokecolor_red->setMinimum(0);
    spinbox_strokecolor_green->setMaximum(255);
    spinbox_strokecolor_green->setMinimum(0);
    spinbox_strokecolor_blue->setMaximum(255);
    spinbox_strokecolor_blue->setMinimum(0);

    colorGroupGridLayout->addWidget(new QLabel("R"), 0, 0);
    colorGroupGridLayout->addWidget(slider_strokecolor_red, 0, 1);
    colorGroupGridLayout->addWidget(new QLabel("G"), 1, 0);
    colorGroupGridLayout->addWidget(slider_strokecolor_green, 1, 1);
    colorGroupGridLayout->addWidget(new QLabel("B"), 2, 0);
    colorGroupGridLayout->addWidget(slider_strokecolor_blue, 2, 1);
    colorGroupGridLayout->addWidget(spinbox_strokecolor_red, 0, 2);
    colorGroupGridLayout->addWidget(spinbox_strokecolor_green, 1, 2);
    colorGroupGridLayout->addWidget(spinbox_strokecolor_blue, 2, 2);

    layout_v_tabpage_Stroke->addWidget(colorGroupBox);

    QGroupBox* lineGroupBox = new QGroupBox("Postavke linije");
    QVBoxLayout* lineGroupVLayout = new QVBoxLayout;
    lineGroupBox->setLayout(lineGroupVLayout);
    slider_stroke_width->setMinimum(1);
    slider_stroke_width->setMaximum(50);
    slider_stroke_opacity->setMinimum(0);
    slider_stroke_opacity->setMaximum(255);
    slider_stroke_opacity->setValue(255);
    buttongroup_capstyle->addButton(radiobutton_flatcap, 0x10);
    buttongroup_capstyle->addButton(radiobutton_roundcap, 0x20);
    buttongroup_capstyle->setExclusive(true);
    radiobutton_roundcap->setChecked(true);
    QIcon emptyLine{}, solidLine{}, dashLine{}, dotLine{}, dashDotLine{}, dashDotDotLine{};
    emptyLine.addFile(":/icons/gfx/line_empty.png");
    solidLine.addFile(":/icons/gfx/line_solid.png");
    dashLine.addFile(":/icons/gfx/line_dash.png");
    dotLine.addFile(":/icons/gfx/line_dot.png");
    dashDotLine.addFile(":/icons/gfx/line_dashdot.png");
    dashDotDotLine.addFile(":/icons/gfx/line_dashdotdot.png");

    combobox_penstyle->insertItem(0, emptyLine, "Bez okvira");
    combobox_penstyle->insertItem(1, solidLine, "Puna linija");
    combobox_penstyle->insertItem(2, dashLine, "Isprekidana linija");
    combobox_penstyle->insertItem(3, dotLine, "Točkasta linija");
    combobox_penstyle->insertItem(4, dashDotLine, "Crta-točka");
    combobox_penstyle->insertItem(5, dashDotDotLine, "Crta-točka-točka");
    combobox_penstyle->setCurrentIndex(1);

    lineGroupVLayout->addWidget(slider_stroke_width);
    lineGroupVLayout->addWidget(label_stroke_width);
    lineGroupVLayout->addWidget(slider_stroke_opacity);
    lineGroupVLayout->addWidget(label_stroke_opacity);
    lineGroupVLayout->addSpacing(10);
    lineGroupVLayout->addWidget(new QLabel("Tip završetka linije:"));
    lineGroupVLayout->addWidget(radiobutton_roundcap);
    lineGroupVLayout->addWidget(radiobutton_flatcap);
    lineGroupVLayout->addSpacing(10);
    lineGroupVLayout->addWidget(new QLabel("Tip iscrtavanja linije:"));
    lineGroupVLayout->addWidget(combobox_penstyle);
    lineGroupVLayout->setAlignment(label_stroke_width, Qt::AlignCenter);
    lineGroupVLayout->setAlignment(label_stroke_opacity, Qt::AlignCenter);

    layout_v_tabpage_Stroke->addWidget(lineGroupBox);

    //Stranica: unutrašnjost (fillOptionsTab)//
    optionsTab->addTab(fillOptionsPage, "Unutrašnjost");
    layout_v_tabpage_Fill->setAlignment(Qt::AlignTop | Qt::AlignCenter);

    QGroupBox* fillColorGroupBox{new QGroupBox("Boja unutrašnjosti", fillOptionsPage)};
    QVBoxLayout*  groupBoxLayout{new QVBoxLayout(fillColorGroupBox)};
    button_fillColor->setObjectName("button_fillColor");
    button_fillColor->setFixedSize(50, 50);
    button_fillColor->setToolTip("Boja punjenja");
    groupBoxLayout->addWidget(button_fillColor, 0 ,Qt::AlignCenter);
    groupBoxLayout->addWidget(label_fillcolor, 0, Qt::AlignCenter);

    QGridLayout* layout_fillTab_colorSliders{new QGridLayout};
    slider_fillcolor_red->setMinimum(0);
    slider_fillcolor_red->setMaximum(255);
    slider_fillcolor_green->setMinimum(0);
    slider_fillcolor_green->setMaximum(255);
    slider_fillcolor_blue->setMinimum(0);
    slider_fillcolor_blue->setMaximum(255);
    spinbox_fillcolor_red->setMaximum(255);
    spinbox_fillcolor_red->setMinimum(0);
    spinbox_fillcolor_green->setMaximum(255);
    spinbox_fillcolor_green->setMinimum(0);
    spinbox_fillcolor_blue->setMaximum(255);
    spinbox_fillcolor_blue->setMinimum(0);
    layout_fillTab_colorSliders->addWidget(new QLabel("R"), 0, 0);
    layout_fillTab_colorSliders->addWidget(slider_fillcolor_red, 0, 1);
    layout_fillTab_colorSliders->addWidget(new QLabel("G"), 1, 0);
    layout_fillTab_colorSliders->addWidget(slider_fillcolor_green, 1, 1);
    layout_fillTab_colorSliders->addWidget(new QLabel("B"), 2, 0);
    layout_fillTab_colorSliders->addWidget(slider_fillcolor_blue, 2, 1);
    layout_fillTab_colorSliders->addWidget(spinbox_fillcolor_red, 0, 2);
    layout_fillTab_colorSliders->addWidget(spinbox_fillcolor_green, 1, 2);
    layout_fillTab_colorSliders->addWidget(spinbox_fillcolor_blue, 2, 2);
    layout_v_tabpage_Fill->addWidget(fillColorGroupBox);
    groupBoxLayout->addLayout(layout_fillTab_colorSliders);

    QGroupBox* fillOptionsGroupBox{new QGroupBox("Postavke unutrašnjosti", fillOptionsPage)};
    QVBoxLayout* groupBoxLayout2{new QVBoxLayout(fillOptionsGroupBox)};
    slider_fill_opacity->setMinimum(0);
    slider_fill_opacity->setMaximum(255);
    slider_fill_opacity->setValue(255);
    combobox_brushstyle->insertItem(0, "Bez punjenja");
    combobox_brushstyle->insertItem(1, "Potpuno punjenje");
    combobox_brushstyle->insertItem(2, "Gusti uzorak #1");
    combobox_brushstyle->insertItem(3, "Gusti uzorak #2");
    combobox_brushstyle->insertItem(4, "Gusti uzorak #3");
    combobox_brushstyle->insertItem(5, "Gusti uzorak #4");
    combobox_brushstyle->insertItem(6, "Gusti uzorak #5");
    combobox_brushstyle->insertItem(7, "Gusti uzorak #6");
    combobox_brushstyle->insertItem(8, "Gusti uzorak #7");
    combobox_brushstyle->insertItem(9, "Horizontalne linije");
    combobox_brushstyle->insertItem(10, "Vertikalne linije");
    combobox_brushstyle->insertItem(11, "Mreža");
    combobox_brushstyle->insertItem(12, "Dijagonalne linije #1");
    combobox_brushstyle->insertItem(13, "Dijagonalne linije #2");
    combobox_brushstyle->insertItem(14, "Dijagonalna mreža");
    groupBoxLayout2->addWidget(slider_fill_opacity, 0);
    groupBoxLayout2->addWidget(label_fill_opacity, 0, Qt::AlignCenter);
    groupBoxLayout2->addSpacing(10);
    groupBoxLayout2->addWidget(new QLabel("Tip punjenja:"));
    groupBoxLayout2->addWidget(combobox_brushstyle);

    colorMapper->setMapping(spinbox_strokecolor_red, "SpinStrokeRed");
    colorMapper->setMapping(spinbox_strokecolor_green, "SpinStrokeGreen");
    colorMapper->setMapping(spinbox_strokecolor_blue, "SpinStrokeBlue");
    colorMapper->setMapping(spinbox_fillcolor_red, "SpinFillRed");
    colorMapper->setMapping(spinbox_fillcolor_green, "SpinFillGreen");
    colorMapper->setMapping(spinbox_fillcolor_blue, "SpinFillBlue");
    colorMapper->setMapping(button_strokeColor, "ButtonStroke");
    colorMapper->setMapping(slider_strokecolor_red, "StrokeRed");
    colorMapper->setMapping(slider_strokecolor_blue, "StrokeBlue");
    colorMapper->setMapping(slider_strokecolor_green, "StrokeGreen");
    colorMapper->setMapping(slider_fillcolor_red, "FillRed");
    colorMapper->setMapping(slider_fillcolor_green, "FillGreen");
    colorMapper->setMapping(slider_fillcolor_blue, "FillBlue");
    colorMapper->setMapping(button_fillColor, "ButtonFill");
    colorMapper->setMapping(slider_stroke_opacity, "StrokeOpacity");
    colorMapper->setMapping(slider_fill_opacity, "FillOpacity");
    colorMapper->setMapping(slider_stroke_width, "StrokeWidth");

    layout_v_tabpage_Fill->addWidget(fillOptionsGroupBox);
    optionsTab->setCurrentIndex(0);
    shapeListView->setModel(getCurrentPaintArea()->getShapeListModel());
    shapeListView->setMaximumWidth(200);

    optionsTabLayout->addWidget(shapeListView);
}

void MainWindow::initializeInfoBox()
{
    shapeInfoBox->setFixedWidth(200);
    QVBoxLayout* infoBoxLayout{new QVBoxLayout(shapeInfoBox)};
    label_selectedShape->setMaximumWidth(180);
    infoBoxLayout->addWidget(label_mousecoordinates, 0 , Qt::AlignTop | Qt::AlignLeft);
    infoBoxLayout->addWidget(label_selectedShape);
    infoBoxLayout->addWidget(label_currentSurface);
    infoBoxLayout->addWidget(label_totalSurface);

    optionsTabLayout->addWidget(shapeInfoBox);
}

void MainWindow::createNewTabPage(const QString& fileName)
{   
    /* centralTab
     *  ->newTab
     *   ->newLayout
     *    ->newScrollArea
     *     ->newPaintArea
     */

    QString tabName {};

    if (fileName.isEmpty())
    {
        tabName = "Slika";
    }
    else
    {
        tabName = QString(QFileInfo(fileName).fileName());
    }

    QWidget* newTab {new QWidget};
    QHBoxLayout* newLayout {new QHBoxLayout};
    QScrollArea* newScrollArea {new QScrollArea};
    PaintArea* newPaintArea {new PaintArea(fileName, QSize(800,600), this)};

    newPaintArea->setObjectName("paintArea");
    newScrollArea->setAlignment(Qt::AlignCenter);
    newScrollArea->setWidget(newPaintArea);
    newLayout->addWidget(newScrollArea);
    newTab->setLayout(newLayout);
    documentTab->addTab(newTab, tabName);
    documentTab->setCurrentWidget(newTab);

    connect(this, SIGNAL(drawModeChanged(DrawMode)), newPaintArea, SLOT(setMode(DrawMode)));
    connect(newPaintArea, SIGNAL(filenameChanged(QString)), this, SLOT(on_paintArea_filenameChanged(QString)));
    connect(newPaintArea, SIGNAL(mouseMoved(QPoint)), this, SLOT(update_mousePosition(QPoint)));
    connect(newPaintArea, SIGNAL(selectionChanged(BaseShape*const)), this, SLOT(on_paintArea_selectionChanged(BaseShape*const)));
    connect(newPaintArea, SIGNAL(selectionChanged(BaseShape*const)), shapeListView, SLOT(selectShape(BaseShape*const)));
}

void MainWindow::on_actiongroup_Tools_triggered(QAction* pressedAction)
{
    mode = static_cast<DrawMode>(pressedAction->data().toInt());
    emit drawModeChanged(mode);
}

void MainWindow::update_paintArea()
{
    getCurrentPaintArea()->update();
    updateTotalSurfaceArea();
}

void MainWindow::on_paintArea_selectionChanged(const BaseShape* const shape)
{
    if (shape != nullptr)
    {
        QString name = shape->getName();
        int currentSurface = shape->calculateSurfaceArea();
        label_selectedShape->setText(QString("Odabrani oblik: [%1]").arg(name));
        label_currentSurface->setText(QString("Površina: %1").arg(currentSurface));
        updateTotalSurfaceArea();
    }
    else
    {
        label_selectedShape->setText("Odabrani oblik: [-]");
        label_currentSurface->setText("Površina: N/A");
        updateTotalSurfaceArea();
    }
}

void MainWindow::on_paintArea_filenameChanged(const QString& newName)
{
    documentTab->setTabText(documentTab->currentIndex(), QFileInfo(newName).fileName());
}

PaintArea* MainWindow::getCurrentPaintArea()
{
    PaintArea* currentPaintArea {documentTab->currentWidget()->findChild<PaintArea*>("paintArea")};
    return currentPaintArea;
}

void MainWindow::on_buttonGroup_capType_buttonClicked(int id)
{
    pen.setCapStyle(static_cast<Qt::PenCapStyle>(id));
}

void MainWindow::on_combobox_penStyle_activated(int index)
{
    pen.setStyle(static_cast<Qt::PenStyle>(index));
}

void MainWindow::on_combobox_brushStyle_activated(int index)
{
    brush.setStyle(static_cast<Qt::BrushStyle>(index));
}

void MainWindow::updateLabel_strokecolor()
{
    button_strokeColor->setStyleSheet(QString("background-color: %1").arg(pen.color().name()));
    label_strokecolor->setText(pen.color().name());
}

void MainWindow::updateLabel_fillcolor()
{
    button_fillColor->setStyleSheet(QString("background-color: %1").arg(brush.color().name()));
    label_fillcolor->setText(brush.color().name());
}

void MainWindow::updateTotalSurfaceArea()
{
    int totalSurfaceArea = getCurrentPaintArea()->calculateTotalSurface();
    label_totalSurface->setText(QString("Površina svih oblika: %1").arg(totalSurfaceArea));
}

void MainWindow::on_action_Undo_triggered()
{
    getCurrentPaintArea()->undo();
}
void MainWindow::on_action_Redo_triggered()
{
    getCurrentPaintArea()->redo();
}

void MainWindow::on_action_newFile_triggered()
{
    createNewTabPage(QString());
}

void MainWindow::on_action_openFile_triggered()
{
    QFileDialog openFileDialog{};
    QString filters{"Sve slikovne datoteke (*.bmp *.jpg *.png *.tiff);;"
                    "Bitmap (*.bmp);;"
                    "JPEG (*.jpg);;"
                    "Portable Network Graphics (*.png);;"
                    "Tagged Image File Format (*.tiff)"};
    QString fileName(openFileDialog.getOpenFileName(this, "Odabir datoteke", lastFileOpenPath.absolutePath(), filters));
    if (!fileName.isEmpty())
    {
        for(int i = 0; i < documentTab->count(); i++)
        {
            PaintArea* paintArea{documentTab->widget(i)->findChild<PaintArea*>()};
            const QString& nameToCheck{paintArea->getFileName()};
            if (nameToCheck == fileName)
            {
                documentTab->setCurrentIndex(i);
                statusBar->showMessage("GREŠKA: Datoteka je već otvorena!", 5000);
                return;
            }
        }
        QFileInfo selectedFile{fileName};
        lastFileOpenPath = selectedFile.dir();
        createNewTabPage(fileName);
    }
}

void MainWindow::on_action_save_triggered()
{
    if (getCurrentPaintArea()->getFileName().isEmpty())
    {
        on_action_saveFileAs_triggered();
    }
    else
    {
        getCurrentPaintArea()->saveImage(getCurrentPaintArea()->getFileName());
    }
}

void MainWindow::on_action_saveFileAs_triggered()
{
    QFileDialog saveFileDialog{};
    QString filters{"Bitmap (*.bmp);;" "JPEG (*.jpg);;" "Portable Network Graphics (*.png);;" "Tagged Image File Format (*.tiff)"};
    QString fileName{saveFileDialog.getSaveFileName(this, "Spremanje datoteke", lastFileOpenPath.absolutePath(), filters)};
    if (!fileName.isEmpty())
    {
        PaintArea* const paintArea = getCurrentPaintArea();
        paintArea->saveImage(fileName);
    }
}

void MainWindow::on_action_copy_triggered()
{
    PaintArea* paintArea {getCurrentPaintArea()};
    {
        if (paintArea)
        {
            if (!paintArea->getShapeListModel()->getShapeListConst().isEmpty() && !paintArea->getImage().isNull())
            {
                clipboard->setImage(paintArea->copyImage());
            }
        }
    }
}

void MainWindow::on_action_paste_triggered()
{
    PaintArea* paintArea {getCurrentPaintArea()};
    if (paintArea)
    {
        if (!clipboard->image().isNull())
        {
            paintArea->pasteImage(clipboard->image());
        }
    }
}
void MainWindow::on_action_delete_triggered()
{
    PaintArea* paintArea = getCurrentPaintArea();
    if (paintArea != nullptr)
    {
        paintArea->deleteImage();
    }
}
void MainWindow::on_action_resizeImage_triggered()
{
    PaintArea* currentPaintArea{getCurrentPaintArea()};
    if (currentPaintArea != nullptr) //Obavezno provjeri važi li pokazivač
    {
        ResizeDialog resizeDialog{this, currentPaintArea->size()};
        if (resizeDialog.exec() == 1)
        {
            QSize newSize {resizeDialog.getNewSize()};
            currentPaintArea->resizeImage(newSize);
        }
    }
}

void MainWindow::on_action_ToggleAntiAliasing_triggered()
{
    if (useAntialiasing == true)
    {
        useAntialiasing = false;
    }
    else
    {
        useAntialiasing = true;
    }
    if (getCurrentPaintArea())
    {
        getCurrentPaintArea()->update();
    }
}
void MainWindow::on_action_ToggleToolBar_triggered()
{
    if (toolBar->isVisible())
    {
        toolBar->setHidden(true);
    }
    else
    {
        toolBar->setVisible(true);
    }
}

void MainWindow::on_action_ToggleInfoBox_triggered()
{
    if (shapeInfoBox->isVisible())
    {
        shapeInfoBox->setHidden(true);
    }
    else
    {
        shapeInfoBox->setVisible(true);
    }
}

void MainWindow::on_action_About_triggered()
{
    AboutDialog* aboutDialog = new AboutDialog(this);
    aboutDialog->exec();
}


void MainWindow::on_documentTab_currentChanged(int newIndex)
{
    if (newIndex != -1)
    {
        shapeListView->setModel(getCurrentPaintArea()->getShapeListModel());
        connect(shapeListView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), getCurrentPaintArea(), SLOT(selectShape(QItemSelection,QItemSelection)));

        action_Save->setEnabled(true);
        action_SaveAs->setEnabled(true);
        action_Print->setEnabled(true);
        menu_Edit->setEnabled(true);
    }
    else
    {
        action_Save->setDisabled(true);
        action_SaveAs->setDisabled(true);
        action_Print->setDisabled(true);
        menu_Edit->setDisabled(true);
    }
}

void MainWindow::on_colorChanged(const QString &changed)
{   if (changed == "SpinStrokeRed")
    {
        slider_strokecolor_red->setValue(spinbox_strokecolor_red->value());
    }
    else if (changed == "SpinStrokeGreen")
    {
        slider_strokecolor_green->setValue(spinbox_strokecolor_green->value());
    }
    else if (changed == "SpinStrokeBlue")
    {
        slider_strokecolor_blue->setValue(spinbox_strokecolor_blue->value());
    }
    else if (changed == "StrokeRed")
    {
        QColor currentColor = pen.color();
        currentColor.setRed(slider_strokecolor_red->value());
        pen.setColor(currentColor);
        spinbox_strokecolor_red->setValue(slider_strokecolor_red->value());
    }
    else if (changed == "StrokeGreen")
    {
        QColor currentColor = pen.color();
        currentColor.setGreen(slider_strokecolor_green->value());
        pen.setColor(currentColor);
        spinbox_strokecolor_green->setValue(slider_strokecolor_green->value());
    }
    else if (changed == "StrokeBlue")
    {
        QColor currentColor = pen.color();
        currentColor.setBlue(slider_strokecolor_blue->value());
        pen.setColor(currentColor);
        spinbox_strokecolor_blue->setValue(slider_strokecolor_blue->value());
    }
    else if (changed == "ButtonStroke")
    {
        QColor newColor = QColorDialog::getColor(pen.color(), this, "Odabir boje okvira");
        if (newColor.isValid())
        {
            pen.setColor(newColor);
            slider_strokecolor_red->setValue(newColor.red());
            slider_strokecolor_green->setValue(newColor.green());
            slider_strokecolor_blue->setValue(newColor.blue());
        }
    }
    else if (changed == "SpinFillRed")
    {
        slider_fillcolor_red->setValue(spinbox_fillcolor_red->value());
    }
    else if (changed == "SpinFillGreen")
    {
        slider_fillcolor_green->setValue(spinbox_fillcolor_green->value());
    }
    else if (changed == "SpinFillBlue")
    {
        slider_fillcolor_blue->setValue(spinbox_fillcolor_blue->value());
    }
    else if (changed == "FillRed")
    {
        QColor currentColor = brush.color();
        currentColor.setRed(slider_fillcolor_red->value());
        brush.setColor(currentColor);
        spinbox_fillcolor_red->setValue(slider_fillcolor_red->value());
    }
    else if (changed == "FillGreen")
    {
        QColor currentColor = brush.color();
        currentColor.setGreen(slider_fillcolor_green->value());
        brush.setColor(currentColor);
        spinbox_fillcolor_green->setValue(slider_fillcolor_green->value());
    }
    else if (changed == "FillBlue")
    {
        QColor currentColor = brush.color();
        currentColor.setBlue(slider_fillcolor_blue->value());
        brush.setColor(currentColor);
        spinbox_fillcolor_blue->setValue(slider_fillcolor_blue->value());
    }
    else if (changed == "ButtonFill")
    {
        QColor newColor = QColorDialog::getColor(brush.color(), this, "Odabir boje unutrašnjosti");
        if (newColor.isValid())
        {
            brush.setColor(newColor);
            slider_fillcolor_red->setValue(newColor.red());
            slider_fillcolor_green->setValue(newColor.green());
            slider_fillcolor_blue->setValue(newColor.blue());
        }
    }
    else if (changed == "StrokeOpacity")
    {
        QColor newColor = pen.color();
        newColor.setAlpha(slider_stroke_opacity->value());
        pen.setColor(newColor);
        label_stroke_opacity->setText(QString("Intenzitet kista (%1)").arg(slider_stroke_opacity->value()));
    }
    else if (changed == "FillOpacity")
    {
        QColor newColor = brush.color();
        newColor.setAlpha(slider_fill_opacity->value());
        brush.setColor(newColor);
        label_fill_opacity->setText(QString("Intenzitet punjenja (%1)").arg(slider_fill_opacity->value()));
    }
    else if (changed == "StrokeWidth")
    {
        pen.setWidth(slider_stroke_width->value());
        label_stroke_width->setText(QString("Veličina kista (%1)").arg(slider_stroke_width->value()));
    }
    updateLabel_strokecolor();
    updateLabel_fillcolor();
}

void MainWindow::update_mousePosition(const QPoint &mousePos)
{
    label_mousecoordinates->setText(QString("Položaj pokazivača: (%1 , %2)").arg(mousePos.x()).arg(mousePos.y()));
}

bool MainWindow::getUseAntialiasing() const
{
    return useAntialiasing;
}

const QPen& MainWindow::getPen() const
{
    return pen;
}

const QBrush& MainWindow::getBrush() const
{
    return brush;
}

const DrawMode& MainWindow::getMode() const
{
    return mode;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    PaintArea* currentPaintArea {nullptr};
    QMessageBox saveMessage;
    QAbstractButton* spremi = saveMessage.addButton("Spremi", QMessageBox::AcceptRole);
    QAbstractButton* odbaci = saveMessage.addButton("Odbaci i ugasi", QMessageBox::DestructiveRole);
    QAbstractButton* odustani = saveMessage.addButton("Odustani", QMessageBox::RejectRole);
    int i = 0;

    while (i < documentTab->count())
    {
        currentPaintArea = documentTab->widget(i)->findChild<PaintArea*>("paintArea");

        if (currentPaintArea->getIsModified() == true)
        {
            documentTab->setCurrentIndex(i);

            saveMessage.setText("Imate nespremljene promjene. Želite li ih spremiti prije izlaska iz aplikacije?");
            saveMessage.setIcon(QMessageBox::Question);
            saveMessage.exec();
            if (saveMessage.clickedButton() == spremi)
            {
                on_action_saveFileAs_triggered();
                i = i+1;
            }
            else if (saveMessage.clickedButton() == odbaci)
            {
                documentTab->removeTab(documentTab->currentIndex());
            }
            else
            {
                event->ignore();
                break;
            }
        }
        else
        {
            break;
        }
    }
}

TabWidget::TabWidget(QWidget* parent):
    QTabWidget(parent)
{
    setTabsClosable(true);
    setMovable(true);
    setTabShape(QTabWidget::Rounded);
    tabBar()->setObjectName("centralTabBar");
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(deleteTab(int)));
}


void TabWidget::deleteTab(int index)
{
    PaintArea* paintArea {widget(index)->findChild<PaintArea*>("paintArea")};
    if (paintArea->getIsModified() == true)
    {
        setCurrentIndex(index);
        QMessageBox saveMessage;
        QAbstractButton* spremi = saveMessage.addButton("Spremi", QMessageBox::AcceptRole);
        QAbstractButton* odbaci = saveMessage.addButton("Odbaci i ugasi", QMessageBox::DestructiveRole);
        QAbstractButton* odustani = saveMessage.addButton("Odustani", QMessageBox::RejectRole);
        saveMessage.setText("Imate nespremljene promjene. Želite li ih spremiti prije izlaska iz aplikacije?");
        saveMessage.setIcon(QMessageBox::Question);
        saveMessage.exec();
        if (saveMessage.clickedButton() == spremi)
        {
            QFileDialog saveFileDialog{};
            QString filters{"Bitmap (*.bmp);;" "JPEG (*.jpg);;" "Portable Network Graphics (*.png);;" "Tagged Image File Format (*.tiff)"};
            QString fileName{saveFileDialog.getSaveFileName(this, "Spremanje datoteke", QDir::homePath(), filters)};
            if (!fileName.isEmpty())
            {
                paintArea->saveImage(fileName);
            }
            delete widget(index);
        }
        else if (saveMessage.clickedButton() == odbaci)
        {
            delete widget(index);
        }
        else
        {

        }
    }
    else
    {
        delete widget(index);
    }
}
