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

#ifndef TEST_H
#define TEST_H

// Qt includes
#include <QMainWindow>

// Qt classes
class QLabel;
class QPushButton;
class QImage;
class QDoubleSpinBox;
class QSpinBox;
class QCheckBox;
class QImageReader;

// Local classes
class ImageDisplay;
class LibkfaceFaceDetector;

namespace kpaface
{
class FaceDetector;
}

struct linearRegression {
    double slope;
    double intercept;
    double correlation;
};

class test : public QMainWindow
{
    Q_OBJECT

public:
    static linearRegression calculateLinearRegression(const QList<QPoint>& coordinates);
    test();
    void loadDirectory(const QString directory);

private slots:
    void chooseDirectory();
    void previousImage();
    void nextImage();
    void libkfaceDetect();
    void opencvDetect();

private: // Functions
    void setImage();
    void addFrames(const QList<QRect> geometriesList, const QString color);

private: // Variables
    ImageDisplay* m_imageDisplay;
    QLabel* m_currentFileLabel;
    QPushButton* m_previousImage;
    QPushButton* m_nextImage;
    QPushButton* m_libkfaceDetect;
    QPushButton* m_opencvDetect;
    QStringList m_files;
    int m_currentFile;
    LibkfaceFaceDetector* m_libkfaceFaceDetector;
    kpaface::FaceDetector* m_libkpafaceFaceDetector;
    QDoubleSpinBox* m_scaleFactor;
    QSpinBox* m_minNeighbours;
    QCheckBox* m_doCannyPruning;
    QDoubleSpinBox* m_minSize;
    QSpinBox* m_accuracy;
    QSpinBox* m_specificity;
    QImageReader* m_imageReader;
    QLabel* m_libkfaceDuration;
    QLabel* m_libkpafaceDuration;
};

#endif // TEST_H
