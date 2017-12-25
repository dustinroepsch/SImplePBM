/*
MIT License

Copyright (c) 2017 Dustin Nicholas Ryan-Roepsch

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "simplepbm.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

const int image_width = 2000;
const int image_height = 4500;
const int num_iterations = 1000000000;

//on a barnsley fern, the entire image is in the area
//−2.1820 < x < 2.6558 and 0 ≤ y < 9.9983.
const double xMin = -2.1820;
const double xMax = 2.6558;
const double yMin = 0;
const double yMax = 9.9983;

double scale(double value, double oldMin, double oldMax, double newMin, double newMax)
{
    //linear interpolation defined by the points (oldMin, newMin), (oldMax, newMax);
    // y = ((newMax - newMin) / (oldMax - oldMin))(x - oldMin) + newMin;
    return ((newMax - newMin) / (oldMax - oldMin)) * (value - oldMin) + newMin;
}

int yx_to_index(int y, int x)
{
    return y * image_width + x;
}

int main()
{
    std::srand(std::time(NULL));

    double x = 0;
    double y = 0;

    //value initialization to initialize the array with zeros
    int *histogram = new int[image_height * image_width]();
    int max = 0;

    for (int i = 0; i < num_iterations; i++)
    {
        int roll = rand() % 100;

        double oldX = x;
        double oldY = y;

        if (roll == 0)
        {
            x = 0;
            y = 0.16 * oldY;
        }
        else if (roll <= 85)
        {
            x = 0.85 * oldX + 0.04 * oldY;
            y = -0.04 * oldX + 0.85 * oldY + 1.6;
        }
        else if (roll <= 92)
        {
            x = 0.2 * oldX - 0.26 * oldY;
            y = 0.23 * oldX + 0.22 * oldY + 1.6;
        }
        else
        {
            x = -0.15 * oldX + 0.28 * oldY;
            y = 0.26 * oldX + 0.24 * oldY + 0.44;
        }

        int plotX = int(scale(x, xMin, xMax, 0, image_width));
        int plotY = int(scale(y, yMin, yMax, 0, image_height));

        if (plotX < 0)
        {
            plotX = 0;
        }

        if (plotY < 0)
        {
            plotY = 0;
        }

        if (plotX >= image_width)
        {
            plotX = image_width - 1;
        }

        if (plotY >= image_height)
        {
            plotY = image_height - 1;
        }

        (histogram[yx_to_index(plotY, plotX)])++;

        if (histogram[yx_to_index(plotY, plotX)] > max)
        {
            max = histogram[yx_to_index(plotY, plotX)];
        }
    }

    std::cout << max << std::endl;

    SimplePBM outputImage(image_width, image_height);

    size_t index = 0;

    for (size_t row = 0; row < image_height; row++)
    {
        for (size_t col = 0; col < image_width; col++)
        {
            if (histogram[index] != 0)
            {
                outputImage.getPixel(image_height - row - 1, col).g = histogram[index] % 255;
            }
            index++;
        }
    }

    delete[] histogram;
    outputImage.writeToFile("fern.ppm");

    return 0;
}