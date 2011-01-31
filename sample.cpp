/*
 * charter
 * Copyright (C) 2011 Michał Janiszewski
 *
 * This work is a thesis. Due to nature of law you are obliged to
 * get a written permission from Technical University of Lodz prior to
 * using any part of this work. If you do get a permission, you are able
 * to use the work, which is thereafter licensed using GNU GPL v3 or
 * (at your option) any later.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sample.h"

Sample::Sample(QObject *parent) :
	QObject(parent)
{
}

Sample::Sample(const Sample &other) :
	QObject(),
	marker(other.marker),
	right(other.right),
	up(other.up),
	left(other.left),
	pos(other.pos),
	time(other.time)
{
}

Sample &Sample::operator =(const Sample& other)
{
	if (this != &other) {
		this->marker = other.marker;
		this->right = other.right;
		this->up = other.up;
		this->left = other.left;
		this->pos = other.pos;
		this->time = other.time;
	}
	return *this;
}