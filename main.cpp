#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <matplot/matplot.h>
#include <AudioFile.h>
#include <vector>
#include <cmath>

#include <fstream>
#include <sstream>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace plt = matplot;
using namespace std;

typedef vector<vector<vector<float>>> Image;
typedef vector<vector<float>> Matrix;

Image loadImage(const string& imageFilePath) {
    ifstream image(imageFilePath);
    string type, width, height, RGB;
    image >> type >> width >> height >> RGB;

    int W = stoi(width);
    int H = stoi(height);
    int C = 3;

    Image img(H, vector<vector<float>>(W, vector<float>(C)));

    int r, g, b;
    for (int h = 0; h < H; ++h) {
        for (int w = 0; w < W; ++w) {
            image >> r >> g >> b;
            img[h][w][0] = r;
            img[h][w][1] = g;
            img[h][w][2] = b;
        }
    }
    return img;
}

void saveNewImage(const Image& image, const string& outputFilePath) {
    ofstream newImage(outputFilePath);
    int H = image.size();
    int W = image[0].size();
    int C = image[0][0].size();

    newImage << "P3" << endl;
    newImage << W << " " << H << endl;
    newImage << "255" << endl;

    for (int h = 0; h < H; ++h) {
        for (int w = 0; w < W; ++w) {
            for (int c = 0; c < C; ++c) {
                newImage << static_cast<int>(image[h][w][c]);
                if (c < C - 1) {
                    newImage << " ";
                }
            }
            newImage << endl;
        }
    }
}

Image filtration(const Image& image, const Matrix& filter) {
    int H = image.size();
    int W = image[0].size();
    int C = image[0][0].size();
    int h = filter.size();
    int w = filter[0].size();

    Image output(H - h + 1, Matrix(W - w + 1, vector<float>(C, 0.0f)));

    for (int i = 0; i <= H - h; ++i) {
        for (int j = 0; j <= W - w; ++j) {
            for (int fh = 0; fh < h; ++fh) {
                for (int fw = 0; fw < w; ++fw) {
                    for (int c = 0; c < C; ++c) {
                        output[i][j][c] += image[i + fh][j + fw][c] * filter[fh][fw];
                    }
                }
            }
        }
    }

    return output;
}

void imageFiltration(const std::string& inputFilePath, const std::string& outputFilePath, const Matrix& filter) {
    Image image = loadImage(inputFilePath);

    Image filteredImage = filtration(image, filter);
    saveNewImage(filteredImage, outputFilePath);
}

vector<double> generateSinus(double frequency) {
    double duration = 5.0; 
    double sampleRate = 1000.0;
    vector<double> result;
    for (double t = 0; t < duration; t += 1.0 / sampleRate) {
        result.push_back(sin(6.28 * frequency * t));
    }
    return result;
}

vector<double> generateCosine(double frequency) {
    double duration = 5.0; 
    double sampleRate = 1000.0;
    vector<double> result;
    for (double t = 0; t < duration; t += 1.0 / sampleRate) {
        result.push_back(cos(6.28 * frequency * t));
    }
    return result;
}

vector<double> generateSawtooth(double frequency) {
    double amplitude = 1.0;
    double duration = 5.0; 
    double sampleRate = 1000.0;
    vector<double> result;
    double period = 1.0 / frequency;
    double amplitudeIncrement = amplitude * (1.0 / (sampleRate * period));

    for (double t = 0; t < duration; t += 1.0 / sampleRate) {
        double value = fmod(t, period) * amplitudeIncrement - (amplitude / 2.0);
        result.push_back(value);
    }

    return result;
}

vector<double> generateRectangular(double frequency) {
    double amplitude = 1.0;
    double duration = 5.0;
    double sampleRate = 1000.0;
    vector<double> result;
    double period = 1.0 / frequency;
    double halfPeriod = period / 2.0;

    for (double t = 0; t < duration; t += 1.0 / sampleRate) {
        double value = (fmod(t, period) < halfPeriod) ? amplitude : -amplitude;
        result.push_back(value);
    }

    return result;
}


void chart_printing(vector<double> &chart){
    plt::plot(chart);
    plt::xlabel("X");
    plt::ylabel("Y");
    plt::title("Chart");
    plt::show();
}

namespace py = pybind11;

PYBIND11_MODULE(_core, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------

        .. currentmodule:: scikit_build_example

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";

    m.def("generateSinus", &generateSinus, R"pbdoc(
    

    )pbdoc");

    m.def("generateCosine", &generateCosine, R"pbdoc(
    

    )pbdoc");

    m.def("chart_printing", &chart_printing, R"pbdoc(
    

    )pbdoc");

    m.def("generateSawtooth", &generateSawtooth, R"pbdoc(
    

    )pbdoc");

    m.def("generateRectangular", &generateRectangular, R"pbdoc(
    

    )pbdoc");
    m.def("imageFiltration", &imageFiltration, R"pbdoc(
    

    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
