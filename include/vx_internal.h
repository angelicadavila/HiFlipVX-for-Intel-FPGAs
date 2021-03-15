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
template <typename TYPE>
class _vx_threshold {
   public:
    /*! \brief Base object */
//    vx_reference_t base;
    /*! \brief From \ref vx_threshold_type_e */
    vx_enum thresh_type;
    /*! \brief TYPEhe binary threshold value */
    TYPE value;
    /*! \brief Lower bound for range threshold */
    TYPE lower;
    /*! \brief Upper bound for range threshold */
    TYPE upper;
    /*! \brief TYPErue value for output */
    TYPE true_value;
    /*! \brief False value for output */
    TYPE false_value;
//   private:
	
    _vx_threshold<TYPE> vxCreateThreshold(vx_enum thresh_type, vx_enum data_type){
	    _vx_threshold<TYPE> threshold;	
		 threshold.thresh_type = thresh_type;
		return threshold;
	}

	
	void vxSetThresholdAttribute(vx_enum attribute, const TYPE const_value, vx_size size){
	
        switch (attribute)
        {
            case VX_THRESHOLD_THRESHOLD_VALUE:
                if (thresh_type == VX_THRESHOLD_TYPE_BINARY)
                {
                    value = const_value;//*(vx_int32 *)ptr;
                }
                break;
            case VX_THRESHOLD_THRESHOLD_LOWER:
                if ( thresh_type == VX_THRESHOLD_TYPE_RANGE)
                {
                    lower = value;
                }
                break;
            case VX_THRESHOLD_THRESHOLD_UPPER:
                if (thresh_type == VX_THRESHOLD_TYPE_RANGE)
                {
                    upper= value;
                }
                break;
            case VX_THRESHOLD_TRUE_VALUE:
                    true_value = value;
                break;
            case VX_THRESHOLD_FALSE_VALUE:
                    false_value = value;
                break;
            default:
            break;
        }
	}
};

#endif
