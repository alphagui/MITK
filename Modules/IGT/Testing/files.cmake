SET(MODULE_TESTS
   mitkCameraVisualizationTest.cpp
   mitkClaronInterfaceTest.cpp
   mitkClaronToolTest.cpp
   mitkClaronTrackingDeviceTest.cpp
   mitkInternalTrackingToolTest.cpp
   mitkNavigationDataDisplacementFilterTest.cpp
   mitkNavigationDataLandmarkTransformFilterTest.cpp
   mitkNavigationDataObjectVisualizationFilterTest.cpp
   mitkNavigationDataTest.cpp
   mitkNavigationDataToMessageFilterTest.cpp
   mitkNavigationDataToNavigationDataFilterTest.cpp
   mitkNavigationDataToPointSetFilterTest.cpp
   mitkNavigationDataTransformFilterTest.cpp
   mitkNDIPassiveToolTest.cpp
   mitkNDIProtocolTest.cpp
   mitkNDITrackingDeviceTest.cpp
   mitkTimeStampTest.cpp
   mitkTrackingVolumeTest.cpp
   mitkTrackingDeviceTest.cpp
   mitkTrackingToolTest.cpp
   mitkNavigationDataRecorderTest.cpp

   # ------------------ Deavtivated Tests ----------------------------------   
   #mitkTrackingDeviceSourceTest.cpp
   #mitkNavigationDataPlayerTest.cpp  # deactivated, see bug 3073
   #mitkNavigationDataSourceTest.cpp   # deactivated because of  bug #2323
   # -----------------------------------------------------------------------
   
   # ------------------ Navigation Tool Management Tests -------------------
   mitkNavigationToolStorageTest.cpp
   mitkNavigationToolStorageSerializerAndDeserializerTest.cpp
   mitkNavigationToolTest.cpp
   mitkNavigationToolReaderAndWriterTest.cpp
   # -----------------------------------------------------------------------
)
