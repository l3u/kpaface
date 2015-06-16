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
#include <QDebug>

// flandmark includes
#include <CFeaturePool.h>
#include <CSparseLBPFeatures.h>

// Local includes
#include "FlandmarkDetector.h"

namespace kpaface
{

FlandmarkDetector::FlandmarkDetector()
{
    m_flandmark = clandmark::Flandmark::getInstanceOf("/usr/clandmark/models/flandmark_model.xml");

    clandmark::CFeaturePool* featurePool = new clandmark::CFeaturePool(
        m_flandmark->getBaseWindowSize()[0],
        m_flandmark->getBaseWindowSize()[1]
    );

    featurePool->addFeaturesToPool(new clandmark::CSparseLBPFeatures(
        featurePool->getWidth(), featurePool->getHeight(),
        featurePool->getPyramidLevels(), featurePool->getCumulativeWidths())
    );

    m_flandmark->setNFfeaturesPool(featurePool);
}

cimg_library::CImg<unsigned char>* FlandmarkDetector::cvImageToCImgImage(cv::Mat& cvImage)
{
    cimg_library::CImg<unsigned char>* convertedImage
        = new cimg_library::CImg<unsigned char>(cvImage.cols, cvImage.rows);

    for (int x = 0; x < cvImage.cols; x++) {
        for (int y = 0; y < cvImage.rows; y++) {
            (*convertedImage) (x, y) = cvImage.at<uchar>(y, x);
        }
    }

    return convertedImage;
}

void FlandmarkDetector::setImage(cv::Mat& cvImage)
{
    m_image = cvImageToCImgImage(cvImage);
}

void FlandmarkDetector::detectLandmarks(QRect& boundingBox)
{
    int bbox[8];
    bbox[0] = boundingBox.left();
    bbox[1] = boundingBox.top();
    bbox[2] = boundingBox.left() + boundingBox.width();
    bbox[3] = boundingBox.top();
    bbox[4] = boundingBox.left() + boundingBox.width();
    bbox[5] = boundingBox.top() + boundingBox.height();
    bbox[6] = boundingBox.left();
    bbox[7] = boundingBox.top() + boundingBox.height();

    // The linker fails as soon as I try to do this:
    //m_flandmark->detect_optimized(m_image, bbox);
    //clandmark::fl_double_t* landmarks = m_flandmark->getLandmarks();
}

}
