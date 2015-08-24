/* * Copyright (C) 2015 Open Source Robotics Foundation
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
#include "ignition/rendering/optix/OptixTextureFactory.hh"
#include "ignition/rendering/optix/OptixScene.hh"
#include "gazebo/common/Console.hh"
#include <FreeImage.h>

using namespace ignition;
using namespace rendering;

// TODO: clone texture sampler, reuse texture buffers

OptixTextureFactory::OptixTextureFactory(OptixScenePtr _scene) :
  scene(_scene)
{
}

//////////////////////////////////////////////////
OptixTextureFactory::~OptixTextureFactory()
{
}

//////////////////////////////////////////////////
optix::TextureSampler OptixTextureFactory::Create(const std::string &_filename)
{
  optix::Buffer buffer = this->CreateBuffer(_filename);
  return this->CreateSampler(buffer);
}

//////////////////////////////////////////////////
optix::TextureSampler OptixTextureFactory::Create()
{
  optix::Buffer buffer = this->CreateBuffer();
  return this->CreateSampler(buffer);
}

//////////////////////////////////////////////////
optix::Buffer OptixTextureFactory::CreateBuffer(const std::string &_filename)
{
  if (_filename.empty())
  {
    gzerr << "Cannot load texture from empty filename" << std::endl;
    return this->CreateBuffer();
  }

  FREE_IMAGE_FORMAT format = FreeImage_GetFileType(_filename.c_str(), 0);
  FIBITMAP *image = FreeImage_Load(format, _filename.c_str());

  if (!image)
  {
    gzerr << "Unable to load texture: " << _filename << std::endl;
    return this->CreateBuffer();
  }

  FIBITMAP *temp = image;
  image = FreeImage_ConvertTo32Bits(image);
  FreeImage_Unload(temp);

  int w = FreeImage_GetWidth(image);
  int h = FreeImage_GetHeight(image);
  int p = FreeImage_GetPitch(image);
  int memSize = h * p;

  unsigned char *data = static_cast<unsigned char *>(FreeImage_GetBits(image));

  optix::Context optixContext = this->scene->GetOptixContext();

  optix::Buffer buffer = optixContext->createBuffer(RT_BUFFER_INPUT);
  buffer->setFormat(RT_FORMAT_UNSIGNED_BYTE4);
  buffer->setSize(w, h);
  std::memcpy(buffer->map(), data, memSize);
  buffer->unmap();
  FreeImage_Unload(image);

  return buffer;
}

//////////////////////////////////////////////////
optix::Buffer OptixTextureFactory::CreateBuffer()
{
  unsigned char data[4] = { 0, 0, 0, 0 };
  unsigned int memSize = sizeof(data);

  optix::Context optixContext = this->scene->GetOptixContext();

  optix::Buffer buffer = optixContext->createBuffer(RT_BUFFER_INPUT);
  buffer->setFormat(RT_FORMAT_UNSIGNED_BYTE4);
  buffer->setSize(1, 1);

  std::memcpy(buffer->map(), &data[0], memSize);
  buffer->unmap();

  return buffer;
}

//////////////////////////////////////////////////
optix::TextureSampler OptixTextureFactory::CreateSampler(optix::Buffer _buffer)
{
  optix::Context optixContext = this->scene->GetOptixContext();
  optix::TextureSampler sampler = optixContext->createTextureSampler();

  sampler->setWrapMode(0, RT_WRAP_REPEAT);
  sampler->setWrapMode(1, RT_WRAP_REPEAT);
  sampler->setWrapMode(2, RT_WRAP_REPEAT);

  sampler->setIndexingMode(RT_TEXTURE_INDEX_NORMALIZED_COORDINATES);
  sampler->setReadMode(RT_TEXTURE_READ_NORMALIZED_FLOAT);
  sampler->setMaxAnisotropy(1.0);
  sampler->setMipLevelCount(1.0);
  sampler->setArraySize(1);
  sampler->setBuffer(0, 0, _buffer);

  sampler->setFilteringModes(RT_FILTER_LINEAR, RT_FILTER_LINEAR,
      RT_FILTER_NONE);

  return sampler;
}