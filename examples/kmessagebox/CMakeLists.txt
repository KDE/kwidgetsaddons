set(SRCS main.cpp)

add_executable(kwidgetaddons-example-kmessagebox ${SRCS})

target_link_libraries(kwidgetaddons-example-kmessagebox
    Qt5::Widgets
    KF5::WidgetsAddons
)

# We don't want to install the tutorial
#install(TARGETS kwidgetaddons-example-kmessagebox  ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
