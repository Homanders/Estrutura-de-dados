#ifndef GENERATOR_H
#define GENERATOR_H

#include "SensorData.h"

#include <random>

class Generator
{
private:

    std::mt19937 engine;

    std::uniform_real_distribution<float> temperaturaDist;
    std::uniform_real_distribution<float> pressaoDist;
    std::uniform_real_distribution<float> umidadeDist;
    std::uniform_real_distribution<float> vibracaoDist;

public:

    // Gera nÃºmeros diferentes a cada execuÃ§Ã£o
    Generator();

    // Permite repetir exatamente o mesmo experimento
    Generator(unsigned int seed);

    SensorData generateReading(int id);
};

#endif