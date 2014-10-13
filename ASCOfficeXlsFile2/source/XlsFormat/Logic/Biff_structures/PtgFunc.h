#pragma once

#include "Ptg.h"
#include "Ftab_Cetab.h"

namespace XLS
{;

class CFRecord;

class PtgFunc : public Ptg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PtgFunc)
	PtgFunc(const unsigned __int16 func_index, const unsigned char data_type);
public:
	PtgFunc();
	BiffStructurePtr clone();

	static PtgPtr create(const std::wstring& word, const unsigned char data_type);

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void loadFields(CFRecord& record);
	virtual void storeFields(CFRecord& record);

	virtual void assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent);

	const long getParametersNum() const;
	
	static const unsigned __int16 fixed_id = 0x01;
private:
	Ftab_Cetab iftab;
};

typedef boost::shared_ptr<PtgFunc> PtgFuncPtr;

} // namespace XLS
