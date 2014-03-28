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

#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include <QObject>
#include <QDateTime>

// mkcal
#include <event.h>

class NemoCalendarEvent : public QObject
{
    Q_OBJECT
    Q_ENUMS(Recur)
    Q_ENUMS(Reminder)
    Q_ENUMS(TimeSpec)

    Q_PROPERTY(QString displayLabel READ displayLabel WRITE setDisplayLabel NOTIFY displayLabelChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QDateTime startTime READ startTime NOTIFY startTimeChanged)
    Q_PROPERTY(QDateTime endTime READ endTime NOTIFY endTimeChanged)
    Q_PROPERTY(bool allDay READ allDay WRITE setAllDay NOTIFY allDayChanged)
    Q_PROPERTY(Recur recur READ recur WRITE setRecur NOTIFY recurChanged)
    Q_PROPERTY(int recurExceptions READ recurExceptions NOTIFY recurExceptionsChanged)
    Q_PROPERTY(Reminder reminder READ reminder WRITE setReminder NOTIFY reminderChanged)
    Q_PROPERTY(QString uniqueId READ uniqueId CONSTANT)
    Q_PROPERTY(QString color READ color NOTIFY colorChanged)
    Q_PROPERTY(QString alarmProgram READ alarmProgram WRITE setAlarmProgram NOTIFY alarmProgramChanged)
    Q_PROPERTY(bool readonly READ readonly CONSTANT)
    Q_PROPERTY(QString calendarUid READ calendarUid NOTIFY calendarUidChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)

public:
    enum Recur {
        RecurOnce,
        RecurDaily,
        RecurWeekly,
        RecurBiweekly,
        RecurMonthly,
        RecurYearly,
        RecurCustom
    };

    enum Reminder {
        ReminderNone,
        ReminderTime,
        Reminder5Min,
        Reminder15Min,
        Reminder30Min,
        Reminder1Hour,
        Reminder2Hour,
        Reminder1Day,
        Reminder2Day
    };

    enum TimeSpec {
        SpecLocalZone,
        SpecClockTime
    };

    explicit NemoCalendarEvent(QObject *parent = 0);
    NemoCalendarEvent(const KCalCore::Event::Ptr &event, QObject *parent = 0);
    ~NemoCalendarEvent();

    QString displayLabel() const;
    void setDisplayLabel(const QString &displayLabel);

    QString description() const;
    void setDescription(const QString &description);

    QDateTime startTime() const;
    Q_INVOKABLE void setStartTime(const QDateTime &startTime, int spec);

    QDateTime endTime() const;
    Q_INVOKABLE void setEndTime(const QDateTime &endTime, int spec);

    bool allDay() const;
    void setAllDay(bool);

    Recur recur() const;
    void setRecur(Recur);

    int recurExceptions() const;
    Q_INVOKABLE void removeException(int);
    Q_INVOKABLE void addException(const QDateTime &);
    Q_INVOKABLE QDateTime recurException(int) const;

    Reminder reminder() const;
    void setReminder(Reminder);

    QString uniqueId() const;

    QString color() const;

    QString alarmProgram() const;
    void setAlarmProgram(const QString &);

    bool readonly() const;
    QString calendarUid() const;

    Q_INVOKABLE void save(const QString &calendarUid = QString());
    Q_INVOKABLE void remove();
    Q_INVOKABLE QString vCalendar(const QString &prodId = QString()) const;

    const KCalCore::Event::Ptr &event() const;
    void setEvent(const KCalCore::Event::Ptr &);

    QString location() const;
    void setLocation(const QString &newLocation);

signals:
    void displayLabelChanged();
    void descriptionChanged();
    void startTimeChanged();
    void endTimeChanged();
    void allDayChanged();
    void recurChanged();
    void recurExceptionsChanged();
    void reminderChanged();
    void alarmProgramChanged();
    void colorChanged();
    void calendarUidChanged();
    void locationChanged();

private:
    bool mNewEvent;
    KCalCore::Event::Ptr mEvent;
};

#endif // CALENDAREVENT_H
