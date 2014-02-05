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

#ifndef CALENDAREVENTCACHE_H
#define CALENDAREVENTCACHE_H

// Qt
#include <QSet>
#include <QObject>
#include <QFutureWatcher>

// mkcal
#include <event.h>
#include <extendedstorage.h>

class NemoCalendarEvent;
class NemoCalendarAgendaModel;
class NemoCalendarEventOccurrence;
class NemoCalendarEventCache : public QObject, public mKCal::ExtendedStorageObserver
{
    Q_OBJECT
private:
    NemoCalendarEventCache();

public:
    static NemoCalendarEventCache *instance();


    /* mKCal::ExtendedStorageObserver */
    void storageModified(mKCal::ExtendedStorage *storage, const QString &info);
    void storageProgress(mKCal::ExtendedStorage *storage, const QString &info);
    void storageFinished(mKCal::ExtendedStorage *storage, bool error, const QString &info);

    QString notebookColor(const QString &) const;
    void setNotebookColor(const QString &, const QString &);

    static QList<NemoCalendarEvent *> events(const KCalCore::Event::Ptr &event);

    void init();
    void update();
    void setStartDate(const QDate &date);
    void setEndDate(const QDate &date);
    int eventCountForDate(const QDate &date);

public slots:
    void handleFinished();

protected:
    virtual bool event(QEvent *);

signals:
    void modelReset();

private:
    friend class NemoCalendarApi;
    friend class NemoCalendarEvent;
    friend class NemoCalendarAgendaModel;
    friend class NemoCalendarEventOccurrence;

    void scheduleAgendaRefresh(NemoCalendarAgendaModel *);
    void cancelAgendaRefresh(NemoCalendarAgendaModel *);
    void doAgendaRefresh();
    void load();

    QStringList mDefaultNotebookColors;

    QSet<QString> mNotebooks;
    QHash<QString, QString> mNotebookColors;
    QSet<NemoCalendarEvent *> mEvents;
    QSet<NemoCalendarEventOccurrence *> mEventOccurrences;

    bool mRefreshEventSent;
    QSet<NemoCalendarAgendaModel *> mRefreshModels;
    QFutureWatcher<void>* mFutureWatcher;
    bool mInitComplete;
    QDate mStartDate;
    QDate mEndDate;
};

#endif // CALENDAREVENTCACHE_H
