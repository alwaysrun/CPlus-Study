#include "xugd.random.h"
#include <ctime>
#include <sstream>
#include <boost/random.hpp>
#include <boost/serialization/singleton.hpp>

namespace xugd{	namespace clib{	
	using namespace std;
	using namespace boost;
	using boost::serialization::singleton;

	//////////////////////////////////////////////////////////////////////////
	template<typename T> class xrng_wrapper{
	public:
		xrng_wrapper():m_tRng((typename T::result_type)time(NULL))
		{ }

		typename T::result_type operator()(int nMin_, int nMax_) {
			uniform_int<> ui(nMin_, nMax_);
			return ui(m_tRng);
		}

	private:
		T m_tRng;
	};
	typedef singleton< xrng_wrapper<mt19937> > XRng_t;

	//////////////////////////////////////////////////////////////////////////
	int XRandom::GetInt(int nMin_, int nMax_){
		return XRng_t::get_mutable_instance()(nMin_, nMax_);
	}

	bool XRandom::GetBool(){
		return GetInt(0, 1) == 0;
	}

	string XRandom::GetString(int nLen_){
		ostringstream outStr;
		for(int i=0 ; i<nLen_ ; ++i){
			outStr<<(char)GetInt(32, 126);
		}

		return outStr.str();
	}

	void XRandom::GetBuff(unsigned char *pByte_, int nLen_)
	{
		for(int i=0 ; i<nLen_ ; ++i)
			pByte_[i] = (unsigned char)(GetInt(0, 255) & 0xFF);
	}

	string XRandom::GetBuff(int nLen_)
	{
		ostringstream strOut;
		for(int i=0 ; i<nLen_ ; ++i)
			strOut<<(char)(GetInt(0, 255) & 0xFF);

		return strOut.str();
	}
} //clib
} // xugd