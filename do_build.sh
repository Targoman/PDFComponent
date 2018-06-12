#!/bin/bash

set -e

__DO_CONFIG=0
__DO_MAKE=0
__DO_LINK=0

EXPORTED_FUNCTIONS=(
    '_PDFiumJS_init'
    '_PDFiumJS_Doc_new'
    '_PDFiumJS_Doc_delete'
    '_PDFiumJS_Doc_get_page_count'
    '_PDFiumJS_Doc_get_page'
    '_PDFiumJS_Page_get_width'
    '_PDFiumJS_Page_get_height'
    '_PDFiumJS_Page_get_bitmap'
    '_PDFiumJS_Bitmap_get_buffer'
    '_PDFiumJS_Bitmap_get_stride'
    '_PDFiumJS_Bitmap_destroy'
    '_PDFiumJS_Page_destroy'
    '_PDFiumJS_GetLayoutAnalsisResults'
    '_PDFiumJS_LayoutAnalsisResults_destroy'
)

EXPORTED_FUNCTIONS=$(printf ",'%s'" ${EXPORTED_FUNCTIONS[*]})
EXPORTED_FUNCTIONS=${EXPORTED_FUNCTIONS:1}

for P in $@; do
    P=$(tr [:upper:] [:lower:] <<< "$P")
    if [ "$P" == "config" ]; then
        __DO_CONFIG=1
    fi
    if [ "$P" == "make" ]; then
        __DO_MAKE=1
    fi
    if [ "$P" == "link" ]; then
        __DO_LINK=1
    fi
done

do_config() {
    build/gyp_pdfium
}

do_make() {
    emmake make BUILDTYPE=Release -j8
}

do_link() {
    OBJS=(
        "out/Release/obj.target/pdfium_js/pdfium.js/pdfium.js.o"
        "out/Release/obj.target/pdfium_js/pdfium.js/pdfium.la.js.o"
        "out/Release/obj.target/pdfium_js/pdfium.js/layout_analysis.o"
        # "out/Release/obj.target/pdfium_js/core/src/reflow/layoutprovider_taggedpdf.o"
    )
    LIBS=(
        "out/Release/obj.target/libpdfium.a"
        "out/Release/obj.target/libfdrm.a"
        "out/Release/obj.target/libfpdfdoc.a"
        "out/Release/obj.target/libfpdfapi.a"
        "out/Release/obj.target/libfpdftext.a"
        "out/Release/obj.target/libformfiller.a"
        "out/Release/obj.target/libfxcodec.a"
        "out/Release/obj.target/libfxcrt.a"
        "out/Release/obj.target/libfxedit.a"
        "out/Release/obj.target/libfxge.a"
        "out/Release/obj.target/libpdfwindow.a"
    )
    OPT_PARAMS=(
        '-g4'
        # '-Oz'
        '--llvm-lto 3'
    )
    em++ \
        ${OPT_PARAMS[*]} \
        --memory-init-file 0 \
        -s ELIMINATE_DUPLICATE_FUNCTIONS=1 \
        -s AGGRESSIVE_VARIABLE_ELIMINATION=1 \
        -s EXPORTED_FUNCTIONS="[$EXPORTED_FUNCTIONS]" \
        --js-library pdfium.js/pdfium.js.lib.js \
        -o web/pdfium.js \
        -Wl,--start-group ${OBJS[*]} ${LIBS[*]} -Wl,--end-group

    test -f web/pdfium.js.mem && cp web/pdfium.js.mem web/viewer/
}

test $__DO_CONFIG -eq 1 && do_config
test $__DO_MAKE -eq 1 && do_make
test $__DO_LINK -eq 1 && do_link