#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QClipboard>
#include <QStatusBar>
#include <QScrollArea>
#include <QFileDialog>
#include <QMenuBar>
#include <QButtonGroup>
#include <QRadioButton>
#include <QComboBox>
#include <QGroupBox>
#include <QPen>
#include <QDir>
#include <QToolBar>
#include <QColorDialog>
#include <QMessageBox>
#include "shapelistview.h"
#include "paintarea.h"
#include "resizedialog.h"
#include "aboutdialog.h"

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget* parent = 0);
protected:
signals:
public slots:
    void deleteTab(int index);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit        MainWindow(QWidget *parent = 0);
    bool            getUseAntialiasing() const;
    const QPen&     getPen() const;
    const QBrush&   getBrush() const;
    const DrawMode& getMode() const;
private:
    QWidget*        centralWidget                       {new QWidget(this)};
    QHBoxLayout*    centralWidgetLayout                 {new QHBoxLayout(centralWidget)};
    TabWidget*      documentTab                         {new TabWidget(centralWidget)};
    QStatusBar*     statusBar                           {new QStatusBar(this)};
    QClipboard*     clipboard                           {QApplication::clipboard()};

    QAction*        action_New                          {new QAction("Nova datoteka", this)};
    QAction*        action_Open                         {new QAction("Otvori datoteku...", this)};
    QAction*        action_Save                         {new QAction("Spremi datoteku", this)};
    QAction*        action_SaveAs                       {new QAction("Spremi kao...", this)};
    QAction*        action_Print                        {new QAction("Ispiši", this)};
    QAction*        action_Exit                         {new QAction("Izlaz", this)};
    QAction*        action_Undo                         {new QAction("Korak natrag", this)};
    QAction*        action_Redo                         {new QAction("Korak naprijed", this)};
    QAction*        action_Copy                         {new QAction("Kopiraj", this)};
    QAction*        action_Paste                        {new QAction("Zalijepi", this)};
    QAction*        action_Delete                       {new QAction("Obriši sliku", this)};
    QAction*        action_ResizeImage                  {new QAction("Promjeni veličinu slike", this)};
    QAction*        action_About                        {new QAction("O programu...", this)};
    QAction*        action_ToggleAntiAliasing           {new QAction("Uključi ublažavanje rubova", this)};
    QAction*        action_ToggleToolBar                {new QAction("Prikaži alatnu traku", this)};
    QAction*        action_ToggleInfoBox                {new QAction("Prikaži informacije o objektu", this)};

    QMenu*          menu_File                           {menuBar()->addMenu("Datoteka")};
    QMenu*          menu_Edit                           {menuBar()->addMenu("Uređivanje")};
    QMenu*          menu_Options                        {menuBar()->addMenu("Opcije")};
    QMenu*          menu_Help                           {menuBar()->addMenu("Pomoć")};

    QToolBar*       toolBar                             {new QToolBar("Alatna traka", this)};
    QActionGroup*   actiongroup_Tools                   {new QActionGroup(toolBar)};
    QAction*        tool_Select                         {new QAction(QIcon(":/icons/gfx/icons/selectTool.png"), "Alat za odabir", actiongroup_Tools)};
    QAction*        tool_Freehand                       {new QAction(QIcon(":/icons/gfx/icons/freehandTool.png"), "Crtanje slobodnih linija", actiongroup_Tools)};
    QAction*        tool_Line                           {new QAction(QIcon(":/icons/gfx/icons/lineTool.png"), "Crtanje pravaca", actiongroup_Tools)};
    QAction*        tool_Rectangle                      {new QAction(QIcon(":/icons/gfx/icons/rectangleTool.png"), "Crtanje pravokutnika", actiongroup_Tools)};
    QAction*        tool_Ellipse                        {new QAction(QIcon(":/icons/gfx/icons/ellipseTool.png"), "Crtanje kružnica", actiongroup_Tools)};
    QAction*        tool_Polygon                        {new QAction(QIcon(":/icons/gfx/icons/polygonTool.png"), "Crtanje poligona", actiongroup_Tools)};

    QTabWidget*     optionsTab                          {new QTabWidget(this)};
    QVBoxLayout*    optionsTabLayout                    {new QVBoxLayout};

    QWidget*        strokeOptionsPage                   {new QWidget(optionsTab)};
    QVBoxLayout*    layout_v_tabpage_Stroke             {new QVBoxLayout(strokeOptionsPage)};
    QPushButton*    button_strokeColor                  {new QPushButton(strokeOptionsPage)};
    QLabel*         label_strokecolor                   {new QLabel("#000000", strokeOptionsPage)};
    QSlider*        slider_strokecolor_red              {new QSlider(Qt::Horizontal, strokeOptionsPage)};
    QSlider*        slider_strokecolor_green            {new QSlider(Qt::Horizontal, strokeOptionsPage)};
    QSlider*        slider_strokecolor_blue             {new QSlider(Qt::Horizontal, strokeOptionsPage)};
    QSpinBox*       spinbox_strokecolor_red             {new QSpinBox(strokeOptionsPage)};
    QSpinBox*       spinbox_strokecolor_green           {new QSpinBox(strokeOptionsPage)};
    QSpinBox*       spinbox_strokecolor_blue            {new QSpinBox(strokeOptionsPage)};
    QSlider*        slider_stroke_width                 {new QSlider(Qt::Horizontal, strokeOptionsPage)};
    QLabel*         label_stroke_width                  {new QLabel("Veličina kista (1)", strokeOptionsPage)};
    QSlider*        slider_stroke_opacity               {new QSlider(Qt::Horizontal, strokeOptionsPage)};
    QLabel*         label_stroke_opacity                {new QLabel("Intenzitet kista (255)", strokeOptionsPage)};
    QButtonGroup*   buttongroup_capstyle                {new QButtonGroup(strokeOptionsPage)};
    QRadioButton*   radiobutton_roundcap                {new QRadioButton("Zaobljeni tip", strokeOptionsPage)};
    QRadioButton*   radiobutton_flatcap                 {new QRadioButton("Pravokutni tip", strokeOptionsPage)};
    QComboBox*      combobox_penstyle                   {new QComboBox(strokeOptionsPage)};

    QWidget*        fillOptionsPage                     {new QWidget(optionsTab)};
    QVBoxLayout*    layout_v_tabpage_Fill               {new QVBoxLayout(fillOptionsPage)};
    QPushButton*    button_fillColor                    {new QPushButton(fillOptionsPage)};
    QLabel*         label_fillcolor                     {new QLabel("#000000", fillOptionsPage)};
    QSlider*        slider_fillcolor_red                {new QSlider(Qt::Horizontal, fillOptionsPage)};
    QSlider*        slider_fillcolor_green              {new QSlider(Qt::Horizontal, fillOptionsPage)};
    QSlider*        slider_fillcolor_blue               {new QSlider(Qt::Horizontal, fillOptionsPage)};
    QSpinBox*       spinbox_fillcolor_red               {new QSpinBox(fillOptionsPage)};
    QSpinBox*       spinbox_fillcolor_green             {new QSpinBox(fillOptionsPage)};
    QSpinBox*       spinbox_fillcolor_blue              {new QSpinBox(fillOptionsPage)};
    QSlider*        slider_fill_opacity                 {new QSlider(Qt::Horizontal, fillOptionsPage)};
    QLabel*         label_fill_opacity                  {new QLabel("Intenzitet punjenja (255)", fillOptionsPage)};
    QComboBox*      combobox_brushstyle                 {new QComboBox(fillOptionsPage)};

    ShapeListView*  shapeListView                       {new ShapeListView(this)};
    QGroupBox*      shapeInfoBox               {new QGroupBox("Info", this)};
    QLabel*         label_mousecoordinates              {new QLabel("Položaj pokazivača: (0 , 0)", strokeOptionsPage)};
    QLabel*         label_selectedShape                 {new QLabel("Odabrani oblik: []")};
    QLabel*         label_currentSurface                {new QLabel("Površina: N/A")};
    QLabel*         label_totalSurface                  {new QLabel("Ukupna površina: 0")};

    QSignalMapper*  colorMapper                         {new QSignalMapper(this)};
    bool            useAntialiasing                     {false};
    DrawMode        mode                                {Type_Freehand};
    QBrush          brush                               {Qt::black, Qt::SolidPattern};
    QPen            pen                                 {brush, 1.0, Qt::SolidLine, Qt::RoundCap};
    QDir            lastFileOpenPath                    {QDir::homePath()};

    void            closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void            initializeUi();
    void            initializeMenus();
    void            initializeToolBar();
    void            initializeStatusBar();
    void            initializeOptionsTab();
    void            initializeInfoBox();
    void            createNewTabPage(const QString& fileName);
    PaintArea*      getCurrentPaintArea();
    void            updateLabel_strokecolor();
    void            updateLabel_fillcolor();
    void            updateTotalSurfaceArea();
signals:
    void            drawModeChanged(const DrawMode&);
private slots:
    void            on_action_Undo_triggered();
    void            on_action_Redo_triggered();
    void            on_action_newFile_triggered();
    void            on_action_openFile_triggered();
    void            on_action_save_triggered();
    void            on_action_saveFileAs_triggered();
    void            on_action_copy_triggered();
    void            on_action_paste_triggered();
    void            on_action_delete_triggered();
    void            on_action_resizeImage_triggered();
    void            on_action_ToggleToolBar_triggered();
    void            on_action_ToggleAntiAliasing_triggered();
    void            on_action_ToggleInfoBox_triggered();
    void            on_action_About_triggered();
    void            on_actiongroup_Tools_triggered(QAction* pressedAction);
    void            on_documentTab_currentChanged(int newIndex);
    void            on_colorChanged(const QString& changed);
    void            on_buttonGroup_capType_buttonClicked(int id);
    void            on_combobox_penStyle_activated(int index);
    void            on_combobox_brushStyle_activated(int index);
    void            update_mousePosition(const QPoint& mousePos);
    void            update_paintArea();
    void            on_paintArea_selectionChanged(const BaseShape* const shape);
    void            on_paintArea_filenameChanged(const QString& newName);
};

#endif // MAINWINDOW_H
