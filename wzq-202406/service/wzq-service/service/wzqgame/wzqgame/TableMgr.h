#ifndef __TABLE_MGR_H__
#define __TABLE_MGR_H__

#include <map>
#include <string>
#include "AppDef.h"
using std::map;


class CTable;
class CTableMgr
{
public:
	CTableMgr();
	virtual ~CTableMgr();

	CTable* CreateTable(string tableID);
	CTable* GetTable(string tableID);
	void RemoveTable(string tableID);
	int Size();
private:
	map<string, CTable*> m_tablesMap;
};

#endif
