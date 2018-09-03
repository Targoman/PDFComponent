#!/bin/bash

# make -j
# rm ../out/bin/test_libPDF
# em++ -s NODERAWFS=1 -Wl,-O1 -Wl,-rpath,/opt/Qt5.10.0/5.10.0/gcc_64/lib -o ../out/bin/test_libPDF.js ../out/obj/render_pdf_test.o ../out/obj/layoutanalysis_test.o ../out/obj/main.o -L/home/behrooz/Projects/PDFComponent/out/lib -lPDF -lfpdfapi -lfpdfdoc -lfpdftext -lfxcodec -lfdrm -lfxcrt -lfxge  -L/opt/Qt5.10.0/5.10.0/gcc_64/lib -lpthread

pushd ../out/bin 
nodejs test_libPDF.js
python3 << EOP
import numpy as np
from matplotlib import pyplot as plt

f = open('./without_chars.unk')
size = np.fromfile(f, np.int32, 2)
data = np.fromfile(f, np.uint8, 4 * size[0] * size[1])

data = data.reshape(size[1], size[0], 4)
data = data[:, :, :3]

plt.imshow(data)
plt.show()
EOP
popd