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
#ifndef IGNITION_RENDERING_BASE_BASERENDERENGINE_HH_
#define IGNITION_RENDERING_BASE_BASERENDERENGINE_HH_

#include <map>
#include <string>
#include <vector>
#include "ignition/rendering/RenderEngine.hh"
#include "ignition/rendering/Storage.hh"

namespace ignition
{
  namespace rendering
  {
    inline namespace IGNITION_RENDERING_VERSION_NAMESPACE {
    //
    class IGNITION_RENDERING_VISIBLE BaseRenderEngine :
      public virtual RenderEngine
    {
      protected: BaseRenderEngine();

      public: virtual ~BaseRenderEngine();

      // Documentation Inherited.
      public: virtual bool Load(
          const std::map<std::string, std::string> &_params = {}) override;

      public: virtual bool Init();

      public: virtual bool Fini();

      public: virtual bool IsLoaded() const;

      public: virtual bool IsInitialized() const;

      public: virtual bool IsEnabled() const;

      public: virtual unsigned int SceneCount() const;

      public: virtual bool HasScene(ConstScenePtr _scene) const;

      public: virtual bool HasSceneId(unsigned int _id) const;

      public: virtual bool HasSceneName(const std::string &_name) const;

      public: virtual ScenePtr SceneById(unsigned int _id) const;

      public: virtual ScenePtr SceneByName(const std::string &_name) const;

      public: virtual ScenePtr SceneByIndex(unsigned int _index) const;

      public: virtual void DestroyScene(ScenePtr _scene);

      public: virtual void DestroySceneById(unsigned int _id);

      public: virtual void DestroySceneByName(const std::string &_name);

      public: virtual void DestroySceneByIndex(unsigned int _index);

      public: virtual void DestroyScenes();

      public: virtual ScenePtr CreateScene(const std::string &_name);

      public: virtual ScenePtr CreateScene(unsigned int _id,
                  const std::string &_name);

      public: virtual void Destroy();

      /// Documentation Inherited
      public: virtual void AddResourcePath(const std::string &_path);

      protected: virtual void PrepareScene(ScenePtr _scene);

      protected: virtual unsigned int NextSceneId();

      /// Engine implementation of Load function.
      /// \param[in] _params Parameters to be passed to the render engine.
      protected: virtual bool LoadImpl(
          const std::map<std::string, std::string> &_params) = 0;

      protected: virtual bool InitImpl() = 0;

      protected: virtual ScenePtr CreateSceneImpl(unsigned int _id,
                  const std::string &_name) = 0;

      protected: virtual SceneStorePtr Scenes() const = 0;

      protected: bool loaded = false;

      protected: bool initialized = false;

      protected: unsigned int nextSceneId;

      /// \brief a list of paths that render engines use to locate their
      /// resources
      protected: std::vector<std::string> resourcePaths;
    };
    }
  }
}
#endif
