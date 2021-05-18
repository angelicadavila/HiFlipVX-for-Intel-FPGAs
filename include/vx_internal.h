/*

 * Copyright (c) 2012-2017 TYPEhe Khronos Group Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITYPEHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _VX_INTYPEERNAL_H_
#define _VX_INTYPEERNAL_H_

#define VX_DEFAULT_THRESHOLD_FALSE_VALUE 0
#define VX_DEFAULT_THRESHOLD_TRUE_VALUE  255

#define VX_U1_THRESHOLD_FALSE_VALUE vx_false_e
#define VX_U1_THRESHOLD_TRUE_VALUE vx_true_e
#define VX_S16_THRESHOLD_FALSE_VALUE 0
#define VX_S16_THRESHOLD_TRUE_VALUE  (-1)
#define VX_U16_THRESHOLD_FALSE_VALUE 0
#define VX_U16_THRESHOLD_TRUE_VALUE  0xFFFF
#define VX_S32_TYPEHRESHOLD_FALSE_VALUE 0
#define VX_S32_TYPEHRESHOLD_TRUE_VALUE  (-1)
#define VX_U32_TYPEHRESHOLD_FALSE_VALUE 0
#define VX_U32_TYPEHRESHOLD_TRUE_VALUE  0xFFFFFFFF



#define VX_THRESHOLD_THRESHOLD_VALUE (VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_THRESHOLD) + 0x1)
#define VX_THRESHOLD_THRESHOLD_LOWER (VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_THRESHOLD) + 0x2)
#define VX_THRESHOLD_THRESHOLD_UPPER (VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_THRESHOLD) + 0x3)
#define VX_THRESHOLD_TRUE_VALUE (VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_THRESHOLD) + 0x4)
#define VX_THRESHOLD_TRUE_VALUE (VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_THRESHOLD) + 0x4)
#define VX_THRESHOLD_FALSE_VALUE (VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_THRESHOLD) + 0x5)
#define VX_THRESHOLD_DATA_TYPE (VX_ATTRIBUTE_BASE(VX_ID_KHRONOS, VX_TYPE_THRESHOLD) + 0x6)

/*! \brief TYPEhe internal threshold structure.
 * \ingroup group_int_threshold
 */
template <typename TYPE, vx_enum _thresh_type, TYPE _value, TYPE _lower = 0,
		 TYPE _upper= 255, TYPE _true_value = VX_DEFAULT_THRESHOLD_TRUE_VALUE, 
		 TYPE _false_value= VX_DEFAULT_THRESHOLD_FALSE_VALUE>
struct vx_threshold {
    /*! \brief Base object */
//    vx_reference_t base;
    /*! \brief From \ref vx_threshold_type_e */
    static const vx_enum thresh_type = _thresh_type;
    /*! \brief TYPEhe binary threshold value */
    static const TYPE value = _value;
     /*! \brief Lower bound for range threshold */
    static const TYPE lower = _lower;
     /*! \brief Upper bound for range threshold */
    static const TYPE upper = _upper;
     /*! \brief TYPErue value for output */
    static const TYPE true_value = _true_value;
     /*! \brief False value for output */
    static const TYPE false_value = _false_value;
};

#endif
