################################################################################
#   Targoman: A robust Machine Translation framework
#
#   Copyright 2014-2018 by ITRC <http://itrc.ac.ir>
#
#   This file is part of Targoman.
#
#   Targoman is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Lesser General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   Targoman is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public License
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
################################################################################
BasePath = "."

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    src/appE4MT.h \
    src/Configs.h \
    src/XMLReader.h \
    fastText/src/dictionary.h \
    fastText/src/args.h \
    fastText/src/dictionary.h \
    fastText/src/fasttext.h \
    fastText/src/matrix.h \
    fastText/src/model.h \
    fastText/src/productquantizer.h \
    fastText/src/qmatrix.h \
    fastText/src/real.h \
    fastText/src/utils.h \
    fastText/src/vector.h \
    src/clsFormalityChecker.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    src/main.cpp \
    src/appE4MT.cpp \
    src/Configs.cpp \
    src/XMLReader.cpp \
    fastText/src/args.cc \
    fastText/src/dictionary.cc \
    fastText/src/fasttext.cc \
    fastText/src/matrix.cc \
    fastText/src/model.cc \
    fastText/src/productquantizer.cc \
    fastText/src/qmatrix.cc \
    fastText/src/utils.cc \
    fastText/src/vector.cc \
    src/clsFormalityChecker.cpp

DependencySearchPaths+=/usr/lib/x86_64-linux-gnu/ # to fix buggy installation location of libxml2 on ubunut and mint
INCLUDEPATH+=/usr/include/libxml2/ # to fix buggy installation location of libxml2 on ubunut and mint
################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFile = $$BasePath/Configs.pri
!exists($$ConfigFile){
error("**** libsrc: Unable to find Configuration file $$ConfigFile ****")
}
include ($$ConfigFile)

TEMPLATE = app
TARGET = $$ProjectName
DESTDIR = $$BaseBinFolder
OBJECTS_DIR = $$BaseBuildFolder/obj
MOC_DIR = $$BaseBuildFolder/moc
INCLUDEPATH += $$BasePath/libsrc
QMAKE_LIBDIR += $$BaseLibraryFolder