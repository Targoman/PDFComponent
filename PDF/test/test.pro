SOURCES += \
    render_pdf_test.cpp \
    layoutanalysis_test.cpp \
    main.cpp

TEMPLATE = app
TARGET = test_libPDF

include(../../Project.pri)

QT -= gui

INCLUDEPATH += $$INC_OUT_DIR
INCLUDEPATH += /usr/include/opencv
QMAKE_LIBDIR += $$LIB_OUT_DIR
DESTDIR = $$BIN_OUT_DIR

LIBS += -L$$LIB_OUT_DIR -lPDF -lfpdfapi -lfpdfdoc -lfpdftext -lfxcodec -lfdrm -lfxcrt -lfxge
#LIBS += -L$$LIB_OUT_DIR -lPDF
LIBS += -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab

CONFIG += c++11
