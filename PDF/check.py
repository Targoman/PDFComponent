import numpy as np
from matplotlib import pyplot as plt

f = open('./without_chars.unk')
size = np.fromfile(f, np.int32, 2)
data = np.fromfile(f, np.uint8, 4 * size[0] * size[1])

data = np.transpose(data.reshape(4, size[0], size[1]), (1, 2, 0))
data = data[:, :, :3]

plt.imshow(data)
plt.show()