/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


// QmitkFunctionality.h: interface for the QmitkFunctionality class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUSFUNCTIONALITY_H__1DC0BA6E_9B8D_4D63_8A63_5B661CE33712__INCLUDED_)
#define AFX_QUSFUNCTIONALITY_H__1DC0BA6E_9B8D_4D63_8A63_5B661CE33712__INCLUDED_

#include <qobject.h>
#include <qstring.h>
#include <qtoolbutton.h>
#include <qaction.h>

class QActionGroup;

#include "mitkDataTree.h"

class QmitkFctMediator;

/*!
\brief class providing a standard interface for MITK application functionality

A functionality contains the application main widget, a controls widget and the 
application logic. The functionality has to set up the main widget according to 
its needs when it is activated, especially if it uses a shared main wiget. 
It does @em not need to reverse the settings when it is deactivated, since it cannot
know if the subsequently activated functionality will use that settings or some other
setting. Furthermore, most functionalities will connect interaction objects in activated()
and disconnect them in deactivated().
*/
class QmitkFunctionality : public QObject
{
  Q_OBJECT
    bool m_Available;
  bool m_Activated;
public:

  /*!
  \brief default constructor for a Functionality object
  @param parent the parent widget
  @param name the name of the functionality widget
  */
  QmitkFunctionality(QObject *parent=0, const char *name=0, mitk::DataTreeIteratorBase* dataIt=NULL);

  /*!
  default destructor
  */
  virtual ~QmitkFunctionality();

  /*!
  \brief method for creating the widget containing the functionalities controls, like sliders, buttons etc.
  */
  virtual QWidget * createControlWidget(QWidget *parent) = 0;

  /*!
  \brief method for creating the functionality's main widget
  */
  virtual QWidget * createMainWidget(QWidget * parent) = 0;

  /*!
  \brief method for creating the connections of main and control widget
  */
  virtual void createConnections() {};

  /*!
  \brief method for creating a QAction object, i.e., toolbar button and menu entry
  */
  virtual QAction * createAction(QActionGroup * parent) { return NULL; };

  /*!
  \brief method for defining the name of the functionality
  */
  virtual QString getFunctionalityName();

  /*!
  \brief called when a functionality becomes active/visible. Often, event-handlers are connected (e.g., 
  GlobalStateMachine::AddInteractor() or AddListener()) in activated() and the connection is removed in deactivated()
  (e.g., GlobalStateMachine::RemoveInteractor() or RemoveListener()).
  */
  virtual void activated();

  /*!
  \brief called when a functionality is deactivated, i.e., is no longer active/visible. Often, event-handlers are connected (e.g., 
  GlobalStateMachine::AddInteractor() or AddListener()) in activated() and the connection is removed in deactivated()
  (e.g., GlobalStateMachine::RemoveInteractor() or RemoveListener()).
  */
  virtual void deactivated();


  //##Documentation
  //## @brief Is the functionality currently active?
  //## 
  //## @warning Will only work if in sub-classes the superclass-methods are called in activated() and deactivated()!
  virtual bool isActivated();

  virtual bool isAvailable();

  /*!
  \brief setter method for data tree attribute
  @param it the reference to a data tree ieterator object
  */
  void setDataTree(mitk::DataTreeIteratorBase* it);

  /*!
  \brief getter for dataTree attribute. It returns the 
  reference to a data tree iterator object
  */
  mitk::DataTreeIteratorBase* getDataTree();

  virtual void treeChanged(const itk::EventObject & treeChangedEvent);

  virtual void treeChanged();

signals:
  void signal_dummy();
  void availabilityChanged(QmitkFunctionality*);
  void availabilityChanged();


protected:
  virtual void setAvailability(bool available);
  /*!
  a reference to a data tree iterator object
  */
  mitk::DataTreeIteratorClone m_DataTreeIterator;

  bool m_TreeChangedWhileInActive;

  unsigned long m_ObserverTag;
  friend class QmitkFctMediator;
};

#endif // !defined(AFX_QUSFUNCTIONALITY_H__1DC0BA6E_9B8D_4D63_8A63_5B661CE33712__INCLUDED_)
