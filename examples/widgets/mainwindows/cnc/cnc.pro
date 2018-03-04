QT += widgets
qtHaveModule(printsupport): QT += printsupport

HEADERS         = mainwindow.h\
                  codeeditor.h
SOURCES         = main.cpp \
                  mainwindow.cpp \
		  codeeditor.cpp
RESOURCES       = dockwidgets.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/cnc
INSTALLS += target
