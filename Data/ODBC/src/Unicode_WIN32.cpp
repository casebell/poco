//
// Unicode.cpp
//
// $Id: //poco/Main/Data/ODBC/src/Unicode_WIN32.cpp#3 $
//
// Library: ODBC
// Package: ODBC
// Module:  Unicode_WIN32
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/ODBC/Unicode_WIN32.h"
#include "Poco/Buffer.h"
#include "Poco/Exception.h"


using Poco::Buffer;
using Poco::InvalidArgumentException;
using Poco::NotImplementedException;


namespace Poco {
namespace Data {
namespace ODBC {


SQLRETURN SQLColAttribute(SQLHSTMT hstmt,
	SQLUSMALLINT   iCol,
	SQLUSMALLINT   iField,
	SQLPOINTER	   pCharAttr,
	SQLSMALLINT	   cbCharAttrMax,	
	SQLSMALLINT*   pcbCharAttr,
	NumAttrPtrType pNumAttr)
{
	if (isString(pCharAttr, cbCharAttrMax))
	{
		Buffer<wchar_t> buffer(stringLength(pCharAttr, cbCharAttrMax));

		SQLRETURN rc = SQLColAttributeW(hstmt,
			iCol,
			iField,
			buffer.begin(),
			(SQLSMALLINT) buffer.size() * sizeof(wchar_t),	
			pcbCharAttr,
			pNumAttr);

		makeUTF8(buffer, *pcbCharAttr, pCharAttr, cbCharAttrMax);
		return rc;
	}

	return SQLColAttributeW(hstmt,
		iCol,
		iField,
		pCharAttr,
		cbCharAttrMax,	
		pcbCharAttr,
		pNumAttr);
}


SQLRETURN SQLColAttributes(SQLHSTMT hstmt,
	SQLUSMALLINT icol,
	SQLUSMALLINT fDescType,
	SQLPOINTER   rgbDesc,
	SQLSMALLINT  cbDescMax,
	SQLSMALLINT* pcbDesc,
	SQLLEN*      pfDesc)
{
	return SQLColAttribute(hstmt,
		icol,
		fDescType,
		rgbDesc,
		cbDescMax,
		pcbDesc,
		pfDesc);
}


SQLRETURN SQLConnect(SQLHDBC hdbc,
	SQLCHAR*    szDSN,
	SQLSMALLINT cbDSN,
	SQLCHAR*    szUID,
	SQLSMALLINT cbUID,
	SQLCHAR*    szAuthStr,
	SQLSMALLINT cbAuthStr)
{
	std::wstring sqlDSN;
	makeUTF16(szDSN, cbDSN, sqlDSN);

	std::wstring sqlUID;
	makeUTF16(szUID, cbUID, sqlUID);

	std::wstring sqlPWD;
	makeUTF16(szAuthStr, cbAuthStr, sqlPWD);
	
	return SQLConnectW(hdbc,
		(SQLWCHAR*) sqlDSN.c_str(),
		(SQLSMALLINT) sqlDSN.size(),
		(SQLWCHAR*) sqlUID.c_str(),
		(SQLSMALLINT) sqlUID.size(),
		(SQLWCHAR*) sqlPWD.c_str(),
		(SQLSMALLINT) sqlPWD.size());
}


SQLRETURN SQLDescribeCol(SQLHSTMT hstmt,
	SQLUSMALLINT icol,
	SQLCHAR*     szColName,
	SQLSMALLINT  cbColNameMax,
	SQLSMALLINT* pcbColName,
	SQLSMALLINT* pfSqlType,
	SQLULEN*     pcbColDef,
	SQLSMALLINT* pibScale,
	SQLSMALLINT* pfNullable)
{
	Buffer<wchar_t> buffer(cbColNameMax);
	SQLRETURN rc = SQLDescribeColW(hstmt,
		icol,
		(SQLWCHAR*) buffer.begin(),
		(SQLSMALLINT) buffer.size(),
		pcbColName,
		pfSqlType,
		pcbColDef,
		pibScale,
		pfNullable);

	makeUTF8(buffer, *pcbColName, szColName, cbColNameMax);
	return rc;
}


SQLRETURN SQLError(SQLHENV henv,
	SQLHDBC      hdbc,
	SQLHSTMT     hstmt,
	SQLCHAR*     szSqlState,
	SQLINTEGER*  pfNativeError,
	SQLCHAR*     szErrorMsg,
	SQLSMALLINT  cbErrorMsgMax,
	SQLSMALLINT* pcbErrorMsg)
{
	throw NotImplementedException("SQLError is obsolete. "
		"Use SQLGetDiagRec instead.");
}


SQLRETURN SQLExecDirect(SQLHSTMT hstmt,
	SQLCHAR*   szSqlStr,
	SQLINTEGER cbSqlStr)
{
	std::wstring sqlStr;
	makeUTF16(szSqlStr, cbSqlStr, sqlStr);

	return SQLExecDirectW(hstmt, 
		(SQLWCHAR*) sqlStr.c_str(), 
		(SQLINTEGER) sqlStr.size());
}


SQLRETURN SQLGetConnectAttr(SQLHDBC hdbc,
	SQLINTEGER  fAttribute,
	SQLPOINTER  rgbValue,
	SQLINTEGER  cbValueMax,
	SQLINTEGER* pcbValue)
{
	if (isString(rgbValue, cbValueMax))
	{
		Buffer<wchar_t> buffer(stringLength(rgbValue, cbValueMax));

		SQLRETURN rc = SQLGetConnectAttrW(hdbc,
				fAttribute,
				buffer.begin(),
				(SQLINTEGER) buffer.size() * sizeof(wchar_t),
				pcbValue);

		makeUTF8(buffer, *pcbValue, rgbValue, cbValueMax);
		return rc;
	}
	

	return SQLGetConnectAttrW(hdbc,
		fAttribute,
		rgbValue,
		cbValueMax,
		pcbValue);
}


SQLRETURN SQLGetCursorName(SQLHSTMT hstmt,
	SQLCHAR*     szCursor,
	SQLSMALLINT  cbCursorMax,
	SQLSMALLINT* pcbCursor)
{
	throw NotImplementedException("Not implemented");
}


SQLRETURN SQLSetDescField(SQLHDESC hdesc,
	SQLSMALLINT iRecord, 
	SQLSMALLINT iField,
	SQLPOINTER  rgbValue, 
	SQLINTEGER  cbValueMax)
{
	if (isString(rgbValue, cbValueMax))
	{
		std::wstring str;
		makeUTF16((SQLCHAR*) rgbValue, cbValueMax, str);

		SQLRETURN rc = SQLSetDescFieldW(hdesc,
			iRecord, 
			iField,
			(SQLPOINTER) str.c_str(), 
			(SQLINTEGER) str.size() * sizeof(std::wstring::value_type));

		return rc;
	}

	return SQLSetDescFieldW(hdesc,
		iRecord, 
		iField,
		rgbValue, 
		cbValueMax);
}


SQLRETURN SQLGetDescField(SQLHDESC hdesc,
	SQLSMALLINT iRecord,
	SQLSMALLINT iField,
	SQLPOINTER  rgbValue,
	SQLINTEGER	cbValueMax,
	SQLINTEGER* pcbValue)
{
	if (isString(rgbValue, cbValueMax))
	{
		Buffer<wchar_t> buffer(stringLength(rgbValue, cbValueMax));

		SQLRETURN rc = SQLGetDescFieldW(hdesc,
			iRecord,
			iField,
			buffer.begin(),
			(SQLINTEGER) buffer.size() * sizeof(wchar_t),
			pcbValue);

		makeUTF8(buffer, *pcbValue, rgbValue, cbValueMax);
		return rc;
	}

	return SQLGetDescFieldW(hdesc,
		iRecord,
		iField,
		rgbValue,
		cbValueMax,
		pcbValue);
}


SQLRETURN SQLGetDescRec(SQLHDESC hdesc,
	SQLSMALLINT  iRecord,
	SQLCHAR*     szName,
	SQLSMALLINT  cbNameMax,
	SQLSMALLINT* pcbName,
	SQLSMALLINT* pfType,
	SQLSMALLINT* pfSubType,
	SQLLEN*      pLength,
	SQLSMALLINT* pPrecision, 
	SQLSMALLINT* pScale,
	SQLSMALLINT* pNullable)
{
	throw NotImplementedException();
}


SQLRETURN SQLGetDiagField(SQLSMALLINT fHandleType,
	SQLHANDLE    handle,
	SQLSMALLINT  iRecord,
	SQLSMALLINT  fDiagField,
	SQLPOINTER   rgbDiagInfo,
	SQLSMALLINT  cbDiagInfoMax,
	SQLSMALLINT* pcbDiagInfo)
{
	if (isString(rgbDiagInfo, cbDiagInfoMax))
	{
		Buffer<wchar_t> buffer(stringLength(rgbDiagInfo, cbDiagInfoMax));

		SQLRETURN rc = SQLGetDiagFieldW(fHandleType,
			handle,
			iRecord,
			fDiagField,
			buffer.begin(),
			(SQLSMALLINT) buffer.size() * sizeof(wchar_t),
			pcbDiagInfo);

		makeUTF8(buffer, *pcbDiagInfo, rgbDiagInfo, cbDiagInfoMax);
		return rc;
	}

	return SQLGetDiagFieldW(fHandleType,
		handle,
		iRecord,
		fDiagField,
		rgbDiagInfo,
		cbDiagInfoMax,
		pcbDiagInfo);
}


SQLRETURN SQLGetDiagRec(SQLSMALLINT fHandleType,
	SQLHANDLE    handle,
	SQLSMALLINT  iRecord,
	SQLCHAR*     szSqlState,
	SQLINTEGER*  pfNativeError,
	SQLCHAR*     szErrorMsg,
	SQLSMALLINT  cbErrorMsgMax,
	SQLSMALLINT* pcbErrorMsg)
{
	const SQLINTEGER stateLen = SQL_SQLSTATE_SIZE + 1;
	Buffer<wchar_t> bufState(stateLen);
	Buffer<wchar_t> bufErr(cbErrorMsgMax);

	SQLRETURN rc = SQLGetDiagRecW(fHandleType,
		handle,
		iRecord,
		bufState.begin(),
		pfNativeError,
		bufErr.begin(),
		(SQLSMALLINT) bufErr.size(),
		pcbErrorMsg);

	makeUTF8(bufState, stateLen, szSqlState, stateLen);
	makeUTF8(bufErr, *pcbErrorMsg, szErrorMsg, cbErrorMsgMax);

	return rc;
}


SQLRETURN SQLPrepare(SQLHSTMT hstmt,
	SQLCHAR*   szSqlStr,
	SQLINTEGER cbSqlStr)
{
	std::wstring sqlStr;
	makeUTF16(szSqlStr, cbSqlStr, sqlStr);

	return SQLPrepareW(hstmt, 
		(SQLWCHAR*) sqlStr.c_str(), 
		(SQLINTEGER) sqlStr.size());
}


SQLRETURN SQLSetConnectAttr(SQLHDBC hdbc,
	SQLINTEGER fAttribute,
	SQLPOINTER rgbValue,
	SQLINTEGER cbValue)
{
	if (isString(rgbValue, cbValue))
	{
		std::wstring str;
		makeUTF16((SQLCHAR*) rgbValue, cbValue, str);

		return SQLSetConnectAttrW(hdbc,
			fAttribute,
			(SQLWCHAR*) str.c_str(), 
			(SQLINTEGER) str.size() * sizeof(std::wstring::value_type));
	}

	return SQLSetConnectAttrW(hdbc,
		fAttribute,
		rgbValue, 
		cbValue);
}


SQLRETURN SQLSetCursorName(SQLHSTMT hstmt,
	SQLCHAR*    szCursor,
	SQLSMALLINT cbCursor)
{
	throw NotImplementedException("Not implemented");
}


SQLRETURN SQLSetStmtAttr(SQLHSTMT hstmt,
	SQLINTEGER fAttribute,
	SQLPOINTER rgbValue,
	SQLINTEGER cbValueMax)
{
	if (isString(rgbValue, cbValueMax))
	{
		std::wstring str;
		makeUTF16((SQLCHAR*) rgbValue, cbValueMax, str);

		return SQLSetStmtAttrW(hstmt,
			fAttribute,
			(SQLPOINTER) str.c_str(),
			(SQLINTEGER) str.size());
	}

	return SQLSetStmtAttrW(hstmt,
		fAttribute,
		rgbValue,
		cbValueMax);
}


SQLRETURN SQLGetStmtAttr(SQLHSTMT hstmt,
	SQLINTEGER  fAttribute,
	SQLPOINTER  rgbValue,
	SQLINTEGER  cbValueMax,
	SQLINTEGER* pcbValue)
{
	if (isString(rgbValue, cbValueMax))
	{
		Buffer<wchar_t> buffer(stringLength(rgbValue, cbValueMax));

		return SQLGetStmtAttrW(hstmt,
			fAttribute,
			(SQLPOINTER) buffer.begin(),
			(SQLINTEGER) buffer.size() * sizeof(wchar_t),
			pcbValue);
	}

	return SQLGetStmtAttrW(hstmt,
			fAttribute,
			rgbValue,
			cbValueMax,
			pcbValue);
}


SQLRETURN SQLColumns(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szTableName,
	SQLSMALLINT cbTableName,
	SQLCHAR*    szColumnName,
	SQLSMALLINT cbColumnName)
{
	throw NotImplementedException();
}


SQLRETURN SQLGetConnectOption(SQLHDBC hdbc,
	SQLUSMALLINT fOption,
	SQLPOINTER   pvParam)
{
	throw NotImplementedException();
}


SQLRETURN SQLGetInfo(SQLHDBC hdbc,
	SQLUSMALLINT fInfoType,
	SQLPOINTER   rgbInfoValue,
	SQLSMALLINT  cbInfoValueMax,
	SQLSMALLINT* pcbInfoValue)
{
	if (cbInfoValueMax)
	{
		Buffer<wchar_t> buffer(cbInfoValueMax);

		SQLRETURN rc = SQLGetInfoW(hdbc,
			fInfoType,
			(SQLPOINTER) buffer.begin(),
			(SQLSMALLINT) buffer.size() * sizeof(wchar_t),
			pcbInfoValue);

		makeUTF8(buffer, *pcbInfoValue, rgbInfoValue, cbInfoValueMax);

		return rc;
	}

	return SQLGetInfoW(hdbc,
		fInfoType,
		rgbInfoValue,
		cbInfoValueMax,
		pcbInfoValue);
}


SQLRETURN SQLGetTypeInfo(SQLHSTMT StatementHandle, SQLSMALLINT DataType)
{
	return SQLGetTypeInfoW(StatementHandle, DataType);
}


SQLRETURN SQLSetConnectOption(SQLHDBC hdbc,
	SQLUSMALLINT fOption,
	SQLULEN      vParam)
{
	throw NotImplementedException();
}


SQLRETURN SQLSpecialColumns(SQLHSTMT hstmt,
	SQLUSMALLINT fColType,
	SQLCHAR*     szCatalogName,
	SQLSMALLINT  cbCatalogName,
	SQLCHAR*     szSchemaName,
	SQLSMALLINT  cbSchemaName,
	SQLCHAR*     szTableName,
	SQLSMALLINT  cbTableName,
	SQLUSMALLINT fScope,
	SQLUSMALLINT fNullable)
{
	throw NotImplementedException();
}


SQLRETURN SQLStatistics(SQLHSTMT hstmt,
	SQLCHAR*     szCatalogName,
	SQLSMALLINT  cbCatalogName,
	SQLCHAR*     szSchemaName,
	SQLSMALLINT  cbSchemaName,
	SQLCHAR*     szTableName,
	SQLSMALLINT  cbTableName,
	SQLUSMALLINT fUnique,
	SQLUSMALLINT fAccuracy)
{
	throw NotImplementedException();
}


SQLRETURN SQLTables(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szTableName,
	SQLSMALLINT cbTableName,
	SQLCHAR*    szTableType,
	SQLSMALLINT cbTableType)
{
	throw NotImplementedException();
}


SQLRETURN SQLDataSources(SQLHENV henv,
	SQLUSMALLINT fDirection,
	SQLCHAR*     szDSN,
	SQLSMALLINT  cbDSNMax,
	SQLSMALLINT* pcbDSN,
	SQLCHAR*     szDesc,
	SQLSMALLINT  cbDescMax,
	SQLSMALLINT* pcbDesc)
{
	Buffer<wchar_t> bufDSN(cbDSNMax);
	Buffer<wchar_t> bufDesc(cbDescMax);

	SQLRETURN rc = SQLDataSourcesW(henv,
		fDirection,
		bufDSN.begin(),
		(SQLSMALLINT) bufDSN.size(),
		pcbDSN,
		bufDesc.begin(),
		(SQLSMALLINT) bufDesc.size(),
		pcbDesc);

	makeUTF8(bufDSN, *pcbDSN, szDSN, cbDSNMax);
	makeUTF8(bufDesc, *pcbDesc, szDesc, cbDescMax);

	return rc;
}


SQLRETURN SQLDriverConnect(SQLHDBC hdbc,
	SQLHWND      hwnd,
	SQLCHAR*     szConnStrIn,
	SQLSMALLINT  cbConnStrIn,
	SQLCHAR*     szConnStrOut,
	SQLSMALLINT  cbConnStrOutMax,
	SQLSMALLINT* pcbConnStrOut,
	SQLUSMALLINT fDriverCompletion)
{
	std::wstring connStrIn;
	int len = cbConnStrIn;
	if (SQL_NTS == len) 
		len = (int) std::strlen((const char*) szConnStrIn);

	Poco::UnicodeConverter::toUTF16((const char *) szConnStrIn, len, connStrIn);

	Buffer<wchar_t> bufOut(cbConnStrOutMax);
	SQLRETURN rc = SQLDriverConnectW(hdbc,
		hwnd,
		(SQLWCHAR*) connStrIn.c_str(),
		(SQLSMALLINT) connStrIn.size(),
		bufOut.begin(),
		(SQLSMALLINT) bufOut.size(),
		pcbConnStrOut,
		fDriverCompletion);

	makeUTF8(bufOut, *pcbConnStrOut, szConnStrOut, cbConnStrOutMax);

	return rc;
}


SQLRETURN SQLBrowseConnect(SQLHDBC hdbc,
	SQLCHAR*     szConnStrIn,
	SQLSMALLINT  cbConnStrIn,
	SQLCHAR*     szConnStrOut,
	SQLSMALLINT  cbConnStrOutMax,
	SQLSMALLINT* pcbConnStrOut)
{
	std::wstring str;
	makeUTF16(szConnStrIn, cbConnStrIn, str);

	Buffer<wchar_t> bufConnStrOut(cbConnStrOutMax);

	SQLRETURN rc = SQLBrowseConnectW(hdbc,
		(SQLWCHAR*) str.c_str(),
		(SQLSMALLINT) str.size(),
		bufConnStrOut.begin(),
		(SQLSMALLINT) bufConnStrOut.size(),
		pcbConnStrOut);

	makeUTF8(bufConnStrOut, *pcbConnStrOut, szConnStrOut, cbConnStrOutMax);

	return rc;
}


SQLRETURN SQLColumnPrivileges(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szTableName,
	SQLSMALLINT cbTableName,
	SQLCHAR*    szColumnName,
	SQLSMALLINT cbColumnName)
{
	throw NotImplementedException();
}


SQLRETURN SQLForeignKeys(SQLHSTMT hstmt,
	SQLCHAR*    szPkCatalogName,
	SQLSMALLINT cbPkCatalogName,
	SQLCHAR*    szPkSchemaName,
	SQLSMALLINT cbPkSchemaName,
	SQLCHAR*    szPkTableName,
	SQLSMALLINT cbPkTableName,
	SQLCHAR*    szFkCatalogName,
	SQLSMALLINT cbFkCatalogName,
	SQLCHAR*    szFkSchemaName,
	SQLSMALLINT cbFkSchemaName,
	SQLCHAR*    szFkTableName,
	SQLSMALLINT cbFkTableName)
{
	throw NotImplementedException();
}


SQLRETURN SQLNativeSql(SQLHDBC hdbc,
	SQLCHAR*    szSqlStrIn,
	SQLINTEGER  cbSqlStrIn,
	SQLCHAR*    szSqlStr,
	SQLINTEGER  cbSqlStrMax,
	SQLINTEGER* pcbSqlStr)
{
	std::wstring str;
	makeUTF16(szSqlStrIn, cbSqlStrIn, str);

	Buffer<wchar_t> bufSQLOut(cbSqlStrMax);

	SQLRETURN rc = SQLNativeSqlW(hdbc,
		(SQLWCHAR*) str.c_str(),
		(SQLINTEGER) str.size(),
		bufSQLOut.begin(),
		(SQLINTEGER) bufSQLOut.size(),
		pcbSqlStr);

	makeUTF8(bufSQLOut, *pcbSqlStr, szSqlStr, cbSqlStrMax);

	return rc;
}


SQLRETURN SQLPrimaryKeys(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szTableName,
	SQLSMALLINT cbTableName)
{
	throw NotImplementedException();
}


SQLRETURN SQLProcedureColumns(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szProcName,
	SQLSMALLINT cbProcName,
	SQLCHAR*    szColumnName,
	SQLSMALLINT cbColumnName)
{
	throw NotImplementedException();
}


SQLRETURN SQLProcedures(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szProcName,
	SQLSMALLINT cbProcName)
{
	throw NotImplementedException();
}


SQLRETURN SQLTablePrivileges(SQLHSTMT hstmt,
	SQLCHAR*    szCatalogName,
	SQLSMALLINT cbCatalogName,
	SQLCHAR*    szSchemaName,
	SQLSMALLINT cbSchemaName,
	SQLCHAR*    szTableName,
	SQLSMALLINT cbTableName)
{
	throw NotImplementedException();
}


SQLRETURN SQLDrivers(SQLHENV henv,
	SQLUSMALLINT fDirection,
	SQLCHAR*     szDriverDesc,
	SQLSMALLINT  cbDriverDescMax,
	SQLSMALLINT* pcbDriverDesc,
	SQLCHAR*     szDriverAttributes,
	SQLSMALLINT  cbDrvrAttrMax,
	SQLSMALLINT* pcbDrvrAttr)
{
	Buffer<wchar_t> bufDriverDesc(cbDriverDescMax);
	Buffer<wchar_t> bufDriverAttr(cbDrvrAttrMax);

	SQLRETURN rc = SQLDriversW(henv,
		fDirection,
		bufDriverDesc.begin(),
		(SQLSMALLINT) bufDriverDesc.size(),
		pcbDriverDesc,
		bufDriverAttr.begin(),
		(SQLSMALLINT) bufDriverAttr.size(),
		pcbDrvrAttr);

	makeUTF8(bufDriverDesc, *pcbDriverDesc, szDriverDesc, cbDriverDescMax);
	makeUTF8(bufDriverAttr, *pcbDrvrAttr, szDriverAttributes, cbDrvrAttrMax);

	return rc;
}


} } } // namespace Poco::Data::ODBC
