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

#ifndef FLANDMARKDETECTOR_H
#define FLANDMARKDETECTOR_H

// OpenCV includes
#include <opencv2/opencv.hpp>

// Qt includes
#include <QRect>

// flandmark includes
#include <Flandmark.h>

namespace kpaface
{

class FlandmarkDetector
{
public:
    FlandmarkDetector();
    void setImage(cv::Mat& cvImage);
    QList<QPoint> detectLandmarks(const QRect& boundingBox) const;

private: // Functions
    cimg_library::CImg<unsigned char>* cvImageToCImgImage(const cv::Mat& cvImage) const;

private: // Variables
    clandmark::Flandmark* m_flandmark;
    cimg_library::CImg<unsigned char>* m_image;
    QList<int> m_relevantLandmarks;
};

}

#endif // FLANDMARKDETECTOR_H
