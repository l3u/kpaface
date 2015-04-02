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
#include <QFileInfo>
#include <QImage>
#include <QDebug>

// Local includes
#include "FaceDetector.h"
#include "OpenCVFaceDetector.h"

namespace kpaface
{

FaceDetector::FaceDetector()
{
    // Search the OpenCV haar cascade we will be using

    QString cascade = QString::fromUtf8("haarcascade_frontalface_alt2.xml");
    QList<QString> directories;
    directories << QString::fromUtf8("/usr/share/OpenCV/haarcascades/")
                << QString::fromUtf8("/usr/share/opencv/haarcascades/");

    QString cascadePath;
    for (QString directory : directories) {
        cascadePath = directory + cascade;
        QFileInfo fileInfo(cascadePath);
        if (fileInfo.isFile() && fileInfo.isReadable()) {
            break;
        }
    }

    // Setup the detector backend
    m_openCVFaceDetector = new OpenCVFaceDetector(cascadePath.toLocal8Bit().constData());

    // Default values for the OpenCV cascade classifier
    m_detectorSettings.scaleFactor   = 1.08;
    m_detectorSettings.minNeighbours = 2;
    m_detectorSettings.flags         = 0;
    m_detectorSettings.minSize       = 0.2;
}

QList<QRect> FaceDetector::detect(QImage image)
{
    // Prepare image for OpenCV processing

    cv::Mat cvImage;

    switch (image.format()) {
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        cvImage = cv::Mat(image.height(), image.width(),
                          CV_8UC4, image.scanLine(0), image.bytesPerLine());
        cvtColor(cvImage, cvImage, CV_RGBA2GRAY);
        break;
    default:
        image = image.convertToFormat(QImage::Format_RGB888);
        cvImage = cv::Mat(image.height(), image.width(),
                          CV_8UC3, image.scanLine(0), image.bytesPerLine());
        cvtColor(cvImage, cvImage, CV_RGB2GRAY);
    }

    equalizeHist(cvImage, cvImage);

    std::vector<cv::Rect> faceCandidates = m_openCVFaceDetector->detectFaces(cvImage,
                                                                             m_detectorSettings);

    // Convert OpenCV's result to Qt structures
    QList<QRect> convertedFaceCandidates;
    for (std::vector<cv::Rect>::const_iterator it = faceCandidates.begin();
         it != faceCandidates.end(); it++) {
        convertedFaceCandidates << QRect(it->x, it->y, it->width, it->height);
    }

    return convertedFaceCandidates;
}

void FaceDetector::setScaleFactor(double scaleFactor)
{
    m_detectorSettings.scaleFactor = scaleFactor;
}

void FaceDetector::setMinNeighbours(int minNeighbours)
{
    m_detectorSettings.minNeighbours = minNeighbours;
}

void FaceDetector::enableCannyPruning(bool state)
{
    if (state == true) {
        m_detectorSettings.flags = CV_HAAR_DO_CANNY_PRUNING;
    } else {
        m_detectorSettings.flags = 0;
    }
}

void FaceDetector::setMinSize(double minSize)
{
    m_detectorSettings.minSize = minSize;
}

}
