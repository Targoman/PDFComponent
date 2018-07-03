SOURCES += \
    pdfiumjs/core/src/fdrm/crypto/fx_crypt.cpp \
    pdfiumjs/core/src/fdrm/crypto/fx_crypt_aes.cpp \
    pdfiumjs/core/src/fdrm/crypto/fx_crypt_sha.cpp

HEADERS += \
    pdfiumjs/core/include/fdrm/fx_crypt.h

ProjectName=fdrm
include(./common.pri)
