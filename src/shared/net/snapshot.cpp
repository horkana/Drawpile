
/*
   Drawpile - a collaborative drawing program.

   Copyright (C) 2008-2013 Calle Laakkonen

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

#include "snapshot.h"

namespace protocol {

SnapshotMode *SnapshotMode::deserialize(const uchar *data, int len)
{
	if(len != 1)
		return 0;

	return new SnapshotMode(Mode(*data));
}

int SnapshotMode::payloadLength() const
{
	return 1;
}

int SnapshotMode::serializePayload(uchar *data) const
{
	*data = _mode;
	return 1;
}

void SnapshotPoint::append(MessagePtr msg)
{
	if(msg->type() == MSG_SNAPSHOT && msg.cast<SnapshotMode>().mode() == SnapshotMode::END)
		_complete = true;
	else
		_substream.append(msg);
}

void SnapshotPoint::abandon()
{
	Q_ASSERT(!_complete);
	if(_complete)
		return;

	_abandoned = true;
	_substream.clear();
}

}
