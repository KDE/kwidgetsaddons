
include(ECMAddTests)

find_package(Qt5 5.2.0 CONFIG REQUIRED Test)

ecm_add_tests(
  kcolorbuttontest.cpp
  kdatecomboboxtest.cpp
  kdatetimeedittest.cpp
  kdualactiontest.cpp
  kselectaction_unittest.cpp
  ktimecomboboxtest.cpp
  kmessagewidgetautotest.cpp
  LINK_LIBRARIES Qt5::Test KF5::WidgetsAddons
)
