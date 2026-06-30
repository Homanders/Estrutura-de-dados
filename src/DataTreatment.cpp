#include "DataTreatment.h"

SensorData DataTreatment::normalize(SensorData sensor)
{
    // Temperatura
    if(sensor.temperatura < 20.0f)
        sensor.temperatura = 20.0f;

    if(sensor.temperatura > 40.0f)
        sensor.temperatura = 40.0f;

    // PressÃ£o
    if(sensor.pressao < 95.0f)
        sensor.pressao = 95.0f;

    if(sensor.pressao > 110.0f)
        sensor.pressao = 110.0f;

    // Umidade
    if(sensor.umidade < 30.0f)
        sensor.umidade = 30.0f;

    if(sensor.umidade > 100.0f)
        sensor.umidade = 100.0f;

    // VibraÃ§Ã£o
    if(sensor.vibracao < 0.0f)
        sensor.vibracao = 0.0f;

    if(sensor.vibracao > 5.0f)
        sensor.vibracao = 5.0f;

    return sensor;
}

bool DataTreatment::isValid(const SensorData& sensor)
{
    if(sensor.temperatura < 20.0f || sensor.temperatura > 40.0f)
        return false;

    if(sensor.pressao < 95.0f || sensor.pressao > 110.0f)
        return false;

    if(sensor.umidade < 30.0f || sensor.umidade > 100.0f)
        return false;

    if(sensor.vibracao < 0.0f || sensor.vibracao > 5.0f)
        return false;

    return true;
}