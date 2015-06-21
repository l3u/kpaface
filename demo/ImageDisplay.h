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

#ifndef IMAGEDISPLAY_H
#define IMAGEDISPLAY_H

// Qt includes
#include <QLabel>

class ImageDisplay : public QLabel
{
public:
    ImageDisplay(QWidget* parent = 0);
    void setImage(QImage image);
    QImage image() const;
    QRect mapToDisplay(QRect geometry) const;

protected:
    virtual void resizeEvent(QResizeEvent*);

private: // Functions
    void updateImage();

private: // Variables
    QImage m_image;
    double m_aspect;
    double m_scaleWidth;
    double m_scaleHeight;
    int m_horizontalBorder;
    int m_verticalBorder;
};

#endif // IMAGEDISPLAY_H
