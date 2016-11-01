#pragma once
#include "MakoCommon.h"
#ifdef MAKO_XAUDIO2_AVAILABLE
#include "MakoAudioDevice.h"
#include "MakoVec3d.h"
#include "MakoLinkedList.h"
#include "MakoOS.h"
#include <xaudio2.h>

MAKO_BEGIN_NAMESPACE

enum CWAVE_FILE_MODE
{
	WAVEFILE_READ,
	WAVEFILE_WRITE
};

// Forward declaration
class Application;

class XAudio2Device : public AudioDevice
{
private:
	class XAudioSound : public Sound
	{
	private:
		// XAudio source voice
		IXAudio2SourceVoice* xaudioVoice;

		// Dynamically allocated BYTE array containing
		// audio file data
		BYTE* waveData;

		Float32 volume;

		bool isPlaying;
	public:
		XAudioSound(IXAudio2SourceVoice* xaudioVoice, 
			BYTE* waveData,
			Float32 volume = 100.f);
		virtual ~XAudioSound();
		void Play();
		void Stop();
		void SetVolume(Float32 volume);
		Float32 GetVolume() const;
		bool IsPlaying() const;

		friend class XAudio2Device;
	};

	class XAudio3DSound : public XAudioSound
	{
	private:
		Vec3df pos;
	public:
		XAudio3DSound(IXAudio2SourceVoice* xaudioVoice, 
			BYTE* waveData,
			Float32 volume = 100.f,
			const Position3d& pos = Vec3df(0));
		~XAudio3DSound();
		
		const Vec3df& GetPosition() const;

		void SetPosition(const Position3d& pos);
	};

	class CWaveFile
	{
	public:
		WAVEFORMATEX* m_pwfx;   // Pointer to WAVEFORMATEX structure
		HMMIO m_hmmio;          // MM I/O handle for the WAVE
		MMCKINFO m_ck;          // Multimedia RIFF chunk
		MMCKINFO m_ckRiff;      // Use in opening a WAVE file
		DWORD m_dwSize;         // The size of the wave file
		MMIOINFO m_mmioinfoOut;
		DWORD m_dwFlags;
		BOOL m_bIsReadingFromMemory;
		BYTE* m_pbData;
		BYTE* m_pbDataCur;
		ULONG m_ulDataSize;
		CHAR* m_pResourceBuffer;

	protected:
		HRESULT ReadMMIO();
		HRESULT WriteMMIO( WAVEFORMATEX* pwfxDest );

	public:
		CWaveFile();
		~CWaveFile();

		HRESULT Open( LPWSTR strFileName, WAVEFORMATEX* pwfx, CWAVE_FILE_MODE dwFlags );
		HRESULT OpenFromMemory( BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, CWAVE_FILE_MODE dwFlags );
		HRESULT Close();

		HRESULT Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead );
		HRESULT Write( UINT nSizeToWrite, BYTE* pbData, UINT* pnSizeWrote );

		DWORD   GetSize();
		HRESULT ResetFile();
		WAVEFORMATEX* GetFormat()
		{
			return m_pwfx;
		};
	};



	Sound* PlayPCM(IXAudio2* pXaudio2, LPCWSTR szFilename);

private:
	Vec3df listenerPos, listenerRot;

	// Main interface to xaudio2
	IXAudio2* xaudio2;

	// Mastering voice connects to audio hardware
	IXAudio2MasteringVoice* masteringVoice;

	// Stl linked list for source voices
	LinkedList<XAudioSound*> sounds;
public:
	MAKO_API XAudio2Device();
	MAKO_API ~XAudio2Device();

	MAKO_API Sound* Play2dSound(const FilePath& fileName);

	//! \todo Provide support for 3d sounds (in XAudio impl). Currently
	//! returns nullptr.
	MAKO_API Sound3d* Play3dSound(const FilePath& fileName);

	MAKO_API const Vec3df& GetListenerPosition() const;

	MAKO_API void SetListenerPosition(const Position3d& pos);

	MAKO_API const Vec3df& GetListenerRotation() const;
	MAKO_API void SetListenerRotation(const Rotation3d& rot);

	MAKO_API void Update();
	MAKO_API void Clear();

	MAKO_API String GetName() const;

	AUDIO_DEVICE_TYPE GetType() const
	{ return ADT_XAUDIO2; }
};

MAKO_END_NAMESPACE
#endif