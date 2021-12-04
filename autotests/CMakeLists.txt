
include(ECMAddTests)

find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

ecm_add_tests(
  kacceleratormanagertest.cpp
  kactionmenutest.cpp
  kcharselect_unittest.cpp
  kcollapsiblegroupbox_test.cpp
  kcolorbuttontest.cpp
  kdatecomboboxtest.cpp
  kdatetimeedittest.cpp
  kdualactiontest.cpp
  kpixmapsequencewidgettest.cpp
  knewpasswordwidgettest.cpp
  kselectaction_unittest.cpp
  ksqueezedtextlabelautotest.cpp
  ktimecomboboxtest.cpp
  ktoolbarpopupactiontest.cpp
  ktooltipwidgettest.cpp
  kmessagewidgetautotest.cpp
  kpagedialogautotest.cpp
  kpassworddialogautotest.cpp
  kpasswordlineedittest.cpp
  ksplittercollapserbuttontest.cpp
  kmultitabbartest.cpp
  ktwofingertaptest.cpp
  ktwofingerswipetest.cpp
  LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test KF5::WidgetsAddons
)

set (CMAKE_AUTOUIC TRUE)
ecm_add_test(
  kcolumnresizertest.cpp
  kcolumnresizertest-forms.ui
  kcolumnresizertest-grids.ui
  kcolumnresizertest-grid-and-form.ui
  TEST_NAME kcolumnresizertest
  NAME_PREFIX "kwidgetsaddons-"
  LINK_LIBRARIES Qt${QT_MAJOR_VERSION}::Test KF5::WidgetsAddons
)
