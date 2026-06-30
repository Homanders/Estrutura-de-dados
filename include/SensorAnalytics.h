#ifndef SENSORANALYTICS_H
#define SENSORANALYTICS_H

#include <string>
#include "SensorData.h"

class SensorAnalytics
{
private:
    bool isTemperatureAnomaly(const SensorData& sensor) const;
    bool isPressureAnomaly(const SensorData& sensor) const;
    bool isHumidityAnomaly(const SensorData& sensor) const;
    bool isVibrationAnomaly(const SensorData& sensor) const;

public:
    void printStatistics(SensorData* dataset, int size) const;
    int countAnomalies(SensorData* dataset, int size) const;
    void printAnomalyReport(SensorData* dataset, int size) const;

    int filterByTimestampRange(SensorData* dataset,
                               int size,
                               long long startTimestamp,
                               long long endTimestamp,
                               SensorData* output) const;

    SensorData** sortByField(SensorData* dataset,
                             int tamanho,
                             const std::string& campo) const;

    void printSorted(SensorData* dataset,
                     int tamanho,
                     const std::string& campo) const;
};

#endif
