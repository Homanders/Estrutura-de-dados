#ifndef SENSORDATA_H
#define SENSORDATA_H

/**
 * @brief Representa uma leitura realizada por um sensor industrial.
 */
struct SensorData
{
    int idSensor;            ///< Identificador Ãºnico do sensor
    long long timestamp;     ///< Momento da leitura

    float temperatura;       ///< Temperatura (Â°C)
    float pressao;           ///< PressÃ£o (kPa)
    float umidade;           ///< Umidade relativa (%)
    float vibracao;          ///< VibraÃ§Ã£o (mm/s)
};

#endif