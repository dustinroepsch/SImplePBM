SimplePBM
==========

SimplePBM is a barebone image IO library, defined entirely in one header file.

Minimal Example:

```c_cpp
#include "simplepbm.hpp"
#include <cstdlib> //for rand();

int main() {
    const int width = 300;
    const int height = 400;

    SimplePBM image(width, height);

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            image.getPixel(row, col).r = std::rand() % 256;
        }
    }

    image.writeToFile("random_red.ppm"); //saves to ppm file format.

    return 0;
}
```

If you want to compile and run the barnsley fern example provied, use:

```
clang++ -Wall -Werror -std=c++14 example.cpp
./a.out
```

Which should generate a file called `fern.ppm` that looks like:

![image of fern](https://i.imgur.com/undefined.png)
