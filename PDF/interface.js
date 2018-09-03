let TargomanPDF = (function libPDFjs_initialize() {
    let f, result = new Promise((_f, _g) => { f = _f; });
    function __initialize__() {

        class clsTextBox {
            constructor(_c_this_) {
                let P0 = Module._malloc(4 * Float32Array.BYTES_PER_ELEMENT);
                Module._clsPDF_getBoundingBox(
                    _c_this_,
                    P0 + 0 * Float32Array.BYTES_PER_ELEMENT,
                    P0 + 1 * Float32Array.BYTES_PER_ELEMENT,
                    P0 + 2 * Float32Array.BYTES_PER_ELEMENT,
                    P0 + 3 * Float32Array.BYTES_PER_ELEMENT
                );
                let A = new Float32Array(Module.HEAPU8.buffer, P0, 4);
                Object.defineProperty(this, 'X', { value: A[0], writable: false });
                Object.defineProperty(this, 'Y', { value: A[1], writable: false });
                Object.defineProperty(this, 'Width', { value: A[2], writable: false });
                Object.defineProperty(this, 'Height', { value: A[3], writable: false });
                Module._free(P0);
            }
        }

        class clsPageTextContent {
            constructor(_c_textcontents_) {
                // Do nothing
            }
        }

        class clsPDFPage  {
            constructor(_c_page_) {
                this._c_this_ = _c_page_;
            }

            delete() {
                Module._clsPDF_deletePage(this._c_this_);
            }

            getTextContents() {
                let _c_textcontents_ = Module._clsPDF_getTextContents(this._c_this_);
                return new clsPageTextContent(_c_textcontents_);
            }

            getRenderedImage() {
                let _c_rendered_image = Module._clsPDF_getRenderedImage(this._c_this_);

                let Width = Module._clsPDF_getWidth(_c_rendered_image);
                let Height = Module._clsPDF_getHeight(_c_rendered_image);
                let Buffer = Module._clsPDF_getBuffer(_c_rendered_image);

                let Canvas = document.createElement('canvas');
                Canvas.width = Width;
                Canvas.height = Height;
                let Context = Canvas.getContext('2d');
                let ImgData = Context.createImageData(Width, Height);
                ImgData.data.set(Module.HEAPU8.subarray(Buffer, Buffer + 4 * Width * Height), 0);
                Context.putImageData(ImgData, 0, 0);
                Context = null;

                Module._clsPDF_deleteRenderedImage(_c_rendered_image);

                return Canvas;
            }
        }

        class clsPDF {
            constructor(_arrayBuffer) {
                let buffer = new Uint8Array(_arrayBuffer);
                let P0 = Module._malloc(buffer.length);
                Module.HEAPU8.set(buffer, P0);
                this._c_this_ = Module._clsPDF_create(P0, buffer.length);
                // Module._free(P0);
            }

            delete() {
                Module.clsPDF_delete(this._c_this_);
            }

            getPageCount() {
                return Module._clsPDF_getPageCount(this._c_this_);
            }

            getPage(_index) {
                let _c_page_ = Module._clsPDF_getPage(this._c_this_, _index);
                return new clsPDFPage(_c_page_);
            }
        }

        f({ clsPDF, clsPDFPage, clsPageTextContent });
    };
    window.Module = {
        noExitRuntime: true,
        print: function() { 
            console.group.apply(console, arguments); 
            console.groupEnd();
        },
        printErr: function() { 
            console.group.apply(console, arguments); 
            console.groupEnd();
        },
        onRuntimeInitialized: __initialize__
    };
    return result;
})()
