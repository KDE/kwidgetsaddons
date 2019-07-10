
include(ECMMarkAsTest)

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

macro(kwidgetsaddons_executable_tests)
  foreach(_testname ${ARGN})
    add_executable(${_testname} ${_testname}.cpp)
    target_link_libraries(${_testname} Qt5::Test KF5::WidgetsAddons)
    ecm_mark_as_test(${_testname})
  endforeach(_testname)
endmacro()

kwidgetsaddons_executable_tests(
  kanimatedbuttontest
  kbusyindicatorwidgettest
  kcharselecttest
  kcollapsiblegroupboxtest
  kdatepicktest
  kdatetimeedittestapp
  kledtest
  kmessageboxtest
  kmessageboxwidtest
  kselectactiontest
  kseparatortest
  ksqueezedtextlabeltest
  ktitlewidgettest
  kfontrequestertest
  kpassworddialogtest
  keditlistwidgettest
  kratingwidgettest
  kactionselectortest
  kcolorcombotest
  kpixmapregionselectordialogtest
  ktoolbarlabelactiontest
  kpagedialogtest
  kpagewidgettest
  kassistantdialogtest
  kmessagewidgettest
  knewpassworddialogtest
  knewpasswordwidget_test
  kpixmapsequenceoverlaypaintertest
  kmimetypechoosertest
  ksplittercollapserbuttongui_test
  ktooltipwidget_test
  kpasswordlineedit_test
)

add_executable(kdatetabletest kdatetabletest.cpp ../src/kdatetable.cpp)
target_include_directories(kdatetabletest PRIVATE ../src)
target_link_libraries(kdatetabletest Qt5::Widgets)
ecm_mark_as_test(kdatetabletest)

set(kcolumnresizertestapp_SRCS kcolumnresizertestapp.cpp)
qt5_wrap_ui(kcolumnresizertestapp_SRCS kcolumnresizertestapp.ui)
add_executable(kcolumnresizertestapp ${kcolumnresizertestapp_SRCS})
target_link_libraries(kcolumnresizertestapp KF5::WidgetsAddons Qt5::Widgets)
ecm_mark_as_test(kcolumnresizertestapp)
