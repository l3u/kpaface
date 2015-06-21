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
#include <QImage>

// Local includes
#include "ImageDisplay.h"
#include "DetectedFace.h"

ImageDisplay::ImageDisplay(QWidget* parent) : QLabel(parent)
{
    setMinimumSize(64, 64);
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

void ImageDisplay::resizeEvent(QResizeEvent*)
{
    updateImage();
}

void ImageDisplay::setImage(QImage image)
{
    for (DetectedFace* frame : findChildren<DetectedFace*>()) {
        frame->deleteLater();
    }

    m_aspect = double(image.width()) / double(image.height());
    m_image = image;
    updateImage();
}

void ImageDisplay::updateImage()
{
    if (m_image.isNull()) {
        setPixmap(QPixmap());
        return;
    }

    QPixmap pixmap;
    if (double(width()) / double(height()) < m_aspect) {
        pixmap = QPixmap::fromImage(m_image.scaled(width(), width() / m_aspect));
    } else {
        pixmap = QPixmap::fromImage(m_image.scaled(height() * m_aspect, height()));
    }

    setPixmap(pixmap);

    m_scaleWidth  = double(m_image.width()) / double(pixmap.width());
    m_scaleHeight = double(m_image.height()) / double(pixmap.height());
    m_horizontalBorder = (width() - pixmap.width()) / 2;
    m_verticalBorder = (height() - pixmap.height()) / 2;

    for (DetectedFace* frame : findChildren<DetectedFace*>()) {
        frame->setGeometry(mapToDisplay(frame->actualGeometry()));
    }
}

QImage ImageDisplay::image() const
{
    return m_image;
}

QRect ImageDisplay::mapToDisplay(QRect geometry) const
{
    return QRect(
        QPoint(int(double(geometry.left() / m_scaleWidth) + m_horizontalBorder),
                int(double(geometry.top() / m_scaleHeight)) + m_verticalBorder),
        QPoint(int(double(geometry.right() / m_scaleWidth) + m_horizontalBorder),
                int(double(geometry.bottom() / m_scaleHeight)) + m_verticalBorder)
    );
}
