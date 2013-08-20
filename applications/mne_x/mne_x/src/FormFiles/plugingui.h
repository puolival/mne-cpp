//=============================================================================================================
/**
* @file     plugingui.h
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     August, 2013
*
* @section  LICENSE
*
* Copyright (C) 2012, Christoph Dinh and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of the Massachusetts General Hospital nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MASSACHUSETTS GENERAL HOSPITAL BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    PluginGui class declaration
*
*/

#ifndef PLUGINGUI_H
#define PLUGINGUI_H

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "pluginitem.h"

#include <QMainWindow>
#include <QMap>

class PluginScene;

class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QButtonGroup;
class QActionGroup;
class QLineEdit;
class QToolButton;
class QAbstractButton;
class QGraphicsView;

class PluginGui : public QMainWindow
{
    Q_OBJECT

public:
   PluginGui();

private slots:
//    void buttonGroupClicked(int id);

    void actionGroupTriggered(QAction* action);

    void deleteItem();
    void pointerGroupClicked(int id);
    void bringToFront();
    void sendToBack();
    void itemInserted(PluginItem *item);

private:
    void createActions();
    void createItemMenu();
    void createToolbars();

    QAction* createItemAction(QString name, PluginItem::DiagramType type, QMenu* menu);

    qint32 m_id;

    QMap<int, PluginItem::DiagramType> m_qMapIdType;
    QMap<int, QString> m_qMapIdName;

    PluginScene *m_pluginScene;
    QGraphicsView *view;

    QAction *addAction;
    QAction *deleteAction;

    QAction *toFrontAction;
    QAction *sendBackAction;

    QMenu *itemMenu;

    QToolBar *m_pToolBarPlugins;
    QToolBar *m_pToolBarPointer;

    QActionGroup *m_pActionGroup;

    QButtonGroup *pointerTypeGroup;
    QAction *fillAction;
    QAction *lineAction;
};

#endif // PLUGINGUI_H
