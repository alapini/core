#include "precompiled_xls.h"

#include "BinaryCodec_RCF.h"

namespace CRYPT
{;

BinaryCodec_RCF::BinaryCodec_RCF()
{
	(void)memset( mpnDigestValue, 0, sizeof( mpnDigestValue ) );
}

BinaryCodec_RCF::~BinaryCodec_RCF()
{
	(void)memset( mpnDigestValue, 0, sizeof( mpnDigestValue ) );
}

void BinaryCodec_RCF::initKey( const unsigned __int16 pnPassData[ 16 ], const unsigned char pnSalt[ 16 ] )
{
	// create little-endian key data array from password data
	unsigned char pnKeyData[ 64 ];
	(void)memset( pnKeyData, 0, sizeof( pnKeyData ) );

	const unsigned __int16* pnCurrPass = pnPassData;
	const unsigned __int16* pnPassEnd = pnPassData + 16;
	unsigned char* pnCurrKey = pnKeyData;
	size_t nPassSize = 0;
	for( ; (pnCurrPass < pnPassEnd) && (*pnCurrPass != 0); ++pnCurrPass, ++nPassSize )
	{
		*pnCurrKey++ = static_cast< unsigned char >( *pnCurrPass );
		*pnCurrKey++ = static_cast< unsigned char >( *pnCurrPass >> 8 );
	}
	pnKeyData[ 2 * nPassSize ] = 0x80;
	pnKeyData[ 56 ] = static_cast< unsigned char >( nPassSize << 4 );

	// fill raw digest of key data into key data
	mhDigest.Update(pnKeyData, sizeof( pnKeyData ) );
	mhDigest.Raw(pnKeyData, DigestMD5::RTL_DIGEST_LENGTH_MD5 );

	// update digest with key data and passed salt data
	for( size_t nIndex = 0; nIndex < 16; ++nIndex )
	{
		mhDigest.Update(pnKeyData, 5 );
		mhDigest.Update(pnSalt, 16 );
	}

	// update digest with padding
	pnKeyData[ 16 ] = 0x80;
	(void)memset( pnKeyData + 17, 0, sizeof( pnKeyData ) - 17 );
	pnKeyData[ 56 ] = 0x80;
	pnKeyData[ 57 ] = 0x0A;
	mhDigest.Update(pnKeyData + 16, sizeof( pnKeyData ) - 16 );

	// fill raw digest of above updates into digest value
	mhDigest.Raw(mpnDigestValue, sizeof( mpnDigestValue ) );

	// erase key data array and leave
	(void)memset( pnKeyData, 0, sizeof( pnKeyData ) );
}

bool BinaryCodec_RCF::verifyKey( const unsigned char pnVerifier[ 16 ], const unsigned char pnVerifierHash[ 16 ] )
{
	if( !startBlock( 0 ) )
		return false;

	unsigned char pnDigest[ DigestMD5::RTL_DIGEST_LENGTH_MD5 ];
	unsigned char pnBuffer[ 64 ];

	// decode salt data into buffer
	mhCipher.Decode(pnVerifier, 16, pnBuffer, sizeof(pnBuffer));

	pnBuffer[ 16 ] = 0x80;
	(void)memset( pnBuffer + 17, 0, sizeof( pnBuffer ) - 17 );
	pnBuffer[ 56 ] = 0x80;

	// fill raw digest of buffer into digest
	mhDigest.Update(pnBuffer, sizeof( pnBuffer ) );
	mhDigest.Raw(pnDigest, sizeof( pnDigest ) );

	// decode original salt digest into buffer
	mhCipher.Decode(pnVerifierHash, 16, pnBuffer, sizeof(pnBuffer));

	// compare buffer with computed digest
	bool bResult = memcmp( pnBuffer, pnDigest, sizeof( pnDigest ) ) == 0;

	// erase buffer and digest arrays and leave
	(void)memset( pnBuffer, 0, sizeof( pnBuffer ) );
	(void)memset( pnDigest, 0, sizeof( pnDigest ) );
	return bResult;
}

bool BinaryCodec_RCF::startBlock( size_t nCounter )
{
	// initialize key data array
	unsigned char pnKeyData[ 64 ];
	(void)memset( pnKeyData, 0, sizeof( pnKeyData ) );

	// fill 40 bit of digest value into [0..4]
	(void)memcpy( pnKeyData, mpnDigestValue, 5 );

	// fill little-endian counter into [5..8], static_cast masks out unneeded bits
	pnKeyData[ 5 ] = static_cast<unsigned char>( nCounter );
	pnKeyData[ 6 ] = static_cast<unsigned char>( nCounter >> 8 );
	pnKeyData[ 7 ] = static_cast<unsigned char>( nCounter >> 16 );
	pnKeyData[ 8 ] = static_cast<unsigned char>( nCounter >> 24 );

	pnKeyData[ 9 ] = 0x80;
	pnKeyData[ 56 ] = 0x48;

	// fill raw digest of key data into key data
	mhDigest.Update(pnKeyData, sizeof( pnKeyData ) );
	mhDigest.Raw(pnKeyData, DigestMD5::RTL_DIGEST_LENGTH_MD5 );

	// initialize cipher with key data (for decoding)
	CipherARCFOUR::rtlCipherError eResult = mhCipher.Init(CipherARCFOUR::rtl_Cipher_DirectionDecode, pnKeyData, DigestMD5::RTL_DIGEST_LENGTH_MD5, 0, 0 );

	// rrase key data array and leave
	(void)memset( pnKeyData, 0, sizeof( pnKeyData ) );
	return eResult == CipherARCFOUR::rtl_Cipher_E_None;
}

bool BinaryCodec_RCF::decode( unsigned char* pnDestData, const unsigned char* pnSrcData, size_t nBytes )
{
	return CipherARCFOUR::rtl_Cipher_E_None == mhCipher.Decode(pnSrcData, static_cast<size_t>(nBytes), pnDestData, static_cast<size_t>(nBytes));
}


bool BinaryCodec_RCF::skip( size_t nBytes )
{
	// decode dummy data in memory to update internal state of RC4 cipher
	unsigned char pnDummy[ 1024 ];
	size_t nBytesLeft = nBytes;
	bool bResult = true;
	while(bResult && (nBytesLeft > 0))
	{
		size_t nBlockLen = nBytesLeft < sizeof(pnDummy) ? nBytesLeft : sizeof(pnDummy);
		bResult = decode(pnDummy, pnDummy, nBlockLen);
		nBytesLeft -= nBlockLen;
	}
	return bResult;
}


};

