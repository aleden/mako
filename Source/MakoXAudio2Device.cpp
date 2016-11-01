#include "MakoCommon.h"
#ifdef MAKO_XAUDIO2_AVAILABLE
#include "MakoXAudio2Device.h"
#include "MakoException.h"
#include "MakoApplication.h"
#include "MakoFileSystem.h"
#include "MakoOS.h"
#include <dxerr.h>
#include <x3daudio.h>

MAKO_BEGIN_NAMESPACE

//#pragma comment (lib, "x3daudio.lib")
#pragma comment (lib, "ole32.lib") // COM

XAudio2Device::XAudio2Device() : xaudio2(nullptr)
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);


	HRESULT hr;
	UINT32 flags = 0;

#ifdef MAKO_DEBUG_MODE
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif
	// Create main interface to XAudio2
	if (FAILED(hr = XAudio2Create(&xaudio2, flags)))
	{
		CoUninitialize();
		throw Exception(Text("Failed to initialize XAudio2 engine"));
	}

	// Create mastering voice to send sound data to the audio hardware.
	if (FAILED(hr = xaudio2->CreateMasteringVoice(&masteringVoice)))
	{
		if(xaudio2)
			xaudio2->Release();
		CoUninitialize();
		throw Exception(Text("Failed creating XAudio2 mastering voice"));
	}

	listenerPos = listenerRot = 0;
}

XAudio2Device::~XAudio2Device()
{
	Clear();

	if (masteringVoice)
		masteringVoice->DestroyVoice();
	if(xaudio2)
		xaudio2->Release();

	CoUninitialize();
}

Sound* XAudio2Device::Play2dSound(const FilePath& fileName)
{
	FilePath found = APP()->FS()->FindFile(fileName);
	if (found.GetAbs().IsEmpty())
		throw Exception(Text("The texture file [") + fileName.GetAbs() + Text("] does not exist"));

	return PlayPCM(xaudio2, found.GetAbs().ToWStringData());
}

Sound3d* XAudio2Device::Play3dSound(const FilePath& fileName)
{ return nullptr; }

const Vec3df& XAudio2Device::GetListenerPosition() const
{ return listenerPos; }

void XAudio2Device::SetListenerPosition(const Position3d& pos)
{ this->listenerPos = pos; }

const Vec3df& XAudio2Device::GetListenerRotation() const
{ return listenerRot; }

void XAudio2Device::SetListenerRotation(const Rotation3d& rot)
{ this->listenerRot = rot; }

void XAudio2Device::Update()
{
	XAUDIO2_VOICE_STATE state;
	BOOL isRunning = TRUE;

	std::list<XAudioSound*>::iterator it = sounds.begin();
	while (it != sounds.end())
	{
		(*it)->xaudioVoice->GetState(&state);
		isRunning = ( state.BuffersQueued > 0 ) != 0;
		if (!isRunning)
		{
			delete (*it);
			it = sounds.erase(it);
		}
		else
		{
			++it;
		}
	}
}

// Stops and deletes all voices
void XAudio2Device::Clear()
{
	std::list<XAudioSound*>::iterator it = sounds.begin();
	while (it != sounds.end())
	{
		delete (*it);
		it = sounds.erase(it);
	}
}

String XAudio2Device::GetName() const
{ return String(Text("XAudio 2")); }
// Plays a wave
Sound* XAudio2Device::PlayPCM(IXAudio2* pXaudio2, LPCWSTR szFilename)
{
	HRESULT hr = S_OK;

	//
	// Read in the wave file
	//
	CWaveFile wav;
	if(FAILED(hr = wav.Open(const_cast<LPWSTR>(szFilename), nullptr, WAVEFILE_READ)))
	{
		throw Exception(Text("Failed reading WAV file in XAudio2Device::PlayPCM()"));
	}

	// Get format of wave file
	WAVEFORMATEX* pwfx = wav.GetFormat();

	// Calculate how many bytes and samples are in the wave
	DWORD cbWaveSize = wav.GetSize();

	// Read the sample data into memory
	BYTE* pbWaveData = new BYTE[cbWaveSize];

	if(FAILED(hr = wav.Read(pbWaveData, cbWaveSize, &cbWaveSize)))
	{
		if (pbWaveData)
			delete [] pbWaveData;
		throw Exception(Text("Failed reading WAV file in XAudio2Device::PlayPCM()"));
	}

	//
	// Play the wave using a XAudio2SourceVoice
	//

	// Create the source voice
	IXAudio2SourceVoice* pSourceVoice;
	if(FAILED(hr = pXaudio2->CreateSourceVoice(&pSourceVoice, pwfx)))
	{
		if (pbWaveData)
			delete [] pbWaveData;
		throw Exception(Text("Error creating source voice in XAudio2Device::PlayPCM()"));
	}

	// Submit the wave sample data using an XAUDIO2_BUFFER structure
	XAUDIO2_BUFFER buffer = {0};
	buffer.pAudioData = pbWaveData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
	buffer.AudioBytes = cbWaveSize;

	if(FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
	{
		pSourceVoice->DestroyVoice();
		if (pbWaveData)
			delete [] pbWaveData;
		throw Exception(Text("Error submitting source buffer in XAudio2Device::PlayPCM()"));
	}

	XAudioSound* sound;

	// Let the sound play
	hr = pSourceVoice->Start(0);
	if (SUCCEEDED(hr))
	{
		sound = new XAudioSound(pSourceVoice, pbWaveData);
		sounds.push_back(sound);
	}
	else
	{
		if (pbWaveData)
			delete [] pbWaveData;
		throw Exception(Text("XAudio2SourceVoice::Start() failed in XAudio2Device::PlayPCM()."));
	}
	return sound;
}

XAudio2Device::XAudioSound::XAudioSound(IXAudio2SourceVoice* xaudioVoice, 
									   BYTE* waveData,
									   Float32 volume)
									   : xaudioVoice(xaudioVoice), waveData(waveData), volume(volume),
									   isPlaying(true)
{}

XAudio2Device::XAudioSound::~XAudioSound()
{
	if (xaudioVoice)
		xaudioVoice->DestroyVoice();
	if (waveData)
		delete [] waveData;
}
void XAudio2Device::XAudioSound::Play()
{
	xaudioVoice->Start();
	isPlaying = true;
}
void XAudio2Device::XAudioSound::Stop()
{
	xaudioVoice->Stop();
	isPlaying = false;
}

void XAudio2Device::XAudioSound::SetVolume(Float32 volume)
{
	this->volume = volume;
	xaudioVoice->SetVolume(volume);
}

Float32 XAudio2Device::XAudioSound::GetVolume() const
{ return volume; }

bool XAudio2Device::XAudioSound::IsPlaying() const
{ return isPlaying; }

XAudio2Device::XAudio3DSound::XAudio3DSound(IXAudio2SourceVoice* xaudioVoice, 
										   BYTE* waveData,
										   Float32 volume,
										   const Position3d& pos)
										   : XAudioSound(xaudioVoice, waveData, volume), pos(pos)
{}

XAudio2Device::XAudio3DSound::~XAudio3DSound()
{}

const Vec3df& XAudio2Device::XAudio3DSound::GetPosition() const
{ return pos; }

void XAudio2Device::XAudio3DSound::SetPosition(const Position3d& pos)
{ this->pos = pos; }

//-----------------------------------------------------------------------------
// Name: CWaveFile::CWaveFile()
// Desc: Constructs the class.  Call Open() to open a wave file for reading.
//       Then call Read() as needed.  Calling the destructor or Close()
//       will close the file.
//-----------------------------------------------------------------------------
XAudio2Device::CWaveFile::CWaveFile()
{
	m_pwfx = nullptr;
	m_hmmio = nullptr;
	m_pResourceBuffer = nullptr;
	m_dwSize = 0;
	m_bIsReadingFromMemory = FALSE;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::~CWaveFile()
// Desc: Destructs the class
//-----------------------------------------------------------------------------
XAudio2Device::CWaveFile::~CWaveFile()
{
	Close();

	if( !m_bIsReadingFromMemory )
	{
		if (m_pwfx)
		{
			delete [] m_pwfx;
			m_pwfx = nullptr;
		}
	}
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::Open()
// Desc: Opens a wave file for reading
//-----------------------------------------------------------------------------
HRESULT XAudio2Device::CWaveFile::Open( LPWSTR strFileName, WAVEFORMATEX* pwfx, CWAVE_FILE_MODE dwFlags )
{
	HRESULT hr;

	m_dwFlags = dwFlags;
	m_bIsReadingFromMemory = FALSE;

	if( m_dwFlags == WAVEFILE_READ )
	{
		if( strFileName == nullptr )
			return E_INVALIDARG;

		if (m_pwfx)
		{
			delete [] m_pwfx;
			m_pwfx = nullptr;
		}

		m_hmmio = mmioOpen( strFileName, nullptr, MMIO_ALLOCBUF | MMIO_READ );

		if( nullptr == m_hmmio )
		{
			HRSRC hResInfo;
			HGLOBAL hResData;
			DWORD dwSize;
			VOID* pvRes;

			// Loading it as a file failed, so try it as a resource
			if( nullptr == ( hResInfo = FindResource( nullptr, strFileName, L"WAVE" ) ) )
			{
				if( nullptr == ( hResInfo = FindResource( nullptr, strFileName, L"WAV" ) ) )
					return DXTRACE_ERR( L"FindResource", E_FAIL );
			}

			if( nullptr == ( hResData = LoadResource( GetModuleHandle( nullptr ), hResInfo ) ) )
				return DXTRACE_ERR( L"LoadResource", E_FAIL );

			if( 0 == ( dwSize = SizeofResource( GetModuleHandle( nullptr ), hResInfo ) ) )
				return DXTRACE_ERR( L"SizeofResource", E_FAIL );

			if( nullptr == ( pvRes = LockResource( hResData ) ) )
				return DXTRACE_ERR( L"LockResource", E_FAIL );

			m_pResourceBuffer = new CHAR[ dwSize ];
			if( m_pResourceBuffer == nullptr )
				return DXTRACE_ERR( L"new", E_OUTOFMEMORY );
			memcpy( m_pResourceBuffer, pvRes, dwSize );

			MMIOINFO mmioInfo;
			ZeroMemory( &mmioInfo, sizeof( mmioInfo ) );
			mmioInfo.fccIOProc = FOURCC_MEM;
			mmioInfo.cchBuffer = dwSize;
			mmioInfo.pchBuffer = ( CHAR* )m_pResourceBuffer;

			m_hmmio = mmioOpen( nullptr, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ );
		}

		if( FAILED( hr = ReadMMIO() ) )
		{
			// ReadMMIO will fail if its an not a wave file
			mmioClose( m_hmmio, 0 );
			return DXTRACE_ERR( L"ReadMMIO", hr );
		}

		if( FAILED( hr = ResetFile() ) )
			return DXTRACE_ERR( L"ResetFile", hr );

		// After the reset, the size of the wav file is m_ck.cksize so store it now
		m_dwSize = m_ck.cksize;
	}
	else
	{
		m_hmmio = mmioOpen( strFileName, nullptr, MMIO_ALLOCBUF |
			MMIO_READWRITE |
			MMIO_CREATE );
		if( nullptr == m_hmmio )
			return DXTRACE_ERR( L"mmioOpen", E_FAIL );

		if( FAILED( hr = WriteMMIO( pwfx ) ) )
		{
			mmioClose( m_hmmio, 0 );
			return DXTRACE_ERR( L"WriteMMIO", hr );
		}

		if( FAILED( hr = ResetFile() ) )
			return DXTRACE_ERR( L"ResetFile", hr );
	}

	return hr;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::OpenFromMemory()
// Desc: copy data to CWaveFile member variable from memory
//-----------------------------------------------------------------------------
HRESULT XAudio2Device::CWaveFile::OpenFromMemory( BYTE* pbData, ULONG ulDataSize,
												WAVEFORMATEX* pwfx, CWAVE_FILE_MODE dwFlags )
{
	m_pwfx = pwfx;
	m_ulDataSize = ulDataSize;
	m_pbData = pbData;
	m_pbDataCur = m_pbData;
	m_bIsReadingFromMemory = TRUE;

	if( dwFlags != WAVEFILE_READ )
		return E_NOTIMPL;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::ReadMMIO()
// Desc: Support function for reading from a multimedia I/O stream.
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_pwfx.
//-----------------------------------------------------------------------------
HRESULT XAudio2Device::CWaveFile::ReadMMIO()
{
	MMCKINFO ckIn;           // chunk info. for general use.
	PCMWAVEFORMAT pcmWaveFormat;  // Temp PCM structure to load in.

	m_pwfx = nullptr;

	if( ( 0 != mmioDescend( m_hmmio, &m_ckRiff, nullptr, 0 ) ) )
		return DXTRACE_ERR( L"mmioDescend", E_FAIL );

	// Check to make sure this is a valid wave file
	if( ( m_ckRiff.ckid != FOURCC_RIFF ) ||
		( m_ckRiff.fccType != mmioFOURCC( 'W', 'A', 'V', 'E' ) ) )
		return DXTRACE_ERR( L"mmioFOURCC", E_FAIL );

	// Search the input file for for the 'fmt ' chunk.
	ckIn.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
	if( 0 != mmioDescend( m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK ) )
		return DXTRACE_ERR( L"mmioDescend", E_FAIL );

	// Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
	// if there are extra parameters at the end, we'll ignore them
	if( ckIn.cksize < ( LONG )sizeof( PCMWAVEFORMAT ) )
		return DXTRACE_ERR( L"sizeof(PCMWAVEFORMAT)", E_FAIL );

	// Read the 'fmt ' chunk into <pcmWaveFormat>.
	if( mmioRead( m_hmmio, ( HPSTR )&pcmWaveFormat,
		sizeof( pcmWaveFormat ) ) != sizeof( pcmWaveFormat ) )
		return DXTRACE_ERR( L"mmioRead", E_FAIL );

	// Allocate the waveformatex, but if its not pcm format, read the next
	// word, and thats how many extra bytes to allocate.
	if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
	{
		m_pwfx = ( WAVEFORMATEX* )new CHAR[ sizeof( WAVEFORMATEX ) ];
		if( nullptr == m_pwfx )
			return DXTRACE_ERR( L"m_pwfx", E_FAIL );

		// Copy the bytes from the pcm structure to the waveformatex structure
		memcpy( m_pwfx, &pcmWaveFormat, sizeof( pcmWaveFormat ) );
		m_pwfx->cbSize = 0;
	}
	else
	{
		// Read in length of extra bytes.
		WORD cbExtraBytes = 0L;
		if( mmioRead( m_hmmio, ( CHAR* )&cbExtraBytes, sizeof( WORD ) ) != sizeof( WORD ) )
			return DXTRACE_ERR( L"mmioRead", E_FAIL );

		m_pwfx = ( WAVEFORMATEX* )new CHAR[ sizeof( WAVEFORMATEX ) + cbExtraBytes ];
		if( nullptr == m_pwfx )
			return DXTRACE_ERR( L"new", E_FAIL );

		// Copy the bytes from the pcm structure to the waveformatex structure
		memcpy( m_pwfx, &pcmWaveFormat, sizeof( pcmWaveFormat ) );
		m_pwfx->cbSize = cbExtraBytes;

		// Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
		if( mmioRead( m_hmmio, ( CHAR* )( ( ( BYTE* )&( m_pwfx->cbSize ) ) + sizeof( WORD ) ),
			cbExtraBytes ) != cbExtraBytes )
		{
			if (m_pwfx)
			{
				delete m_pwfx;
				m_pwfx = nullptr;
			}
			return DXTRACE_ERR( L"mmioRead", E_FAIL );
		}
	}

	// Ascend the input file out of the 'fmt ' chunk.
	if( 0 != mmioAscend( m_hmmio, &ckIn, 0 ) )
	{
		if (m_pwfx)
		{
			delete m_pwfx;
			m_pwfx = nullptr;
		}
		return DXTRACE_ERR( L"mmioAscend", E_FAIL );
	}

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::GetSize()
// Desc: Retuns the size of the read access wave file
//-----------------------------------------------------------------------------
DWORD XAudio2Device::CWaveFile::GetSize()
{
	return m_dwSize;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::ResetFile()
// Desc: Resets the internal m_ck pointer so reading starts from the
//       beginning of the file again
//-----------------------------------------------------------------------------
HRESULT XAudio2Device::CWaveFile::ResetFile()
{
	if( m_bIsReadingFromMemory )
	{
		m_pbDataCur = m_pbData;
	}
	else
	{
		if( m_hmmio == nullptr )
			return CO_E_NOTINITIALIZED;

		if( m_dwFlags == WAVEFILE_READ )
		{
			// Seek to the data
			if( -1 == mmioSeek( m_hmmio, m_ckRiff.dwDataOffset + sizeof( FOURCC ),
				SEEK_SET ) )
				return DXTRACE_ERR( L"mmioSeek", E_FAIL );

			// Search the input file for the 'data' chunk.
			m_ck.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
			if( 0 != mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) )
				return DXTRACE_ERR( L"mmioDescend", E_FAIL );
		}
		else
		{
			// Create the 'data' chunk that holds the waveform samples.
			m_ck.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
			m_ck.cksize = 0;

			if( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) )
				return DXTRACE_ERR( L"mmioCreateChunk", E_FAIL );

			if( 0 != mmioGetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
				return DXTRACE_ERR( L"mmioGetInfo", E_FAIL );
		}
	}

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::Read()
// Desc: Reads section of data from a wave file into pBuffer and returns
//       how much read in pdwSizeRead, reading not more than dwSizeToRead.
//       This uses m_ck to determine where to start reading from.  So
//       subsequent calls will be continue where the last left off unless
//       Reset() is called.
//-----------------------------------------------------------------------------
HRESULT XAudio2Device::CWaveFile::Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead )
{
	if( m_bIsReadingFromMemory )
	{
		if( m_pbDataCur == nullptr )
			return CO_E_NOTINITIALIZED;
		if( pdwSizeRead != nullptr )
			*pdwSizeRead = 0;

		if( ( BYTE* )( m_pbDataCur + dwSizeToRead ) >
			( BYTE* )( m_pbData + m_ulDataSize ) )
		{
			dwSizeToRead = m_ulDataSize - ( DWORD )( m_pbDataCur - m_pbData );
		}

#pragma warning( disable: 4616 )    // disable warning about warning number '22104' being out of range
#pragma warning( disable: 22104 )   // disable PREfast warning during static code analysis
		CopyMemory( pBuffer, m_pbDataCur, dwSizeToRead );
#pragma warning( default: 22104 )
#pragma warning( default: 4616 )

		if( pdwSizeRead != nullptr )
			*pdwSizeRead = dwSizeToRead;

		return S_OK;
	}
	else
	{
		MMIOINFO mmioinfoIn; // current status of m_hmmio

		if( m_hmmio == nullptr )
			return CO_E_NOTINITIALIZED;
		if( pBuffer == nullptr || pdwSizeRead == nullptr )
			return E_INVALIDARG;

		*pdwSizeRead = 0;

		if( 0 != mmioGetInfo( m_hmmio, &mmioinfoIn, 0 ) )
			return DXTRACE_ERR( L"mmioGetInfo", E_FAIL );

		UINT cbDataIn = dwSizeToRead;
		if( cbDataIn > m_ck.cksize )
			cbDataIn = m_ck.cksize;

		m_ck.cksize -= cbDataIn;

		for( DWORD cT = 0; cT < cbDataIn; cT++ )
		{
			// Copy the bytes from the io to the buffer.
			if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
			{
				if( 0 != mmioAdvance( m_hmmio, &mmioinfoIn, MMIO_READ ) )
					return DXTRACE_ERR( L"mmioAdvance", E_FAIL );

				if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
					return DXTRACE_ERR( L"mmioinfoIn.pchNext", E_FAIL );
			}

			// Actual copy.
			*( ( BYTE* )pBuffer + cT ) = *( ( BYTE* )mmioinfoIn.pchNext );
			mmioinfoIn.pchNext++;
		}

		if( 0 != mmioSetInfo( m_hmmio, &mmioinfoIn, 0 ) )
			return DXTRACE_ERR( L"mmioSetInfo", E_FAIL );

		*pdwSizeRead = cbDataIn;

		return S_OK;
	}
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::Close()
// Desc: Closes the wave file
//-----------------------------------------------------------------------------
HRESULT XAudio2Device::CWaveFile::Close()
{
	if( m_dwFlags == WAVEFILE_READ )
	{
		mmioClose( m_hmmio, 0 );
		m_hmmio = nullptr;

		if (m_pResourceBuffer)
		{
			delete [] m_pResourceBuffer;
			m_pResourceBuffer = nullptr;
		}
	}
	else
	{
		m_mmioinfoOut.dwFlags |= MMIO_DIRTY;

		if( m_hmmio == nullptr )
			return CO_E_NOTINITIALIZED;

		if( 0 != mmioSetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
			return DXTRACE_ERR( L"mmioSetInfo", E_FAIL );

		// Ascend the output file out of the 'data' chunk -- this will cause
		// the chunk size of the 'data' chunk to be written.
		if( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
			return DXTRACE_ERR( L"mmioAscend", E_FAIL );

		// Do this here instead...
		if( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
			return DXTRACE_ERR( L"mmioAscend", E_FAIL );

		mmioSeek( m_hmmio, 0, SEEK_SET );

		if( 0 != ( INT )mmioDescend( m_hmmio, &m_ckRiff, nullptr, 0 ) )
			return DXTRACE_ERR( L"mmioDescend", E_FAIL );

		m_ck.ckid = mmioFOURCC( 'f', 'a', 'c', 't' );

		if( 0 == mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) )
		{
			DWORD dwSamples = 0;
			mmioWrite( m_hmmio, ( HPSTR )&dwSamples, sizeof( DWORD ) );
			mmioAscend( m_hmmio, &m_ck, 0 );
		}

		// Ascend the output file out of the 'RIFF' chunk -- this will cause
		// the chunk size of the 'RIFF' chunk to be written.
		if( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
			return DXTRACE_ERR( L"mmioAscend", E_FAIL );

		mmioClose( m_hmmio, 0 );
		m_hmmio = nullptr;
	}

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::WriteMMIO()
// Desc: Support function for reading from a multimedia I/O stream
//       pwfxDest is the WAVEFORMATEX for this new wave file.
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_ck.
//-----------------------------------------------------------------------------
HRESULT XAudio2Device::CWaveFile::WriteMMIO( WAVEFORMATEX* pwfxDest )
{
	DWORD dwFactChunk; // Contains the actual fact chunk. Garbage until WaveCloseWriteFile.
	MMCKINFO ckOut1;

	dwFactChunk = ( DWORD )-1;

	// Create the output file RIFF chunk of form type 'WAVE'.
	m_ckRiff.fccType = mmioFOURCC( 'W', 'A', 'V', 'E' );
	m_ckRiff.cksize = 0;

	if( 0 != mmioCreateChunk( m_hmmio, &m_ckRiff, MMIO_CREATERIFF ) )
		return DXTRACE_ERR( L"mmioCreateChunk", E_FAIL );

	// We are now descended into the 'RIFF' chunk we just created.
	// Now create the 'fmt ' chunk. Since we know the size of this chunk,
	// specify it in the MMCKINFO structure so MMIO doesn't have to seek
	// back and set the chunk size after ascending from the chunk.
	m_ck.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
	m_ck.cksize = sizeof( PCMWAVEFORMAT );

	if( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) )
		return DXTRACE_ERR( L"mmioCreateChunk", E_FAIL );

	// Write the PCMWAVEFORMAT structure to the 'fmt ' chunk if its that type.
	if( pwfxDest->wFormatTag == WAVE_FORMAT_PCM )
	{
		if( mmioWrite( m_hmmio, ( HPSTR )pwfxDest,
			sizeof( PCMWAVEFORMAT ) ) != sizeof( PCMWAVEFORMAT ) )
			return DXTRACE_ERR( L"mmioWrite", E_FAIL );
	}
	else
	{
		// Write the variable length size.
		if( ( UINT )mmioWrite( m_hmmio, ( HPSTR )pwfxDest,
			sizeof( *pwfxDest ) + pwfxDest->cbSize ) !=
			( sizeof( *pwfxDest ) + pwfxDest->cbSize ) )
			return DXTRACE_ERR( L"mmioWrite", E_FAIL );
	}

	// Ascend out of the 'fmt ' chunk, back into the 'RIFF' chunk.
	if( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
		return DXTRACE_ERR( L"mmioAscend", E_FAIL );

	// Now create the fact chunk, not required for PCM but nice to have.  This is filled
	// in when the close routine is called.
	ckOut1.ckid = mmioFOURCC( 'f', 'a', 'c', 't' );
	ckOut1.cksize = 0;

	if( 0 != mmioCreateChunk( m_hmmio, &ckOut1, 0 ) )
		return DXTRACE_ERR( L"mmioCreateChunk", E_FAIL );

	if( mmioWrite( m_hmmio, ( HPSTR )&dwFactChunk, sizeof( dwFactChunk ) ) !=
		sizeof( dwFactChunk ) )
		return DXTRACE_ERR( L"mmioWrite", E_FAIL );

	// Now ascend out of the fact chunk...
	if( 0 != mmioAscend( m_hmmio, &ckOut1, 0 ) )
		return DXTRACE_ERR( L"mmioAscend", E_FAIL );

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::Write()
// Desc: Writes data to the open wave file
//-----------------------------------------------------------------------------
HRESULT XAudio2Device::CWaveFile::Write( UINT nSizeToWrite, BYTE* pbSrcData, UINT* pnSizeWrote )
{
	UINT cT;

	if( m_bIsReadingFromMemory )
		return E_NOTIMPL;
	if( m_hmmio == nullptr )
		return CO_E_NOTINITIALIZED;
	if( pnSizeWrote == nullptr || pbSrcData == nullptr )
		return E_INVALIDARG;

	*pnSizeWrote = 0;

	for( cT = 0; cT < nSizeToWrite; cT++ )
	{
		if( m_mmioinfoOut.pchNext == m_mmioinfoOut.pchEndWrite )
		{
			m_mmioinfoOut.dwFlags |= MMIO_DIRTY;
			if( 0 != mmioAdvance( m_hmmio, &m_mmioinfoOut, MMIO_WRITE ) )
				return DXTRACE_ERR( L"mmioAdvance", E_FAIL );
		}

		*( ( BYTE* )m_mmioinfoOut.pchNext ) = *( ( BYTE* )pbSrcData + cT );
		( BYTE* )m_mmioinfoOut.pchNext++;

		( *pnSizeWrote )++;
	}

	return S_OK;
}

MAKO_END_NAMESPACE
#endif // MAKO_XAUDIO2_AVAILABLE