/*
   Drawpile - a collaborative drawing program.

   Copyright (C) 2007-2014 Calle Laakkonen

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
#include <QDebug>

#include "net/userlist.h"
#include "../shared/net/meta.h"

namespace net {

UserListModel::UserListModel(QObject *parent)
	: QAbstractListModel(parent)
{
}


QVariant UserListModel::data(const QModelIndex& index, int role) const
{
	if(index.isValid() && index.row() >= 0 && index.row() < _users.size()) {
		if(role == Qt::DisplayRole)
			return QVariant::fromValue(_users.at(index.row()));
	}

	return QVariant();
}

int UserListModel::rowCount(const QModelIndex& parent) const
{
	if(parent.isValid())
		return 0;
	return _users.count();
}

void UserListModel::addUser(const User &user)
{
	// Check that the user doesn't exist already
	for(int i=0;i<_users.count();++i) {
		User &u = _users[i];
		if(u.id == user.id) {
			qWarning() << "replacing user" << u.id << u.name << "with" << user.name;
			u.name = user.name;
			u.isLocal = user.isLocal;

			QModelIndex idx = index(i);
			emit dataChanged(idx, idx);
			return;
		}
	}

	int pos = _users.count();
	beginInsertRows(QModelIndex(),pos,pos);
	_users.append(user);
	endInsertRows();
}

void UserListModel::updateUser(const protocol::UserAttr &ua)
{
	for(int i=0;i<_users.count();++i) {
		if(_users.at(i).id == ua.contextId()) {

			User &u = _users[i];
			u.isOperator = ua.isOp();
			u.isLocked = ua.isLocked();
			u.isMod = ua.isMod();
			u.isAuth = ua.isAuth();

			QModelIndex idx = index(i);
			emit dataChanged(idx, idx);
			return;
		}
	}
}

void UserListModel::removeUser(int id)
{
	for(int pos=0;pos<_users.count();++pos) {
		if(_users.at(pos).id == id) {
			beginRemoveRows(QModelIndex(),pos,pos);
			User u = _users[pos];
			_users.remove(pos);
			endRemoveRows();
			_pastUsers[u.id] = u;
			return;
		}
	}
}

void UserListModel::clearUsers()
{
	beginRemoveRows(QModelIndex(), 0, _users.count()-1);
	foreach(const User &u, _users)
		_pastUsers[u.id] = u;
	_users.clear();
	endRemoveRows();
}

User UserListModel::getUserById(int id) const
{
	// Try active users first
	foreach(const User &u, _users)
		if(u.id == id)
			return u;

	// Then the past users
	if(_pastUsers.contains(id))
		return _pastUsers[id];

	// Nothing found
	return User();
}

QString UserListModel::getUsername(int id) const
{
	// Try active users first
	foreach(const User &u, _users)
		if(u.id == id)
			return u.name;

	// Then the past users
	if(_pastUsers.contains(id)) {
		if(_pastUsers.contains(id))
			return _pastUsers[id].name;
	}

	// Not found
	return tr("User #%1").arg(id);
}

}
