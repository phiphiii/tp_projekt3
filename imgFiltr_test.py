import scikit_build_example as sbe

import numpy as np

filter = np.array([
    [0.2126, 0.7152, 0.0722],
    [0.2126, 0.7152, 0.0722],
    [0.2126, 0.7152, 0.0722]
], dtype=np.float32)

filter_list = filter.tolist()

sbe.imageFiltration("landscape.ppm", "newLandscape.ppm", filter_list)