
include(ECMAddTests)

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test UiTools)

ecm_add_tests(
  kcollapsiblegroupbox_test.cpp
  kcolorbuttontest.cpp
  kdatecomboboxtest.cpp
  kdatetimeedittest.cpp
  kdualactiontest.cpp
  knewpasswordwidgettest.cpp
  kselectaction_unittest.cpp
  ktimecomboboxtest.cpp
  kmessagewidgetautotest.cpp
  kpagedialogautotest.cpp
  ksplittercollapserbuttontest.cpp
  LINK_LIBRARIES Qt5::Test KF5::WidgetsAddons
)

ecm_add_tests(
  kcolumnresizertest.cpp
  NAME_PREFIX "kwidgetsaddons-"
  LINK_LIBRARIES Qt5::Test KF5::WidgetsAddons Qt5::UiTools
)
