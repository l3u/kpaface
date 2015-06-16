/* Copyright 2015 Tobias Leupold <tobias.leupold@web.de>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KPAFACEDETECTOR_H
#define KPAFACEDETECTOR_H

// OpenCV includes
#include <opencv2/opencv.hpp>

// Qt includes
#include <QList>
#include <QRect>

// Local includes
#include "structs.h"

// Qt classes
class QImage;

namespace kpaface
{

// Local classes
class OpenCVFaceDetector;
class FlandmarkDetector;

class FaceDetector
{
public:
    FaceDetector();
    QList<QRect> detect(QImage image);
    void setScaleFactor(double scaleFactor);
    void setMinNeighbours(int minNeighbours);
    void enableCannyPruning(bool state);
    void setMinSize(double minSize);
    linearRegression calculateLinearRegression(const QList<QPoint>& coordinates);

private: // Variables
    DetectorSettings m_detectorSettings;
    OpenCVFaceDetector* m_openCVFaceDetector;
    FlandmarkDetector* m_flandmarkDetector;
};

}

#endif // KPAFACEDETECTOR_H
