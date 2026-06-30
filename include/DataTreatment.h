#ifndef DATATREATMENT_H
#define DATATREATMENT_H

#include "SensorData.h"

class DataTreatment
{
public:

    // Corrige valores fora da faixa permitida
    SensorData normalize(SensorData sensor);

    // Verifica se a leitura Ã© vÃ¡lida
    bool isValid(const SensorData& sensor);
};

#endif