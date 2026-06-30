#include "Generator.h"

#include <ctime>

Generator::Generator()
    : temperaturaDist(20.0f, 40.0f),
      pressaoDist(95.0f, 110.0f),
      umidadeDist(30.0f, 100.0f),
      vibracaoDist(0.0f, 5.0f)
{
    std::random_device rd;
    engine.seed(rd());
}

Generator::Generator(unsigned int seed)
    : temperaturaDist(20.0f, 40.0f),
      pressaoDist(95.0f, 110.0f),
      umidadeDist(30.0f, 100.0f),
      vibracaoDist(0.0f, 5.0f)
{
    engine.seed(seed);
}

SensorData Generator::generateReading(int id)
{
    SensorData sensor;

    sensor.idSensor = id;
    sensor.timestamp = time(nullptr);

    sensor.temperatura = temperaturaDist(engine);
    sensor.pressao = pressaoDist(engine);
    sensor.umidade = umidadeDist(engine);
    sensor.vibracao = vibracaoDist(engine);

    return sensor;
}