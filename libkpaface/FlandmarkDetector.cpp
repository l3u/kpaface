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
    m_models
        << std::string("JOINT_MV_AFLW_SPLIT_1_-30deg.xml")
        << std::string("JOINT_MV_AFLW_SPLIT_1_30deg.xml")
        << std::string("JOINT_MV_AFLW_SPLIT_1_frontal.xml")
    ;

    QList<int> profileLandmarksMinus30 = QList<int>()
        << 6  // canthus-ll
        << 7  // canthus-lr
        << 9  // canthus-rl
        << 10 // canthus-rr
        << 13 // nose
        << 18 // chin
    ;
    m_relevantLandmarks << profileLandmarksMinus30;

    QList<int> profileLandmarksPlus30 = QList<int>()
        << 6  // canthus-ll
        << 7  // canthus-lr
        << 9  // canthus-rl
        << 10 // canthus-rr
        << 11 // nose
        << 18 // chin
    ;
    m_relevantLandmarks << profileLandmarksPlus30;

    QList<int> frontalLandmarks = QList<int>()
        << 6  // canthus-ll
        << 7  // canthus-lr
        << 9  // canthus-rl
        << 10 // canthus-rr
        << 13 // nose
        << 20 // chin
    ;
    m_relevantLandmarks << frontalLandmarks;

    for (std::string& model : m_models) {
        m_flandmarkPool << clandmark::Flandmark::getInstanceOf(model.c_str());
    }

    clandmark::CFeaturePool* featurePool = new clandmark::CFeaturePool(
        m_flandmarkPool[0]->getBaseWindowSize()[0],
        m_flandmarkPool[0]->getBaseWindowSize()[1]
    );

    featurePool->addFeaturesToPool(new clandmark::CSparseLBPFeatures(
        featurePool->getWidth(), featurePool->getHeight(),
        featurePool->getPyramidLevels(), featurePool->getCumulativeWidths())
    );

    for (int i = 0; i < m_flandmarkPool.count(); i++) {
        m_flandmarkPool[i]->setNFfeaturesPool(featurePool);
    }
}

cimg_library::CImg<unsigned char>* FlandmarkDetector::cvImageToCImgImage(const cv::Mat& cvImage) const
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

QList<QPoint> FlandmarkDetector::detectLandmarks(const QRect& boundingBox) const
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

    int bestModel;
    clandmark::fl_double_t lastScore = 0;
    for (int i = 0; i < m_flandmarkPool.count(); i++) {
        m_flandmarkPool[i]->detect_optimized(m_image, bbox);
        clandmark::fl_double_t score = m_flandmarkPool[i]->getScore();
        if (score > lastScore) {
            lastScore = score;
            bestModel = i;
        }
    }

    clandmark::fl_double_t* landmarks = m_flandmarkPool[bestModel]->getLandmarks();

    QList<int> xValues;
    QList<int> yValues;

    for (int i = 0; i < 2 * m_flandmarkPool[bestModel]->getLandmarksCount(); i += 2) {
        xValues.insert(i / 2, int(landmarks[i]));
    }

    for (int i = 1; i < 2 * m_flandmarkPool[bestModel]->getLandmarksCount(); i += 2) {
        yValues.insert((i - 1) / 2, int(landmarks[i]));
    }

    QList<QPoint> convertedLandmarks;
    for (int i = 0; i < xValues.size(); i++) {
        if (m_relevantLandmarks[bestModel].contains(i)) {
            convertedLandmarks << QPoint(xValues[i], yValues[i]);
        }
    }

    return convertedLandmarks;
}

}
