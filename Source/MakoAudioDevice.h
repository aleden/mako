#pragma once
#include "MakoCommon.h"
#include "MakoString.h"
#include "MakoFilePath.h"
#include "MakoVec3d.h"
#include "MakoReferenceCounted.h"
#include "MakoDevice.h"

MAKO_BEGIN_NAMESPACE

//! This class represents a sound. Each implementation of an AudioDevice must also
//! create it's own implementation of this. It doesn't have to be playing, and it doesn't
//! have to be stopped. A sound does not need to be reference counted (inherit
//! ReferenceCounted) because there is simply no need for a sound to be reference
//! counted. In fact, if a sound was reference counted, it may lead to undesireable
//! behavior.
//! 
//! Consider that you have a pointer to a Sound in some place far away from where
//! you created it. If you are going to attempt to share the pointer between the
//! places, you will have playing/stopping confusions. 
class Sound
{
public:
	//! Resumes playing sound if Stop() was called. Else, does
	//! nothing.
	virtual void Play() = 0;

	//! Pauses the sound immediatly, and keeps it's track
	//! position
	virtual void Stop() = 0;
	
	//! Sets the volume for the sound
	//! \param[in] volume The new volume, 0.f - 100.f. A volume level of 1.0 means 
	//! there is no attenuation or gain and 0 means silence. Anything higher than 1.0
	//! is not reccomended to be set and it's behavior is specific to the implementation
	//! device.
	virtual void SetVolume(Float32 volume) = 0;
	
	//! Get the volume for the sound
	//! \return The volume
	virtual Float32 GetVolume() const = 0;

	//! \return True if the sound is currently playing, false if not
	virtual bool IsPlaying() const = 0;
};

//! This class represents a sound in 3d space. Each implementation of an AudioDevice
//! must also create it's own implementation of this. The speaker channels' volumes are
//! dictated by the position and rotation of the listener (see AudioDevice) and
//! the position of the 3d sound.
class Sound3d : public Sound
{
public:
	//! Gets the position of the 3d sound
	//! \return The position
	virtual const Position3d& GetPosition() const = 0;

	//! Sets the position of the 3d sound
	//! \param[in] pos The new position
	virtual void SetPosition(const Position3d& pos) = 0;
};

//! This enum describes various types of AudioDevices
enum AUDIO_DEVICE_TYPE
{
#ifdef MAKO_XAUDIO2_AVAILABLE
	//! XAudio2 is a low-level, cross-platform audio API for 
	//! Microsoft Windows and Xbox 360.
	ADT_XAUDIO2,
#endif
	ADT_ENUM_LENGTH
};

//! This abstract class handles playing regular and 3d sounds.
class AudioDevice : public Device
{
public:
	//! Creates a 2d sound, and calls Play() on it, then returns it
	//! \return The newly created sound
	virtual Sound* Play2dSound(const FilePath& fileName) = 0;
	
	//! Creates a 3d sound, and calls Play() on it, then returns it
	//! \return The newly created sound
	virtual Sound3d* Play3dSound(const FilePath& fileName) = 0;
	
	//! Gets the 3d listener position. This only affects 3d sounds.
	//! \return The listener's position
	virtual const Vec3df& GetListenerPosition() const = 0;

	//! Sets the 3d listener position for 3d sounds.
	//! \param[in] pos The new listener's position
	virtual void SetListenerPosition(const Position3d& pos) = 0;

	//! Gets the listener's rotation
	//! \return The listener's rotation
	virtual const Vec3df& GetListenerRotation() const = 0;
	
	//! Sets the listener's rotation
	//! \param[in] rot The new rotation
	virtual void SetListenerRotation(const Rotation3d& rot) = 0;

	//! Updates the device.
	virtual void Update() = 0;

	//! Destroys all sounds currently alive
	virtual void Clear() = 0;

	//! Get the actual type of AudioDevice this is
	virtual AUDIO_DEVICE_TYPE GetType() const = 0;

	//! Virtual empty deconstructor
	virtual ~AudioDevice() {}
};

MAKO_END_NAMESPACE