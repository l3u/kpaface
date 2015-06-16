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
#include <QFileDialog>
#include <QImageReader>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDir>
#include <QDebug>
#include <QPushButton>
#include <QCloseEvent>
#include <QFrame>
#include <QSpinBox>
#include <QCheckBox>
#include <QTime>
#include <QFrame>

// Flandmark includes
#include <Flandmark.h>

// Local includes
#include "test.h"
#include "DetectedFace.h"
#include "LibkfaceFaceDetector.h"
#include "libkpaface/FaceDetector.h"
#include "ImageDisplay.h"

test::test()
{
    QWidget* mainWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);
    setCentralWidget(mainWidget);

    // Image display

    m_imageDisplay = new ImageDisplay;
    m_imageDisplay->setPixmap(QPixmap(640, 480));
    mainLayout->addWidget(m_imageDisplay);

    // File navigation

    QHBoxLayout* navigationLayout = new QHBoxLayout;
    mainLayout->addLayout(navigationLayout);

    m_previousImage = new QPushButton(QString::fromUtf8("<<<"));
    connect(m_previousImage, SIGNAL(clicked()), this, SLOT(previousImage()));
    navigationLayout->addWidget(m_previousImage);
    m_previousImage->setEnabled(false);

    m_nextImage = new QPushButton(QString::fromUtf8(">>>"));
    connect(m_nextImage, SIGNAL(clicked()), this, SLOT(nextImage()));
    navigationLayout->addWidget(m_nextImage);
    m_nextImage->setEnabled(false);

    m_currentFileLabel = new QLabel(QString::fromUtf8("No images"));
    navigationLayout->addWidget(m_currentFileLabel);

    QPushButton* chooseDirectory = new QPushButton(QString::fromUtf8("Open directory"));
    connect(chooseDirectory, SIGNAL(clicked()), this, SLOT(chooseDirectory()));
    navigationLayout->addWidget(chooseDirectory);

    // Libkface

    QFrame* libkfaceSeparator = new QFrame;
    libkfaceSeparator->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(libkfaceSeparator);

    QGridLayout* libkfaceLayout = new QGridLayout;
    mainLayout->addLayout(libkfaceLayout);

    m_libkfaceDetect = new QPushButton(QString::fromUtf8("Detect with libkface"));
    connect(m_libkfaceDetect, SIGNAL(clicked()), this, SLOT(libkfaceDetect()));
    libkfaceLayout->addWidget(m_libkfaceDetect, 0, 0);
    m_libkfaceDetect->setEnabled(false);

    m_libkfaceDuration = new QLabel;
    libkfaceLayout->addWidget(m_libkfaceDuration, 1, 0);

    QLabel* accuracyLabel = new QLabel(QString::fromUtf8("Accuracy:"));
    libkfaceLayout->addWidget(accuracyLabel, 0, 1);

    m_accuracy = new QSpinBox;
    m_accuracy->setMaximum(100);
    m_accuracy->setValue(80);
    libkfaceLayout->addWidget(m_accuracy, 0, 2);

    QLabel* specificityLabel = new QLabel(QString::fromUtf8("Specificity:"));
    libkfaceLayout->addWidget(specificityLabel, 1, 1);

    m_specificity = new QSpinBox;
    m_specificity->setMaximum(100);
    m_specificity->setValue(80);
    libkfaceLayout->addWidget(m_specificity, 1, 2);

    // Libkpaface

    QFrame* libkpafaceSeparator = new QFrame;
    libkpafaceSeparator->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(libkpafaceSeparator);

    QGridLayout* libkpafacelayout = new QGridLayout;
    mainLayout->addLayout(libkpafacelayout);

    m_opencvDetect = new QPushButton(QString::fromUtf8("Detect with libkpaface"));
    connect(m_opencvDetect, SIGNAL(clicked()), this, SLOT(opencvDetect()));
    libkpafacelayout->addWidget(m_opencvDetect, 0, 0);
    m_opencvDetect->setEnabled(false);

    m_libkpafaceDuration = new QLabel;
    libkpafacelayout->addWidget(m_libkpafaceDuration, 1, 0);

    QLabel* scaleFactorLabel = new QLabel(QString::fromUtf8("Scale factor:"));
    libkpafacelayout->addWidget(scaleFactorLabel, 0, 1);

    m_scaleFactor = new QDoubleSpinBox;
    m_scaleFactor->setMinimum(1.01);
    m_scaleFactor->setSingleStep(0.01);
    m_scaleFactor->setValue(1.08);
    libkpafacelayout->addWidget(m_scaleFactor, 0, 2);

    QLabel* minNeighboursLabel = new QLabel(QString::fromUtf8("Min Neighbours:"));
    libkpafacelayout->addWidget(minNeighboursLabel, 1, 1);

    m_minNeighbours = new QSpinBox;
    m_minNeighbours->setMinimum(1);
    m_minNeighbours->setValue(2);
    libkpafacelayout->addWidget(m_minNeighbours, 1, 2);

    QLabel* minSizeLabel = new QLabel(QString::fromUtf8("Min size (% image area):"));
    libkpafacelayout->addWidget(minSizeLabel, 2, 1);

    m_minSize = new QDoubleSpinBox;
    m_minSize->setMaximum(20);
    m_minSize->setSingleStep(0.01);
    m_minSize->setValue(0.2);
    libkpafacelayout->addWidget(m_minSize, 2, 2);

    m_doCannyPruning = new QCheckBox(QString::fromUtf8("Canny Pruning"));
    m_doCannyPruning->setChecked(false);
    libkpafacelayout->addWidget(m_doCannyPruning, 3, 1, 1, 2);

    // Setup detectors

    m_libkfaceFaceDetector = new LibkfaceFaceDetector;
    m_libkpafaceFaceDetector = new kpaface::FaceDetector;
    m_imageReader = new QImageReader();
}

void test::chooseDirectory()
{
    QString directory = QFileDialog::getExistingDirectory(this,
        QString::fromUtf8("Choose directory to browse"),
        QString(),
        QFileDialog::ShowDirsOnly
    );

    if (! directory.isEmpty()) {
        loadDirectory(directory);
    }
}

void test::loadDirectory(const QString directory)
{
    m_imageDisplay->setImage(QImage());
    m_files.clear();
    m_currentFileLabel->setText(QString::fromUtf8("No images"));
    m_currentFile = 0;

    QDir imagesDirectory = QDir(directory);
    QStringList files = imagesDirectory.entryList(QDir::Files
                                                  | QDir::NoDotAndDotDot
                                                  | QDir::Readable);
    for (QString fileName : files) {
        QString fullPath = directory + QString::fromUtf8("/") + fileName;
        m_imageReader->setFileName(fullPath);
        if (m_imageReader->canRead()) {
            m_files << fullPath;
        }
    }

    if (m_files.count() > 0) {
        m_previousImage->setEnabled(true);
        m_nextImage->setEnabled(true);
        m_libkfaceDetect->setEnabled(true);
        m_opencvDetect->setEnabled(true);
        setImage();
    } else {
        m_previousImage->setEnabled(false);
        m_nextImage->setEnabled(false);
        m_libkfaceDetect->setEnabled(false);
        m_opencvDetect->setEnabled(false);
    }
}

void test::previousImage()
{
    if (m_currentFile == 0) {
        return;
    }

    m_currentFile--;
    setImage();
}

void test::nextImage()
{
    if (m_currentFile == m_files.count() - 1) {
        return;
    }

    m_currentFile++;
    setImage();
}

void test::setImage()
{
    m_previousImage->setEnabled(m_currentFile > 0);
    m_nextImage->setEnabled(!(m_currentFile == m_files.count() - 1));
    m_currentFileLabel->setText(
        QString::fromUtf8("Image %1 of %2").arg(m_currentFile + 1).arg(m_files.count())
    );
    m_imageDisplay->setImage(QImage(m_files[m_currentFile]));
}

void test::libkfaceDetect()
{
    QTime time;
    time.start();

    m_libkfaceFaceDetector->updateSettings(m_accuracy->value(), m_specificity->value());
    addFrames(m_libkfaceFaceDetector->detect(m_imageDisplay->image()),
              QString::fromUtf8("red"));

    m_libkfaceDuration->setText(QString::fromUtf8("Detection took %1 ms").arg(time.elapsed()));
}

void test::opencvDetect()
{
    QTime time;
    time.start();

    m_libkpafaceFaceDetector->setScaleFactor(m_scaleFactor->value());
    m_libkpafaceFaceDetector->setMinNeighbours(m_minNeighbours->value());
    m_libkpafaceFaceDetector->enableCannyPruning(m_doCannyPruning->isChecked());
    m_libkpafaceFaceDetector->setMinSize(m_minSize->value());

    addFrames(m_libkpafaceFaceDetector->detect(m_imageDisplay->image()),
              QString::fromUtf8("lime"));

    m_libkpafaceDuration->setText(QString::fromUtf8("Detection took %1 ms").arg(time.elapsed()));
}

void test::addFrames(const QList<QRect> geometriesList, const QString color)
{
    for (DetectedFace* frame : m_imageDisplay->findChildren<DetectedFace*>()) {
        frame->deleteLater();
    }

    for (QRect geometry : geometriesList) {
        new DetectedFace(m_imageDisplay, m_imageDisplay->mapToDisplay(geometry), geometry, color);
    }
}
