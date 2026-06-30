#include "Visualization.h"

// [IA-GENERATED] Visualizacoes de terminal e comparacao real vs teorica implementadas com assistência de IA. Ver IA_USAGE.md.

#include <iostream>
#include <iomanip>

using namespace std;

int Visualization::calculateBarLength(double value,
                                      double maxValue,
                                      int maxBarSize) const
{
    if(maxValue <= 0.0)
    {
        return 0;
    }

    double ratio = value / maxValue;

    int length = static_cast<int>(ratio * maxBarSize);

    if(value > 0.0 && length == 0)
    {
        length = 1;
    }

    return length;
}

void Visualization::printBarChart(const string& title,
                                  string labels[],
                                  double values[],
                                  int size,
                                  const string& unit) const
{
    cout << "\n================ " << title << " ================\n\n";

    if(size <= 0)
    {
        cout << "Sem dados para exibir.\n";
        return;
    }

    double maxValue = values[0];

    for(int i = 1; i < size; i++)
    {
        if(values[i] > maxValue)
        {
            maxValue = values[i];
        }
    }

    const int maxBarSize = 50;

    for(int i = 0; i < size; i++)
    {
        int barLength = calculateBarLength(values[i],
                                           maxValue,
                                           maxBarSize);

        cout << left << setw(22) << labels[i]
             << " | ";

        for(int j = 0; j < barLength; j++)
        {
            cout << "#";
        }

        cout << " "
             << fixed << setprecision(4)
             << values[i]
             << " "
             << unit
             << endl;
    }

    cout << "============================================================\n";
}

void Visualization::printRanking(const string& title,
                                 string labels[],
                                 double values[],
                                 int size,
                                 const string& unit) const
{
    cout << "\n================ " << title << " ================\n\n";

    string tempLabels[20];
    double tempValues[20];

    for(int i = 0; i < size; i++)
    {
        tempLabels[i] = labels[i];
        tempValues[i] = values[i];
    }

    for(int i = 0; i < size - 1; i++)
    {
        for(int j = 0; j < size - i - 1; j++)
        {
            if(tempValues[j] > tempValues[j + 1])
            {
                double auxValue = tempValues[j];
                tempValues[j] = tempValues[j + 1];
                tempValues[j + 1] = auxValue;

                string auxLabel = tempLabels[j];
                tempLabels[j] = tempLabels[j + 1];
                tempLabels[j + 1] = auxLabel;
            }
        }
    }

    for(int i = 0; i < size; i++)
    {
        cout << setw(2)
             << i + 1
             << " - "
             << left << setw(22)
             << tempLabels[i]
             << " : "
             << fixed << setprecision(4)
             << tempValues[i]
             << " "
             << unit
             << endl;
    }

    cout << "============================================================\n";
}

void Visualization::printComplexitySummary() const
{
    cout << "\n================ RESUMO TEORICO DE COMPLEXIDADE ================\n\n";

    cout << left << setw(22) << "Estrutura"
         << setw(18) << "Insercao"
         << setw(18) << "Busca"
         << setw(18) << "Remocao"
         << endl;

    cout << "--------------------------------------------------------------------------\n";

    cout << left << setw(22) << "AVL Tree"
         << setw(18) << "O(log n)"
         << setw(18) << "O(log n)"
         << setw(18) << "O(log n)"
         << endl;

    cout << left << setw(22) << "Hash Table"
         << setw(18) << "O(1) medio"
         << setw(18) << "O(1) medio"
         << setw(18) << "O(1) medio"
         << endl;

    cout << left << setw(22) << "Optimized Hash"
         << setw(18) << "O(1) medio"
         << setw(18) << "O(1) medio"
         << setw(18) << "O(1) medio"
         << endl;

    cout << left << setw(22) << "Linked List"
         << setw(18) << "O(1)"
         << setw(18) << "O(n)"
         << setw(18) << "O(n)"
         << endl;

    cout << left << setw(22) << "Skip List"
         << setw(18) << "O(log n) esp."
         << setw(18) << "O(log n) esp."
         << setw(18) << "O(log n) esp."
         << endl;

    cout << left << setw(22) << "Bloom Filter"
         << setw(18) << "O(k)"
         << setw(18) << "O(k)"
         << setw(18) << "O(k)"
         << endl;

    cout << "\nObservacao: k representa a quantidade de funcoes hash.\n";

    cout << "==========================================================================\n";
}

void Visualization::printFinalDashboard(string labels[],
                                        double insertTimes[],
                                        double searchTimes[],
                                        double removeTimes[],
                                        int size) const
{
    cout << "\n============================================================\n";
    cout << " DASHBOARD FINAL DE DESEMPENHO\n";
    cout << "============================================================\n";

    printBarChart("TEMPO DE INSERCAO",
                  labels,
                  insertTimes,
                  size,
                  "ms");

    printBarChart("TEMPO DE BUSCA",
                  labels,
                  searchTimes,
                  size,
                  "ms");

    printBarChart("TEMPO DE REMOCAO",
                  labels,
                  removeTimes,
                  size,
                  "ms");

    printRanking("RANKING - INSERCAO MAIS RAPIDA",
                 labels,
                 insertTimes,
                 size,
                 "ms");

    printRanking("RANKING - BUSCA MAIS RAPIDA",
                 labels,
                 searchTimes,
                 size,
                 "ms");

    printRanking("RANKING - REMOCAO MAIS RAPIDA",
                 labels,
                 removeTimes,
                 size,
                 "ms");

    printComplexitySummary();

    cout << "\nDashboard finalizado.\n";
}
void Visualization::printComplexityComparison(const string labels[],
                                              const string complexities[],
                                              const double observedGrowth[],
                                              const double theoreticalGrowth[],
                                              int size) const
{
    cout << "\n================ COMPARACAO REAL VS TEORICA ================\n\n";

    /*
     * A comparacao real vs teorica nao prova formalmente a complexidade,
     * mas indica se o crescimento medido e compativel com o comportamento
     * esperado das estruturas.
     */
    cout << left << setw(22) << "Estrutura"
         << setw(24) << "Complexidade"
         << right << setw(18) << "Obs."
         << setw(18) << "Teor."
         << setw(16) << "Avaliacao" << "\n";

    cout << "------------------------------------------------------------------------------------------------\n";

    for(int i = 0; i < size; i++)
    {
        double diff = theoreticalGrowth[i] <= 0.0 ? 0.0 : observedGrowth[i] / theoreticalGrowth[i];
        string avaliacao = "Condizente";

        if(diff > 1.8 || diff < 0.35)
            avaliacao = "Diverge";
        else if(diff > 1.35 || diff < 0.65)
            avaliacao = "Atencao";

        cout << left << setw(22) << labels[i]
             << setw(24) << complexities[i]
             << right << setw(17) << fixed << setprecision(2) << observedGrowth[i] << "x"
             << setw(17) << fixed << setprecision(2) << theoreticalGrowth[i] << "x"
             << setw(16) << avaliacao << "\n";
    }

    cout << "============================================================\n";
}
