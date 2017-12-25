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

#pragma once

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <iostream>

struct PBM_RGB
{
    uint8_t r, g, b;
    PBM_RGB()
    {
        r = g = b = 0;
    };
};

class SimplePBM
{
  public:
    SimplePBM(uint32_t width, uint32_t height) : width(width), height(height)
    {
        raster = new PBM_RGB[width * height];
    };

    ~SimplePBM()
    {
        delete[] raster;
    };

    PBM_RGB &getPixel(uint32_t row, uint32_t col)
    {
        if (row >= height || col >= width)
        {
            std::stringstream errorMessage;
            errorMessage << "Pixel (" << col << "." << row << ") is out of bounds";
            throw errorMessage.str();
        }

        return raster[row * width + col];
    };

    void writeToFile(const std::string &filename)
    {
        std::ofstream outputStream(filename, std::ios::trunc | std::ios::out);

        //specification found here http://netpbm.sourceforge.net/doc/ppm.html

        //A "magic number" for identifying the file type. A ppm image's magic number is the two characters "P6"
        outputStream << "P6";

        //Whitespace (blanks, TABs, CRs, LFs).
        outputStream << "\n";

        //A width, formatted as ASCII characters in decimal.
        outputStream << width;

        //Whitespace.
        outputStream << " ";

        //A height, again in ASCII decimal.
        outputStream << height;

        //Whitespace.
        outputStream << "\n";

        //The maximum color value (Maxval), again in ASCII decimal. Must be less than 65536 and more than zero.
        outputStream << 255;

        //A single whitespace character (usually a newline)
        outputStream << "\n";

        //The rest of the file will be binary data, so we need to reopen are stream
        outputStream.close();

        std::ofstream binaryOutputStream(filename, std::ios::app | std::ios::out | std::ios::binary);

        //A raster of Height rows, in order from top to bottom.
        for (size_t row = 0; row < height; row++)
        {
            //Each row consists of Width pixels, in order from left to right.
            for (size_t col = 0; col < width; col++)
            {
                //Each pixel is a triplet of red, green, and blue samples, in that order.
                //Each sample is represented in pure binary by either 1 or 2 bytes.
                //If the Maxval is less than 256, it is 1 byte. Otherwise, it is 2 bytes. The most significant byte is first.

                binaryOutputStream.write(reinterpret_cast<char *>(&(getPixel(row, col).r)), 1);
                binaryOutputStream.write(reinterpret_cast<char *>(&(getPixel(row, col).g)), 1);
                binaryOutputStream.write(reinterpret_cast<char *>(&(getPixel(row, col).b)), 1);
            }
        }
        binaryOutputStream.close();
    };

  private:
    const uint32_t width, height;
    PBM_RGB *raster;
};