#ifndef COMMONDEFINE_H
#define COMMONDEFINE_H
#include <cstddef>
#include <QString>
#include <QVector>

constexpr std::size_t MAX_FILE_COUNT = 300;
constexpr size_t PROGRESS_WIDTH = 300;
constexpr size_t PROGRESS_MAX = 300;
constexpr const char *PROJECT_DIR = "projectDir";

using DbResult = QVector<QMap<QString, QString>>;

#endif // COMMONDEFINE_H
