#include "SensorAnalytics.h"

// [IA-GENERATED] Estatisticas completas, histograma e ordenacao por campo implementados com assistência de IA. Ver IA_USAGE.md.

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

bool SensorAnalytics::isTemperatureAnomaly(const SensorData& sensor) const
{
    return sensor.temperatura < 20.0f || sensor.temperatura > 39.0f;
}

bool SensorAnalytics::isPressureAnomaly(const SensorData& sensor) const
{
    return sensor.pressao < 95.0f || sensor.pressao > 109.0f;
}

bool SensorAnalytics::isHumidityAnomaly(const SensorData& sensor) const
{
    return sensor.umidade < 30.0f || sensor.umidade > 95.0f;
}

bool SensorAnalytics::isVibrationAnomaly(const SensorData& sensor) const
{
    return sensor.vibracao > 4.5f;
}

namespace
{
    struct CampoResumo
    {
        double soma = 0.0;
        double somaQuadrados = 0.0;
        float minimo = 0.0f;
        float maximo = 0.0f;
    };

    void atualizarResumo(CampoResumo& resumo, float valor, bool primeiro)
    {
        if(primeiro)
        {
            resumo.minimo = valor;
            resumo.maximo = valor;
        }

        if(valor < resumo.minimo)
            resumo.minimo = valor;

        if(valor > resumo.maximo)
            resumo.maximo = valor;

        resumo.soma += valor;
        resumo.somaQuadrados += static_cast<double>(valor) * valor;
    }

    void imprimirResumo(const string& titulo, const string& unidade,
                        const CampoResumo& resumo, int quantidade)
    {
        const double media = resumo.soma / quantidade;
        double variancia = (resumo.somaQuadrados / quantidade) - (media * media);

        if(variancia < 0.0 && variancia > -0.000001)
            variancia = 0.0;

        const double desvio = sqrt(variancia);

        cout << titulo << " (" << unidade << "):\n";
        cout << "  Media  : " << setw(8) << media
             << " | Desvio Padrao: " << setw(8) << desvio << "\n";
        cout << "  Min    : " << setw(8) << resumo.minimo
             << " | Max          : " << setw(8) << resumo.maximo << "\n\n";
    }

    void imprimirLinhaRegistro(const SensorData* s)
    {
        cout << "TS=" << s->timestamp
             << " | ID=" << s->idSensor
             << " | T=" << fixed << setprecision(2) << s->temperatura
             << " | P=" << s->pressao
             << " | U=" << s->umidade
             << " | V=" << s->vibracao << "\n";
    }
}

void SensorAnalytics::printStatistics(SensorData* dataset, int size) const
{
    cout << "\n================ ESTATISTICAS DOS SENSORES ================\n\n";

    if(dataset == nullptr || size <= 0)
    {
        cout << "Dataset vazio ou invalido.\n";
        cout << "============================================================\n";
        return;
    }

    /*
     * Media, minimo e maximo descrevem tendencia central e limites.
     * O desvio padrao mostra dispersao das leituras.
     * O histograma permite visualizar a distribuicao da temperatura
     * sem depender de interface grafica.
     */
    CampoResumo temperatura;
    CampoResumo pressao;
    CampoResumo umidade;
    CampoResumo vibracao;
    int histograma[10] = {0};

    for(int i = 0; i < size; i++)
    {
        const bool primeiro = (i == 0);

        atualizarResumo(temperatura, dataset[i].temperatura, primeiro);
        atualizarResumo(pressao, dataset[i].pressao, primeiro);
        atualizarResumo(umidade, dataset[i].umidade, primeiro);
        atualizarResumo(vibracao, dataset[i].vibracao, primeiro);

        int faixa = static_cast<int>((dataset[i].temperatura - 20.0f) / 2.0f);
        if(faixa < 0)
            faixa = 0;
        if(faixa > 9)
            faixa = 9;
        histograma[faixa]++;
    }

    cout << fixed << setprecision(2);
    cout << "Registros analisados: " << size << "\n\n";

    imprimirResumo("Temperatura", "C", temperatura, size);
    imprimirResumo("Pressao", "kPa", pressao, size);
    imprimirResumo("Umidade", "%", umidade, size);
    imprimirResumo("Vibracao", "mm/s", vibracao, size);

    int maiorFaixa = 0;
    for(int i = 0; i < 10; i++)
    {
        if(histograma[i] > maiorFaixa)
            maiorFaixa = histograma[i];
    }

    cout << "Distribuicao de Temperatura:\n";
    for(int i = 0; i < 10; i++)
    {
        const int inicio = 20 + i * 2;
        const int fim = inicio + 2;
        const int barras = maiorFaixa == 0 ? 0 : static_cast<int>((40.0 * histograma[i]) / maiorFaixa);

        cout << "[" << setw(2) << inicio << "-" << setw(2) << fim << "] | ";
        for(int j = 0; j < barras; j++)
            cout << "#";
        cout << setw(42 - barras) << " " << " | " << histograma[i] << " registros\n";
    }

    cout << "============================================================\n";
}

int SensorAnalytics::countAnomalies(SensorData* dataset, int size) const
{
    if(dataset == nullptr || size <= 0)
        return 0;

    int count = 0;
    for(int i = 0; i < size; i++)
    {
        if(isTemperatureAnomaly(dataset[i]) ||
           isPressureAnomaly(dataset[i]) ||
           isHumidityAnomaly(dataset[i]) ||
           isVibrationAnomaly(dataset[i]))
        {
            count++;
        }
    }

    return count;
}

void SensorAnalytics::printAnomalyReport(SensorData* dataset, int size) const
{
    cout << "\n================ RELATORIO DE ANOMALIAS ================\n\n";

    if(dataset == nullptr || size <= 0)
    {
        cout << "Dataset vazio ou invalido.\n";
        cout << "=========================================================\n";
        return;
    }

    int tempCount = 0;
    int pressCount = 0;
    int umidCount = 0;
    int vibCount = 0;
    int totalAnomalies = 0;

    for(int i = 0; i < size; i++)
    {
        bool anomalous = false;

        if(isTemperatureAnomaly(dataset[i]))
        {
            tempCount++;
            anomalous = true;
        }

        if(isPressureAnomaly(dataset[i]))
        {
            pressCount++;
            anomalous = true;
        }

        if(isHumidityAnomaly(dataset[i]))
        {
            umidCount++;
            anomalous = true;
        }

        if(isVibrationAnomaly(dataset[i]))
        {
            vibCount++;
            anomalous = true;
        }

        if(anomalous)
            totalAnomalies++;
    }

    cout << "Registros analisados........: " << size << "\n";
    cout << "Registros anomalos..........: " << totalAnomalies << "\n";
    cout << fixed << setprecision(2);
    cout << "Percentual de anomalias.....: " << (100.0 * totalAnomalies) / size << "%\n";

    cout << "\nAnomalias por tipo:\n\n";
    cout << "Temperatura fora da faixa...: " << tempCount << "\n";
    cout << "Pressao fora da faixa.......: " << pressCount << "\n";
    cout << "Umidade fora da faixa.......: " << umidCount << "\n";
    cout << "Vibracao elevada............: " << vibCount << "\n";

    cout << "\nLimites usados:\n";
    cout << "Temperatura: 20.0 a 39.0\n";
    cout << "Pressao....: 95.0 a 109.0\n";
    cout << "Umidade....: 30.0 a 95.0\n";
    cout << "Vibracao...: ate 4.5\n";
    cout << "=========================================================\n";
}

int SensorAnalytics::filterByTimestampRange(SensorData* dataset,
                                            int size,
                                            long long startTimestamp,
                                            long long endTimestamp,
                                            SensorData* output) const
{
    if(dataset == nullptr || output == nullptr || size <= 0)
        return 0;

    int count = 0;
    for(int i = 0; i < size; i++)
    {
        if(dataset[i].timestamp >= startTimestamp &&
           dataset[i].timestamp <= endTimestamp)
        {
            output[count] = dataset[i];
            count++;
        }
    }

    return count;
}

SensorData** SensorAnalytics::sortByField(SensorData* dataset,
                                          int tamanho,
                                          const string& campo) const
{
    if(dataset == nullptr || tamanho <= 0)
        return nullptr;

    /*
     * A ordenacao por campo permite explorar o dataset sob diferentes criterios,
     * como leituras mais quentes, maiores pressoes ou timestamps extremos.
     * Essa operacao complementa as buscas por chave e atende a analise otimizada
     * dos dados.
     */
    SensorData** ponteiros = new SensorData*[tamanho];

    for(int i = 0; i < tamanho; i++)
        ponteiros[i] = &dataset[i];

    sort(ponteiros, ponteiros + tamanho, [&](const SensorData* a, const SensorData* b)
    {
        if(campo == "temperatura")
            return a->temperatura < b->temperatura;
        if(campo == "pressao")
            return a->pressao < b->pressao;
        if(campo == "umidade")
            return a->umidade < b->umidade;
        if(campo == "vibracao")
            return a->vibracao < b->vibracao;
        if(campo == "idSensor")
            return a->idSensor < b->idSensor;

        return a->timestamp < b->timestamp;
    });

    return ponteiros;
}

void SensorAnalytics::printSorted(SensorData* dataset,
                                  int tamanho,
                                  const string& campo) const
{
    cout << "\n================ ORDENACAO POR CAMPO: " << campo << " ================\n\n";

    SensorData** ordenado = sortByField(dataset, tamanho, campo);

    if(ordenado == nullptr)
    {
        cout << "Dataset vazio ou invalido.\n";
        cout << "============================================================\n";
        return;
    }

    const int limite = tamanho < 10 ? tamanho : 10;

    cout << "Primeiros " << limite << " registros:\n";
    for(int i = 0; i < limite; i++)
        imprimirLinhaRegistro(ordenado[i]);

    cout << "\nUltimos " << limite << " registros:\n";
    for(int i = tamanho - limite; i < tamanho; i++)
        imprimirLinhaRegistro(ordenado[i]);

    delete[] ordenado;

    cout << "============================================================\n";
}
