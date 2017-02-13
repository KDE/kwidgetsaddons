
include(ECMAddTests)

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

ecm_add_tests(
  kcollapsiblegroupbox_test.cpp
  kcolorbuttontest.cpp
  kdatecomboboxtest.cpp
  kdatetimeedittest.cpp
  kdualactiontest.cpp
  kpixmapsequencewidgettest.cpp
  knewpasswordwidgettest.cpp
  kselectaction_unittest.cpp
  ktimecomboboxtest.cpp
  ktooltipwidgettest.cpp
  kmessagewidgetautotest.cpp
  kpagedialogautotest.cpp
  ksplittercollapserbuttontest.cpp
  LINK_LIBRARIES Qt5::Test KF5::WidgetsAddons
)

set (CMAKE_AUTOUIC TRUE)
ecm_add_test(
  kcolumnresizertest.cpp
  kcolumnresizertest-forms.ui
  kcolumnresizertest-grids.ui
  kcolumnresizertest-grid-and-form.ui
  TEST_NAME kcolumnresizertest
  NAME_PREFIX "kwidgetsaddons-"
  LINK_LIBRARIES Qt5::Test KF5::WidgetsAddons
)
