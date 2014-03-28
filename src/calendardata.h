#ifndef NEMOCALENDARDATA_H
#define NEMOCALENDARDATA_H

#include <QString>

#include "calendarevent.h"

namespace NemoCalendarData {

struct EventOccurence {
    QString eventUid;
    QDateTime startTime;
    QDateTime endTime;

    QString getId() const
    {
        QDateTime tmp(startTime.date());
        return QString("%1-%2").arg(eventUid).arg(tmp.toMSecsSinceEpoch());
    }
};

struct Event {
    QString displayLabel;
    QString description;
    KDateTime startTime;
    KDateTime endTime;
    bool allDay;
    NemoCalendarEvent::Recur recur;
    QList<KDateTime> recurExceptionDates;
    NemoCalendarEvent::Reminder reminder;
    QString uniqueId;
    QString alarmProgram;
    bool readonly;
    QString location;
    QString calendarUid;

    bool operator==(const Event& other) const
    {
        return uniqueId == other.uniqueId;
    }
};

struct Notebook {
    QString name;
    QString uid;
    QString description;
    QString color;
    bool isDefault;
    bool readOnly;
    bool localCalendar;
    bool excluded;

    bool operator==(const Notebook other) const
    {
        return uid == other.uid && name == other.name && description == other.description &&
                color == other.color && isDefault == other.isDefault && readOnly == other.readOnly &&
                localCalendar == other.localCalendar && excluded == other.excluded;
    }

    bool operator!=(const Notebook other) const
    {
        return !operator==(other);
    }
};

typedef QPair<QDate,QDate> Range;

}
#endif // NEMOCALENDARDATA_H
