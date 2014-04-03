#include <QObject>
#include <QtTest>

#include "calendarapi.h"
#include "calendarevent.h"
#include "plugin.cpp"

#include <QQmlEngine>
#include <QSignalSpy>

class tst_CalendarEvent : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void initialValues();
    void setters();
    void recurExceptions();
    void cleanupTestCase();

private:
    QQmlEngine *engine;
};

void tst_CalendarEvent::initTestCase()
{
    // Create plugin, it shuts down the DB in proper order, see NemoCalendarDb::dropReferences()
    engine = new QQmlEngine(this);
    NemoCalendarPlugin* plugin = new NemoCalendarPlugin();
    plugin->initializeEngine(engine, "foobar");
}

void tst_CalendarEvent::initialValues()
{
    NemoCalendarApi calendarApi(this);
    NemoCalendarEvent *event = calendarApi.createEvent();
    QVERIFY(event != 0);

    // Check default values
    QVERIFY(!event->uniqueId().isEmpty());
    QVERIFY(event->alarmProgram().isEmpty());
    QVERIFY(event->allDay());
    QVERIFY(event->calendarUid().isEmpty());
    QVERIFY(event->color() == "black");
    QVERIFY(event->description().isEmpty());
    QVERIFY(event->displayLabel().isEmpty());
    QVERIFY(event->location().isEmpty());
    QVERIFY(!event->endTime().isValid());

    // Cannot test initial readonly value, mKCal::Notebook::isReadOnly refers null pointer if event
    // is not saved
    // QVERIFY(!event->readonly());

    QVERIFY(event->recur() == NemoCalendarEvent::RecurOnce);
    QVERIFY(event->recurExceptions() == 0);
    QVERIFY(event->reminder() == NemoCalendarEvent::ReminderNone);
    QVERIFY(!event->startTime().isValid());
}

void tst_CalendarEvent::setters() {
    NemoCalendarApi calendarApi(this);
    NemoCalendarEvent *event = calendarApi.createEvent();
    QVERIFY(event != 0);

    QSignalSpy alarmProgramSpy(event, SIGNAL(alarmProgramChanged()));
    QLatin1String alarmProgram("alarmProgram");
    event->setAlarmProgram(alarmProgram);
    QCOMPARE(alarmProgramSpy.count(), 1);
    QCOMPARE(event->alarmProgram(), alarmProgram);

    QSignalSpy allDaySpy(event, SIGNAL(allDayChanged()));
    bool allDay = !event->allDay();
    event->setAllDay(allDay);
    QCOMPARE(allDaySpy.count(), 1);
    QCOMPARE(event->allDay(), allDay);

    QSignalSpy descriptionSpy(event, SIGNAL(descriptionChanged()));
    QLatin1String description("Test event");
    event->setDescription(description);
    QCOMPARE(descriptionSpy.count(), 1);
    QCOMPARE(event->description(), description);

    QSignalSpy displayLabelSpy(event, SIGNAL(displayLabelChanged()));
    QLatin1String displayLabel("Test display label");
    event->setDisplayLabel(displayLabel);
    QCOMPARE(displayLabelSpy.count(), 1);
    QCOMPARE(event->displayLabel(), displayLabel);

    QSignalSpy locationSpy(event, SIGNAL(locationChanged()));
    QLatin1String location("Test location");
    event->setLocation(location);
    QCOMPARE(locationSpy.count(), 1);
    QCOMPARE(event->location(), location);

    QSignalSpy endTimeSpy(event, SIGNAL(endTimeChanged()));
    QDateTime endTime = QDateTime::currentDateTime();
    event->setEndTime(endTime, NemoCalendarEvent::SpecLocalZone);
    QCOMPARE(endTimeSpy.count(), 1);
    QCOMPARE(event->endTime(), endTime);

    QSignalSpy recurSpy(event, SIGNAL(recurChanged()));
    NemoCalendarEvent::Recur recur = NemoCalendarEvent::RecurDaily; // Default value is RecurOnce
    event->setRecur(recur);
    QCOMPARE(recurSpy.count(), 1);
    QCOMPARE(event->recur(), recur);

    QSignalSpy reminderSpy(event, SIGNAL(reminderChanged()));
    NemoCalendarEvent::Reminder reminder = NemoCalendarEvent::ReminderTime; // default is ReminderNone
    event->setReminder(reminder);
    QCOMPARE(reminderSpy.count(), 1);
    QCOMPARE(event->reminder(), reminder);

    QSignalSpy startTimeSpy(event, SIGNAL(startTimeChanged()));
    QDateTime startTime = QDateTime::currentDateTime();
    event->setStartTime(startTime, NemoCalendarEvent::SpecLocalZone);
    QCOMPARE(startTimeSpy.count(), 1);
    QCOMPARE(event->startTime(), startTime);


    // TODO: test SpecClockTime?

    // save, set color for notebook
 //   QSignalSpy colorSpy(event, SIGNAL(colorChanged());
 //   QLatin1String color("blue");

}

void tst_CalendarEvent::recurExceptions()
{
    NemoCalendarApi calendarApi(this);
    NemoCalendarEvent *event = calendarApi.createEvent();
    QVERIFY(event != 0);

    event->setStartTime(QDateTime::currentDateTime(), NemoCalendarEvent::SpecLocalZone);
    event->setEndTime(QDateTime::currentDateTime().addSecs(3600), NemoCalendarEvent::SpecLocalZone);
    QCOMPARE(event->recurExceptions(), 0);
    QCOMPARE(event->recur(), NemoCalendarEvent::RecurOnce);
    QSignalSpy recurExceptionSpy(event, SIGNAL(recurExceptionsChanged()));

    // Add invalid recur exception date to a non-recurring event, nothing should happen
    event->addException(QDateTime());
    QCOMPARE(recurExceptionSpy.count(), 0);

    // Add valid recur exception date to a non-recurring event, nothing should happen
    event->addException(event->startTime().addDays(1));
    QCOMPARE(recurExceptionSpy.count(), 0);

    event->setRecur(NemoCalendarEvent::RecurDaily);
    // Add invalid recur exception date to a recurring event, nothing should happen
    event->addException(QDateTime());
    QCOMPARE(recurExceptionSpy.count(), 0);

    // Add valid recur exception date to a recurring event
    QDateTime exceptionDateTimeA = event->startTime().addDays(1);
    event->addException(exceptionDateTimeA);
    QCOMPARE(recurExceptionSpy.count(), 1);
    QCOMPARE(event->recurExceptions(), 1);
    QCOMPARE(event->recurException(0), exceptionDateTimeA);

    // Add a second recur exception
    QDateTime exceptionDateTimeB = event->startTime().addDays(2);
    event->addException(exceptionDateTimeB);
    QCOMPARE(recurExceptionSpy.count(), 2);
    QCOMPARE(event->recurExceptions(), 2);
    QCOMPARE(event->recurException(0), exceptionDateTimeA);
    QCOMPARE(event->recurException(1), exceptionDateTimeB);

    // Request non-valid recurException, valid index are 0,1.
    QVERIFY(!event->recurException(-1).isValid());
    QVERIFY(!event->recurException(17).isValid());

    // Remove invalid recur exception, valid index are 0,1.
    event->removeException(-7);
    QCOMPARE(event->recurExceptions(), 2);
    QCOMPARE(recurExceptionSpy.count(), 2);
    event->removeException(4);
    QCOMPARE(event->recurExceptions(), 2);
    QCOMPARE(recurExceptionSpy.count(), 2);

    // Remove valid recur exception
    event->removeException(0);
    QCOMPARE(recurExceptionSpy.count(), 3);
    QCOMPARE(event->recurException(0), exceptionDateTimeB);
}



// test recurexceptions: add exception and check that the list changes

// save alarm without notebook: default notebook used
// save alarm with notebook: correct notebook used
// save alarm with all day
// save alarm with reminder(s)
// save alarms with invalid start date?

void tst_CalendarEvent::cleanupTestCase()
{
    //NemoCalendarDb::dropReferences();
}

#include "tst_calendarevent.moc"
QTEST_MAIN(tst_CalendarEvent)
