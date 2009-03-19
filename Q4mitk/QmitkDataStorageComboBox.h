#ifndef QmitkDataStorageComboBox_h
#define QmitkDataStorageComboBox_h

// Own Includes
#include "mitkDataStorage.h"
#include "mitkDataTreeNode.h"

// Toolkit Includes
#include <QComboBox>

// Forward Declartions
class QmitkDataStorageListModel;
class QmitkDataStorageTableModel;

///
/// \class QmitkDataStorageComboBox
/// \author Michael Mueller
/// \version 4.0
/// \date 2009-02-09
/// \ingroup Widgets
/// \brief Displays all or a subset (defined by a predicate) of nodes of the Data Storage.
///
class QMITK_EXPORT QmitkDataStorageComboBox : public QComboBox
{
  Q_OBJECT

  public:
    QmitkDataStorageComboBox(QWidget* parent = 0, bool _AutoSelectFirstItem=false);
    QmitkDataStorageComboBox(mitk::DataStorage::Pointer dataStorage, mitk::NodePredicateBase* pred = 0, QWidget* parent = 0
      , bool _AutoSelectFirstItem=false);
    ~QmitkDataStorageComboBox();

    void init(mitk::DataStorage::Pointer dataStorage, mitk::NodePredicateBase* pred);
    //# Getter/Setter

    void SetDataStorage(mitk::DataStorage::Pointer dataStorage);    
    mitk::DataStorage::Pointer GetDataStorage() const;
   
    void SetPredicate(mitk::NodePredicateBase* pred);    
    mitk::NodePredicateBase* GetPredicate() const;
    
    const mitk::DataTreeNode::Pointer GetSelectedNode() const;
   
  signals:
    void OnSelectionChanged(const mitk::DataTreeNode::Pointer);
  protected slots:
    void OnCurrentIndexChanged(int);
    
  protected:
    QmitkDataStorageListModel* m_DataStorageListModel;
	  ///
	  /// \brief Autoselects first item if true.
	  bool m_AutoSelectFirstItem;
};

#endif // QmitkDataStorageComboBox_h

