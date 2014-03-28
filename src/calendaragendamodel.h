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

#ifndef CALENDARAGENDAMODEL_H
#define CALENDARAGENDAMODEL_H

#include <QDate>
#include <QAbstractListModel>
#include <QQmlParserStatus>

class NemoCalendarEvent;
class NemoCalendarEventOccurrence;

class NemoCalendarAgendaModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QDate startDate READ startDate WRITE setStartDate NOTIFY startDateChanged)
    Q_PROPERTY(QDate endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)

public:
    enum {
        EventObjectRole = Qt::UserRole,
        OccurrenceObjectRole,
        SectionBucketRole
    };

    explicit NemoCalendarAgendaModel(QObject *parent = 0);
    virtual ~NemoCalendarAgendaModel();

    QDate startDate() const;
    void setStartDate(const QDate &startDate);

    QDate endDate() const;
    void setEndDate(const QDate &endDate);

    int count() const;

    // NemoCalendarAgendaModel takes ownership of the NemoCalendarEventOccurrence objects
    void doRefresh(QList<NemoCalendarEventOccurrence *>, bool reset = false);

    int rowCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;

    virtual void classBegin();
    virtual void componentComplete();
signals:
    void countChanged();
    void startDateChanged();
    void endDateChanged();

protected:
    virtual QHash<int, QByteArray> roleNames() const;

private slots:
    void refresh();

private:
    QDate mStartDate;
    QDate mEndDate;
    QList<NemoCalendarEventOccurrence *> mEvents;

    bool mIsComplete;
};

#endif // CALENDARAGENDAMODEL_H
