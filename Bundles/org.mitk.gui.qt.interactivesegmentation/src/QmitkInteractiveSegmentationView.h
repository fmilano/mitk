#ifndef QMITKINTERACTIVESEGMENTATIONVIEW_H_
#define QMITKINTERACTIVESEGMENTATIONVIEW_H_

#include <cherryIPartListener.h>

#include <QmitkViewPart.h>
#include <QmitkStandardViews.h>
#include <QmitkStdMultiWidgetEditor.h>

#include "mitkQtInteractiveSegmentationDll.h"
#include "ui_QmitkInteractiveSegmentationControls.h"

class MITK_QT_INTERACTIVESEGMENTATION QmitkInteractiveSegmentationView : public QObject
  , public QmitkViewPart  
{
  Q_OBJECT
public:

  void SetFocus();

  virtual ~QmitkInteractiveSegmentationView();

protected:

  void CreateQtPartControl(QWidget* parent);

// QmitkInteractiveSegmentation
public:

  virtual void CreateConnections();

  virtual void Activated();

  virtual void Deactivated();

  void OnNodePropertiesChanged();
  void OnNewNodesGenerated();

  protected slots:  

    void TreeChanged();

    void ReinitializeToImage();
    void CreateNewSegmentation();
    void LoadSegmentation();
    void DeleteSegmentation();
    void SaveSegmentation();
    void SetReferenceImagePixelSmoothing(bool on);

    void OnToolSelected(int id);
    void OnReferenceNodeSelected(const mitk::DataTreeNode*);

  protected:
    void HandleException( const char* str, QWidget* parent, bool showDialog ) const;
    void CheckImageAlignment(mitk::Image* image);

    QmitkStdMultiWidget * m_MultiWidget;

    QWidget* m_Parent;

    Ui::QmitkInteractiveSegmentationControls * m_Controls;
  /// from QmitkInteractiveSegmentation

};

#endif /*QMITKINTERACTIVESEGMENTATIONVIEW_H_*/
