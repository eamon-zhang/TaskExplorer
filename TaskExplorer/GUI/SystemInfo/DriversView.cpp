#include "stdafx.h"
#include "../TaskExplorer.h"
#include "DriversView.h"
#include "../../Common/Common.h"
#ifdef WIN32
#include "../../API/Windows/WinDriver.h"		
#endif
#include "..\..\Common\SortFilterProxyModel.h"

CDriversView::CDriversView(QWidget *parent)
	:CPanelView(parent)
{
	m_pMainLayout = new QHBoxLayout();
	m_pMainLayout->setMargin(0);
	this->setLayout(m_pMainLayout);

	m_pDriverModel = new CDriverModel();
	
	m_pSortProxy = new CSortFilterProxyModel(false, this);
	m_pSortProxy->setSortRole(Qt::EditRole);
    m_pSortProxy->setSourceModel(m_pDriverModel);
	m_pSortProxy->setDynamicSortFilter(true);


	// Driver List
	m_pDriverList = new QTreeViewEx();
	m_pDriverList->setItemDelegate(new CStyledGridItemDelegate(m_pDriverList->fontMetrics().height() + 3, this));

	m_pDriverList->setModel(m_pSortProxy);

	m_pDriverList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_pDriverList->setSortingEnabled(true);

	m_pDriverList->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_pDriverList, SIGNAL(customContextMenuRequested( const QPoint& )), this, SLOT(OnMenu(const QPoint &)));

	m_pMainLayout->addWidget(m_pDriverList);
	// 

	//m_pMenu = new QMenu();
	AddPanelItemsToMenu();

	connect(theAPI, SIGNAL(DriverListUpdated(QSet<QString>, QSet<QString>, QSet<QString>)), this, SLOT(OnDriverListUpdated(QSet<QString>, QSet<QString>, QSet<QString>)));
}


CDriversView::~CDriversView()
{

}


void CDriversView::OnDriverListUpdated(QSet<QString> Added, QSet<QString> Changed, QSet<QString> Removed)
{
	QMap<QString, CDriverPtr> DriverList = theAPI->GetDriverList();
	
	m_pDriverModel->Sync(DriverList);
}