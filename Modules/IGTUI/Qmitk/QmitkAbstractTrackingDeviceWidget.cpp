/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "QmitkAbstractTrackingDeviceWidget.h";

QmitkAbstractTrackingDeviceWidget::QmitkAbstractTrackingDeviceWidget(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{
  m_TestConnectionWorkerThread = new QThread();
  m_TestConnectionWorker = new QmitkTrackingDeviceConfigurationWidgetConnectionWorker();
  CreateQtPartControl(this);
  CreateConnections();
  m_ErrorMessage = "";
};

QmitkAbstractTrackingDeviceWidget::~QmitkAbstractTrackingDeviceWidget(){
  if (m_TestConnectionWorker) delete m_TestConnectionWorker;
  if (m_TestConnectionWorkerThread) delete m_TestConnectionWorkerThread;
};

void QmitkAbstractTrackingDeviceWidget::TestConnectionFinished(bool connected, QString output)
{
  m_TestConnectionWorkerThread->quit();
  AddOutput(output.toStdString());
  MITK_INFO << "Test connection: " << connected;
  this->setEnabled(true);
  emit ProgressFinished();
}

void QmitkAbstractTrackingDeviceWidget::TestConnection()
{
  this->setEnabled(false);
  //construct a tracking device:
  mitk::TrackingDevice::Pointer testTrackingDevice = ConstructTrackingDevice();
  m_TestConnectionWorker->SetTrackingDevice(testTrackingDevice);
  m_TestConnectionWorkerThread->start();
  emit ProgressStarted();
}

void QmitkAbstractTrackingDeviceWidget::CreateConnections() {
  connect(m_TestConnectionWorker, SIGNAL(ConnectionTested(bool, QString)), this, SLOT(TestConnectionFinished(bool, QString)));
  connect(m_TestConnectionWorkerThread, SIGNAL(started()), m_TestConnectionWorker, SLOT(TestConnectionThreadFunc()));

  //move the worker to the thread
  m_TestConnectionWorker->moveToThread(m_TestConnectionWorkerThread);
};