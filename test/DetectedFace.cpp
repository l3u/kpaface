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

// Local includes
#include "DetectedFace.h"

DetectedFace::DetectedFace(QWidget* parent,
                           QRect geometry,
                           QRect actualGeometry,
                           QString color)
    : QFrame(parent),
      m_actualGeometry(actualGeometry)
{
    setFrameShape(QFrame::Box);
    setStyleSheet(QString::fromUtf8("color: %1;").arg(color));
    setGeometry(geometry);
    setVisible(true);
}

QRect DetectedFace::actualGeometry() const
{
    return m_actualGeometry;
}
