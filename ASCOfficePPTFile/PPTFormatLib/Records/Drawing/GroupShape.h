﻿#pragma once
#include "../../Reader/Records.h"

class CRecordGroupShape : public CUnknownRecord
{
public:
	RECT m_oBounds;

public:
	
	CRecordGroupShape()
	{
	}

	~CRecordGroupShape()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_oBounds.left		= StreamUtils::ReadLONG(pStream);
		m_oBounds.top		= StreamUtils::ReadLONG(pStream);
		m_oBounds.right		= StreamUtils::ReadLONG(pStream);
		m_oBounds.bottom	= StreamUtils::ReadLONG(pStream);

        m_oBounds.left		= (std::max)(0, (int)m_oBounds.left);
        m_oBounds.top		= (std::max)(0, (int)m_oBounds.top);
        m_oBounds.right		= (std::max)(0, (int)m_oBounds.right);
        m_oBounds.bottom	= (std::max)(0, (int)m_oBounds.bottom);
	}
	virtual CString ToString()
	{
		CString str = _T("");
		str.Format(_T("(%d,%d,%d,%d)"), m_oBounds.left, m_oBounds.top, m_oBounds.right, m_oBounds.bottom);

		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));
		oWriter.WriteAttribute(_T("rect"), str);

		oWriter.WriteNodeEnd(strName, TRUE);

		return oWriter.GetXmlString();
	}
};