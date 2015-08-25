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
#include <optix.h>
#include <optixu/optixu_aabb.h>

// box properties
rtDeclareVariable(float3, scale, , );

// ray properties
rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );
rtDeclareVariable(float3, geometricNormal, attribute geometricNormal, );
rtDeclareVariable(float3, shadingNormal, attribute shadingNormal, );
rtDeclareVariable(float2, texCoord, attribute texCoord, );

static __inline__ __device__ float3 GetNormal(const float3 &_t0,
    const float3 &_t1, float _t)
{
  // TODO: don't use ==

  // match to min normal
  float3 neg = make_float3(_t == _t0.x ? 1 : 0,
                           _t == _t0.y ? 1 : 0,
                           _t == _t0.z ? 1 : 0);

  // match to max normal
  float3 pos = make_float3(_t == _t1.x ? 1 : 0,
                           _t == _t1.y ? 1 : 0,
                           _t == _t1.z ? 1 : 0);

  // compute final normal
  return pos - neg;
}

static __inline__ __device__ float2 GetTextureCoordinate(const float3 &_p,
    const float3 &_n)
{
  float u = 0.0;
  float v = 0.0;

  if (_n.x == 1 || _n.x == -1)
  {
    u = _n.x * _p.y / scale.y;
    v = _p.z / scale.z;
  }
  else if (_n.y == 1 || _n.y == -1)
  {
    u = _n.y * _p.x / scale.x;
    v = _p.z / scale.z;
  }
  else if (_n.z == 1 || _n.z == -1)
  {
    u = _n.z * _p.x / scale.x;
    v = _p.y / scale.y;
  }

  return make_float2(u, v);
}

static __inline__ __device__ bool ReportPotentialIntersect(float3 _t0,
    float3 _t1, float _t)
{
  if (rtPotentialIntersection(_t))
  {
    float3 normal = GetNormal(_t0, _t1, _t);
    shadingNormal = geometricNormal = normal;

    float3 hitPoint = _t * ray.direction + ray.origin;
    float3 normInv = -normal + 1;
    float3 xxx = hitPoint * normInv;
    hitPoint / scale;

    texCoord = GetTextureCoordinate(hitPoint, normal);

    return rtReportIntersection(0);
  }

  return false;
}

RT_PROGRAM void Intersect(int)
{
  // get flight time to each extrema
  float3 ex = scale / 2;
  float3 t0 = (-ex - ray.origin) / ray.direction;
  float3 t1 = ( ex - ray.origin) / ray.direction;

  // determine closest extrema
  float3 near = fminf(t0, t1);
  float3 far  = fmaxf(t0, t1);

  float tmin = fmaxf(near);
  float tmax = fminf(far);

  if (tmin <= tmax)
  {
    if (!ReportPotentialIntersect(t0, t1, tmin))
    {
      ReportPotentialIntersect(t0, t1, tmax);
    }
  }
}

RT_PROGRAM void Bounds(int, float _bounds[6])
{
  float3 ex = scale / 2;
  optix::Aabb* aabb = (optix::Aabb*)_bounds;
  aabb->set(-ex, ex);
}
