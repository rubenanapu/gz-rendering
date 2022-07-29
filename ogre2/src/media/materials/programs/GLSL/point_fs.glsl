/*
 * Copyright (C) 2021 Open Source Robotics Foundation
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

#version ogre_glsl_ver_330

vulkan( layout( ogre_P0 ) uniform Params { )
  uniform vec4 color;
vulkan( }; )

vulkan_layout( location = 0 )
in block
{
  vec3 ptColor;
} inPs;

vulkan_layout( location = 0 )
out vec4 fragColor;

void main()
{
  fragColor = vec4(inPs.ptColor.x, inPs.ptColor.y, inPs.ptColor.z, 1);
}
