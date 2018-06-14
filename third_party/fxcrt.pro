SOURCES += \
        pdfiumjs/core/src/fxcrt/fxcrt_platforms.cpp \
        pdfiumjs/core/src/fxcrt/fxcrt_posix.cpp \
        pdfiumjs/core/src/fxcrt/fxcrt_windows.cpp \
        pdfiumjs/core/src/fxcrt/fx_arabic.cpp \
        pdfiumjs/core/src/fxcrt/fx_basic_array.cpp \
        pdfiumjs/core/src/fxcrt/fx_basic_bstring.cpp \
        pdfiumjs/core/src/fxcrt/fx_basic_buffer.cpp \
        pdfiumjs/core/src/fxcrt/fx_basic_coords.cpp \
        pdfiumjs/core/src/fxcrt/fx_basic_gcc.cpp \
        pdfiumjs/core/src/fxcrt/fx_basic_list.cpp \
        pdfiumjs/core/src/fxcrt/fx_basic_maps.cpp \
        pdfiumjs/core/src/fxcrt/fx_basic_memmgr.cpp \
        pdfiumjs/core/src/fxcrt/fx_basic_plex.cpp \
        pdfiumjs/core/src/fxcrt/fx_basic_utf.cpp \
        pdfiumjs/core/src/fxcrt/fx_basic_util.cpp \
        pdfiumjs/core/src/fxcrt/fx_basic_wstring.cpp \
        pdfiumjs/core/src/fxcrt/fx_extension.cpp \
        pdfiumjs/core/src/fxcrt/fx_ucddata.cpp \
        pdfiumjs/core/src/fxcrt/fx_unicode.cpp \
        pdfiumjs/core/src/fxcrt/fx_xml_composer.cpp \
        pdfiumjs/core/src/fxcrt/fx_xml_parser.cpp
        
HEADERS += \
        pdfiumjs/core/include/fxcrt/fx_arb.h \
        pdfiumjs/core/include/fxcrt/fx_basic.h \
        pdfiumjs/core/include/fxcrt/fx_coordinates.h \
        pdfiumjs/core/include/fxcrt/fx_ext.h \
        pdfiumjs/core/include/fxcrt/fx_memory.h \
        pdfiumjs/core/include/fxcrt/fx_stream.h \
        pdfiumjs/core/include/fxcrt/fx_string.h \
        pdfiumjs/core/include/fxcrt/fx_system.h \
        pdfiumjs/core/include/fxcrt/fx_ucd.h \
        pdfiumjs/core/include/fxcrt/fx_xml.h \
        pdfiumjs/core/src/fxcrt/extension.h \
        pdfiumjs/core/src/fxcrt/fxcrt_platforms.h \
        pdfiumjs/core/src/fxcrt/fxcrt_posix.h \
        pdfiumjs/core/src/fxcrt/fxcrt_windows.h \
        pdfiumjs/core/src/fxcrt/fx_arabic.h \
        pdfiumjs/core/src/fxcrt/plex.h \
        pdfiumjs/core/src/fxcrt/xml_int.h

ProjectName=fxcrt
include(./common.pri)