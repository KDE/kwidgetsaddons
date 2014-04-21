cmake_minimum_required(VERSION 2.8.12)

project(KWidgetsAddons)

find_package(ECM 0.0.12 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings)
include(KDECMakeSettings)
set(REQUIRED_QT_VERSION 5.2.0)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Widgets)

include(FeatureSummary)
include(GenerateExportHeader)
include(ECMPackageConfigHelpers)
include(ECMSetupVersion)
include(ECMGenerateHeaders)

include(ECMCreateQmFromPoFiles)

set(KF5_VERSION "4.98.0")

ecm_setup_version(${KF5_VERSION} VARIABLE_PREFIX KWIDGETSADDONS
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kwidgetsaddons_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5WidgetsAddonsConfigVersion.cmake"
                        SOVERSION 5)

file(GLOB PO_FILES "po/*.po")
if (PO_FILES)
  ecm_create_qm_from_po_files(PO_FILES ${PO_FILES} CATALOG_NAME kwidgetsaddons5_qt)
endif()

add_subdirectory(src)
add_subdirectory(autotests)
add_subdirectory(tests)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5WidgetsAddons")

ecm_configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/KF5WidgetsAddonsConfig.cmake.in"
                              "${CMAKE_CURRENT_BINARY_DIR}/KF5WidgetsAddonsConfig.cmake"
                              INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
                              )

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/KF5WidgetsAddonsConfig.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/KF5WidgetsAddonsConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel )

install(EXPORT KF5WidgetsAddonsTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5WidgetsAddonsTargets.cmake NAMESPACE KF5:: )

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/kwidgetsaddons_version.h
  DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel
)

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
