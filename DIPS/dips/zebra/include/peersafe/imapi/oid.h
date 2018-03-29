#ifndef __H_IMAPI_OID_H__
#define __H_IMAPI_OID_H__

#include <string>

namespace peersafe {
namespace im {

#define OID_GET     0
#define OID_SET     1
#define OID_TRAP    2
#define OID_RET     3

	//
	struct OidInfo{
		std::string	oid;
		int			type;
		std::string	value;
		int			data_type; // 0:string 1:int 2:float
		OidInfo() :oid(""), type(0), value(""), data_type(0){
		}
	};

} // namesapce im
} // namespace peersafe

#endif  //  __H_IMAPI_OID_H__
