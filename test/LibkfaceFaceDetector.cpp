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

// Qt includes
#include <QRect>
#include <QRectF>
#include <QImage>

// Local includes
#include "LibkfaceFaceDetector.h"

LibkfaceFaceDetector::LibkfaceFaceDetector()
{
    m_faceDetector = new KFaceIface::FaceDetector();
}

void LibkfaceFaceDetector::updateSettings(int accuracy, int specificity)
{
    QVariantMap params;
    params[QString::fromUtf8("accuracy")] = accuracy;
    params[QString::fromUtf8("specificity")] = float(specificity) / 100;
    m_faceDetector->setParameters(params);
}

QList<QRect> LibkfaceFaceDetector::detect(QImage image)
{
    QSize imageSize = image.size();
    QList<QRectF> faces = m_faceDetector->detectFaces(image);
    QList<QRect> mappedFaces;

    for (int i = 0; i < faces.size(); ++i) {
        mappedFaces << KFaceIface::FaceDetector::toAbsoluteRect(faces.at(i), imageSize);
    }

    return mappedFaces;
}
