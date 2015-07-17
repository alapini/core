#pragma once

#include <string>
#include <iosfwd>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace oox {

    class xlsx_num_fmts
    {
    public:
        xlsx_num_fmts();
        ~xlsx_num_fmts();
    public:
        size_t num_format_id(const std::wstring & format_code);
        void xlsx_serialize(std::wostream & _Wostream) const;

    private:
        struct Impl;
		boost::scoped_ptr<Impl> impl_;
    };

    void xlsx_serialize(std::wostream & _Wostream, const xlsx_num_fmts & numFmts);

}