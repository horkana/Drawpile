/*
   Drawpile - a collaborative drawing program.

   Copyright (C) 2014 Calle Laakkonen

   Drawpile is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Drawpile is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Drawpile.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "utils.h"

#include <QString>

namespace docks {

	QString defaultDockStylesheet()
	{
#ifdef Q_OS_MAC // the default look is fine on Mac
		return QString();
#else
		static QString css = QStringLiteral(
			"QDockWidget {"
				"titlebar-close-icon: url(:/icons/builtin/dock-close.png);"
				"titlebar-normal-icon: url(:/icons/builtin/dock-detach.png);"

			"}"
		);
		return css;
#endif
	}
}
