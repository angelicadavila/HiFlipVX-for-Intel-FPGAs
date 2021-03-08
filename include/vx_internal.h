/*

 * Copyright (c) 2012-2017 The Khronos Group Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _VX_INTERNAL_H_
#define _VX_INTERNAL_H_

#define VX_DEFAULT_THRESHOLD_FALSE_VALUE 0
#define VX_DEFAULT_THRESHOLD_TRUE_VALUE  255

#define VX_U1_THRESHOLD_FALSE_VALUE vx_false_e
#define VX_U1_THRESHOLD_TRUE_VALUE vx_true_e
#define VX_S16_THRESHOLD_FALSE_VALUE 0
#define VX_S16_THRESHOLD_TRUE_VALUE  (-1)
#define VX_U16_THRESHOLD_FALSE_VALUE 0
#define VX_U16_THRESHOLD_TRUE_VALUE  0xFFFF
#define VX_S32_THRESHOLD_FALSE_VALUE 0
#define VX_S32_THRESHOLD_TRUE_VALUE  (-1)
#define VX_U32_THRESHOLD_FALSE_VALUE 0
#define VX_U32_THRESHOLD_TRUE_VALUE  0xFFFFFFFF

/*! \brief The internal threshold structure.
 * \ingroup group_int_threshold
 */
typedef struct _vx_threshold {
    /*! \brief Base object */
    vx_reference_t base;
    /*! \brief From \ref vx_threshold_type_e */
    vx_enum thresh_type;
    /*! \brief From \ref vx_type_e */
    vx_enum data_type;
    /*! \brief The binary threshold value */
    vx_pixel_value_t value;
    /*! \brief Lower bound for range threshold */
    vx_pixel_value_t lower;
    /*! \brief Upper bound for range threshold */
    vx_pixel_value_t upper;
    /*! \brief True value for output */
    vx_pixel_value_t true_value;
    /*! \brief False value for output */
    vx_pixel_value_t false_value;
    /*! \brief The input image format */
    vx_df_image input_format;
    /*! \brief The output image format  */
    vx_df_image output_format;
} vx_threshold_t;

#endif
