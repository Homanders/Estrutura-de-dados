#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <string>

using namespace std;

class Visualization
{
private:
    int calculateBarLength(double value, double maxValue, int maxBarSize) const;

public:
    void printBarChart(const string& title,
                       string labels[],
                       double values[],
                       int size,
                       const string& unit) const;

    void printRanking(const string& title,
                      string labels[],
                      double values[],
                      int size,
                      const string& unit) const;

    void printComplexitySummary() const;

    void printComplexityComparison(const string labels[],
                                   const string complexities[],
                                   const double observedGrowth[],
                                   const double theoreticalGrowth[],
                                   int size) const;

    void printFinalDashboard(string labels[],
                             double insertTimes[],
                             double searchTimes[],
                             double removeTimes[],
                             int size) const;
};

#endif
