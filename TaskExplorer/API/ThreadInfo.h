#pragma once
#include <qobject.h>
#include "StackTrace.h"
#include "AbstractTask.h"

class CThreadInfo: public CAbstractTask
{
	Q_OBJECT

public:
	CThreadInfo(QObject *parent = nullptr);
	virtual ~CThreadInfo();

	virtual quint64 GetThreadId()	const			{ QReadLocker Locker(&m_Mutex); return m_ThreadId; }
	virtual quint64 GetProcessId()	const			{ QReadLocker Locker(&m_Mutex); return m_ProcessId; }

	virtual QString GetStartAddressString() const = 0;
	virtual int GetState()	const					{ QReadLocker Locker(&m_Mutex); return m_State; }
	virtual quint64 GetWaitState()	const			{ QReadLocker Locker(&m_Mutex); return ((quint64)m_State) | ((quint64)m_WaitReason << 32); }
	virtual QString GetStateString() const = 0;
	virtual int GetWaitReason()	const				{ QReadLocker Locker(&m_Mutex); return m_WaitReason; }

	virtual void SetMainThread(bool bSet = true)	{ QWriteLocker Locker(&m_Mutex); m_IsMainThread = bSet; }
	virtual bool IsMainThread() const				{ QReadLocker Locker(&m_Mutex); return m_IsMainThread; }

	virtual STaskStats GetCpuStats() const			{ QReadLocker Locker(&m_StatsMutex); return m_CpuStats; }

	virtual QSharedPointer<QObject>	GetProcess() const;
	//virtual QSharedPointer<QObject>	GetProcess() const { QReadLocker Locker(&m_Mutex); return m_pProcess; }
	virtual void SetProcess(QSharedPointer<QObject> pProcess) { QWriteLocker Locker(&m_Mutex); m_pProcess = pProcess; }

public slots:
	virtual quint64 TraceStack() = 0;

signals:
	void			StackTraced(const CStackTracePtr& StackTrace);

protected:
	quint64			m_ThreadId;
	quint64			m_ProcessId;

	bool			m_IsMainThread;

	int				m_State;
	int				m_WaitReason;

	QSharedPointer<QObject>	m_pProcess;

	STaskStats		m_CpuStats;
};

typedef QSharedPointer<CThreadInfo> CThreadPtr;
typedef QWeakPointer<CThreadInfo> CThreadRef;