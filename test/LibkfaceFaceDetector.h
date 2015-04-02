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

#ifndef LIBKFACEDETECTOR_H
#define LIBKFACEDETECTOR_H

// Qt includes
#include <QList>

// libkface includes
#include <libkface/facedetector.h>

// Qt classes
class QRect;
class QImage;

class LibkfaceFaceDetector
{
public:
    LibkfaceFaceDetector();
    void updateSettings(int accuracy, int specificity);
    QList<QRect> detect(QImage image);

private: // Variables
    KFaceIface::FaceDetector* m_faceDetector;
};

#endif // LIBKFACEDETECTOR_H
