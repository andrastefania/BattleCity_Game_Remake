#pragma once
#ifndef NOGREENSCREEN_H
#define NOGREENSCREEN_H

#include <QImage>

QImage RemoveGreenScreen(const QString& imagePath, int threshold = 30);

#endif // NOGREENSCREEN_H
