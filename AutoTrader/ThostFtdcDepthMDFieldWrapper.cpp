#include "stdafx.h"
#include "DBWrapper.h"
#include <sstream>
#include <assert.h>
#include <time.h>
#include "config.h"
#include "ThostFtdcDepthMDFieldWrapper.h"
#include "TechVec.h"
#include "CommonUtils.h"

bool CThostFtdcDepthMDFieldWrapper::firstlanuch = true;

CThostFtdcDepthMDFieldWrapper::CThostFtdcDepthMDFieldWrapper(CThostFtdcDepthMarketDataField* p)
	: recoveryData(false)
	, m_techvec(nullptr)
{
	assert(p);
	memcpy(&m_MdData, p, sizeof(CThostFtdcDepthMarketDataField));
	m_uuid = this->toTimeStamp();
	//m_techvec = new StrategyTech(m_uuid, p->InstrumentID);
}

CThostFtdcDepthMDFieldWrapper::~CThostFtdcDepthMDFieldWrapper()
{
	if (m_techvec)
	{
		delete m_techvec;
		m_techvec = nullptr;
	}
}

CThostFtdcDepthMDFieldWrapper::CThostFtdcDepthMDFieldWrapper(const CThostFtdcDepthMDFieldWrapper& obj)
{
	m_MdData = obj.m_MdData;
	m_uuid = obj.m_uuid;
	recoveryData = obj.recoveryData;

	if (obj.m_techvec){
		size_t size = obj.m_techvec->ObjSize();
		m_techvec = (StrategyTech*)malloc(size);
		memcpy(m_techvec, obj.m_techvec, size);
	}
	else{
		m_techvec = nullptr;
	}
}

CThostFtdcDepthMDFieldWrapper& CThostFtdcDepthMDFieldWrapper::operator = (const CThostFtdcDepthMDFieldWrapper& obj)
{
	if (this == &obj)
		return *this;

	m_MdData = obj.m_MdData;
	m_uuid = obj.m_uuid;
	recoveryData = obj.recoveryData;

	delete m_techvec;

	if (obj.m_techvec){
		size_t size = obj.m_techvec->ObjSize();
		m_techvec = (StrategyTech*)malloc(size);
		memcpy(m_techvec, obj.m_techvec, size);
	}
	else{
		m_techvec = nullptr;
	}
	return *this;
}

CThostFtdcDepthMDFieldWrapper::CThostFtdcDepthMDFieldWrapper(CThostFtdcDepthMDFieldWrapper && obj)
	: m_MdData(obj.m_MdData)
	, m_uuid(obj.m_uuid)
	, recoveryData(obj.recoveryData)
{
	m_techvec = obj.m_techvec;
	obj.m_techvec = nullptr;
}

long long CThostFtdcDepthMDFieldWrapper::toTimeStamp() const{
	long long ret = CommonUtils::DateTimeToTimestamp(m_MdData.TradingDay, m_MdData.UpdateTime) * 2 + m_MdData.UpdateMillisec / 500;
	return ret;
}

void CThostFtdcDepthMDFieldWrapper::serializeToDB(DBWrapper& db) const {
	// if this item is recovered from db, so that we don't need serialize it to db again. 
	if (recoveryData)
		return;

	std::string tableName(m_MdData.InstrumentID);

	DBUtils::CreateTickTableIfNotExists(Config::Instance()->DBName(), tableName);

	std::stringstream sql;
	sql << "INSERT INTO `" << tableName << "` (`";
	sql << "Date" << "`,`";
	sql << "InstrumentID" << "`,`";
	sql << "ExchangeID" << "`,`";
	sql << "ExchangeInstID" << "`,`";
	sql << "LastPrice" << "`,`";
	sql << "PreSettlementPrice" << "`,`";
	sql << "PreClosePrice" << "`,`";
	sql << "PreOpenInterest" << "`,`";
	sql << "OpenPrice" << "`,`";
	sql << "HighestPrice" << "`,`";
	sql << "LowestPrice" << "`,`";
	sql << "Volume" << "`,`";
	sql << "Turnover" << "`,`";
	sql << "OpenInterest" << "`,`";
	sql << "ClosePrice" << "`,`";
	sql << "SettlementPrice" << "`,`";
	sql << "UpperLimitPrice" << "`,`";
	sql << "LowerLimitPrice" << "`,`";
	sql << "PreDelta" << "`,`";
	sql << "CurrDelta"<< "`,`";
	sql << "UpdateTime"<< "`,`";
	sql << "UpdateMillisec" << "`,`";
	sql << "BidPrice1" << "`,`";
	sql << "BidVolume1" << "`,`";
	sql << "AskPrice1" << "`,`";
	sql << "AskVolume1" << "`,`";
	sql << "BidPrice2" << "`,`";
	sql << "BidVolume2" << "`,`";
	sql << "AskPrice2" << "`,`";
	sql << "AskVolume2" << "`,`";
	sql << "BidPrice3" << "`,`";
	sql << "BidVolume3" << "`,`";
	sql << "AskPrice3" << "`,`";
	sql << "AskVolume3" << "`,`";
	sql << "BidPrice4" << "`,`";
	sql << "BidVolume4" << "`,`";
	sql << "AskPrice4" << "`,`";
	sql << "AskVolume4" << "`,`";
	sql << "BidPrice5" << "`,`";
	sql << "BidVolume5" << "`,`";
	sql << "AskPrice5" << "`,`";
	sql << "AskVolume5" << "`,`";
	sql << "AveragePrice" << "`,`";
	sql << "ActionDay" << "`,`";
	sql << "uuid" << "`";
	sql << ") VALUES(\"";
	sql << m_MdData.TradingDay << "\", \"";//m_MdData.TradingDay
	sql <<  m_MdData.InstrumentID << "\", \"";
	sql <<  m_MdData.ExchangeID << "\", \"";
	sql <<  m_MdData.ExchangeInstID << "\", ";
	sql <<  m_MdData.LastPrice << ", ";
	sql <<  m_MdData.PreSettlementPrice << ", ";
	sql <<  m_MdData.PreClosePrice << ", ";
	sql <<  m_MdData.PreOpenInterest << ", ";
	sql <<  m_MdData.OpenPrice << ", ";
	sql <<  m_MdData.HighestPrice << ", ";
	sql <<  m_MdData.LowestPrice << ", ";
	sql <<  m_MdData.Volume << ", ";
	sql <<  m_MdData.Turnover << ", ";
	sql <<  m_MdData.OpenInterest << ", ";
	sql <<  m_MdData.ClosePrice << ", ";
	sql <<  m_MdData.SettlementPrice << ", ";
	sql <<  m_MdData.UpperLimitPrice << ", ";
	sql <<  m_MdData.LowerLimitPrice << ", ";
	sql <<  m_MdData.PreDelta << ", ";
	sql <<  m_MdData.CurrDelta << ", \"";
	sql <<  m_MdData.UpdateTime << "\", ";  // m_MdData.UpdateTime
	sql <<  m_MdData.UpdateMillisec << ", ";
	sql <<  m_MdData.BidPrice1 << ", ";
	sql <<  m_MdData.BidVolume1 << ", ";
	sql <<  m_MdData.AskPrice1 << ", ";
	sql <<  m_MdData.AskVolume1 << ", ";
	sql <<  m_MdData.BidPrice2 << ", ";
	sql <<  m_MdData.BidVolume2 << ", ";
	sql <<  m_MdData.AskPrice2 << ", ";
	sql <<  m_MdData.AskVolume2 << ", ";
	sql <<  m_MdData.BidPrice3 << ", ";
	sql <<  m_MdData.BidVolume3 << ", ";
	sql <<  m_MdData.AskPrice3 << ", ";
	sql <<  m_MdData.AskVolume3 << ", ";
	sql <<  m_MdData.BidPrice4 << ", ";
	sql <<  m_MdData.BidVolume4 << ", ";
	sql <<  m_MdData.AskPrice4 << ", ";
	sql <<  m_MdData.AskVolume4 << ", ";
	sql <<  m_MdData.BidPrice5 << ", ";
	sql <<  m_MdData.BidVolume5 << ", ";
	sql <<  m_MdData.AskPrice5 << ", ";
	sql <<  m_MdData.AskVolume5 << ", ";
	sql <<  m_MdData.AveragePrice << ", \"";
	sql <<  m_MdData.ActionDay << "\", "; // m_MdData.ActionDay
	sql <<  m_uuid << ")";
	//"INSERT INTO `test` (`name`) VALUES (1234) 
	//std::cerr << sql.str() << std::endl;
	db.ExecuteNoResult(sql.str());
}

CThostFtdcDepthMDFieldWrapper CThostFtdcDepthMDFieldWrapper::RecoverFromDB(const CThostFtdcDepthMDFieldDBStruct& vec)
{
	CThostFtdcDepthMarketDataField mdStuct;
	memset(&mdStuct, 0, sizeof(mdStuct));
	strcpy_s(mdStuct.TradingDay, CommonUtils::ConvertTime(vec[1]).c_str());// todo: from "2015-04-30 00:00:00" to "20150430"
	strcpy_s(mdStuct.InstrumentID, vec[2].c_str());
	strcpy_s(mdStuct.ExchangeID, vec[3].c_str());
	strcpy_s(mdStuct.ExchangeInstID, vec[4].c_str());
	mdStuct.LastPrice = CommonUtils::StringtoDouble(vec[5]);
	mdStuct.PreSettlementPrice = CommonUtils::StringtoDouble(vec[6]);
	mdStuct.PreClosePrice = CommonUtils::StringtoDouble(vec[7]);
	mdStuct.PreOpenInterest = CommonUtils::StringtoDouble(vec[8]);
	mdStuct.OpenPrice = CommonUtils::StringtoDouble(vec[9]);
	mdStuct.HighestPrice = CommonUtils::StringtoDouble(vec[10]);
	mdStuct.LowestPrice = CommonUtils::StringtoDouble(vec[11]);
	mdStuct.Volume = CommonUtils::Stringtolong(vec[12]);
	mdStuct.Turnover = CommonUtils::StringtoDouble(vec[13]);
	mdStuct.OpenInterest = CommonUtils::StringtoDouble(vec[14]);
	mdStuct.ClosePrice = CommonUtils::StringtoDouble(vec[15]);
	mdStuct.SettlementPrice = CommonUtils::StringtoDouble(vec[16]);

	mdStuct.UpperLimitPrice = CommonUtils::StringtoDouble(vec[17]);
	mdStuct.LowerLimitPrice = CommonUtils::StringtoDouble(vec[18]);
	mdStuct.PreDelta = CommonUtils::StringtoDouble(vec[19]);
	mdStuct.CurrDelta = CommonUtils::StringtoDouble(vec[20]);
	strcpy_s(mdStuct.UpdateTime, vec[21].c_str());// todo: from "00:00:00" to "150430" // enhance :specify the size of str
	mdStuct.UpdateMillisec = CommonUtils::StringtoInt(vec[22]);
	mdStuct.BidPrice1 = CommonUtils::StringtoDouble(vec[23]);
	mdStuct.BidVolume1 = CommonUtils::Stringtolong(vec[24]);
	mdStuct.AskPrice1 = CommonUtils::StringtoDouble(vec[25]);
	mdStuct.AskVolume1 = CommonUtils::Stringtolong(vec[26]);
	mdStuct.BidPrice2 = CommonUtils::StringtoDouble(vec[27]);
	mdStuct.BidVolume2 = CommonUtils::Stringtolong(vec[28]);
	mdStuct.AskPrice2 = CommonUtils::StringtoDouble(vec[29]);
	mdStuct.AskVolume2 = CommonUtils::Stringtolong(vec[30]);
	mdStuct.BidPrice3 = CommonUtils::StringtoDouble(vec[31]);
	mdStuct.BidVolume3 = CommonUtils::Stringtolong(vec[32]);
	mdStuct.AskPrice3 = CommonUtils::StringtoDouble(vec[33]);
	mdStuct.AskVolume3 = CommonUtils::Stringtolong(vec[34]);
	mdStuct.BidPrice4 = CommonUtils::StringtoDouble(vec[35]);
	mdStuct.BidVolume4 = CommonUtils::Stringtolong(vec[36]);
	mdStuct.AskPrice4 = CommonUtils::StringtoDouble(vec[37]);
	mdStuct.AskVolume4 = CommonUtils::Stringtolong(vec[38]);
	mdStuct.BidPrice5 = CommonUtils::StringtoDouble(vec[39]);
	mdStuct.BidVolume5 = CommonUtils::Stringtolong(vec[40]);
	mdStuct.AskPrice5 = CommonUtils::StringtoDouble(vec[41]);
	mdStuct.AskVolume5 = CommonUtils::Stringtolong(vec[42]);
	mdStuct.AveragePrice = CommonUtils::StringtoDouble(vec[43]);
	strcpy_s(mdStuct.ActionDay, CommonUtils::ConvertTime(vec[44]).c_str());
	CThostFtdcDepthMDFieldWrapper mdObject(&mdStuct);
	mdObject.recoveryData = true;
	return mdObject;
}
