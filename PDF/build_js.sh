#!/bin/bash
EXPORTED_FUNCTIONS=(
"_clsPDF_create"
"_clsPDF_delete"
"_clsPDF_getPageCount"
"_clsPDF_getPage"
"_clsPDF_deletePage"
"_clsPDF_getBoundingBox"
"_clsPDF_getTextContents"
"_clsPDF_deleteTextContents"
"_clsPDF_getRenderedImage"
"_clsPDF_deleteRenderedImage"
"_clsPDF_getBuffer"
"_clsPDF_getWidth"
"_clsPDF_getHeight"
"_clsPDF_getParagraphCount"
"_clsPDF_getParagraph"
"_clsPDF_deleteParagraph"
"_clsPDF_getParagraphType"
"_clsPDF_getContentString"
"_clsPDF_getSentenceCount"
"_clsPDF_getSentence"
"_clsPDF_deleteSentence"
"_clsPDF_getLineCount"
"_clsPDF_getLine"
"_clsPDF_deleteLine"
"_clsPDF_getLineContents"
)
EXPORTED_FUNCTIONS=$(printf ",'%s'" ${EXPORTED_FUNCTIONS[*]})
EXPORTED_FUNCTIONS=${EXPORTED_FUNCTIONS:1}

mkdir -p ../out/jslib/

em++ -Oz --llvm-lto 3 libsrc/js_interface.cpp --std=c++11 -o ../out/jslib/libPDF.js -I../out/include  -L../out/lib -lPDF -lfpdfapi -lfpdfdoc -lfpdftext -lfxcodec -lfxcrt -lfdrm -lfxge -s EXPORTED_FUNCTIONS="[$EXPORTED_FUNCTIONS]" --memory-init-file 0
                                                                                                     
