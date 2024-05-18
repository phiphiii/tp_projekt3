#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <matplot/matplot.h>
#include <AudioFile.h>
//#include <imageio/imageio.h>
#include <vector>
#include <cmath>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace plt = matplot;

std::vector<double> generateSinus(double frequency) {
    double duration = 5.0; 
    double sampleRate = 10000.0;
    std::vector<double> result;
    for (double t = 0; t < duration; t += 1.0 / sampleRate) {
        result.push_back(std::sin(6.28 * frequency * t));
    }
    return result;
}

std::vector<double> generateCosine(double frequency) {
    double duration = 5.0; 
    double sampleRate = 10000.0;
    std::vector<double> result;
    for (double t = 0; t < duration; t += 1.0 / sampleRate) {
        result.push_back(std::cos(2 * 3.14 * frequency * t));
    }
    return result;
}

std::vector<double> generateSawtooth(double frequency) {
    double amplitude = 1.0;
    double duration = 5.0; 
    double sampleRate = 10000.0;
    std::vector<double> result;
    double period = 1.0 / frequency;
    double amplitudeIncrement = amplitude * (1.0 / (sampleRate * period));

    for (double t = 0; t < duration; t += 1.0 / sampleRate) {
        double value = fmod(t, period) * amplitudeIncrement - (amplitude / 2.0);
        result.push_back(value);
    }

    return result;
}

std::vector<double> generateSquare(double frequency) {
    double amplitude = 1.0;
    double duration = 5.0;
    double sampleRate = 10000.0;
    std::vector<double> result;
    double period = 1.0 / frequency;
    double halfPeriod = period / 2.0;

    for (double t = 0; t < duration; t += 1.0 / sampleRate) {
        double value = (fmod(t, period) < halfPeriod) ? amplitude : -amplitude;
        result.push_back(value);
    }

    return result;
}


void chart_printing(std::vector<double> &chart){
    plt::plot(chart);
    plt::xlabel("X axis");
    plt::ylabel("Y axis");
    plt::title("Sinus chart");
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
    Chuj2

    )pbdoc");

    m.def("generateCosine", &generateCosine, R"pbdoc(
    Chuj2

    )pbdoc");

    m.def("chart_printing", &chart_printing, R"pbdoc(
    Chuj3

    )pbdoc");

    m.def("generateSawtooth", &generateSawtooth, R"pbdoc(
    Chuj4

    )pbdoc");

    m.def("generateSquare", &generateSquare, R"pbdoc(
    Chuj5

    )pbdoc");


#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
