/*
 * Copyright (C) 2015 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "ignition/rendering/PixelFormat.hh"
#include "gazebo/common/Console.hh"

using namespace ignition;
using namespace rendering;

//////////////////////////////////////////////////
const char *PixelUtil::names[PF_COUNT] =
    {
      "UNKNOWN",
      "L8",
      "R8G8B8",
      "B8G8R8",
      "BAYER_RGGB8",
      "BAYER_BGGR8",
      "BAYER_GBGR8",
      "BAYER_GRGB8",
    };

//////////////////////////////////////////////////
const unsigned char PixelUtil::channelCounts[PF_COUNT] =
    {
      0, // UKNOWN
      1, // L8
      3, // R8G8B8
      3, // B8G8R8
      4, // BAYER_RGGB8
      4, // BAYER_BGGR8
      4, // BAYER_GBGR8
      4, // BAYER_GRGB8
    };

//////////////////////////////////////////////////
const unsigned char PixelUtil::channelByteCounts[PF_COUNT] =
    {
      0, // UKNOWN
      1, // L8
      1, // R8G8B8
      1, // B8G8R8
      1, // BAYER_RGGB8
      1, // BAYER_BGGR8
      1, // BAYER_GBGR8
      1, // BAYER_GRGB8
    };

//////////////////////////////////////////////////
bool PixelUtil::IsValid(PixelFormat _format)
{
  return _format > 0 && _format < PF_COUNT;
}

//////////////////////////////////////////////////
PixelFormat PixelUtil::Sanitize(PixelFormat _format)
{
  // check if value within enum bounds
  if (!PixelUtil::IsValid(_format))
  {
    gzerr << "Invalid PixelFormat value: " << _format << std::endl;
    return PF_UNKNOWN;
  }

  return _format;
}

//////////////////////////////////////////////////
std::string PixelUtil::GetName(PixelFormat _format)
{
  _format = PixelUtil::Sanitize(_format);
  return PixelUtil::names[_format];
}

//////////////////////////////////////////////////
unsigned int PixelUtil::GetChannelCount(PixelFormat _format)
{
  _format = PixelUtil::Sanitize(_format);
  return PixelUtil::channelCounts[_format];
}

//////////////////////////////////////////////////
unsigned int PixelUtil::GetBytesPerChannel(PixelFormat _format)
{
  _format = PixelUtil::Sanitize(_format);
  return PixelUtil::channelByteCounts[_format];
}

//////////////////////////////////////////////////
unsigned int PixelUtil::GetBytesPerPixel(PixelFormat _format)
{
  _format = PixelUtil::Sanitize(_format);
  unsigned int channels = PixelUtil::channelCounts[_format];
  unsigned int bytes = PixelUtil::channelByteCounts[_format];
  return channels * bytes;
}

//////////////////////////////////////////////////
unsigned int PixelUtil::GetMemorySize(PixelFormat _format, unsigned int _width,
    unsigned int _height)
{
  unsigned int bytesPerPixel = PixelUtil::GetBytesPerPixel(_format);
  return _width * _height * bytesPerPixel;
}

//////////////////////////////////////////////////
PixelFormat PixelUtil::GetEnum(const std::string &_name)
{
  // search over all enum elements
  for (unsigned int i = 1; i < PF_COUNT; ++i)
  {
    PixelFormat format = static_cast<PixelFormat>(i);

    // check if names match
    if (PixelUtil::GetName(format) == _name)
    {
      return format;
    }
  }

  // no match found
  return PF_UNKNOWN;
}