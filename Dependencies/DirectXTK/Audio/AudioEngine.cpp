//--------------------------------------------------------------------------------------
// File: AudioEngine.cpp
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Audio.h"
#include "PlatformHelpers.h"

#include <list>

using namespace DirectX;
using namespace Microsoft::WRL;


namespace
{
    struct EngineCallback : public IXAudio2EngineCallback
    {
        EngineCallback()
        {
#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
            mCriticalError = CreateEventEx( nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE );
#else
            mCriticalError = CreateEvent( nullptr, FALSE, FALSE, nullptr );
#endif
            if ( !mCriticalError )
            {
                throw std::exception( "CreateEvent" );
            }
        };

        ~EngineCallback()
        {
            CloseHandle( mCriticalError );
        }

        STDMETHOD_(void, OnProcessingPassStart) () override {}
        STDMETHOD_(void, OnProcessingPassEnd)() override {}

        STDMETHOD_(void, OnCriticalError) (THIS_ HRESULT error)
        {
#ifndef _DEBUG
            UNREFERENCED_PARAMETER(error);
#endif
            DebugTrace( "ERROR: AudioEngine encountered critical error (%08X)\n", error );
            SetEvent( mCriticalError );
        }

        HANDLE  mCriticalError;
    };

    struct VoiceCallback : public IXAudio2VoiceCallback
    {
        VoiceCallback()
        {
#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
            mBufferEnd = CreateEventEx( nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE );
#else
            mBufferEnd = CreateEvent( nullptr, FALSE, FALSE, nullptr );
#endif
            if ( !mBufferEnd )
            {
                throw std::exception( "CreateEvent" );
            }
        }

        ~VoiceCallback()
        {
            CloseHandle( mBufferEnd ); 
        }

        STDMETHOD_(void, OnVoiceProcessingPassStart) (UINT32) override {}
        STDMETHOD_(void, OnVoiceProcessingPassEnd)() override {}
        STDMETHOD_(void, OnStreamEnd)() override {}
        STDMETHOD_(void, OnBufferStart)( void* ) override {}

        STDMETHOD_(void, OnBufferEnd)( void* context ) override
        {
            if ( context )
            {
                auto inotify = reinterpret_cast<IVoiceNotify*>( context );
                inotify->OnBufferEnd();
                SetEvent( mBufferEnd );
            }
        }

        STDMETHOD_(void, OnLoopEnd)( void* ) override {}
        STDMETHOD_(void, OnVoiceError)( void*, HRESULT ) override {}

        HANDLE mBufferEnd;
    };

    static const XAUDIO2FX_REVERB_I3DL2_PARAMETERS gReverbPresets[] =
    {
        XAUDIO2FX_I3DL2_PRESET_DEFAULT,             // Reverb_Off
        XAUDIO2FX_I3DL2_PRESET_DEFAULT,             // Reverb_Default
        XAUDIO2FX_I3DL2_PRESET_GENERIC,             // Reverb_Generic
        XAUDIO2FX_I3DL2_PRESET_FOREST,              // Reverb_Forest
        XAUDIO2FX_I3DL2_PRESET_PADDEDCELL,          // Reverb_PaddedCell
        XAUDIO2FX_I3DL2_PRESET_ROOM,                // Reverb_Room
        XAUDIO2FX_I3DL2_PRESET_BATHROOM,            // Reverb_Bathroom
        XAUDIO2FX_I3DL2_PRESET_LIVINGROOM,          // Reverb_LivingRoom
        XAUDIO2FX_I3DL2_PRESET_STONEROOM,           // Reverb_StoneRoom
        XAUDIO2FX_I3DL2_PRESET_AUDITORIUM,          // Reverb_Auditorium
        XAUDIO2FX_I3DL2_PRESET_CONCERTHALL,         // Reverb_ConcertHall
        XAUDIO2FX_I3DL2_PRESET_CAVE,                // Reverb_Cave
        XAUDIO2FX_I3DL2_PRESET_ARENA,               // Reverb_Arena
        XAUDIO2FX_I3DL2_PRESET_HANGAR,              // Reverb_Hangar
        XAUDIO2FX_I3DL2_PRESET_CARPETEDHALLWAY,     // Reverb_CarpetedHallway
        XAUDIO2FX_I3DL2_PRESET_HALLWAY,             // Reverb_Hallway
        XAUDIO2FX_I3DL2_PRESET_STONECORRIDOR,       // Reverb_StoneCorridor
        XAUDIO2FX_I3DL2_PRESET_ALLEY,               // Reverb_Alley
        XAUDIO2FX_I3DL2_PRESET_CITY,                // Reverb_City
        XAUDIO2FX_I3DL2_PRESET_MOUNTAINS,           // Reverb_Mountains
        XAUDIO2FX_I3DL2_PRESET_QUARRY,              // Reverb_Quarry
        XAUDIO2FX_I3DL2_PRESET_PLAIN,               // Reverb_Plain
        XAUDIO2FX_I3DL2_PRESET_PARKINGLOT,          // Reverb_ParkingLot
        XAUDIO2FX_I3DL2_PRESET_SEWERPIPE,           // Reverb_SewerPipe
        XAUDIO2FX_I3DL2_PRESET_UNDERWATER,          // Reverb_Underwater
        XAUDIO2FX_I3DL2_PRESET_SMALLROOM,           // Reverb_SmallRoom
        XAUDIO2FX_I3DL2_PRESET_MEDIUMROOM,          // Reverb_MediumRoom
        XAUDIO2FX_I3DL2_PRESET_LARGEROOM,           // Reverb_LargeRoom
        XAUDIO2FX_I3DL2_PRESET_MEDIUMHALL,          // Reverb_MediumHall
        XAUDIO2FX_I3DL2_PRESET_LARGEHALL,           // Reverb_LargeHall
        XAUDIO2FX_I3DL2_PRESET_PLATE,               // Reverb_Plate
    };
}

static_assert( _countof(gReverbPresets) == Reverb_MAX, "AUDIO_ENGINE_REVERB enum mismatch" );


//======================================================================================
// AudioEngine
//======================================================================================

// Internal object implementation class.
class AudioEngine::Impl
{
public:
    Impl() :
        mMasterVoice( nullptr ),
        mReverbVoice( nullptr ),
        masterChannelMask( 0 ),
        masterChannels( 0 ),
        masterRate( 0 ),
        mCriticalError( false ),
        mReverbEnabled( false ),
        mEngineFlags( AudioEngine_Default ),
        mCategory( AudioCategory_GameEffects )
    {
        memset( &mX3DAudio, 0, X3DAUDIO_HANDLE_BYTESIZE );
    };

    HRESULT Initialize( AUDIO_ENGINE_FLAGS flags, _In_opt_ const WAVEFORMATEX* wfx, _In_opt_z_ const wchar_t* deviceId, AUDIO_STREAM_CATEGORY category );

    HRESULT Reset( _In_opt_ const WAVEFORMATEX* wfx, _In_opt_z_ const wchar_t* deviceId );

    void SetSilentMode();

    void Shutdown();

    bool Update();

    void SetReverb( _In_opt_ const XAUDIO2FX_REVERB_PARAMETERS* native );

    AudioStatistics GetStatistics() const;
    
    void AllocateVoice( _In_ const WAVEFORMATEX* wfx, SOUND_EFFECT_INSTANCE_FLAGS flags, bool oneshot, _Outptr_ IXAudio2SourceVoice** voice );

    void RegisterNotify( _In_ IVoiceNotify* notify, bool usesUpdate );
    void UnregisterNotify( _In_ IVoiceNotify* notify, bool oneshots, bool usesUpdate );

    ComPtr<IXAudio2>                    xaudio2;
    IXAudio2MasteringVoice*             mMasterVoice;
    IXAudio2SubmixVoice*                mReverbVoice;
 
    uint32_t                            masterChannelMask;
    uint32_t                            masterChannels;
    uint32_t                            masterRate;

    X3DAUDIO_HANDLE                     mX3DAudio;

    bool                                mCriticalError;
    bool                                mReverbEnabled;

    AUDIO_ENGINE_FLAGS                  mEngineFlags;

private:
    AUDIO_STREAM_CATEGORY               mCategory;
    ComPtr<IUnknown>                    mReverbEffect;
    std::list<IXAudio2SourceVoice*>     mOneShots;
    std::set<IVoiceNotify*>             mNotifyObjects;
    std::set<IVoiceNotify*>             mNotifyUpdates;
    VoiceCallback                       mVoiceCallback;
    EngineCallback                      mEngineCallback;
};


_Use_decl_annotations_
HRESULT AudioEngine::Impl::Initialize( AUDIO_ENGINE_FLAGS flags, const WAVEFORMATEX* wfx, const wchar_t* deviceId, AUDIO_STREAM_CATEGORY category )
{
    mEngineFlags = flags;
    mCategory = category;

    return Reset( wfx, deviceId );
}


_Use_decl_annotations_
HRESULT AudioEngine::Impl::Reset( const WAVEFORMATEX* wfx, const wchar_t* deviceId )
{
    if ( wfx )
    {
        if ( wfx->wFormatTag != WAVE_FORMAT_PCM )
            return HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED );

        if ( !wfx->nChannels || wfx->nChannels > XAUDIO2_MAX_AUDIO_CHANNELS )
            return HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED );

        if ( wfx->nSamplesPerSec < XAUDIO2_MIN_SAMPLE_RATE || wfx->nSamplesPerSec > XAUDIO2_MAX_SAMPLE_RATE )
            return HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED );

        // We don't use other data members of WAVEFORMATEX here to describe the device format, so no need to fully validate
    }

    assert( !mMasterVoice );
    assert( !mReverbVoice );

    masterChannelMask = masterChannels = masterRate = 0;

    memset( &mX3DAudio, 0, X3DAUDIO_HANDLE_BYTESIZE );

    mCriticalError = false;
    mReverbEnabled = false;

    //
    // Create XAudio2 engine
    //
    UINT32 eflags = 0;
#if (_WIN32_WINNT < _WIN32_WINNT_WIN8)
    if ( mEngineFlags & AudioEngine_Debug )
    {
        eflags |= XAUDIO2_DEBUG_ENGINE;
        OutputDebugStringA( "INFO: XAudio 2.7 debugging enabled\n" );
    }
#endif

    HRESULT hr = XAudio2Create( xaudio2.ReleaseAndGetAddressOf(), eflags );
    if( FAILED( hr ) )
    {
#if (_WIN32_WINNT < _WIN32_WINNT_WIN8)
        DebugTrace( "ERROR: XAudio 2.7 not found (have you called CoInitialize? Do you have the Developer Runtime installed?)\n" );
#endif
        return hr;
    }

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
    if ( mEngineFlags & AudioEngine_Debug )
    {
        // To see the trace output, you need to view ETW logs for this application:
        //    Go to Control Panel, Administrative Tools, Event Viewer.
        //    View->Show Analytic and Debug Logs.
        //    Applications and Services Logs / Microsoft / Windows / XAudio2. 
        //    Right click on Microsoft Windows XAudio2 debug logging, Properties, then Enable Logging, and hit OK 
        XAUDIO2_DEBUG_CONFIGURATION debug ={0};
        debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
        debug.BreakMask = XAUDIO2_LOG_ERRORS;
        xaudio2->SetDebugConfiguration( &debug, 0 );
        OutputDebugStringA( "INFO: XAudio 2.8 debugging enabled\n" );
    }
#endif

    hr = xaudio2->RegisterForCallbacks( &mEngineCallback );
    if ( FAILED(hr) )
    {
        xaudio2.Reset();
        return hr;
    }

    //
    // Create mastering voice for device
    //

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)

    hr = xaudio2->CreateMasteringVoice( &mMasterVoice,
                                        (wfx) ? wfx->nChannels : XAUDIO2_DEFAULT_CHANNELS,
                                        (wfx) ? wfx->nSamplesPerSec : XAUDIO2_DEFAULT_SAMPLERATE,
                                        0, deviceId, nullptr, mCategory );
    if ( FAILED(hr) )
    {
        xaudio2.Reset();
        return hr;
    }

    DWORD dwChannelMask;
    hr = mMasterVoice->GetChannelMask( &dwChannelMask );
    if ( FAILED(hr) )
    {
        mMasterVoice = nullptr;
        xaudio2.Reset();
        return hr;
    }

    XAUDIO2_VOICE_DETAILS details;
    mMasterVoice->GetVoiceDetails( &details );

    masterChannelMask = dwChannelMask;
    masterChannels = details.InputChannels;
    masterRate = details.InputSampleRate;

#else

    UINT32 devIndex = 0;
    if ( deviceId )
    {
        // Translate device ID back into device index
        UINT32 count = 0;
        hr = xaudio2->GetDeviceCount( &count );
        if ( FAILED(hr) )
        {
            xaudio2.Reset();
            return hr;
        }

        devIndex = UINT32(-1);
        for( UINT32 j = 0; j < count; ++j )
        {
            XAUDIO2_DEVICE_DETAILS details;
            hr = xaudio2->GetDeviceDetails( j, &details );
            if ( SUCCEEDED(hr) )
            {
                if ( wcsncmp( deviceId, details.DeviceID, 256 ) == 0 )
                {
                    devIndex = j;
                    masterChannelMask = details.OutputFormat.dwChannelMask;
                    break;
                }
            }
        }

        if ( devIndex == UINT32(-1) )
        {
            xaudio2.Reset();
            return E_FAIL;
        }
    }
    else
    {
        // No search needed
        XAUDIO2_DEVICE_DETAILS details;
        hr = xaudio2->GetDeviceDetails( 0, &details );
        if ( FAILED(hr) )
        {
            xaudio2.Reset();
            return hr;
        }

        masterChannelMask = details.OutputFormat.dwChannelMask;
    }

    hr = xaudio2->CreateMasteringVoice( &mMasterVoice,
                                        (wfx) ? wfx->nChannels : XAUDIO2_DEFAULT_CHANNELS,
                                        (wfx) ? wfx->nSamplesPerSec : XAUDIO2_DEFAULT_SAMPLERATE,
                                        0, devIndex, nullptr );
    if ( FAILED(hr) )
    {
        xaudio2.Reset();
        return hr;
    }

    XAUDIO2_VOICE_DETAILS details;
    mMasterVoice->GetVoiceDetails( &details );

    masterChannels = details.InputChannels;
    masterRate = details.InputSampleRate;

#endif

    //
    // Setup environmental reverb for 3D audio (optional)
    //
    if ( mEngineFlags & AudioEngine_EnvironmentalReverb )
    {
        UINT32 rflags = 0;
#if (_WIN32_WINNT < _WIN32_WINNT_WIN8)
        if ( mEngineFlags & AudioEngine_Debug )
        {
            rflags |= XAUDIO2FX_DEBUG;
        }
#endif
        hr = XAudio2CreateReverb( mReverbEffect.ReleaseAndGetAddressOf(), rflags );
        if ( FAILED(hr) )
        {
            mMasterVoice = nullptr;
            xaudio2.Reset();
            return hr;
        }

        XAUDIO2_EFFECT_DESCRIPTOR effects[] = { { mReverbEffect.Get(), TRUE, 1 } };
        XAUDIO2_EFFECT_CHAIN effectChain = { 1, effects };

        mReverbEnabled = true;

        hr = xaudio2->CreateSubmixVoice( &mReverbVoice, 1, masterRate,
                                         (mEngineFlags & AudioEngine_ReverbUseFilters ) ? XAUDIO2_VOICE_USEFILTER : 0, 0,
                                         nullptr, &effectChain );
        if ( FAILED(hr) )
        {
            mMasterVoice = nullptr;
            mReverbEffect.Reset();
            xaudio2.Reset();
            return hr;
        }

        XAUDIO2FX_REVERB_PARAMETERS native;
        ReverbConvertI3DL2ToNative( &gReverbPresets[ Reverb_Default ], &native );
        hr = mReverbVoice->SetEffectParameters( 0, &native, sizeof( XAUDIO2FX_REVERB_PARAMETERS ) );
        if ( FAILED(hr) )
        {
            mMasterVoice = nullptr;
            mReverbVoice = nullptr;
            mReverbEffect.Reset();
            xaudio2.Reset();
            return hr;
        }
    }

    //
    // Setup 3D audio
    //
    const float SPEEDOFSOUND = X3DAUDIO_SPEED_OF_SOUND;

#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
    hr = X3DAudioInitialize( masterChannelMask, SPEEDOFSOUND, mX3DAudio );
    if ( FAILED(hr) )
    {
        mMasterVoice = nullptr;
        mReverbVoice = nullptr;
        mReverbEffect.Reset();
        xaudio2.Reset();
        return hr;
    }
#else
    X3DAudioInitialize( masterChannelMask, SPEEDOFSOUND, mX3DAudio );
#endif

    //
    // Inform any notify objects we are ready to go again
    //
    for( auto it = mNotifyObjects.begin(); it != mNotifyObjects.end(); ++it )
    {
        assert( *it != 0 );
        (*it)->OnReset();
    }

    return S_OK;
}


void AudioEngine::Impl::SetSilentMode()
{
    for( auto it = mNotifyObjects.begin(); it != mNotifyObjects.end(); ++it )
    {
        assert( *it != 0 );
        (*it)->OnCriticalError();
    }

    mOneShots.clear();

    mMasterVoice = nullptr;
    mReverbVoice = nullptr;

    mReverbEffect.Reset();
    xaudio2.Reset();
}


void AudioEngine::Impl::Shutdown()
{
    for( auto it = mNotifyObjects.begin(); it != mNotifyObjects.end(); ++it )
    {
        assert( *it != 0 );
        (*it)->OnDestroyEngine();
    }

    if ( xaudio2 )
    {
        xaudio2->UnregisterForCallbacks( &mEngineCallback );

        xaudio2->StopEngine();

        mMasterVoice = nullptr;
        mReverbVoice = nullptr;

        mReverbEffect.Reset();
        xaudio2.Reset();

        masterChannelMask = masterChannels = masterRate = 0;

        mCriticalError = false;
        mReverbEnabled = false;

        memset( &mX3DAudio, 0, X3DAUDIO_HANDLE_BYTESIZE );
    }
}


bool AudioEngine::Impl::Update()
{
    if ( !xaudio2 )
        return false;

    HANDLE events[2] = { mEngineCallback.mCriticalError, mVoiceCallback.mBufferEnd };
    DWORD result = WaitForMultipleObjectsEx( 2, events, FALSE, 0, FALSE );
    switch( result )
    {
    case WAIT_TIMEOUT:
        break;

    case WAIT_OBJECT_0:     // OnCritialError
        mCriticalError = true;

        SetSilentMode();
        return false;
    
    case WAIT_OBJECT_0 + 1: // OnBufferEnd
        for( auto it = mOneShots.begin(); it != mOneShots.end(); )
        {
            assert( *it != 0 );

            XAUDIO2_VOICE_STATE state;
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
            (*it)->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED );
#else
            (*it)->GetState(&state);
#endif

            if ( !state.BuffersQueued )
            {
                (*it)->Stop( 0 );
                (*it)->DestroyVoice();
                it = mOneShots.erase( it );
            }
            else
                ++it;
        }
        break;

    case WAIT_FAILED:
        throw std::exception( "WaitForMultipleObjects" );
    }

    //
    // Inform any notify objects of updates
    //
    for( auto it = mNotifyUpdates.begin(); it != mNotifyUpdates.end(); ++it )
    {
        assert( *it != 0 );
        (*it)->OnUpdate();
    }

    return true;
}


_Use_decl_annotations_
void AudioEngine::Impl::SetReverb( const XAUDIO2FX_REVERB_PARAMETERS* native )
{
    if ( !mReverbVoice )
        return;

    if ( native )
    {
        if ( !mReverbEnabled )
        {
            mReverbEnabled = true;
            mReverbVoice->EnableEffect( 0 );
        }

        mReverbVoice->SetEffectParameters( 0, native, sizeof( XAUDIO2FX_REVERB_PARAMETERS ) );
    }
    else if ( mReverbEnabled )
    {
        mReverbEnabled = false;
        mReverbVoice->DisableEffect( 0 );
    }
}


AudioStatistics AudioEngine::Impl::GetStatistics() const
{
    AudioStatistics stats;
    memset( &stats, 0, sizeof(stats) );

    stats.allocatedVoices = stats.allocatedVoicesOneShot = mOneShots.size();

    for( auto it = mNotifyObjects.begin(); it != mNotifyObjects.end(); ++it )
    {
        assert( *it != 0 );
        (*it)->GatherStatistics( stats );
    }

    return stats;
}


_Use_decl_annotations_
void AudioEngine::Impl::AllocateVoice( const WAVEFORMATEX* wfx, SOUND_EFFECT_INSTANCE_FLAGS flags, bool oneshot, IXAudio2SourceVoice** voice )
{
    if ( !xaudio2 || mCriticalError )
    {
        voice = nullptr;
        return;
    }

    // No need to call IsValid on wfx because CreateSourceVoice will do that

    if ( oneshot )
    {
        // TODO - Add voice resuse (similiar format, can change rate, flags)
        // Need DefaultSampleRate and MaxFrequencyRatio as settings
    }

    HRESULT hr;
    if ( flags & SoundEffectInstance_Use3D )
    {
        XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];      
        sendDescriptors[0].Flags = sendDescriptors[1].Flags = (flags & SoundEffectInstance_ReverbUseFilters) ? XAUDIO2_SEND_USEFILTER : 0;
        sendDescriptors[0].pOutputVoice = mMasterVoice;
        sendDescriptors[1].pOutputVoice = mReverbVoice;
        const XAUDIO2_VOICE_SENDS sendList = { mReverbVoice ? 2 : 1, sendDescriptors };

        hr = xaudio2->CreateSourceVoice( voice, wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &mVoiceCallback, &sendList, nullptr );
    }
    else
    {
        hr = xaudio2->CreateSourceVoice( voice, wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &mVoiceCallback, nullptr, nullptr );
    }

    if ( FAILED(hr) )
    {
        DebugTrace( "ERROR: CreateSourceVoice failed with error %08X\n", hr );
        throw std::exception( "CreateSourceVoice" );
    }

    if ( oneshot )
    {
        assert( *voice != 0 );
        mOneShots.push_back( *voice );
    }
}


void AudioEngine::Impl::RegisterNotify( _In_ IVoiceNotify* notify, bool usesUpdate )
{
    assert( notify != 0 );
    mNotifyObjects.insert( notify );

    if ( usesUpdate )
    {
        mNotifyUpdates.insert( notify );
    }
}


void AudioEngine::Impl::UnregisterNotify( _In_ IVoiceNotify* notify, bool usesOneShots, bool usesUpdate )
{
    assert( notify != 0 );
    mNotifyObjects.erase( notify );

    // Check for any pending one-shots for this notification object
    if ( usesOneShots )
    {
        bool setevent = false;

        for( auto it = mOneShots.begin(); it != mOneShots.end(); ++it )
        {
            assert( *it != 0 );

            XAUDIO2_VOICE_STATE state;
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
            (*it)->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED );
#else
            (*it)->GetState(&state);
#endif

            if ( state.pCurrentBufferContext == notify )
            {
                (*it)->Stop( 0 );
                (*it)->FlushSourceBuffers();
                setevent = true;
            }
        }

        if (setevent)
        {
            // Trigger scan on next call to Update...
            SetEvent( mVoiceCallback.mBufferEnd );
        }
    }

    if ( usesUpdate )
    {
        mNotifyUpdates.erase( notify );
    }
}


//--------------------------------------------------------------------------------------
// AudioEngine
//--------------------------------------------------------------------------------------

// Public constructor.
_Use_decl_annotations_
AudioEngine::AudioEngine( AUDIO_ENGINE_FLAGS flags, const WAVEFORMATEX* wfx, const wchar_t* deviceId, AUDIO_STREAM_CATEGORY category )
  : pImpl(new Impl() )
{
    HRESULT hr = pImpl->Initialize( flags, wfx, deviceId, category );
    if ( FAILED(hr) )
    {
        if ( hr == HRESULT_FROM_WIN32( ERROR_NOT_FOUND ) )
        {
            if ( flags & AudioEngine_ThrowOnNoAudioHW )
            {
                DebugTrace( "ERROR: AudioEngine found no default audio device\n" );
                throw std::exception( "AudioEngineNoAudioHW" );
            }
            else
            {
                DebugTrace( "WARNING: AudioEngine found no default audio device; running in 'silent mode'\n" );
            }
        }
        else
        {
            DebugTrace( "ERROR: AudioEngine failed (%08X) to initialize using device [%S]\n", hr, ( deviceId ) ? deviceId : L"default" );
            throw std::exception( "AudioEngine" );
        }
    }
}


// Move constructor.
AudioEngine::AudioEngine(AudioEngine&& moveFrom)
  : pImpl(std::move(moveFrom.pImpl))
{
}


// Move assignment.
AudioEngine& AudioEngine::operator= (AudioEngine&& moveFrom)
{
    pImpl = std::move(moveFrom.pImpl);
    return *this;
}


// Public destructor.
AudioEngine::~AudioEngine()
{
    if ( pImpl )
    {
        pImpl->Shutdown();
    }
}


// Public methods.
bool AudioEngine::Update()
{
    return pImpl->Update();
}


_Use_decl_annotations_
bool AudioEngine::Reset( const WAVEFORMATEX* wfx, const wchar_t* deviceId )
{
    if ( pImpl->xaudio2 )
    {
        DebugTrace( "WARNING: Called Reset for active audio graph; going silent in preparation for migration" );
        pImpl->SetSilentMode();
    }

    HRESULT hr = pImpl->Reset( wfx, deviceId );
    if ( FAILED(hr) )
    {
        if ( hr == HRESULT_FROM_WIN32( ERROR_NOT_FOUND ) )
        {
            if ( pImpl->mEngineFlags & AudioEngine_ThrowOnNoAudioHW )
            {
                DebugTrace( "ERROR: AudioEngine found no default audio device on Reset\n" );
                throw std::exception( "AudioEngineNoAudioHW" );
            }
            else
            {
                DebugTrace( "WARNING: AudioEngine found no default audio device on Reset; running in 'silent mode'\n" );
                return false;
            }
        }
        else
        {
            DebugTrace( "ERROR: AudioEngine failed (%08X) to Reset using device [%S]\n", hr, ( deviceId ) ? deviceId : L"default" );
            throw std::exception( "AudioEngine::Reset" );
        }
    }

    DebugTrace( "INFO: AudioEngine Reset using device [%S]\n", ( deviceId ) ? deviceId : L"default" );

    return true;
}


void AudioEngine::Suspend()
{
    if ( !pImpl->xaudio2 )
        return;

    pImpl->xaudio2->StopEngine();
}
 

void AudioEngine::Resume()
{
    if ( !pImpl->xaudio2 )
        return;

    HRESULT hr = pImpl->xaudio2->StartEngine();
    ThrowIfFailed( hr );
}


void AudioEngine::SetReverb( AUDIO_ENGINE_REVERB reverb )
{
    if ( reverb >= Reverb_MAX )
        throw std::out_of_range( "AudioEngine::SetReverb" );

    if ( reverb == Reverb_Off )
    {
        pImpl->SetReverb( nullptr );
    }
    else
    {
        XAUDIO2FX_REVERB_PARAMETERS native;
        ReverbConvertI3DL2ToNative( &gReverbPresets[ reverb ], &native );
        pImpl->SetReverb( &native );
    }
}


_Use_decl_annotations_
void AudioEngine::SetReverb( const XAUDIO2FX_REVERB_PARAMETERS* native )
{
    pImpl->SetReverb( native );
}


// Public accessors.
AudioStatistics AudioEngine::GetStatistics() const
{
    return pImpl->GetStatistics();
}


WAVEFORMATEXTENSIBLE AudioEngine::GetOutputFormat() const
{
    WAVEFORMATEXTENSIBLE wfx;
    memset( &wfx, 0, sizeof(WAVEFORMATEXTENSIBLE) );

    if ( !pImpl->xaudio2 )
        return wfx;

    wfx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
    wfx.Format.wBitsPerSample = wfx.Samples.wValidBitsPerSample = 16; // This is a guess
    wfx.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);

    wfx.Format.nChannels = static_cast<WORD>( pImpl->masterChannels );
    wfx.Format.nSamplesPerSec = pImpl->masterRate;
    wfx.dwChannelMask = pImpl->masterChannelMask;

    wfx.Format.nBlockAlign = WORD( wfx.Format.nChannels * wfx.Format.wBitsPerSample / 8 );
    wfx.Format.nAvgBytesPerSec = wfx.Format.nSamplesPerSec * wfx.Format.nBlockAlign;

    static const GUID s_pcm = { WAVE_FORMAT_PCM, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 };
    memcpy( &wfx.SubFormat, &s_pcm, sizeof(GUID) );

    return wfx;
}


uint32_t AudioEngine::GetChannelMask() const
{
    return pImpl->masterChannelMask;
}


int AudioEngine::GetOutputChannels() const
{
    return pImpl->masterChannels;
}


bool AudioEngine::IsAudioDevicePresent() const
{
    return ( pImpl->xaudio2.Get() != 0 ) && !pImpl->mCriticalError;
}


bool AudioEngine::IsCriticalError() const
{
    return pImpl->mCriticalError;
}


// Voice management.
_Use_decl_annotations_
void AudioEngine::AllocateVoice( const WAVEFORMATEX* wfx, SOUND_EFFECT_INSTANCE_FLAGS flags, bool oneshot, IXAudio2SourceVoice** voice )
{
    pImpl->AllocateVoice( wfx, flags, oneshot, voice );
}


void AudioEngine::RegisterNotify( _In_ IVoiceNotify* notify, bool usesUpdate )
{
    pImpl->RegisterNotify( notify, usesUpdate );
}


void AudioEngine::UnregisterNotify( _In_ IVoiceNotify* notify, bool oneshots, bool usesUpdate )
{
    pImpl->UnregisterNotify( notify, oneshots, usesUpdate );
}


IXAudio2* AudioEngine::GetInterface() const
{
    return pImpl->xaudio2.Get();
}


IXAudio2MasteringVoice* AudioEngine::GetMasterVoice() const
{
    return pImpl->mMasterVoice;
}


IXAudio2SubmixVoice* AudioEngine::GetReverbVoice() const
{
    return pImpl->mReverbVoice;
}


X3DAUDIO_HANDLE& AudioEngine::Get3DHandle() const
{
    return pImpl->mX3DAudio;
}


// Static methods.
#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#include <phoneaudioclient.h>
#elif defined(_XBOX_ONE)
#include <Windows.Media.Devices.h>
#include <wrl.h>
#elif (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
#pragma comment(lib,"runtimeobject.lib")
#include <Windows.Devices.Enumeration.h>
#include <wrl.h>
#endif

std::vector<AudioEngine::RendererDetail> AudioEngine::GetRendererDetails()
{
    std::vector<RendererDetail> list;

#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP

    LPCWSTR id = GetDefaultAudioRenderId( Default );
    if ( !id )
        return list;

    RendererDetail device;
    device.deviceId = id;
    device.description = L"Default";

    CoTaskMemFree( (LPVOID)id );

#elif defined(_XBOX_ONE)

    using namespace Microsoft::WRL;
    using namespace Microsoft::WRL::Wrappers;
    using namespace ABI::Windows::Media::Devices;

    Microsoft::WRL::ComPtr<IMediaDeviceStatics> mdStatics;
    HRESULT hr = ABI::Windows::Foundation::GetActivationFactory( HStringReference(RuntimeClass_Windows_Media_Devices_MediaDevice).Get(), &mdStatics );
    ThrowIfFailed( hr );

    HString id;
    hr = mdStatics->GetDefaultAudioRenderId( AudioDeviceRole_Default, id.GetAddressOf() );
    ThrowIfFailed( hr );

    RendererDetail device;
    device.deviceId = id.GetRawBuffer( nullptr );
    device.description = L"Default";
    list.emplace_back( device );

#elif (_WIN32_WINNT >= _WIN32_WINNT_WIN8)

#if defined(__cplusplus_winrt)

    // Enumerating with WinRT using C++/CX (Windows Store apps)
    using Windows::Devices::Enumeration::DeviceClass;
    using Windows::Devices::Enumeration::DeviceInformation;
    using Windows::Devices::Enumeration::DeviceInformationCollection;

    auto operation = DeviceInformation::FindAllAsync(DeviceClass::AudioRender);
    while (operation->Status != Windows::Foundation::AsyncStatus::Completed)
        ;

    DeviceInformationCollection^ devices = operation->GetResults();

    for (unsigned i = 0; i < devices->Size; ++i)
    {
        using Windows::Devices::Enumeration::DeviceInformation;

        DeviceInformation^ d = devices->GetAt(i);

        RendererDetail device;
        device.deviceId = d->Id->Data();
        device.description = d->Name->Data();
        list.emplace_back(device);
    }
#else

    // Enumerating with WinRT using WRL (Win32 desktop app for Windows 8.x)
    using namespace Microsoft::WRL;
    using namespace Microsoft::WRL::Wrappers;
    using namespace ABI::Windows::Foundation;
    using namespace ABI::Windows::Foundation::Collections;
    using namespace ABI::Windows::Devices::Enumeration;

    RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
    HRESULT hr = initialize;
    ThrowIfFailed( hr );

    Microsoft::WRL::ComPtr<IDeviceInformationStatics> diFactory;
    hr = ABI::Windows::Foundation::GetActivationFactory( HStringReference(RuntimeClass_Windows_Devices_Enumeration_DeviceInformation).Get(), &diFactory );
    ThrowIfFailed( hr );

    Event findCompleted( CreateEventEx( nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, WRITE_OWNER | EVENT_ALL_ACCESS ) );
    if ( !findCompleted.IsValid() )
        throw std::exception( "CreateEventEx" );

    auto callback = Callback<IAsyncOperationCompletedHandler<DeviceInformationCollection*>>(
        [&findCompleted,list]( IAsyncOperation<DeviceInformationCollection*>* aDevices, AsyncStatus status ) -> HRESULT
    {
        UNREFERENCED_PARAMETER(aDevices);
        UNREFERENCED_PARAMETER(status);
        SetEvent( findCompleted.Get() );
        return S_OK;
    });

    ComPtr<IAsyncOperation<DeviceInformationCollection*>> operation;
    hr = diFactory->FindAllAsyncDeviceClass( DeviceClass_AudioRender, operation.GetAddressOf() );
    ThrowIfFailed( hr );

    operation->put_Completed( callback.Get() );

    (void)WaitForSingleObjectEx( findCompleted.Get(), INFINITE, FALSE );

    ComPtr<IVectorView<DeviceInformation*>> devices;
    operation->GetResults( devices.GetAddressOf() );

    unsigned int count = 0;
    hr = devices->get_Size( &count );
    ThrowIfFailed( hr );

    if ( !count )
        return list;

    for( unsigned int j = 0; j < count; ++j )
    {
        ComPtr<IDeviceInformation> deviceInfo;
        hr = devices->GetAt( j, deviceInfo.GetAddressOf() );
        if ( SUCCEEDED(hr) )
        {
            HString id;
            deviceInfo->get_Id( id.GetAddressOf() );

            HString name;
            deviceInfo->get_Name( name.GetAddressOf() );

            RendererDetail device;
            device.deviceId = id.GetRawBuffer( nullptr );
            device.description = name.GetRawBuffer( nullptr );
            list.emplace_back( device );
        }
    }

#endif 

#else // _WIN32_WINNT < _WIN32_WINNT_WIN8

    // Enumerating with XAudio 2.7
    ComPtr<IXAudio2> pXAudio2;

    HRESULT hr = XAudio2Create( pXAudio2.GetAddressOf() );
    if ( FAILED(hr) )
    {
        DebugTrace( "ERROR: XAudio 2.7 not found (have you called CoInitialize?)\n");
        throw std::exception( "XAudio2Create" );
    }

    UINT32 count = 0;
    hr = pXAudio2->GetDeviceCount( &count );
    ThrowIfFailed(hr);

    if ( !count )
        return list;

    list.reserve( count );

    for( UINT32 j = 0; j < count; ++j )
    {
        XAUDIO2_DEVICE_DETAILS details;
        hr = pXAudio2->GetDeviceDetails( j, &details );
        if ( SUCCEEDED(hr) )
        {
            RendererDetail device;
            device.deviceId = details.DeviceID;
            device.description = details.DisplayName;
            list.emplace_back( device );
        }
    }

#endif

    return list;
}