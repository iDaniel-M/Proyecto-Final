#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include "Component.h"
#include <vector>
#include <QString>

class ReportGenerator {
public:
    static bool generateCSV(const std::vector<Component>& components, const QString& filePath);
};

#endif // REPORTGENERATOR_H
