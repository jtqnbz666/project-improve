#include "TableMgr.h"
#include "AppDef.h"
#include "GameService.h"
#include "Table.h"
#include "log.h"

extern CGameService* g_pService ;

CTableMgr::CTableMgr()
{
}

CTableMgr::~CTableMgr()
{
}

CTable* CTableMgr::CreateTable(string tableID)
{
    mcgWriteLog("CTableMgr::CreateTable : %s",tableID.c_str());
	CTable* pTable = NULL;
	map<string, CTable*>::iterator iter = m_tablesMap.find(tableID);
	if (iter != m_tablesMap.end())
	{
        mcgWriteLog("CTableMgr::CreateTable : %s 2",tableID.c_str());
		pTable = iter->second;
	}
	else
	{
        mcgWriteLog("CTableMgr::CreateTable : %s 3\n",tableID.c_str());
		pTable = new CTable(tableID);
		if (!pTable->Init())
		{
            mcgWriteLog("CTableMgr::CreateTable : %s 5",tableID.c_str());
			delete pTable;
			return NULL;
		}
         mcgWriteLog("CTableMgr::CreateTable : %s 4\n",tableID.c_str());
		m_tablesMap[tableID] = pTable;
	}

	return pTable;
}

CTable* CTableMgr::GetTable(string tableID)
{
	CTable* pTable = NULL;
	map<string, CTable*>::iterator iter = m_tablesMap.find(tableID);
	if (iter != m_tablesMap.end())
	{
		pTable = iter->second;
	}

	return pTable;
}

void CTableMgr::RemoveTable(string tableID)
{
	map<string, CTable*>::iterator iter = m_tablesMap.find(tableID);
	if (iter != m_tablesMap.end())
	{
		m_tablesMap.erase(iter);
	}
}

int CTableMgr::Size()
{
	return int(m_tablesMap.size());
}
