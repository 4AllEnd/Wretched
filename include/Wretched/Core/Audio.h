#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <glm/glm.hpp>

#include <al.h>
#include <alc.h>
#include <Wretched/System/Entity.h>
//#include <portaudio/portaudio.h>
#include <vector>
#include <inttypes.h>
#include <sndfile.h>

auto alGetErrorString = [](ALenum error) {
  switch (error) {
    case AL_INVALID_NAME: return "AL_INVALID_NAME";
      break;
    case AL_INVALID_ENUM: return "AL_INVALID_ENUM";
      break;
    case AL_INVALID_VALUE: return "AL_INVALID_VALUE";
      break;
    case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION";
      break;
    case AL_OUT_OF_MEMORY: return "AL_OUT_OF_MEMORY";
      break;
    case AL_NO_ERROR: return "";
      break;
    default: return "unknown";
  }
};

/*
struct RIFF_Header {
  char chunkID[4];
  char format[4];
  int chunkSize;//size not including chunkSize or chunkID
};

//Struct to hold fmt awsubchunk data for WAVE files.

struct WAVE_Format {
  char subChunkID[4];//4byte
  int subChunkSize; //4byte
  short audioFormat;//2byte
  short numChannels;//2byte
  int sampleRate;//4byte
  int byteRate;//4byte
  short blockAlign;//2byte
  short bitsPerSample;//2byte
};

//Struct to hold the data of the wave file

struct WAVE_Data {
  char subChunkID[4]; //should contain the word data
  int subChunk2Size; //Stores the size of the data block
};


static bool loadWavFile(const std::string filename, ALuint* buffer,
  ALsizei* size, ALsizei* frequency,
  ALenum* format) {
  //Local Declarations
  FILE* File = NULL;
  WAVE_Format wave_format;
  RIFF_Header riff_header;
  WAVE_Data wave_data;
  unsigned char* data;

  try {
#ifdef WIN32
    fopen_s(&File,filename.c_str(), "rb");
#else
    File = fopen(filename.c_str(), "rb");
#endif
    if (!File)
      throw (filename);

    // Read in the first chunk into the struct
    char GID[4],FMT[4];

    int blocksize;
    fread(&riff_header.chunkID, sizeof(char), 4, File);
    fread(&riff_header.format, sizeof(char), 4, File);
    fread(&riff_header.chunkSize, sizeof(int), 1, File);
    //memcpy(&riff_header.chunkSize,&blocksize,4);
    //check for RIFF and WAVE tag in memeory
    if (std::string(riff_header.chunkID,4)!="RIFF" ||
      std::string(riff_header.format,4)!="WAVE"){
      printf("didn't read header right, oh well\n GID: %s FMT: %s blocksize:%c\n",std::string(riff_header.chunkID,4).c_str(), std::string(riff_header.format, 4).c_str(), riff_header.chunkSize);
      //throw ("Invalid RIFF or WAVE Header");
      }
      

    //Read in the 2nd chunk for the wave info
    fread(&wave_format, sizeof(WAVE_Format), 1, File);
    //check for fmt tag in memory
    if (std::string(wave_format.subChunkID,4) != "fmt ")
      throw ("Invalid Wave Format");
    //check for extra parameters;
    if (wave_format.subChunkSize > 16)
      fseek(File, sizeof(short), SEEK_CUR);

    //Read in the the last byte of data before the sound file
    fread(&wave_data, sizeof(WAVE_Data), 1, File);
    //check for data tag in memory
    if (std::string(wave_data.subChunkID)!="data")
      throw ("Invalid data header");

    data = (unsigned char*)malloc(wave_data.subChunk2Size);

    // Read in the sound data into the soundData variable
    if (!fread(data, wave_data.subChunk2Size, 1, File))
      throw ("error loading WAVE data into struct!");

    *size = wave_data.subChunk2Size;
    *frequency = wave_format.sampleRate;
    if (wave_format.numChannels == 1) {
      wave_format.bitsPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
      }
    else if (wave_format.numChannels == 2) {
      *format = wave_format.bitsPerSample==8?AL_FORMAT_STEREO8: AL_FORMAT_STEREO16;
      }
    alBufferData(*buffer, *format, (virtual void*)data,*size, *frequency);
    fclose(File);
    return true;
  }
  catch (std::string error) {
    //our catch statement for if we throw a string
    std::cerr << error << " : trying to load "
      << filename << std::endl;
    //clean up memory if wave loading fails
    if (File != NULL)
      fclose(File);
    //return false to indicate the failure to load wave

    printf("OpenAL error: %s \n", alGetErrorString(alGetError()));
    printf("OpenAL error: %s \n", alGetErrorString(alGetError()));
    return false;
  }
}
*/

namespace Wretched {
  namespace Audio {
    struct Manager;
    struct Sound {
      std::string filename;
      ALenum format;
      ALsizei size;
      ALsizei freq;
      ALboolean loop;
      ALvoid *data = nullptr;
      ALuint buffer;
      ALuint source;
      Sound(Manager& mgr, const char *filename);
      Sound() {
        alGenBuffers(1, &buffer);
        alGenSources(1, &source);
      }
      ~Sound() {
        alDeleteSources(1, &source);
        alDeleteBuffers(1, &buffer);
      }
    };

    typedef glm::vec3 vec3;

    struct Manager {
      ALCdevice *device = nullptr;
      ALCcontext *context = nullptr;
      Entity::Manager *mgr = nullptr;
      std::vector<char*> cache;
      Manager(Entity::Manager &a) {
        mgr = &a;
        device = alcOpenDevice(nullptr);
        context = alcCreateContext(device, nullptr);
        alcMakeContextCurrent(context);
        alDistanceModel(AL_INVERSE_DISTANCE);
        alDopplerFactor(0);
      }

      ~Manager() {
        context = alcGetCurrentContext();
        device = alcGetContextsDevice(context);
        alcMakeContextCurrent(NULL);
        alcDestroyContext(context);
        alcCloseDevice(device);
      }

      virtual Sound* createSound(Entity &entity, const char *filename) {
        Sound *ptr = entity.createcomponent<Sound>(*this, filename);
        return ptr;
      }

      static void setgain(const float intensity) {
        alListenerf(AL_GAIN, intensity);
      }

      static void setposition(const vec3 position) {
        alListener3f(AL_POSITION, position.x, position.y, position.z);
      }

      static void setvelocity(const vec3 velocity) {
        alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
      }

      //set orientation for listener
      static void setorientation(const vec3 front, const vec3 up) {
        float ori[6] = {up.x,up.y,up.z,front.x,front.y,front.z};
        alListenerfv(AL_ORIENTATION, ori);
      }

      //set direction for sound
      static void setdirection(const Sound &sound, const vec3 direction) {
        alSource3f(sound.source, AL_DIRECTION, direction.x, direction.y, direction.z);
      }


      //set position for sound
      static void setposition(const Sound &sound, const vec3 position) {
        alSource3f(sound.source,AL_POSITION, position.x, position.y, position.z);
      }

      //set velocity for sound
      static void setvelocity(const Sound &sound, const vec3 velocity) {
        alSource3f(sound.source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
      }

      //set relative positioning
      static void setrelative(const Sound &sound, bool relativetoposition) {
        alSourcei(sound.source, AL_SOURCE_RELATIVE, relativetoposition);
      }

      //set gain for sound
      static void setgain(const Sound &sound, float intensity) {
        alSourcef(sound.source,AL_GAIN, intensity);
      }

      //play sound
      static void play(Sound &sound) {
        alSourcePlay(sound.source);
      }

      //pause sound
      static void pause(Sound &sound) {
        alSourcePause(sound.source);
      }

      //stop sound
      static void stop(Sound &sound) {
        alSourceStop(sound.source);
      }

      //seek sound
      static void seek(Sound &sound, float point) {
        ALint buffer_size;
        alGetBufferi(sound.buffer, AL_SIZE, &buffer_size);
        alSourcei(sound.source, AL_BYTE_OFFSET, buffer_size * point);
      }

      //set looping
      static void loop(Sound &sound, bool loopingon) {
        alSourcei(sound.source,AL_LOOPING, loopingon
        );
      }

      static bool isplaying(Sound &sound) {
        ALint state;
        alGetSourcei(sound.source,AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
      }

      static float getplaybackpos(Sound &sound) {
        ALint state;
        alGetSourcei(sound.source, AL_BYTE_OFFSET, &state);
        return float(state) / float(sound.size);
      }

      static float getplaybackpossec(Sound &sound) {
        ALfloat ff;
        alGetSourcef(sound.source, AL_SEC_OFFSET, &ff);
        return ff;
      }

      float getduration(Sound &sound) {
        ALfloat ff;
        switch (sound.format) {
          case AL_FORMAT_MONO8: ff = 1;
            break;
          case AL_FORMAT_MONO16: ff = 2;
            break;
          case AL_FORMAT_STEREO8: ff = 2;
            break;
          case AL_FORMAT_STEREO16: ff = 4;
            break;
          default: ff = 1;
        }
        return float(sound.size) / (ff * sound.freq);
      }

      static bool isstopped(Sound &sound) {
        ALint state;
        alGetSourcei(sound.source, AL_SOURCE_STATE, &state);
        return state == AL_STOPPED;
      }

      //load into sound
      static void load(Sound &sound, const char *filename) {
        //printf("error: %s \n", alGetErrorString(alGetError()));
        SF_INFO info;
        SNDFILE *file;
        ALenum format;
        void* buf;
        file = sf_open(filename, SFM_READ, &info);
        if(file == nullptr) {
          printf("Wretched error File %s will not load: %s \n", filename, sf_strerror(NULL));
          sf_close(file);
          return;
        } else {
          printf("...Loading Sound File %s\n", filename);
        }
        buf = malloc(info.frames * info.channels * sizeof(short));
        sf_readf_short(file, (short*)buf, info.frames);
        sf_close(file);
        format = info.samplerate > 8 ? (info.channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16)
                                     : (info.channels > 1? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8);
        sound.freq = info.samplerate;
        sound.size = (ALsizei)info.frames * info.channels * sizeof(short);
        sound.format = format;
        alBufferData(sound.buffer, format, buf, sound.size, info.samplerate);
        free(buf);

        //loadWavFile(filename,&sound.buffer,&sound.size,&sound.freq,&sound.format);
        alSourcei(sound.source, AL_BUFFER, sound.buffer);
      }

    };
    Sound::Sound(Manager& mgr, const char *filename):Sound() {
      this->filename = filename;
      mgr.load(*this, filename);
      alSourcef(this->source, AL_GAIN, .99f);
      //alSourcef(ptr->source, AL_MIN_GAIN, .0f);
      //alSourcef(ptr->source, AL_MAX_DISTANCE, 100000);
      alSourcef(this->source, AL_REFERENCE_DISTANCE, 5000);
      alSourcef(this->source, AL_ROLLOFF_FACTOR, 2);
      alSpeedOfSound(3500);
    }
  }
}