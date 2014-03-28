/*
 * Copyright (C) 2013 Jolla Ltd.
 * Contact: Robin Burchell <robin.burchell@jollamobile.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include "calendarevent.h"

#include <QQmlInfo>

#include "calendarmanager.h"

NemoCalendarEvent::NemoCalendarEvent(NemoCalendarManager *manager, const QString &uid, bool newEvent)
    : QObject(manager), mManager(manager), mUniqueId(uid), mNewEvent(newEvent)
{
    connect(mManager, SIGNAL(notebookColorChanged(QString)), this, SLOT(notebookColorChanged(QString)));
}

NemoCalendarEvent::~NemoCalendarEvent()
{
}

QString NemoCalendarEvent::displayLabel() const
{
    return mManager->getEvent(mUniqueId).displayLabel;
}

void NemoCalendarEvent::setDisplayLabel(const QString &displayLabel)
{
    mManager->setDisplayLabel(mUniqueId, displayLabel);
}

QString NemoCalendarEvent::description() const
{
    return mManager->getEvent(mUniqueId).description;
}

void NemoCalendarEvent::setDescription(const QString &description)
{
    mManager->setDescription(mUniqueId, description);
}

QDateTime NemoCalendarEvent::startTime() const
{
    return mManager->getEvent(mUniqueId).startTime.dateTime();
}

void NemoCalendarEvent::setStartTime(const QDateTime &startTime, int spec)
{
    KDateTime::SpecType kSpec = KDateTime::LocalZone;
    if (spec == SpecClockTime) {
        kSpec = KDateTime::ClockTime;
    }

    KDateTime kStart(startTime, kSpec);
    mManager->setStartTime(mUniqueId, kStart);
}

QDateTime NemoCalendarEvent::endTime() const
{
    return mManager->getEvent(mUniqueId).endTime.dateTime();
}

void NemoCalendarEvent::setEndTime(const QDateTime &endTime, int spec)
{
    KDateTime::SpecType kSpec = KDateTime::LocalZone;
    if (spec == SpecClockTime) {
        kSpec = KDateTime::ClockTime;
    }

    KDateTime kEnd(endTime, kSpec);
    mManager->setEndTime(mUniqueId, kEnd);
}

bool NemoCalendarEvent::allDay() const
{
    return mManager->getEvent(mUniqueId).allDay;
}

void NemoCalendarEvent::setAllDay(bool allDay)
{
    mManager->setAllDay(mUniqueId, allDay);
}

NemoCalendarEvent::Recur NemoCalendarEvent::recur() const
{
    return mManager->getEvent(mUniqueId).recur;
}

void NemoCalendarEvent::setRecur(Recur r)
{
    mManager->setRecurrence(mUniqueId, r);
}

void NemoCalendarEvent::removeException(int index)
{
    if (recur() == RecurOnce)
        return;

    QList<KDateTime> exceptionDates = mManager->getEvent(mUniqueId).recurExceptionDates;
    if (index < 0 && exceptionDates.count() <= index)
        return;

    exceptionDates.removeAt(index);
    mManager->setExceptions(mUniqueId, exceptionDates);
}

void NemoCalendarEvent::addException(const QDateTime &date)
{
    if (recur() == RecurOnce) {
        qmlInfo(this) << "Cannot add exception to non-recurring event";
        return;
    }

    KDateTime kDate = KDateTime(date, KDateTime::Spec(KDateTime::LocalZone));
    QList<KDateTime> exceptionDates = mManager->getEvent(mUniqueId).recurExceptionDates;
    if (exceptionDates.contains(kDate))
        return;

    exceptionDates.append(kDate);
    mManager->setExceptions(mUniqueId, exceptionDates);
}

QDateTime NemoCalendarEvent::recurException(int index) const
{
    if (recur() != RecurOnce) {
        QList<KDateTime> list = mManager->getEvent(mUniqueId).recurExceptionDates;
        if (index >= 0 && index < list.count())
            return list.at(index).dateTime();
    }

    return QDateTime();
}

int NemoCalendarEvent::recurExceptions() const
{
    return  mManager->getEvent(mUniqueId).recurExceptionDates.count();
}

NemoCalendarEvent::Reminder NemoCalendarEvent::reminder() const
{
    return  mManager->getEvent(mUniqueId).reminder;
}

void NemoCalendarEvent::setReminder(Reminder reminder)
{
    mManager->setReminder(mUniqueId, reminder);
}

QString NemoCalendarEvent::uniqueId() const
{
    return mUniqueId;
}

QString NemoCalendarEvent::color() const
{
    return mManager->getNotebookColor(mManager->getEvent(mUniqueId).calendarUid);
}

QString NemoCalendarEvent::alarmProgram() const
{
    return mManager->getEvent(mUniqueId).alarmProgram;
}

void NemoCalendarEvent::setAlarmProgram(const QString &program)
{
    mManager->setAlarmProgram(mUniqueId, program);
}

bool NemoCalendarEvent::readonly() const
{
    return mManager->getEvent(mUniqueId).readonly;
}

QString NemoCalendarEvent::calendarUid() const
{
    return mManager->getEvent(mUniqueId).calendarUid;
}

void NemoCalendarEvent::save(const QString &calendarUid)
{
    if (mNewEvent)
        mNewEvent = false;

    mManager->saveEvent(mUniqueId, calendarUid);
}

// Removes the entire event
void NemoCalendarEvent::remove()
{
    if (!mNewEvent) {
        mManager->deleteEvent(mUniqueId);

        // TODO: this sucks
        mManager->save();
    }
}

// Returns the event as a VCalendar string
QString NemoCalendarEvent::vCalendar(const QString &prodId) const
{
    if (mUniqueId.isEmpty()) {
        qWarning() << "Event has no uid, returning empty VCalendar string."
                   << "Save event before calling this function";
        return "";
    }

    return mManager->convertEventToVEventSync(mUniqueId, prodId);
}

QString NemoCalendarEvent::location() const
{
    return mManager->getEvent(mUniqueId).location;
}

void NemoCalendarEvent::setLocation(const QString &newLocation)
{
    mManager->setLocation(mUniqueId, newLocation);
}

void NemoCalendarEvent::notebookColorChanged(QString notebookUid)
{
    if (mManager->getEvent(mUniqueId).calendarUid == notebookUid)
        emit colorChanged();
}
