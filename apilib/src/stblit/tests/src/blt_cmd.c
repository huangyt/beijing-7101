/*****************************************************************************

File name   : blt_cmd.c

Description : STBLIT macros

COPYRIGHT (C) STMicroelectronics 2004.

Date                     Modification                  Name
----                     ------------                  ----
20 September 2003        Created                       HE
30 May 2006              Updated for 7109 device	   WinCE Team-ST Noida
*******************************************************************************/

/* Private preliminary definitions (internal use only) ---------------------- */

/* Includes ----------------------------------------------------------------- */
#include <string.h>
#ifdef ST_OS21
    #include "os21debug.h"
    #include <sys/stat.h>
    #include <fcntl.h>
#endif
#ifdef OS_20
    #include <debug.h>
#endif
#include <stdio.h>
#include "stddefs.h"
#include "startup.h"
#include "blt_cmd.h"
#include "blt_debug.h"
#include "blt_engine.h"
#include "blt_drawing.h"
#include "blt_wrapper.h"
#ifdef ENABLE_RECCURENCE_MODE_TEST
#include "stlayer.h"
#endif
#ifdef ST_OSLINUX
#include "stlayer.h"
#endif
#ifndef STBLIT_EMULATOR
#include "clintmr.h"
#endif
#ifdef ST_OSLINUX
#include "iocstapigat.h"
#else
#include "sttbx.h"
#endif
#ifndef STBLIT_EMULATOR
#include "clintmr.h"
#endif
#if defined (DVD_SECURED_CHIP)
#include "stmes.h"
#endif



/* Private Types ------------------------------------------------------------ */


/* Private Constants -------------------------------------------------------- */
#define FILL_METHOD
#define TYPE_CLUT2 2
#define TYPE_CLUT4 4
#define TYPE_CLUT8 8
#define TYPE_RGB565 565
#define TYPE_RGB888 888
#define TYPE_YCbCr422R 422

#if defined(ST_OSLINUX)
#define TEST_DELAY 10000 /* for test prupose to avoid timeout problems */
#endif

#define SM_TASK_STACK_SIZE                  (5*1024)
#define SM_TASK_PRIORITY                    (MIN_USER_PRIORITY)

#if defined (ST_OSLINUX)
#if defined (DVD_SECURED_CHIP)
#define SECURED_DST_BITMAP TRUE
#if defined (SECURED_BITMAP)
#define SECURED_SRC_BITMAP TRUE
#else
#define SECURED_SRC_BITMAP FALSE
#endif
#else
#define SECURED_SRC_BITMAP FALSE
#define SECURED_DST_BITMAP FALSE
#endif

#else /* ! (ST_OSLINUX) */
#if defined (DVD_SECURED_CHIP)
#if defined (SECURED_NODE)
#define AVMEM_PARTITION_NUM   0      /*secured partition to allocate node, CLUT and filters coeifficients*/
#else
#define AVMEM_PARTITION_NUM   2      /*un-secured partition to allocate node, CLUT and filters coeifficients*/
#endif
#if defined (SECURED_BITMAP)
#define SRC_BITMAP_AVMEM_PARTITION_NUM 0  /*secured partition for source bitmap allocation*/
#else
#define SRC_BITMAP_AVMEM_PARTITION_NUM 2  /*un-secured partition for source bitmap allocation*/
#endif
#define DST_BITMAP_AVMEM_PARTITION_NUM 0  /* the destination Bitmap is alawys in secured partition*/

#else
#define AVMEM_PARTITION_NUM   0
#define SRC_BITMAP_AVMEM_PARTITION_NUM 0
#define DST_BITMAP_AVMEM_PARTITION_NUM 0
#endif
#endif

#ifdef ST_OSLINUX
U8 SrcPartitionInfo = (U8)SECURED_SRC_BITMAP;
U8 DstPartitionInfo = (U8)SECURED_DST_BITMAP;
#else /* !ST_OSLINUX */
U8 SrcPartitionInfo = (U8)SRC_BITMAP_AVMEM_PARTITION_NUM;
U8 DstPartitionInfo = (U8)DST_BITMAP_AVMEM_PARTITION_NUM;
#endif /* ST_OSLINUX */


/* Test features related */
#ifdef TEST_FEATURES_FULL_MODE
#define TEST_FEATURES_NB_STEPS 50
#define TEST_FEATURES_NB_TESTS 20000

#ifdef ST_5105
#define TEST_FEATURES_COPY_REF_FILE_NAME                                "../../../ref/crc/5105Ref_FullMode_Copy.ref"
#define TEST_FEATURES_FILL_REF_FILE_NAME                                "../../../ref/crc/5105Ref_FullMode_Fill.ref"
#define TEST_FEATURES_OVERLAP_COPY_REF_FILE_NAME                        "../../../ref/crc/5105Ref_FullMode_Overlap_Copy.ref"
#define TEST_FEATURES_OVERLAP_BLIT_REF_FILE_NAME                        "../../../ref/crc/5105Ref_FullMode_Overlap_Blit.ref"
#define TEST_FEATURES_BITMAP_BLEND_REF_FILE_NAME                        "../../../ref/crc/5105Ref_FullMode_Bitmap_Blend.ref"
#define TEST_FEATURES_FILL_BLEND_REF_FILE_NAME                          "../../../ref/crc/5105Ref_FullMode_Fill_Blend.ref"
#define TEST_FEATURES_COPY_2_SRC_REF_FILE_NAME                          "../../../ref/crc/5105Ref_FullMode_Copy_2_src.ref"
#define TEST_FEATURES_CLIP_REF_FILE_NAME                                "../../../ref/crc/5105Ref_FullMode_Clip.ref"
#define TEST_FEATURES_RESIZE_REF_FILE_NAME                              "../../../ref/crc/5105Ref_FullMode_Resize.ref"
#define TEST_FEATURES_ANTIALIASING_REF_FILE_NAME                        "../../../ref/crc/5105Ref_FullMode_AntiAliasing.ref"
#define TEST_FEATURES_ANTIFLUTTER_REF_FILE_NAME                         "../../../ref/crc/5105Ref_FullMode_AntiFlutter.ref"
#define TEST_FEATURES_COLOR_CONVERSION_REF_FILE_NAME                    "../../../ref/crc/5105Ref_FullMode_Color_Conversion.ref"
#define TEST_FEATURES_JOB_REF_FILE_NAME                                 "../../../ref/crc/5105Ref_FullMode_Job.ref"
#define TEST_FEATURES_SETPIXEL_REF_FILE_NAME                            "../../../ref/crc/5105Ref_FullMode_SetPixel.ref"
#define TEST_FEATURES_DRAWLINE_REF_FILE_NAME                            "../../../ref/crc/5105Ref_FullMode_DrawLine.ref"
#endif /* ST_5105 */

#ifdef ST_7710
#define TEST_FEATURES_COPY_REF_FILE_NAME                                "../../../ref/crc/7710Ref_FullMode_Copy.ref"
#define TEST_FEATURES_FILL_REF_FILE_NAME                                "../../../ref/crc/7710Ref_FullMode_Fill.ref"
#define TEST_FEATURES_OVERLAP_COPY_REF_FILE_NAME                        "../../../ref/crc/7710Ref_FullMode_Overlap_Copy.ref"
#define TEST_FEATURES_OVERLAP_BLIT_REF_FILE_NAME                        "../../../ref/crc/7710Ref_FullMode_Overlap_Blit.ref"
#define TEST_FEATURES_BITMAP_BLEND_REF_FILE_NAME                        "../../../ref/crc/7710Ref_FullMode_Bitmap_Blend.ref"
#define TEST_FEATURES_FILL_BLEND_REF_FILE_NAME                          "../../../ref/crc/7710Ref_FullMode_Fill_Blend.ref"
#define TEST_FEATURES_COPY_2_SRC_REF_FILE_NAME                          "../../../ref/crc/7710Ref_FullMode_Copy_2_src.ref"
#define TEST_FEATURES_CLIP_REF_FILE_NAME                                "../../../ref/crc/7710Ref_FullMode_Clip.ref"
#define TEST_FEATURES_RESIZE_REF_FILE_NAME                              "../../../ref/crc/7710Ref_FullMode_Resize.ref"
#define TEST_FEATURES_COLOR_CONVERSION_REF_FILE_NAME                    "../../../ref/crc/7710Ref_FullMode_Color_Conversion.ref"
#define TEST_FEATURES_JOB_REF_FILE_NAME                                 "../../../ref/crc/7710Ref_FullMode_Job.ref"
#endif /* ST_7710 */


#if defined(ST_7109) || defined(ST_7200)
#ifdef ST_OSLINUX
#define TEST_FEATURES_COPY_REF_FILE_NAME                                "./blt/ref/7109Ref_FullMode_Copy.ref"
#define TEST_FEATURES_FILL_REF_FILE_NAME                                "./blt/ref/7109Ref_FullMode_Fill.ref"
#define TEST_FEATURES_OVERLAP_COPY_REF_FILE_NAME                        "./blt/ref/7109Ref_FullMode_Overlap_Copy.ref"
#define TEST_FEATURES_OVERLAP_BLIT_REF_FILE_NAME                        "./blt/ref/7109Ref_FullMode_Overlap_Blit.ref"
#define TEST_FEATURES_BITMAP_BLEND_REF_FILE_NAME                        "./blt/ref/7109Ref_FullMode_Bitmap_Blend.ref"
#define TEST_FEATURES_FILL_BLEND_REF_FILE_NAME                          "./blt/ref/7109Ref_FullMode_Fill_Blend.ref"
#define TEST_FEATURES_COPY_2_SRC_REF_FILE_NAME                          "./blt/ref/7109Ref_FullMode_Copy_2_src.ref"
#define TEST_FEATURES_CLIP_REF_FILE_NAME                                "./blt/ref/7109Ref_FullMode_Clip.ref"
#define TEST_FEATURES_RESIZE_REF_FILE_NAME                              "./blt/ref/7109Ref_FullMode_Resize.ref"
#define TEST_FEATURES_COLOR_CONVERSION_REF_FILE_NAME                    "./blt/ref/7109Ref_FullMode_Color_Conversion.ref"
#define TEST_FEATURES_JOB_REF_FILE_NAME                                 "./blt/ref/7109Ref_FullMode_Job.ref"
#define TEST_FEATURES_ANTIALIASING_REF_FILE_NAME                        "./blt/ref/7109Ref_FullMode_AntiAliasing.ref"
#define TEST_FEATURES_ANTIFLUTTER_REF_FILE_NAME                         "./blt/ref/7109Ref_FullMode_AntiFlutter.ref"
#define TEST_FEATURES_ALU_REF_FILE_NAME                                 "./blt/ref/7109Ref_FullMode_ALU.ref"
#define TEST_FEATURES_MASK_WORD_REF_FILE_NAME                           "./blt/ref/7109Ref_FullMode_Mask_word.ref"
#define TEST_FEATURES_COLOR_KEY_REF_FILE_NAME                           "./blt/ref/7109Ref_FullMode_Color_Key.ref"
#define TEST_FEATURES_SETPIXEL_REF_FILE_NAME                            "./blt/ref/7109Ref_FullMode_SetPixel.ref"
#define TEST_FEATURES_DRAWLINE_REF_FILE_NAME                            "./blt/ref/7109Ref_FullMode_DrawLine.ref"
#define TEST_FEATURES_VC1_RANGE_REF_FILE_NAME                           "./blt/ref/7109Ref_FullMode_VC1Range.ref"


#else /* !ST_OSLINUX */
#define TEST_FEATURES_COPY_REF_FILE_NAME                                "../../../ref/crc/7109Ref_FullMode_Copy.ref"
#define TEST_FEATURES_FILL_REF_FILE_NAME                                "../../../ref/crc/7109Ref_FullMode_Fill.ref"
#define TEST_FEATURES_OVERLAP_COPY_REF_FILE_NAME                        "../../../ref/crc/7109Ref_FullMode_Overlap_Copy.ref"
#define TEST_FEATURES_OVERLAP_BLIT_REF_FILE_NAME                        "../../../ref/crc/7109Ref_FullMode_Overlap_Blit.ref"
#define TEST_FEATURES_BITMAP_BLEND_REF_FILE_NAME                        "../../../ref/crc/7109Ref_FullMode_Bitmap_Blend.ref"
#define TEST_FEATURES_FILL_BLEND_REF_FILE_NAME                          "../../../ref/crc/7109Ref_FullMode_Fill_Blend.ref"
#define TEST_FEATURES_COPY_2_SRC_REF_FILE_NAME                          "../../../ref/crc/7109Ref_FullMode_Copy_2_src.ref"
#define TEST_FEATURES_CLIP_REF_FILE_NAME                                "../../../ref/crc/7109Ref_FullMode_Clip.ref"
#define TEST_FEATURES_RESIZE_REF_FILE_NAME                              "../../../ref/crc/7109Ref_FullMode_Resize.ref"
#define TEST_FEATURES_COLOR_CONVERSION_REF_FILE_NAME                    "../../../ref/crc/7109Ref_FullMode_Color_Conversion.ref"
#define TEST_FEATURES_JOB_REF_FILE_NAME                                 "../../../ref/crc/7109Ref_FullMode_Job.ref"
#define TEST_FEATURES_ANTIALIASING_REF_FILE_NAME                        "../../../ref/crc/7109Ref_FullMode_AntiAliasing.ref"
#define TEST_FEATURES_ANTIFLUTTER_REF_FILE_NAME                         "../../../ref/crc/7109Ref_FullMode_AntiFlutter.ref"
#define TEST_FEATURES_ALU_REF_FILE_NAME                                 "../../../ref/crc/7109Ref_FullMode_ALU.ref"
#define TEST_FEATURES_MASK_WORD_REF_FILE_NAME                           "../../../ref/crc/7109Ref_FullMode_Mask_word.ref"
#define TEST_FEATURES_COLOR_KEY_REF_FILE_NAME                           "../../../ref/crc/7109Ref_FullMode_Color_Key.ref"
#define TEST_FEATURES_SETPIXEL_REF_FILE_NAME                            "../../../ref/crc/7109Ref_FullMode_SetPixel.ref"
#define TEST_FEATURES_DRAWLINE_REF_FILE_NAME                            "../../../ref/crc/7109Ref_FullMode_DrawLine.ref"
#define TEST_FEATURES_VC1_RANGE_REF_FILE_NAME                           "../../../ref/crc/7109Ref_FullMode_VC1Range.ref"
#endif /* ST_OSLINUX */
#endif /* ST_7109 */

#ifdef ST_7100
#ifdef ST_OSLINUX
#define TEST_FEATURES_COPY_REF_FILE_NAME                                "./blt/ref/7100Ref_FullMode_Copy.ref"
#define TEST_FEATURES_FILL_REF_FILE_NAME                                "./blt/ref/7100Ref_FullMode_Fill.ref"
#define TEST_FEATURES_OVERLAP_COPY_REF_FILE_NAME                        "./blt/ref/7100Ref_FullMode_Overlap_Copy.ref"
#define TEST_FEATURES_OVERLAP_BLIT_REF_FILE_NAME                        "./blt/ref/7100Ref_FullMode_Overlap_Blit.ref"
#define TEST_FEATURES_BITMAP_BLEND_REF_FILE_NAME                        "./blt/ref/7100Ref_FullMode_Bitmap_Blend.ref"
#define TEST_FEATURES_FILL_BLEND_REF_FILE_NAME                          "./blt/ref/7100Ref_FullMode_Fill_Blend.ref"
#define TEST_FEATURES_COPY_2_SRC_REF_FILE_NAME                          "./blt/ref/7100Ref_FullMode_Copy_2_src.ref"
#define TEST_FEATURES_CLIP_REF_FILE_NAME                                "./blt/ref/7100Ref_FullMode_Clip.ref"
#define TEST_FEATURES_RESIZE_REF_FILE_NAME                              "./blt/ref/7100Ref_FullMode_Resize.ref"
#define TEST_FEATURES_COLOR_CONVERSION_REF_FILE_NAME                    "./blt/ref/7100Ref_FullMode_Color_Conversion.ref"
#define TEST_FEATURES_JOB_REF_FILE_NAME                                 "./blt/ref/7100Ref_FullMode_Job.ref"
#define TEST_FEATURES_ALU_REF_FILE_NAME                                 "./blt/ref/7100Ref_FullMode_ALU.ref"
#define TEST_FEATURES_MASK_WORD_REF_FILE_NAME                           "./blt/ref/7100Ref_FullMode_Mask_word.ref"
#define TEST_FEATURES_COLOR_KEY_REF_FILE_NAME                           "./blt/ref/7100Ref_FullMode_Color_Key.ref"
#define TEST_FEATURES_SETPIXEL_REF_FILE_NAME                            "./blt/ref/7100Ref_FullMode_SetPixel.ref"
#define TEST_FEATURES_DRAWLINE_REF_FILE_NAME                            "./blt/ref/7100Ref_FullMode_DrawLine.ref"
#else /* !ST_OSLINUX */
#define TEST_FEATURES_COPY_REF_FILE_NAME                                "../../../ref/crc/7100Ref_FullMode_Copy.ref"
#define TEST_FEATURES_FILL_REF_FILE_NAME                                "../../../ref/crc/7100Ref_FullMode_Fill.ref"
#define TEST_FEATURES_OVERLAP_COPY_REF_FILE_NAME                        "../../../ref/crc/7100Ref_FullMode_Overlap_Copy.ref"
#define TEST_FEATURES_OVERLAP_BLIT_REF_FILE_NAME                        "../../../ref/crc/7100Ref_FullMode_Overlap_Blit.ref"
#define TEST_FEATURES_BITMAP_BLEND_REF_FILE_NAME                        "../../../ref/crc/7100Ref_FullMode_Bitmap_Blend.ref"
#define TEST_FEATURES_FILL_BLEND_REF_FILE_NAME                          "../../../ref/crc/7100Ref_FullMode_Fill_Blend.ref"
#define TEST_FEATURES_COPY_2_SRC_REF_FILE_NAME                          "../../../ref/crc/7100Ref_FullMode_Copy_2_src.ref"
#define TEST_FEATURES_CLIP_REF_FILE_NAME                                "../../../ref/crc/7100Ref_FullMode_Clip.ref"
#define TEST_FEATURES_RESIZE_REF_FILE_NAME                              "../../../ref/crc/7100Ref_FullMode_Resize.ref"
#define TEST_FEATURES_COLOR_CONVERSION_REF_FILE_NAME                    "../../../ref/crc/7100Ref_FullMode_Color_Conversion.ref"
#define TEST_FEATURES_JOB_REF_FILE_NAME                                 "../../../ref/crc/7100Ref_FullMode_Job.ref"
#define TEST_FEATURES_ALU_REF_FILE_NAME                                 "../../../ref/crc/7100Ref_FullMode_ALU.ref"
#define TEST_FEATURES_MASK_WORD_REF_FILE_NAME                           "../../../ref/crc/7100Ref_FullMode_Mask_word.ref"
#define TEST_FEATURES_COLOR_KEY_REF_FILE_NAME                           "../../../ref/crc/7100Ref_FullMode_Color_Key.ref"
#define TEST_FEATURES_SETPIXEL_REF_FILE_NAME                            "../../../ref/crc/7100Ref_FullMode_SetPixel.ref"
#define TEST_FEATURES_DRAWLINE_REF_FILE_NAME                            "../../../ref/crc/7100Ref_FullMode_DrawLine.ref"
#endif /* ST_OSLINUX */
#endif /* ST_7100 */

#else /* !TEST_FEATURES_FULL_MODE */
#define TEST_FEATURES_NB_STEPS 10
#define TEST_FEATURES_NB_TESTS 2000

#ifdef ST_5105
#define TEST_FEATURES_COPY_REF_FILE_NAME                                "../../../ref/crc/5105Ref_ReducedMode_Copy.ref"
#define TEST_FEATURES_FILL_REF_FILE_NAME                                "../../../ref/crc/5105Ref_ReducedMode_Fill.ref"
#define TEST_FEATURES_OVERLAP_COPY_REF_FILE_NAME                        "../../../ref/crc/5105Ref_ReducedMode_Overlap_Copy.ref"
#define TEST_FEATURES_OVERLAP_BLIT_REF_FILE_NAME                        "../../../ref/crc/5105Ref_ReducedMode_Overlap_Blit.ref"
#define TEST_FEATURES_BITMAP_BLEND_REF_FILE_NAME                        "../../../ref/crc/5105Ref_ReducedMode_Bitmap_Blend.ref"
#define TEST_FEATURES_FILL_BLEND_REF_FILE_NAME                          "../../../ref/crc/5105Ref_ReducedMode_Fill_Blend.ref"
#define TEST_FEATURES_COPY_2_SRC_REF_FILE_NAME                          "../../../ref/crc/5105Ref_ReducedMode_Copy_2_src.ref"
#define TEST_FEATURES_CLIP_REF_FILE_NAME                                "../../../ref/crc/5105Ref_ReducedMode_Clip.ref"
#define TEST_FEATURES_RESIZE_REF_FILE_NAME                              "../../../ref/crc/5105Ref_ReducedMode_Resize.ref"
#define TEST_FEATURES_COLOR_CONVERSION_REF_FILE_NAME                    "../../../ref/crc/5105Ref_ReducedMode_Color_Conversion.ref"
#define TEST_FEATURES_JOB_REF_FILE_NAME                                 "../../../ref/crc/5105Ref_ReducedMode_Job.ref"
#define TEST_FEATURES_ANTIALIASING_REF_FILE_NAME                        "../../../ref/crc/5105Ref_ReducedMode_AntiAliasing.ref"
#define TEST_FEATURES_ANTIFLUTTER_REF_FILE_NAME                         "../../../ref/crc/5105Ref_ReducedMode_AntiFlutter.ref"
#define TEST_FEATURES_SETPIXEL_REF_FILE_NAME                            "../../../ref/crc/5105Ref_ReducedMode_SetPixel.ref"
#define TEST_FEATURES_DRAWLINE_REF_FILE_NAME                            "../../../ref/crc/5105Ref_ReducedMode_DrawLine.ref"
#endif /* ST_5105 */

#ifdef ST_7710
#define TEST_FEATURES_COPY_REF_FILE_NAME                                "../../../ref/crc/7710Ref_ReducedMode_Copy.ref"
#define TEST_FEATURES_FILL_REF_FILE_NAME                                "../../../ref/crc/7710Ref_ReducedMode_Fill.ref"
#define TEST_FEATURES_OVERLAP_COPY_REF_FILE_NAME                        "../../../ref/crc/7710Ref_ReducedMode_Overlap_Copy.ref"
#define TEST_FEATURES_OVERLAP_BLIT_REF_FILE_NAME                        "../../../ref/crc/7710Ref_ReducedMode_Overlap_Blit.ref"
#define TEST_FEATURES_BITMAP_BLEND_REF_FILE_NAME                        "../../../ref/crc/7710Ref_ReducedMode_Bitmap_Blend.ref"
#define TEST_FEATURES_FILL_BLEND_REF_FILE_NAME                          "../../../ref/crc/7710Ref_ReducedMode_Fill_Blend.ref"
#define TEST_FEATURES_COPY_2_SRC_REF_FILE_NAME                          "../../../ref/crc/7710Ref_ReducedMode_Copy_2_src.ref"
#define TEST_FEATURES_CLIP_REF_FILE_NAME                                "../../../ref/crc/7710Ref_ReducedMode_Clip.ref"
#define TEST_FEATURES_RESIZE_REF_FILE_NAME                              "../../../ref/crc/7710Ref_ReducedMode_Resize.ref"
#define TEST_FEATURES_COLOR_CONVERSION_REF_FILE_NAME                    "../../../ref/crc/7710Ref_ReducedMode_Color_Conversion.ref"
#define TEST_FEATURES_JOB_REF_FILE_NAME                                 "../../../ref/crc/7710Ref_ReducedMode_Job.ref"
#endif /* ST_7710 */

#if defined(ST_7109) || defined(ST_7200)
#ifdef ST_OSLINUX
#define TEST_FEATURES_COPY_REF_FILE_NAME                                "./blt/ref/7109Ref_ReducedMode_Copy.ref"
#define TEST_FEATURES_FILL_REF_FILE_NAME                                "./blt/ref/7109Ref_ReducedMode_Fill.ref"
#define TEST_FEATURES_OVERLAP_COPY_REF_FILE_NAME                        "./blt/ref/7109Ref_ReducedMode_Overlap_Copy.ref"
#define TEST_FEATURES_OVERLAP_BLIT_REF_FILE_NAME                        "./blt/ref/7109Ref_ReducedMode_Overlap_Blit.ref"
#define TEST_FEATURES_BITMAP_BLEND_REF_FILE_NAME                        "./blt/ref/7109Ref_ReducedMode_Bitmap_Blend.ref"
#define TEST_FEATURES_FILL_BLEND_REF_FILE_NAME                          "./blt/ref/7109Ref_ReducedMode_Fill_Blend.ref"
#define TEST_FEATURES_COPY_2_SRC_REF_FILE_NAME                          "./blt/ref/7109Ref_ReducedMode_Copy_2_src.ref"
#define TEST_FEATURES_CLIP_REF_FILE_NAME                                "./blt/ref/7109Ref_ReducedMode_Clip.ref"
#define TEST_FEATURES_RESIZE_REF_FILE_NAME                              "./blt/ref/7109Ref_ReducedMode_Resize.ref"
#define TEST_FEATURES_COLOR_CONVERSION_REF_FILE_NAME                    "./blt/ref/7109Ref_ReducedMode_Color_Conversion.ref"
#define TEST_FEATURES_JOB_REF_FILE_NAME                                 "./blt/ref/7109Ref_ReducedMode_Job.ref"
#define TEST_FEATURES_ANTIALIASING_REF_FILE_NAME                        "./blt/ref/7109Ref_ReducedMode_AntiAliasing.ref"
#define TEST_FEATURES_ANTIFLUTTER_REF_FILE_NAME                         "./blt/ref/7109Ref_ReducedMode_AntiFlutter.ref"
#define TEST_FEATURES_ALU_REF_FILE_NAME                                 "./blt/ref/7109Ref_ReducedMode_ALU.ref"
#define TEST_FEATURES_MASK_WORD_REF_FILE_NAME                           "./blt/ref/7109Ref_ReducedMode_Mask_word.ref"
#define TEST_FEATURES_COLOR_KEY_REF_FILE_NAME                           "./blt/ref/7109Ref_ReducedMode_Color_Key.ref"
#define TEST_FEATURES_SETPIXEL_REF_FILE_NAME                            "./blt/ref/7109Ref_ReducedMode_SetPixel.ref"
#define TEST_FEATURES_DRAWLINE_REF_FILE_NAME                            "./blt/ref/7109Ref_ReducedMode_DrawLine.ref"
#define TEST_FEATURES_VC1_RANGE_REF_FILE_NAME                           "./blt/ref/7109Ref_ReducedMode_VC1Range.ref"

#else /* !ST_OSLINUX */
#define TEST_FEATURES_COPY_REF_FILE_NAME                                "../../../ref/crc/7109Ref_ReducedMode_Copy.ref"
#define TEST_FEATURES_FILL_REF_FILE_NAME                                "../../../ref/crc/7109Ref_ReducedMode_Fill.ref"
#define TEST_FEATURES_OVERLAP_COPY_REF_FILE_NAME                        "../../../ref/crc/7109Ref_ReducedMode_Overlap_Copy.ref"
#define TEST_FEATURES_OVERLAP_BLIT_REF_FILE_NAME                        "../../../ref/crc/7109Ref_ReducedMode_Overlap_Blit.ref"
#define TEST_FEATURES_BITMAP_BLEND_REF_FILE_NAME                        "../../../ref/crc/7109Ref_ReducedMode_Bitmap_Blend.ref"
#define TEST_FEATURES_FILL_BLEND_REF_FILE_NAME                          "../../../ref/crc/7109Ref_ReducedMode_Fill_Blend.ref"
#define TEST_FEATURES_COPY_2_SRC_REF_FILE_NAME                          "../../../ref/crc/7109Ref_ReducedMode_Copy_2_src.ref"
#define TEST_FEATURES_CLIP_REF_FILE_NAME                                "../../../ref/crc/7109Ref_ReducedMode_Clip.ref"
#define TEST_FEATURES_RESIZE_REF_FILE_NAME                              "../../../ref/crc/7109Ref_ReducedMode_Resize.ref"
#define TEST_FEATURES_COLOR_CONVERSION_REF_FILE_NAME                    "../../../ref/crc/7109Ref_ReducedMode_Color_Conversion.ref"
#define TEST_FEATURES_JOB_REF_FILE_NAME                                 "../../../ref/crc/7109Ref_ReducedMode_Job.ref"
#define TEST_FEATURES_ANTIALIASING_REF_FILE_NAME                        "../../../ref/crc/7109Ref_ReducedMode_AntiAliasing.ref"
#define TEST_FEATURES_ANTIFLUTTER_REF_FILE_NAME                         "../../../ref/crc/7109Ref_ReducedMode_AntiFlutter.ref"
#define TEST_FEATURES_ALU_REF_FILE_NAME                                 "../../../ref/crc/7109Ref_ReducedMode_ALU.ref"
#define TEST_FEATURES_MASK_WORD_REF_FILE_NAME                           "../../../ref/crc/7109Ref_ReducedMode_Mask_word.ref"
#define TEST_FEATURES_COLOR_KEY_REF_FILE_NAME                           "../../../ref/crc/7109Ref_ReducedMode_Color_Key.ref"
#define TEST_FEATURES_SETPIXEL_REF_FILE_NAME                            "../../../ref/crc/7109Ref_ReducedMode_SetPixel.ref"
#define TEST_FEATURES_DRAWLINE_REF_FILE_NAME                            "../../../ref/crc/7109Ref_ReducedMode_DrawLine.ref"
#define TEST_FEATURES_VC1_RANGE_REF_FILE_NAME                           "../../../ref/crc/7109Ref_ReducedMode_VC1Range.ref"
#endif /* ST_OSLINUX */
#endif /* ST_7109 */

#ifdef ST_7100
#ifdef ST_OSLINUX
#define TEST_FEATURES_COPY_REF_FILE_NAME                                "./blt/ref/7100Ref_ReducedMode_Copy.ref"
#define TEST_FEATURES_FILL_REF_FILE_NAME                                "./blt/ref/7100Ref_ReducedMode_Fill.ref"
#define TEST_FEATURES_OVERLAP_COPY_REF_FILE_NAME                        "./blt/ref/7100Ref_ReducedMode_Overlap_Copy.ref"
#define TEST_FEATURES_OVERLAP_BLIT_REF_FILE_NAME                        "./blt/ref/7100Ref_ReducedMode_Overlap_Blit.ref"
#define TEST_FEATURES_BITMAP_BLEND_REF_FILE_NAME                        "./blt/ref/7100Ref_ReducedMode_Bitmap_Blend.ref"
#define TEST_FEATURES_FILL_BLEND_REF_FILE_NAME                          "./blt/ref/7100Ref_ReducedMode_Fill_Blend.ref"
#define TEST_FEATURES_COPY_2_SRC_REF_FILE_NAME                          "./blt/ref/7100Ref_ReducedMode_Copy_2_src.ref"
#define TEST_FEATURES_CLIP_REF_FILE_NAME                                "./blt/ref/7100Ref_ReducedMode_Clip.ref"
#define TEST_FEATURES_RESIZE_REF_FILE_NAME                              "./blt/ref/7100Ref_ReducedMode_Resize.ref"
#define TEST_FEATURES_COLOR_CONVERSION_REF_FILE_NAME                    "./blt/ref/7100Ref_ReducedMode_Color_Conversion.ref"
#define TEST_FEATURES_JOB_REF_FILE_NAME                                 "./blt/ref/7100Ref_ReducedMode_Job.ref"
#define TEST_FEATURES_ALU_REF_FILE_NAME                                 "./blt/ref/7100Ref_ReducedMode_ALU.ref"
#define TEST_FEATURES_MASK_WORD_REF_FILE_NAME                           "./blt/ref/7100Ref_ReducedMode_Mask_word.ref"
#define TEST_FEATURES_COLOR_KEY_REF_FILE_NAME                           "./blt/ref/7100Ref_ReducedMode_Color_Key.ref"
#define TEST_FEATURES_SETPIXEL_REF_FILE_NAME                            "./blt/ref/7100Ref_ReducedMode_SetPixel.ref"
#define TEST_FEATURES_DRAWLINE_REF_FILE_NAME                            "./blt/ref/7100Ref_ReducedMode_DrawLine.ref"

#else /* !ST_OSLINUX */
#define TEST_FEATURES_COPY_REF_FILE_NAME                                "../../../ref/crc/7100Ref_ReducedMode_Copy.ref"
#define TEST_FEATURES_FILL_REF_FILE_NAME                                "../../../ref/crc/7100Ref_ReducedMode_Fill.ref"
#define TEST_FEATURES_OVERLAP_COPY_REF_FILE_NAME                        "../../../ref/crc/7100Ref_ReducedMode_Overlap_Copy.ref"
#define TEST_FEATURES_OVERLAP_BLIT_REF_FILE_NAME                        "../../../ref/crc/7100Ref_ReducedMode_Overlap_Blit.ref"
#define TEST_FEATURES_BITMAP_BLEND_REF_FILE_NAME                        "../../../ref/crc/7100Ref_ReducedMode_Bitmap_Blend.ref"
#define TEST_FEATURES_FILL_BLEND_REF_FILE_NAME                          "../../../ref/crc/7100Ref_ReducedMode_Fill_Blend.ref"
#define TEST_FEATURES_COPY_2_SRC_REF_FILE_NAME                          "../../../ref/crc/7100Ref_ReducedMode_Copy_2_src.ref"
#define TEST_FEATURES_CLIP_REF_FILE_NAME                                "../../../ref/crc/7100Ref_ReducedMode_Clip.ref"
#define TEST_FEATURES_RESIZE_REF_FILE_NAME                              "../../../ref/crc/7100Ref_ReducedMode_Resize.ref"
#define TEST_FEATURES_COLOR_CONVERSION_REF_FILE_NAME                    "../../../ref/crc/7100Ref_ReducedMode_Color_Conversion.ref"
#define TEST_FEATURES_JOB_REF_FILE_NAME                                 "../../../ref/crc/7100Ref_ReducedMode_Job.ref"
#define TEST_FEATURES_ALU_REF_FILE_NAME                                 "../../../ref/crc/7100Ref_ReducedMode_ALU.ref"
#define TEST_FEATURES_MASK_WORD_REF_FILE_NAME                           "../../../ref/crc/7100Ref_ReducedMode_Mask_word.ref"
#define TEST_FEATURES_COLOR_KEY_REF_FILE_NAME                           "../../../ref/crc/7100Ref_ReducedMode_Color_Key.ref"
#define TEST_FEATURES_SETPIXEL_REF_FILE_NAME                            "../../../ref/crc/7100Ref_ReducedMode_SetPixel.ref"
#define TEST_FEATURES_DRAWLINE_REF_FILE_NAME                            "../../../ref/crc/7100Ref_ReducedMode_DrawLine.ref"
#endif /* ST_OSLINUX */
#endif /* ST_7100 */

#endif /* TEST_FEATURES_FULL_MODE */

#ifdef ST_OSLINUX
#define TEST_FEATURES_SOURCE1_FILE_NAME                                 "./blt/files/crc_files/ElJam.gam"
#define TEST_FEATURES_SOURCE2_FILE_NAME                                 "./blt/files/crc_files/SidiBou.gam"
#define TEST_FEATURES_TARGET1_FILE_NAME                                 "./blt/files/crc_files/SidiBouARGB8888.gam"
#define TEST_FEATURES_TARGET2_FILE_NAME                                 "./blt/files/crc_files/SidiBou422R.gam"
#define TEST_FEATURES_TARGET3_FILE_NAME                                 "./blt/files/crc_files/SidiBouCLUT8.gam"

#define TEST_FEATURES_SOURCE_ARGB8888_FILE_NAME                         "./blt/files/crc_files/SidiBouARGB8888.gam"
#define TEST_FEATURES_SOURCE_RGB888_FILE_NAME                           "./blt/files/crc_files/SidiBouRGB888.gam"
#define TEST_FEATURES_SOURCE_RGB565_FILE_NAME                           "./blt/files/crc_files/SidiBouRGB565.gam"
#define TEST_FEATURES_SOURCE_422R_FILE_NAME                             "./blt/files/crc_files/SidiBou422R.gam"
#define TEST_FEATURES_SOURCE_420MB_FILE_NAME                            "./blt/files/crc_files/SidiBou420MB.gam"
#define TEST_FEATURES_SOURCE_CLUT8_FILE_NAME                            "./blt/files/crc_files/SidiBouCLUT8.gam"
#define TEST_FEATURES_SOURCE_CLUT4_FILE_NAME                            "./blt/files/crc_files/SidiBouCLUT4.gam"
#define TEST_FEATURES_LOGICAL_ZERO_FILE_NAME                            "./blt/files/crc_files/Logical_zero_ARGB8888.gam"
#define TEST_FEATURES_LOGICAL_ONE_FILE_NAME                             "./blt/files/crc_files/Logical_one_ARGB8888.gam"
#define TEST_FEATURES_COLOR_KEY_FILE_NAME                               "./blt/files/crc_files/GreenColorKey_ARGB8888.gam"
#else /* ST_OSLINUX */
#define TEST_FEATURES_SOURCE1_FILE_NAME                                 "../../ElJam.gam"
#define TEST_FEATURES_SOURCE2_FILE_NAME                                 "../../SidiBou.gam"

#define TEST_FEATURES_TARGET1_FILE_NAME                                 "../../SidiBouARGB8888.gam"
#define TEST_FEATURES_TARGET2_FILE_NAME                                 "../../SidiBou422R.gam"
#define TEST_FEATURES_TARGET3_FILE_NAME                                 "../../SidiBouCLUT8.gam"

#define TEST_FEATURES_SOURCE_ARGB8888_FILE_NAME                         "../../SidiBouARGB8888.gam"
#define TEST_FEATURES_SOURCE_RGB888_FILE_NAME                           "../../SidiBouRGB888.gam"
#define TEST_FEATURES_SOURCE_RGB565_FILE_NAME                           "../../SidiBouRGB565.gam"
#define TEST_FEATURES_SOURCE_422R_FILE_NAME                             "../../SidiBou422R.gam"
#define TEST_FEATURES_SOURCE_420MB_FILE_NAME                            "../../SidiBou420MB.gam"
#define TEST_FEATURES_SOURCE_CLUT8_FILE_NAME                            "../../SidiBouCLUT8.gam"
#define TEST_FEATURES_SOURCE_CLUT4_FILE_NAME                            "../../SidiBouCLUT4.gam"
#define TEST_FEATURES_LOGICAL_ZERO_FILE_NAME                            "../../Logical_zero_ARGB8888.gam"
#define TEST_FEATURES_LOGICAL_ONE_FILE_NAME                             "../../Logical_one_ARGB8888.gam"
#define TEST_FEATURES_COLOR_KEY_FILE_NAME                               "../../GreenColorKey_ARGB8888.gam"
#endif


#if defined(STBLIT_DEBUG_GET_STATISTICS) || defined(STBLIT_DEBUG_GET_STATISTICS_APPLICATION_LEVEL)
/* Performences test related */
#define TEST_PERFORMENCES_STEP 5
#define TEST_PERFORMENCES_DELAY 5
#endif

/* MultiInstances test related */
#define TEST_MULTI_INSTANCES_STEP 1
#define TEST_MULTI_INSTANCES_DELAY_VALUE 50

#ifdef ST_OSLINUX
#define TEST_MULTI_INSTANCES_SOURCE1_FILE_NAME                          "./blt/files/crc_files/ElJam.gam"
#define TEST_MULTI_INSTANCES_TARGET1_FILE_NAME                          "./blt/files/crc_files/ElJam.gam"
#define TEST_MULTI_INSTANCES_TARGET2_FILE_NAME                          "./blt/files/crc_files/ElJam.gam"

#if defined(ST_7109) || defined(ST_7200)
#define TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP1_FILE_NAME        "./blt/result/test_MultiInstances7109_SynchronousMode_Bitmap1.gam"
#define TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP2_FILE_NAME        "./blt/result/test_MultiInstances7109_SynchronousMode_Bitmap2.gam"
#define TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP1_FILE_NAME       "./blt/result/test_MultiInstances7109_AsynchronousMode_Bitmap1.gam"
#define TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP2_FILE_NAME       "./blt/result/test_MultiInstances7109_AsynchronousMode_Bitmap2.gam"
#define TEST_MULTI_INSTANCES_REF_FILE_NAME                              "./blt/ref/7109Ref_MultiInstances.ref"
#endif /* ST_7109 */

#ifdef ST_7100
#define TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP1_FILE_NAME        "./blt/result/test_MultiInstances7100_SynchronousMode_Bitmap1.gam"
#define TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP2_FILE_NAME        "./blt/result/test_MultiInstances7100_SynchronousMode_Bitmap2.gam"
#define TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP1_FILE_NAME       "./blt/result/test_MultiInstances7100_AsynchronousMode_Bitmap1.gam"
#define TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP2_FILE_NAME       "./blt/result/test_MultiInstances7100_AsynchronousMode_Bitmap2.gam"
#define TEST_MULTI_INSTANCES_REF_FILE_NAME                              "./blt/ref/7100Ref_MultiInstances.ref"
#endif /* ST_7100 */

#else /* !ST_OSLINUX */
#define TEST_MULTI_INSTANCES_SOURCE1_FILE_NAME                          "../../ElJam.gam"
#define TEST_MULTI_INSTANCES_TARGET1_FILE_NAME                          "../../ElJam.gam"
#define TEST_MULTI_INSTANCES_TARGET2_FILE_NAME                          "../../ElJam.gam"
#if defined(ST_7109) || defined(ST_7200)
#define TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP1_FILE_NAME        "../../../result/test_MultiInstances7109_SynchronousMode_Bitmap1.gam"
#define TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP2_FILE_NAME        "../../../result/test_MultiInstances7109_SynchronousMode_Bitmap2.gam"
#define TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP1_FILE_NAME       "../../../result/test_MultiInstances7109_AsynchronousMode_Bitmap1.gam"
#define TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP2_FILE_NAME       "../../../result/test_MultiInstances7109_AsynchronousMode_Bitmap2.gam"
#define TEST_MULTI_INSTANCES_REF_FILE_NAME                              "../../../ref/crc/7109Ref_MultiInstances.ref"
#endif /* ST_7109 */
#ifdef ST_7100
#define TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP1_FILE_NAME        "../../../result/test_MultiInstances7100_SynchronousMode_Bitmap1.gam"
#define TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP2_FILE_NAME        "../../../result/test_MultiInstances7100_SynchronousMode_Bitmap2.gam"
#define TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP1_FILE_NAME       "../../../result/test_MultiInstances7100_AsynchronousMode_Bitmap1.gam"
#define TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP2_FILE_NAME       "../../../result/test_MultiInstances7100_AsynchronousMode_Bitmap2.gam"
#define TEST_MULTI_INSTANCES_REF_FILE_NAME                              "../../../ref/crc/7100Ref_MultiInstances.ref"
#endif /* ST_7100 */
#ifdef ST_5105
#define TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP1_FILE_NAME        "../../../result/test_MultiInstances5105_SynchronousMode_Bitmap1.gam"
#define TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP2_FILE_NAME        "../../../result/test_MultiInstances5105_SynchronousMode_Bitmap2.gam"
#define TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP1_FILE_NAME       "../../../result/test_MultiInstances5105_AsynchronousMode_Bitmap1.gam"
#define TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP2_FILE_NAME       "../../../result/test_MultiInstances5105_AsynchronousMode_Bitmap2.gam"
#define TEST_MULTI_INSTANCES_REF_FILE_NAME                              "../../../ref/crc/5105Ref_MultiInstances.ref"
#endif /* ST_5105 */
#ifdef ST_7710
#define TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP1_FILE_NAME        "../../../result/test_MultiInstances7710_SynchronousMode_Bitmap1.gam"
#define TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP2_FILE_NAME        "../../../result/test_MultiInstances7710_SynchronousMode_Bitmap2.gam"
#define TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP1_FILE_NAME       "../../../result/test_MultiInstances7710_AsynchronousMode_Bitmap1.gam"
#define TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP2_FILE_NAME       "../../../result/test_MultiInstances7710_AsynchronousMode_Bitmap2.gam"
#define TEST_MULTI_INSTANCES_REF_FILE_NAME                              "../../../ref/crc/7710Ref_MultiInstances.ref"
#endif /* ST_7710 */
#endif /* ST_OSLINUX */

#if defined (ST_5100)
#define NB_TICKS_PER_MILLI_SECOND   15625000
#elif defined (ST_5105)
#define NB_TICKS_PER_MILLI_SECOND   6250
#elif defined (ST_5301)
#define NB_TICKS_PER_MILLI_SECOND   1002345000
#elif defined (ST_7710)
#define NB_TICKS_PER_MILLI_SECOND   6250
#elif defined (ST_7109)
#ifdef ST_OSLINUX
#define NB_TICKS_PER_MILLI_SECOND   1
#else /* !ST_OSLINUX */
#define NB_TICKS_PER_MILLI_SECOND   259.277
#endif /* ST_OSLINUX */
#elif defined (ST_7100)
#ifdef ST_OSLINUX
#define NB_TICKS_PER_MILLI_SECOND   1
#else /* !ST_OSLINUX */
#define NB_TICKS_PER_MILLI_SECOND   259.277
#endif /* ST_OSLINUX */
#elif defined (ST_7200)
#ifdef ST_OSLINUX
#define NB_TICKS_PER_MILLI_SECOND   1
#else /* !ST_OSLINUX */
#define NB_TICKS_PER_MILLI_SECOND   0.341796
#endif /* ST_OSLINUX */

#endif

/* Blit Demo related */
#define BLIT_DEMO_DEFAULT_INIT_MODE 0
#define BLIT_DEMO_DEFAULT_DEVICE_NAME "Blitter"
#define BLIT_DEMO_DEFAULT_OFFSETX 20
#define BLIT_DEMO_DEFAULT_OFFSETY 20
#define BLIT_DEMO_DEFAULT_WIDTH 200
#define BLIT_DEMO_DEFAULT_HEIGHT 200
#define BLIT_DEMO_DEFAULT_STEP 2
#ifdef ST_OSLINUX
#define BLIT_DEMO_DEFAULT_SOURCE_PATH "./blt/files/suzieRGB565.gam"
#else
#define BLIT_DEMO_DEFAULT_SOURCE_PATH "../../suzieRGB565.gam"
#endif




/* Private Variables (static)------------------------------------------------ */
static char BLT_Msg[200];                              /* text for trace */

/* TypeRasterTopBottom Test related */
static U32 NbBytePerPixel = 2; /* For RGB565 */
static STGXOBJ_ColorType_t TopBottomFieldColorType = STGXOBJ_COLOR_TYPE_RGB565;
static STGXOBJ_BitmapType_t TopBottomFieldBitmapType = STGXOBJ_BITMAP_TYPE_RASTER_TOP_BOTTOM;
static BOOL TopBottomFieldPreMultipliedColor = FALSE;
static STGXOBJ_ColorSpaceConversionMode_t TopBottomFieldColorSpaceConversion = STGXOBJ_ITU_R_BT709;
static STGXOBJ_AspectRatio_t TopBottomFieldAspectRatio = STGXOBJ_ASPECT_RATIO_SQUARE;
static U32 TopBottomFieldWidth = 741;
static U32 TopBottomFieldHeight = 492;
static U32 TopBottomFieldAllocAlignment = 2; /* For RGB565 */

#ifdef ST_OSLINUX
static char TopBottomSourceFileName[]      = "./blt/files/crowRGB565.gam"   ;
static char TopFieldFileName[]    = "./blt/files/crowRGB565_TopField.bin";
static char BottomFieldFileName[] = "./blt/files/crowRGB565_BottomField.bin";
#else
static char TopBottomSourceFileName[]      = "../../crowRGB565.gam"   ;
static char TopFieldFileName[]    = "../../crowRGB565_TopField.bin";
static char BottomFieldFileName[] = "../../crowRGB565_BottomField.bin";
#endif
#ifdef ST_OSLINUX
static char SrcFileName[]="./blt/files/suzieRGB565.gam";
static char DstFileName[]="./blt/files/merouRGB565.gam";
#else
static char SrcFileName[30]="../../suzieRGB565.gam";
static char DstFileName[30]="../../merouRGB565.gam";
#endif
static char UserTagString[]="BLIT";
static STGXOBJ_Bitmap_t        SourceBitmap;
static BOOL SnakeIsRunning = FALSE;
#ifdef ENABLE_RECCURENCE_MODE_TEST
static STLAYER_ViewPortHandle_t OSDVPHandle;
#endif
static STGXOBJ_Bitmap_t TargetBitmap;

static task_t*  InterruptProcessTask_p;
static void*    InterruptProcessTaskStack;
static tdesc_t  InterruptProcessTaskDesc;           /* Interrupt Process task descriptor */

#if defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109) || defined (ST_7200)
static char* BitmapColorType[] =
{
  "STGXOBJ_COLOR_TYPE_ARGB8888",
  "STGXOBJ_COLOR_TYPE_RGB888",
  "STGXOBJ_COLOR_TYPE_ARGB8565",
  "STGXOBJ_COLOR_TYPE_RGB565",
  "STGXOBJ_COLOR_TYPE_ARGB1555",
  "STGXOBJ_COLOR_TYPE_ARGB4444",

  "STGXOBJ_COLOR_TYPE_CLUT8",
  "STGXOBJ_COLOR_TYPE_CLUT4",
  "STGXOBJ_COLOR_TYPE_CLUT2",
  "STGXOBJ_COLOR_TYPE_CLUT1",
  "STGXOBJ_COLOR_TYPE_ACLUT88",
  "STGXOBJ_COLOR_TYPE_ACLUT44",

  "STGXOBJ_COLOR_TYPE_SIGNED_YCBCR888_444",
  "STGXOBJ_COLOR_TYPE_UNSIGNED_YCBCR888_444",
  "STGXOBJ_COLOR_TYPE_SIGNED_YCBCR888_422",
  "STGXOBJ_COLOR_TYPE_UNSIGNED_YCBCR888_422",
  "STGXOBJ_COLOR_TYPE_SIGNED_YCBCR888_420",
  "STGXOBJ_COLOR_TYPE_UNSIGNED_YCBCR888_420",
  "STGXOBJ_COLOR_TYPE_UNSIGNED_AYCBCR6888_444",
  "STGXOBJ_COLOR_TYPE_SIGNED_AYCBCR8888",
  "STGXOBJ_COLOR_TYPE_UNSIGNED_AYCBCR8888",

  "STGXOBJ_COLOR_TYPE_ALPHA1",
  "STGXOBJ_COLOR_TYPE_ALPHA4",
  "STGXOBJ_COLOR_TYPE_ALPHA8",
  "STGXOBJ_COLOR_TYPE_BYTE",

  "STGXOBJ_COLOR_TYPE_ARGB8888_255",
  "STGXOBJ_COLOR_TYPE_ARGB8565_255",
  "STGXOBJ_COLOR_TYPE_ACLUT88_255",
  "STGXOBJ_COLOR_TYPE_ALPHA8_255"
};

static task_t   *Task1_p, *Task2_p, *Task3_p, *Task4_p;
static void     *Task1Stack, *Task2Stack, *Task3Stack, *Task4Stack;
static tdesc_t  Task1Desc, Task2Desc, Task3Desc, Task4Desc;
static semaphore_t* Task1StartSemaphore_p;
static semaphore_t* Task1CompletedSemaphore_p;
static semaphore_t* Task2StartSemaphore_p;
static semaphore_t* Task2CompletedSemaphore_p;
static semaphore_t* Task3StartSemaphore_p;
static semaphore_t* Task3CompletedSemaphore_p;
static semaphore_t* Task4StartSemaphore_p;
static semaphore_t* Task4CompletedSemaphore_p;


/* MultiInstances test related */
static STGXOBJ_Bitmap_t        MultiInstancesTest_SourceBitmap, MultiInstancesTest_Target1Bitmap, MultiInstancesTest_Target2Bitmap;
static STBLIT_Handle_t         STBlitHandle1, STBlitHandle2, STBlitHandle3, STBlitHandle4;
static STEVT_SubscriberID_t    MultiInstancesTest_SubscriberID;
static BOOL                    Task1IsRunning = TRUE, Task2IsRunning = TRUE, Task3IsRunning = TRUE, Task4IsRunning = TRUE;
static BOOL                    Task1PerformBlit = FALSE, Task2PerformBlit = FALSE, Task3PerformBlit = FALSE, Task4PerformBlit = FALSE;
static BOOL                    MultiInstancesTest_WaitForNotification = FALSE;
static ST_ErrorCode_t          Task1ErrCode, Task2ErrCode, Task3ErrCode, Task4ErrCode;
char   Task1Tag[10] = "Task1Tag";
char   Task2Tag[10] = "Task2Tag";
char   Task3Tag[10] = "Task3Tag";
char   Task4Tag[10] = "Task4Tag";
#endif /* defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109) || defined (ST_7200) */


/* Blit Demo related */
typedef struct
{
	U32               InitMode;
	ST_DeviceName_t   DeviceName;
	U32               OffsetX;
    U32               OffsetY;
	U32               Width;
    U32               Height;
    U32               Step;
	char              SourcePath[100];
    STGXOBJ_Bitmap_t  SourceBitmap;
	STGXOBJ_Bitmap_t  TargetBitmap;
    STGXOBJ_Palette_t Palette;
} BlitDemoParameters_t;

static STEVT_DeviceSubscribeParams_t BlitDemoEvtSubscribeParams;
static STEVT_SubscriberID_t          BlitDemoSubscriberID;

static STGXOBJ_Bitmap_t     SnakeDemoTargetBitmap;

static BlitDemoParameters_t BlitDemoParameters;
static task_t*              BlitDemo_InterruptProcessTask_p;
static void*                BlitDemo_InterruptProcessTaskStack;
static tdesc_t              BlitDemo_InterruptProcessTaskDesc;           /* Interrupt Process task descriptor */
semaphore_t*                DemoBlitCompletionSemaphore_p;
char                        BlitDemoTagString[] = "BLIT_DEMO_TAG";
static BOOL                 BlitDemoIsRunning = FALSE;

static BlitDemoParameters_t RandomFillDemoParameters;
static task_t*              RandomFillDemo_InterruptProcessTask_p;
static void*                RandomFillDemo_InterruptProcessTaskStack;
static tdesc_t              RandomFillDemo_InterruptProcessTaskDesc;           /* Interrupt Process task descriptor */
semaphore_t*                RandomFillDemoCompletionSemaphore_p;
char                        RandomFillDemoTagString[] = "RANDOM_FILL_DEMO_TAG";
static BOOL                 RandomFillDemoIsRunning = FALSE;

static BlitDemoParameters_t RandomCopyDemoParameters;
static task_t*              RandomCopyDemo_InterruptProcessTask_p;
static void*                RandomCopyDemo_InterruptProcessTaskStack;
static tdesc_t              RandomCopyDemo_InterruptProcessTaskDesc;           /* Interrupt Process task descriptor */
semaphore_t*                RandomCopyDemoCompletionSemaphore_p;
char                        RandomCopyDemoTagString[] = "RANDOM_COPY_DEMO_TAG";
static BOOL                 RandomCopyDemoIsRunning = FALSE;




/* Extern Variables ------------------------------------------------ */
#ifndef ST_OSLINUX
extern STAVMEM_PartitionHandle_t     AvmemPartitionHandle[];
#endif
extern ST_Partition_t*               SystemPartition_p;
extern U32 InterruptCount;
extern U32 PrintInterruptCount;
#ifndef ST_OSLINUX
extern STAVMEM_SharedMemoryVirtualMapping_t VirtualMapping;
#endif
#ifdef ST_OSLINUX
extern MapParams_t   BlitterMap;
#define LINUX_BLITTER_BASE_ADDRESS    BlitterMap.MappedBaseAddress
#endif
#ifdef ST_OSLINUX
extern STLAYER_Handle_t FakeLayerHndl ;
#endif




/* Global Variables --------------------------------------------------------- */
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
#ifdef ST_OSLINUX
static char ResizeHExpansion_ResultingBitmap_FileName[]                             = "./blt/result/Resize_Horizontal_Expansion.gam";
static char ResizeHCompression_ResultingBitmap_FileName[]                           = "./blt/result/Resize_Horizontal_Compression.gam";
static char ResizeVExpansion_ResultingBitmap_FileName[]                             = "./blt/result/Resize_Vertical_Expansion.gam";
static char ResizeVCompression_ResultingBitmap_FileName[]                           = "./blt/result/Resize_Vertical_Compression.gam";
static char AntiFlutter_ResultingBitmap_FileName[]                                  = "./blt/result/AntiAliFlutter.gam";
static char AntiAliasing_ResultingBitmap_FileName[]                                 = "./blt/result/AntiAliasing.gam";
static char Fill_ResultingBitmap_FileName[]                                         = "./blt/result/Fill.gam";
static char Copy_ResultingBitmap_FileName[]                                         = "./blt/result/Copy.gam";

static char OverlapCopy_ResultingBitmap_FileName1[]                                 = "./blt/result/OverlapCopy1.gam";
static char OverlapCopy_ResultingBitmap_FileName2[]                                 = "./blt/result/OverlapCopy2.gam";
static char OverlapCopy_ResultingBitmap_FileName3[]                                 = "./blt/result/OverlapCopy3.gam";
static char OverlapCopy_ResultingBitmap_FileName4[]                                 = "./blt/result/OverlapCopy4.gam";
static char OverlapCopy_ResultingBitmap_FileName5[]                                 = "./blt/result/OverlapCopy5.gam";
static char OverlapCopy_ResultingBitmap_FileName6[]                                 = "./blt/result/OverlapCopy6.gam";
static char OverlapCopy_ResultingBitmap_FileName7[]                                 = "./blt/result/OverlapCopy7.gam";
static char OverlapCopy_ResultingBitmap_FileName8[]                                 = "./blt/result/OverlapCopy8.gam";

static char OverlapBlit_ResultingBitmap_FileName1[]                                 = "./blt/result/OverlapBlit1.gam";
static char OverlapBlit_ResultingBitmap_FileName2[]                                 = "./blt/result/OverlapBlit2.gam";
static char OverlapBlit_ResultingBitmap_FileName3[]                                 = "./blt/result/OverlapBlit3.gam";
static char OverlapBlit_ResultingBitmap_FileName4[]                                 = "./blt/result/OverlapBlit4.gam";
static char OverlapBlit_ResultingBitmap_FileName5[]                                 = "./blt/result/OverlapBlit5.gam";
static char OverlapBlit_ResultingBitmap_FileName6[]                                 = "./blt/result/OverlapBlit6.gam";
static char OverlapBlit_ResultingBitmap_FileName7[]                                 = "./blt/result/OverlapBlit7.gam";
static char OverlapBlit_ResultingBitmap_FileName8[]                                 = "./blt/result/OverlapBlit8.gam";

static char Draw_ResultingBitmap_FileName[]                                         = "./blt/result/Draw.gam";
static char Pix_ResultingBitmap_FileName[]                                          = "./blt/result/Pix.gam";
static char ColorKey_ResultingBitmap_FileName[]                                     = "./blt/result/ColorKey.gam";
static char AluAnd_ResultingBitmap_FileName[]                                       = "./blt/result/AluAnd.gam";
static char AluAndRev_ResultingBitmap_FileName[]                                    = "./blt/result/AluAndRev.gam";
static char AluAndInvert_ResultingBitmap_FileName[]                                 = "./blt/result/AluAndInvert.gam";
static char AluAndNoop_ResultingBitmap_FileName[]                                   = "./blt/result/AluAndNoop.gam";
static char AluXor_ResultingBitmap_FileName[]                                       = "./blt/result/AluXor.gam";
static char AluOr_ResultingBitmap_FileName[]                                        = "./blt/result/AluOr.gam";
static char AluNor_ResultingBitmap_FileName[]                                       = "./blt/result/AluNor.gam";
static char AluOrReverse_ResultingBitmap_FileName[]                                 = "./blt/result/AluOrReverse.gam";
static char AluOrInvert_ResultingBitmap_FileName[]                                  = "./blt/result/AluOrInvert.gam";
static char AluNand_ResultingBitmap_FileName[]                                      = "./blt/result/AluOrReverse.gam";
static char MaskWordRed_ResultingBitmap_FileName[]                                  = "./blt/result/MaskWordRed.gam";
static char MaskWordGreen_ResultingBitmap_FileName[]                                = "./blt/result/MaskWordGreen.gam";
static char MaskWordBlue_ResultingBitmap_FileName[]                                 = "./blt/result/MaskWordBlue.gam";
static char MaskWordRemoveRed_ResultingBitmap_FileName[]                            = "./blt/result/MaskWordRemoveRed.gam";
static char MaskWordRemoveGreen_ResultingBitmap_FileName[]                          = "./blt/result/MaskWordRemoveGreen.gam";
static char MaskWordRemoveBlue_ResultingBitmap_FileName[]                           = "./blt/result/MaskWordRemoveBlue.gam";
static char ClipInside_ResultingBitmap_FileName[]                                   = "./blt/result/ClipInside.gam";
static char ClipOutside_ResultingBitmap_FileName[]                                  = "./blt/result/ClipOutside.gam";
static char ColorConversion_ARGB8888_to_CLUT8_ResultingBitmap_FileName[]            = "./blt/result/ColorConversion_ARGB8888_to_CLUT8.gam";
static char ColorConversion_ARGB8888_to_YCbCr422R_ResultingBitmap_FileName[]        = "./blt/result/ColorConversion_ARGB8888_to_YCbCr422R.gam";
static char ColorConversion_RGB888_to_ARGB8888_ResultingBitmap_FileName[]           = "./blt/result/ColorConversion_RGB888_to_ARGB8888.gam";
static char ColorConversion_RGB888_to_YCbCr422R_ResultingBitmap_FileName[]          = "./blt/result/ColorConversion_RGB888_to_YCbCr422R.gam";
static char ColorConversion_RGB565_to_ARGB8888_ResultingBitmap_FileName[]           = "./blt/result/ColorConversion_RGB565_to_ARGB8888.gam";
static char ColorConversion_RGB565_to_YCbCr422R_ResultingBitmap_FileName[]          = "./blt/result/ColorConversion_RGB565_to_YCbCr422R.gam";
static char ColorConversion_YCbCr422R_to_ARGB8888_ResultingBitmap_FileName[]        = "./blt/result/ColorConversion_YCbCr422R_to_ARGB8888.gam";
static char ColorConversion_YCbCr422R_to_CLUT8_ResultingBitmap_FileName[]           = "./blt/result/ColorConversion_YCbCr422R_to_CLUT8.gam";
static char ColorConversion_MB_to_ARGB8888_ResultingBitmap_FileName[]               = "./blt/result/ColorConversion_MB_to_ARGB8888.gam";
static char ColorConversion_MB_to_YCbCr422R_ResultingBitmap_FileName[]              = "./blt/result/ColorConversion_MB_to_YCbCr422R.gam";

static char VC1Range_Mapping_MB_to_YCbCr422R_ResultingBitmap_FileName[]             = "./blt/result/VC1Range_Mapping_MB_to_YCbCr422R.gam";
static char VC1Range_Reduction_MB_to_YCbCr422R_ResultingBitmap_FileName[]           = "./blt/result/VC1Range_Reduction_MB_to_YCbCr422R.gam";

static char ColorConversion_CLUT8_to_ARGB8888_ResultingBitmap_FileName[]            = "./blt/result/ColorConversion_CLUT8_to_ARGB8888.gam";
static char ColorConversion_CLUT8_to_YCbCr422R_ResultingBitmap_FileName[]           = "./blt/result/ColorConversion_CLUT8_to_YCbCr422R.gam";

static char ColorConversion_CLUT4_to_ARGB8888_ResultingBitmap_FileName[]            = "./blt/result/ColorConversion_CLUT4_to_ARGB8888.gam";
static char ColorConversion_CLUT4_to_YCbCr422R_ResultingBitmap_FileName[]           = "./blt/result/ColorConversion_CLUT4_to_YCbCr422R.gam";

static char Copy2Src_ResultingBitmap_FileName[]                                     = "./blt/result/Copy2Src.gam";
static char BlendBitmap_ResultingBitmap_FileName[]                                  = "./blt/result/BlendBitmap.gam";
static char BlendColor_ResultingBitmap_FileName[]                                   = "./blt/result/BlendColor.gam";
static char FlickerFilter_ResultingBitmap_FileName[]                                = "./blt/result/FlickerFilter.gam";
static char VC1Range_ResultingBitmap_FileName[]                                     = "./blt/result/VC1Range.gam";
static char MBConversion_ResultingBitmap_FileName[]                                 = "./blt/result/MBConversion.gam";
static char Job_ResultingBitmap_FileName1[]                                         = "./blt/result/Job1.gam";
static char Job_ResultingBitmap_FileName2[]                                         = "./blt/result/Job2.gam";
static char SetPixel_ResultingBitmap_FileName[]                                     = "./blt/result/SetPixel.gam";
static char DrawHLine_ResultingBitmap_FileName[]                                     = "./blt/result/DrawHLine.gam";
static char DrawVLine_ResultingBitmap_FileName[]                                     = "./blt/result/DrawVLine.gam";

static char ColorKeySourceMode_ResultingBitmap_FileName[]                           = "./blt/result/ColorKeySourceMode.gam";
static char ColorKeyDestinationMode_ResultingBitmap_FileName[]                      = "./blt/result/ColorKeyDestinationMode.gam";



#else
static char ResizeHExpansion_ResultingBitmap_FileName[]                             = "../../../result/Resize_Horizontal_Expansion.gam";
static char ResizeHCompression_ResultingBitmap_FileName[]                           = "../../../result/Resize_Horizontal_Compression.gam";
static char ResizeVExpansion_ResultingBitmap_FileName[]                             = "../../../result/Resize_Vertical_Expansion.gam";
static char ResizeVCompression_ResultingBitmap_FileName[]                           = "../../../result/Resize_Vertical_Compression.gam";
static char AntiFlutter_ResultingBitmap_FileName[]                                  = "../../../result/AntiAliFlutter.gam";
static char AntiAliasing_ResultingBitmap_FileName[]                                 = "../../../result/AntiAliasing.gam";
static char Fill_ResultingBitmap_FileName[]                                         = "../../../result/Fill.gam";
static char Copy_ResultingBitmap_FileName[]                                         = "../../../result/Copy.gam";

static char OverlapCopy_ResultingBitmap_FileName1[]                                 = "../../../result/OverlapCopy1.gam";
static char OverlapCopy_ResultingBitmap_FileName2[]                                 = "../../../result/OverlapCopy2.gam";
static char OverlapCopy_ResultingBitmap_FileName3[]                                 = "../../../result/OverlapCopy3.gam";
static char OverlapCopy_ResultingBitmap_FileName4[]                                 = "../../../result/OverlapCopy4.gam";
static char OverlapCopy_ResultingBitmap_FileName5[]                                 = "../../../result/OverlapCopy5.gam";
static char OverlapCopy_ResultingBitmap_FileName6[]                                 = "../../../result/OverlapCopy6.gam";
static char OverlapCopy_ResultingBitmap_FileName7[]                                 = "../../../result/OverlapCopy7.gam";
static char OverlapCopy_ResultingBitmap_FileName8[]                                 = "../../../result/OverlapCopy8.gam";

static char OverlapBlit_ResultingBitmap_FileName1[]                                 = "../../../result/OverlapBlit1.gam";
static char OverlapBlit_ResultingBitmap_FileName2[]                                 = "../../../result/OverlapBlit2.gam";
static char OverlapBlit_ResultingBitmap_FileName3[]                                 = "../../../result/OverlapBlit3.gam";
static char OverlapBlit_ResultingBitmap_FileName4[]                                 = "../../../result/OverlapBlit4.gam";
static char OverlapBlit_ResultingBitmap_FileName5[]                                 = "../../../result/OverlapBlit5.gam";
static char OverlapBlit_ResultingBitmap_FileName6[]                                 = "../../../result/OverlapBlit6.gam";
static char OverlapBlit_ResultingBitmap_FileName7[]                                 = "../../../result/OverlapBlit7.gam";
static char OverlapBlit_ResultingBitmap_FileName8[]                                 = "../../../result/OverlapBlit8.gam";

static char Draw_ResultingBitmap_FileName[]                                         = "../../../result/Draw.gam";
static char Pix_ResultingBitmap_FileName[]                                          = "../../../result/Pix.gam";
static char ColorKeySourceMode_ResultingBitmap_FileName[]                           = "../../../result/ColorKeySourceMode.gam";
static char ColorKeyDestinationMode_ResultingBitmap_FileName[]                      = "../../../result/ColorKeyDestinationMode.gam";
static char AluAnd_ResultingBitmap_FileName[]                                       = "../../../result/AluAnd.gam";
static char AluAndRev_ResultingBitmap_FileName[]                                    = "../../../result/AluAndRev.gam";
static char AluAndInvert_ResultingBitmap_FileName[]                                 = "../../../result/AluAndInvert.gam";
static char AluAndNoop_ResultingBitmap_FileName[]                                   = "../../../result/AluAndNoop.gam";
static char AluXor_ResultingBitmap_FileName[]                                       = "../../../result/AluXor.gam";
static char AluOr_ResultingBitmap_FileName[]                                        = "../../../result/AluOr.gam";
static char AluNor_ResultingBitmap_FileName[]                                       = "../../../result/AluNor.gam";
static char AluOrReverse_ResultingBitmap_FileName[]                                 = "../../../result/AluOrReverse.gam";
static char AluOrInvert_ResultingBitmap_FileName[]                                  = "../../../result/AluOrInvert.gam";
static char AluNand_ResultingBitmap_FileName[]                                      = "../../../result/AluOrReverse.gam";
static char MaskWordRed_ResultingBitmap_FileName[]                                  = "../../../result/MaskWordRed.gam";
static char MaskWordGreen_ResultingBitmap_FileName[]                                = "../../../result/MaskWordGreen.gam";
static char MaskWordBlue_ResultingBitmap_FileName[]                                 = "../../../result/MaskWordBlue.gam";
static char MaskWordRemoveRed_ResultingBitmap_FileName[]                            = "../../../result/MaskWordRemoveRed.gam";
static char MaskWordRemoveGreen_ResultingBitmap_FileName[]                          = "../../../result/MaskWordRemoveGreen.gam";
static char MaskWordRemoveBlue_ResultingBitmap_FileName[]                           = "../../../result/MaskWordRemoveBlue.gam";
static char ClipInside_ResultingBitmap_FileName[]                                   = "../../../result/ClipInside.gam";
static char ClipOutside_ResultingBitmap_FileName[]                                  = "../../../result/ClipOutside.gam";
static char ColorConversion_ARGB8888_to_CLUT8_ResultingBitmap_FileName[]            = "../../../result/ColorConversion_ARGB8888_to_CLUT8.gam";
static char ColorConversion_ARGB8888_to_YCbCr422R_ResultingBitmap_FileName[]        = "../../../result/ColorConversion_ARGB8888_to_YCbCr422R.gam";
static char ColorConversion_RGB888_to_ARGB8888_ResultingBitmap_FileName[]           = "../../../result/ColorConversion_RGB888_to_ARGB8888.gam";
static char ColorConversion_RGB888_to_YCbCr422R_ResultingBitmap_FileName[]          = "../../../result/ColorConversion_RGB888_to_YCbCr422R.gam";
static char ColorConversion_RGB565_to_ARGB8888_ResultingBitmap_FileName[]           = "../../../result/ColorConversion_RGB565_to_ARGB8888.gam";
static char ColorConversion_RGB565_to_YCbCr422R_ResultingBitmap_FileName[]          = "../../../result/ColorConversion_RGB565_to_YCbCr422R.gam";
static char ColorConversion_YCbCr422R_to_ARGB8888_ResultingBitmap_FileName[]        = "../../../result/ColorConversion_YCbCr422R_to_ARGB8888.gam";
static char ColorConversion_YCbCr422R_to_CLUT8_ResultingBitmap_FileName[]           = "../../../result/ColorConversion_YCbCr422R_to_CLUT8.gam";
static char ColorConversion_MB_to_ARGB8888_ResultingBitmap_FileName[]               = "../../../result/ColorConversion_MB_to_ARGB8888.gam";
static char ColorConversion_MB_to_YCbCr422R_ResultingBitmap_FileName[]              = "../../../result/ColorConversion_MB_to_YCbCr422R.gam";
static char VC1Range_Mapping_MB_to_YCbCr422R_ResultingBitmap_FileName[]             = "../../../result/VC1Range_Mapping_MB_to_YCbCr422R.gam";
static char VC1Range_Reduction_MB_to_YCbCr422R_ResultingBitmap_FileName[]           = "../../../result/VC1Range_Reduction_MB_to_YCbCr422R.gam";
static char ColorConversion_CLUT8_to_ARGB8888_ResultingBitmap_FileName[]            = "../../../result/ColorConversion_CLUT8_to_ARGB8888.gam";
static char ColorConversion_CLUT8_to_YCbCr422R_ResultingBitmap_FileName[]           = "../../../result/ColorConversion_CLUT8_to_YCbCr422R.gam";
static char ColorConversion_CLUT4_to_ARGB8888_ResultingBitmap_FileName[]            = "../../../result/ColorConversion_CLUT4_to_ARGB8888.gam";
static char ColorConversion_CLUT4_to_YCbCr422R_ResultingBitmap_FileName[]           = "../../../result/ColorConversion_CLUT4_to_YCbCr422R.gam";
static char Copy2Src_ResultingBitmap_FileName[]                                     = "../../../result/Copy2Src.gam";
static char BlendBitmap_ResultingBitmap_FileName[]                                  = "../../../result/BlendBitmap.gam";
static char BlendColor_ResultingBitmap_FileName[]                                   = "../../../result/BlendColor.gam";
static char FlickerFilter_ResultingBitmap_FileName[]                                = "../../../result/FlickerFilter.gam";
static char VC1Range_ResultingBitmap_FileName[]                                     = "../../../result/VC1Range.gam";
static char MBConversion_ResultingBitmap_FileName[]                                 = "../../../result/MBConversion.gam";
static char Job_ResultingBitmap_FileName1[]                                         = "../../../result/Job1.gam";
static char Job_ResultingBitmap_FileName2[]                                         = "../../../result/Job2.gam";
static char SetPixel_ResultingBitmap_FileName[]                                     = "../../../result/SetPixel.gam";
static char DrawHLine_ResultingBitmap_FileName[]                                    = "../../../result/DrawHLine.gam";
static char DrawVLine_ResultingBitmap_FileName[]                                    = "../../../result/DrawVLine.gam";
#endif
#endif /* TEST_FEATURE_SAVE_OUTPUT_BITMAP */
/* Private Macros ----------------------------------------------------------- */


/* Private Function prototypes ---------------------------------------------- */
static void SnakeDemo_Task(void *Param_p);
static void BlitDemo_Task(void *Param_p);
static void RandomFillDemo_Task(void *Param_p);
static void RandomCopyDemo_Task(void *Param_p);

#if defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109) || defined (ST_7200)
static void MultiInstancesTest_Task1(void *Param_p);
static void MultiInstancesTest_Task2(void *Param_p);
static void MultiInstancesTest_Task3(void *Param_p);
static void MultiInstancesTest_Task4(void *Param_p);
#endif /* defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109) */

/* Extern Function prototypes ---------------------------------------------- */


/* Functions ---------------------------------------------------------------- */


/*
================================================================================
Utilities functions
================================================================================
*/
/*----------------------------------------------------------------------
* Function : random
            give a number between min and max
*  Input    : int min, int max
*  Output   : none
*  Return   : int nbr
* --------------------------------------------------------------------*/
static int BlitDemo_Random(int min, int max)
{
	if (max <= min)
	{
		return min;
	}
	else
	{
		srand (time_now());
    	return(min + (int)(rand()%(max+1-min)));
	}
}


#ifndef STBLIT_EMULATOR
static void ResetEngine(void)
{
#if defined(ST_GX1)

#else
    void *Address;

#ifdef ST_OSLINUX
    Address = LINUX_BLITTER_BASE_ADDRESS;
#else
    Address = CURRENT_BASE_ADDRESS;
#endif

#ifndef ST_OSLINUX   /*not implemented for linux */
    STSYS_WriteRegDev32LE(Address, 1);
    STSYS_WriteRegDev32LE(Address, 0);
#endif
#endif
}
#endif


#ifdef ST_OSLINUX
void layer_init( void )
{
#ifndef ST_ZEUS_PTV_MEM
    STLAYER_InitParams_t    STLAYER_InitParams;
    STLAYER_LayerParams_t   STLAYER_LayerParams;
    ST_ErrorCode_t          Err;

    STLAYER_LayerParams.AspectRatio = STGXOBJ_ASPECT_RATIO_4TO3;
    STLAYER_LayerParams.ScanType    = STGXOBJ_INTERLACED_SCAN;
    /* init vtg before layer! */
    STLAYER_LayerParams.Width       = 720;
    STLAYER_LayerParams.Height      = 480;

    STLAYER_InitParams.DeviceBaseAddress_p         = (void *)0;
    STLAYER_InitParams.SharedMemoryBaseAddress_p   = (void *)0;

    STLAYER_InitParams.LayerParams_p               = &STLAYER_LayerParams;
    STLAYER_InitParams.CPUPartition_p              = DriverPartition_p;
    STLAYER_InitParams.CPUBigEndian                = false;
    STLAYER_InitParams.AVMEM_Partition             = (STAVMEM_PartitionHandle_t)0;
    STLAYER_InitParams.NodeBufferUserAllocated     = FALSE;
    STLAYER_InitParams.ViewPortNodeBuffer_p        = (void *)NULL;
    STLAYER_InitParams.ViewPortBufferUserAllocated = FALSE;
    STLAYER_InitParams.ViewPortBuffer_p            = (void *)NULL;

    strcpy(STLAYER_InitParams.EventHandlerName, STEVT_DEVICE_NAME);

    STLAYER_InitParams.MaxHandles          = 1;
    STLAYER_InitParams.MaxViewPorts        = 1;

#ifdef ST_7100
    STLAYER_InitParams.LayerType           = STLAYER_HDDISPO2_VIDEO1;
    STLAYER_InitParams.BaseAddress_p       = (void *)ST7100_VID1_LAYER_BASE_ADDRESS;
    STLAYER_InitParams.BaseAddress2_p      = (void *)ST7100_DISP0_BASE_ADDRESS;
#endif

#ifdef ST_7109
    STLAYER_InitParams.LayerType           = STLAYER_DISPLAYPIPE_VIDEO1;
    STLAYER_InitParams.BaseAddress_p       = (void *)ST7109_VID1_LAYER_BASE_ADDRESS;
    STLAYER_InitParams.BaseAddress2_p      = (void *)ST7109_DISP0_BASE_ADDRESS;
#endif

#ifdef ST_7200
    STLAYER_InitParams.LayerType           = STLAYER_DISPLAYPIPE_VIDEO2;
    STLAYER_InitParams.BaseAddress_p       = (void *)(ST7200_VID1_LAYER_BASE_ADDRESS);
    STLAYER_InitParams.BaseAddress2_p      = (void *)(ST7200_VDP_MAIN_BASE_ADDRESS);
#endif


    Err = STLAYER_Init("FOR_BLIT",&STLAYER_InitParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
    }
#endif
}

void layer_term( void )
{
#ifndef ST_ZEUS_PTV_MEM
    STLAYER_TermParams_t    STLAYER_TermParams;
    ST_ErrorCode_t          Err;

    STLAYER_TermParams.ForceTerminate = TRUE;
    Err =  STLAYER_Term( "FOR_BLIT", &STLAYER_TermParams ) ;
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"STLAYER_Term=%s\n", Err ));
    }
#endif  /*ST_ZEUS_PTV_MEM*/

}
#endif

/*-----------------------------------------------------------------------------
 * Function : BLIT_GetRevision
 *            Get driver revision number
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
static BOOL BLIT_GetRevision( parse_t *pars_p, char *result_sym_p ) {
    ST_Revision_t RevisionNumber;

    RevisionNumber = STBLIT_GetRevision( );
    sprintf( BLT_Msg, "STBLIT_GetRevision() : revision=%s\n", RevisionNumber );
    STTBX_Print((  BLT_Msg ));

    return ( FALSE );
} /* end BLIT_GetRevision */

/*-------------------------------------------------------------------------
 * Function : WaitForBlitterComplite
 * Input    :
 * Output   :
 * Return   :
 * ----------------------------------------------------------------------*/
static ST_ErrorCode_t WaitForBlitterComplite()
{
    STOS_Clock_t            time;
    time = STOS_time_plus(STOS_time_now(), 6250000 * 5);

    return STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_SetSource
 *
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
static BOOL BLIT_SetSource (STTST_Parse_t *pars_p, char *result_sym_p)
{
    BOOL Err;

    Err = STTST_GetString( pars_p,"../../suzieRGB565.gam", SrcFileName, sizeof(SrcFileName) );

    return Err;
}

/*-------------------------------------------------------------------------
 * Function : BLIT_SetTargetBitmap
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_SetTargetBitmap(STTST_Parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t RetErr;
    S32            Lvar;

    /*------------ get the osd layer vp handle ------------*/
    RetErr = STTST_GetInteger(pars_p, 0, &Lvar);
    if ( RetErr )
    {
        STTST_TagCurrentLine( pars_p, "expected bitmap handle" );
        return(TRUE);
    }

	TargetBitmap = *((STGXOBJ_Bitmap_t*)Lvar);

#ifdef ST_OSLINUX
	TargetBitmap.Data1_p = (void *)STLAYER_KernelToUser((U32)TargetBitmap.Data1_p);
#endif


    return (FALSE);
}


/*-------------------------------------------------------------------------
 * Function : BLIT_Init
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_Init( STTST_Parse_t *pars_p, char *result_sym_p )
{
    ST_ErrorCode_t          Err;
    STBLIT_InitParams_t     InitParams;
	ST_DeviceName_t         DeviceName;

	/*------------ get device name ------------*/
    Err = STTST_GetString( pars_p, STBLIT_DEVICE_NAME, DeviceName, sizeof(DeviceName) );
    if (Err)
    {
    	STTST_TagCurrentLine( pars_p, "Expected DeviceName" );
    	return(TRUE);
    }


    /* ------------ Blit Device Init ------------ */
	InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                       = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                       = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 10;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188)|| defined (ST_5525)\
|| defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;

/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
|| defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined (ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

	#ifdef ST_OSLINUX
    	layer_init();
	#endif

    Err = STBLIT_Init(DeviceName,&InitParams);
    if (Err == ST_NO_ERROR)
    {
        STTBX_Print (("STBLIT_Init(%s) : OK\n", DeviceName));
    }
    else
    {
        STTBX_Print (("STBLIT_Init(%s) : %d\n", DeviceName, Err));
        return (TRUE);
    }

    return Err;
}

/*-------------------------------------------------------------------------
 * Function : BLIT_Term
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_Term( STTST_Parse_t *pars_p, char *result_sym_p )
{
    ST_ErrorCode_t          Err;
    STBLIT_TermParams_t     TermParams;

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(STBLIT_DEVICE_NAME,&TermParams);
    if (Err == ST_NO_ERROR)
    {
        STTBX_Print (("STBLIT_Term() : OK\n"));
    }
    else
    {
        STTBX_Print (("STBLIT_Term() : %d\n",Err));
        return (TRUE);
    }

    return(Err);
}

#ifdef STBLIT_PERF_TEST
void testCopyCallback(U32 ID)
{
    printf ("In ********* %s ************ \n", __FUNCTION__);
}
#endif


/*-----------------------------------------------------------------------------
 * MES IP block initialization
 * --------------------------------------------------------------------------*/

#if defined (DVD_SECURED_CHIP)

#define SYS_REGION_BASE_ADDR    0x05580000
#define SYS_REGION_SZ           0x00A00000


/******************************************************************************
Displays the access mode String for readability.
******************************************************************************/
char *DisplayModeString(U32 uMode)
{
    char *pMode;
    switch(uMode)
    {
        case ACCESS_ALL :
            pMode= ": ACCESS_ALL :";
            break;
        case ACCESS_NO_STORE_INSECURE :
            pMode= ": ACCESS_NO_STORE_INSECURE :";
            break;
        case ACCESS_NO_SECURE :
            pMode= ": ACCESS_NO_SECURE :";
            break;
        case ACCESS_LOAD_SECURE_ONLY :
            pMode= ":ACCESS_LOAD_SECURE_ONLY :";
            break;
        default:
            pMode= ": Undefied Access Mode :";
            break;
    }
    return(pMode);
}

/******************************************************************************
Displays the Error String for an Error Code or returns a pointer to an error
string depending on the bReturnStringFlag. 0 = display error string, 1 = return
pointer to error string.
******************************************************************************/
char *DisplayError(ST_ErrorCode_t Error, U32 bReturnErrorString)
{
    char *pError;
    switch(Error)
    {
    case 0 :
        pError= "ST_NO_ERROR";
        break;
    case STMES_ERROR_ALREADY_INITIALISED :
        pError= "STMES_ERROR_ALREADY_INITIALISED";
        break;
    case STMES_ERROR_NOT_INITIALISED :
        pError= "STMES_ERROR_NOT_INITIALISED";
        break;
    case STMES_ERROR_ADDR_RANGE :
        pError= "STMES_ERROR_ADDR_RANGE";
        break;
    case STMES_ERROR_ADDR_ALIGN :
        pError= "STMES_ERROR_ADDR_ALIGN";
        break;
    case STMES_ERROR_MES_INCORRECT :
        pError= "STMES_ERROR_MES_INCORRECT";
        break;
    case STMES_ERROR_SID_INCORRECT :
        pError= "STMES_ERROR_SID_INCORRECT";
        break;
    case STMES_ERROR_NO_REGIONS :
        pError= "STMES_ERROR_NO_REGIONS";
        break;
    case STMES_ERROR_BUSY :
        pError= "STMES_ERROR_BUSY";
        break;
    default:
        pError= "Undefied Error Description";
        break;
    }
    /* if flag set to return the string pointer then return it */
    if( bReturnErrorString )
    return(pError);
    /* else display it */
    printf(":%s:", pError);
}

/*-----------------------------------------------------------------------------
 * Function :  Blit_MesInit
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
BOOL Blit_MesInit (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t ErrCode;
    BOOL RetErr;

    STMES_InitParams_t Init;
    unsigned int uError;
    unsigned int uCPU_Mode;
    unsigned int uMemoryStatus;
    unsigned int uExit= FALSE;

    /* get the driver version this can be done before the driver is Initialised. */
    printf("STMES Driver Version= %s\n", (char *)STMES_GetVersion());

    /* initialise the driver leaving all interrutps alone*/
    memset(&Init,0,sizeof(STMES_InitParams_t));
    if((uError= STMES_Init(&Init)) != ST_NO_ERROR)
    {
        printf("MES Init Failed : %s :\n", DisplayError(uError, TRUE));
        return(FALSE);
    }

    /* get the mode of the CPU that we are running on (SH4) this should be ACCESS_NO_SECURE as this is the default state from reset */
    if( (uCPU_Mode = STMES_GetSourceMode( SID_SH4_CPU, SYS_MES )) != ACCESS_NO_SECURE )
    {
        if(uCPU_Mode >= STMES_ERROR_ALREADY_INITIALISED)
        {
            printf("SH4 CPU is not in default Access Mode : %s :\n", DisplayModeString(uCPU_Mode) );
        }
        else
        {
            printf("Error for STMES_GetSourceMode() : %s :\n", DisplayError(uCPU_Mode, TRUE) );
            uExit= TRUE;
        }
    }
    else
    {
        printf("SH4 is in the default Access Mode : ACCESS_NO_SECURE :\n");
    }

    /* test to see if the memory in the SYS_LMI that we want to configure as a secure region is currently insecure */
    uMemoryStatus = STMES_IsMemorySecure( (void*)SYS_REGION_BASE_ADDR, SYS_REGION_SZ, 0 );
    if( uMemoryStatus >= STMES_ERROR_ALREADY_INITIALISED )
    {
        printf("Error for STMES_IsMemorySecure(0x%08x,0x%08x,0) : %s\n", SYS_REGION_BASE_ADDR, SYS_REGION_SZ, DisplayError(uMemoryStatus, TRUE));
        uExit= TRUE;
    }
    else if(uMemoryStatus == INSECURE_REGION)
    {
        printf("OK : Memory is INSECURE : STMES_IsMemorySecure(0x%08x,0x%08x,0)\n", SYS_REGION_BASE_ADDR, SYS_REGION_SZ);
    }
    else
    {
        /* the memory is already in a secure region so cannot be put into another secure region as the hardware is not defined to handle this case */
        printf("ERROR: Memory is SECURE : STMES_IsMemorySecure(0x%08x,0x%08x,N/A)\n", SYS_REGION_BASE_ADDR, SYS_REGION_SZ);
        uExit= TRUE;
    }

    /* now set up the region if able to do so, so that there is a secure region for the above address range */
    if( !uExit )
    {
        uMemoryStatus= STMES_SetSecureRange((void *)SYS_REGION_BASE_ADDR, (void *)(SYS_REGION_BASE_ADDR + SYS_REGION_SZ - 1), 0);
        if( uMemoryStatus != ST_NO_ERROR )
        {
            printf("Error for STMES_SetSecureRange(0x%08x,0x%08x,N/A) : %s\n", SYS_REGION_BASE_ADDR, (SYS_REGION_BASE_ADDR + SYS_REGION_SZ - 1),
            DisplayError(uMemoryStatus, TRUE));
            uExit= TRUE;
        }
        else
        {
            printf("Region Created : Start Addr= 0x%08x : End Addr= 0x%08x\n", SYS_REGION_BASE_ADDR, (SYS_REGION_BASE_ADDR + SYS_REGION_SZ - 1) );
        }
    }
    else
    {
        printf("Error : Region Not Created as Previous Error Encountered\n");
    }

    /* check that the memory that has been defined in a secure region is actually in a secure region */
    uMemoryStatus= STMES_IsMemorySecure( (void*)SYS_REGION_BASE_ADDR, SYS_REGION_SZ, 0 );
    if( uMemoryStatus >= STMES_ERROR_ALREADY_INITIALISED )
    {
        printf("Error for STMES_IsMemorySecure(0x%08x,0x%08x,N/A) : %s\n", SYS_REGION_BASE_ADDR, SYS_REGION_SZ, DisplayError(uMemoryStatus, TRUE));
        uExit= TRUE;
    }
    else if( uMemoryStatus == SECURE_REGION )
    {
        printf("OK : Memory Region is Now Secure\n");
        if( uCPU_Mode == ACCESS_ALL || uCPU_Mode == ACCESS_NO_STORE_INSECURE )
        {
            printf("Secure Region is Accessible by CPU\n");
        }
        else
        {
            printf("Secure Region is Inaccessible by CPU as CPU Mode= %s\n",
            DisplayModeString(uCPU_Mode) );
        }
    }
    else
    {
        printf("Error : Memory Region is Still Insecure !!!\n");
    }

    /* finally terminate the driver as finished with it */
//    if( (uError= STMES_Term()) != ST_NO_ERROR )
//    {
//    printf("Error Terminating the MES Driver %s\n", DisplayError(uError , TRUE) );
//    return(FALSE);
//    }

    return(TRUE);
}
#endif /* (DVD_SECURED_CHIP) */


/*-----------------------------------------------------------------------------
 * Function :  SetSrcFileName
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
BOOL SetSrcFileName (parse_t *pars_p, char *result_sym_p)

{
    BOOL ErrCode=FALSE;
    S32 Lvar;

    ErrCode = STTST_GetInteger( pars_p, 1, &Lvar);
    if (ErrCode == TRUE)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Expected file format\n"));
    }


    switch ( Lvar )
    {
        case  TYPE_CLUT2 :
            #ifdef ST_OSLINUX
                strcpy(SrcFileName,"./blt/files/suzieCLUT2.gam");
            #else
                strcpy(SrcFileName,"../../suzieCLUT2.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Src file format is CLUT2"));
            break;
        case  TYPE_CLUT4 :
            #ifdef ST_OSLINUX
                strcpy(SrcFileName,"./blt/files/suzieCLUT4.gam");
            #else
                strcpy(SrcFileName,"../../suzieCLUT4.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Src file format is CLUT4"));
            break;
        case  TYPE_CLUT8 :
            #ifdef ST_OSLINUX
                strcpy(SrcFileName,"./blt/files/suzieCLUT8.gam");
            #else
                strcpy(SrcFileName,"../../suzieCLUT8.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Src file format is CLUT8"));
            break;
        case  TYPE_RGB565 :
            #ifdef ST_OSLINUX
                strcpy(SrcFileName,"./blt/files/suzieRGB565.gam");
            #else
                strcpy(SrcFileName,"../../suzieRGB565.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Src file format is RGB565"));
            break;
        case  TYPE_RGB888 :
            #ifdef ST_OSLINUX
                strcpy(SrcFileName,"./blt/files/suzieRGB888.gam");
            #else
                strcpy(SrcFileName,"../../suzieRGB888.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Src file format is RGB888"));
            break;
        case  TYPE_YCbCr422R :
            #ifdef ST_OSLINUX
                strcpy(SrcFileName,"./blt/files/suzieYCbCr422R.gam");
            #else
                strcpy(SrcFileName,"../../suzieYCbCr422R.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Src file format is YCbCr422R"));
            break;
        default:
            #ifdef ST_OSLINUX
                strcpy(SrcFileName,"./blt/files/suzieRGB565.gam");
            #else
                strcpy(SrcFileName,"../../suzieRGB565.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Src file format is RGB565"));
            break;
    }

    return ErrCode;
}

/*-----------------------------------------------------------------------------
 * Function :  SetDstFileName
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
BOOL SetDstFileName (parse_t *pars_p, char *result_sym_p)
{
    BOOL ErrCode=FALSE;
    S32 Lvar;

    ErrCode = STTST_GetInteger( pars_p, 1, &Lvar);
    if (ErrCode == TRUE)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Expected file format\n"));
    }


    switch ( Lvar )
    {
        case  TYPE_CLUT2 :
            #ifdef ST_OSLINUX
                strcpy(DstFileName,"./blt/files/merouCLUT2.gam");
            #else
                strcpy(DstFileName,"../../merouCLUT2.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Dst file format is CLUT2"));
            break;
        case  TYPE_CLUT4 :
            #ifdef ST_OSLINUX
                strcpy(DstFileName,"./blt/files/merouCLUT4.gam");
            #else
                strcpy(DstFileName,"../../merouCLUT4.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Dst file format is CLUT4"));
            break;
        case  TYPE_CLUT8 :
            #ifdef ST_OSLINUX
                strcpy(DstFileName,"./blt/files/merouCLUT8.gam");
            #else
                strcpy(DstFileName,"../../merouCLUT8.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Dst file format is CLUT8"));
            break;
        case  TYPE_RGB565 :
            #ifdef ST_OSLINUX
                strcpy(DstFileName,"./blt/files/merouRGB565.gam");
            #else
                strcpy(DstFileName,"../../merouRGB565.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Dst file format is RGB565"));
            break;
        case  TYPE_RGB888 :
            #ifdef ST_OSLINUX
                strcpy(DstFileName,"./blt/files/merouRGB888.gam");
            #else
                strcpy(DstFileName,"../../merouRGB888.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Dst file format is RGB888"));
            break;
        case  TYPE_YCbCr422R :
            #ifdef ST_OSLINUX
                strcpy(DstFileName,"./blt/files/merouYCbCr422R.gam");
            #else
                strcpy(DstFileName,"../../merouYCbCr422R.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Dst file format is YCbCr422R"));
            break;
        default:
            #ifdef ST_OSLINUX
                strcpy(DstFileName,"./blt/files/merouRGB565.gam");
            #else
                strcpy(DstFileName,"../../merouRGB565.gam");
            #endif
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Dst file format is RGB565"));
            break;
    }

    return ErrCode;
}


/*
================================================================================
Level 1 functions
================================================================================
*/

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestBlitterCrashCheck;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestBlitterCrashCheck (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";
    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;

    STGXOBJ_Bitmap_t        TargetBitmap;
    STGXOBJ_Rectangle_t     Rectangle;
    STGXOBJ_Palette_t       Palette;
    STGXOBJ_Color_t         Color;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    BOOL                    EndBlits;
    U32                     i;

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

    /* ------------ Blit Init ------------ */
#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if ( Err != ST_NO_ERROR  &&  Err != ST_ERROR_ALREADY_INITIALIZED )
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }
    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }
    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /*-----------------2/26/01 9:04AM-------------------
     * Fill
     * --------------------------------------------------*/

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 20 ;
    Rectangle.PositionY = 20 ;
    Rectangle.Width     = 180 ;
    Rectangle.Height    = 180 ;

    /* ------------ Set Color ------------ */
    Color.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;

    Color.Value.RGB565.R     = 0xff ;
    Color.Value.RGB565.G     = 0x00 ;
    Color.Value.RGB565.B     = 0x00 ;

    /* ------------ Set Dest ------------ */
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = TRUE;
    Context.ClipRectangle           = Rectangle;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;



    /*=======================================================================*/
    /* Test1: Synchronous Mode                                               */
    /*=======================================================================*/

    /* --------------- Reset InterruptCount ------------ */
    PrintInterruptCount = TRUE;
    InterruptCount = 0;

    /* ------------ Blit ------------ */
    STTBX_Print(("\n\n--------- Test Synchronous Mode ---------"));
    for(i=1;i<=20;i++)
    {
        STTBX_Print(("\n\nSTBLIT_FillRectangle(%d)\n",i));
        Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&Rectangle,&Color,&Context );
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
            return (TRUE);
        }

    #ifndef STBLIT_EMULATOR
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*15);
            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT\n"));
            }
    #else
        /*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

        STTBX_Print(("\n> Blit Completion(%d)",i));
    #endif
    }


    /*=======================================================================*/
    /* Test2: Assynchronous Mode                                               */
    /*=======================================================================*/

    /* --------------- Reset InterruptCount ------------ */
    PrintInterruptCount = TRUE;
    InterruptCount = 0;

    /* ------------ Blit ------------ */
    STTBX_Print(("\n\n--------- Test Asynchronous Mode ---------"));

    for(i=1;i<=20;i++)
    {
        STTBX_Print(("\n\nSTBLIT_FillRectangle(%d)\n",i));
        Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&Rectangle,&Color,&Context );
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
            return (TRUE);
        }
    }
    /* Wait for Blits Completion*/
    EndBlits = FALSE;

    STTBX_Print(("\n\nWait for completion\n"));
    while(!EndBlits)
    {
        time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("\n\n END BLITS \n"));
            EndBlits = TRUE;
        }
    }

    PrintInterruptCount = FALSE;

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }


#ifdef ST_OSLINUX
    layer_term();
#endif


    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestResize;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestResize (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STGXOBJ_Palette_t       Palette;

	#ifndef STBLIT_EMULATOR
        STOS_Clock_t            time;
	#endif


    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;


    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/
    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)

    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        /*STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));*/
        /*return (TRUE);*/
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 120;
    Src.Rectangle.PositionY  = 50;
    Src.Rectangle.Width      = 100;
    Src.Rectangle.Height     = 100;
    Src.Palette_p            = &Palette;

    /* ------------ Set Dest ------------ */
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(DstFileName,SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(DstFileName,DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 20;
    Dst.Rectangle.PositionY   = 20;
    Dst.Rectangle.Width       = TargetBitmap.Width-40;
    Dst.Rectangle.Height      = Src.Rectangle.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
/*    Context.ClipRectangle           = Dst.Rectangle;*/
/*    Context.WriteInsideClipRectangle = TRUE;*/
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start horizontal expansion \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*200);
        #endif
        if (STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, &time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        }
        else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_2.3_1_resize_H_expansion.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_2.3_1_resize_H_expansion.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_2.3_1_resize_H_expansion.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_2.3_1_resize_H_expansion.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

#ifdef ST_OSLINUX
    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 120;
    Src.Rectangle.PositionY  = 50;
    Src.Rectangle.Width      = 100;
    Src.Rectangle.Height     = 100;
    Src.Palette_p            = &Palette;
#endif


    /* ------------ Set Dest ------------ */
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(DstFileName,SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(DstFileName,DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 20;
    Dst.Rectangle.PositionY   = 20;
    Dst.Rectangle.Width       = 20;
    Dst.Rectangle.Height      = Src.Rectangle.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
/*    Context.ClipRectangle           = Dst.Rectangle;*/
/*    Context.WriteInsideClipRectangle = TRUE;*/
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start horizontal compression \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*200);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_2.3_2_resize_H_compression.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_2.3_2_resize_H_compression.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_2.3_2_resize_H_compression.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_2.3_2_resize_H_compression.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

#ifdef ST_OSLINUX
    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 120;
    Src.Rectangle.PositionY  = 50;
    Src.Rectangle.Width      = 100;
    Src.Rectangle.Height     = 100;
    Src.Palette_p            = &Palette;

#endif


    /* ------------ Set Dest ------------ */
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(DstFileName,SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(DstFileName,DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 20;
    Dst.Rectangle.PositionY   = 20;
    Dst.Rectangle.Width       = Src.Rectangle.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height-40;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
/*    Context.ClipRectangle           = Dst.Rectangle;*/
/*    Context.WriteInsideClipRectangle = TRUE;*/
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start vertical expansion \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*200);
        #endif

        if (STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "ERROR"));
        }
        else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_2.3_3_resize_V_expansion.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_2.3_3_resize_V_expansion.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_2.3_3_resize_V_expansion.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_2.3_3_resize_V_expansion.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

#ifdef ST_OSLINUX
    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 120;
    Src.Rectangle.PositionY  = 50;
    Src.Rectangle.Width      = 100;
    Src.Rectangle.Height     = 100;
    Src.Palette_p            = &Palette;

#endif

    /* ------------ Set Dest ------------ */
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(DstFileName,SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(DstFileName,DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 20;
    Dst.Rectangle.PositionY   = 20;
    Dst.Rectangle.Width       = Src.Rectangle.Width;
    Dst.Rectangle.Height      = Src.Rectangle.Height/2;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
/*    Context.ClipRectangle           = Dst.Rectangle;*/
/*    Context.WriteInsideClipRectangle = TRUE;*/
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start vertical compression \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*200);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	   /* else */
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_2.3_4_resize_V_compression.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_2.3_4_resize_V_compression.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_2.3_4_resize_V_compression.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_2.3_4_resize_V_compression.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif



    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}


/*-----------------------------------------------------------------------------
 * Function : BLIT_TestAntiFlutter;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestAntiFlutter (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STGXOBJ_Palette_t       Palette;

	#ifndef STBLIT_EMULATOR
        STOS_Clock_t            time;
	#endif


    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;


    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/
    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)

    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        /*STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));*/
        /*return (TRUE);*/
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 120;
    Src.Rectangle.PositionY  = 50;
    Src.Rectangle.Width      = 100;
    Src.Rectangle.Height     = 100;
    Src.Palette_p            = &Palette;

    /* ------------ Set Dest ------------ */
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(DstFileName,SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(DstFileName,DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 20;
    Dst.Rectangle.PositionY   = 20;
    Dst.Rectangle.Width       = TargetBitmap.Width - 40;
    Dst.Rectangle.Height      = Src.Rectangle.Height + 40;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_ALPHA_BLEND;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 80;
    Context.EnableClipRectangle     = FALSE;
/*    Context.ClipRectangle           = Dst.Rectangle;*/
/*    Context.WriteInsideClipRectangle = TRUE;*/
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* EnableAntiFlutter feature*/
    STBLIT_EnableAntiFlutter(Handle);

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Blit operation with AntiAliFlutter Enabled \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
#ifdef ST_OSLINUX
    time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
#else
    time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*200);
#endif
    if (STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, &time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
    }


    /* set destination parameters */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 20;
    Dst.Rectangle.PositionY   = 170;
    Dst.Rectangle.Width       = TargetBitmap.Width - 40;
    Dst.Rectangle.Height      = Src.Rectangle.Height + 40;
    Dst.Palette_p             = NULL;

    /* DisableAntiAliasing feature*/
    STBLIT_DisableAntiFlutter(Handle);

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Blit operation with AntiFlutter Disabled \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }


	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*200);
        #endif
        if (STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, &time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        }
        else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_AntiAliFlutter.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_AntiFlutter.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_AntiFlutter.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_AntiFlutter.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestAntiAliasing;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestAntiAliasing (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STGXOBJ_Palette_t       Palette;

	#ifndef STBLIT_EMULATOR
        STOS_Clock_t            time;
	#endif


    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;


    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/
    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)

    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        /*STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));*/
        /*return (TRUE);*/
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 120;
    Src.Rectangle.PositionY  = 50;
    Src.Rectangle.Width      = 100;
    Src.Rectangle.Height     = 100;
    Src.Palette_p            = &Palette;

    /* ------------ Set Dest ------------ */
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(DstFileName,SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(DstFileName,DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 20;
    Dst.Rectangle.PositionY   = 20;
    Dst.Rectangle.Width       = TargetBitmap.Width - 40;
    Dst.Rectangle.Height      = Src.Rectangle.Height + 40;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_ALPHA_BLEND;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 80;
    Context.EnableClipRectangle     = FALSE;
/*    Context.ClipRectangle           = Dst.Rectangle;*/
/*    Context.WriteInsideClipRectangle = TRUE;*/
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* EnableAntiAliasing feature*/
    STBLIT_EnableAntiAliasing(Handle);

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Blit operation with AntiAliasing Enabled \n"));

    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
#ifdef ST_OSLINUX
    time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
#else
    time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*200);
#endif
    if (STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, &time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
    }


    /* set destination position */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 20;
    Dst.Rectangle.PositionY   = 170;
    Dst.Rectangle.Width       = TargetBitmap.Width - 40;
    Dst.Rectangle.Height      = Src.Rectangle.Height + 40;
    Dst.Palette_p             = NULL;

    /* DisableAntiAliasing feature*/
    STBLIT_DisableAntiAliasing(Handle);
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Blit operation with AntiAliasing Disabled \n"));

    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }


	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*200);
        #endif
        if (STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, &time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        }
        else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_AntiAliasing.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_AntiAliasing.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_AntiAliasing.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_AntiAliasing.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif



    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}


/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFill;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestFill (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";
    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;

    STGXOBJ_Bitmap_t        TargetBitmap;
    STGXOBJ_Rectangle_t     Rectangle;
    STGXOBJ_Palette_t       Palette;
    STGXOBJ_Color_t         Color;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
#ifdef STBLIT_PERF_TEST
    U32                     numLoops;
    U32                     numSizes;
#endif

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR  &&  Err != ST_ERROR_ALREADY_INITIALIZED)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init TestFill : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }
    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }
    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /*-----------------2/26/01 9:04AM-------------------
     * Fill
     * --------------------------------------------------*/

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 20 ;
    Rectangle.PositionY = 20 ;
    Rectangle.Width     = 180 ;
    Rectangle.Height    = 180 ;

    /* ------------ Set Color ------------ */
    Color.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;

    Color.Value.RGB565.R     = 0xff ;
    Color.Value.RGB565.G     = 0x00 ;
    Color.Value.RGB565.B     = 0x00 ;

    /* ------------ Set Dest ------------ */
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = TRUE;
    Context.ClipRectangle           = Rectangle;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;



    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Fill rectangle \n"));

/*#define CALC_TIME 1*/
#ifdef CALC_TIME
    {
        STOS_Clock_t     Tdiff, Tav, Ttot ;
        STOS_Clock_t     t1, t2 ;
        int              Count;

        Ttot = 0 ;

        /*Context.NotifyBlitCompletion    = FALSE;*/
        for ( Count = 1 ; Count <= 50 ; Count++ )
        {
            Rectangle.PositionX = 20 + Count % 50 ;
            Rectangle.PositionY = 20 + Count % 50 ;
            Rectangle.Width     = 100 + Count % 50 ;
            Rectangle.Height    = 100 + Count % 50 ;


            t1 = get_time_now();
            Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&Rectangle,&Color,&Context );

            if (STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY)!=0)
            {
                STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIME OUT"));
                ResetEngine();
            }

            t2 = get_time_now();
            Tdiff = t2 - t1 ;
            printf("---(%d)----> Time diff=%d \n", Count, Tdiff );

            Ttot += Tdiff ;
        }
        Tav = Ttot / Count-1 ;
        printf("-------> Count = %d, Time avarage=%d \n", Count-1, Tav );
    }
#else

#ifdef BLIT_PERFS_TEST
  if (gettimeofday(&t1, NULL) < 0)
  {
  	printf ("%s:.........gettimeofday() returned error \n", __FUNCTION__);
  }
#endif
    Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&Rectangle,&Color,&Context );

#ifdef BLIT_PERFS_TEST
    if (gettimeofday(&t2, NULL) < 0)
    {
    printf ("%s:.........gettimeofday() returned error \n", __FUNCTION__);
    }
    printf ("Time FillRectangle =  %.6d \n",  t2.tv_usec - t1.tv_usec + ((t2.tv_sec - t1.tv_sec) * 1000000));
#endif


    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*200);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT\n"));
            ResetEngine();
        }
        else
		{
	        /* Generate Bitmap */
            #ifdef ST_OSLINUX
#ifndef BLIT_PERFS_TEST
                Err = ConvertBitmapToGamma("./blt/result/test_2.4_Fill.gam",&TargetBitmap,&Palette);
#endif
            #else
                Err = ConvertBitmapToGamma("../../../result/test_2.4_Fill.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_2.4_Fill.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_2.4_Fill.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
	        return (TRUE);
		}
	#endif
#endif


    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }
    else
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Event Close : OK \n"));
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }


#ifdef ST_OSLINUX
    layer_term();
#endif


    return(FALSE);
}

#ifdef STBLIT_PERF_TEST
void testCopyCallback(U32 ID)
{
    printf ("In ********* %s ************ \n", __FUNCTION__);
}
#endif


/*-----------------------------------------------------------------------------
 * Function : BLIT_TestCopy;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestCopy (parse_t *pars_p, char *result_sym_p)
{


    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;

    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STGXOBJ_Rectangle_t     Rectangle;
    STGXOBJ_Palette_t       Palette;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif

#ifdef STBLIT_PERF_TEST
    U32                     numLoops;
    U32                     numSizes;
#endif

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                       = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                       = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188)|| defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;

/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR  &&  Err != ST_ERROR_ALREADY_INITIALIZED)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);


    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    Context.UserTag_p   = &UserTagString;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 40 ;
    Rectangle.PositionY = 30 ;
    Rectangle.Width     = 200 ;
    Rectangle.Height    = 200 ;

    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Dest ------------ */
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(DstFileName,SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(DstFileName,DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    /*Context.EnableClipRectangle     = TRUE;*/
    Context.ClipRectangle           = Rectangle;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
#ifdef STBLIT_PERF_TEST
    Context.UserTag_p               = /*0x5*/(void *) (testCopyCallback);
#else
    Context.UserTag_p               = NULL;
#endif
    Context.EnableResizeFilter      = TRUE;


    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Copy rectangle \n"));
#ifdef STBLIT_PERF_TEST
printf ("STBLIT Operation      Op Size (WxH)          Time (usec) for %d blits \n", NUM_BLIT_OP_LOOPS);
printf ("----------------------------------------------------------------------\n");
for (numSizes = 0; numSizes < NUM_BLIT_OP_SIZE_TABLE_ELEM; numSizes++)
{
  /*update fill rectangle size*/
  Rectangle.Width     = BlitOpSizeTable[numSizes].Width ;
  Rectangle.Height    = BlitOpSizeTable[numSizes].Height;
  Context.ClipRectangle           = Rectangle;
  printf ("COPY                  %dx%d                ", Rectangle.Width, Rectangle.Height);
  fflush(stdout);
  if (gettimeofday(&StartTime, NULL) < 0)
  {
  	printf ("%s:.........gettimeofday() returned error \n", __FUNCTION__);
  }
  for (numLoops = 0; numLoops < NUM_BLIT_OP_LOOPS; numLoops++)
  {

#endif /*STBLIT_PERF_TEST*/
    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&TargetBitmap,20,20,&Context );
     if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }


	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
            /* Generate Bitmap */
            #ifdef ST_OSLINUX
            #if !defined(PTV)  &&  !defined(STBLIT_PERF_TEST)
                Err = ConvertBitmapToGamma("./blt/result/test_2.5_Copy.gam",&TargetBitmap,&Palette);
            #endif
            #else
                Err = ConvertBitmapToGamma("../../../result/test_2.5_Copy.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
#ifdef STBLIT_PERF_TEST
          } /* for (numLoops)*/

  if (gettimeofday(&EndTime, NULL) < 0)
  {
  	printf ("%s:.........gettimeofday() returned error \n", __FUNCTION__);
  }


  printf ("  %.6d \n",  EndTime.tv_usec - StartTime.tv_usec + ((EndTime.tv_sec - StartTime.tv_sec) * 1000000));
} /* for (numSizes)*/
#endif /*STBLIT_PERF_TEST*/
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_2.5_Copy.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_2.5_Copy.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
#ifdef PTV
    Err = STBLIT_Close(Handle);
#else
    Err = STBLIT_Term(Name,&TermParams);
#endif /* PTV */
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestExtractFields;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestExtractFields (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;

    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STGXOBJ_Rectangle_t     Rectangle;
    STGXOBJ_Palette_t       Palette;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif

    STAVMEM_BlockHandle_t       BlockHandle1;
    STAVMEM_BlockHandle_t       BlockHandle2;
    STGXOBJ_BitmapAllocParams_t Params1;
    STGXOBJ_BitmapAllocParams_t Params2;
    FILE*                       fstream;        /* File handle for read operation          */
    U32                         size;


    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                       = AvmemPartitionHandle[0];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined (ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

    fstream = NULL;        /* File handle for read operation          */
    size = 0;

#ifdef ST_OSLINUX
    layer_init();
#endif

    /* ------------ Blit Init ------------ */
#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;


    Context.UserTag_p   = &UserTagString;
    Context.EnableResizeFilter      = TRUE;


    /*=================================================
                    Extracting Top Field
      =================================================*/
    STTBX_Print(("--------- Extracting Top Field ---------\n"));

    /* ------------ Load Src ------------ */
    SourceBitmap.Data1_p = NULL;

    Err = ConvertGammaToBitmap(TopBottomSourceFileName,0,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Top Field Height ------------ */
    SourceBitmap.Height = ( SourceBitmap.Height + 1 ) / 2;

    /* ------------ Alloc Top Field ------------*/
    TargetBitmap = SourceBitmap;

    Err = STBLIT_GetBitmapAllocParams(Handle,&SourceBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    AllocBitmapBuffer (&TargetBitmap,FALSE,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    AllocBitmapBuffer (&TargetBitmap, (void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[0],
                       &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    /* Set Field mabagment params */
    SourceBitmap.Pitch  = SourceBitmap.Pitch * 2;

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 0 ;
    Rectangle.PositionY = 0 ;
    Rectangle.Width     = SourceBitmap.Width ;
    Rectangle.Height    = SourceBitmap.Height ;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    /*Context.EnableClipRectangle     = TRUE;*/
    Context.ClipRectangle           = Rectangle;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Copy ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Copy rectangle \n"));
    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&TargetBitmap,0,0,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Wait For Evt ------------ */
#ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
    #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
    #endif

    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        ResetEngine();
    }
    else
    {
        /* Open stream */
        fstream = fopen(TopFieldFileName, "wb");

        /* Generate BinFile */
        STTBX_Print(("Saving top data ...\n"));
#ifdef  ST_OSLINUX
        size = fwrite((void *)TargetBitmap.Data1_p, 1,(TargetBitmap.Size1), fstream);
#else
        size = fwrite((void *)STAVMEM_VirtualToCPU((void *)TargetBitmap.Data1_p,&VirtualMapping), 1,(TargetBitmap.Size1), fstream);
#endif
        if (size != (TargetBitmap.Size1))
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR,"Write Bitmap error %d byte instead of %d\n",size,(TargetBitmap.Size1)));
            Err = ST_ERROR_BAD_PARAMETER;
        }

        /* Close stream */
        fclose (fstream);
    }
#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        Err = ConvertBitmapToGamma(TopFieldFileName,&TargetBitmap,&Palette);
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
#endif


    /*=================================================
                    Extracting Bottom Field
      =================================================*/

    STTBX_Print(("--------- Extracting Bottom Field ---------\n"));

    /* ------------ Load Src ------------ */
    SourceBitmap.Data1_p = NULL;

    Err = ConvertGammaToBitmap(TopBottomSourceFileName,0,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Bottom Field Height ------------ */
    SourceBitmap.Height = SourceBitmap.Height / 2;

    /* ------------ Alloc Bottom Field ------------*/
    TargetBitmap = SourceBitmap;

    Err = STBLIT_GetBitmapAllocParams(Handle,&SourceBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    AllocBitmapBuffer (&TargetBitmap,FALSE,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    AllocBitmapBuffer (&TargetBitmap, (void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[0],
                       &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    /* Set Field mabagment params */
    SourceBitmap.Data1_p  = (void*)( (U32)SourceBitmap.Data1_p + SourceBitmap.Pitch );
    SourceBitmap.Pitch  = SourceBitmap.Pitch * 2;

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 0 ;
    Rectangle.PositionY = 0 ;
    Rectangle.Width     = SourceBitmap.Width;
    Rectangle.Height    = SourceBitmap.Height;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    /*Context.EnableClipRectangle     = TRUE;*/
    Context.ClipRectangle           = Rectangle;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Copy ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Copy rectangle \n"));
    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&TargetBitmap,0,0,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Wait For Evt ------------ */
#ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
    #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
    #endif

    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        ResetEngine();
    }
    else
    {
        /* Open stream */
        fstream = fopen(BottomFieldFileName, "wb");

        /* Generate BinFile */
        STTBX_Print(("Saving Bottom data ...\n"));
#ifdef ST_OSLINUX
        size = fwrite((void *)TargetBitmap.Data1_p, 1,(TargetBitmap.Size1), fstream);
#else
        size = fwrite((void *)STAVMEM_VirtualToCPU((void *)TargetBitmap.Data1_p,&VirtualMapping), 1,(TargetBitmap.Size1), fstream);
#endif
        if (size != (TargetBitmap.Size1))
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR,"Write Bitmap error %d byte instead of %d\n",size,(TargetBitmap.Size1)));
            Err = ST_ERROR_BAD_PARAMETER;
        }

        /* Close stream */
        fclose (fstream);
    }
#else
        /*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        Err = ConvertBitmapToGamma(BottomFieldFileName,&TargetBitmap,&Palette);
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
#endif


    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}


/*-----------------------------------------------------------------------------
 * Function  : BLIT_TestTypeRasterTopBottom;
 * Input     : *pars_p, *result_sym_p
 * Output    :
 * Return    : TRUE if error, FALSE if success
 * Assuption : BLIT_TestExtractFields sould be called before this function
 *             at least one time
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestTypeRasterTopBottom (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STGXOBJ_Palette_t       Palette;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;

    STEVT_SubscriberID_t    SubscriberID;

	#ifndef STBLIT_EMULATOR
        STOS_Clock_t            time;
	#endif

#ifndef ST_OSLINUX
    STAVMEM_BlockHandle_t  BlockHandle1;
    STAVMEM_BlockHandle_t  BlockHandle2;
#endif
    STGXOBJ_BitmapAllocParams_t Params1;
    STGXOBJ_BitmapAllocParams_t Params2;
    FILE*                       fstream;        /* File handle for read operation          */
    U32                         size;
    STGXOBJ_Rectangle_t     Rectangle;



    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                       = AvmemPartitionHandle[0];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_7109) || defined (ST_5188) || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109)\
 || defined (ST_5188) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

    /* ------------ Blit Init ------------ */
#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }

#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;


    /* ------------ Set source ------------ */

    /* Source Initialisations */
    SourceBitmap.ColorType            = TopBottomFieldColorType;
    SourceBitmap.BitmapType           = TopBottomFieldBitmapType;
    SourceBitmap.PreMultipliedColor   = TopBottomFieldPreMultipliedColor;
    SourceBitmap.ColorSpaceConversion = TopBottomFieldColorSpaceConversion;
    SourceBitmap.AspectRatio          = TopBottomFieldAspectRatio;
    SourceBitmap.Width                = TopBottomFieldWidth;
    SourceBitmap.Height               = TopBottomFieldHeight;
    SourceBitmap.Pitch                = ( TopBottomFieldWidth * NbBytePerPixel );
    SourceBitmap.Size1                = ( TopBottomFieldWidth * TopBottomFieldHeight * NbBytePerPixel ) / 2;
    SourceBitmap.Size2                = SourceBitmap.Size1;
    SourceBitmap.Data1_p              = NULL;
    SourceBitmap.Data2_p              = NULL;

    /* Set alloc params */
    Params1.AllocBlockParams.Size      = SourceBitmap.Size1;
    Params1.AllocBlockParams.Alignment = TopBottomFieldAllocAlignment;
    Params1.Pitch                      = SourceBitmap.Pitch;

    /* Alloc bottom Field */
#ifdef ST_OSLINUX
    AllocBitmapBuffer (&SourceBitmap,FALSE,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    AllocBitmapBuffer (&SourceBitmap, (void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[0],
                       &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    SourceBitmap.Data2_p=SourceBitmap.Data1_p;

    /* Alloc Top Field */
#ifdef ST_OSLINUX
    AllocBitmapBuffer (&SourceBitmap,FALSE,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    AllocBitmapBuffer (&SourceBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[0],
                       &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    /* Read Top Field from file */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Reading top Bitmap data"));
    fstream = fopen(TopFieldFileName, "rb");
    if( fstream == NULL )
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR,"Unable to open \'%s\'\n", "c:/scripts/Chat_data1_p.bin" ));
        Err = ST_ERROR_BAD_PARAMETER;
    }

    size = fread ((void *)(SourceBitmap.Data1_p), 1,(SourceBitmap.Size1), fstream);
    if (size != (SourceBitmap.Size1))
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR,"Read error %d byte instead of %d\n",size,SourceBitmap.Size1));
    }
    fclose (fstream);

    /* Read Bottom Field from file */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Reading bottom Bitmap data"));
    fstream = fopen(BottomFieldFileName, "rb");
    if( fstream == NULL )
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR,"Unable to open \'%s\'\n", "c:/scripts/Chat_data1_p.bin" ));
        Err = ST_ERROR_BAD_PARAMETER;
    }

    size = fread ((void *)(SourceBitmap.Data2_p), 1,(SourceBitmap.Size2), fstream);
    if (size != (SourceBitmap.Size2))
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR,"Read error %d byte instead of %d\n",size,SourceBitmap.Size1));
    }
    fclose (fstream);


    /*=================================
             TEST COPY FUNCTION
      =================================*/
    STTBX_Print(("--------- COPY TEST ---------\n"));

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 100;
    Rectangle.PositionY = 100 ;
    Rectangle.Width     = SourceBitmap.Width - 200 ;
    Rectangle.Height    = SourceBitmap.Height - 200 ;

    /* ------------ Set Dest ------------ */
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/BATEAU.GAM",0,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../BATEAU.GAM",0,&TargetBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
    Context.EnableMaskBitmap        = FALSE;
    Context.EnableColorCorrection   = FALSE;
    Context.Trigger.EnableTrigger   = FALSE;
    Context.EnableClipRectangle     = FALSE;
    Context.ClipRectangle           = Rectangle;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Copy ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Copy rectangle\n"));
    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&TargetBitmap,40,40,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

    #ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_TypeRasterTopBottom_Copy.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_TypeRasterTopBottom_Copy.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_TypeRasterTopBottom_Copy.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_TypeRasterTopBottom_Copy.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif


    /*=================================
             TEST BLIT FUNCTION
      =================================*/
    STTBX_Print(("--------- BLEND TEST ---------\n"));

    /* ------------ Set Dest ------------ */
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/BATEAU.GAM",0,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../BATEAU.GAM",0,&TargetBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 50;
    Dst.Rectangle.PositionY   = 50;
    Dst.Rectangle.Width       = TargetBitmap.Width - 100;
    Dst.Rectangle.Height      = TargetBitmap.Height - 100;
    Dst.Palette_p             = NULL;


    /* ------------ Set Src ------------ */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = Dst.Rectangle.Width / 4;
    Src.Rectangle.PositionY  = Dst.Rectangle.Height / 4;
    Src.Rectangle.Width      = Dst.Rectangle.Width / 2;
    Src.Rectangle.Height     = Dst.Rectangle.Height / 2;
    Src.Palette_p            = &Palette;


    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_ALPHA_BLEND;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 75;
    Context.EnableClipRectangle     = /*TRUE*/FALSE;
    /*Context.ClipRectangle           = Rectangle;*/
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;


    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Bitmap Blit\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_TypeRasterTopBottom_Blit.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_TypeRasterTopBottom_Blit.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_TypeRasterTopBottom_Blit.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_TypeRasterTopBottom_Blit.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestOverlapCopy;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestOverlapCopy (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;

    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STGXOBJ_Rectangle_t     Rectangle;
    STGXOBJ_Palette_t       Palette;


    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;

    STEVT_SubscriberID_t    SubscriberID;


#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif


    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188)|| defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710)  || defined (ST_5301) || defined (ST_7109)\
 || defined (ST_7100) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif


#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;


    Context.UserTag_p   = &UserTagString;
    Context.EnableResizeFilter      = TRUE;


    /**********************************************************/
    /*  Case 1 of Overlap copy: source is up left destination */
    /**********************************************************/

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 50 ;
    Rectangle.PositionY = 40 ;
    Rectangle.Width     = 140;
    Rectangle.Height    = 120;

    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

     /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    /*Context.EnableClipRectangle     = TRUE;*/
    Context.ClipRectangle           = Rectangle;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Copy rectangle case 1 \n"));
    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&SourceBitmap,80,90,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy1.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy1.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
        /*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy1.gam",&SourceBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy1.gam",&SourceBitmap,&Palette);
    #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /************************************************************/
    /*  Case 2 of Overlap copy: source is up middle destination */
    /***********************************************************/

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 50 ;
    Rectangle.PositionY = 40 ;
    Rectangle.Width     = 140;
    Rectangle.Height    = 120;

    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Copy rectangle case 2 \n"));
    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&SourceBitmap,50,90,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy2.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy2.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
        /*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy2.gam",&SourceBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy2.gam",&SourceBitmap,&Palette);
    #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /************************************************************/
    /*  Case 3 of Overlap copy: source is up right destination */
    /***********************************************************/

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 50 ;
    Rectangle.PositionY = 40 ;
    Rectangle.Width     = 140;
    Rectangle.Height    = 120;

    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

 #ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
   if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Copy rectangle case 3 \n"));
    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&SourceBitmap,20,90,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy3.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy3.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy3.gam",&SourceBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy3.gam",&SourceBitmap,&Palette);
    #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /*************************************************************/
    /*  Case 4 of Overlap copy: source is down right destination */
    /*************************************************************/

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 80 ;
    Rectangle.PositionY = 90 ;
    Rectangle.Width     = 140;
    Rectangle.Height    = 120;

    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

 #ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
   if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Copy rectangle case 4 \n"));
    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&SourceBitmap,50,40,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy4.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy4.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy4.gam",&SourceBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy4.gam",&SourceBitmap,&Palette);
    #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /**************************************************************/
    /*  Case 5 of Overlap copy: source is down middle destination */
    /**************************************************************/

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 50 ;
    Rectangle.PositionY = 90 ;
    Rectangle.Width     = 140;
    Rectangle.Height    = 120;

    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Copy rectangle case 5 \n"));
    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&SourceBitmap,50,40,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy5.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy5.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy5.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy5.gam",&SourceBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /*************************************************************/
    /*  Case 6 of Overlap copy: source is down left destination  */
    /*************************************************************/

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 20 ;
    Rectangle.PositionY = 90 ;
    Rectangle.Width     = 140;
    Rectangle.Height    = 120;

    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

 #ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
   if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Copy rectangle case 6 \n"));
    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&SourceBitmap,50,40,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy6.gam",&SourceBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy6.gam",&SourceBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy6.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy6.gam",&SourceBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /**************************************************************/
    /*  Case 7 of Overlap copy: source is middle left destination */
    /**************************************************************/

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 20 ;
    Rectangle.PositionY = 40 ;
    Rectangle.Width     = 140;
    Rectangle.Height    = 120;

    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

 #ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
   if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Copy rectangle case 7 \n"));
    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&SourceBitmap,50,40,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy7.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy7.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy7.gam",&SourceBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy7.gam",&SourceBitmap,&Palette);
    #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /***************************************************************/
    /*  Case 8 of Overlap copy: source is middle right destination */
    /***************************************************************/

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = 90 ;
    Rectangle.PositionY = 40 ;
    Rectangle.Width     = 140;
    Rectangle.Height    = 120;

    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
  if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Copy rectangle case 8 \n"));
    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&SourceBitmap,50,40,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy8.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy8.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_OverlapCopy8.gam",&SourceBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_OverlapCopy8.gam",&SourceBitmap,&Palette);
    #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestOverlapBlit;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestOverlapBlit (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;

    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STBLIT_Destination_t    Dst;
    STBLIT_Source_t         Src2;

    STGXOBJ_Palette_t       Palette;


    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;


#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif


    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188) || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710)  || defined (ST_5301) || defined (ST_7109)\
 || defined (ST_7100) || defined (ST_5188) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif


#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    Context.UserTag_p   = &UserTagString;
    Context.EnableResizeFilter      = TRUE;


    /**********************************************************/
    /*  Case 1 of Overlap Blit: source is up left destination */
    /**********************************************************/

    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

 #ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
   if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

     /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    /*Context.EnableClipRectangle     = TRUE;*/
    /*Context.ClipRectangle           = Rectangle;*/
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Blit ------------ */
    /* Set Src2 */
    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &SourceBitmap ;
    Src2.Rectangle.PositionX  = 0;
    Src2.Rectangle.PositionY  = 0;
    Src2.Rectangle.Width      = 100;
    Src2.Rectangle.Height     = 100;
    Src2.Palette_p            = NULL;

    /* Set Dst */
    Dst.Bitmap_p              = &SourceBitmap;


    Dst.Rectangle.PositionX  = 80;
    Dst.Rectangle.PositionY  = 90;
    Dst.Rectangle.Width      = 150;
    Dst.Rectangle.Height     = 100;
    Dst.Palette_p            = NULL;
    Dst.Palette_p            = NULL;

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Blit operation case 1 \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit1.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit1.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit1.gam",&SourceBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit1.gam",&SourceBitmap,&Palette);
    #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /************************************************************/
    /*  Case 2 of Overlap Blit: source is up middle destination */
    /***********************************************************/

    /* ------------ Set Rectangle ------------ */
    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }









    /* Set Src2 */
    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &SourceBitmap ;
    Src2.Rectangle.PositionX  = 50;
    Src2.Rectangle.PositionY  = 40;
    Src2.Rectangle.Width      = 100;
    Src2.Rectangle.Height     = 50;
    Src2.Palette_p            = NULL;

    /* Set Dst */
    Dst.Bitmap_p              = &SourceBitmap;
    Dst.Rectangle.PositionX  = 50;
    Dst.Rectangle.PositionY  = 90;
    Dst.Rectangle.Width      = 100;
    Dst.Rectangle.Height     = 120;
    Dst.Palette_p            = NULL;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Blit operation case 2 \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }


	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit2.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit2.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit2.gam",&SourceBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit2.gam",&SourceBitmap,&Palette);
    #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif
    /************************************************************/
    /*  Case 3 of Overlap Blit: source is up right destination */
    /***********************************************************/

    /* ------------ Set Rectangle ------------ */
    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Err = ConvertGammaToBitmap(DstFileName,0,&TargetBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }









    /* Set Src2 */
    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &SourceBitmap ;
    Src2.Rectangle.PositionX  = 50;
    Src2.Rectangle.PositionY  = 40;
    Src2.Rectangle.Width      = 100;
    Src2.Rectangle.Height     = 120;
    Src2.Palette_p            = NULL;

    /* Set Dst */
    Dst.Bitmap_p              = &SourceBitmap;
    Dst.Rectangle.PositionX  = 20;
    Dst.Rectangle.PositionY  = 90;
    Dst.Rectangle.Width      = 80;
    Dst.Rectangle.Height     = 120;
    Dst.Palette_p            = NULL;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Blit operation case 3 \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit3.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit3.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit3.gam",&SourceBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit3.gam",&SourceBitmap,&Palette);
    #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /*************************************************************/
    /*  Case 4 of Overlap Blit: source is down right destination */
    /*************************************************************/

    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }









    /* Set Src2 */
    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &SourceBitmap ;
    Src2.Rectangle.PositionX  = 80;
    Src2.Rectangle.PositionY  = 90;
    Src2.Rectangle.Width      = 100;
    Src2.Rectangle.Height     = 120;
    Src2.Palette_p            = NULL;

    /* Set Dst */
    Dst.Bitmap_p              = &SourceBitmap;
    Dst.Rectangle.PositionX  = 50;
    Dst.Rectangle.PositionY  = 40;
    Dst.Rectangle.Width      = 120;
    Dst.Rectangle.Height     = 120;
    Dst.Palette_p            = NULL;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Blit operation case 4 \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit4.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit4.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit4.gam",&SourceBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit4.gam",&SourceBitmap,&Palette);
    #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif
    /**************************************************************/
    /*  Case 5 of Overlap Blit: source is down middle destination */
    /**************************************************************/
    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }









    /* Set Src2 */
    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &SourceBitmap ;
    Src2.Rectangle.PositionX  = 50;
    Src2.Rectangle.PositionY  = 90;
    Src2.Rectangle.Width      = 100;
    Src2.Rectangle.Height     = 100;
    Src2.Palette_p            = NULL;

    /* Set Dst */
    Dst.Bitmap_p              = &SourceBitmap;
    Dst.Rectangle.PositionX  = 50;
    Dst.Rectangle.PositionY  = 40;
    Dst.Rectangle.Width      = 100;
    Dst.Rectangle.Height     = 140;
    Dst.Palette_p            = NULL;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Blit operation case 5 \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit5.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit5.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit5.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit5.gam",&SourceBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif


    /*************************************************************/
    /*  Case 6 of Overlap Blit: source is down left destination  */
    /*************************************************************/

    /* ------------ Set Rectangle ------------ */
    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }









    /* Set Src2 */
    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &SourceBitmap ;
    Src2.Rectangle.PositionX  = 20;
    Src2.Rectangle.PositionY  = 90;
    Src2.Rectangle.Width      = 100;
    Src2.Rectangle.Height     = 80;
    Src2.Palette_p            = NULL;
    /* Set Dst */
    Dst.Bitmap_p              = &SourceBitmap;
    Dst.Rectangle.PositionX  = 50;
    Dst.Rectangle.PositionY  = 40;
    Dst.Rectangle.Width      = 200;
    Dst.Rectangle.Height     = 120;
    Dst.Palette_p            = NULL;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Blit operation case 6 \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
    #ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit6.gam",&SourceBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit6.gam",&SourceBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
        /*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit6.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit6.gam",&SourceBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /**************************************************************/
    /*  Case 7 of Overlap Blit: source is middle left destination */
    /**************************************************************/


    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
    /* Set Src2 */
    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &SourceBitmap ;
    Src2.Rectangle.PositionX  = 20;
    Src2.Rectangle.PositionY  = 40;
    Src2.Rectangle.Width      = 100;
    Src2.Rectangle.Height     = 120;
    Src2.Palette_p            = NULL;

    /* Set Dst */
    Dst.Bitmap_p              = &SourceBitmap;
    Dst.Rectangle.PositionX  = 80;
    Dst.Rectangle.PositionY  = 40;
    Dst.Rectangle.Width      = 130;
    Dst.Rectangle.Height     = 120;
    Dst.Palette_p            = NULL;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Blit operation case 7 \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
    #ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit7.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit7.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit7.gam",&SourceBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit7.gam",&SourceBitmap,&Palette);
    #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /***************************************************************/
    /*  Case 8 of Overlap Blit: source is middle right destination */
    /***************************************************************/

    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* Set Src2 */
    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &SourceBitmap ;
    Src2.Rectangle.PositionX  = 90;
    Src2.Rectangle.PositionY  = 40;
    Src2.Rectangle.Width      = 100;
    Src2.Rectangle.Height     = 120;
    Src2.Palette_p            = NULL;

    /* Set Dst */
    Dst.Bitmap_p              = &SourceBitmap;
    Dst.Rectangle.PositionX  = 50;
    Dst.Rectangle.PositionY  = 90;
    Dst.Rectangle.Width      = 120;
    Dst.Rectangle.Height     = 120;
    Dst.Palette_p            = NULL;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Blit operation case 8 \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
        {
			/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit8.gam",&SourceBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit8.gam",&SourceBitmap,&Palette);
        #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_OverlapBlit8.gam",&SourceBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_OverlapBlit8.gam",&SourceBitmap,&Palette);
    #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif


    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);


    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*----------------------------------------------------------------
 * Function : BLIT_TestDraw;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestDraw (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;

#ifdef ST_OSLINUX
    STGXOBJ_Bitmap_t        SourceBitmap;
#endif
    STGXOBJ_Bitmap_t        TargetBitmap;
    STGXOBJ_Palette_t       Palette;
    STGXOBJ_Color_t         Color;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif


    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;
    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188)|| defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif


#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /* ------------ Set Color ------------ */
    Color.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888 ;
    Color.Value.ARGB8888.Alpha = 0x50 ;
    Color.Value.ARGB8888.R     = 0xff ;
    Color.Value.ARGB8888.G     = 0x00 ;
    Color.Value.ARGB8888.B     = 0x00 ;

#ifdef ST_OSLINUX
    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif


    /* ------------ Set Dest ------------ */
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
/*    Context.ClipRectangle           = Dst.Rectangle;*/
/*    Context.WriteInsideClipRectangle = TRUE;*/
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Draw \n"));
    Err = STBLIT_DrawHLine(Handle,&TargetBitmap,20,20,(TargetBitmap.Width-40),&Color,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif
    Err = STBLIT_DrawHLine(Handle,&TargetBitmap,20,(TargetBitmap.Height-20),(TargetBitmap.Width-40),&Color,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    Err = STBLIT_DrawVLine(Handle,&TargetBitmap,20,20,(TargetBitmap.Height-40),&Color,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    Err = STBLIT_DrawVLine(Handle,&TargetBitmap,(TargetBitmap.Width-20),20,(TargetBitmap.Height-40),&Color,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_2.6.1_2.6.2_DrawVHline.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_2.6.1_2.6.2_DrawVHline.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_2.6.1_2.6.2_DrawVHline.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_2.6.1_2.6.2_DrawVHline.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestPix;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestPix (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;

#ifdef ST_OSLINUX
    STGXOBJ_Bitmap_t        SourceBitmap;
#endif

    STGXOBJ_Bitmap_t        TargetBitmap;
    STGXOBJ_Palette_t       Palette;
    STGXOBJ_Color_t       Color;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif


    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;
    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109)\
 || defined (ST_5301) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /* ------------ Set Color ------------ */
    Color.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888 ;
    Color.Value.ARGB8888.Alpha = 0x50 ;
    Color.Value.ARGB8888.R     = 0xff ;
    Color.Value.ARGB8888.G     = 0x00 ;
    Color.Value.ARGB8888.B     = 0x00 ;

#ifdef ST_OSLINUX
    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif


    /* ------------ Set Dest ------------ */
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
/*    Context.ClipRectangle           = Dst.Rectangle;*/
/*    Context.WriteInsideClipRectangle = TRUE;*/
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Set Pixel \n"));
    Err = STBLIT_SetPixel(Handle,&TargetBitmap,0,0,&Color,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
    #endif
    Err = STBLIT_SetPixel(Handle,&TargetBitmap,0,5,&Color,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif
    Err = STBLIT_SetPixel(Handle,&TargetBitmap,5,0,&Color,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif
    Err = STBLIT_SetPixel(Handle,&TargetBitmap,5,5,&Color,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
		else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_2.6.3_setpix.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_2.6.3_setpix.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_2.6.3_setpix.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_2.6.3_setpix.gam",&TargetBitmap,&Palette);
        #endif
	    if (Err != ST_NO_ERROR)
		{
	 	    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
	 	    return (TRUE);
		}
	#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestColorKey;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestColorKey (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STGXOBJ_Palette_t       Palette;


    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;


#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif


    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined(ST_7109) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);


    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/suzie.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../green.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;

    TargetBitmap.Data1_p = NULL;
    /* ------------ Set Dest ------------ */
#ifdef ST_OSLINUX
#if defined(ST_7015) || defined(ST_7020) || defined(ST_GX1) || defined(ST_5528) || defined (ST_7710) || defined (ST_7100)
    Err = ConvertGammaToBitmap("./blt/files/merouARGB8888.gam",SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#endif
#else /* !ST_OSLINUX */
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif /* ST_OSLINUX */

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Palette_p             = NULL;

    if ( SourceBitmap.Width >= TargetBitmap.Width)
    {
        Src.Rectangle.Width = TargetBitmap.Width;
        Dst.Rectangle.Width = TargetBitmap.Width;
    }
    else
    {
        Dst.Rectangle.Width = SourceBitmap.Width;
    }
    if ( SourceBitmap.Height >= TargetBitmap.Height)
    {
        Src.Rectangle.Height = TargetBitmap.Height;
        Dst.Rectangle.Height = TargetBitmap.Height;
    }
    else
    {
        Dst.Rectangle.Height = SourceBitmap.Height;
    }


    /* ------------ Set Context ------------ */
    Context.ColorKey.Type           = STGXOBJ_COLOR_KEY_TYPE_RGB565;
    Context.ColorKey.Value.RGB888.RMin    = 0x0;
    Context.ColorKey.Value.RGB888.RMax    = 0x10;
    Context.ColorKey.Value.RGB888.ROut    = 1;
    Context.ColorKey.Value.RGB888.REnable = 1;
    Context.ColorKey.Value.RGB888.GMin    = 0x20;
    Context.ColorKey.Value.RGB888.GMax    = 0x30;
    Context.ColorKey.Value.RGB888.GOut    = 1;
    Context.ColorKey.Value.RGB888.GEnable = 1;
    Context.ColorKey.Value.RGB888.BMin    = 0x0;
    Context.ColorKey.Value.RGB888.BMax    = 0x10;
    Context.ColorKey.Value.RGB888.BOut    = 1;
    Context.ColorKey.Value.RGB888.BEnable = 1;
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = /*TRUE*/FALSE;
/*    Context.ClipRectangle           = Dst.Rectangle;*/
    Context.WriteInsideClipRectangle = FALSE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /*=======================================================================*/
    /* Test1 : Test ColorKey mode source                                     */
    /*=======================================================================*/

    /* ------------ Set color key mode ------------ */
     Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_SRC;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start ColorKey mode source\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
		else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_ColorKey_source.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_ColorKey_source.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_ColorKey_source.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_ColorKey_source.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /*=======================================================================*/
    /* Test2 : Test ColorKey mode destination                                */
    /*=======================================================================*/

    /* ------------ Set Dest ------------ */
#ifdef ST_OSLINUX
#if defined(ST_7015) || defined(ST_7020) || defined(ST_GX1) || defined(ST_5528) || defined (ST_7710) || defined (ST_7100)
    Err = ConvertGammaToBitmap("./blt/files/merouARGB8888.gam",SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#endif
#else /* !ST_OSLINUX */
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif /* ST_OSLINUX */


    /* ------------ Set color key mode ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_DST;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start ColorKey mode destination\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
		else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_ColorKey_destination.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_ColorKey_destination.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_ColorKey_destination.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_ColorKey_destination.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif



    /* --------------- Free Bitmap ------------ */

    GUTIL_Free(SourceBitmap.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestALU;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestALU (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src1,Src2;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Bitmap_t        Source1Bitmap,Source2Bitmap, TargetBitmap;
    STGXOBJ_Palette_t       Palette;

#ifndef ST_OSLINUX
    STAVMEM_BlockHandle_t  BlockHandle1;
    STAVMEM_BlockHandle_t  BlockHandle2;
    STAVMEM_FreeBlockParams_t FreeBlockParams;
#endif
    STGXOBJ_BitmapAllocParams_t Params1;
    STGXOBJ_BitmapAllocParams_t Params2;


    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;


#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined(ST_7109) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif

/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);


    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /* ------------ Set source 1 ------------ */
    Source1Bitmap.Data1_p = NULL;
    Source2Bitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
    TargetBitmap.Data1_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/Logical_zero.gam",SECURED_SRC_BITMAP,&Source1Bitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../Logical_zero.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&Source1Bitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src1.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src1.Data.Bitmap_p        = &Source1Bitmap;
    Src1.Rectangle.PositionX  = 0;
    Src1.Rectangle.PositionY  = 0;
    Src1.Palette_p            = &Palette;

    /* ------------ Set src 2 ------------ */
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/Logical_one.gam",SECURED_SRC_BITMAP,&Source2Bitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../Logical_one.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&Source2Bitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &Source2Bitmap;
    Src2.Rectangle.PositionX  = 0;
    Src2.Rectangle.PositionY  = 0;
    Src2.Palette_p            = &Palette;

    if ( Source1Bitmap.Width >= Source2Bitmap.Width )
    {
        Src1.Rectangle.Width = Source2Bitmap.Width;
        Src2.Rectangle.Width = Source2Bitmap.Width;
    }
    else
    {
        Src1.Rectangle.Width = Source1Bitmap.Width;
        Src2.Rectangle.Width = Source1Bitmap.Width;
    }

    if ( Source1Bitmap.Height >= Source2Bitmap.Height )
    {
        Src1.Rectangle.Height = Source2Bitmap.Height;
        Src2.Rectangle.Height = Source2Bitmap.Height;
    }
    else
    {
        Src1.Rectangle.Height = Source1Bitmap.Height;
        Src2.Rectangle.Height = Source1Bitmap.Height;
    }

    /* ------------ Set Dst ------------ */

    TargetBitmap.ColorType              = Source1Bitmap.ColorType;
    TargetBitmap.BitmapType             = Source1Bitmap.BitmapType;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT709;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = Src1.Rectangle.Width;
    TargetBitmap.Height                 = Src1.Rectangle.Height;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    Err = AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                             &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err TargetBitmap Allocation : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;
/*    Rectangle                 = Dst.Rectangle;*/



    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = /*TRUE*/FALSE;
/*    Context.ClipRectangle           = Rectangle;*/
    Context.WriteInsideClipRectangle = /*TRUE*/FALSE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;


    /*=======================================================================*/
    /* Test1 : Test ALU "AND"                                                */
    /*=======================================================================*/

    /* ------------ Set AluMode ------------ */
    Context.AluMode                 = STBLIT_ALU_AND;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Test ALU : S1(0) AND S2(1)\n"));
    Err = STBLIT_Blit(Handle,&Src1,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
		else
		{
            /* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_ALU_S1_AND_S2.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_ALU_S1_AND_S2.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		 /* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_ALU_S1_AND_S2.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_ALU_S1_AND_S2.gam",&TargetBitmap,&Palette);
         #endif
		 if (Err != ST_NO_ERROR)
		 {
			 STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			 return (TRUE);
		 }
	#endif

    /*=======================================================================*/
    /* Test2: Test ALU "OR"                                                  */
    /*=======================================================================*/

    /* ------------ Set AluMode ------------ */
    Context.AluMode                 = STBLIT_ALU_OR;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Test ALU : S1(0) OR S2(1)\n"));
    Err = STBLIT_Blit(Handle,&Src1,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
		else
		{
            /* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_ALU_S1_OR_S2.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_ALU_S1_OR_S2.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		 /* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_ALU_S1_OR_S2.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_ALU_S1_OR_S2.gam",&TargetBitmap,&Palette);
         #endif
		 if (Err != ST_NO_ERROR)
		 {
			 STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			 return (TRUE);
		 }
	#endif

    /*=======================================================================*/
    /* Test3: Test ALU "NAND"                                                */
    /*=======================================================================*/

    /* ------------ Set AluMode ------------ */
    Context.AluMode                 = STBLIT_ALU_NAND;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Test ALU : S1(0) NAND S2(1)\n"));
    Err = STBLIT_Blit(Handle,&Src1,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
		else
		{
            /* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_ALU_S1_NAND_S2.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_ALU_S1_NAND_S2.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		 /* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_ALU_S1_NAND_S2.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_ALU_S1_NAND_S2.gam",&TargetBitmap,&Palette);
         #endif
		 if (Err != ST_NO_ERROR)
		 {
			 STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			 return (TRUE);
		 }
	#endif

    /*=======================================================================*/
    /* Test4: Test ALU "NOOP"                                                */
    /*=======================================================================*/

    /* ------------ Set AluMode ------------ */
    Context.AluMode                 = STBLIT_ALU_NOOP;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Test ALU : NOOP, Result = S1(0)\n"));
    Err = STBLIT_Blit(Handle,&Src1,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
		else
		{
            /* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_ALU_NOOP.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_ALU_NOOP.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		 /* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_ALU_NOOP.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_ALU_NOOP.gam",&TargetBitmap,&Palette);
         #endif
		 if (Err != ST_NO_ERROR)
		 {
			 STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			 return (TRUE);
		 }
	#endif


    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(Source1Bitmap.Data1_p);
    GUTIL_Free(Source2Bitmap.Data1_p);



#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
    /* Free TargetBitmap */
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[0];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestMaskWord;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestMaskWord (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Bitmap_t        SourceBitmap,TargetBitmap1,TargetBitmap2,TargetBitmap3;
    STGXOBJ_Palette_t       Palette;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;


#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined(ST_7109) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p  = NULL;
    TargetBitmap1.Data1_p = NULL;
    TargetBitmap2.Data1_p = NULL;
    TargetBitmap3.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/suzie.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../suzie.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;

    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;


    /* ------------ Set Dest ------------ */
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_SRC_BITMAP,&TargetBitmap1,&Palette);
#else
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap1,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_DST_BITMAP,&TargetBitmap2,&Palette);
#else
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap2,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_DST_BITMAP,&TargetBitmap3,&Palette);
#else
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap3,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap1;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap1.Width;
    Dst.Rectangle.Height      = TargetBitmap1.Height;
    Dst.Palette_p             = NULL;

    if ( TargetBitmap1.Width >= SourceBitmap.Width )
    {
        Src.Rectangle.Width = SourceBitmap.Width;
        Dst.Rectangle.Width = SourceBitmap.Width;
    }
    else
    {
        Src.Rectangle.Width = TargetBitmap1.Width;
        Dst.Rectangle.Width = TargetBitmap1.Width;
    }

    if ( TargetBitmap1.Height >= SourceBitmap.Height )
    {
        Src.Rectangle.Height = SourceBitmap.Height;
        Dst.Rectangle.Height = SourceBitmap.Height;
    }
    else
    {
        Src.Rectangle.Height = TargetBitmap1.Height;
        Dst.Rectangle.Height = TargetBitmap1.Height;
    }

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = TRUE;
    Context.MaskWord                = 0xF800;
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
/*    Context.ClipRectangle           = Dst.Rectangle;*/
    Context.WriteInsideClipRectangle = FALSE/*TRUE*/;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Mask Word \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
		else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_MaskWord_red.gam",&TargetBitmap1,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_MaskWord_red.gam",&TargetBitmap1,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_MaskWord_red.gam",&TargetBitmap1,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_MaskWord_red.gam",&TargetBitmap1,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    Dst.Bitmap_p              = &TargetBitmap2;
    Context.MaskWord          = 0x7E0;

    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
		else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_MaskWord_green.gam",&TargetBitmap2,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_MaskWord_green.gam",&TargetBitmap2,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

        /* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_MaskWord_green.gam",&TargetBitmap2,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_MaskWord_green.gam",&TargetBitmap2,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
	#endif
    Context.MaskWord                = 0x001F;
    Dst.Bitmap_p              = &TargetBitmap3;


    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
		else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_MaskWord_blue.gam",&TargetBitmap3,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_MaskWord_blue.gam",&TargetBitmap3,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_MaskWord_blue.gam",&TargetBitmap3,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_MaskWord_blue.gam",&TargetBitmap3,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }

	#endif


    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);
    GUTIL_Free(TargetBitmap1.Data1_p);
    GUTIL_Free(TargetBitmap2.Data1_p);
    GUTIL_Free(TargetBitmap3.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}


/*-----------------------------------------------------------------------------
 * Function : BLIT_TestMaskBitmap;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestMaskBitmap (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Foreground;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Bitmap_t        ForegroundBitmap,MaskBitmap,TargetBitmap;
    STGXOBJ_Palette_t       Palette;


#ifdef ST_OSLINUX
    STLAYER_AllocDataParams_t    AllocDataParams;
#else
    STAVMEM_BlockHandle_t  Work_Handle;
    STAVMEM_AllocBlockParams_t  AllocBlockParams; /* Allocation param*/
    STAVMEM_MemoryRange_t  RangeArea[2];
    STAVMEM_FreeBlockParams_t FreeBlockParams;
#endif

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;


#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U8                      NbForbiddenRange;

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[0];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
    InitParams.SingleBlitNodeMaxNumber              = 30;
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_7710) || defined (ST_7100) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /* ------------ Set Foreground and Mask------------ */
    ForegroundBitmap.Data1_p = NULL;
    MaskBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX

#if defined(ST_7015) || defined(ST_7020) || defined(ST_GX1) || defined(ST_5528) || defined (ST_7710) || defined (ST_7100)
    Err = ConvertGammaToBitmap("./blt/files/merouARGB8888.gam",0,&ForegroundBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",0,&ForegroundBitmap,&Palette);
#endif

#else

#if defined(ST_7015) || defined(ST_7020) || defined(ST_GX1) || defined(ST_5528) || defined (ST_7710) || defined (ST_7100)
    Err = ConvertGammaToBitmap("../../merouargb8888.gam",0,&ForegroundBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",0,&ForegroundBitmap,&Palette);
#endif

#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/mmask.gam",0,&MaskBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../mmask.gam",0,&MaskBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Foreground.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Foreground.Data.Bitmap_p        = &ForegroundBitmap;
    Foreground.Rectangle.PositionX  = 0;
    Foreground.Rectangle.PositionY  = 0;
    Foreground.Rectangle.Width      = MaskBitmap.Width;
    Foreground.Rectangle.Height     = MaskBitmap.Height;
    Foreground.Palette_p            = &Palette;

    /* ------------ Set Dst ------------ */
#ifdef ST_OSLINUX

#if defined(ST_7015) || defined(ST_7020) || defined(ST_GX1) || defined(ST_5528) || defined (ST_7710) || defined (ST_7100)
    Err = ConvertGammaToBitmap("./blt/files/BATEAU.GAM",0,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("./blt/files/suzie.gam",0,&TargetBitmap,&Palette);
#endif

#else

#if defined(ST_7015) || defined(ST_7020) || defined(ST_GX1) || defined(ST_5528) || defined (ST_7710) || defined (ST_7100)
    Err = ConvertGammaToBitmap("../../crow.gam",0,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../suzie.gam",0,&TargetBitmap,&Palette);
#endif

#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle             = Foreground.Rectangle;
    Dst.Palette_p             = NULL;

#ifdef ST_OSLINUX

    AllocDataParams.Alignment = 16;
    AllocDataParams.Size = ForegroundBitmap.Size1;

    Err = STLAYER_AllocData( FakeLayerHndl, &AllocDataParams, (void**)&(Context.WorkBuffer_p) );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }

#else
    /* ------------- Allocation Work buffer ---------------*/
    NbForbiddenRange = 1;
    if (VirtualMapping.VirtualWindowOffset > 0)
    {
        RangeArea[0].StartAddr_p = (void *) VirtualMapping.VirtualBaseAddress_p;
        RangeArea[0].StopAddr_p  = (void *) ((U32)(RangeArea[0].StartAddr_p) +
                                             (U32)(VirtualMapping.VirtualWindowOffset) - 1);
    }
    else /*  VirtualWindowOffset = 0 */
    {
        RangeArea[0].StartAddr_p = (void *) VirtualMapping.VirtualBaseAddress_p;
        RangeArea[0].StopAddr_p  = (void *) RangeArea[0].StartAddr_p;
    }

    if ((VirtualMapping.VirtualWindowOffset + VirtualMapping.VirtualWindowSize) !=
         VirtualMapping.VirtualSize)
    {
        RangeArea[1].StartAddr_p = (void *) ((U32)(RangeArea[0].StartAddr_p) +
                                             (U32)(VirtualMapping.VirtualWindowOffset) +
                                             (U32)(VirtualMapping.VirtualWindowSize));
        RangeArea[1].StopAddr_p  = (void *) ((U32)(VirtualMapping.VirtualBaseAddress_p) +
                                                  (U32)(VirtualMapping.VirtualSize) - 1);

        NbForbiddenRange= 2;
    }
    AllocBlockParams.PartitionHandle            = AvmemPartitionHandle[0];
    AllocBlockParams.AllocMode                  = STAVMEM_ALLOC_MODE_BOTTOM_TOP;
    AllocBlockParams.NumberOfForbiddenRanges  = NbForbiddenRange;
    AllocBlockParams.ForbiddenRangeArray_p    = &RangeArea[0];
    AllocBlockParams.NumberOfForbiddenBorders   = 0;
    AllocBlockParams.ForbiddenBorderArray_p     = NULL;
    AllocBlockParams.Alignment = 16;

    AllocBlockParams.Size = ForegroundBitmap.Size1;

    Err = STAVMEM_AllocBlock(&AllocBlockParams,&Work_Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
    STAVMEM_GetBlockAddress(Work_Handle,(void**)&(Context.WorkBuffer_p));
#endif

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;

    Context.EnableMaskBitmap        = TRUE;
    Context.MaskBitmap_p            = &MaskBitmap;
    Context.MaskRectangle           = Foreground.Rectangle;

    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
/*    Context.ClipRectangle           = Dst.Rectangle;*/
/*    Context.WriteInsideClipRectangle = TRUE;*/
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Mask Bitmap \n"));
    Err = STBLIT_Blit(Handle,NULL,&Foreground,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
/*        else*/
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_Mask_bitmap.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_Mask_bitmap.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Mask_bitmap.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Mask_bitmap.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
	#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(MaskBitmap.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);
    GUTIL_Free(ForegroundBitmap.Data1_p);

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,Context.WorkBuffer_p );
#else
    /* Free Work buffer */
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[0];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&Work_Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestMaskBitmapFill;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestMaskBitmapFill (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Foreground;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Bitmap_t        ForegroundBitmap,MaskBitmap,TargetBitmap;
    STGXOBJ_Palette_t       Palette;

#ifdef ST_OSLINUX
    STLAYER_AllocDataParams_t    AllocDataParams;
#else
    STAVMEM_BlockHandle_t  Work_Handle;
    STAVMEM_AllocBlockParams_t  AllocBlockParams; /* Allocation param*/
    STAVMEM_MemoryRange_t  RangeArea[2];
    STAVMEM_FreeBlockParams_t FreeBlockParams;
#endif

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;


#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U8                      NbForbiddenRange;
    STGXOBJ_Color_t         Color;
    STGXOBJ_Rectangle_t     Rectangle;
    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[0];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
    InitParams.SingleBlitNodeMaxNumber              = 30;
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_7710) || defined (ST_7100) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /* ------------ Set Foreground and Mask------------ */
    ForegroundBitmap.Data1_p = NULL;
    MaskBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX

#if defined(ST_7015) || defined(ST_7020) || defined(ST_GX1) || defined(ST_5528) || defined (ST_7710) || defined (ST_7100)
    Err = ConvertGammaToBitmap("./blt/files/merouARGB8888.gam",0,&ForegroundBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",0,&ForegroundBitmap,&Palette);
#endif

#else

#if defined(ST_7015) || defined(ST_7020) || defined(ST_GX1) || defined(ST_5528) || defined (ST_7710) || defined (ST_7100)
    Err = ConvertGammaToBitmap("../../merouargb8888.gam",0,&ForegroundBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",0,&ForegroundBitmap,&Palette);
#endif

#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }


#ifdef ST_OSLINUX
    ConvertGammaToBitmap("./blt/files/MASK6.gam",0,&MaskBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../mask6.gam",0,&MaskBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Color.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888 ;
    Color.Value.ARGB8888.Alpha = 128 ;
    Color.Value.ARGB8888.R     = 0x00 ;
    Color.Value.ARGB8888.G     = 0x00 ;
    Color.Value.ARGB8888.B     = 0xFF ;


    Foreground.Type                 = STBLIT_SOURCE_TYPE_COLOR;
    Foreground.Data.Color_p        = &Color;


    Rectangle.PositionX  = 0;
    Rectangle.PositionY  = 0;
    Rectangle.Width      = MaskBitmap.Width;
    Rectangle.Height     = MaskBitmap.Height;

    /* ------------ Set Dst ------------ */
#ifdef ST_OSLINUX

#if defined(ST_7015) || defined(ST_7020) || defined(ST_GX1) || defined(ST_5528) || defined (ST_7710) || defined (ST_7100)
    Err = ConvertGammaToBitmap("./blt/files/crow.gam",0,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("./blt/file/suzie.gam",0,&TargetBitmap,&Palette);
#endif

#else

#if defined(ST_7015) || defined(ST_7020) || defined(ST_GX1) || defined(ST_5528) || defined (ST_7710) || defined (ST_7100)
    Err = ConvertGammaToBitmap("../../crow.gam",0,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../suzie.gam",0,&TargetBitmap,&Palette);
#endif


#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle             = Rectangle;
    Dst.Palette_p             = NULL;

#ifdef ST_OSLINUX

    AllocDataParams.Alignment = 16;

#ifdef FILL_METHOD
    AllocDataParams.Size = 3000;  /* More than 256 * 4 for palette */
#else
    AllocDataParams.Size = ForegroundBitmap.Size1;
#endif

    Err = STLAYER_AllocData( FakeLayerHndl, &AllocDataParams, (void**)&(Context.WorkBuffer_p) );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }


#else

    /* ------------- Allocation Work buffer ---------------*/
    NbForbiddenRange = 1;
    if (VirtualMapping.VirtualWindowOffset > 0)
    {
        RangeArea[0].StartAddr_p = (void *) VirtualMapping.VirtualBaseAddress_p;
        RangeArea[0].StopAddr_p  = (void *) ((U32)(RangeArea[0].StartAddr_p) +
                                             (U32)(VirtualMapping.VirtualWindowOffset) - 1);
    }
    else /*  VirtualWindowOffset = 0 */
    {
        RangeArea[0].StartAddr_p = (void *) VirtualMapping.VirtualBaseAddress_p;
        RangeArea[0].StopAddr_p  = (void *) RangeArea[0].StartAddr_p;
    }

    if ((VirtualMapping.VirtualWindowOffset + VirtualMapping.VirtualWindowSize) !=
         VirtualMapping.VirtualSize)
    {
        RangeArea[1].StartAddr_p = (void *) ((U32)(RangeArea[0].StartAddr_p) +
                                             (U32)(VirtualMapping.VirtualWindowOffset) +
                                             (U32)(VirtualMapping.VirtualWindowSize));
        RangeArea[1].StopAddr_p  = (void *) ((U32)(VirtualMapping.VirtualBaseAddress_p) +
                                                  (U32)(VirtualMapping.VirtualSize) - 1);

        NbForbiddenRange= 2;
    }
    AllocBlockParams.PartitionHandle            = AvmemPartitionHandle[0];
    AllocBlockParams.AllocMode                  = STAVMEM_ALLOC_MODE_BOTTOM_TOP;
    AllocBlockParams.NumberOfForbiddenRanges    = NbForbiddenRange;
    AllocBlockParams.ForbiddenRangeArray_p      = &RangeArea[0];
    AllocBlockParams.NumberOfForbiddenBorders   = 0;
    AllocBlockParams.ForbiddenBorderArray_p     = NULL;
    AllocBlockParams.Alignment                  = 16;

#ifdef FILL_METHOD
    AllocBlockParams.Size = 3000;  /* More than 256 * 4 for palette */
#else
    AllocBlockParams.Size = ForegroundBitmap.Size1;
#endif
    Err = STAVMEM_AllocBlock(&AllocBlockParams,&Work_Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
    STAVMEM_GetBlockAddress(Work_Handle,(void**)&(Context.WorkBuffer_p));
#endif

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;

    Context.EnableMaskBitmap        = TRUE;
    Context.MaskBitmap_p            = &MaskBitmap;
    Context.MaskRectangle           = Rectangle;

    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
/*    Context.ClipRectangle           = Dst.Rectangle;*/
/*    Context.WriteInsideClipRectangle = TRUE;*/
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;



    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Mask Bitmap Fill \n"));
#ifdef FILL_METHOD
    Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&Rectangle,&Color,&Context );
#else
    Err = STBLIT_Blit(Handle,NULL,&Foreground,&Dst,&Context );
#endif

    if (Err != ST_NO_ERROR)
    {
        #ifdef FILL_METHOD
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Fill : %d\n",Err));
        #else
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        #endif
        return (TRUE);
    }
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
		else
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_Mask_bitmapFill.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_Mask_bitmapFill.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
			{
				STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
				return (TRUE);
			}
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Mask_bitmapFill.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Mask_bitmapFill.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
	#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(MaskBitmap.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);
    GUTIL_Free(ForegroundBitmap.Data1_p);

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,Context.WorkBuffer_p );
#else
    /* Free Work buffer */
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[0];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&Work_Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestClip;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestClip (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap, TargetBitmap2;
#ifdef ST_OSLINUX
    STGXOBJ_Bitmap_t        TargetBitmap3;
#endif
    STGXOBJ_Palette_t       Palette;
    STGXOBJ_Rectangle_t     Rectangle;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188)|| defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    TargetBitmap2.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 40;
    Src.Rectangle.PositionY  = 30;
    Src.Rectangle.Width      = 200;/*SourceBitmap.Width;*/
    Src.Rectangle.Height     = 200;/*SourceBitmap.Height; */
    Src.Palette_p            = &Palette;

    /* ------------ Set Dest ------------ */
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(DstFileName,SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(DstFileName,DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_DST_BITMAP,&TargetBitmap2,&Palette);
#else
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap2,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }


    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 20;
    Dst.Rectangle.PositionY   = 20;
    Dst.Rectangle.Width       = 200;
    Dst.Rectangle.Height      = 200;
    Dst.Palette_p             = NULL;

    /* ------------ Set Clip Rectangle ------------ */
    Rectangle.PositionX = 50 ;
    Rectangle.PositionY = 50 ;
    Rectangle.Width     = 140 ;
    Rectangle.Height    = 140 ;


    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
    Context.EnableMaskBitmap        = FALSE;
    Context.EnableColorCorrection   = FALSE;
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = TRUE;
    Context.ClipRectangle           = Rectangle;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Clipping : Write inside \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
 #ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
    #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
    #endif

    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        ResetEngine();
    }
    else
    {
        /* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Clipping_inside.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Clipping_inside.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
    }
#else
    /*  Wait for Blit to be completed */
    STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

    /* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Clipping_inside.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Clipping_inside.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif

/*    TargetBitmap2.Data1_p=NULL;
    Err = ConvertGammaToBitmap(DstFileName,&TargetBitmap2,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }*/

    Dst.Bitmap_p              = &TargetBitmap2;
    Context.WriteInsideClipRectangle = FALSE;

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Clipping : Write outside \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
#ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
    #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
    #endif

    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        ResetEngine();
    }
    else
    {
        /* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Clipping_outside.gam",&TargetBitmap2,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Clipping_outside.gam",&TargetBitmap2,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
    }
#else
    /*  Wait for Blit to be completed */
    STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

    /* Generate Bitmap */
    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_Clipping_outside.gam",&TargetBitmap2,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_Clipping_outside.gam",&TargetBitmap2,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif


    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);
    GUTIL_Free(TargetBitmap2.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestColor;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestColor (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STGXOBJ_Palette_t       Palette;

#ifndef ST_OSLINUX
    STAVMEM_BlockHandle_t  BlockHandle1;
    STAVMEM_BlockHandle_t  BlockHandle2;
#endif
    STGXOBJ_BitmapAllocParams_t Params1;
    STGXOBJ_BitmapAllocParams_t Params2;
    STAVMEM_FreeBlockParams_t FreeBlockParams;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif

#if !defined(ST_5100) && !defined(ST_5105) && !defined(ST_5301) && !defined(ST_5188) && !defined (ST_5525) && !defined(ST_5107)

#ifdef ST_OSLINUX
    STLAYER_AllocDataParams_t    AllocDataParams;
#else
    U8                      NbForbiddenRange;
    STAVMEM_BlockHandle_t  PaletteHandle;
    STAVMEM_AllocBlockParams_t  AllocBlockParams; /* Allocation param*/
    STAVMEM_MemoryRange_t  RangeArea[2];
#endif

                            /* A R G B  */
    U32 DataPalette[256]={  0xFF000000, 0xFF000033, 0xFF000066, 0xFF000099, 0xFF0000CC, 0xFF0000FF,
                            0xFF003300, 0xFF003333, 0xFF003366, 0xFF003399, 0xFF0033CC, 0xFF0033FF,
                            0xFF006600, 0xFF006633, 0xFF006666, 0xFF006699, 0xFF0066CC, 0xFF0066FF,
                            0xFF009900, 0xFF009933, 0xFF009966, 0xFF009999, 0xFF0099CC, 0xFF0099FF,
                            0xFF00CC00, 0xFF00CC33, 0xFF00CC66, 0xFF00CC99, 0xFF00CCCC, 0xFF00CCFF,
                            0xFF00FF00, 0xFF00FF33, 0xFF00FF66, 0xFF00FF99, 0xFF00FFCC, 0xFF00FFFF,

                            0xFF330000, 0xFF330033, 0xFF330066, 0xFF330099, 0xFF3300CC, 0xFF3300FF,
                            0xFF333300, 0xFF333333, 0xFF333366, 0xFF333399, 0xFF3333CC, 0xFF3333FF,
                            0xFF336600, 0xFF336633, 0xFF336666, 0xFF336699, 0xFF3366CC, 0xFF3366FF,
                            0xFF339900, 0xFF339933, 0xFF339966, 0xFF339999, 0xFF3399CC, 0xFF3399FF,
                            0xFF33CC00, 0xFF33CC33, 0xFF33CC66, 0xFF33CC99, 0xFF33CCCC, 0xFF33CCFF,
                            0xFF33FF00, 0xFF33FF33, 0xFF33FF66, 0xFF33FF99, 0xFF33FFCC, 0xFF33FFFF,

                            0xFF660000, 0xFF660033, 0xFF660066, 0xFF660099, 0xFF6600CC, 0xFF6600FF,
                            0xFF663300, 0xFF663333, 0xFF663366, 0xFF663399, 0xFF6633CC, 0xFF6633FF,
                            0xFF666600, 0xFF666633, 0xFF666666, 0xFF666699, 0xFF6666CC, 0xFF6666FF,
                            0xFF669900, 0xFF669933, 0xFF669966, 0xFF669999, 0xFF6699CC, 0xFF6699FF,
                            0xFF66CC00, 0xFF66CC33, 0xFF66CC66, 0xFF66CC99, 0xFF66CCCC, 0xFF66CCFF,
                            0xFF66FF00, 0xFF66FF33, 0xFF66FF66, 0xFF66FF99, 0xFF66FFCC, 0xFF66FFFF,

                            0xFF990000, 0xFF990033, 0xFF990066, 0xFF990099, 0xFF9900CC, 0xFF9900FF,
                            0xFF993300, 0xFF993333, 0xFF993366, 0xFF993399, 0xFF9933CC, 0xFF9933FF,
                            0xFF996600, 0xFF996633, 0xFF996666, 0xFF996699, 0xFF9966CC, 0xFF9966FF,
                            0xFF999900, 0xFF999933, 0xFF999966, 0xFF999999, 0xFF9999CC, 0xFF9999FF,
                            0xFF99CC00, 0xFF99CC33, 0xFF99CC66, 0xFF99CC99, 0xFF99CCCC, 0xFF99CCFF,
                            0xFF99FF00, 0xFF99FF33, 0xFF99FF66, 0xFF99FF99, 0xFF99FFCC, 0xFF99FFFF,

                            0xFFCC0000, 0xFFCC0033, 0xFFCC0066, 0xFFCC0099, 0xFFCC00CC, 0xFFCC00FF,
                            0xFFCC3300, 0xFFCC3333, 0xFFCC3366, 0xFFCC3399, 0xFFCC33CC, 0xFFCC33FF,
                            0xFFCC6600, 0xFFCC6633, 0xFFCC6666, 0xFFCC6699, 0xFFCC66CC, 0xFFCC66FF,
                            0xFFCC9900, 0xFFCC9933, 0xFFCC9966, 0xFFCC9999, 0xFFCC99CC, 0xFFCC99FF,
                            0xFFCCCC00, 0xFFCCCC33, 0xFFCCCC66, 0xFFCCCC99, 0xFFCCCCCC, 0xFFCCCCFF,
                            0xFFCCFF00, 0xFFCCFF33, 0xFFCCFF66, 0xFFCCFF99, 0xFFCCFFCC, 0xFFCCFFFF,

                            0xFFFF0000, 0xFFFF0033, 0xFFFF0066, 0xFFFF0099, 0xFFFF00CC, 0xFFFF00FF,
                            0xFFFF3300, 0xFFFF3333, 0xFFFF3366, 0xFFFF3399, 0xFFFF33CC, 0xFFFF33FF,
                            0xFFFF6600, 0xFFFF6633, 0xFFFF6666, 0xFFFF6699, 0xFFFF66CC, 0xFFFF66FF,
                            0xFFFF9900, 0xFFFF9933, 0xFFFF9966, 0xFFFF9999, 0xFFFF99CC, 0xFFFF99FF,
                            0xFFFFCC00, 0xFFFFCC33, 0xFFFFCC66, 0xFFFFCC99, 0xFFFFCCCC, 0xFFFFCCFF,
                            0xFFFFFF00, 0xFFFFFF33, 0xFFFFFF66, 0xFFFFFF99, 0xFFFFFFCC, 0xFFFFFFFF};
#endif


    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined(ST_5188) || defined(ST_5525)\
 || defined (ST_5107)  || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;

    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_ARGB8888;
    TargetBitmap.BitmapType             = SourceBitmap.BitmapType;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT709;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    Err = AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                             &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
    Context.EnableMaskBitmap        = FALSE;
    Context.EnableColorCorrection   = FALSE;
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Conversion RGB565 -> ARGB8888\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
#ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
    #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
    #endif

    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        ResetEngine();
    }
    else
    {
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Conversion_RGB565_to_ARGB8888.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Conversion_RGB565_to_ARGB8888.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
    }
#else
    /*  Wait for Blit to be completed */
    STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_Conversion_RGB565_to_ARGB8888.gam"&TargetBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_Conversion_RGB565_to_ARGB8888.gam",&TargetBitmap,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif

    /* --------------- Free Bitmap ------------ */
#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
    /* Free TargetBitmap */
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    TargetBitmap.Data1_p=NULL;

    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_UNSIGNED_YCBCR888_422;
    TargetBitmap.BitmapType             = SourceBitmap.BitmapType;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT709;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    Err = AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                             &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Conversion RGB565 -> YCbCr422R\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
#ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
    #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
    #endif

    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        ResetEngine();
    }
    else
    {
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Conversion_RGB565_to_YCbCr422R.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Conversion_RGB565_to_YCbCr422R.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
    }
#else
    /*  Wait for Blit to be completed */
    STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_Conversion_RGB565_to_YCbCr422R.gam",&TargetBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_Conversion_RGB565_to_YCbCr422R.gam",&TargetBitmap,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif

#if !defined (ST_5100) &&  !defined (ST_5105) &&  !defined (ST_5301) &&  !defined (ST_7109) &&  !defined (ST_5188)\
&&  !defined (ST_5525) &&  !defined (ST_5107) && !defined (ST_7200)
    /* --------------- Free Bitmap ------------ */

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
    /* Free TargetBitmap */
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
    TargetBitmap.Data1_p=NULL;
#endif

    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_CLUT8;
    TargetBitmap.BitmapType             = SourceBitmap.BitmapType;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT709;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;

    Palette.PaletteType = STGXOBJ_PALETTE_TYPE_DEVICE_INDEPENDENT;
    Palette.ColorType   = STGXOBJ_COLOR_TYPE_ARGB8888;
    Palette.ColorDepth  = 8;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX

    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    Err = AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                             &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif


#ifdef ST_OSLINUX

    AllocDataParams.Alignment = 16;
    AllocDataParams.Size = 256*4;
    Err = STLAYER_AllocData( FakeLayerHndl, &AllocDataParams, (void**)&(Palette.Data_p) );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }


#else

    /* Block allocation parameter */
    NbForbiddenRange = 1;
    if (VirtualMapping.VirtualWindowOffset > 0)
    {
        RangeArea[0].StartAddr_p = (void *) VirtualMapping.VirtualBaseAddress_p;
        RangeArea[0].StopAddr_p  = (void *) ((U32)(RangeArea[0].StartAddr_p) +
                                             (U32)(VirtualMapping.VirtualWindowOffset) - 1);
    }
    else /*  VirtualWindowOffset = 0 */
    {
        RangeArea[0].StartAddr_p = (void *) VirtualMapping.VirtualBaseAddress_p;
        RangeArea[0].StopAddr_p  = (void *) RangeArea[0].StartAddr_p;
    }

    if ((VirtualMapping.VirtualWindowOffset + VirtualMapping.VirtualWindowSize) !=
         VirtualMapping.VirtualSize)
    {
        RangeArea[1].StartAddr_p = (void *) ((U32)(RangeArea[0].StartAddr_p) +
                                             (U32)(VirtualMapping.VirtualWindowOffset) +
                                             (U32)(VirtualMapping.VirtualWindowSize));
        RangeArea[1].StopAddr_p  = (void *) ((U32)(VirtualMapping.VirtualBaseAddress_p) +
                                                  (U32)(VirtualMapping.VirtualSize) - 1);

        NbForbiddenRange= 2;
    }


    AllocBlockParams.PartitionHandle            = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    AllocBlockParams.AllocMode                  = STAVMEM_ALLOC_MODE_BOTTOM_TOP;
    AllocBlockParams.NumberOfForbiddenRanges    = NbForbiddenRange;
    AllocBlockParams.ForbiddenRangeArray_p      = &RangeArea[0];
    AllocBlockParams.NumberOfForbiddenBorders   = 0;
    AllocBlockParams.ForbiddenBorderArray_p     = NULL;
    AllocBlockParams.Alignment = 16;

    AllocBlockParams.Size = 256*4;


    Err = STAVMEM_AllocBlock(&AllocBlockParams,&PaletteHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
    STAVMEM_GetBlockAddress(PaletteHandle,(void**)&(Palette.Data_p));
#endif

#ifdef ST_OSLINUX
    {
        U32 Tmp;
        for ( Tmp = 0; Tmp <  256 ; Tmp++ )
        {
            *((U32 *) (((U32 *) Palette.Data_p) + Tmp)) = DataPalette[Tmp];
        }
    }
#else
    Err=STAVMEM_CopyBlock1D((void*)  DataPalette, STAVMEM_VirtualToCPU((void*)Palette.Data_p,&VirtualMapping),
                             AllocBlockParams.Size);
    if (Err != 0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR,"Unable to fill for palette\n"));
    }
#endif


    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = &Palette;

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Conversion RGB565 -> CLUT8 \n"));
	Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
	if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
#ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
        time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY );  /* Timeout period was not enough in case of Os21*/
    #endif
    #ifdef ST_OS21
        time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*13 );  /* Timeout period was not enough in case of Os21*/
    #endif
    #ifdef ST_OS20
        time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*50);
    #endif
    #if defined(ST_OSLINUX) || defined(ST_OS21)
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
    #endif
    #ifdef ST_OS20
        #if !defined( ST_7710 )         /* Bug : timeout with 7710 */
            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
                ResetEngine();
            }
        #else
            if( 1 )
            {
            }
        #endif
    #endif
    else
    {
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Conversion_RGB565_to_CLUT8.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Conversion_RGB565_to_CLUT8.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
    }
#else
    /*  Wait for Blit to be completed */
    STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_Conversion_RGB565_to_CLUT8.gam",&TargetBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_Conversion_RGB565_to_CLUT8.gam",&TargetBitmap,&Palette);
    #endif
	if (Err != ST_NO_ERROR)
	{
		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
		return (TRUE);
	}
#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
    STLAYER_FreeData( FakeLayerHndl,Palette.Data_p );
#else
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }

    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&PaletteHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    TargetBitmap.Data1_p=NULL;
    Palette.Data_p=NULL;
    SourceBitmap.Data1_p=NULL;
#endif /* !defined (ST_5100) &&  !defined (ST_5105)  && !defined (ST_7109) &&  !defined (ST_5188) &&  !defined (ST_5107)*/

    /* ------------ Set source ------------ */

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/merouARGB8888.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../merouARGB8888.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap 3: %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;

    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_RGB565;
    TargetBitmap.BitmapType             = SourceBitmap.BitmapType;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT709;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;

    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX

    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    Err = AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                             &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Conversion ARGB8888 -> RGB565\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
#ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
    #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
    #endif

    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        ResetEngine();
    }
    else
    {
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Conversion_ARGB8888_to_RGB565.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Conversion_ARGB8888_to_RGB565.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
    }
#else
    /*  Wait for Blit to be completed */
    STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_Conversion_ARGB8888_to_RGB565.gam",&TargetBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_Conversion_ARGB8888_to_RGB565.gam",&TargetBitmap,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
		return (TRUE);
    }
#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    TargetBitmap.Data1_p=NULL;
    SourceBitmap.Data1_p=NULL;


/*#if !defined (ST_5105) &&  !defined (ST_5188) &&  !defined (ST_5107)*/
    /* ------------ Set source ------------ */
    #ifdef ST_OSLINUX
        Err = ConvertGammaToBitmap("./blt/files/YcbCr422r.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
    #else
        Err = ConvertGammaToBitmap("../../YcbCr422r.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;

    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_RGB565;
    TargetBitmap.BitmapType             = SourceBitmap.BitmapType;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT709;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX

    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);

#else
    Err = AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                             &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;
    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Conversion YCbCr422R -> RGB565\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

#ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
    #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
    #endif

    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        ResetEngine();
    }
    else
    {
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Conversion_YCbCr422R_to_RGB565.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Conversion_YCbCr422R_to_RGB565.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
    }
#else
    /*  Wait for Blit to be completed */
    STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_Conversion_YCbCr422R_to_RGB565.gam",&TargetBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_Conversion_YCbCr422R_to_RGB565.gam",&TargetBitmap,&Palette);
    #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
     /* --------------- Free Target Bitmap ------------ */
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    TargetBitmap.Data1_p=NULL;
/*#endif*/


#if !defined (ST_5100) &&  !defined (ST_5105) &&  !defined (ST_5301) && !defined (ST_7109) &&  !defined (ST_5188)\
&&  !defined (ST_5525) &&  !defined (ST_5107) && !defined (ST_7200)
    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_CLUT8;
    TargetBitmap.BitmapType             = SourceBitmap.BitmapType;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT709;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;

    Palette.PaletteType = STGXOBJ_PALETTE_TYPE_DEVICE_INDEPENDENT;
    Palette.ColorType   = STGXOBJ_COLOR_TYPE_ARGB8888;
    Palette.ColorDepth  = 8;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    Err = AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                             &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

#ifdef ST_OSLINUX

    AllocDataParams.Alignment = 16;
    AllocDataParams.Size = 256*4;

    Err = STLAYER_AllocData( FakeLayerHndl, &AllocDataParams, (void**)&(Palette.Data_p) );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }

#else

    /* Block allocation parameter */
    NbForbiddenRange = 1;
    if (VirtualMapping.VirtualWindowOffset > 0)
    {
        RangeArea[0].StartAddr_p = (void *) VirtualMapping.VirtualBaseAddress_p;
        RangeArea[0].StopAddr_p  = (void *) ((U32)(RangeArea[0].StartAddr_p) +
                                             (U32)(VirtualMapping.VirtualWindowOffset) - 1);
    }
    else /*  VirtualWindowOffset = 0 */
    {
        RangeArea[0].StartAddr_p = (void *) VirtualMapping.VirtualBaseAddress_p;
        RangeArea[0].StopAddr_p  = (void *) RangeArea[0].StartAddr_p;
    }

    if ((VirtualMapping.VirtualWindowOffset + VirtualMapping.VirtualWindowSize) !=
         VirtualMapping.VirtualSize)
    {
        RangeArea[1].StartAddr_p = (void *) ((U32)(RangeArea[0].StartAddr_p) +
                                             (U32)(VirtualMapping.VirtualWindowOffset) +
                                             (U32)(VirtualMapping.VirtualWindowSize));
        RangeArea[1].StopAddr_p  = (void *) ((U32)(VirtualMapping.VirtualBaseAddress_p) +
                                                  (U32)(VirtualMapping.VirtualSize) - 1);

        NbForbiddenRange= 2;
    }

    AllocBlockParams.PartitionHandle            = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    AllocBlockParams.Size                       = 0;
    AllocBlockParams.AllocMode                  = STAVMEM_ALLOC_MODE_BOTTOM_TOP;
    AllocBlockParams.NumberOfForbiddenRanges    = NbForbiddenRange;
    AllocBlockParams.ForbiddenRangeArray_p      = &RangeArea[0];
    AllocBlockParams.NumberOfForbiddenBorders   = 0;
    AllocBlockParams.ForbiddenBorderArray_p     = NULL;
    AllocBlockParams.Alignment = 16;

    AllocBlockParams.Size = 256*4;

    Err = STAVMEM_AllocBlock(&AllocBlockParams,&PaletteHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
    STAVMEM_GetBlockAddress(PaletteHandle,(void**)&(Palette.Data_p));
#endif

#ifdef ST_OSLINUX
    {
        U32 Tmp;
        for ( Tmp = 0; Tmp <  256 ; Tmp++ )
        {
            *((U32 *) (((U32 *) Palette.Data_p) + Tmp)) = DataPalette[Tmp];
        }
    }

#else
    Err=STAVMEM_CopyBlock1D((void*)DataPalette,STAVMEM_VirtualToCPU((void*)Palette.Data_p,&VirtualMapping),
                            AllocBlockParams.Size );
    if (Err != 0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR,"Unable to fill for palette\n"));
    }
#endif
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = &Palette;

    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Conversion YCbCr422R -> CLUT8\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
#ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
    #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
    #endif

    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        ResetEngine();
    }
    else
    {
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Conversion_YCbCr422R_to_CLUT8.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Conversion_YCbCr422R_to_CLUT8.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
    }
#else
    /*  Wait for Blit to be completed */
    STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_Conversion_YCbCr422R_to_CLUT8.gam",&TargetBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_Conversion_YCbCr422R_to_CLUT8.gam",&TargetBitmap,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
    STLAYER_FreeData( FakeLayerHndl,Palette.Data_p );
#else
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }

    Err = STAVMEM_FreeBlock(&FreeBlockParams,&PaletteHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    TargetBitmap.Data1_p=NULL;
    Palette.Data_p=NULL;
    SourceBitmap.Data1_p=NULL;
#endif /* !defined (ST_5100) &&  !defined (ST_5105)  && !defined (ST_7109) &&  !defined (ST_5188) &&  !defined (ST_5107) */


     STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"START CLUT4-->RGB Convert Gamma to Bitmap\n"));
    /* ------------ Set source ------------ */
    #ifdef ST_OSLINUX
        Err = ConvertGammaToBitmap("./blt/files/Chrisb4.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
    #else
        Err = ConvertGammaToBitmap("../../Chrisb4.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;

    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_RGB565;
    TargetBitmap.BitmapType             = SourceBitmap.BitmapType;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT709;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    Err = AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                             &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif


    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Conversion ACLUT4 -> RGB565\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
#ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
    #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
    #endif

    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        ResetEngine();
    }
    else
    {
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Conversion_ACLUT4_to_RGB565.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Conversion_ACLUT4_to_RGB565.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
    }
#else
    /*  Wait for Blit to be completed */
    STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_Conversion_ACLUT4_to_RGB565.gam",&TargetBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_Conversion_ACLUT4_to_RGB565.gam",&TargetBitmap,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
    /* --------------- Free Bitmap ------------ */
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    TargetBitmap.Data1_p=NULL;


    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_UNSIGNED_YCBCR888_422;
    TargetBitmap.BitmapType             = SourceBitmap.BitmapType;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT709;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    Err = AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                             &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Conversion ACLUT4 -> YCbCr422R\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
#ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
    #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
    #endif

    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        ResetEngine();
    }
    else
    {
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Conversion_ACLUT4_to_YCbCr422R.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Conversion_ACLUT4_to_YCbCr422R.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
    }
#else
    /*  Wait for Blit to be completed */
    STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_Conversion_ACLUT4_to_YCbCr422R.gam",&TargetBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_Conversion_ACLUT4_to_YCbCr422R.gam",&TargetBitmap,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    TargetBitmap.Data1_p=NULL;
    SourceBitmap.Data1_p=NULL;

    /* ------------ Set source ------------ */
    #ifdef ST_OSLINUX
        Err = ConvertGammaToBitmap("./blt/files/Chrisb8.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
    #else
        Err = ConvertGammaToBitmap("../../Chrisb8.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;

    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_RGB565;
    TargetBitmap.BitmapType             = SourceBitmap.BitmapType;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT709;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    Err = AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                             &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Conversion ACLUT8 -> RGB565\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
#ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
    #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
    #endif

    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        ResetEngine();
    }
/*    else*/
    {
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Conversion_ACLUT8_to_RGB565.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Conversion_ACLUT8_to_RGB565.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
    }
#else
    /*  Wait for Blit to be completed */
    STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_Conversion_ACLUT8_to_RGB565.gam",&TargetBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_Conversion_ACLUT8_to_RGB565.gam",&TargetBitmap,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
    /* --------------- Free Bitmap ------------ */
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    TargetBitmap.Data1_p=NULL;


    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_UNSIGNED_YCBCR888_422;
    TargetBitmap.BitmapType             = SourceBitmap.BitmapType;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT709;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);

#else
    Err = AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                             &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Conversion ACLUT8 -> YCbCr422R\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }
#ifndef STBLIT_EMULATOR
    #ifdef ST_OSLINUX
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
    #else
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
    #endif

    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        ResetEngine();
    }
    else
    {
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Conversion_ACLUT8_to_YCbCr422R.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Conversion_ACLUT8_to_YCbCr422R.gam",&TargetBitmap,&Palette);
        #endif
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
    }
#else
    /*  Wait for Blit to be completed */
    STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

    #ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_Conversion_ACLUT8_to_YCbCr422R.gam",&TargetBitmap,&Palette);
    #else
        Err = ConvertBitmapToGamma("../../../result/test_Conversion_ACLUT8_to_YCbCr422R.gam",&TargetBitmap,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif

    /* --------------- Free Bitmap ------------ */

    GUTIL_Free(SourceBitmap.Data1_p);

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    TargetBitmap.Data1_p=NULL;
    SourceBitmap.Data1_p=NULL;

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);

}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestCopy2Src;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestCopy2Src (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src1,Src2;
    STBLIT_Destination_t    Dst;


    STGXOBJ_Bitmap_t        Source1Bitmap,Source2Bitmap, TargetBitmap;
    STGXOBJ_Palette_t       Palette1,Palette2,PaletteT;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

	#ifndef STBLIT_EMULATOR
        STOS_Clock_t            time;
	#endif

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105)  || defined (ST_5301) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif


#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /* ------------ Set source 1 ------------ */
    Source1Bitmap.Data1_p = NULL;
    Palette1.Data_p = NULL;


#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/suzie.gam",SECURED_SRC_BITMAP,&Source1Bitmap,&Palette1);
#else
    Err = ConvertGammaToBitmap("../../suzie.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&Source1Bitmap,&Palette1);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }


    Src1.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src1.Data.Bitmap_p        = &Source1Bitmap;
    Src1.Rectangle.PositionX  = 50;
    Src1.Rectangle.PositionY  = 50;
    Src1.Rectangle.Width      = 150;
    Src1.Rectangle.Height     = 150;
    Src1.Palette_p            = &Palette1;

    /* ------------ Set source ------------ */
    Source2Bitmap.Data1_p = NULL;
    Palette2.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/BATEAU.GAM",SECURED_SRC_BITMAP,&Source2Bitmap,&Palette2);
#else
    Err = ConvertGammaToBitmap("../../bateau.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&Source2Bitmap,&Palette1);
  #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &Source2Bitmap;
    Src2.Rectangle.PositionX  = 100;
    Src2.Rectangle.PositionY  = 100;
    Src2.Rectangle.Width      = /*60*/150;
    Src2.Rectangle.Height     = 150;
    Src2.Palette_p            = &Palette2;













    /* ------------ Set Dest ------------ */
    TargetBitmap.Data1_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_DST_BITMAP,&TargetBitmap,&PaletteT);
#else
    Err = ConvertGammaToBitmap("../../merouRGB565.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&PaletteT);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 50;
    Dst.Rectangle.PositionY   = 50;
    Dst.Rectangle.Width       = 150;
    Dst.Rectangle.Height      = 150;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = /*STBLIT_ALU_COPY*/STBLIT_ALU_ALPHA_BLEND;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 20;
    Context.EnableClipRectangle     = /*TRUE*/FALSE;
    /*Context.ClipRectangle           = Rectangle;*/
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;


    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Copy 2Src \n"));
    Err = STBLIT_Blit(Handle,&Src1,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	   /* else */
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_Copy_2Src.gam",&TargetBitmap,&PaletteT);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_Copy_2Src.gam",&TargetBitmap,&PaletteT);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Copy_2Src.gam",&TargetBitmap,&PaletteT);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Copy_2Src.gam",&TargetBitmap,&PaletteT);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(Source1Bitmap.Data1_p);
    GUTIL_Free(Source2Bitmap.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}



/*-----------------------------------------------------------------------------
 * Function : BLIT_TestConcat
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestConcat (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";
    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;

    STGXOBJ_Bitmap_t        AlphaBitmap, ColorBitmap, TargetBitmap;
    STGXOBJ_Palette_t       Palette;

    STGXOBJ_Rectangle_t     SrcRectangle;

#ifndef ST_OSLINUX
    STAVMEM_BlockHandle_t  BlockHandle1;
    STAVMEM_BlockHandle_t  BlockHandle2;
    STAVMEM_FreeBlockParams_t FreeBlockParams;
#endif
    STGXOBJ_BitmapAllocParams_t Params1;
    STGXOBJ_BitmapAllocParams_t Params2;


    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[0];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }
    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }
    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;

    /*-----------------2/26/01 9:04AM-------------------
     * Concat
     * --------------------------------------------------*/

    AlphaBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/MaskAlpha8.gam",0,&AlphaBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../MaskAlpha8.gam",0,&AlphaBitmap,&Palette);
#endif
    AlphaBitmap.ColorType = STGXOBJ_COLOR_TYPE_ALPHA8;
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    ColorBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/m888.gam",0,&ColorBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../m888.gam",0,&ColorBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Dest ------------ */
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;


    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_ARGB8888;
    TargetBitmap.BitmapType             = ColorBitmap.BitmapType;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = /*STGXOBJ_ITU_R_BT601*/STGXOBJ_ITU_R_BT709;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = AlphaBitmap.Width;
    TargetBitmap.Height                 = AlphaBitmap.Height;
    TargetBitmap.Data1_p                = NULL;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[0],
                       &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif


    /* ------------ Set Context ------------ */
    /* Set Context */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = TRUE;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;


    /* ------------ Blit ------------ */


    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Concat operation\n"));


    /* Concatenation */

    SrcRectangle.PositionX  = 0;
    SrcRectangle.PositionY  = 0;
    SrcRectangle.Width      = 720;
    SrcRectangle.Height     = 480;

    Err = STBLIT_Concat(Handle,&AlphaBitmap,&SrcRectangle,&ColorBitmap,&SrcRectangle, &TargetBitmap, &SrcRectangle,&Context);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Concat: %d\n",Err));
        return (TRUE);
    }

    #ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
        else
		{
	        /* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_Concat_Bitmap.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_Concat_Bitmap.gam",&TargetBitmap,&Palette);
            #endif
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Concat_Bitmap.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Concat_Bitmap.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
	        return (TRUE);
		}
	#endif

    /* --------------- Free Bitmap ------------ */

    /* Free Bitmapq */
    GUTIL_Free(AlphaBitmap.Data1_p);
    GUTIL_Free(ColorBitmap.Data1_p);
/*    GUTIL_Free(TargetBitmap.Data1_p);*/

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[0];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif


    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}


/*-----------------------------------------------------------------------------
 * Function : BLIT_TestBlend;

 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestBlend (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STGXOBJ_Palette_t       Palette;
    STGXOBJ_Color_t         Color;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;


    #ifndef STBLIT_EMULATOR
        STOS_Clock_t            time;
	#endif

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined (ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;


    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 20;
    Src.Rectangle.PositionY  = 20;
    Src.Rectangle.Width      = 200;
    Src.Rectangle.Height     = 200;
    Src.Palette_p            = &Palette;

    /* ------------ Set Dest ------------ */
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(DstFileName,SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(DstFileName,DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 10;
    Dst.Rectangle.PositionY   = 10;
    Dst.Rectangle.Width       = 200;
    Dst.Rectangle.Height      = 200;


    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = /*STBLIT_ALU_COPY*/STBLIT_ALU_ALPHA_BLEND;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 60;
    Context.EnableClipRectangle     = /*TRUE*/FALSE;
    /*Context.ClipRectangle           = Rectangle;*/
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;



    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;


    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Bitmap Blend \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
			STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	   /* else */
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_Blend_Bitmap.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_Blend_Bitmap.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_Blend_Bitmap.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Blend_Bitmap.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /* ------------ Set Source ------------ */
    Color.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;
    Color.Value.RGB565.R     = 0x00 ;
    Color.Value.RGB565.G     = 0x00 ;
    Color.Value.RGB565.B     = 0xff ;

    Src.Type                 = STBLIT_SOURCE_TYPE_COLOR;
    Src.Data.Color_p=&Color;

    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    SourceBitmap.Data1_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(SrcFileName,SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(SrcFileName,SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 20;
    Src.Rectangle.PositionY  = 20;
    Src.Rectangle.Width      = 200;
    Src.Rectangle.Height     = 200;
    Src.Palette_p            = &Palette;

#endif


    /* ------------ Set Dest ------------ */
    #ifdef ST_OSLINUX
        Err = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_DST_BITMAP,&TargetBitmap,&Palette);
    #else
        Err = ConvertGammaToBitmap("../../merouRGB565.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 10;
    Dst.Rectangle.PositionY   = 10;
    Dst.Rectangle.Width       = 200;
    Dst.Rectangle.Height      = 200;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = /*STBLIT_ALU_COPY*/STBLIT_ALU_ALPHA_BLEND;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 60;
    Context.EnableClipRectangle     = /*TRUE*/FALSE;
    /*Context.ClipRectangle           = Rectangle;*/
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;


    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Color Blend \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	   /* else */
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_Blend_Color.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_Blend_Color.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertGammaToBitmap("./blt/result/test_Blend_Color.gam",0,&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_Blend_Color.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}


/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFilterFlicker;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestFilterFlicker (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STGXOBJ_Palette_t       Palette;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;


    #ifndef STBLIT_EMULATOR
        STOS_Clock_t            time;
	#endif

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined (ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;


    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/bateauARB8888.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../bateauARB8888.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;

    /* ------------ Set Dest ------------ */
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/bateauARB8888.gam",SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../bateauARB8888.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = SourceBitmap.Width;
    Dst.Rectangle.Height      = SourceBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 60;
    Context.EnableClipRectangle     = /*TRUE*/FALSE;
    /*Context.ClipRectangle           = Rectangle;*/
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = TRUE/*FALSE*/;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;


    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Flicker Fliter Test \n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	   /* else */
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_FlickerFilter_Mode_SIMPLE.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_FlickerFilter_Mode_SIMPLE.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_FlickerFilter_Mode_SIMPLE.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_FlickerFilter_Mode_SIMPLE.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

#if defined (ST_7710) || defined (ST_7100)

	/* ------------ Set Dest ------------ */
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

    #ifdef ST_OSLINUX
        Err = ConvertGammaToBitmap("./blt/files/bateauARB8888.gam",SECURED_DST_BITMAP,&TargetBitmap,&Palette);
    #else
        Err = ConvertGammaToBitmap("../../bateauARB8888.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
    #endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

	/* ------------ Set FF mode ------------ */
	Err = STBLIT_SetFlickerFilterMode(Handle,STBLIT_FLICKER_FILTER_MODE_ADAPTIVE);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set FF mode : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Flicker Fliter Test Mode Adaptif\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	   /* else */
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_FlickerFilter_Mode_ADAPTIVE.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_FlickerFilter_Mode_ADAPTIVE.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertGammaToBitmap("./blt/result/test_FlickerFilter_Mode_ADAPTIVE.gam",0,&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_FlickerFilter_Mode_ADAPTIVE.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif
#endif /* defined (ST_7710) || defined (ST_7100) */

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestVC1Range;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestVC1Range (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STGXOBJ_Palette_t       Palette;


#ifndef ST_OSLINUX
    STAVMEM_BlockHandle_t  BlockHandle1;
    STAVMEM_BlockHandle_t  BlockHandle2;
    STAVMEM_FreeBlockParams_t FreeBlockParams;
#endif
    STGXOBJ_BitmapAllocParams_t Params1;
    STGXOBJ_BitmapAllocParams_t Params2;


    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

    #ifndef STBLIT_EMULATOR
        STOS_Clock_t            time;
	#endif

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                       = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                       = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined (ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined (ST_7200)

    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;


    /*=======================================================================*/
    /* Test1 : Range Mapping test                                            */
    /*=======================================================================*/
    /* ------------ Set source ------------ */

    SourceBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/bateauMB420.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../bateauMB420.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Data.Bitmap_p->BitmapType = STGXOBJ_BITMAP_TYPE_MB_RANGE_MAP;
    /* set Src YUVScaling factors for range Reduction */
    Src.Data.Bitmap_p->YUVScaling.ScalingFactorY  = YUV_RANGE_MAP_6;
    Src.Data.Bitmap_p->YUVScaling.ScalingFactorUV = YUV_RANGE_MAP_6;

    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;


    /* ------------ Set Dst ------------ */
    TargetBitmap.Data1_p=NULL;
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_UNSIGNED_YCBCR888_422;
    TargetBitmap.BitmapType             = STGXOBJ_BITMAP_TYPE_RASTER_PROGRESSIVE;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT601;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;
    TargetBitmap.Data1_p                = NULL;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                       &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY /*STBLIT_ALU_ALPHA_BLEND*/;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 60;
    Context.EnableClipRectangle     = /*TRUE*/FALSE;
    /*Context.ClipRectangle           = Rectangle;*/
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;


    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start VC1 Range Mapping\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	   /* else */
        {
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_MB_To_YCbCr422R_VC1_RangeMapping.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_MB_To_YCbCr422R_VC1_RangeMapping.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_MB_To_YCbCr422R_VC1_RangeMapping.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_MB_To_YCbCr422R_VC1_RangeMapping.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /* --------------- Free Bitmap ------------ */
    /* Free TargetBitmap */

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif


    TargetBitmap.Data1_p=NULL;

    /*=======================================================================*/
    /* Test2 : Range Reduction Test                                          */
    /*=======================================================================*/
#ifdef ST_OSLINUX
    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

    Err = ConvertGammaToBitmap("./blt/files/bateauMB420.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    /* set Src YUVScaling factors for range Reduction */
    Src.Data.Bitmap_p->YUVScaling.ScalingFactorY  = YUV_RANGE_MAP_7;
    Src.Data.Bitmap_p->YUVScaling.ScalingFactorUV = YUV_RANGE_MAP_7;

    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;
#endif


    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_UNSIGNED_YCBCR888_422;
    TargetBitmap.BitmapType             = STGXOBJ_BITMAP_TYPE_RASTER_PROGRESSIVE;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT601;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;
    TargetBitmap.Data1_p                = NULL;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                       &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY /*STBLIT_ALU_ALPHA_BLEND*/;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 60;
    Context.EnableClipRectangle     = /*TRUE*/FALSE;
    /*Context.ClipRectangle           = Rectangle;*/
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;


    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Range Reduction\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	   /* else */
        {
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_MB_To_YCbCr422R_VC1_RangeReduction.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_MB_To_YCbCr422R_VC1_RangeReduction.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_MB_To_YCbCr422R_VC1_RangeReduction.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_MB_To_YCbCr422R_VC1_RangeReduction.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /* --------------- Free Bitmap ------------ */
    /* Free TargetBitmap */

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}
/*-----------------------------------------------------------------------------
 * Function : BLIT_TestMacroBlocConversion;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestMBConversion (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Bitmap_t        SourceBitmap, TargetBitmap;
    STGXOBJ_Palette_t       Palette;


#ifndef ST_OSLINUX
    STAVMEM_BlockHandle_t  BlockHandle1;
    STAVMEM_BlockHandle_t  BlockHandle2;
    STAVMEM_FreeBlockParams_t FreeBlockParams;
#endif
    STGXOBJ_BitmapAllocParams_t Params1;
    STGXOBJ_BitmapAllocParams_t Params2;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

    #ifndef STBLIT_EMULATOR
        STOS_Clock_t            time;
	#endif

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                       = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                       = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined (ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined (ST_7200)

    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif


#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;


    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap("./blt/files/bateauMB420.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap("../../bateauMB420.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&SourceBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;

    /*=======================================================================*/
    /* Test1 : Test color conversion from MB to RGB565                       */
    /*=======================================================================*/

    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_RGB565;
    TargetBitmap.BitmapType             = STGXOBJ_BITMAP_TYPE_RASTER_PROGRESSIVE;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT601;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;
    TargetBitmap.Data1_p                = NULL;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    Err = AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                             &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;


    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY /*STBLIT_ALU_ALPHA_BLEND*/;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 60;
    Context.EnableClipRectangle     = /*TRUE*/FALSE;
    /*Context.ClipRectangle           = Rectangle;*/
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;

    /* ------------ Blit ------------ */
  STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Conversion from MB to RGB565\n"));
   Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	   /* else */
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_MB_To_RGB565_Conversion.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_MB_To_RGB565_Conversion.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_MB_To_YCbCr422R_Conversion.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_MB_To_RGB565_Conversion.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /* --------------- Free Bitmap ------------ */
#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
    /* Free TargetBitmap */
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif

    TargetBitmap.Data1_p=NULL;

    /*=======================================================================*/
    /* Test2 : Test color conversion from MB to YCbCr422                     */
    /*=======================================================================*/
#ifdef ST_OSLINUX
    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

    Err = ConvertGammaToBitmap("./blt/files/bateauMB420.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;
#endif


    /* ------------ Set Dst ------------ */
    TargetBitmap.ColorType              = STGXOBJ_COLOR_TYPE_UNSIGNED_YCBCR888_422;
    TargetBitmap.BitmapType             = STGXOBJ_BITMAP_TYPE_RASTER_PROGRESSIVE;
    TargetBitmap.PreMultipliedColor     = FALSE;
    TargetBitmap.ColorSpaceConversion   = STGXOBJ_ITU_R_BT601;
    TargetBitmap.AspectRatio            = STGXOBJ_ASPECT_RATIO_SQUARE;
    TargetBitmap.Width                  = SourceBitmap.Width;
    TargetBitmap.Height                 = SourceBitmap.Height;
    TargetBitmap.Data1_p                = NULL;

    /* Alloc Destination */
    Err = STBLIT_GetBitmapAllocParams(Handle,&TargetBitmap,&Params1,&Params2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Get Bitmap Alloc Params : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    Err = AllocBitmapBuffer (&TargetBitmap,SECURED_DST_BITMAP,(void*)TEST_SHARED_MEM_BASE_ADDRESS,
                             &Params1, &Params2);
#else
    AllocBitmapBuffer (&TargetBitmap,(void*)TEST_SHARED_MEM_BASE_ADDRESS, AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM],
                       &BlockHandle1,&Params1, &BlockHandle2,&Params2);
#endif

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY /*STBLIT_ALU_ALPHA_BLEND*/;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 60;
    Context.EnableClipRectangle     = /*TRUE*/FALSE;
    /*Context.ClipRectangle           = Rectangle;*/
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;


    /* ------------ Blit ------------ */
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start Conversion from MB to YCbCr422R\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	   /* else */
        {
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_MB_To_YCbCr422R_Conversion.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_MB_To_YCbCr422R_Conversion.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_MB_To_YCbCr422R_Conversion.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_MB_To_YCbCr422R_Conversion.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif

    /* --------------- Free Bitmap ------------ */
    /* Free TargetBitmap */

#ifdef ST_OSLINUX
    STLAYER_FreeData( FakeLayerHndl,TargetBitmap.Data1_p );
#else
    FreeBlockParams.PartitionHandle = AvmemPartitionHandle[DST_BITMAP_AVMEM_PARTITION_NUM];
    Err = STAVMEM_FreeBlock(&FreeBlockParams,&BlockHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "Shared memory allocation failed"));
        return(STBLIT_ERROR_NO_AV_MEMORY);
    }
#endif


    TargetBitmap.Data1_p=NULL;
    Palette.Data_p = NULL;

#if defined(ST_OSLINUX) && (defined(ST_7109) || defined(ST_7200))
#else

    /*=======================================================================*/
    /* Test3 : Test MB Copy                                                  */
    /*=======================================================================*/

#ifdef ST_OSLINUX
    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;
    Err = ConvertGammaToBitmap("./blt/files/bateauMB420.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Palette_p            = &Palette;
#endif


    Src.Rectangle.PositionX  = 200;
    Src.Rectangle.PositionY  = 200;
    Src.Rectangle.Width      = 200;
    Src.Rectangle.Height     = 200;
#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(DstFileName,SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(DstFileName,DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 20;
    Dst.Rectangle.PositionY   = 20;
    Dst.Rectangle.Width       = Src.Rectangle.Width;
    Dst.Rectangle.Height      = Src.Rectangle.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY /*STBLIT_ALU_ALPHA_BLEND*/;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 60;
    Context.EnableClipRectangle     = /*TRUE*/FALSE;
    /*Context.ClipRectangle           = Rectangle;*/
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;


    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start MB Copy\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	   /* else */
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_MB_Copy.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_MB_Copy.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
        #ifdef ST_OSLINUX
            Err = ConvertBitmapToGamma("./blt/result/test_MB_Copy.gam",&TargetBitmap,&Palette);
        #else
            Err = ConvertBitmapToGamma("../../../result/test_MB_Copy.gam",&TargetBitmap,&Palette);
        #endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif
    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(TargetBitmap.Data1_p);

    /*=======================================================================*/
    /* Test4 : Test MB Resize                                                */
    /*=======================================================================*/

#ifdef ST_OSLINUX
    /* ------------ Set source ------------ */
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p = NULL;

    Err = ConvertGammaToBitmap("./blt/files/bateauMB420.gam",SECURED_SRC_BITMAP,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Palette_p            = &Palette;
#endif


    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;

#ifdef ST_OSLINUX
    Err = ConvertGammaToBitmap(DstFileName,SECURED_DST_BITMAP,&TargetBitmap,&Palette);
#else
    Err = ConvertGammaToBitmap(DstFileName,DST_BITMAP_AVMEM_PARTITION_NUM,&TargetBitmap,&Palette);
#endif
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }


    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 20;
    Dst.Rectangle.PositionY   = 20;
    Dst.Rectangle.Width       = TargetBitmap.Width - 40;
    Dst.Rectangle.Height      = TargetBitmap.Height - 40;


    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 60;
    Context.EnableClipRectangle     = /*TRUE*/FALSE;
    /*Context.ClipRectangle           = Rectangle;*/
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;


    /* ------------ Blit ------------ */

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Start MB Resize\n"));
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Blit : %d\n",Err));
        return (TRUE);
    }

	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif

        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
            ResetEngine();
        }
	   /* else */
		{
			/* Generate Bitmap */
            #ifdef ST_OSLINUX
                Err = ConvertBitmapToGamma("./blt/result/test_MB_Resize.gam",&TargetBitmap,&Palette);
            #else
                Err = ConvertBitmapToGamma("../../../result/test_MB_Resize.gam",&TargetBitmap,&Palette);
            #endif
			fflush(stdout);
			if (Err != ST_NO_ERROR)
	        {
		        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			    return (TRUE);
	        }
		}
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);

		STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Blit Done"));

		/* Generate Bitmap */
#ifdef ST_OSLINUX
        Err = ConvertBitmapToGamma("./blt/result/test_MB_Resize.gam",&TargetBitmap,&Palette);
#else
        Err = ConvertBitmapToGamma("../../../result/test_MB_Resize.gam",&TargetBitmap,&Palette);
#endif
		if (Err != ST_NO_ERROR)
		{
			STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
			return (TRUE);
		}
	#endif
#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(Name,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestJob;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestJob (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          ErrCode;
    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     Open_Params;
    STBLIT_TermParams_t     Term_Params;
    STBLIT_Handle_t         Hdl;
    STBLIT_Source_t         Src1, Src2, Src3, Src4, SrcForeground;
    STBLIT_Destination_t    Dst1,Dst2,Dst3;
    STGXOBJ_Color_t         ColorFill;
    STBLIT_BlitContext_t    Context;

    STGXOBJ_Bitmap_t        Bmp1, Bmp2, Bmp3, Bmp4, BmpT;
    STGXOBJ_Palette_t       Pal;
    STGXOBJ_Rectangle_t     PositionRectangle1,PositionRectangle2,PositionRectangle3,ClipRectangle;
    ST_DeviceName_t         Name="Blitter\0";
    STBLIT_JobParams_t      Job_Params;
    STBLIT_JobHandle_t      Job1;
    STBLIT_JobBlitHandle_t  JBHandle_1,JBHandle_2,JBHandle_3;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;


    /* ------------ Blit Device Init ------------ */

    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                      = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                      = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
     InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188) || defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.JobBlitNodeMaxNumber                 = 1000;
#else
    InitParams.JobBlitNodeMaxNumber                 = 30;
#endif
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 6;
/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)

    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        ErrCode = STBLIT_Init(Name,&InitParams);
        if (ErrCode != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",ErrCode));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    ErrCode = STBLIT_Open(Name,&Open_Params,&Hdl);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",ErrCode));
        return (TRUE);
    }

    /* ------------ Process Blit operation ------------ */

    /* ----- Set Job Params ----*/
    Job_Params.NotifyJobCompletion = TRUE;

    /* ------------ Initialize global semaphores ------------ */
    JobCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* Open Event handler */
    ErrCode = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",ErrCode));
        return (TRUE);
    }

    /* Subscribe to Blit Completed event */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    ErrCode = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",ErrCode));
        return (TRUE);
    }


    EvtSubscribeParams.NotifyCallback   = JobCompletedHandler;
    ErrCode = STEVT_SubscribeDeviceEvent(EvtHandle, Name,STBLIT_JOB_COMPLETED_EVT,&EvtSubscribeParams);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Job completion : %d\n",ErrCode));
        return (TRUE);
    }

    /* Get Subscriber ID */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;
    Job_Params.EventSubscriberID = SubscriberID;


    /* ---- Set Job 1 --------*/
    ErrCode = STBLIT_CreateJob(Hdl,&Job_Params,&Job1);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Create job 1: %d\n",ErrCode));
        return (TRUE);
    }


    /* Set Bitmap */
    Bmp1.Data1_p=NULL;
    Bmp2.Data1_p=NULL;
    Bmp3.Data1_p=NULL;
    Bmp4.Data1_p=NULL;
    BmpT.Data1_p=NULL;
    Pal.Data_p=NULL;

#ifdef ST_OSLINUX
    ErrCode = ConvertGammaToBitmap("./blt/files/merouRGB565.gam",SECURED_SRC_BITMAP, &Bmp1, &Pal);
    ErrCode = ConvertGammaToBitmap("./blt/files/tempo1.gam",SECURED_SRC_BITMAP, &Bmp2, &Pal);
    ErrCode = ConvertGammaToBitmap("./blt/files/bateau_test1.gam",SECURED_SRC_BITMAP, &Bmp3, &Pal);
    ErrCode = ConvertGammaToBitmap("./blt/files/JO.GAM",SECURED_SRC_BITMAP, &Bmp4, &Pal);
    ErrCode = ConvertGammaToBitmap("./blt/files/BATEAU.GAM",SECURED_DST_BITMAP, &BmpT, &Pal);
#else

    ErrCode = ConvertGammaToBitmap("../../merouRGB565.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&Bmp1,&Pal);
    ErrCode = ConvertGammaToBitmap("../../tempo1.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&Bmp2,&Pal);
    ErrCode = ConvertGammaToBitmap("../../bateau_test1.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&Bmp3,&Pal);
    ErrCode = ConvertGammaToBitmap("../../JO.gam",SRC_BITMAP_AVMEM_PARTITION_NUM,&Bmp4,&Pal);
    ErrCode = ConvertGammaToBitmap("../../BATEAU.gam",DST_BITMAP_AVMEM_PARTITION_NUM,&BmpT,&Pal);

#endif


    /* Set Src1 */
    Src1.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src1.Data.Bitmap_p        = &Bmp1;
    Src1.Rectangle.PositionX  = 0;
    Src1.Rectangle.PositionY  = 0;
    Src1.Rectangle.Width      = 150;
    Src1.Rectangle.Height     = 150;
    Src1.Palette_p            = NULL;

    /* Set Src2 */
    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &Bmp2;
    Src2.Rectangle.PositionX  = 0;
    Src2.Rectangle.PositionY  = 0;
    Src2.Rectangle.Width      = 150;
    Src2.Rectangle.Height     = 150;
    Src2.Palette_p            = NULL;

    /* Set Src3 */
    Src3.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src3.Data.Bitmap_p        = &Bmp3;
    Src3.Rectangle.PositionX  = 0;
    Src3.Rectangle.PositionY  = 0;
    Src3.Rectangle.Width      = 150;
    Src3.Rectangle.Height     = 150;
    Src3.Palette_p            = NULL;

    /* Set Src4 */
    Src4.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src4.Data.Bitmap_p        = &Bmp4;
    Src4.Rectangle.PositionX  = 0;
    Src4.Rectangle.PositionY  = 0;
    Src4.Rectangle.Width      = 150;
    Src4.Rectangle.Height     = 150;
    Src4.Palette_p            = NULL;

    SrcForeground.Type         = STBLIT_SOURCE_TYPE_COLOR;
    SrcForeground.Data.Color_p = &ColorFill;
    SrcForeground.Palette_p    = NULL;

    /* Set Dst */
    Dst1.Bitmap_p              = &BmpT;
    Dst1.Rectangle.PositionX   = 30;
    Dst1.Rectangle.PositionY   = 30;
    Dst1.Rectangle.Width       = 150;
    Dst1.Rectangle.Height      = 150;
    Dst1.Palette_p             = NULL;

    Dst2.Bitmap_p              = &BmpT;
    Dst2.Rectangle.PositionX   = 30;
    Dst2.Rectangle.PositionY   = 200;
    Dst2.Rectangle.Width       = 150;
    Dst2.Rectangle.Height      = 150;
    Dst2.Palette_p             = NULL;

    Dst3.Bitmap_p              = &BmpT;
    Dst3.Rectangle.PositionX   = 30;
    Dst3.Rectangle.PositionY   = 370;
    Dst3.Rectangle.Width       = 150;
    Dst3.Rectangle.Height      = 150;
    Dst3.Palette_p             = NULL;

    /* Set Main Context */
    Context.ColorKeyCopyMode            = STBLIT_COLOR_KEY_MODE_NONE;
    Context.AluMode                     = STBLIT_ALU_COPY;
    Context.EnableMaskWord              = FALSE;
    Context.EnableMaskBitmap            = FALSE;
    Context.EnableColorCorrection       = FALSE;
    Context.Trigger.EnableTrigger       = FALSE;
    Context.GlobalAlpha                 = 50;
    Context.EnableFlickerFilter         = FALSE;
    Context.UserTag_p                   = NULL;
    Context.EnableResizeFilter          = TRUE;
    Context.NotifyBlitCompletion        = FALSE;
    Context.JobHandle                   = Job1;


    /* Set Job Blit 1 Context */
    Context.EnableClipRectangle      = FALSE;
    Context.ClipRectangle            = ClipRectangle;
    Context.WriteInsideClipRectangle = TRUE;

    STBLIT_Blit(Hdl,NULL,&Src1,&Dst1,&Context );

    STBLIT_GetJobBlitHandle( Hdl, Job1, &JBHandle_1) ;

    /* Set Job Blit 2 Context */
    ClipRectangle.PositionX = 50;
    ClipRectangle.PositionY = 220;
    ClipRectangle.Width     = 80;
    ClipRectangle.Height    = 80;

    Context.EnableClipRectangle      = TRUE;
    Context.ClipRectangle            = ClipRectangle;
    Context.WriteInsideClipRectangle = FALSE;

    STBLIT_Blit(Hdl,NULL,&Src1,&Dst2,&Context );

    STBLIT_GetJobBlitHandle( Hdl, Job1, &JBHandle_2) ;

    /* Set Job Blit 3 Context */
    Context.EnableClipRectangle      = FALSE;
    Context.ClipRectangle            = ClipRectangle;
    Context.WriteInsideClipRectangle = TRUE;

    ColorFill.Type               = STGXOBJ_COLOR_TYPE_RGB565 ;
    ColorFill.Value.RGB565.R     = 0x00 ;
    ColorFill.Value.RGB565.G     = 0x00 ;
    ColorFill.Value.RGB565.B     = 0x1F ;

    STBLIT_Blit(Hdl,NULL,&SrcForeground,&Dst3,&Context );

    STBLIT_GetJobBlitHandle( Hdl, Job1, &JBHandle_3) ;

    /* ---- 1st Passe--------*/

    /* SubmitJob */
    ErrCode = STBLIT_SubmitJobFront(Hdl, Job1);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Submit job : %d\n",ErrCode));
        return (TRUE);
    }

    /* Wait for Job blit to be completed */
    STOS_SemaphoreWaitTimeOut(JobCompletionSemaphore_p, TIMEOUT_INFINITY);

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Job Done"));

    /* ---- 2d Passe--------*/
    PositionRectangle1.PositionX   = 200;
    PositionRectangle1.PositionY   = 30;
    PositionRectangle1.Width       = 150;
    PositionRectangle1.Height      = 150;

    ErrCode = STBLIT_SetJobBlitRectangle( Hdl, JBHandle_1, STBLIT_DATA_TYPE_DESTINATION, &PositionRectangle1 );
    ErrCode = STBLIT_SetJobBlitBitmap   ( Hdl, JBHandle_1, STBLIT_DATA_TYPE_FOREGROUND , &Bmp2               );

    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",ErrCode));
        return (TRUE);
    }

    ClipRectangle.PositionX = 220;
    ClipRectangle.PositionY = 220;
    ClipRectangle.Width     = 80;
    ClipRectangle.Height    = 80;

    PositionRectangle2.PositionX   = 220;
    PositionRectangle2.PositionY   = 200;
    PositionRectangle2.Width       = 150;
    PositionRectangle2.Height      = 150;

    ErrCode = STBLIT_SetJobBlitRectangle( Hdl, JBHandle_2, STBLIT_DATA_TYPE_DESTINATION, &PositionRectangle2 ) ;
    ErrCode = STBLIT_SetJobBlitClipRectangle(Hdl, JBHandle_2, TRUE,&ClipRectangle);

    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",ErrCode));
        return (TRUE);
    }

    ColorFill.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;
    ColorFill.Value.RGB565.R     = 0x00 ;
    ColorFill.Value.RGB565.G     = 0x3F ;
    ColorFill.Value.RGB565.B     = 0x00 ;

    PositionRectangle3.PositionX   = 200;
    PositionRectangle3.PositionY   = 370;
    PositionRectangle3.Width       = 150;
    PositionRectangle3.Height      = 150;

    ErrCode = STBLIT_SetJobBlitRectangle( Hdl, JBHandle_3, STBLIT_DATA_TYPE_DESTINATION, &PositionRectangle3 ) ;
    ErrCode = STBLIT_SetJobBlitColorFill( Hdl, JBHandle_3, STBLIT_DATA_TYPE_FOREGROUND , &ColorFill          );
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",ErrCode));
        return (TRUE);
    }

    /* SubmitJob */
    ErrCode = STBLIT_SubmitJobFront(Hdl, Job1);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Submit job : %d\n",ErrCode));
        return (TRUE);
    }

    /* Wait for Job blit to be completed */
    STOS_SemaphoreWaitTimeOut(JobCompletionSemaphore_p, TIMEOUT_INFINITY);

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Job Done"));

    /* ---- 3d Passe--------*/

    PositionRectangle1.PositionX   = 370;
    PositionRectangle1.PositionY   = 30;
    PositionRectangle1.Width       = 150;
    PositionRectangle1.Height      = 150;

    ErrCode = STBLIT_SetJobBlitRectangle( Hdl, JBHandle_1, STBLIT_DATA_TYPE_DESTINATION, &PositionRectangle1 ) ;
    ErrCode = STBLIT_SetJobBlitBitmap   ( Hdl, JBHandle_1, STBLIT_DATA_TYPE_FOREGROUND, &Bmp3               );
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",ErrCode));
        return (TRUE);
    }

    ClipRectangle.PositionX = 420;
    ClipRectangle.PositionY = 250;
    ClipRectangle.Width     = 80;
    ClipRectangle.Height    = 80;

    PositionRectangle2.PositionX   = 370;
    PositionRectangle2.PositionY   = 200;
    PositionRectangle2.Width       = 150;
    PositionRectangle2.Height      = 150;

    ErrCode = STBLIT_SetJobBlitRectangle( Hdl, JBHandle_2, STBLIT_DATA_TYPE_DESTINATION, &PositionRectangle2 ) ;
    ErrCode = STBLIT_SetJobBlitClipRectangle(Hdl, JBHandle_2, FALSE,&ClipRectangle);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",ErrCode));
        return (TRUE);
    }

    ColorFill.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;
    ColorFill.Value.RGB565.R     = 0x1F ;
    ColorFill.Value.RGB565.G     = 0x00 ;
    ColorFill.Value.RGB565.B     = 0x00 ;

    PositionRectangle3.PositionX   = 370;
    PositionRectangle3.PositionY   = 370;
    PositionRectangle3.Width       = 150;
    PositionRectangle3.Height      = 150;

    ErrCode = STBLIT_SetJobBlitRectangle( Hdl, JBHandle_3, STBLIT_DATA_TYPE_DESTINATION, &PositionRectangle3 ) ;
    ErrCode = STBLIT_SetJobBlitColorFill( Hdl, JBHandle_3, STBLIT_DATA_TYPE_FOREGROUND , &ColorFill          );
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",ErrCode));
        return (TRUE);
    }

    /* SubmitJob */
    ErrCode = STBLIT_SubmitJobFront(Hdl, Job1);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Submit job : %d\n",ErrCode));
        return (TRUE);
    }

    /* Wait for Job blit to be completed */
    STOS_SemaphoreWaitTimeOut(JobCompletionSemaphore_p, TIMEOUT_INFINITY);

    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Job Done"));

    /* ---- 4d Passe--------*/
    PositionRectangle1.PositionX   = 540;
    PositionRectangle1.PositionY   = 30;
    PositionRectangle1.Width       = 150;
    PositionRectangle1.Height      = 150;

    ErrCode = STBLIT_SetJobBlitRectangle( Hdl, JBHandle_1, STBLIT_DATA_TYPE_DESTINATION, &PositionRectangle1 ) ;
    ErrCode = STBLIT_SetJobBlitBitmap   ( Hdl, JBHandle_1, STBLIT_DATA_TYPE_FOREGROUND, &Bmp4               );
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",ErrCode));
        return (TRUE);
    }

    ClipRectangle.PositionX = 590;
    ClipRectangle.PositionY = 250;
    ClipRectangle.Width     = 80;
    ClipRectangle.Height    = 80;

    PositionRectangle2.PositionX   = 540;
    PositionRectangle2.PositionY   = 200;
    PositionRectangle2.Width       = 150;
    PositionRectangle2.Height      = 150;

    ErrCode = STBLIT_SetJobBlitRectangle( Hdl, JBHandle_2, STBLIT_DATA_TYPE_DESTINATION, &PositionRectangle2 ) ;
    ErrCode = STBLIT_SetJobBlitClipRectangle(Hdl, JBHandle_2, TRUE,&ClipRectangle);

    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",ErrCode));
        return (TRUE);
    }

    ColorFill.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;
    ColorFill.Value.RGB565.R     = 0x1F ;
    ColorFill.Value.RGB565.G     = 0x3F ;
    ColorFill.Value.RGB565.B     = 0x1F ;

    PositionRectangle3.PositionX   = 540;
    PositionRectangle3.PositionY   = 370;
    PositionRectangle3.Width       = 150;
    PositionRectangle3.Height      = 150;

    ErrCode = STBLIT_SetJobBlitRectangle( Hdl, JBHandle_3, STBLIT_DATA_TYPE_DESTINATION, &PositionRectangle3 ) ;
    ErrCode = STBLIT_SetJobBlitColorFill( Hdl, JBHandle_3, STBLIT_DATA_TYPE_FOREGROUND , &ColorFill          );
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",ErrCode));
        return (TRUE);
    }

    /* SubmitJob */
    ErrCode = STBLIT_SubmitJobFront(Hdl, Job1);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Submit job : %d\n",ErrCode));
        return (TRUE);
    }

    STOS_SemaphoreWaitTimeOut(JobCompletionSemaphore_p, TIMEOUT_INFINITY);
    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Job Done"));


    /* Generate Bitmap */
#ifdef ST_OSLINUX
    ErrCode = ConvertBitmapToGamma("./blt/result/test_job.gam", &BmpT, &Pal);
#else
    ErrCode = ConvertBitmapToGamma("../../../result/test_job.gam", &BmpT, &Pal);
#endif
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",ErrCode));
        return (TRUE);
    }


    /* Delete Job */
    ErrCode = STBLIT_DeleteJob(Hdl, Job1);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err delete job 1: %d\n",ErrCode));
        return (TRUE);
    }

    GUTIL_Free(Bmp1.Data1_p);
    GUTIL_Free(Bmp2.Data1_p);
    GUTIL_Free(Bmp3.Data1_p);
    GUTIL_Free(Bmp4.Data1_p);
    GUTIL_Free(BmpT.Data1_p);

    /* --------------- Close Evt ----------*/
    ErrCode = STEVT_Close(EvtHandle);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",ErrCode));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    Term_Params.ForceTerminate = TRUE;
    ErrCode = STBLIT_Term(Name,&Term_Params);
    if (ErrCode != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",ErrCode));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*
================================================================================
Visual Test functions
================================================================================
*/

/*-------------------------------------------------------------------------
 * Function : BLIT_Display_Test
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_Display_Test (STTST_Parse_t *pars_p, char *result_sym_p)
{
    BOOL Err;
    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Rectangle_t     Rectangle, ClearRectangle;
    STGXOBJ_Color_t         Color, ClearColor;
    STGXOBJ_Palette_t       Palette;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;

    STEVT_SubscriberID_t    SubscriberID;
    int                     i,j;
    char                    Value;
    int                     Step = 2;
    int OffsetX = 20;
    int OffsetY = 20;

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                       = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                       = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif

    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined (ST_5188) || defined (ST_5525) || defined (ST_7109) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 30;
    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 2;
    InitParams.WorkBufferUserAllocated              = FALSE;
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109)\
 || defined (ST_5301) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif

    Err = STBLIT_Init(STBLIT_DEVICE_NAME,&InitParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print (("Err Init : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(STBLIT_DEVICE_NAME,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print (("Err Open : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);


    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print (("Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,STBLIT_DEVICE_NAME,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print (("Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
    Context.EventSubscriberID   = SubscriberID;
    Context.UserTag_p   = &UserTagString;

    /* ------------ Set Src ------------ */
    Err = ConvertGammaToBitmap(SrcFileName,SrcPartitionInfo,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print (("Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set ClearColor ------------ */
    if(SourceBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888;
        ClearColor.Value.ARGB8888.Alpha = 0x50;
        ClearColor.Value.ARGB8888.R     = 0x50;
        ClearColor.Value.ARGB8888.G     = 0x50;
        ClearColor.Value.ARGB8888.B     = 0x50;
    }

    if(SourceBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565 )
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;
        ClearColor.Value.RGB565.R       = 0x50 ;
        ClearColor.Value.RGB565.G       = 0x50 ;
        ClearColor.Value.RGB565.B       = 0x50 ;
     }

    if(SourceBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB888 )
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_RGB888 ;
        ClearColor.Value.RGB888.R       = 0x50 ;
        ClearColor.Value.RGB888.G       = 0x50 ;
        ClearColor.Value.RGB888.B       = 0x50 ;
    }

    if(SourceBitmap.ColorType == STGXOBJ_COLOR_TYPE_CLUT8 )
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_CLUT8 ;
        ClearColor.Value.CLUT8          = 0x50 ;
    }


    /* ------------ Set ClearRectangle ------------ */
    ClearRectangle.PositionX = 0;
    ClearRectangle.PositionY = 0;
    ClearRectangle.Width     = TargetBitmap.Width;
    ClearRectangle.Height    = TargetBitmap.Height;

    /* ------------ Set General Context ------------ */
    Context.ColorKeyCopyMode         = STBLIT_COLOR_KEY_MODE_NONE;
    Context.AluMode                  = STBLIT_ALU_COPY;
    Context.EnableClipRectangle      = FALSE;
    Context.EnableMaskWord           = FALSE;
    Context.EnableMaskBitmap         = FALSE;
    Context.EnableColorCorrection    = FALSE;
    Context.Trigger.EnableTrigger    = FALSE;
    Context.EnableFlickerFilter      = FALSE;
    Context.JobHandle                = STBLIT_NO_JOB_HANDLE;
    Context.NotifyBlitCompletion     = TRUE;
    Context.GlobalAlpha              = 60;

    /*
     * Test 1 : Test Copy feature
     * ==========================
     */

    /* ------------ Print Infos ------------ */
    STTBX_Print(("***********************************\n"));
    STTBX_Print((" Test 1 : Copy feature\n"));
    STTBX_Print(("***********************************\n"));

    /* ------------ Reset Bitmap ------------ */
    Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&ClearRectangle,&ClearColor,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit Fill : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Wait Blit Complit ------------ */
    Err = WaitForBlitterComplite();
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit Fill : Time Out\n"));
        return (TRUE);
    }

    for ( j=20;j<=TargetBitmap.Height-40;j+=Step)
    {
        for ( i=20;i<TargetBitmap.Width-40;i+=Step)
        {
            /* ------------ Set Rectangle ------------ */
            Rectangle.PositionX = i;
            Rectangle.PositionY = j;
            Rectangle.Width     = Step;
            Rectangle.Height    = Step;

            /* ------------ Blit ------------ */
            Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&TargetBitmap,Rectangle.PositionX,Rectangle.PositionY,&Context );
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Rectangle=(%d,%d,%d,%d) STBLIT_CopyRectangle(...)=%d",Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height,Err));
                return (TRUE);
            }

            /* ------------ Wait Blit Complit ------------ */
            Err = WaitForBlitterComplite();
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Rectangle=(%d,%d,%d,%d) STBLIT_CopyRectangle(...)= TIME OUT",Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height));
                return (TRUE);
            }
        }
    }

    /* ------------ Wait User confirm ------------ */
    STTBX_Print(("> Is the display OK on the screen ?"));
    STTBX_InputChar(&Value);
    if ( Value!='y' && Value!='Y')
    {
        STTBX_Print(("\nTEST FAILED !\n\n"));
        return (TRUE);
    }
    else
    {
        STTBX_Print(("\nTEST PASSED\n\n"));
    }


    /*
     * Test 2 : Test Clip feature
     * ==========================
     */

    /* ------------ Reset Bitmap ------------ */
    Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&ClearRectangle,&ClearColor,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit Fill : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Wait Blit Complit ------------ */
    Err = WaitForBlitterComplite();
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit Fill : Time Out\n"));
        return (TRUE);
    }

    /* ------------ Init Src & Dst ------------ */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = NULL;

    Dst.Rectangle.PositionX   = OffsetX;
    Dst.Rectangle.PositionY   = OffsetY;
    Dst.Rectangle.Width       = TargetBitmap.Width-40;
    Dst.Rectangle.Height      = TargetBitmap.Height-40;

    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Palette_p             = NULL;

    /* ------------ Set Specific Context ------------ */
    Context.EnableClipRectangle      = TRUE;
    Context.WriteInsideClipRectangle = FALSE;

    /* ------------ Print Infos ------------ */
    STTBX_Print(("***********************************\n"));
    STTBX_Print((" Test 2 : Clip feature\n"));
    STTBX_Print(("***********************************\n"));

    for ( i=TargetBitmap.Width-80;i>100;i-=Step)
    {
        /* ------------ Set Rectangle ------------ */
        Rectangle.PositionX   = 40;
        Rectangle.PositionY   = 40;
        Rectangle.Width       = i;
        Rectangle.Height      = TargetBitmap.Height-80;
        Context.ClipRectangle = Rectangle;

        /* ------------ Blit ------------ */
        Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
        if (Err != ST_NO_ERROR)
        {
            STTBX_Print(("ClipRectangle=(%d,%d,%d,%d) STBLIT_Blit(...)=%d",Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height,Err));
            return (TRUE);
        }

        /* ------------ Wait Blit Complit ------------ */
        Err = WaitForBlitterComplite();
        if (Err != ST_NO_ERROR)
        {
            STTBX_Print(("ClipRectangle=(%d,%d,%d,%d) STBLIT_Blit(...)= TIME OUT",Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height));
            return (TRUE);
        }
    }
    for ( j=TargetBitmap.Height-100;j>=0;j-=Step)
    {
        /* ------------ Set Rectangle ------------ */
        Rectangle.Height      = j;
        Context.ClipRectangle = Rectangle;
        if ( j<=10)
        {
            Context.EnableClipRectangle      = FALSE;
        }


        /* ------------ Blit ------------ */
        Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
        if (Err != ST_NO_ERROR)
        {
            STTBX_Print(("ClipRectangle=(%d,%d,%d,%d) STBLIT_Blit(...)=%d",Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height,Err));
            return (TRUE);
        }

        /* ------------ Wait Blit Complit ------------ */
        Err = WaitForBlitterComplite();
        if (Err != ST_NO_ERROR)
        {
            STTBX_Print(("ClipRectangle=(%d,%d,%d,%d) STBLIT_Blit(...)= TIME OUT",Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height));
            return (TRUE);
        }
    }

    /* ------------ Set Specific Context ------------ */
    Context.EnableClipRectangle      = FALSE;

    /* ------------ Wait User confirm ------------ */
    STTBX_Print(("> Is the display OK on the screen ?"));
    STTBX_InputChar(&Value);
    if ( Value!='y' && Value!='Y')
    {
        STTBX_Print(("\nTEST FAILED !\n\n"));
        return (TRUE);
    }
    else
    {
        STTBX_Print(("\nTEST PASSED\n\n"));
    }


    /*
     * Test 3 : Test Fill feature
     * ==========================
     */

    /* ------------ Reset Bitmap ------------ */
    Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&ClearRectangle,&ClearColor,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit Fill : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Wait Blit Complit ------------ */
    Err = WaitForBlitterComplite();
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit Fill : Time Out\n"));
        return (TRUE);
    }

    /* ------------ Set Color ------------ */
    if(SourceBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        Color.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888;
        Color.Value.ARGB8888.Alpha = 0x00;
        Color.Value.ARGB8888.R     = 0x00;
        Color.Value.ARGB8888.G     = 0x00;
        Color.Value.ARGB8888.B     = 0xFF;
    }

    if(SourceBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565 )
    {
        Color.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;
        Color.Value.RGB565.R       = 0x00 ;
        Color.Value.RGB565.G       = 0x00 ;
        Color.Value.RGB565.B       = 0x50 ;
     }

    if(SourceBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB888 )
    {
        Color.Type                 = STGXOBJ_COLOR_TYPE_RGB888 ;
        Color.Value.RGB888.R       = 0x00 ;
        Color.Value.RGB888.G       = 0x00 ;
        Color.Value.RGB888.B       = 0x50 ;
    }

    if(SourceBitmap.ColorType == STGXOBJ_COLOR_TYPE_CLUT8 )
    {
        Color.Type                 = STGXOBJ_COLOR_TYPE_CLUT8 ;
        Color.Value.CLUT8          = 0x0 ;
    }


    /* ------------ Print Infos ------------ */
    STTBX_Print(("***********************************\n"));
    STTBX_Print((" Test 3 : Fill feature\n"));
    STTBX_Print(("***********************************\n"));

    for ( j=20;j<=TargetBitmap.Height-40;j+=Step)
    {
        for ( i=20;i<TargetBitmap.Width-40;i+=Step)
        {
            /* ------------ Set Rectangle ------------ */
            Rectangle.PositionX = i;
            Rectangle.PositionY = j;
            Rectangle.Width     = Step;
            Rectangle.Height    = Step;

            /* ------------ Set Color ------------ */
            Color.Value.RGB565.R     += j;
            Color.Value.RGB565.G     += i;
            Color.Value.RGB565.B     -= j;

            if(SourceBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
            {
                Color.Value.ARGB8888.Alpha = 0x00;
                Color.Value.ARGB8888.R     += j;
                Color.Value.ARGB8888.G     += i;
                Color.Value.ARGB8888.B     -= j;
            }

            if(SourceBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565 )
            {
                Color.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;
                Color.Value.RGB565.R       += j;
                Color.Value.RGB565.G       += i;
                Color.Value.RGB565.B       -= j;
            }

            if(SourceBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB888 )
            {
                Color.Type                 = STGXOBJ_COLOR_TYPE_RGB888 ;
                Color.Value.RGB888.R       += j;
                Color.Value.RGB888.G       += i;
                Color.Value.RGB888.B       -= j;
            }

            if(SourceBitmap.ColorType == STGXOBJ_COLOR_TYPE_CLUT8 )
            {
                Color.Type                 = STGXOBJ_COLOR_TYPE_CLUT8 ;
                Color.Value.CLUT8          += j + i;
            }





            /* ------------ Blit ------------ */
            Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&Rectangle,&Color,&Context );
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Rectangle=(%d,%d,%d,%d) STBLIT_FillRectangle(...)=%d",Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height,Err));
                return (TRUE);
            }

            /* ------------ Wait Blit Complit ------------ */
            Err = WaitForBlitterComplite();
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Rectangle=(%d,%d,%d,%d) STBLIT_FillRectangle(...)= TIME OUT",Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height));
                return (TRUE);
            }
        }
    }

    /* ------------ Wait User confirm ------------ */
    STTBX_Print(("> Is the display OK on the screen ?"));
    STTBX_InputChar(&Value);
    if ( Value!='y' && Value!='Y')
    {
        STTBX_Print(("\nTEST FAILED !\n\n"));
        return (TRUE);
    }
    else
    {
        STTBX_Print(("\nTEST PASSED\n\n"));
    }

    /*
     * Test 4 : Test Resize feature
     * ============================
     */

    /* ------------ Reset Bitmap ------------ */
    Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&ClearRectangle,&ClearColor,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit Fill : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Wait Blit Complit ------------ */
    Err = WaitForBlitterComplite();
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit Fill : Time Out\n"));
        return (TRUE);
    }


    /* ------------ Init Src & Dst ------------ */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = NULL;

    Dst.Rectangle.PositionX   = 20;
    Dst.Rectangle.PositionY   = 20;
    Dst.Rectangle.Height      = 150;
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Palette_p             = NULL;



    /* ------------ Print Infos ------------ */
    STTBX_Print(("***********************************\n"));
    STTBX_Print((" Test 4 : Resize feature\n"));
    STTBX_Print(("***********************************\n"));

    for ( i=5;i<TargetBitmap.Width-40;i+=Step)
    {
        /* ------------ Set Dst ------------ */
        Dst.Rectangle.Width       = i;

        /* ------------ Blit ------------ */
        Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
        if (Err != ST_NO_ERROR)
        {
            STTBX_Print(("Destination=(%d,%d,%d,%d) STBLIT_Blit(...)=%d",Dst.Rectangle.PositionX,Dst.Rectangle.PositionY,Dst.Rectangle.Width,Dst.Rectangle.Height,Err));
            return (TRUE);
        }

        /* ------------ Wait Blit Complit ------------ */
        Err = WaitForBlitterComplite();
        if (Err != ST_NO_ERROR)
        {
            STTBX_Print(("Destination=(%d,%d,%d,%d) STBLIT_Blit(...)= TIME OUT",Dst.Rectangle.PositionX,Dst.Rectangle.PositionY,Dst.Rectangle.Width,Dst.Rectangle.Height));
            return (TRUE);
        }
    }

    for ( i=155;i<TargetBitmap.Height-40;i+=Step)
    {
        /* ------------ Set Dst ------------ */
        Dst.Rectangle.Height      = i;

        /* ------------ Blit ------------ */
        Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
        if (Err != ST_NO_ERROR)
        {
            STTBX_Print(("STBLIT_BLIT(%d,%d,%d,%d)=%d",Dst.Rectangle.PositionX,Dst.Rectangle.PositionY,Dst.Rectangle.Width,Dst.Rectangle.Height,Err));
            return (TRUE);
        }

        /* ------------ Wait Blit Complit ------------ */
        Err = WaitForBlitterComplite();
        if (Err != ST_NO_ERROR)
        {
            STTBX_Print(("STBLIT_BLIT(%d,%d,%d,%d)= TIME OUT",Dst.Rectangle.PositionX,Dst.Rectangle.PositionY,Dst.Rectangle.Width,Dst.Rectangle.Height));
            return (TRUE);
        }
    }

    /* ------------ Wait User confirm ------------ */
    STTBX_Print(("> Is the display OK on the screen ?"));
    STTBX_InputChar(&Value);
    if ( Value!='y' && Value!='Y')
    {
        STTBX_Print(("\nTEST FAILED !\n\n"));
        return (TRUE);
    }
    else
    {
        STTBX_Print(("\nTEST PASSED\n\n"));
    }

    /*
     * Test 5 : Test Blend feature
     * ==========================
     */
    if(SourceBitmap.ColorType != STGXOBJ_COLOR_TYPE_CLUT8 )
    {
        /* ------------ Reset Bitmap ------------ */
        Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&ClearRectangle,&ClearColor,&Context );
        if (Err != ST_NO_ERROR)
        {
            STTBX_Print(("Err Blit Fill : %d\n",Err));
            return (TRUE);
        }

        /* ------------ Wait Blit Complit ------------ */
        Err = WaitForBlitterComplite();
        if (Err != ST_NO_ERROR)
        {
            STTBX_Print(("Err Blit Fill : Time Out\n"));
            return (TRUE);
        }

        /* ------------ Init Src & Dst ------------ */
        Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
        Src.Data.Bitmap_p        = &SourceBitmap;
        Src.Rectangle.PositionX  = 0;
        Src.Rectangle.PositionY  = 0;
        Src.Rectangle.Width      = SourceBitmap.Width;
        Src.Rectangle.Height     = SourceBitmap.Height;
        Src.Palette_p            = NULL;

        Dst.Rectangle.PositionX   = 20;
        Dst.Rectangle.PositionY   = 20;
        Dst.Rectangle.Width       = TargetBitmap.Width-40;
        Dst.Rectangle.Height      = TargetBitmap.Height-40;
        Dst.Bitmap_p              = &TargetBitmap;
        Dst.Palette_p             = NULL;

        /* ------------ Set Specific Context ------------ */
        Context.AluMode                 = STBLIT_ALU_ALPHA_BLEND;

        /* ------------ Print Infos ------------ */
        STTBX_Print(("***********************************\n"));
        STTBX_Print((" Test 5 : Blend feature\n"));
        STTBX_Print(("***********************************\n"));

        i=0;
        for ( j=0;j<=500;j++)
        {
            if ( j%5==0 )
            {
                i++;
            }

            /* ------------ Set GlobalAlpha ------------ */
            Context.GlobalAlpha       = i;

            /* ------------ Blit ------------ */
            Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("GlobalAlpha=%d STBLIT_Blit(...)=%d",Context.GlobalAlpha,Err));
                return (TRUE);
            }

            /* ------------ Wait Blit Complit ------------ */
            Err = WaitForBlitterComplite();
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("GlobalAlpha=%d STBLIT_Blit(...)= TIME OUT",Context.GlobalAlpha));
                return (TRUE);
            }
        }

        /* ------------ Wait User confirm ------------ */
        STTBX_Print(("> Is the display OK on the screen ?"));
        STTBX_InputChar(&Value);
        if ( Value!='y' && Value!='Y')
        {
            STTBX_Print(("\nTEST FAILED !\n\n"));
            return (TRUE);
        }
        else
        {
            STTBX_Print(("\nTEST PASSED\n\n"));
        }
    }

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
    Err = STBLIT_Term(STBLIT_DEVICE_NAME,&TermParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Term : %d\n",Err));
        return (TRUE);
    }
    return ( Err );
}

/*-------------------------------------------------------------------------
 * Function : BLIT_BlitDemoEvtSetup
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_BlitDemoEvtSetup(STTST_Parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t      RetErr;
    STEVT_OpenParams_t  EvtOpenParams;
    STEVT_Handle_t      EvtHandle;
	ST_DeviceName_t     DeviceName;

	/*------------ get device name ------------*/
    RetErr = STTST_GetString( pars_p, DEFAULT_DEMO_DEVICE_NAME, DeviceName, sizeof(DeviceName) );
    if (RetErr)
    {
    	STTST_TagCurrentLine( pars_p, "Expected DeviceName" );
    	return(TRUE);
    }

    /* ------------ Open Event handler -------------- */
    RetErr = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (RetErr != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",RetErr));
        BlitDemoIsRunning  = FALSE;
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    BlitDemoEvtSubscribeParams.NotifyCallback     = BlitDemoCompletedHandler;
    BlitDemoEvtSubscribeParams.SubscriberData_p   = NULL;
    RetErr = STEVT_SubscribeDeviceEvent(EvtHandle,DeviceName,STBLIT_BLIT_COMPLETED_EVT,&BlitDemoEvtSubscribeParams);
    if (RetErr != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",RetErr));
        BlitDemoIsRunning  = FALSE;
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&BlitDemoSubscriberID);


	return (FALSE);
}




/*-------------------------------------------------------------------------
 * Function : BLIT_SetSnakeDemoTargetBitmap
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_SetSnakeDemoTargetBitmap(STTST_Parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t RetErr;
    S32            Lvar;

    /*------------ get the osd layer vp handle ------------*/
    RetErr = STTST_GetInteger(pars_p, 0, &Lvar);
    if ( RetErr )
    {
        STTST_TagCurrentLine( pars_p, "expected bitmap handle" );
        return(TRUE);
    }

	SnakeDemoTargetBitmap = *((STGXOBJ_Bitmap_t*)Lvar);

#ifdef ST_OSLINUX
	SnakeDemoTargetBitmap.Data1_p = (void *)STLAYER_KernelToUser((U32)SnakeDemoTargetBitmap.Data1_p);
#endif

	SnakeDemoParameters.TargetBitmap_p = &SnakeDemoTargetBitmap;

    return (FALSE);
}


/*-------------------------------------------------------------------------
 * Function : BLIT_Snake_Start
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_Snake_Start(STTST_Parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t ErrorCode;
    BOOL RetErr;
    ST_DeviceName_t IntTMRDeviceName;
#if defined (ST_7100)
    /*------------ init IntTMRDeviceName ------------*/
    strcpy(IntTMRDeviceName, STINTMR_DEVICE_NAME);
#endif

    if (SnakeIsRunning == FALSE)
    {
        /*------------ demo parameters ------------*/
		ST_DeviceName_t DeviceName;
		S32 InitMode;
		S32 Width, Height;
        S32 OffsetX ,OffsetY;
        S32 Complexity;
        S32 Alpha;
        S32 NbrObstacles;
		S32 Lvar;

        /*------------ get the Init mode ------------*/
        RetErr = STTST_GetInteger( pars_p, DEFAULT_DEMO_INIT_MODE, &InitMode );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected InitMode" );
            return(TRUE);
        }
        SnakeDemoParameters.InitMode = InitMode;

		/*------------ get device name ------------*/
    	RetErr = STTST_GetString( pars_p, DEFAULT_DEMO_DEVICE_NAME, DeviceName, sizeof(DeviceName) );
    	if (RetErr)
    	{
    	    STTST_TagCurrentLine( pars_p, "Expected DeviceName" );
    	    return(TRUE);
    	}
		strcpy(SnakeDemoParameters.DeviceName,DeviceName);
        RetErr = STTST_GetInteger( pars_p, DEFAULT_DEMO_OFFSETX, &OffsetX );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected OffsetX" );
            return(TRUE);
        }
        SnakeDemoParameters.OffsetX = OffsetX;


        /*------------ get the demo OffsetY ------------*/
        RetErr = STTST_GetInteger( pars_p, DEFAULT_DEMO_OFFSETY, &OffsetY );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected OffsetY" );
            return(TRUE);
        }
        SnakeDemoParameters.OffsetY = OffsetY;


        /*------------ get the demo Width ------------*/
        RetErr = STTST_GetInteger( pars_p, DEFAULT_DEMO_WIDTH, &Width );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected Width" );
            return(TRUE);
        }
        SnakeDemoParameters.Width = Width;

        /*------------ get the demo Height ------------*/
        RetErr = STTST_GetInteger( pars_p, DEFAULT_DEMO_HEIGHT, &Height );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected Height" );
            return(TRUE);
        }
        SnakeDemoParameters.Height = Height;

        /*------------ get the demo Complexity ------------*/
        RetErr = STTST_GetInteger( pars_p, DEFAULT_DEMO_COMPLEXITY, &Complexity );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected Complexity" );
            return(TRUE);
        }
        SnakeDemoParameters.Complexity = Complexity;

        /*------------ get the demo Alpha ------------*/
        RetErr = STTST_GetInteger( pars_p, DEFAULT_DEMO_ALPHA, &Alpha );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected Alpha" );
            return(TRUE);
        }
        SnakeDemoParameters.Alpha = Alpha;


        /*------------ get the demo NbrObstacles ------------*/
        RetErr = STTST_GetInteger( pars_p, DEFAULT_DEMO_NBR_OBSTACLES, &NbrObstacles );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected NbrObstacles" );
            return(TRUE);
        }
        SnakeDemoParameters.NbrObstacles = NbrObstacles;

        /*------------ get the Blit Mode: Synchrone mode(wait for Blit notification) if Lvar=1, Asynchrone Mode if else ------------*/
        RetErr = STTST_GetInteger( pars_p, 0, &Lvar );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected WaitForBlitCompletetion" );
            return(TRUE);
        }
        WaitForBlitCompletetion = Lvar;

        STTBX_Print(("Snake start(InitMode=%d,DeviceName=%s,TargetBitmap.Data1_p=0x%x,OffsetX=%d,OffsetY=%d,Width=%d,Height=%d,Complexity=%d,Alpha=%d,WaitForBlitCompletetion=%d): Ok\n",
		              InitMode,DeviceName,SnakeDemoParameters.TargetBitmap_p->Data1_p,OffsetX,OffsetY,Width,Height,Complexity,Alpha,WaitForBlitCompletetion));


        /* ------------ Allocate memory ressources for demo ------------ */
        DemoEngine_Init();

        /* ------------ Blit Device Init ------------ */
#ifdef ST_OSLINUX
        ErrorCode = DemoBlit_Setup(SystemPartition_p, (STAVMEM_PartitionHandle_t)NULL,STEVT_DEVICE_NAME, IntTMRDeviceName, BlitDemoSubscriberID);
#else
        ErrorCode = DemoBlit_Setup(SystemPartition_p, AvmemPartitionHandle[AVMEM_PARTITION_NUM],STEVT_DEVICE_NAME, IntTMRDeviceName, BlitDemoSubscriberID);
#endif
        if (ErrorCode != ST_NO_ERROR)
            return (TRUE);

        /*------------ Create and run a demo task ------------*/
        if (STOS_TaskCreate ((void (*) (void*))SnakeDemo_Task,
                            (void*)NULL,
                            SystemPartition_p,
                            SM_TASK_STACK_SIZE,
                            &InterruptProcessTaskStack,
                            SystemPartition_p,
                            &InterruptProcessTask_p,
                            &InterruptProcessTaskDesc,
                            SM_TASK_PRIORITY,
                            "SNAKE_DEMO",
                            0 /*task_flags_no_min_stack_size*/ ) != ST_NO_ERROR)
        {
            return(ST_ERROR_BAD_PARAMETER);
        }



    }

    else  /* Snake is started*/
    {
        STTBX_Print(("Snake demo is already running.\n"));
    }
        return ( ErrorCode );
}

/*-------------------------------------------------------------------------
 * Function   : SnakeDemo_Task
 * Input      :
 * Output     :
 * Return     :
 * Description:
 * ----------------------------------------------------------------------*/
static void SnakeDemo_Task(void *Param_p)
{
    BOOL TargetReached;
    ST_ErrorCode_t ErrorCode = ST_NO_ERROR;

    ErrorCode = DemoDraw_ClearOSDBitmap(SnakeDemoParameters.TargetBitmap_p);
    if (ErrorCode != ST_NO_ERROR)
    {
        return;
    }
    SnakeIsRunning  = TRUE;
    while ( (SnakeIsRunning  == TRUE) && (ErrorCode == ST_NO_ERROR) )
    {
        /*------------ Initialize a target position ------------*/
        DemoEngine_InitTarget();

        /*------------ Target is not yet reached ------------*/
        TargetReached = FALSE;

        while ( (TargetReached  == FALSE) && (SnakeIsRunning  == TRUE) && (ErrorCode == ST_NO_ERROR) )
        {
            /*------------ Blit all matrix cells ------------*/
            ErrorCode = DemoDraw_DrawMatrix(SnakeDemoParameters.TargetBitmap_p);

#ifdef ENABLE_RECCURENCE_MODE_TEST
            /*--------- Call STLAYER API to compose the OSD bitmap since draw operation is terminated -----------*/
            ErrorCode = STLAYER_PerformViewPortComposition(OSDVPHandle);
#endif

            /*--------- Calculate the snake next cell -----------*/
            TargetReached = DemoEngine_MoveSnake();

        }/*while*/
    } /*while*/

    /*------------ Deallocate memory ressources for demo ------------*/
    DemoEngine_Term();

    /* ------------ Term the Blit handle ------------*/
    DemoBlit_Term();
    return;
}

/*-------------------------------------------------------------------------
 * Function : BLI_EndDemo
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_Snake_End(STTST_Parse_t *pars_p, char *result_sym_p)
{
    if (SnakeIsRunning == TRUE)
    {
        SnakeIsRunning = FALSE;
        /*Snake demo is finished*/
        STTBX_Print(("Snake Term( Snake demo terminated ): Ok\n"));
    }
    else
    {
        STTBX_Print(("Snake demo is not yet started!\n"));
    }
    return true;
}


/*-------------------------------------------------------------------------
 * Function : BLIT_SetBlitDemoTargetBitmap
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_SetBlitDemoTargetBitmap(STTST_Parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t RetErr;
    S32            Lvar;

    /*------------ get the osd layer vp handle ------------*/
    RetErr = STTST_GetInteger(pars_p, 0, &Lvar);
    if ( RetErr )
    {
        STTST_TagCurrentLine( pars_p, "expected bitmap handle" );
        return(TRUE);
    }

	BlitDemoParameters.TargetBitmap = *((STGXOBJ_Bitmap_t*)Lvar);

#ifdef ST_OSLINUX
	BlitDemoParameters.TargetBitmap.Data1_p = (void *)STLAYER_KernelToUser((U32)BlitDemoParameters.TargetBitmap.Data1_p);
#endif

    return (FALSE);
}


/*-------------------------------------------------------------------------
 * Function : BLIT_BlitDemo_Start
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_BlitDemo_Start(STTST_Parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t ErrorCode;
    BOOL RetErr;

    if (BlitDemoIsRunning == FALSE)
    {
        /*------------ demo parameters ------------*/
		ST_DeviceName_t DeviceName;
		S32 InitMode;
		S32 Width, Height;
        S32 OffsetX ,OffsetY;
        S32 Step;
		char SourcePath[100];

        /*------------ get the Init mode ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_INIT_MODE, &InitMode );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected InitMode" );
            return(TRUE);
        }
        BlitDemoParameters.InitMode = InitMode;

		/*------------ get device name ------------*/
    	RetErr = STTST_GetString( pars_p, BLIT_DEMO_DEFAULT_DEVICE_NAME, DeviceName, sizeof(DeviceName) );
    	if (RetErr)
    	{
    	    STTST_TagCurrentLine( pars_p, "Expected DeviceName" );
    	    return(TRUE);
    	}
		strcpy(BlitDemoParameters.DeviceName,DeviceName);

		/*------------ get device name ------------*/
    	RetErr = STTST_GetString( pars_p, BLIT_DEMO_DEFAULT_SOURCE_PATH, SourcePath, sizeof(SourcePath) );
    	if (RetErr)
    	{
    	    STTST_TagCurrentLine( pars_p, "Expected SourcePath" );
    	    return(TRUE);
    	}
		strcpy(BlitDemoParameters.SourcePath,SourcePath);

        /*------------ get the demo OffsetX ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_OFFSETX, &OffsetX );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected OffsetX" );
            return(TRUE);
        }
        BlitDemoParameters.OffsetX = OffsetX;


        /*------------ get the demo OffsetY ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_OFFSETY, &OffsetY );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected OffsetY" );
            return(TRUE);
        }
        BlitDemoParameters.OffsetY = OffsetY;


        /*------------ get the demo Width ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_WIDTH, &Width );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected Width" );
            return(TRUE);
        }
        BlitDemoParameters.Width = Width;

        /*------------ get the demo Height ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_HEIGHT, &Height );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected Height" );
            return(TRUE);
        }
        BlitDemoParameters.Height = Height;

        /*------------ get the demo step ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_STEP, &Step );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected Complexity" );
            return(TRUE);
        }
        BlitDemoParameters.Step = Step;

    	/* ------------ Set Src ------------ */
    	BlitDemoParameters.SourceBitmap.Data1_p = NULL;
    	BlitDemoParameters.Palette.Data_p = NULL;
	#ifdef ST_OSLINUX
    	RetErr = ConvertGammaToBitmap(BlitDemoParameters.SourcePath,SECURED_SRC_BITMAP,&BlitDemoParameters.SourceBitmap,&BlitDemoParameters.Palette);
	#else
    	RetErr = ConvertGammaToBitmap(BlitDemoParameters.SourcePath,SRC_BITMAP_AVMEM_PARTITION_NUM,&BlitDemoParameters.SourceBitmap,&BlitDemoParameters.Palette);
	#endif
    	if (RetErr != ST_NO_ERROR)
    	{
    	    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",RetErr));
    	    BlitDemoIsRunning  = FALSE;
    	}



        STTBX_Print(("Blit Demo start(InitMode=%d,DeviceName=%s,TargetBitmap.Data1_p=0x%x,OffsetX=%d,OffsetY=%d,Width=%d,Height=%d,Step=%d): Ok\n",
		              InitMode,DeviceName,BlitDemoParameters.TargetBitmap.Data1_p,OffsetX,OffsetY,Width,Height,Step));

        /*------------ Create and run a demo task ------------*/
        if (STOS_TaskCreate ((void (*) (void*))BlitDemo_Task,
                            (void*)NULL,
                            SystemPartition_p,
                            SM_TASK_STACK_SIZE,
                            &BlitDemo_InterruptProcessTaskStack,
                            SystemPartition_p,
                            &BlitDemo_InterruptProcessTask_p,
                            &BlitDemo_InterruptProcessTaskDesc,
                            SM_TASK_PRIORITY,
                            "BLIT_DEMO",
                            0 /*task_flags_no_min_stack_size*/ ) != ST_NO_ERROR)
        {
            return(ST_ERROR_BAD_PARAMETER);
        }
    }

    else  /* Snake is started*/
    {
        STTBX_Print(("Blit demo is already running.\n"));
    }
        return ( ErrorCode );
}

/*-------------------------------------------------------------------------
 * Function   : BlitDemo_Task
 * Input      :
 * Output     :
 * Return     :
 * Description:
 * ----------------------------------------------------------------------*/
static void BlitDemo_Task(void *Param_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         DeviceName="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;


    STGXOBJ_Rectangle_t     Rectangle, ClearRectangle;
	STGXOBJ_Color_t         ClearColor, FillColor;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

	U32                     i,j;

#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif

	/*STTBX_Print(("-- Init Blit demo\n"));*/

	/* ------------ Init BlitDemoIsRunning ------------ */
	BlitDemoIsRunning  = TRUE;

    /* ------------ Blit Device Init ------------ */
	if (BlitDemoParameters.InitMode == 0)
    {
		InitParams.CPUPartition_p                       = SystemPartition_p;
    	InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    	InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
	#ifdef ST_OSLINUX
    	InitParams.AVMEMPartition                       = (STAVMEM_PartitionHandle_t)NULL;
	#else
    	InitParams.AVMEMPartition                       = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
	#endif
    	InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    	InitParams.MaxHandles                           = 1;

    	InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
	#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188)|| defined (ST_5525)\
 	|| defined (ST_5107)
    	InitParams.SingleBlitNodeMaxNumber              = 1000;
	#else
    	InitParams.SingleBlitNodeMaxNumber              = 30;
	#endif
	/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    	InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    	InitParams.JobBlitNodeMaxNumber                 = 30;
	/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    	InitParams.JobBlitBufferUserAllocated = FALSE;
    	InitParams.JobBlitMaxNumber       = 30;
	/*    InitParams.JobBlitBuffer_p        = NULL;*/

    	InitParams.JobBufferUserAllocated               = FALSE;
    	InitParams.JobMaxNumber                         = 2;

	/*    InitParams.JobBuffer_p                        = NULL;*/

    	InitParams.WorkBufferUserAllocated              = FALSE;
	/*    InitParams.WorkBuffer_p                       = NULL;*/
    	InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    	strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

	#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
	#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 	|| defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107)
    	InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    	InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
	#else
    	InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    	strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
	#endif
	#endif

	#ifdef ST_OSLINUX
    	layer_init();
	#endif

		/* ------------ Blit Init ------------ */
		Err = STBLIT_Init(DeviceName,&InitParams);
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
    	    BlitDemoIsRunning  = FALSE;
    	}

    	/* ------------ Open Event handler -------------- */
    	Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
    	    BlitDemoIsRunning  = FALSE;
    	}

    	/* ------------ Subscribe to Blit Completed event ---------------- */
    	EvtSubscribeParams.NotifyCallback     = BlitDemoCompletedHandler;
    	EvtSubscribeParams.SubscriberData_p   = NULL;
    	Err = STEVT_SubscribeDeviceEvent(EvtHandle,DeviceName,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
    	    BlitDemoIsRunning  = FALSE;
    	}

    	/* ------------ Get Subscriber ID ------------ */
    	STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
	}
	else  /* (BlitDemoParameters.InitMode == 1) */
	{
    	strcpy(DeviceName, BlitDemoParameters.DeviceName);
		SubscriberID = BlitDemoSubscriberID;
	} /* (BlitDemoParameters.InitMode == 0) */


	/* ------------ Blit Open ------------ */
    Err = STBLIT_Open(DeviceName,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
    	STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
    	BlitDemoIsRunning  = FALSE;
    }

    /* ------------ Initialize global semaphores ------------ */
    DemoBlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

	/* ------------ Set Subscriber ID ------------ */
	Context.EventSubscriberID   = SubscriberID;


	Context.UserTag_p   = BlitDemoTagString;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    /*Context.EnableClipRectangle     = TRUE;*/
    Context.ClipRectangle           = Rectangle;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.EnableResizeFilter      = TRUE;

	/* ------------ Set ClearColor ------------ */
    if(BlitDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888;
        ClearColor.Value.ARGB8888.Alpha = 0xFF;
        ClearColor.Value.ARGB8888.R     = 0x50;
        ClearColor.Value.ARGB8888.G     = 0x50;
        ClearColor.Value.ARGB8888.B     = 0x50;

        FillColor.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888;
        FillColor.Value.ARGB8888.Alpha = 0xFF;
        FillColor.Value.ARGB8888.R     = 0xFF;
        FillColor.Value.ARGB8888.G     = 0x00;
        FillColor.Value.ARGB8888.B     = 0x00;
    }

    if(BlitDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565 )
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;
        ClearColor.Value.RGB565.R       = 0x50 ;
        ClearColor.Value.RGB565.G       = 0x50 ;
        ClearColor.Value.RGB565.B       = 0x50 ;

        FillColor.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;
        FillColor.Value.RGB565.R       = 0xFF ;
        FillColor.Value.RGB565.G       = 0x00 ;
        FillColor.Value.RGB565.B       = 0x00 ;
     }

    if(BlitDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB888 )
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_RGB888 ;
        ClearColor.Value.RGB888.R       = 0x50 ;
        ClearColor.Value.RGB888.G       = 0x50 ;
        ClearColor.Value.RGB888.B       = 0x50 ;

        FillColor.Type                 = STGXOBJ_COLOR_TYPE_RGB888 ;
        FillColor.Value.RGB888.R       = 0xFF ;
        FillColor.Value.RGB888.G       = 0x00 ;
        FillColor.Value.RGB888.B       = 0x00 ;
    }

    if(BlitDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_CLUT8 )
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_CLUT8 ;
        ClearColor.Value.CLUT8          = 0x50 ;

        FillColor.Type                 = STGXOBJ_COLOR_TYPE_CLUT8 ;
        FillColor.Value.CLUT8          = 0x00 ;
    }


    /* ------------ Set ClearRectangle ------------ */
    ClearRectangle.PositionX = 0;
    ClearRectangle.PositionY = 0;
    ClearRectangle.Width     = BlitDemoParameters.TargetBitmap.Width;
    ClearRectangle.Height    = BlitDemoParameters.TargetBitmap.Height;

	/*STTBX_Print(("-- Start Blit demo blit operations.\n"));*/


    while ( (BlitDemoIsRunning  == TRUE) && (Err == ST_NO_ERROR) )
    {
		/*
    	 * Test 1 : Test Copy feature
    	 * ==========================
    	 */

     	/* ------------ Reset Bitmap ------------ */
    	Err = STBLIT_FillRectangle(Handle,&BlitDemoParameters.TargetBitmap,&ClearRectangle,&ClearColor,&Context );
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Print(("Err Blit Fill : %d\n",Err));
    	    BlitDemoIsRunning  = FALSE;
    	}

    	/* ------------ Wait Blit Complit ------------ */
#ifdef ST_OSLINUX
        time = time_plus(time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
#else
        time = time_plus(time_now(), ST_GetClocksPerSecond()*5);
#endif

        if(STOS_SemaphoreWaitTimeOut(DemoBlitCompletionSemaphore_p,&time)!=0)
        {
        	STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        	ResetEngine();
        }

    	for ( j=BlitDemoParameters.OffsetY;j<(BlitDemoParameters.Height + BlitDemoParameters.OffsetY);j+=BlitDemoParameters.Step)
    	{
    	    for ( i=BlitDemoParameters.OffsetX;i<(BlitDemoParameters.Width + BlitDemoParameters.OffsetX);i+=BlitDemoParameters.Step)
    	    {
    	        /* ------------ Set Rectangle ------------ */
    	        Rectangle.PositionX = i;
    	        Rectangle.PositionY = j;
    	        Rectangle.Width     = BlitDemoParameters.Step;
    	        Rectangle.Height    = BlitDemoParameters.Step;

				/* ------------ Blit ------------ */
    	        Err = STBLIT_CopyRectangle(Handle,&BlitDemoParameters.SourceBitmap,&Rectangle,&BlitDemoParameters.TargetBitmap,Rectangle.PositionX,Rectangle.PositionY,&Context );
    	        if (Err != ST_NO_ERROR)
    	        {
    	            STTBX_Print(("Rectangle=(%d,%d,%d,%d) STBLIT_CopyRectangle(...)=%d",Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height,Err));
    	            BlitDemoIsRunning  = FALSE;
    	        }

    	        /* ------------ Wait Blit Complit ------------ */
        #ifdef ST_OSLINUX
        		time = time_plus(time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
        		time = time_plus(time_now(), ST_GetClocksPerSecond()*5);
        #endif

        		if(STOS_SemaphoreWaitTimeOut(DemoBlitCompletionSemaphore_p,&time)!=0)
        		{
        		    STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        		    ResetEngine();
        		}
    	    }
    	}

    	/*
    	 * Test 2 : Test Fill feature
    	 * ==========================
    	 */

		/* ------------ Reset Bitmap ------------ */
    	Err = STBLIT_FillRectangle(Handle,&BlitDemoParameters.TargetBitmap,&ClearRectangle,&ClearColor,&Context );
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Print(("Err Blit Fill : %d\n",Err));
    	    BlitDemoIsRunning  = FALSE;
    	}

    	/* ------------ Wait Blit Complit ------------ */
#ifdef ST_OSLINUX
        time = time_plus(time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
#else
        time = time_plus(time_now(), ST_GetClocksPerSecond()*5);
#endif

        if(STOS_SemaphoreWaitTimeOut(DemoBlitCompletionSemaphore_p,&time)!=0)
        {
        	STTBX_Print(("Reset TIMEOUT"));
        	ResetEngine();
        }

    	for ( j=BlitDemoParameters.OffsetY;j<(BlitDemoParameters.Height + BlitDemoParameters.OffsetY);j+=BlitDemoParameters.Step)
    	{
    	    for ( i=BlitDemoParameters.OffsetX;i<(BlitDemoParameters.Width + BlitDemoParameters.OffsetX);i+=BlitDemoParameters.Step)
    	    {
    	        /* ------------ Set Rectangle ------------ */
    	        Rectangle.PositionX = i;
    	        Rectangle.PositionY = j;
    	        Rectangle.Width     = BlitDemoParameters.Step;
    	        Rectangle.Height    = BlitDemoParameters.Step;

				/* ------------ Set ClearColor ------------ */
    			if(BlitDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    			{
    			    FillColor.Value.ARGB8888.R -= i;
    			    FillColor.Value.ARGB8888.G += i;
    			    FillColor.Value.ARGB8888.B += j;
    			}

    			if(BlitDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565 )
    			{
    			    FillColor.Value.RGB565.R -= i;
    			    FillColor.Value.RGB565.G += i;
    			    FillColor.Value.RGB565.B += j;
    			 }

    			if(BlitDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB888 )
    			{
    			    FillColor.Value.RGB888.R -= i;
    			    FillColor.Value.RGB888.G += i;
    			    FillColor.Value.RGB888.B += j;
    			}

    			if(BlitDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_CLUT8 )
    			{
    			    FillColor.Value.CLUT8    += (i+j) ;
    			}


				/* ------------ Blit ------------ */
				Err = STBLIT_FillRectangle(Handle,&BlitDemoParameters.TargetBitmap,&Rectangle,&FillColor,&Context );
    	        if (Err != ST_NO_ERROR)
    	        {
    	            STTBX_Print(("Rectangle=(%d,%d,%d,%d) STBLIT_FillRectangle(...)=%d",Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height,Err));
    	            BlitDemoIsRunning  = FALSE;
    	        }

    	        /* ------------ Wait Blit Complit ------------ */
        #ifdef ST_OSLINUX
        		time = time_plus(time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
        		time = time_plus(time_now(), ST_GetClocksPerSecond()*5);
        #endif

        		if(STOS_SemaphoreWaitTimeOut(DemoBlitCompletionSemaphore_p,&time)!=0)
        		{
        		    STTBX_Print(("Fill TIMEOUT"));
        		    ResetEngine();
        		}
    	    }
    	}

		/*
		 * Test 4 : Test Resize feature
		 * ============================
		 */

		/* ------------ Reset Bitmap ------------ */
    	Err = STBLIT_FillRectangle(Handle,&BlitDemoParameters.TargetBitmap,&ClearRectangle,&ClearColor,&Context );
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Print(("Err Blit Fill : %d\n",Err));
    	    BlitDemoIsRunning  = FALSE;
    	}

    	/* ------------ Wait Blit Complit ------------ */
#ifdef ST_OSLINUX
        time = time_plus(time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
#else
        time = time_plus(time_now(), ST_GetClocksPerSecond()*5);
#endif

        if(STOS_SemaphoreWaitTimeOut(DemoBlitCompletionSemaphore_p,&time)!=0)
        {
        	STTBX_Print(("Reset TIMEOUT"));
        	BlitDemoIsRunning  = FALSE;
        }

    	/* ------------ Init Src & Dst ------------ */
    	Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    	Src.Data.Bitmap_p        = &BlitDemoParameters.SourceBitmap;
    	Src.Rectangle.PositionX  = 0;
    	Src.Rectangle.PositionY  = 0;
    	Src.Rectangle.Width      = BlitDemoParameters.SourceBitmap.Width;
    	Src.Rectangle.Height     = BlitDemoParameters.SourceBitmap.Height;
    	Src.Palette_p            = &BlitDemoParameters.Palette;

    	Dst.Rectangle.PositionX  = BlitDemoParameters.OffsetX;
    	Dst.Rectangle.PositionY  = BlitDemoParameters.OffsetY;
    	Dst.Rectangle.Height     = BlitDemoParameters.Height;
    	Dst.Bitmap_p             = &BlitDemoParameters.TargetBitmap;
    	Dst.Palette_p            = NULL;

    	for (i=10 ; i<BlitDemoParameters.Width ; i+=BlitDemoParameters.Step)
    	{
    	    /* ------------ Set Dst ------------ */
    	    Dst.Rectangle.Width       = i;

    	    /* ------------ Blit ------------ */
    	    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    	    if (Err != ST_NO_ERROR)
    	    {
    	        STTBX_Print(("Destination=(%d,%d,%d,%d) STBLIT_Blit(...)=%d",Dst.Rectangle.PositionX,Dst.Rectangle.PositionY,Dst.Rectangle.Width,Dst.Rectangle.Height,Err));
    	        BlitDemoIsRunning  = FALSE;
    	    }

    		/* ------------ Wait Blit Complit ------------ */
	#ifdef ST_OSLINUX
        	time = time_plus(time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
	#else
        	time = time_plus(time_now(), ST_GetClocksPerSecond()*5);
	#endif

        	if(STOS_SemaphoreWaitTimeOut(DemoBlitCompletionSemaphore_p,&time)!=0)
        	{
        		STTBX_Print(("Resize 1 TIMEOUT"));
        		BlitDemoIsRunning  = FALSE;
        	}
    	}

		/* ------------ Reset Bitmap ------------ */
    	Err = STBLIT_FillRectangle(Handle,&BlitDemoParameters.TargetBitmap,&ClearRectangle,&ClearColor,&Context );
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Print(("Err Blit Fill : %d\n",Err));
    	    BlitDemoIsRunning  = FALSE;
    	}

    	/* ------------ Wait Blit Complit ------------ */
#ifdef ST_OSLINUX
        time = time_plus(time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
#else
        time = time_plus(time_now(), ST_GetClocksPerSecond()*5);
#endif

        if(STOS_SemaphoreWaitTimeOut(DemoBlitCompletionSemaphore_p,&time)!=0)
        {
        	STTBX_Print(("Reset TIMEOUT"));
        	BlitDemoIsRunning  = FALSE;
        }

    	/* ------------ Init Src & Dst ------------ */
    	Dst.Rectangle.Width      = BlitDemoParameters.Width;

    	for (i=10 ; i<BlitDemoParameters.Height ; i+=BlitDemoParameters.Step)
    	{
    	    /* ------------ Set Dst ------------ */
    	    Dst.Rectangle.Height       = i;

    	    /* ------------ Blit ------------ */
    	    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    	    if (Err != ST_NO_ERROR)
    	    {
    	        STTBX_Print(("Destination=(%d,%d,%d,%d) STBLIT_Blit(...)=%d",Dst.Rectangle.PositionX,Dst.Rectangle.PositionY,Dst.Rectangle.Width,Dst.Rectangle.Height,Err));
    	        BlitDemoIsRunning  = FALSE;
    	    }

    		/* ------------ Wait Blit Complit ------------ */
	#ifdef ST_OSLINUX
        	time = time_plus(time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
	#else
        	time = time_plus(time_now(), ST_GetClocksPerSecond()*5);
	#endif

        	if(STOS_SemaphoreWaitTimeOut(DemoBlitCompletionSemaphore_p,&time)!=0)
        	{
        		STTBX_Print(("Resize 1 TIMEOUT"));
        		BlitDemoIsRunning  = FALSE;
        	}
    	}





    } /*while*/

    return;
}

/*-------------------------------------------------------------------------
 * Function : BLIT_SetRandomFillDemoTargetBitmap
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_SetRandomFillDemoTargetBitmap(STTST_Parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t RetErr;
    S32            Lvar;

    /*------------ get the osd layer vp handle ------------*/
    RetErr = STTST_GetInteger(pars_p, 0, &Lvar);
    if ( RetErr )
    {
        STTST_TagCurrentLine( pars_p, "expected bitmap handle" );
        return(TRUE);
    }

	RandomFillDemoParameters.TargetBitmap = *((STGXOBJ_Bitmap_t*)Lvar);

#ifdef ST_OSLINUX
	RandomFillDemoParameters.TargetBitmap.Data1_p = (void *)STLAYER_KernelToUser((U32)RandomFillDemoParameters.TargetBitmap.Data1_p);
#endif

    return (FALSE);
}

/*-------------------------------------------------------------------------
 * Function : BLIT_RandomFillDemo_Start
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_RandomFillDemo_Start(STTST_Parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t ErrorCode;
    BOOL RetErr;

    if (RandomFillDemoIsRunning == FALSE)
    {
        /*------------ demo parameters ------------*/
		ST_DeviceName_t DeviceName;
		S32 InitMode;
		S32 Width, Height;
        S32 OffsetX ,OffsetY;
        S32 Step;

        /*------------ get the Init mode ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_INIT_MODE, &InitMode );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected InitMode" );
            return(TRUE);
        }
        RandomFillDemoParameters.InitMode = InitMode;

		/*------------ get device name ------------*/
    	RetErr = STTST_GetString( pars_p, BLIT_DEMO_DEFAULT_DEVICE_NAME, DeviceName, sizeof(DeviceName) );
    	if (RetErr)
    	{
    	    STTST_TagCurrentLine( pars_p, "Expected DeviceName" );
    	    return(TRUE);
    	}
		strcpy(RandomFillDemoParameters.DeviceName,DeviceName);

        /*------------ get the demo OffsetX ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_OFFSETX, &OffsetX );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected OffsetX" );
            return(TRUE);
        }
        RandomFillDemoParameters.OffsetX = OffsetX;


        /*------------ get the demo OffsetY ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_OFFSETY, &OffsetY );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected OffsetY" );
            return(TRUE);
        }
        RandomFillDemoParameters.OffsetY = OffsetY;


        /*------------ get the demo Width ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_WIDTH, &Width );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected Width" );
            return(TRUE);
        }
        RandomFillDemoParameters.Width = Width;

        /*------------ get the demo Height ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_HEIGHT, &Height );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected Height" );
            return(TRUE);
        }
        RandomFillDemoParameters.Height = Height;

        /*------------ get the demo step ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_STEP, &Step );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected Complexity" );
            return(TRUE);
        }
        RandomFillDemoParameters.Step = Step;


        STTBX_Print(("Random Fill Demo start(InitMode=%d,DeviceName=%s,TargetBitmap.Data1_p=0x%x,OffsetX=%d,OffsetY=%d,Width=%d,Height=%d,Step=%d): Ok\n",
		              InitMode,DeviceName,RandomFillDemoParameters.TargetBitmap.Data1_p,OffsetX,OffsetY,Width,Height,Step));

        /*------------ Create and run a demo task ------------*/
        if (STOS_TaskCreate ((void (*) (void*))RandomFillDemo_Task,
                            (void*)NULL,
                            SystemPartition_p,
                            SM_TASK_STACK_SIZE,
                            &RandomFillDemo_InterruptProcessTaskStack,
                            SystemPartition_p,
                            &RandomFillDemo_InterruptProcessTask_p,
                            &RandomFillDemo_InterruptProcessTaskDesc,
                            SM_TASK_PRIORITY,
                            "RANDOM_Fill_DEMO",
                            0 /*task_flags_no_min_stack_size*/ ) != ST_NO_ERROR)
        {
            return(ST_ERROR_BAD_PARAMETER);
        }
    }

    else  /* Snake is started*/
    {
        STTBX_Print(("RandomFill demo is already running.\n"));
    }
        return ( ErrorCode );
}

/*-------------------------------------------------------------------------
 * Function   : RandomFillDemo_Task
 * Input      :
 * Output     :
 * Return     :
 * Description:
 * ----------------------------------------------------------------------*/
static void RandomFillDemo_Task(void *Param_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         DeviceName="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;

    STGXOBJ_Rectangle_t     Rectangle, ClearRectangle;
	STGXOBJ_Color_t         ClearColor, FillColor;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif

	/*STTBX_Print(("-- Init Random Fill demo\n"));*/

	/* ------------ Init BlitDemoIsRunning ------------ */
	RandomFillDemoIsRunning  = TRUE;

    /* ------------ Blit Device Init ------------ */
	if (RandomFillDemoParameters.InitMode == 0)
    {
		InitParams.CPUPartition_p                       = SystemPartition_p;
    	InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    	InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
	#ifdef ST_OSLINUX
    	InitParams.AVMEMPartition                       = (STAVMEM_PartitionHandle_t)NULL;
	#else
    	InitParams.AVMEMPartition                       = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
	#endif
    	InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    	InitParams.MaxHandles                           = 1;

    	InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
	#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188)|| defined (ST_5525)\
 	|| defined (ST_5107)
    	InitParams.SingleBlitNodeMaxNumber              = 1000;
	#else
    	InitParams.SingleBlitNodeMaxNumber              = 30;
	#endif
	/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    	InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    	InitParams.JobBlitNodeMaxNumber                 = 30;
	/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    	InitParams.JobBlitBufferUserAllocated = FALSE;
    	InitParams.JobBlitMaxNumber       = 30;
	/*    InitParams.JobBlitBuffer_p        = NULL;*/

    	InitParams.JobBufferUserAllocated               = FALSE;
    	InitParams.JobMaxNumber                         = 2;

	/*    InitParams.JobBuffer_p                        = NULL;*/

    	InitParams.WorkBufferUserAllocated              = FALSE;
	/*    InitParams.WorkBuffer_p                       = NULL;*/
    	InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    	strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

	#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
	#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 	|| defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107)
    	InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    	InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
	#else
    	InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    	strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
	#endif
	#endif

	#ifdef ST_OSLINUX
    	layer_init();
	#endif

		/* ------------ Blit Init ------------ */
		Err = STBLIT_Init(DeviceName,&InitParams);
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
    	    RandomFillDemoIsRunning  = FALSE;
    	}

    	/* ------------ Open Event handler -------------- */
    	Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
    	    RandomFillDemoIsRunning  = FALSE;
    	}

    	/* ------------ Subscribe to Blit Completed event ---------------- */
    	EvtSubscribeParams.NotifyCallback     = BlitDemoCompletedHandler;
    	EvtSubscribeParams.SubscriberData_p   = NULL;
    	Err = STEVT_SubscribeDeviceEvent(EvtHandle,DeviceName,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
    	    RandomFillDemoIsRunning  = FALSE;
    	}

    	/* ------------ Get Subscriber ID ------------ */
    	STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
	}
	else  /* (BlitDemoParameters.InitMode == 1) */
	{
    	strcpy(DeviceName, RandomFillDemoParameters.DeviceName);
		SubscriberID = BlitDemoSubscriberID;
	} /* (BlitDemoParameters.InitMode == 0) */


	/* ------------ Blit Open ------------ */
    Err = STBLIT_Open(DeviceName,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
    	STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
    	RandomFillDemoIsRunning  = FALSE;
    }

    /* ------------ Initialize global semaphores ------------ */
    RandomFillDemoCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

	/* ------------ Set Subscriber ID ------------ */
	Context.EventSubscriberID   = SubscriberID;


	Context.UserTag_p   = RandomFillDemoTagString;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    /*Context.EnableClipRectangle     = TRUE;*/
    Context.ClipRectangle           = Rectangle;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.EnableResizeFilter      = TRUE;

	/* ------------ Set ClearColor ------------ */
    if(RandomFillDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888;
        ClearColor.Value.ARGB8888.Alpha = 0xFF;
        ClearColor.Value.ARGB8888.R     = 0x00;
        ClearColor.Value.ARGB8888.G     = 0x00;
        ClearColor.Value.ARGB8888.B     = 0xFF;

        FillColor.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888;
        FillColor.Value.ARGB8888.Alpha = 0xFF;
    }

    if(RandomFillDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565 )
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;
        ClearColor.Value.RGB565.R       = 0x00 ;
        ClearColor.Value.RGB565.G       = 0x00 ;
        ClearColor.Value.RGB565.B       = 0xFF ;

        FillColor.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;

     }

    if(RandomFillDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB888 )
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_RGB888 ;
        ClearColor.Value.RGB888.R       = 0x00 ;
        ClearColor.Value.RGB888.G       = 0x00 ;
        ClearColor.Value.RGB888.B       = 0xFF ;

        FillColor.Type                 = STGXOBJ_COLOR_TYPE_RGB888 ;
    }

    if(RandomFillDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_CLUT8 )
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_CLUT8 ;
        ClearColor.Value.CLUT8          = 0x80 ;

        FillColor.Type                 = STGXOBJ_COLOR_TYPE_CLUT8 ;
    }

    /* ------------ Set ClearRectangle ------------ */
    ClearRectangle.PositionX = 0;
    ClearRectangle.PositionY = 0;
    ClearRectangle.Width     = RandomFillDemoParameters.TargetBitmap.Width;
    ClearRectangle.Height    = RandomFillDemoParameters.TargetBitmap.Height;

	/*STTBX_Print(("-- Start Random Fill demo blit operations.\n"));*/

    while ( (RandomFillDemoIsRunning  == TRUE) && (Err == ST_NO_ERROR) )
    {
    	/* ------------ Reset Bitmap ------------ */

#ifdef AAA
        Err = STBLIT_FillRectangle(Handle,&RandomFillDemoParameters.TargetBitmap,&ClearRectangle,&ClearColor,&Context );
    	if (Err != ST_NO_ERROR)
    	{
    		STTBX_Print(("Err Blit Fill : %d\n",Err));
    		RandomFillDemoIsRunning  = FALSE;
    	}

    	/* ------------ Wait Blit Complit ------------ */
	#ifdef ST_OSLINUX
    	time = time_plus(time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
	#else
    	time = time_plus(time_now(), ST_GetClocksPerSecond()*5);
	#endif

    	if(STOS_SemaphoreWaitTimeOut(RandomFillDemoCompletionSemaphore_p,&time)!=0)
    	{
    	    STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
    	    ResetEngine();
    	}
#endif
		/* ------------ Set Rectangle ------------ */
    	Rectangle.PositionX = BlitDemo_Random(RandomFillDemoParameters.OffsetX, (RandomFillDemoParameters.Width + RandomFillDemoParameters.OffsetX));
    	Rectangle.PositionY = BlitDemo_Random(RandomFillDemoParameters.OffsetY, (RandomFillDemoParameters.Height + RandomFillDemoParameters.OffsetY));
    	Rectangle.Width     = BlitDemo_Random(2, (RandomFillDemoParameters.Width + RandomFillDemoParameters.OffsetX - Rectangle.PositionX));
    	Rectangle.Height    = BlitDemo_Random(2, (RandomFillDemoParameters.Height + RandomFillDemoParameters.OffsetY - Rectangle.PositionY));

		/* ------------ Set FillColor ------------ */
    	if(RandomFillDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    	{
    	    FillColor.Value.ARGB8888.R     = BlitDemo_Random(0,255);
    	    FillColor.Value.ARGB8888.G     = BlitDemo_Random(0,255);
    	    FillColor.Value.ARGB8888.B     = BlitDemo_Random(0,255);
    	}

    	if(RandomFillDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565 )
    	{
    	    FillColor.Value.RGB565.R       = BlitDemo_Random(0,31);
    	    FillColor.Value.RGB565.G       = BlitDemo_Random(0,63);
    	    FillColor.Value.RGB565.B       = BlitDemo_Random(0,31);
    	 }

    	if(RandomFillDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB888 )
    	{
    	    FillColor.Value.RGB888.R       = BlitDemo_Random(0,255);
    	    FillColor.Value.RGB888.G       = BlitDemo_Random(0,255);
    	    FillColor.Value.RGB888.B       = BlitDemo_Random(0,255);
    	}

    	if(RandomFillDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_CLUT8 )
    	{
    	    FillColor.Value.CLUT8          = BlitDemo_Random(0,255);
    	}

		/* ------------ Blit ------------ */
		Err = STBLIT_FillRectangle(Handle,&RandomFillDemoParameters.TargetBitmap,&Rectangle,&FillColor,&Context );
    	if (Err != ST_NO_ERROR)
    	{
			STTBX_Print(("Rectangle=(%d,%d,%d,%d) STBLIT_FillRectangle(...)=%d",Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height,Err));
    	    RandomFillDemoIsRunning  = FALSE;
    	}

    	        /* ------------ Wait Blit Complit ------------ */
#ifdef ST_OSLINUX
        time = time_plus(time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
#else
        time = time_plus(time_now(), ST_GetClocksPerSecond()*5);
#endif

        if(STOS_SemaphoreWaitTimeOut(RandomFillDemoCompletionSemaphore_p,&time)!=0)
        {
        	STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
        	ResetEngine();
        }

    } /*while*/

    return;
}

/*-------------------------------------------------------------------------
 * Function : BLIT_SetRandomCopyDemoTargetBitmap
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_SetRandomCopyDemoTargetBitmap(STTST_Parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t RetErr;
    S32            Lvar;

    /*------------ get the osd layer vp handle ------------*/
    RetErr = STTST_GetInteger(pars_p, 0, &Lvar);
    if ( RetErr )
    {
        STTST_TagCurrentLine( pars_p, "expected bitmap handle" );
        return(TRUE);
    }

	RandomCopyDemoParameters.TargetBitmap = *((STGXOBJ_Bitmap_t*)Lvar);

#ifdef ST_OSLINUX
	RandomCopyDemoParameters.TargetBitmap.Data1_p = (void *)STLAYER_KernelToUser((U32)RandomCopyDemoParameters.TargetBitmap.Data1_p);
#endif

    return (FALSE);
}


/*-------------------------------------------------------------------------
 * Function : BLIT_RandomCopyDemo_Start
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_RandomCopyDemo_Start(STTST_Parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t ErrorCode;
    BOOL RetErr;

    if (RandomCopyDemoIsRunning == FALSE)
    {
        /*------------ demo parameters ------------*/
		ST_DeviceName_t DeviceName;
		S32 InitMode;
		S32 Width, Height;
        S32 OffsetX ,OffsetY;
        S32 Step;
		char SourcePath[100];

        /*------------ get the Init mode ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_INIT_MODE, &InitMode );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected InitMode" );
            return(TRUE);
        }
        RandomCopyDemoParameters.InitMode = InitMode;

		/*------------ get device name ------------*/
    	RetErr = STTST_GetString( pars_p, BLIT_DEMO_DEFAULT_DEVICE_NAME, DeviceName, sizeof(DeviceName) );
    	if (RetErr)
    	{
    	    STTST_TagCurrentLine( pars_p, "Expected DeviceName" );
    	    return(TRUE);
    	}
		strcpy(RandomCopyDemoParameters.DeviceName,DeviceName);

		/*------------ get device name ------------*/
    	RetErr = STTST_GetString( pars_p, BLIT_DEMO_DEFAULT_SOURCE_PATH, SourcePath, sizeof(SourcePath) );
    	if (RetErr)
    	{
    	    STTST_TagCurrentLine( pars_p, "Expected SourcePath" );
    	    return(TRUE);
    	}
		strcpy(RandomCopyDemoParameters.SourcePath,SourcePath);

		/*------------ get the demo OffsetX ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_OFFSETX, &OffsetX );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected OffsetX" );
            return(TRUE);
        }
        RandomCopyDemoParameters.OffsetX = OffsetX;


        /*------------ get the demo OffsetY ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_OFFSETY, &OffsetY );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected OffsetY" );
            return(TRUE);
        }
        RandomCopyDemoParameters.OffsetY = OffsetY;


        /*------------ get the demo Width ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_WIDTH, &Width );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected Width" );
            return(TRUE);
        }
        RandomCopyDemoParameters.Width = Width;

        /*------------ get the demo Height ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_HEIGHT, &Height );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected Height" );
            return(TRUE);
        }
        RandomCopyDemoParameters.Height = Height;

        /*------------ get the demo step ------------*/
        RetErr = STTST_GetInteger( pars_p, BLIT_DEMO_DEFAULT_STEP, &Step );
        if ( RetErr )
        {
            STTST_TagCurrentLine( pars_p, "expected Complexity" );
            return(TRUE);
        }
        RandomCopyDemoParameters.Step = Step;

    	/* ------------ Set Src ------------ */
    	RandomCopyDemoParameters.SourceBitmap.Data1_p = NULL;
    	RandomCopyDemoParameters.Palette.Data_p = NULL;
	#ifdef ST_OSLINUX
    	RetErr = ConvertGammaToBitmap(RandomCopyDemoParameters.SourcePath,SECURED_SRC_BITMAP,&RandomCopyDemoParameters.SourceBitmap,&RandomCopyDemoParameters.Palette);
	#else
    	RetErr = ConvertGammaToBitmap(RandomCopyDemoParameters.SourcePath,SRC_BITMAP_AVMEM_PARTITION_NUM,&RandomCopyDemoParameters.SourceBitmap,&RandomCopyDemoParameters.Palette);
	#endif
    	if (RetErr != ST_NO_ERROR)
    	{
    	    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",RetErr));
    	    BlitDemoIsRunning  = FALSE;
    	}

        STTBX_Print(("Random Copy Demo start(InitMode=%d,DeviceName=%s,TargetBitmap.Data1_p=0x%x,OffsetX=%d,OffsetY=%d,Width=%d,Height=%d,Step=%d): Ok\n",
		              InitMode,DeviceName,RandomCopyDemoParameters.TargetBitmap.Data1_p,OffsetX,OffsetY,Width,Height,Step));

        /*------------ Create and run a demo task ------------*/
        if (STOS_TaskCreate ((void (*) (void*))RandomCopyDemo_Task,
                            (void*)NULL,
                            SystemPartition_p,
                            SM_TASK_STACK_SIZE,
                            &RandomCopyDemo_InterruptProcessTaskStack,
                            SystemPartition_p,
                            &RandomCopyDemo_InterruptProcessTask_p,
                            &RandomCopyDemo_InterruptProcessTaskDesc,
                            SM_TASK_PRIORITY,
                            "RANDOM_COPY_DEMO",
                            0 /*task_flags_no_min_stack_size*/ ) != ST_NO_ERROR)
        {
            return(ST_ERROR_BAD_PARAMETER);
        }
    }

    else  /* Snake is started*/
    {
        STTBX_Print(("RandomCopy demo is already running.\n"));
    }
        return ( ErrorCode );
}

/*-------------------------------------------------------------------------
 * Function   : RandomCopyDemo_Task
 * Input      :
 * Output     :
 * Return     :
 * Description:
 * ----------------------------------------------------------------------*/
static void RandomCopyDemo_Task(void *Param_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         DeviceName="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;

    STGXOBJ_Rectangle_t     Rectangle, ClearRectangle;
	STGXOBJ_Color_t         ClearColor;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif

	/*STTBX_Print(("-- Init Random Copy demo\n"));*/

	/* ------------ Init BlitDemoIsRunning ------------ */
	RandomCopyDemoIsRunning  = TRUE;

    /* ------------ Blit Device Init ------------ */
	if (RandomCopyDemoParameters.InitMode == 0)
    {
		InitParams.CPUPartition_p                       = SystemPartition_p;
    	InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    	InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
	#ifdef ST_OSLINUX
    	InitParams.AVMEMPartition                       = (STAVMEM_PartitionHandle_t)NULL;
	#else
    	InitParams.AVMEMPartition                       = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
	#endif
    	InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    	InitParams.MaxHandles                           = 1;

    	InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
	#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188)|| defined (ST_5525)\
 	|| defined (ST_5107)
    	InitParams.SingleBlitNodeMaxNumber              = 1000;
	#else
    	InitParams.SingleBlitNodeMaxNumber              = 30;
	#endif
	/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    	InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    	InitParams.JobBlitNodeMaxNumber                 = 30;
	/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    	InitParams.JobBlitBufferUserAllocated = FALSE;
    	InitParams.JobBlitMaxNumber       = 30;
	/*    InitParams.JobBlitBuffer_p        = NULL;*/

    	InitParams.JobBufferUserAllocated               = FALSE;
    	InitParams.JobMaxNumber                         = 2;

	/*    InitParams.JobBuffer_p                        = NULL;*/

    	InitParams.WorkBufferUserAllocated              = FALSE;
	/*    InitParams.WorkBuffer_p                       = NULL;*/
    	InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    	strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

	#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
	#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 	|| defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107)
    	InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    	InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
	#else
    	InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    	strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
	#endif
	#endif

	#ifdef ST_OSLINUX
    	layer_init();
	#endif

		/* ------------ Blit Init ------------ */
		Err = STBLIT_Init(DeviceName,&InitParams);
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
    	    RandomCopyDemoIsRunning  = FALSE;
    	}

    	/* ------------ Open Event handler -------------- */
    	Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
    	    RandomCopyDemoIsRunning  = FALSE;
    	}

    	/* ------------ Subscribe to Blit Completed event ---------------- */
    	EvtSubscribeParams.NotifyCallback     = BlitDemoCompletedHandler;
    	EvtSubscribeParams.SubscriberData_p   = NULL;
    	Err = STEVT_SubscribeDeviceEvent(EvtHandle,DeviceName,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
    	    RandomCopyDemoIsRunning  = FALSE;
    	}

    	/* ------------ Get Subscriber ID ------------ */
    	STEVT_GetSubscriberID(EvtHandle,&SubscriberID);
	}
	else  /* (BlitDemoParameters.InitMode == 1) */
	{
    	strcpy(DeviceName, RandomCopyDemoParameters.DeviceName);
		SubscriberID = BlitDemoSubscriberID;
	} /* (BlitDemoParameters.InitMode == 0) */


	/* ------------ Blit Open ------------ */
    Err = STBLIT_Open(DeviceName,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
    	STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
    	RandomCopyDemoIsRunning  = FALSE;
    }

    /* ------------ Initialize global semaphores ------------ */
    RandomCopyDemoCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

	/* ------------ Set Subscriber ID ------------ */
	Context.EventSubscriberID   = SubscriberID;

	Context.UserTag_p   = RandomCopyDemoTagString;

    /* ------------ Set Context ------------ */
	Context.AluMode                 = STBLIT_ALU_COPY;
	Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    /*Context.EnableClipRectangle     = TRUE;*/
    Context.ClipRectangle           = Rectangle;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.EnableResizeFilter      = TRUE;

	/* ------------ Set ClearColor ------------ */
    if(RandomCopyDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888;
        ClearColor.Value.ARGB8888.Alpha = 0xFF;
        ClearColor.Value.ARGB8888.R     = 0x00;
        ClearColor.Value.ARGB8888.G     = 0xFF;
        ClearColor.Value.ARGB8888.B     = 0x00;
    }

    if(RandomCopyDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565 )
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_RGB565 ;
        ClearColor.Value.RGB565.R       = 0x00 ;
        ClearColor.Value.RGB565.G       = 0xFF ;
        ClearColor.Value.RGB565.B       = 0x00 ;
     }

    if(RandomCopyDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB888 )
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_RGB888 ;
        ClearColor.Value.RGB888.R       = 0x00 ;
        ClearColor.Value.RGB888.G       = 0xFF ;
        ClearColor.Value.RGB888.B       = 0x00 ;
    }

    if(RandomCopyDemoParameters.TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_CLUT8 )
    {
        ClearColor.Type                 = STGXOBJ_COLOR_TYPE_CLUT8 ;
        ClearColor.Value.CLUT8          = 0x10 ;
    }

    /* ------------ Set ClearRectangle ------------ */
    ClearRectangle.PositionX = 0;
    ClearRectangle.PositionY = 0;
    ClearRectangle.Width     = RandomCopyDemoParameters.TargetBitmap.Width;
    ClearRectangle.Height    = RandomCopyDemoParameters.TargetBitmap.Height;

    /* ------------ Init Src & Dst ------------ */
	Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &RandomCopyDemoParameters.SourceBitmap;
    Src.Palette_p            = &RandomCopyDemoParameters.Palette;

    Dst.Bitmap_p              = &RandomCopyDemoParameters.TargetBitmap;
    Dst.Palette_p             = NULL;



	/*STTBX_Print(("-- Start Random Copy demo blit operations.\n"));*/

    while ( (RandomCopyDemoIsRunning  == TRUE) && (Err == ST_NO_ERROR) )
    {
		/* ------------ Reset Bitmap ------------ */
    	Err = STBLIT_FillRectangle(Handle,&RandomCopyDemoParameters.TargetBitmap,&ClearRectangle,&ClearColor,&Context );
    	if (Err != ST_NO_ERROR)
    	{
    		STTBX_Print(("Err Blit Fill : %d\n",Err));
    		RandomCopyDemoIsRunning  = FALSE;
    	}

    	/* ------------ Wait Blit Complit ------------ */
	#ifdef ST_OSLINUX
    	time = time_plus(time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
	#else
    	time = time_plus(time_now(), ST_GetClocksPerSecond()*5);
	#endif

    	if(STOS_SemaphoreWaitTimeOut(RandomCopyDemoCompletionSemaphore_p,&time)!=0)
    	{
    	    STTBX_Report((STTBX_REPORT_LEVEL_ERROR, "TIMEOUT"));
    	    ResetEngine();
    	}

		/* ------------ Set Rectangle ------------ */
		Src.Rectangle.PositionX   = BlitDemo_Random(RandomCopyDemoParameters.OffsetX, (RandomCopyDemoParameters.Width + RandomCopyDemoParameters.OffsetX));
    	Src.Rectangle.PositionY   = BlitDemo_Random(RandomCopyDemoParameters.OffsetY, (RandomCopyDemoParameters.Height + RandomCopyDemoParameters.OffsetY));
    	Src.Rectangle.Width       = BlitDemo_Random(2, (RandomCopyDemoParameters.Width + RandomCopyDemoParameters.OffsetX - Src.Rectangle.PositionX));
    	Src.Rectangle.Height      = BlitDemo_Random(2, (RandomCopyDemoParameters.Height + RandomCopyDemoParameters.OffsetY - Src.Rectangle.PositionY));
		Dst.Rectangle.PositionX   = BlitDemo_Random(RandomCopyDemoParameters.OffsetX, (RandomCopyDemoParameters.Width + RandomCopyDemoParameters.OffsetX));
    	Dst.Rectangle.PositionY   = BlitDemo_Random(RandomCopyDemoParameters.OffsetY, (RandomCopyDemoParameters.Height + RandomCopyDemoParameters.OffsetY));
    	Dst.Rectangle.Width       = BlitDemo_Random(2, (RandomCopyDemoParameters.Width + RandomCopyDemoParameters.OffsetX - Dst.Rectangle.PositionX));
    	Dst.Rectangle.Height      = BlitDemo_Random(2, (RandomCopyDemoParameters.Height + RandomCopyDemoParameters.OffsetY - Dst.Rectangle.PositionY));

		/* ------------ Blit ------------ */
		Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    	if (Err != ST_NO_ERROR)
    	{
    	    STTBX_Print(("Src=(%d,%d,%d,%d) Dst=(%d,%d,%d,%d) STBLIT_CopyRectangle(...)=%d",Src.Rectangle.PositionX,Src.Rectangle.PositionY,Src.Rectangle.Width,Src.Rectangle.Height,
			                                                                                Dst.Rectangle.PositionX,Dst.Rectangle.PositionY,Dst.Rectangle.Width,Dst.Rectangle.Height,Err));
    	    RandomCopyDemoIsRunning  = FALSE;
    	}

    	        /* ------------ Wait Blit Complit ------------ */
#ifdef ST_OSLINUX
        time = time_plus(time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
#else
        time = time_plus(time_now(), ST_GetClocksPerSecond()*5);
#endif

        if(STOS_SemaphoreWaitTimeOut(RandomCopyDemoCompletionSemaphore_p,&time)!=0)
        {
    	    STTBX_Print(("Src=(%d,%d,%d,%d) Dst=(%d,%d,%d,%d) STBLIT_CopyRectangle(...)=TimeOut",Src.Rectangle.PositionX,Src.Rectangle.PositionY,Src.Rectangle.Width,Src.Rectangle.Height,
			                                                                                Dst.Rectangle.PositionX,Dst.Rectangle.PositionY,Dst.Rectangle.Width,Dst.Rectangle.Height));
    	    RandomCopyDemoIsRunning  = FALSE;
        }
    } /*while*/

    return;
}




#ifdef ENABLE_RECCURENCE_MODE_TEST
/*-------------------------------------------------------------------------
 * Function : BLIT_SetVPHandle
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if ok, else FALSE
 * ----------------------------------------------------------------------*/
static BOOL BLIT_SetOSDVPHandle(STTST_Parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t RetErr;
    S32 Lvar;

    /*------------ get the osd layer vp handle ------------*/
    RetErr = STTST_GetInteger(pars_p, OSDVPHandle, &Lvar);
    if ( RetErr )
    {
        STTST_TagCurrentLine( pars_p, "expected vp handle" );
        return(TRUE);
    }

    OSDVPHandle = (STLAYER_ViewPortHandle_t)Lvar;
}
#endif



#if defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109) || defined (ST_7200)

/*
================================================================================
Features Test functions
================================================================================
*/

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesCopy
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesCopy (U32                   SrcPositionX,
                                     U32                   SrcPositionY,
                                     U32                   SrcWidth,
                                     U32                   SrcHeight,
                                     U32                   DstPositionX,
                                     U32                   DstPositionY,
                                     STBLIT_Handle_t       Handle,
                                     STGXOBJ_Bitmap_t      SourceBitmap,
                                     STGXOBJ_Bitmap_t      TargetBitmap,
                                     STBLIT_BlitContext_t  Context,
                                     U32                   CRCRefValue,
                                     U32                   TestIndex,
                                     FILE*                 CRCfstream)

{
    ST_ErrorCode_t          Err;
    STGXOBJ_Rectangle_t     Rectangle;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;


    /* ------------ Set Rectangle & Destinantion ------------ */
    Rectangle.PositionX = SrcPositionX;
    Rectangle.PositionY = SrcPositionY;
    Rectangle.Width     = SrcWidth;
    Rectangle.Height    = SrcHeight;

    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);

    /* ------------ Blit operation ------------ */
    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&TargetBitmap,DstPositionX,DstPositionY,&Context);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()* TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_CopyRectangle( S(%d,%d,%d,%d) D(%d,%d) )=",TestIndex,Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height,DstPositionX,DstPositionY));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesOverlapCopy
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesOverlapCopy (  U32                   SrcPositionX,
                                            U32                   SrcPositionY,
                                            U32                   SrcWidth,
                                            U32                   SrcHeight,
                                            U32                   DstPositionX,
                                            U32                   DstPositionY,
                                            STBLIT_Handle_t       Handle,
                                            STGXOBJ_Bitmap_t      SourceBitmap,
                                            STGXOBJ_Bitmap_t      TargetBitmap,
                                            STBLIT_BlitContext_t  Context,
                                            U32                   CRCRefValue,
                                            U32                   TestIndex,
                                            FILE*                 CRCfstream)
{
    ST_ErrorCode_t          Err;
    STGXOBJ_Rectangle_t     Rectangle;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;

    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Copy source bitmap into destination bitmap ------------ */
    Rectangle.PositionX = 0;
    Rectangle.PositionY = 0;
    Rectangle.Width     = SourceBitmap.Width;
    Rectangle.Height    = SourceBitmap.Height;

    Err = STBLIT_CopyRectangle (Handle,&SourceBitmap,&Rectangle,&TargetBitmap,0,0,&Context);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif


    /* ------------ Blit operation ------------ */
    Rectangle.PositionX = SrcPositionX;
    Rectangle.PositionY = SrcPositionY;
    Rectangle.Width     = SrcWidth;
    Rectangle.Height    = SrcHeight;

    Err = STBLIT_CopyRectangle(Handle,&TargetBitmap,&Rectangle,&TargetBitmap,DstPositionX,DstPositionY,&Context);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_CopyRectangle( S(%d,%d,%d,%d) D(%d,%d) )=",TestIndex,Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height,DstPositionX,DstPositionY));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesOverlapBlit
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesOverlapBlit (  U32                   SrcPositionX,
                                            U32                   SrcPositionY,

                                            U32                   SrcWidth,
                                            U32                   SrcHeight,

                                            U32                   DstPositionX,
                                            U32                   DstPositionY,

                                            STBLIT_Handle_t       Handle,
                                            STGXOBJ_Bitmap_t      SourceBitmap,
                                            STGXOBJ_Bitmap_t      TargetBitmap,
                                            STBLIT_BlitContext_t  Context,
                                            U32                   CRCRefValue,
                                            U32                   TestIndex,
                                            FILE*                 CRCfstream)
{
    ST_ErrorCode_t          Err;
    STGXOBJ_Rectangle_t     Rectangle;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;
    STBLIT_Destination_t    Dst;
    STBLIT_Source_t         Src;

    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Copy source bitmap into destination bitmap ------------ */
    Rectangle.PositionX = 0;
    Rectangle.PositionY = 0;
    Rectangle.Width     = SourceBitmap.Width;
    Rectangle.Height    = SourceBitmap.Height;

    Err = STBLIT_CopyRectangle (Handle,&SourceBitmap,&Rectangle,&TargetBitmap,0,0,&Context);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
#ifndef STBLIT_EMULATOR
#ifdef ST_OSLINUX
    time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
#else
    time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
#endif
    if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
    {
        STTBX_Print(("Timeout%d\n"));
        return (FALSE);
    }
#else
    /*  Wait for Blit to be completed */
    STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
#endif


    /* ------------ Blit operation ------------ */
    Rectangle.PositionX = SrcPositionX;
    Rectangle.PositionY = SrcPositionY;
    Rectangle.Width     = SrcWidth;
    Rectangle.Height    = SrcHeight;


    /* ------------ Blit ------------ */
    /* Set Src */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &TargetBitmap;
    Src.Rectangle.PositionX  = SrcPositionX;
    Src.Rectangle.PositionY  = SrcPositionY;
    Src.Rectangle.Width      = SrcWidth;
    Src.Rectangle.Height     = SrcHeight;
    Src.Palette_p            = NULL;

    /* Set Dst */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = DstPositionX;
    Dst.Rectangle.PositionY   = DstPositionY;
    Dst.Rectangle.Width       = SrcWidth;
    Dst.Rectangle.Height      = SrcHeight;
    Dst.Palette_p             = NULL;

    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_CopyRectangle( S(%d,%d,%d,%d) D(%d,%d) )=",TestIndex,Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height,DstPositionX,DstPositionY));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}


/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesFill
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesFill (U32                   SrcPositionX,
                                     U32                   SrcPositionY,
                                     U32                   SrcWidth,
                                     U32                   SrcHeight,
                                     U32                   R_Value,
                                     U32                   G_Value,
                                     U32                   B_Value,
                                     STBLIT_Handle_t       Handle,
                                     STGXOBJ_Bitmap_t      TargetBitmap,
                                     STBLIT_BlitContext_t  Context,
                                     U32                   CRCRefValue,
                                     U32                   TestIndex,
                                     FILE*                 CRCfstream)

{
    ST_ErrorCode_t          Err;
    STGXOBJ_Rectangle_t     Rectangle;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    STGXOBJ_Color_t         Color;
    BOOL                    ReportTestResult;


    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = SrcPositionX;
    Rectangle.PositionY = SrcPositionY;
    Rectangle.Width     = SrcWidth;
    Rectangle.Height    = SrcHeight;

    /* ------------ Set Color ------------ */
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        Color.Type             = STGXOBJ_COLOR_TYPE_ARGB8888 ;
        Color.Value.ARGB8888.Alpha = 0xFF;
        Color.Value.ARGB8888.R = R_Value;
        Color.Value.ARGB8888.G = G_Value;
        Color.Value.ARGB8888.B = B_Value;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        Color.Type           = STGXOBJ_COLOR_TYPE_RGB565 ;
        Color.Value.RGB565.R = R_Value;
        Color.Value.RGB565.G = G_Value;
        Color.Value.RGB565.B = B_Value;
    }



    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Blit ------------ */
    Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&Rectangle,&Color,&Context);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif


    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_FillRectangle( R(%d,%d,%d,%d) C(%d,%d,%d) )=",TestIndex,Rectangle.PositionX,Rectangle.PositionY,Rectangle.Width,Rectangle.Height,R_Value,G_Value,B_Value));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}


/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesSetPixel
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesSetPixel (U32                 SrcPositionX,
                                     U32                   SrcPositionY,
                                     U32                   R_Value,
                                     U32                   G_Value,
                                     U32                   B_Value,
                                     STBLIT_Handle_t       Handle,
                                     STGXOBJ_Bitmap_t      TargetBitmap,
                                     STBLIT_BlitContext_t  Context,
                                     U32                   CRCRefValue,
                                     U32                   TestIndex,
                                     FILE*                 CRCfstream)

{
    ST_ErrorCode_t          Err;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    STGXOBJ_Color_t         Color;
    BOOL                    ReportTestResult;



    /* ------------ Set Color ------------ */
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        Color.Type             = STGXOBJ_COLOR_TYPE_ARGB8888 ;
        Color.Value.ARGB8888.Alpha = 0xFF;
        Color.Value.ARGB8888.R = R_Value;
        Color.Value.ARGB8888.G = G_Value;
        Color.Value.ARGB8888.B = B_Value;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        Color.Type           = STGXOBJ_COLOR_TYPE_RGB565 ;
        Color.Value.RGB565.R = R_Value;
        Color.Value.RGB565.G = G_Value;
        Color.Value.RGB565.B = B_Value;
    }



    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Blit ------------ */
    Err = STBLIT_SetPixel     (Handle,&TargetBitmap,SrcPositionX,SrcPositionY,&Color,&Context );

    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif


    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_SetPixel( R(%d,%d,%d,%d) C(%d,%d,%d) )=",TestIndex,SrcPositionX,SrcPositionX,R_Value,G_Value,B_Value));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesDrawHLine
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesDrawHLine (U32                 SrcPositionX,
                                     U32                   SrcPositionY,
                                     U32                   Length,
                                     U32                   R_Value,
                                     U32                   G_Value,
                                     U32                   B_Value,
                                     STBLIT_Handle_t       Handle,
                                     STGXOBJ_Bitmap_t      TargetBitmap,
                                     STBLIT_BlitContext_t  Context,
                                     U32                   CRCRefValue,
                                     U32                   TestIndex,
                                     FILE*                 CRCfstream)

{
    ST_ErrorCode_t          Err;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    STGXOBJ_Color_t         Color;
    BOOL                    ReportTestResult;



    /* ------------ Set Color ------------ */
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        Color.Type             = STGXOBJ_COLOR_TYPE_ARGB8888 ;
        Color.Value.ARGB8888.Alpha = 0xFF;
        Color.Value.ARGB8888.R = R_Value;
        Color.Value.ARGB8888.G = G_Value;
        Color.Value.ARGB8888.B = B_Value;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        Color.Type           = STGXOBJ_COLOR_TYPE_RGB565 ;
        Color.Value.RGB565.R = R_Value;
        Color.Value.RGB565.G = G_Value;
        Color.Value.RGB565.B = B_Value;
    }



    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Blit ------------ */
    Err = STBLIT_DrawHLine(Handle,&TargetBitmap,SrcPositionX,SrcPositionY,Length,&Color,&Context );

    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif


    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_DrawHLine( X,Y,L(%d,%d) C(%d,%d,%d) )=",TestIndex,SrcPositionX,SrcPositionX,Length,R_Value,G_Value,B_Value));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesDrawVLine
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesDrawVLine (U32                 SrcPositionX,
                                     U32                   SrcPositionY,
                                     U32                   Length,
                                     U32                   R_Value,
                                     U32                   G_Value,
                                     U32                   B_Value,
                                     STBLIT_Handle_t       Handle,
                                     STGXOBJ_Bitmap_t      TargetBitmap,
                                     STBLIT_BlitContext_t  Context,
                                     U32                   CRCRefValue,
                                     U32                   TestIndex,
                                     FILE*                 CRCfstream)

{
    ST_ErrorCode_t          Err;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    STGXOBJ_Color_t         Color;
    BOOL                    ReportTestResult;



    /* ------------ Set Color ------------ */
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        Color.Type             = STGXOBJ_COLOR_TYPE_ARGB8888 ;
        Color.Value.ARGB8888.Alpha = 0xFF;
        Color.Value.ARGB8888.R = R_Value;
        Color.Value.ARGB8888.G = G_Value;
        Color.Value.ARGB8888.B = B_Value;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        Color.Type           = STGXOBJ_COLOR_TYPE_RGB565 ;
        Color.Value.RGB565.R = R_Value;
        Color.Value.RGB565.G = G_Value;
        Color.Value.RGB565.B = B_Value;
    }



    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Blit ------------ */
    Err = STBLIT_DrawVLine(Handle,&TargetBitmap,SrcPositionX,SrcPositionY,Length,&Color,&Context );

    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif


    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_DrawVLine( X,Y,L(%d,%d) C(%d,%d,%d) )=",TestIndex,SrcPositionX,SrcPositionX,Length,R_Value,G_Value,B_Value));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesBitmapBlend
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesBitmapBlend (U32                   SrcPositionX,
                                            U32                   SrcPositionY,
                                            U32                   SrcWidth,
                                            U32                   SrcHeight,
                                            U32                   DstPositionX,
                                            U32                   DstPositionY,
                                            U32                   DstWidth,
                                            U32                   DstHeight,
                                            U32                   AlphaValue,
                                            STBLIT_Handle_t       Handle,
                                            STGXOBJ_Bitmap_t      SourceBitmap,
                                            STGXOBJ_Bitmap_t      TargetBitmap,
                                            STBLIT_BlitContext_t  Context,
                                            U32                   CRCRefValue,
                                            U32                   TestIndex,
                                            FILE*                 CRCfstream)

{
    ST_ErrorCode_t          Err;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;


    /* ------------ Set Source ------------ */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = SrcPositionX;
    Src.Rectangle.PositionY  = SrcPositionY;
    Src.Rectangle.Width      = SrcWidth;
    Src.Rectangle.Height     = SrcHeight;
    Src.Palette_p            = NULL;

    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = DstPositionX;
    Dst.Rectangle.PositionY   = DstPositionY;
    Dst.Rectangle.Width       = DstWidth;
    Dst.Rectangle.Height      = DstHeight;
    Dst.Palette_p             = NULL;

    /* ------------ Set Global alpha ------------ */
    Context.GlobalAlpha       = AlphaValue;


    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Blit operation ------------ */
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_BLIT( S(%d,%d,%d,%d) D(%d,%d,%d,%d) A(%d) )=",TestIndex,SrcPositionX,SrcPositionY,SrcWidth,SrcHeight,
                                                                                      DstPositionX,DstPositionY,DstWidth,DstHeight,AlphaValue));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}



/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesFillBlend
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesFillBlend ( STGXOBJ_Rectangle_t RectBlend,
                                         STGXOBJ_ColorARGB_t ColorBlend,
                                        STBLIT_Handle_t       Handle,
                                        STGXOBJ_Bitmap_t      TargetBitmap,
                                        STEVT_SubscriberID_t  SubscriberID,
                                        U32                   CRCRefValue,
                                        U32                   TestIndex,
                                        FILE*                 CRCfstream)

{
    ST_ErrorCode_t          Err;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
    STGXOBJ_Color_t         Color;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;
    STBLIT_BlitContext_t    Context;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    Context.AluMode                 = STBLIT_ALU_ALPHA_BLEND;
    Context.EnableMaskWord          = FALSE;
    Context.EnableMaskBitmap        = FALSE;
    Context.EnableColorCorrection   = FALSE;
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;
    Context.EventSubscriberID       = SubscriberID;


    /* ------------ Set Color ------------ */
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        Color.Type             = STGXOBJ_COLOR_TYPE_ARGB8888 ;
        Color.Value.ARGB8888.Alpha = 0xFF;
        Color.Value.ARGB8888.R = ColorBlend.R;
        Color.Value.ARGB8888.G = ColorBlend.G;
        Color.Value.ARGB8888.B = ColorBlend.B;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        Color.Type           = STGXOBJ_COLOR_TYPE_RGB565 ;
        Color.Value.RGB565.R =  ColorBlend.R;
        Color.Value.RGB565.G =  ColorBlend.G;
        Color.Value.RGB565.B =  ColorBlend.B;
    }


    /* ------------ Set Source ------------ */
    Src.Type                 = STBLIT_SOURCE_TYPE_COLOR;
    Src.Data.Color_p         = &Color;
    Src.Palette_p            = NULL;

    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = RectBlend.PositionX;
    Dst.Rectangle.PositionY   = RectBlend.PositionY;
    Dst.Rectangle.Width       = RectBlend.Width;
    Dst.Rectangle.Height      = RectBlend.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Set Global alpha ------------ */
    Context.GlobalAlpha       = ColorBlend.Alpha;


    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Blit operation ------------ */
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
    #ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_BLIT( R(%d,%d,%d,%d) A(%d) )=",TestIndex,
                RectBlend.PositionX, RectBlend.PositionY,
                RectBlend.Width, RectBlend.Height, ColorBlend.Alpha/*AlphaValue*/));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}


/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesCopy2Src
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesCopy2Src (U32                   Src1PositionX,
                                         U32                   Src1PositionY,
                                         U32                   Src2PositionX,
                                         U32                   Src2PositionY,
                                         U32                   Src2Width,
                                         U32                   Src2Height,
                                         U32                   DstPositionX,
                                         U32                   DstPositionY,
                                         U32                   DstWidth,
                                         U32                   DstHeight,
                                         STBLIT_Handle_t       Handle,
                                         STGXOBJ_Bitmap_t      SourceBitmap1,
                                         STGXOBJ_Bitmap_t      SourceBitmap2,
                                         STGXOBJ_Bitmap_t      TargetBitmap,
                                         STBLIT_BlitContext_t  Context,
                                         U32                   CRCRefValue,
                                         U32                   TestIndex,
                                         FILE*                 CRCfstream)
{
    ST_ErrorCode_t          Err;
    STBLIT_Source_t         Src1,Src2;
    STBLIT_Destination_t    Dst;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;


    /* ------------ Set Source ------------ */
    Src1.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src1.Data.Bitmap_p        = &SourceBitmap1;
    Src1.Rectangle.PositionX  = Src1PositionX;
    Src1.Rectangle.PositionY  = Src1PositionY;
    Src1.Rectangle.Width      = DstWidth;
    Src1.Rectangle.Height     = DstHeight;
    Src1.Palette_p            = NULL;

    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &SourceBitmap2;
    Src2.Rectangle.PositionX  = Src2PositionX;
    Src2.Rectangle.PositionY  = Src2PositionY;
    Src2.Rectangle.Width      = Src2Width;
    Src2.Rectangle.Height     = Src2Height;
    Src2.Palette_p            = NULL;


    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = DstPositionX;
    Dst.Rectangle.PositionY   = DstPositionY;
    Dst.Rectangle.Width       = DstWidth;
    Dst.Rectangle.Height      = DstHeight;
    Dst.Palette_p             = NULL;

    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Blit operation ------------ */
    Err = STBLIT_Blit(Handle,&Src1,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_BLIT( S1(%d,%d) S2(%d,%d,%d,%d) D(%d,%d,%d,%d) )=",TestIndex,Src1PositionX,Src1PositionY,
                                                                                        Src2PositionX,Src2PositionY,Src2Width,Src2Height,
                                                                                        DstPositionX,DstPositionY,DstWidth,DstHeight));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesClip
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesClip (U32                   ClipPositionX,
                                     U32                   ClipPositionY,
                                     U32                   ClipWidth,
                                     U32                   ClipHeight,
                                     BOOL                  WriteInsideClipRectangle,
                                     STBLIT_Handle_t       Handle,
                                     STGXOBJ_Bitmap_t      SourceBitmap,
                                     STGXOBJ_Bitmap_t      TargetBitmap,
                                     STBLIT_BlitContext_t  Context,
                                     U32                   CRCRefValue,
                                     U32                   TestIndex,
                                     FILE*                 CRCfstream)
{
    ST_ErrorCode_t          Err;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
    STGXOBJ_Rectangle_t     Rectangle;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;


    /* ------------ Set Source ------------ */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 50;
    Src.Rectangle.PositionY  = 50;
    Src.Rectangle.Width      = SourceBitmap.Width - 100;
    Src.Rectangle.Height     = SourceBitmap.Height - 100;
    Src.Palette_p            = NULL;


    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 50;
    Dst.Rectangle.PositionY   = 50;
    Dst.Rectangle.Width       = SourceBitmap.Width - 100;
    Dst.Rectangle.Height      = SourceBitmap.Height - 100;
    Dst.Palette_p             = NULL;

    /* ------------ Set Rectangle ------------ */
    Rectangle.PositionX = Src.Rectangle.PositionX + ClipPositionX;
    Rectangle.PositionY = Src.Rectangle.PositionY + ClipPositionY;
    Rectangle.Width     = ClipWidth;
    Rectangle.Height    = ClipHeight;


    /* ------------ Set Context ------------ */
    Context.ClipRectangle            = Rectangle;
    Context.WriteInsideClipRectangle = WriteInsideClipRectangle;


    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Blit operation ------------ */
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_BLIT( CLIP(%d,%d,%d,%d) RICR(%d) )=",TestIndex,ClipPositionX,ClipPositionY,ClipWidth,ClipHeight,WriteInsideClipRectangle));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesResize
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesResize (U32                   SrcPositionX,
                                       U32                   SrcPositionY,
                                       U32                   SrcWidth,
                                       U32                   SrcHeight,
                                       U32                   DstPositionX,
                                       U32                   DstPositionY,
                                       U32                   DstWidth,
                                       U32                   DstHeight,
                                       STBLIT_Handle_t       Handle,
                                       STGXOBJ_Bitmap_t      SourceBitmap,
                                       STGXOBJ_Bitmap_t      TargetBitmap,
                                       STBLIT_BlitContext_t  Context,
                                       U32                   CRCRefValue,
                                       U32                   TestIndex,
                                       FILE*                 CRCfstream)

{
    ST_ErrorCode_t          Err;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;


    /* ------------ Set Source ------------ */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = SrcPositionX;
    Src.Rectangle.PositionY  = SrcPositionY;
    Src.Rectangle.Width      = SrcWidth;
    Src.Rectangle.Height     = SrcHeight;
    Src.Palette_p            = NULL;

    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = DstPositionX;
    Dst.Rectangle.PositionY   = DstPositionY;
    Dst.Rectangle.Width       = DstWidth;
    Dst.Rectangle.Height      = DstHeight;
    Dst.Palette_p             = NULL;

    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Blit operation ------------ */
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_BLIT( S(%d,%d,%d,%d) D(%d,%d,%d,%d) )=",TestIndex,SrcPositionX,SrcPositionY,SrcWidth,SrcHeight,
                                                                        DstPositionX,DstPositionY,DstWidth,DstHeight));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesColorConversion
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesColorConversion (STBLIT_Handle_t       Handle,
                                                STGXOBJ_Bitmap_t      SourceBitmap,
                                                STGXOBJ_Palette_t     Palette,
                                                STGXOBJ_Bitmap_t      TargetBitmap,
                                                STBLIT_BlitContext_t  Context,
                                                U32                   CRCRefValue,
                                                U32                   TestIndex,
                                                FILE*                 CRCfstream)

{
    ST_ErrorCode_t          Err;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;


    /* ------------ Set Source ------------ */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;

    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Blit operation ------------ */
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_BLIT( S(%d,%d,%d,%d) D(%d,%d,%d,%d) )=",TestIndex,
                                                                                Src.Rectangle.PositionX,Src.Rectangle.PositionY,
                                                                                Src.Rectangle.Width,
                                                                                Src.Rectangle.Height,
                                                                                Dst.Rectangle.PositionX,Dst.Rectangle.PositionY,
                                                                                Dst.Rectangle.Width,
                                                                                Dst.Rectangle.Height));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}

#if defined(ST_7109) || defined(ST_7200)
/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesVC1Range
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesVC1Range (         STBLIT_Handle_t       Handle,
                                                STGXOBJ_Bitmap_t      SourceBitmap,
                                                STGXOBJ_Palette_t     Palette,
                                                STGXOBJ_Bitmap_t      TargetBitmap,
                                                YUV_Scaling_t         ScalingFactorY,
                                                YUV_Scaling_t         ScalingFactorUV,
                                                STBLIT_BlitContext_t  Context,
                                                U32                   CRCRefValue,
                                                U32                   TestIndex,
                                                FILE*                 CRCfstream)

{
    ST_ErrorCode_t          Err;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;


    /* ------------ Set Source ------------ */
    Src.Type                                        = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p                               = &SourceBitmap;
    Src.Data.Bitmap_p->BitmapType                   = STGXOBJ_BITMAP_TYPE_MB_RANGE_MAP;

    /* set Src YUVScaling factors for range Reduction */
    Src.Data.Bitmap_p->YUVScaling.ScalingFactorY    = ScalingFactorY;
    Src.Data.Bitmap_p->YUVScaling.ScalingFactorUV   = ScalingFactorUV;

    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = &Palette;

    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = TargetBitmap.Width;
    Dst.Rectangle.Height      = TargetBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Blit operation ------------ */
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_BLIT( S(%d,%d,%d,%d) D(%d,%d,%d,%d) )=",TestIndex,
                                                                                Src.Rectangle.PositionX,Src.Rectangle.PositionY,
                                                                                Src.Rectangle.Width,
                                                                                Src.Rectangle.Height,
                                                                                Dst.Rectangle.PositionX,Dst.Rectangle.PositionY,
                                                                                Dst.Rectangle.Width,
                                                                                Dst.Rectangle.Height));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}
#endif /*defined(ST_7109) || defined(ST_7200)*/

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesJobTestInit
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static ST_ErrorCode_t BLIT_TestFeaturesJobTestInit (STBLIT_Handle_t         Handle,
                                                      STBLIT_JobHandle_t      Job1,
                                                      STBLIT_JobBlitHandle_t* JBHandle_1_p,
                                                      STBLIT_JobBlitHandle_t* JBHandle_2_p,
                                                      STBLIT_JobBlitHandle_t* JBHandle_3_p,
                                                      STGXOBJ_Bitmap_t        SourceBitmap,
                                                      STGXOBJ_Palette_t       Palette,
                                                      STGXOBJ_Bitmap_t        TargetBitmap)
{
    ST_ErrorCode_t          Err = ST_NO_ERROR;
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src,SrcForeground;
    STBLIT_Destination_t    Dst1, Dst2, Dst3;
    STGXOBJ_Color_t         ColorFill;
    STGXOBJ_Rectangle_t     ClipRectangle;
    U32                     OffsetX, OffsetY, RectangleWidth, RectangleHeight;


    OffsetX = 20;
    OffsetY = 20;
    RectangleWidth = RectangleHeight = ((TargetBitmap.Height - 4*OffsetY)/3);


    /* Set Src */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = NULL;

    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        ColorFill.Type             = STGXOBJ_COLOR_TYPE_ARGB8888 ;
        ColorFill.Value.ARGB8888.Alpha = 0xFF;
        ColorFill.Value.ARGB8888.R = 0x00;
        ColorFill.Value.ARGB8888.G = 0x3F;
        ColorFill.Value.ARGB8888.B = 0x00;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        ColorFill.Type           = STGXOBJ_COLOR_TYPE_RGB565 ;
        ColorFill.Value.RGB565.R = 0x00;
        ColorFill.Value.RGB565.G = 0x3F;
        ColorFill.Value.RGB565.B = 0x00;
    }

    SrcForeground.Type                 = STBLIT_SOURCE_TYPE_COLOR;
    SrcForeground.Data.Color_p         = &ColorFill;
    SrcForeground.Palette_p            = NULL;

    /* Set Dst */
    Dst1.Bitmap_p              = &TargetBitmap;
    Dst1.Rectangle.PositionX   = OffsetX;
    Dst1.Rectangle.PositionY   = OffsetY;
    Dst1.Rectangle.Width       = RectangleWidth;
    Dst1.Rectangle.Height      = RectangleHeight;
    Dst1.Palette_p             = NULL;

    Dst2.Bitmap_p              = &TargetBitmap;
    Dst2.Rectangle.PositionX   = OffsetX;
    Dst2.Rectangle.PositionY   = 2*OffsetY + RectangleHeight;
    Dst2.Rectangle.Width       = RectangleWidth;
    Dst2.Rectangle.Height      = RectangleHeight;
    Dst2.Palette_p             = NULL;

    Dst3.Bitmap_p              = &TargetBitmap;
    Dst3.Rectangle.PositionX   = OffsetX;
    Dst3.Rectangle.PositionY   = 3*OffsetY + 2*RectangleHeight;
    Dst3.Rectangle.Width       = RectangleWidth;
    Dst3.Rectangle.Height      = RectangleHeight;
    Dst3.Palette_p             = NULL;

    /* Set Main Context */
    Context.ColorKeyCopyMode            = STBLIT_COLOR_KEY_MODE_NONE;
    Context.AluMode                     = STBLIT_ALU_COPY;
    Context.EnableMaskWord              = FALSE;
    Context.EnableMaskBitmap            = FALSE;
    Context.EnableColorCorrection       = FALSE;
    Context.Trigger.EnableTrigger       = FALSE;
    Context.GlobalAlpha                 = 50;
    Context.EnableFlickerFilter         = FALSE;
    Context.UserTag_p                   = NULL;
    Context.EnableResizeFilter          = TRUE;
    Context.NotifyBlitCompletion        = FALSE;
    Context.JobHandle                   = Job1;


    /* Set Job Blit 1 Context */
    Context.EnableClipRectangle      = FALSE;

    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst1,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (Err);
    }

    Err = STBLIT_GetJobBlitHandle(Handle, Job1, JBHandle_1_p) ;
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (Err);
    }


    /* Set Job Blit 2 Context */
    ClipRectangle.PositionX = Dst2.Rectangle.PositionX + 10;
    ClipRectangle.PositionY = Dst2.Rectangle.PositionY + 10;
    ClipRectangle.Width     = 30;
    ClipRectangle.Height    = 30;

    Context.EnableClipRectangle      = TRUE;
    Context.ClipRectangle            = ClipRectangle;
    Context.WriteInsideClipRectangle = FALSE;

    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst2,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (Err);
    }

    Err = STBLIT_GetJobBlitHandle(Handle, Job1, JBHandle_2_p) ;
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (Err);
    }


    /* Set Job Blit 3 Context */
    Context.EnableClipRectangle      = FALSE;

    Err = STBLIT_Blit(Handle,NULL,&SrcForeground,&Dst3,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (Err);
    }


    Err = STBLIT_GetJobBlitHandle( Handle, Job1, JBHandle_3_p);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (Err);
    }

#if 0
    /* SubmitJob */
    Err = STBLIT_SubmitJobFront(Handle, Job1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Submit job : %d\n",Err));
        return (TRUE);
    }

    /* Wait for Job blit to be completed */
    #ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(JobCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(JobCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif


    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Job Done"));
#endif



    return (Err);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesJobTestDestination
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesJobTestDestination (STBLIT_Handle_t         Handle,
                                                   STBLIT_JobHandle_t      Job1,
                                                   STBLIT_JobBlitHandle_t  JBHandle_1,
                                                   STBLIT_JobBlitHandle_t  JBHandle_2,
                                                   STBLIT_JobBlitHandle_t  JBHandle_3,
                                                   U32                     OffsetPositionX,
                                                   U32                     OffsetPositionY,
                                                   U32                     OffsetWidth,
                                                   U32                     OffsetHeight,
                                                   STGXOBJ_Bitmap_t        SourceBitmap,
                                                   STGXOBJ_Palette_t       Palette,
                                                   STGXOBJ_Bitmap_t        TargetBitmap,
                                                   U32                     CRCRefValue,
                                                   U32                     TestIndex,
                                                   FILE*                   CRCfstream)
{
    ST_ErrorCode_t          Err = ST_NO_ERROR;
    STGXOBJ_Rectangle_t     PositionRectangle1, PositionRectangle2, PositionRectangle3;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     OffsetX, OffsetY, RectangleWidth, RectangleHeight, CRCValue;
    BOOL                    ReportTestResult;


    OffsetX = 20;
    OffsetY = 20;
    RectangleWidth = RectangleHeight = ((TargetBitmap.Height - 4*OffsetY)/3);

    /* Set Job Blit 1 params */
    PositionRectangle1.PositionX   = OffsetX + OffsetPositionX;
    PositionRectangle1.PositionY   = OffsetY + OffsetPositionY;
    PositionRectangle1.Width       = 50 + OffsetWidth;
    PositionRectangle1.Height      = 50 + OffsetHeight;

    Err = STBLIT_SetJobBlitRectangle(Handle, JBHandle_1, STBLIT_DATA_TYPE_DESTINATION, &PositionRectangle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",Err));
        return (FALSE);
    }


    /* Set Job Blit 2 params */
    PositionRectangle2.PositionX   = OffsetX + OffsetPositionX;
    PositionRectangle2.PositionY   = 2 * OffsetY + RectangleHeight + OffsetPositionY;
    PositionRectangle2.Width       = 50 + OffsetWidth;
    PositionRectangle2.Height      = 50 + OffsetHeight;

    Err = STBLIT_SetJobBlitRectangle(Handle, JBHandle_2, STBLIT_DATA_TYPE_DESTINATION, &PositionRectangle2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",Err));
        return (FALSE);
    }

    /* Set Job Blit 3 params */
    PositionRectangle3.PositionX   = OffsetX + OffsetPositionX;
    PositionRectangle3.PositionY   = 3*OffsetY + 2*RectangleHeight + OffsetPositionY;
    PositionRectangle3.Width       = 50 + OffsetWidth;
    PositionRectangle3.Height      = 50 + OffsetHeight;

    Err = STBLIT_SetJobBlitRectangle(Handle, JBHandle_3, STBLIT_DATA_TYPE_DESTINATION, &PositionRectangle3);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",Err));
        return (FALSE);
    }


    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* SubmitJob */
    Err = STBLIT_SubmitJobFront(Handle, Job1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Submit job : %d\n",Err));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(JobCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(JobCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_SubmitJobFront( JB1_D(%d,%d,%d,%d) JB2_D(%d,%d,%d,%d) JB3_D(%d,%d,%d,%d))=",TestIndex,
                    PositionRectangle1.PositionX, PositionRectangle1.PositionY, PositionRectangle1.Width, PositionRectangle1.Height,
                    PositionRectangle2.PositionX, PositionRectangle2.PositionY, PositionRectangle2.Width, PositionRectangle2.Height,
                    PositionRectangle3.PositionX, PositionRectangle3.PositionY, PositionRectangle3.Width, PositionRectangle3.Height));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesJobTestContent
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesJobTestContent (STBLIT_Handle_t         Handle,
                                               STBLIT_JobHandle_t      Job1,
                                               STBLIT_JobBlitHandle_t  JBHandle_1,
                                               STBLIT_JobBlitHandle_t  JBHandle_2,
                                               STBLIT_JobBlitHandle_t  JBHandle_3,
                                               U32                     OffsetPositionX,
                                               U32                     OffsetPositionY,
                                               BOOL                    WriteIntoClipRectangle,
                                               U32                     R_Value,
                                               U32                     G_Value,
                                               U32                     B_Value,
                                               STGXOBJ_Bitmap_t        SourceBitmap,
                                               STGXOBJ_Palette_t       Palette,
                                               STGXOBJ_Bitmap_t        TargetBitmap,
                                               U32                     CRCRefValue,
                                               U32                     TestIndex,
                                               FILE*                   CRCfstream)
{
    ST_ErrorCode_t          Err = ST_NO_ERROR;
    STGXOBJ_Rectangle_t     ClipRectangle;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     OffsetX, OffsetY, RectangleWidth, RectangleHeight, CRCValue;
    STGXOBJ_Color_t         ColorFill;
    BOOL                    ReportTestResult;


    OffsetX = 20;
    OffsetY = 20;
    RectangleWidth = RectangleHeight = ((TargetBitmap.Height - 4*OffsetY)/3);

    /* Set Job Blit 1 params */
    Err = STBLIT_SetJobBlitBitmap   ( Handle, JBHandle_1, STBLIT_DATA_TYPE_FOREGROUND , &SourceBitmap);

    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",Err));
        return (FALSE);
    }


    /* Set Job Blit 2 params */
    ClipRectangle.PositionX   = 10 + OffsetX + OffsetPositionX;
    ClipRectangle.PositionY   = 10 + 2*OffsetY + RectangleHeight + OffsetPositionY;
    ClipRectangle.Width       = 30;
    ClipRectangle.Height      = 30;

    Err = STBLIT_SetJobBlitClipRectangle(Handle, JBHandle_2, WriteIntoClipRectangle,&ClipRectangle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",Err));
        return (FALSE);
    }

    /* Set Job Blit 3 params */
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        ColorFill.Type             = STGXOBJ_COLOR_TYPE_ARGB8888 ;
        ColorFill.Value.ARGB8888.Alpha = 0xFF;
        ColorFill.Value.ARGB8888.R = R_Value;
        ColorFill.Value.ARGB8888.G = G_Value;
        ColorFill.Value.ARGB8888.B = B_Value;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        ColorFill.Type           = STGXOBJ_COLOR_TYPE_RGB565 ;
        ColorFill.Value.RGB565.R = R_Value;
        ColorFill.Value.RGB565.G = G_Value;
        ColorFill.Value.RGB565.B = B_Value;
    }

    Err = STBLIT_SetJobBlitColorFill( Handle, JBHandle_3, STBLIT_DATA_TYPE_FOREGROUND , &ColorFill);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Set job : %d\n",Err));
        return (FALSE);
    }


    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);

    /* SubmitJob */
    Err = STBLIT_SubmitJobFront(Handle, Job1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Submit job : %d\n",Err));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(JobCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(JobCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_SubmitJobFront( JB1_S(%x) JB2_Clip(%d,%d,%d,%d) JB3_CF(%d,%d,%d))=",TestIndex,(U32)(&SourceBitmap),
                    ClipRectangle.PositionX, ClipRectangle.PositionY, ClipRectangle.Width, ClipRectangle.Height,
                    R_Value, G_Value, B_Value));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}


#if defined (ST_7109) || defined(ST_7200)
/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesALU
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesALU (STBLIT_Handle_t       Handle,
                                    STGXOBJ_Bitmap_t      SourceBitmap1,
                                    STGXOBJ_Bitmap_t      SourceBitmap2,
                                    STGXOBJ_Bitmap_t      TargetBitmap,
                                    STBLIT_BlitContext_t  Context,
                                    U32                   CRCRefValue,
                                    U32                   TestIndex,
                                    FILE*                 CRCfstream)
{
    ST_ErrorCode_t          Err;
    STBLIT_Source_t         Src1,Src2;
    STBLIT_Destination_t    Dst;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;


    /* ------------ Set Source ------------ */
    Src1.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src1.Data.Bitmap_p        = &SourceBitmap1;
    Src1.Rectangle.PositionX  = 0;
    Src1.Rectangle.PositionY  = 0;
    Src1.Rectangle.Width      = SourceBitmap1.Width;
    Src1.Rectangle.Height     = SourceBitmap1.Height;
    Src1.Palette_p            = NULL;

    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &SourceBitmap2;
    Src2.Rectangle.PositionX  = 0;
    Src2.Rectangle.PositionY  = 0;
    Src2.Rectangle.Width      = SourceBitmap2.Width;
    Src2.Rectangle.Height     = SourceBitmap2.Height;
    Src2.Palette_p            = NULL;


    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = SourceBitmap2.Width;
    Dst.Rectangle.Height      = SourceBitmap2.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Blit operation ------------ */
    Err = STBLIT_Blit(Handle,&Src1,&Src2,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_BLIT( S1(%d,%d) S2(%d,%d,%d,%d) D(%d,%d,%d,%d))=",TestIndex,
                                                                                       Src1.Rectangle.PositionX,Src1.Rectangle.PositionY,
                                                                                       Src2.Rectangle.PositionX,Src2.Rectangle.PositionY,Src2.Rectangle.Width,Src2.Rectangle.Height,
                                                                                       Dst.Rectangle.PositionX,Dst.Rectangle.PositionY,Dst.Rectangle.Width,Dst.Rectangle.Height));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}
#endif /* 7109 */

#if defined (ST_7109) || defined (ST_7200) || defined (ST_7100)
/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesMaskWord
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesMaskWord (STBLIT_Handle_t       Handle,
                                         STGXOBJ_Bitmap_t      SourceBitmap,
                                         STGXOBJ_Bitmap_t      TargetBitmap,
                                         U32                   MaskWordValue,
                                         STBLIT_BlitContext_t  Context,
                                         U32                   CRCRefValue,
                                         U32                   TestIndex,
                                         FILE*                 CRCfstream)

{
    ST_ErrorCode_t          Err;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;


    /* ------------ Set Source ------------ */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap.Width;
    Src.Rectangle.Height     = SourceBitmap.Height;
    Src.Palette_p            = NULL;

    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = SourceBitmap.Width;
    Dst.Rectangle.Height      = SourceBitmap.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Clear TargetBitmap ------------ */
    /*memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);*/

    /* ------------ Set Context ------------ */
    Context.MaskWord          = MaskWordValue;

    /* ------------ Blit operation ------------ */
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_BLIT( S(%d,%d,%d,%d) D(%d,%d,%d,%d) M=%x )=",TestIndex,
                                                                      Src.Rectangle.PositionX,Src.Rectangle.PositionY,Src.Rectangle.Width,Src.Rectangle.Height,
                                                                      Dst.Rectangle.PositionX,Dst.Rectangle.PositionY,Dst.Rectangle.Width,Dst.Rectangle.Height,MaskWordValue));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesColorKey
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesColorKey (STBLIT_Handle_t           Handle,
                                         STGXOBJ_Bitmap_t          SourceBitmap,
                                         STGXOBJ_Bitmap_t          SourceBitmap1,
                                         STGXOBJ_Bitmap_t          TargetBitmap,
                                         U32                       GMin,
                                         U32                       GMax,
                                         STBLIT_ColorKeyCopyMode_t ColorKeyCopyMode,
                                         STBLIT_BlitContext_t      Context,
                                         U32                       CRCRefValue,
                                         U32                       TestIndex,
                                         FILE*                     CRCfstream)
{
    ST_ErrorCode_t          Err;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
    STGXOBJ_Rectangle_t     Rectangle;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;

    /* ------------ Clear TargetBitmap ------------ */
    memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);


    /* ------------ Copy source bitmap into destination bitmap ------------ */
    Context.ColorKeyCopyMode = STBLIT_COLOR_KEY_MODE_NONE;
    Rectangle.PositionX = 0;
    Rectangle.PositionY = 0;
    Rectangle.Width     = SourceBitmap.Width;
    Rectangle.Height    = SourceBitmap.Height;

    Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&TargetBitmap,0,0,&Context);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif



    /* ------------ Set Source ------------ */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap1;
    Src.Rectangle.PositionX  = 0;
    Src.Rectangle.PositionY  = 0;
    Src.Rectangle.Width      = SourceBitmap1.Width;
    Src.Rectangle.Height     = SourceBitmap1.Height;
    Src.Palette_p            = NULL;

    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = SourceBitmap1.Width;
    Dst.Rectangle.Height      = SourceBitmap1.Height;
    Dst.Palette_p             = NULL;

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode              = ColorKeyCopyMode;
    Context.ColorKey.Value.RGB888.GMin    = GMin;
    Context.ColorKey.Value.RGB888.GMax    = GMax;


    /* ------------ Blit operation ------------ */
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif


    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_BLIT( S(%d,%d,%d,%d) D(%d,%d,%d,%d) GMin=%d GMax=%d Mode=%d)=",TestIndex,
                                                                      Src.Rectangle.PositionX,Src.Rectangle.PositionY,Src.Rectangle.Width,Src.Rectangle.Height,
                                                                      Dst.Rectangle.PositionX,Dst.Rectangle.PositionY,Dst.Rectangle.Width,Dst.Rectangle.Height,
                                                                      GMin,GMax,ColorKeyCopyMode));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}
#endif /* (ST_7109) || defined (ST_7200) || defined (ST_7100) */

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesAntiAliasing
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesAntiAliasing (U32                   SrcPositionX,
                                       U32                   SrcPositionY,
                                       U32                   SrcWidth,
                                       U32                   SrcHeight,
                                       U32                   DstPositionX,
                                       U32                   DstPositionY,
                                       U32                   DstWidth,
                                       U32                   DstHeight,
                                       STBLIT_Handle_t       Handle,
                                       STGXOBJ_Bitmap_t      SourceBitmap,
                                       STGXOBJ_Bitmap_t      TargetBitmap,
                                       STBLIT_BlitContext_t  Context,
                                       U32                   CRCRefValue,
                                       U32                   TestIndex,
                                       FILE*                 CRCfstream)

{
    ST_ErrorCode_t          Err;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;


    /* ------------ Set Source ------------ */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = SrcPositionX;
    Src.Rectangle.PositionY  = SrcPositionY;
    Src.Rectangle.Width      = SrcWidth;
    Src.Rectangle.Height     = SrcHeight;
    Src.Palette_p            = NULL;

    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = DstPositionX;
    Dst.Rectangle.PositionY   = DstPositionY;
    Dst.Rectangle.Width       = DstWidth;
    Dst.Rectangle.Height      = DstHeight;
    Dst.Palette_p             = NULL;

    /* ------------ Clear TargetBitmap ------------ */
    /*memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);*/

    /* EnableAntiAliasing feature*/
    STBLIT_EnableAntiAliasing(Handle);

    /* ------------ Blit operation ------------ */
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond() * TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* EnableAntiAliasing feature*/
    STBLIT_DisableAntiAliasing(Handle);

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_BLIT( S(%d,%d,%d,%d) D(%d,%d,%d,%d) )=",TestIndex,SrcPositionX,SrcPositionY,SrcWidth,SrcHeight,
                                                                        DstPositionX,DstPositionY,DstWidth,DstHeight));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}

/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeaturesAntiFlutter
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static BOOL BLIT_TestFeaturesAntiFlutter (U32                   SrcPositionX,
                                       U32                   SrcPositionY,
                                       U32                   SrcWidth,
                                       U32                   SrcHeight,
                                       U32                   DstPositionX,
                                       U32                   DstPositionY,
                                       U32                   DstWidth,
                                       U32                   DstHeight,
                                       STBLIT_Handle_t       Handle,
                                       STGXOBJ_Bitmap_t      SourceBitmap,
                                       STGXOBJ_Bitmap_t      TargetBitmap,
                                       STBLIT_BlitContext_t  Context,
                                       U32                   CRCRefValue,
                                       U32                   TestIndex,
                                       FILE*                 CRCfstream)

{
    ST_ErrorCode_t          Err;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
#ifndef STBLIT_EMULATOR
    STOS_Clock_t            time;
#endif
    U32                     CRCValue;
    BOOL                    ReportTestResult;


    /* ------------ Set Source ------------ */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &SourceBitmap;
    Src.Rectangle.PositionX  = SrcPositionX;
    Src.Rectangle.PositionY  = SrcPositionY;
    Src.Rectangle.Width      = SrcWidth;
    Src.Rectangle.Height     = SrcHeight;
    Src.Palette_p            = NULL;

    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Rectangle.PositionX   = DstPositionX;
    Dst.Rectangle.PositionY   = DstPositionY;
    Dst.Rectangle.Width       = DstWidth;
    Dst.Rectangle.Height      = DstHeight;
    Dst.Palette_p             = NULL;

    /* ------------ Clear TargetBitmap ------------ */
    /*memset(TargetBitmap.Data1_p , 0xFF, TargetBitmap.Size1);*/



    /* EnableAntiFlutter feature*/
    STBLIT_EnableAntiFlutter(Handle);

    /* ------------ Blit operation ------------ */
    Err = STBLIT_Blit(Handle,NULL,&Src,&Dst,&Context );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* ------------ Wait complete event ------------ */
	#ifndef STBLIT_EMULATOR
        #ifdef ST_OSLINUX
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
        #else
                time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
        #endif
        if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
        {
            STTBX_Print(("Timeout%d\n"));
            return (FALSE);
        }
	#else
		/*  Wait for Blit to be completed */
        STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p, TIMEOUT_INFINITY);
	#endif

    /* DisableAntiFlutter feature*/
    STBLIT_DisableAntiFlutter(Handle);

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue = GetBitmapCRC(&TargetBitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Determine test result ------------ */
    Err = TRUE;

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue, 1, 4, CRCfstream);
#else
    /* ------------ Determine test result ------------ */
    if (CRCValue == CRCRefValue)
    {
        Err = TRUE;
    }
    else
    {
        Err = FALSE;
    }
#endif

    /* ------------ Determine report result status ------------ */
#ifdef TEST_FEATURES_REPORT_ONLY_FAILED_TESTS
    ReportTestResult = !Err;
#else
    ReportTestResult = TRUE;
#endif

    /* ------------ Report result ------------ */
    if (ReportTestResult)
    {
        STTBX_Print (("[Test %d] STBLIT_BLIT( S(%d,%d,%d,%d) D(%d,%d,%d,%d) )=",TestIndex,SrcPositionX,SrcPositionY,SrcWidth,SrcHeight,
                                                                        DstPositionX,DstPositionY,DstWidth,DstHeight));
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
        STTBX_Print (("%x\n",CRCValue));
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
        if (Err)
        {
            STTBX_Print (("OK\n"));
        }
        else
        {
            STTBX_Print (("FAILED: Val=%x Ref=%x\n",CRCValue, CRCRefValue));
        }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */
    }

    return (Err);
}


/*-----------------------------------------------------------------------------
 * Function : BLIT_InitReferenceFile
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static ST_ErrorCode_t BLIT_InitReferenceFile (char*  FileName,
                                              FILE** CRCfstream,
                                              U32*   CRCValuesTab)
{
    ST_ErrorCode_t          Err = ST_NO_ERROR;

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Open Reference File for write------------ */
    STTBX_Print (("-> Open CRC file\n"));
    (*CRCfstream) = fopen(FileName, "wb");
    if( (*CRCfstream) == NULL )
    {
        STTBX_Report((STTBX_REPORT_LEVEL_ERROR,"Unable to open \'%s\'\n", FileName ));
        Err = TRUE;
    }
#else /* !TEST_FEATURES_SAVE_CRC_VALUES */
    /* ------------ Read Reference File ------------ */
    STTBX_Print (("-> Read Reference File\n"));
    Err = Blit_LoadReferenceFile(FileName, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Read Reference File : %d\n",Err));
        Err = TRUE;
    }
#endif /* TEST_FEATURES_SAVE_CRC_VALUES */

    return (Err);
}

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
/*-----------------------------------------------------------------------------
 * Function : BLIT_TermReferenceFile
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static void BLIT_TermReferenceFile (FILE* CRCfstream)
{
    U32                     Temp;

    /* Write DEFAULT_EOF_VALUE */
    Temp = DEFAULT_EOF_VALUE;
    fwrite((void *)&Temp, 1, 4, CRCfstream);

    /* Close file handle */
    STTBX_Print (("\n-> Close CRC file\n"));
    fclose (CRCfstream);
}
#endif



/*-----------------------------------------------------------------------------
 * Function : BLIT_TestFeatures
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestFeatures (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;

    STGXOBJ_Bitmap_t        SourceBitmap,SourceBitmap1;
#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
    STGXOBJ_Bitmap_t        TempBitmap, TargetBitmap2, TargetBitmap3;
#endif
    STGXOBJ_Palette_t       Palette;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams, EvtJobSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID, JobSubscriberID;

    U32                        i, j;
    U32                        NbTests_Copy_Run = 0;
    U32                        NbTests_Fill_Run = 0;
    U32                        NbTests_Overlap_Copy_Run = 0;
    U32                        NbTests_Overlap_Blit_Run = 0;
    U32                        NbTests_Bitmap_Blend_Run = 0;
    U32                        NbTests_Fill_Blend_Run = 0;
    U32                        NbTests_Copy_2_src_Run = 0;
    U32                        NbTests_Clip_Run = 0;
    U32                        NbTests_Resize_Run = 0;
    U32                        NbTests_Color_Conversion_Run = 0;
    U32                        NbTests_Job_Run = 0;
    U32                        NbTests_AntiAliasing_Run = 0;
    U32                        NbTests_AntiFlutter_Run = 0;
    U32                        NbTests_SetPixel_Run = 0;
    U32                        NbTests_DrawLine_Run = 0;
    U32                        NbTests_Copy_Passed = 0;
    U32                        NbTests_Fill_Passed = 0;
    U32                        NbTests_Overlap_Copy_Passed = 0;
    U32                        NbTests_Overlap_Blit_Passed = 0;
    U32                        NbTests_Bitmap_Blend_Passed = 0;
    U32                        NbTests_Fill_Blend_Passed = 0;
    U32                        NbTests_Copy_2_src_Passed = 0;
    U32                        NbTests_Clip_Passed = 0;
    U32                        NbTests_Resize_Passed = 0;
    U32                        NbTests_Color_Conversion_Passed = 0;
    U32                        NbTests_Job_Passed = 0;
    U32                        NbTests_AntiAliasing_Passed = 0;
    U32                        NbTests_AntiFlutter_Passed = 0;
    U32                        NbTests_SetPixel_Passed = 0;
    U32                        NbTests_DrawLine_Passed = 0;

#if defined (ST_7109) || defined (ST_7200) || defined (ST_7100)
    U32                        NbTests_Mask_Word_Run = 0;
    U32                        NbTests_Mask_Word_Passed = 0;
    U32                        MaskWordValue;

    U32                        NbTests_Color_Key_Run = 0;
    U32                        NbTests_Color_Key_Passed = 0;

    U32                        NbTests_ALU_Run = 0;
    U32                        NbTests_ALU_Passed = 0;

    U32                        NbTests_VC1_Range_Run = 0;
    U32                        NbTests_VC1_Range_Passed = 0;
#endif

    U32                        CRCValuesTab[TEST_FEATURES_NB_TESTS];
    BOOL                       TestResult;

    STBLIT_JobParams_t         Job_Params;
    STBLIT_JobHandle_t         Job1;
    STBLIT_JobBlitHandle_t     JBHandle_1, JBHandle_2, JBHandle_3;

    FILE*                      CRCfstream;


    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                       = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                       = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188)|| defined (ST_5525)\
 || defined (ST_5107) || defined(ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 500;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 500;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 6;

/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR  &&  Err != ST_ERROR_ALREADY_INITIALIZED)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }


    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ----- Set Job Params ----*/
    Job_Params.NotifyJobCompletion = TRUE;


    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);
    JobCompletionSemaphore_p  = STOS_SemaphoreCreateFifoTimeOut(NULL,0);


    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);


    /* ------------ Subscribe to Job Completed event ---------------- */
    EvtJobSubscribeParams.NotifyCallback   = JobCompletedHandler;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle, Name,STBLIT_JOB_COMPLETED_EVT,&EvtJobSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Job completion : %d\n",Err));
        return (TRUE);
    }

    /* Get Job Subscriber ID */
    STEVT_GetSubscriberID(EvtHandle,&JobSubscriberID);
    Job_Params.EventSubscriberID = SubscriberID;

    /* ------------ Set Src ------------ */
    STTBX_Print (("-> Load source 1\n"));
    SourceBitmap1.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE2_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }


    STTBX_Print (("-> Load source 2\n"));
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE1_FILE_NAME,SrcPartitionInfo,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
    /* ------------ Set Dst ------------ */
    STTBX_Print (("-> Load destination\n"));
    TempBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_TARGET1_FILE_NAME,DstPartitionInfo,&TempBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    TargetBitmap = TempBitmap;
#endif /* STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST */


    /* ------------ Init CRCValue ------------ */
    STTBX_Print (("-> Init Crc32 table\n"));
    Blit_InitCrc32table();

    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;
    Context.EventSubscriberID       = SubscriberID;



#ifndef TEST_FEATURES_NO_COLOR_CONVERSION
    /*=======================================================================
            Test10: Test Color Conversion
      =======================================================================*/
    /*
     ARGB8888    -->       YCbCr422R

     RGB888      -->       ARGB8888
     RGB888      -->       YCbCr422R

     RGB565      -->       ARGB8888
     RGB565      -->       YCbCr422R

     YCbCr422R   -->       ARGB8888          Note: this conversion is not supported for 51xx

     420MB       -->       ARGB8888
     420MB       -->       YCbCr422R

     CLUT8       -->       ARGB8888
     CLUT8       -->       YCbCr422R

     CLUT4       -->       ARGB8888
     CLUT4       -->       YCbCr422R

     ARGB8888    -->       CLUT8             Note: this conversion is supported only for 7100
     YCbCr422R   -->       CLUT8             Note: this conversion is supported only for 7100
    */

    if ((TargetBitmap.ColorType != STGXOBJ_COLOR_TYPE_UNSIGNED_YCBCR888_420) &&
        (TargetBitmap.ColorType != STGXOBJ_COLOR_TYPE_CLUT2))
    {
        STTBX_Print (("===================================\n"));
        STTBX_Print (("Test10: Color Conversion\n"));
        STTBX_Print (("===================================\n"));

        /* ------------ Init Test Counter ------------ */
        NbTests_Color_Conversion_Run    = 0;
        NbTests_Color_Conversion_Passed = 0;

        /* ------------ Init Reference File ------------ */
        Err = BLIT_InitReferenceFile (TEST_FEATURES_COLOR_CONVERSION_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
            return (TRUE);
        }


#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
        /* ------------ Set destination 2 ------------ */
        STTBX_Print (("-> Load destination 2\n"));
        TargetBitmap2.Data1_p = NULL;
        Palette.Data_p     = NULL;

        Err = ConvertGammaToBitmap(TEST_FEATURES_TARGET2_FILE_NAME,DstPartitionInfo,&TargetBitmap2,&Palette);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }

#endif /* STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST */

        /* ------------ Set Context ------------ */
        Context.AluMode                 = STBLIT_ALU_COPY;
        Context.EnableClipRectangle     = FALSE;

#if !defined(ST_7100)
        /* ------------ Test ARGB8888 --> YCbCr422R conversion ------------ */
    /* ------------ Set Src ------------ */
    STTBX_Print (("-> Free source1\n"));
    GUTIL_Free(SourceBitmap1.Data1_p);

    STTBX_Print (("-> Load new source1\n"));
    SourceBitmap1.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE2_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
        STTBX_Print (("\n--- Test conversion from %s to %s\n",BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap2.ColorType]));
        TestResult = BLIT_TestFeaturesColorConversion (Handle,SourceBitmap1,Palette,TargetBitmap2,Context,CRCValuesTab[NbTests_Color_Conversion_Run++],NbTests_Color_Conversion_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Conversion_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(ColorConversion_ARGB8888_to_YCbCr422R_ResultingBitmap_FileName,&TargetBitmap2,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif

        }
#endif
#endif

        /* ------------ Test RGB888 conversion ------------ */
        STTBX_Print (("-> Free source1\n"));
        GUTIL_Free(SourceBitmap1.Data1_p);

        STTBX_Print (("-> Load new source1\n"));
        SourceBitmap1.Data1_p = NULL;
        Palette.Data_p       = NULL;

        Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE_RGB888_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }

        STTBX_Print (("\n--- Test conversion from %s to %s\n",BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap.ColorType]));
        TestResult = BLIT_TestFeaturesColorConversion (Handle,SourceBitmap1,Palette,TargetBitmap,Context,CRCValuesTab[NbTests_Color_Conversion_Run++],NbTests_Color_Conversion_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Conversion_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(ColorConversion_RGB888_to_ARGB8888_ResultingBitmap_FileName,&TargetBitmap,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif

        }

#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
        STTBX_Print (("\n--- Test conversion from %s to %s\n",BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap2.ColorType]));
        TestResult = BLIT_TestFeaturesColorConversion (Handle,SourceBitmap1,Palette,TargetBitmap2,Context,CRCValuesTab[NbTests_Color_Conversion_Run++],NbTests_Color_Conversion_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Conversion_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(ColorConversion_RGB888_to_YCbCr422R_ResultingBitmap_FileName,&TargetBitmap2,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif

        }
#endif

        /* ------------ Test RGB565 conversion ------------ */
        STTBX_Print (("-> Free source1\n"));
        GUTIL_Free(SourceBitmap1.Data1_p);

        STTBX_Print (("-> Load new source1\n"));
        SourceBitmap1.Data1_p = NULL;
        Palette.Data_p       = NULL;

        Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE_RGB565_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }

        STTBX_Print (("\n--- Test conversion from %s to %s\n",BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap.ColorType]));
        TestResult = BLIT_TestFeaturesColorConversion (Handle,SourceBitmap1,Palette,TargetBitmap,Context,CRCValuesTab[NbTests_Color_Conversion_Run++],NbTests_Color_Conversion_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Conversion_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(ColorConversion_RGB565_to_ARGB8888_ResultingBitmap_FileName,&TargetBitmap,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
        }

#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
        STTBX_Print (("\n--- Test conversion from %s to %s\n",BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap2.ColorType]));
        TestResult = BLIT_TestFeaturesColorConversion (Handle,SourceBitmap1,Palette,TargetBitmap2,Context,CRCValuesTab[NbTests_Color_Conversion_Run++],NbTests_Color_Conversion_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Conversion_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(ColorConversion_RGB565_to_YCbCr422R_ResultingBitmap_FileName,&TargetBitmap2,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
        }
#endif


#if !defined (ST_5105)
        /* ------------ Test YCbCr422R --> ARGB8888 conversion ------------ */
        STTBX_Print (("-> Free source1\n"));
        GUTIL_Free(SourceBitmap1.Data1_p);

        STTBX_Print (("-> Load new source1\n"));
        SourceBitmap1.Data1_p = NULL;
        Palette.Data_p       = NULL;

        Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE_422R_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }

        STTBX_Print (("\n--- Test conversion from %s to %s\n",BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap.ColorType]));
        TestResult = BLIT_TestFeaturesColorConversion (Handle,SourceBitmap1,Palette,TargetBitmap,Context,CRCValuesTab[NbTests_Color_Conversion_Run++],NbTests_Color_Conversion_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Conversion_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(ColorConversion_YCbCr422R_to_ARGB8888_ResultingBitmap_FileName,&TargetBitmap,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
        }

        /* ------------ Test YCbCr420MB conversion ------------ */
        STTBX_Print (("-> Free source1\n"));
        GUTIL_Free(SourceBitmap1.Data1_p);

        STTBX_Print (("-> Load new source1\n"));
        SourceBitmap1.Data1_p = NULL;
        Palette.Data_p       = NULL;

        Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE_420MB_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#if !defined(ST_7100)
        STTBX_Print (("\n--- Test conversion from %s to %s\n",BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap.ColorType]));
        TestResult = BLIT_TestFeaturesColorConversion (Handle,SourceBitmap1,Palette,TargetBitmap,Context,CRCValuesTab[NbTests_Color_Conversion_Run++],NbTests_Color_Conversion_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Conversion_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(ColorConversion_MB_to_ARGB8888_ResultingBitmap_FileName,&TargetBitmap,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
        }
#endif

#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
        STTBX_Print (("\n--- Test conversion from %s to %s\n",BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap2.ColorType]));
        TestResult = BLIT_TestFeaturesColorConversion (Handle,SourceBitmap1,Palette,TargetBitmap2,Context,CRCValuesTab[NbTests_Color_Conversion_Run++],NbTests_Color_Conversion_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Conversion_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(ColorConversion_MB_to_YCbCr422R_ResultingBitmap_FileName,&TargetBitmap2,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
        }

#endif

#endif /* !5105 */

        /* ------------ Test CLUT8 conversion ------------ */
        STTBX_Print (("-> Free source1\n"));
        GUTIL_Free(SourceBitmap1.Data1_p);

        STTBX_Print (("-> Load new source1\n"));
        SourceBitmap1.Data1_p = NULL;
        Palette.Data_p       = NULL;

        Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE_CLUT8_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }

        STTBX_Print (("\n--- Test conversion from %s to %s\n",BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap.ColorType]));
        TestResult = BLIT_TestFeaturesColorConversion (Handle,SourceBitmap1,Palette,TargetBitmap,Context,CRCValuesTab[NbTests_Color_Conversion_Run++],NbTests_Color_Conversion_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Conversion_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(ColorConversion_CLUT8_to_ARGB8888_ResultingBitmap_FileName,&TargetBitmap,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
        }

#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
        STTBX_Print (("\n--- Test conversion from %s to %s\n",BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap2.ColorType]));
        TestResult = BLIT_TestFeaturesColorConversion (Handle,SourceBitmap1,Palette,TargetBitmap2,Context,CRCValuesTab[NbTests_Color_Conversion_Run++],NbTests_Color_Conversion_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Conversion_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(ColorConversion_CLUT8_to_YCbCr422R_ResultingBitmap_FileName,&TargetBitmap2,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif

        }
#endif


        /* ------------ Test CLUT4 conversion ------------ */
        STTBX_Print (("-> Free source1\n"));
        GUTIL_Free(SourceBitmap1.Data1_p);

        STTBX_Print (("-> Load new source1\n"));
        SourceBitmap1.Data1_p = NULL;
        Palette.Data_p       = NULL;

        Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE_CLUT4_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }

        STTBX_Print (("\n--- Test conversion from %s to %s\n",BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap.ColorType]));
        TestResult = BLIT_TestFeaturesColorConversion (Handle,SourceBitmap1,Palette,TargetBitmap,Context,CRCValuesTab[NbTests_Color_Conversion_Run++],NbTests_Color_Conversion_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Conversion_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(ColorConversion_CLUT4_to_ARGB8888_ResultingBitmap_FileName,&TargetBitmap,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
        }

#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
        STTBX_Print (("\n--- Test conversion from %s to %s\n",BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap2.ColorType]));
        TestResult = BLIT_TestFeaturesColorConversion (Handle,SourceBitmap1,Palette,TargetBitmap2,Context,CRCValuesTab[NbTests_Color_Conversion_Run++],NbTests_Color_Conversion_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Conversion_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(ColorConversion_CLUT4_to_YCbCr422R_ResultingBitmap_FileName,&TargetBitmap2,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif

        }
#endif



#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
        /* ------------ Free destination 2 ------------ */
        STTBX_Print (("-> Free destination 2\n"));
        GUTIL_Free(TargetBitmap2.Data1_p);

        /* ------------ Free destination 3 ------------ */
        STTBX_Print (("-> Free destination 3\n"));
        GUTIL_Free(TargetBitmap3.Data1_p);

#endif
    }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif

#if defined(ST_7109) || defined(ST_7200)

#ifndef TEST_FEATURES_NO_VC1_RANGE
    /*=======================================================================
            Test19: Test VC1 Range
      =======================================================================*/

        STTBX_Print (("===================================\n"));
        STTBX_Print (("Test19: VC1 range\n"));
        STTBX_Print (("===================================\n"));

        /* ------------ Init Test Counter ------------ */
        NbTests_VC1_Range_Run    = 0;
        NbTests_VC1_Range_Passed = 0;

        /* ------------ Init Reference File ------------ */
        Err = BLIT_InitReferenceFile (TEST_FEATURES_VC1_RANGE_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
            return (TRUE);
        }

        /* ------------ Set Context ------------ */
        Context.AluMode                 = STBLIT_ALU_COPY;
        Context.EnableClipRectangle     = FALSE;

        STTBX_Print (("-> Free source1\n"));
        GUTIL_Free(SourceBitmap1.Data1_p);

        STTBX_Print (("-> Load new source1\n"));
        SourceBitmap1.Data1_p = NULL;
        Palette.Data_p       = NULL;

        Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE_420MB_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }


#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
        /* ------------ Set destination 2 ------------ */
        STTBX_Print (("-> Load destination 2\n"));
        TargetBitmap2.Data1_p = NULL;
        Palette.Data_p     = NULL;

        Err = ConvertGammaToBitmap(TEST_FEATURES_TARGET2_FILE_NAME, DstPartitionInfo, &TargetBitmap2, &Palette);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif /* STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST */


        /* ------------ Test VC1 Mapping ------------ */
        STTBX_Print (("\n--- Test range mapping for %s to %s\n", BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap2.ColorType]));
        TestResult = BLIT_TestFeaturesVC1Range (Handle,SourceBitmap1,Palette,TargetBitmap2, YUV_RANGE_MAP_6, YUV_RANGE_MAP_6,Context,CRCValuesTab[NbTests_VC1_Range_Run ++],NbTests_VC1_Range_Run, CRCfstream);

        if (TestResult)
        {
            NbTests_VC1_Range_Passed ++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(VC1Range_Mapping_MB_to_YCbCr422R_ResultingBitmap_FileName,&TargetBitmap2,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
        }

        /* ------------ Test VC1 reduction ------------ */
        STTBX_Print (("\n--- Test range reduction for %s to %s\n",BitmapColorType[SourceBitmap1.ColorType],BitmapColorType[TargetBitmap2.ColorType]));
        TestResult = BLIT_TestFeaturesVC1Range (Handle,SourceBitmap1,Palette,TargetBitmap2, YUV_RANGE_MAP_7, YUV_RANGE_MAP_7,Context,CRCValuesTab[NbTests_VC1_Range_Run ++],NbTests_VC1_Range_Run, CRCfstream);

        if (TestResult)
        {
            NbTests_VC1_Range_Passed ++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(VC1Range_Reduction_MB_to_YCbCr422R_ResultingBitmap_FileName,&TargetBitmap2,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
        }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
        /* ------------ Free destination 2 ------------ */
        STTBX_Print (("-> Free destination 2\n"));
        GUTIL_Free(TargetBitmap2.Data1_p);
#endif
#endif
#endif /*defined (ST_7109) || defined (ST_7200)*/



#ifndef TEST_FEATURES_NO_COPY
    /*=======================================================================
            Test1: Test copy
      =======================================================================*/
    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test1: Copy\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_Copy_Run = 0;
    NbTests_Copy_Passed = 0;

    /* ------------ Set Src ------------ */
    STTBX_Print (("-> Load source 1\n"));
    SourceBitmap1.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE2_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }


    STTBX_Print (("-> Load source 2\n"));
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE1_FILE_NAME,SrcPartitionInfo,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
    /* ------------ Set Dst ------------ */
    STTBX_Print (("-> Load destination\n"));
    TempBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_TARGET1_FILE_NAME,DstPartitionInfo,&TempBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    TargetBitmap = TempBitmap;
#endif /* STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST */


    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_COPY_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }

    /* Test full image copy */
    STTBX_Print (("\n--- Test full image copy\n"));
    TestResult = BLIT_TestFeaturesCopy (0,0,SourceBitmap.Width,SourceBitmap.Height,0,0,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_Run++],NbTests_Copy_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_Copy_Passed++;

#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(Copy_ResultingBitmap_FileName,&TargetBitmap,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
    }
    /* Test trivial Src positions */
    STTBX_Print (("\n--- Test trivial Src positions\n"));
    TestResult = BLIT_TestFeaturesCopy (0,0,1,1,0,0,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_Run++],NbTests_Copy_Run, CRCfstream);
    if (TestResult)
            NbTests_Copy_Passed++;

    TestResult = BLIT_TestFeaturesCopy (1,0,1,1,0,0,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_Run++],NbTests_Copy_Run, CRCfstream);
    if (TestResult)
            NbTests_Copy_Passed++;

    TestResult = BLIT_TestFeaturesCopy (1,1,1,1,0,0,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_Run++],NbTests_Copy_Run, CRCfstream);
    if (TestResult)
            NbTests_Copy_Passed++;


    /* Test trivial Dst positions */
    STTBX_Print (("\n---- Test trivial Dst positions\n"));
    TestResult = BLIT_TestFeaturesCopy (0,0,1,1,0,0,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_Run++],NbTests_Copy_Run, CRCfstream);
    if (TestResult)
            NbTests_Copy_Passed++;

    TestResult = BLIT_TestFeaturesCopy (0,0,1,1,1,0,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_Run++],NbTests_Copy_Run, CRCfstream);
    if (TestResult)
            NbTests_Copy_Passed++;

    TestResult = BLIT_TestFeaturesCopy (0,0,1,1,1,1,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_Run++],NbTests_Copy_Run, CRCfstream);
    if (TestResult)
            NbTests_Copy_Passed++;

    /* Test Src positions X & Y */
    STTBX_Print (("\n--- Test Src positions X & Y\n"));
    for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesCopy (i,j,100,100,20,20,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_Run++],NbTests_Copy_Run, CRCfstream);
            if (TestResult)
                    NbTests_Copy_Passed++;
        }
    }

    /* Test Dst positions X & Y */
    STTBX_Print (("\n--- Test Dst positions X & Y\n"));
    for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesCopy (20,20,100,100,i,j,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_Run++],NbTests_Copy_Run, CRCfstream);
            if (TestResult)
                    NbTests_Copy_Passed++;
        }
    }

    /* Test Small Src Width & Height */
    STTBX_Print (("\n--- Test Small Src Width & Height\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesCopy (20,20,i,j,20,20,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_Run++],NbTests_Copy_Run, CRCfstream);
            if (TestResult)
                    NbTests_Copy_Passed++;

        }
    }

    /* Test Large Src Width & Height */
    STTBX_Print (("\n--- Test Large Src Width & Height\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesCopy (20,20,(300 + i),(300 + j),20,20,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_Run++],NbTests_Copy_Run, CRCfstream);
            if (TestResult)
                    NbTests_Copy_Passed++;

        }
    }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif



#endif

#ifndef TEST_FEATURES_NO_FILL
    /*=======================================================================
            Test2: Test fill
      =======================================================================*/
    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test2: Fill\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_Fill_Run = 0;
    NbTests_Fill_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_FILL_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }

    /* Test full image fill */
    STTBX_Print (("\n--- Test full image fill\n"));
    TestResult = BLIT_TestFeaturesFill (0,0,SourceBitmap.Width,SourceBitmap.Height,0xff,0,0,Handle,TargetBitmap,Context,CRCValuesTab[NbTests_Fill_Run++],NbTests_Fill_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_Fill_Passed++;

#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(Fill_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif
    }

    /* Test trivial fill positions */
    STTBX_Print (("\n--- Test trivial fill positions\n"));
    TestResult = BLIT_TestFeaturesFill (0,0,1,1,0xff,0,0,Handle,TargetBitmap,Context,CRCValuesTab[NbTests_Fill_Run++],NbTests_Fill_Run, CRCfstream);
    if (TestResult)
            NbTests_Fill_Passed++;

    TestResult = BLIT_TestFeaturesFill (1,0,1,1,0xff,0,0,Handle,TargetBitmap,Context,CRCValuesTab[NbTests_Fill_Run++],NbTests_Fill_Run, CRCfstream);
    if (TestResult)
            NbTests_Fill_Passed++;

    TestResult = BLIT_TestFeaturesFill (1,1,1,1,0xff,0,0,Handle,TargetBitmap,Context,CRCValuesTab[NbTests_Fill_Run++],NbTests_Fill_Run, CRCfstream);
    if (TestResult)
            NbTests_Fill_Passed++;

    /* Test positions X & Y */
    STTBX_Print (("\n--- Test positions X & Y\n"));
    for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesFill (i,j,100,100,0xff,20,20,Handle,TargetBitmap,Context,CRCValuesTab[NbTests_Fill_Run++],NbTests_Fill_Run, CRCfstream);
            if (TestResult)
                    NbTests_Fill_Passed++;
        }
    }

    /* Test Small Width & Height */
    STTBX_Print (("\n--- Test Small Width & Height\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesFill (20,20,i,j,0xff,20,20,Handle,TargetBitmap,Context,CRCValuesTab[NbTests_Fill_Run++],NbTests_Fill_Run, CRCfstream);
            if (TestResult)
                    NbTests_Fill_Passed++;

        }
    }

    /* Test Large Width & Height */
    STTBX_Print (("\n--- Test Large Width & Height\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesFill (20,20,(300 + i),(300 + j),0xff,20,20,Handle,TargetBitmap,Context,CRCValuesTab[NbTests_Fill_Run++],NbTests_Fill_Run, CRCfstream);
            if (TestResult)
                    NbTests_Fill_Passed++;

        }
    }


#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif

#if !defined(ST_OSLINUX)
#ifndef TEST_FEATURES_NO_OVERLAP_COPY
    /*=======================================================================
            Test3: Test overlap copy
      =======================================================================*/
    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test3: Overlap Copy\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_Overlap_Copy_Run = 0;
    NbTests_Overlap_Copy_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_OVERLAP_COPY_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }


    /* Case 1 of Overlap copy: source is up left destination */
    STTBX_Print (("\n--- Case 1 of Overlap copy: source is up left destination\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS/2) ; i++)
    {
        for (j=0 ; j < (TEST_FEATURES_NB_STEPS/2) ; j++)
        {
            TestResult = BLIT_TestFeaturesOverlapCopy (i,j,100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Copy_Run++],NbTests_Overlap_Copy_Run, CRCfstream);
            if (TestResult)
            {
                NbTests_Overlap_Copy_Passed++;

#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
                if( (i == ((TEST_FEATURES_NB_STEPS/2) -1)) && (j == ((TEST_FEATURES_NB_STEPS/2) -1)))
                {
                    Err = ConvertBitmapToGamma(OverlapCopy_ResultingBitmap_FileName1,&TargetBitmap,&Palette);
                    fflush(stdout);
                    if (Err != ST_NO_ERROR)
                    {
                        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                        return (TRUE);
                    }
                }
#endif
            }

        }/*For j*/
    }/*For i*/

    /* Case 2 of Overlap copy: source is up middle destination */
    STTBX_Print (("\n--- Case 2 of Overlap copy: source is up middle destination\n"));
    for (j=0 ; j < (TEST_FEATURES_NB_STEPS/2) ; j++)
    {
        TestResult = BLIT_TestFeaturesOverlapCopy (100,j,100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Copy_Run++],NbTests_Overlap_Copy_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Overlap_Copy_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if ( j == ((TEST_FEATURES_NB_STEPS/2) -1) )
            {
                Err = ConvertBitmapToGamma(OverlapCopy_ResultingBitmap_FileName2,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
        }
    }

    /* Case 3 of Overlap copy: source is up right destination  */
    STTBX_Print (("\n--- Case 3 of Overlap copy: source is up right destination\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS/2) ; i++)
    {
        for (j=0 ; j < (TEST_FEATURES_NB_STEPS/2) ; j++)
        {
            TestResult = BLIT_TestFeaturesOverlapCopy ((150+i),j,100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Copy_Run++],NbTests_Overlap_Copy_Run, CRCfstream);

            if (TestResult)
            NbTests_Overlap_Copy_Passed++;

#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if( (i == ((TEST_FEATURES_NB_STEPS/2) -1)) && (j == ((TEST_FEATURES_NB_STEPS/2) -1)))
            {
                Err = ConvertBitmapToGamma(OverlapCopy_ResultingBitmap_FileName3,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif

        }
    }

    /* Case 4 of Overlap copy: source is down right destination  */
    STTBX_Print (("\n--- Case 4 of Overlap copy: source is down right destination\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS/2) ; i++)
    {
        for (j=0 ; j < (TEST_FEATURES_NB_STEPS/2) ; j++)
        {
            TestResult = BLIT_TestFeaturesOverlapCopy ((150+i),(150+j),100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Copy_Run++],NbTests_Overlap_Copy_Run, CRCfstream);
            if (TestResult)
            {
                NbTests_Overlap_Copy_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
                if( (i == ((TEST_FEATURES_NB_STEPS/2) -1)) && (j == ((TEST_FEATURES_NB_STEPS/2) -1)))
                {
                    Err = ConvertBitmapToGamma(OverlapCopy_ResultingBitmap_FileName4,&TargetBitmap,&Palette);
                    fflush(stdout);
                    if (Err != ST_NO_ERROR)
                    {
                        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                        return (TRUE);
                    }
                }
#endif
            }
        }
    }

    /* Case 5 of Overlap copy: source is down middle destination  */
    STTBX_Print (("\n--- Case 5 of Overlap copy: source is down middle destination\n"));
    for (j=0 ; j < (TEST_FEATURES_NB_STEPS/2) ; j++)
    {
        TestResult = BLIT_TestFeaturesOverlapCopy (100,(150+j),100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Copy_Run++],NbTests_Overlap_Copy_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Overlap_Copy_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if( j == ((TEST_FEATURES_NB_STEPS/2) -1) )
            {
                Err = ConvertBitmapToGamma(OverlapCopy_ResultingBitmap_FileName5,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
        }
    }

    /* Case 6 of Overlap copy: source is down left destination  */
    STTBX_Print (("\n--- Case 6 of Overlap copy: source is down left destination\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS/2) ; i++)
    {
        for (j=0 ; j < (TEST_FEATURES_NB_STEPS/2) ; j++)
        {
            TestResult = BLIT_TestFeaturesOverlapCopy (i,(150+j),100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Copy_Run++],NbTests_Overlap_Copy_Run, CRCfstream);
            if (TestResult)
            {
                NbTests_Overlap_Copy_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
                if( (i == ((TEST_FEATURES_NB_STEPS/2) -1)) && (j == ((TEST_FEATURES_NB_STEPS/2) -1)) )
                {
                    Err = ConvertBitmapToGamma(OverlapCopy_ResultingBitmap_FileName6,&TargetBitmap,&Palette);
                    fflush(stdout);
                    if (Err != ST_NO_ERROR)
                    {
                        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                        return (TRUE);
                    }
                }
#endif
            }
        }
    }


    /* Case 7 of Overlap copy: source is middle left destination  */
    STTBX_Print (("\n--- Case 7 of Overlap copy: source is middle left destination\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS/2) ; i++)
    {
        TestResult = BLIT_TestFeaturesOverlapCopy (i,100,100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Copy_Run++],NbTests_Overlap_Copy_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Overlap_Copy_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if( i == ((TEST_FEATURES_NB_STEPS/2) -1) )
            {
                Err = ConvertBitmapToGamma(OverlapCopy_ResultingBitmap_FileName7,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
        }
    }

    /* Case 8 of Overlap copy: source is middle right destination  */
    STTBX_Print (("\n--- Case 8 of Overlap copy: source is middle right destination\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS/2) ; i++)
    {
        TestResult = BLIT_TestFeaturesOverlapCopy ((150+i),100,100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Copy_Run++],NbTests_Overlap_Copy_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Overlap_Copy_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if( i == ((TEST_FEATURES_NB_STEPS/2) -1) )
            {
                Err = ConvertBitmapToGamma(OverlapCopy_ResultingBitmap_FileName8,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
        }
    }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif

#ifndef TEST_FEATURES_NO_OVERLAP_BLIT
    /*=======================================================================
            Test4: Test overlap BLIT
      =======================================================================*/
    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test3: Overlap Blit\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_Overlap_Blit_Run = 0;
    NbTests_Overlap_Blit_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_OVERLAP_BLIT_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }


    /* Case 1 of Overlap Blit: source is up left destination */
    STTBX_Print (("\n--- Case 1 of Overlap Blit: source is up left destination\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS/2) ; i++)
    {
        for (j=0 ; j < (TEST_FEATURES_NB_STEPS/2) ; j++)
        {
            TestResult = BLIT_TestFeaturesOverlapBlit(i,j,100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Blit_Run++],NbTests_Overlap_Blit_Run, CRCfstream);
            if (TestResult)
            {
                NbTests_Overlap_Blit_Passed++;

#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
                if( (i == ((TEST_FEATURES_NB_STEPS/2) -1)) && (j == ((TEST_FEATURES_NB_STEPS/2) -1)))
                {
                    Err = ConvertBitmapToGamma(OverlapBlit_ResultingBitmap_FileName1,&TargetBitmap,&Palette);
                    fflush(stdout);
                    if (Err != ST_NO_ERROR)
                    {
                        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                        return (TRUE);
                    }
                }
#endif
            }

        }/*For j*/
    }/*For i*/

    /* Case 2 of Overlap copy: source is up middle destination */
    STTBX_Print (("\n--- Case 2 of Overlap blit: source is up middle destination\n"));
    for (j=0 ; j < (TEST_FEATURES_NB_STEPS/2) ; j++)
    {
        TestResult = BLIT_TestFeaturesOverlapBlit (100,j,100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Blit_Run++],NbTests_Overlap_Blit_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Overlap_Blit_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if ( j == ((TEST_FEATURES_NB_STEPS/2) -1) )
            {
                Err = ConvertBitmapToGamma(OverlapBlit_ResultingBitmap_FileName2,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
        }
    }

    /* Case 3 of Overlap blit: source is up right destination  */
    STTBX_Print (("\n--- Case 3 of Overlap blit: source is up right destination\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS/2) ; i++)
    {
        for (j=0 ; j < (TEST_FEATURES_NB_STEPS/2) ; j++)
        {
            TestResult = BLIT_TestFeaturesOverlapBlit ((150+i),j,100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Blit_Run++],NbTests_Overlap_Blit_Run, CRCfstream);

            if (TestResult)
            NbTests_Overlap_Blit_Passed++;

#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if( (i == ((TEST_FEATURES_NB_STEPS/2) -1)) && (j == ((TEST_FEATURES_NB_STEPS/2) -1)))
            {
                Err = ConvertBitmapToGamma(OverlapBlit_ResultingBitmap_FileName3,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif

        }
    }

    /* Case 4 of Overlap Blit: source is down right destination  */
    STTBX_Print (("\n--- Case 4 of Overlap Blit: source is down right destination\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS/2) ; i++)
    {
        for (j=0 ; j < (TEST_FEATURES_NB_STEPS/2) ; j++)
        {
            TestResult = BLIT_TestFeaturesOverlapBlit ((150+i),(150+j),100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Blit_Run++],NbTests_Overlap_Blit_Run, CRCfstream);
            if (TestResult)
            {
                NbTests_Overlap_Blit_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
                if( (i == ((TEST_FEATURES_NB_STEPS/2) -1)) && (j == ((TEST_FEATURES_NB_STEPS/2) -1)))
                {
                    Err = ConvertBitmapToGamma(OverlapBlit_ResultingBitmap_FileName4,&TargetBitmap,&Palette);
                    fflush(stdout);
                    if (Err != ST_NO_ERROR)
                    {
                        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                        return (TRUE);
                    }
                }
#endif
            }
        }
    }

    /* Case 5 of Overlap Blit: source is down middle destination  */
    STTBX_Print (("\n--- Case 5 of Overlap Blit: source is down middle destination\n"));
    for (j=0 ; j < (TEST_FEATURES_NB_STEPS/2) ; j++)
    {
        TestResult = BLIT_TestFeaturesOverlapBlit (100,(150+j),100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Blit_Run++],NbTests_Overlap_Blit_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Overlap_Blit_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if( j == ((TEST_FEATURES_NB_STEPS/2) -1) )
            {
                Err = ConvertBitmapToGamma(OverlapBlit_ResultingBitmap_FileName5,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
        }
    }

    /* Case 6 of Overlap Blit: source is down left destination  */
    STTBX_Print (("\n--- Case 6 of Overlap Blit: source is down left destination\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS/2) ; i++)
    {
        for (j=0 ; j < (TEST_FEATURES_NB_STEPS/2) ; j++)
        {
            TestResult = BLIT_TestFeaturesOverlapBlit (i,(150+j),100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Blit_Run++],NbTests_Overlap_Blit_Run, CRCfstream);
            if (TestResult)
            {
                NbTests_Overlap_Blit_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
                if( (i == ((TEST_FEATURES_NB_STEPS/2) -1)) && (j == ((TEST_FEATURES_NB_STEPS/2) -1)) )
                {
                    Err = ConvertBitmapToGamma(OverlapBlit_ResultingBitmap_FileName6,&TargetBitmap,&Palette);
                    fflush(stdout);
                    if (Err != ST_NO_ERROR)
                    {
                        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                        return (TRUE);
                    }
                }
#endif
            }
        }
    }


    /* Case 7 of Overlap Blit: source is middle left destination  */
    STTBX_Print (("\n--- Case 7 of Overlap Blit: source is middle left destination\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS/2) ; i++)
    {
        TestResult = BLIT_TestFeaturesOverlapBlit (i,100,100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Blit_Run++],NbTests_Overlap_Blit_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Overlap_Blit_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if( i == ((TEST_FEATURES_NB_STEPS/2) -1) )
            {
                Err = ConvertBitmapToGamma(OverlapBlit_ResultingBitmap_FileName7,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
        }
    }

    /* Case 8 of Overlap Blit: source is middle right destination  */
    STTBX_Print (("\n--- Case 8 of Overlap Blit: source is middle right destination\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS/2) ; i++)
    {
        TestResult = BLIT_TestFeaturesOverlapBlit ((150+i),100,100,100,100,100,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Overlap_Blit_Run++],NbTests_Overlap_Blit_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Overlap_Blit_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if( i == ((TEST_FEATURES_NB_STEPS/2) -1) )
            {
                Err = ConvertBitmapToGamma(OverlapBlit_ResultingBitmap_FileName8,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
        }
    }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif
#endif

#ifndef TEST_FEATURES_NO_BLEND_BITMAP
    /*=======================================================================
            Test5: Test Bitmap Blend
      =======================================================================*/
    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test5: Bitmap Blend\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_Bitmap_Blend_Run = 0;
    NbTests_Bitmap_Blend_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_BITMAP_BLEND_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Set Context ------------ */
    Context.AluMode                 = STBLIT_ALU_ALPHA_BLEND;

    /* Test full image copy */
    STTBX_Print (("--- Test full screen image\n"));
    TestResult = BLIT_TestFeaturesBitmapBlend (0,0,SourceBitmap.Width,SourceBitmap.Height,0,0,SourceBitmap.Width,SourceBitmap.Height,60,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Bitmap_Blend_Run++],NbTests_Bitmap_Blend_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_Bitmap_Blend_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(BlendBitmap_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif
    }

    /* Test trivial Src positions */
    STTBX_Print (("\n--- Test trivial Src positions\n"));
    TestResult = BLIT_TestFeaturesBitmapBlend (0,0,1,1,0,0,1,1,60,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Bitmap_Blend_Run++],NbTests_Bitmap_Blend_Run, CRCfstream);
    if (TestResult)
            NbTests_Bitmap_Blend_Passed++;

    TestResult = BLIT_TestFeaturesBitmapBlend (1,0,1,1,0,0,1,1,60,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Bitmap_Blend_Run++],NbTests_Bitmap_Blend_Run, CRCfstream);
    if (TestResult)
            NbTests_Bitmap_Blend_Passed++;

    TestResult = BLIT_TestFeaturesBitmapBlend (1,1,1,1,0,0,1,1,60,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Bitmap_Blend_Run++],NbTests_Bitmap_Blend_Run, CRCfstream);
    if (TestResult)
            NbTests_Bitmap_Blend_Passed++;


    /* Test trivial Dst positions */
    STTBX_Print (("\n---- Test trivial Dst positions\n"));
    TestResult = BLIT_TestFeaturesBitmapBlend (0,0,1,1,0,0,1,1,60,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Bitmap_Blend_Run++],NbTests_Bitmap_Blend_Run, CRCfstream);
    if (TestResult)
            NbTests_Bitmap_Blend_Passed++;

    TestResult = BLIT_TestFeaturesBitmapBlend (0,0,1,1,1,0,1,1,60,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Bitmap_Blend_Run++],NbTests_Bitmap_Blend_Run, CRCfstream);
    if (TestResult)
            NbTests_Bitmap_Blend_Passed++;

    TestResult = BLIT_TestFeaturesBitmapBlend (0,0,1,1,1,1,1,1,60,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Bitmap_Blend_Run++],NbTests_Bitmap_Blend_Run, CRCfstream);
    if (TestResult)
            NbTests_Bitmap_Blend_Passed++;

    /* Test Src positions X & Y */
    STTBX_Print (("\n--- Test Src positions X & Y\n"));
    for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesBitmapBlend (i,j,100,100,20,20,100,100,60,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Bitmap_Blend_Run++],NbTests_Bitmap_Blend_Run, CRCfstream);
            if (TestResult)
                    NbTests_Bitmap_Blend_Passed++;
        }
    }

    /* Test Dst positions X & Y */
    STTBX_Print (("\n--- Test Dst positions X & Y\n"));
    for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesBitmapBlend (20,20,100,100,i,j,100,100,60,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Bitmap_Blend_Run++],NbTests_Bitmap_Blend_Run, CRCfstream);
            if (TestResult)
                    NbTests_Bitmap_Blend_Passed++;
        }
    }

    /* Test Small Src Width & Height */
    STTBX_Print (("\n--- Test Small Src Width & Height\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesBitmapBlend (20,20,i,j,20,20,i,j,60,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Bitmap_Blend_Run++],NbTests_Bitmap_Blend_Run, CRCfstream);
            if (TestResult)
                    NbTests_Bitmap_Blend_Passed++;

        }
    }

    /* Test Large Src Width & Height */
    STTBX_Print (("\n--- Test Large Src Width & Height\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesBitmapBlend (20,20,(300 + i),(300+ j),20,20,(300 + i),(300 + j),60,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Bitmap_Blend_Run++],NbTests_Bitmap_Blend_Run, CRCfstream);
            if (TestResult)
                    NbTests_Bitmap_Blend_Passed++;

        }
    }


    /* Test alpha value */
    STTBX_Print (("\n--- Test alpha value\n"));
    for (i=0 ; i <= 12 ; i++)
    {
        TestResult = BLIT_TestFeaturesBitmapBlend (20,20,100,100,20,20,100,100,(i*10),Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Bitmap_Blend_Run++],NbTests_Bitmap_Blend_Run, CRCfstream);
        if (TestResult)
                NbTests_Bitmap_Blend_Passed++;
    }


#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif

#if ((!defined(ST_OSLINUX)) ||(defined(STBLIT_LINUX_FULL_USER_VERSION)))
#ifndef TEST_FEATURES_NO_BLEND_COLOR
    /*=======================================================================
            Test6: Test fill blend
      =======================================================================*/

    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test6: Fill Blend\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_Fill_Blend_Run = 0;
    NbTests_Fill_Blend_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_FILL_BLEND_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Set Context ------------ */
    Context.AluMode                 = STBLIT_ALU_ALPHA_BLEND;

    {
        STGXOBJ_Rectangle_t Rect;
        STGXOBJ_ColorARGB_t Color;

        Rect.PositionX = 0;
        Rect.PositionY = 0;
        Rect.Width = SourceBitmap.Width;
        Rect.Height = SourceBitmap.Height;

        Color.R = 0xFF;
        Color.G = 0;
        Color.B = 0;
        Color.Alpha = 60;


        /* Test full image fill */
        STTBX_Print (("--- Test full image fill\n"));
        TestResult = BLIT_TestFeaturesFillBlend (Rect, Color, Handle,TargetBitmap,SubscriberID,
                CRCValuesTab[NbTests_Fill_Blend_Run++],NbTests_Fill_Blend_Run, CRCfstream);

        if (TestResult)
        {
            NbTests_Fill_Blend_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(BlendColor_ResultingBitmap_FileName,&TargetBitmap,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
        }


        /* Test trivial fill positions */
        STTBX_Print (("\n--- Test trivial fill positions\n"));


        Rect.PositionX = 0;
        Rect.PositionY = 0;
        Rect.Width = 1;
        Rect.Height = 1;



        TestResult = BLIT_TestFeaturesFillBlend (Rect, Color, Handle,TargetBitmap,SubscriberID,
                CRCValuesTab[NbTests_Fill_Blend_Run++],NbTests_Fill_Blend_Run, CRCfstream);


        Rect.PositionX = 1;
        Rect.PositionY = 0;

        if (TestResult)
                NbTests_Fill_Blend_Passed++;



        TestResult = BLIT_TestFeaturesFillBlend (Rect, Color,Handle,TargetBitmap, SubscriberID, CRCValuesTab[NbTests_Fill_Blend_Run++],
                NbTests_Fill_Blend_Run, CRCfstream);
        if (TestResult)
                NbTests_Fill_Blend_Passed++;

        Rect.PositionX = 1;
        Rect.PositionY = 1;


        TestResult = BLIT_TestFeaturesFillBlend (Rect, Color,Handle,TargetBitmap, SubscriberID, CRCValuesTab[NbTests_Fill_Blend_Run++],
                    NbTests_Fill_Blend_Run, CRCfstream);
        if (TestResult)
                NbTests_Fill_Blend_Passed++;
    }


    /* Test positions X & Y */
    STTBX_Print (("\n--- Test positions X & Y\n"));
    {
        STGXOBJ_Rectangle_t Rect;
        STGXOBJ_ColorARGB_t Color;
        for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
        {
            for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
            {
                STOS_TaskDelayUs(1);
                Rect.PositionX = i;
                Rect.PositionY = j;
                Rect.Width = 100;
                Rect.Height = 100;
                Color.R = 0xFF;
                Color.G = 20;
                Color.B = 20;
                Color.Alpha = 60;

                TestResult = BLIT_TestFeaturesFillBlend ( Rect,Color,Handle,TargetBitmap,SubscriberID, CRCValuesTab[NbTests_Fill_Blend_Run++],NbTests_Fill_Blend_Run, CRCfstream);
                if (TestResult)
                {
                        NbTests_Fill_Blend_Passed++;
                }
            }
        }
    }

    /* Test Small Width & Height */
    STTBX_Print (("\n--- Test Small Width & Height\n"));
    {
        STGXOBJ_Rectangle_t Rect;
        STGXOBJ_ColorARGB_t Color;

        for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
        {
            for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
            {
                STOS_TaskDelayUs(1);
                Rect.PositionX = 20;
                Rect.PositionY = 20;
                Rect.Width = i;
                Rect.Height = j;
                Color.R = 0xff;
                Color.G = 20;
                Color.B = 20;
                Color.Alpha = 60;

                /*NbTests_Fill_Blend_Run++;*/
                TestResult = BLIT_TestFeaturesFillBlend (Rect,Color,Handle,TargetBitmap,SubscriberID, CRCValuesTab[NbTests_Fill_Blend_Run++],NbTests_Fill_Blend_Run, CRCfstream);

                if (TestResult)
                {
                        NbTests_Fill_Blend_Passed++;
                }
            }
        }
    }

    /* Test Large Width & Height */
    STTBX_Print (("\n--- Test Large Width & Height\n"));
    {
        STGXOBJ_Rectangle_t Rect;
        STGXOBJ_ColorARGB_t Color;

        for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
        {
            for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
            {
                STOS_TaskDelayUs(1);
                Rect.PositionX = 20;
                Rect.PositionY = 20;
                Rect.Width = 300+i;
                Rect.Height = 300+j;
                Color.R = 0xFF;
                Color.G = 20;
                Color.B = 20;
                Color.Alpha = 60;

                TestResult = BLIT_TestFeaturesFillBlend (Rect,Color,Handle,TargetBitmap,SubscriberID, CRCValuesTab[NbTests_Fill_Blend_Run++],NbTests_Fill_Blend_Run, CRCfstream);
                if (TestResult)
                {
                    NbTests_Fill_Blend_Passed++;
                }
            }
        }
    }
    /* Test alpha value */
    STTBX_Print (("\n--- Test alpha value\n"));
    {
        STGXOBJ_Rectangle_t Rect;
        STGXOBJ_ColorARGB_t Color;

        for (i=0 ; i <= 12 ; i++)
        {
            Rect.PositionX = 20;
            Rect.PositionY = 20;
            Rect.Width = 100;
            Rect.Height = 100;
            Color.R = 0xFF;
            Color.G = 20;
            Color.B = 20;
            Color.Alpha = i*10;

            TestResult = BLIT_TestFeaturesFillBlend (Rect,Color,Handle,TargetBitmap,SubscriberID, CRCValuesTab[NbTests_Fill_Blend_Run++],NbTests_Fill_Blend_Run, CRCfstream);
            if (TestResult)
            {
                NbTests_Fill_Blend_Passed++;
            }


        }
    }


#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif
#endif
#endif



#ifndef TEST_FEATURES_NO_COPY2SRC
    /*=======================================================================
            Test7: Copy 2 Src
      =======================================================================*/

    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test7: Copy 2 src\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_Copy_2_src_Run = 0;
    NbTests_Copy_2_src_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_COPY_2_SRC_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Set Context ------------ */
    Context.AluMode                 = STBLIT_ALU_ALPHA_BLEND;
    Context.GlobalAlpha             = 20;


    /* Test full image copy */
    STTBX_Print (("--- Test full image copy\n"));
    TestResult = BLIT_TestFeaturesCopy2Src(0,0,0,0,SourceBitmap.Width,SourceBitmap.Height,0,0,SourceBitmap.Width,SourceBitmap.Height,
                                             Handle,SourceBitmap1,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_2_src_Run++],NbTests_Copy_2_src_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_Copy_2_src_Passed++;

#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(Copy2Src_ResultingBitmap_FileName,&TargetBitmap,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
    }

    /* Test Src1 positions X & Y */
    STTBX_Print (("\n--- Test Src1 positions X & Y\n"));
    for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesCopy2Src(i,j,20,20,100,100,20,20,100,100,Handle,SourceBitmap1,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_2_src_Run++],NbTests_Copy_2_src_Run, CRCfstream);
            if (TestResult)
                    NbTests_Copy_2_src_Passed++;
        }
    }


    /* Test Src2 positions X & Y */
    STTBX_Print (("\n--- Test Src2 positions X & Y\n"));
    for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesCopy2Src (20,20,i,j,100,100,20,20,100,100,Handle,SourceBitmap1,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_2_src_Run++],NbTests_Copy_2_src_Run, CRCfstream);
            if (TestResult)
                    NbTests_Copy_2_src_Passed++;
        }
    }

    /* Test Small Src2 Width & Height */
    STTBX_Print (("\n--- Test Small Src2 Width & Height\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesCopy2Src (20,20,20,20,i,j,20,20,100,100,Handle,SourceBitmap1,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_2_src_Run++],NbTests_Copy_2_src_Run, CRCfstream);
            if (TestResult)
                    NbTests_Copy_2_src_Passed++;

        }
    }

    /* Test Large Src2 Width & Height */
    STTBX_Print (("\n--- Test Large Src2 Width & Height\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesCopy2Src (20,20,20,20,(300 + i),(300 + j),20,20,100,100,Handle,SourceBitmap1,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_2_src_Run++],NbTests_Copy_2_src_Run, CRCfstream);
            if (TestResult)
                    NbTests_Copy_2_src_Passed++;

        }
    }


    /* Test Dst positions X & Y */
    STTBX_Print (("\n--- Test Dst positions X & Y\n"));
    for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesCopy2Src (20,20,20,20,100,100,i,j,100,100,Handle,SourceBitmap1,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_2_src_Run++],NbTests_Copy_2_src_Run, CRCfstream);
            if (TestResult)
                    NbTests_Copy_2_src_Passed++;
        }
    }

#if !defined(ST_7100)
    /* Test Small Dst Width & Height */
    STTBX_Print (("\n--- Test Small Dst Width & Height\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesCopy2Src (20,20,20,20,100,100,20,20,i,j,Handle,SourceBitmap1,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_2_src_Run++],NbTests_Copy_2_src_Run, CRCfstream);
            if (TestResult)
                    NbTests_Copy_2_src_Passed++;

        }
    }
#endif

    /* Test Large Dst Width & Height */
    STTBX_Print (("\n--- Test Large Dst Width & Height\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesCopy2Src (20,20,20,20,100,100,20,20,(300 + i),(300 + j),Handle,SourceBitmap1,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Copy_2_src_Run++],NbTests_Copy_2_src_Run, CRCfstream);
            if (TestResult)
                    NbTests_Copy_2_src_Passed++;

        }
    }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif

#ifndef TEST_FEATURES_NO_CLIP
    /*=======================================================================
            Test8: Test Clip
      =======================================================================*/

    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test8: Clip\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_Clip_Run = 0;
    NbTests_Clip_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_CLIP_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Set Context ------------ */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableClipRectangle     = TRUE;



    /* Test Clip positions X & Y with WriteInsideClipRectangle = TRUE  */
    STTBX_Print (("\n--- Test Clip positions X & Y with WriteInsideClipRectangle = TRUE\n"));
    for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesClip (i,j,100,100,TRUE,Handle,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_Clip_Run++],NbTests_Clip_Run, CRCfstream);
            if (TestResult)
                    NbTests_Clip_Passed++;
        }
    }

    /* Test Clip Small Width & Height with WriteInsideClipRectangle = TRUE  */
    STTBX_Print (("\n--- Test Clip Small Width & Height with WriteInsideClipRectangle = TRUE\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesClip (50,50,i,j,TRUE,Handle,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_Clip_Run++],NbTests_Clip_Run, CRCfstream);
            if (TestResult)
                    NbTests_Clip_Passed++;
        }
    }

    /* Test Clip Large Width & Height with WriteInsideClipRectangle = TRUE  */
    STTBX_Print (("\n--- Test Clip Large Width & Height with WriteInsideClipRectangle = TRUE\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesClip (10,10,(150 + i),(150 + j),TRUE,Handle,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_Clip_Run++],NbTests_Clip_Run, CRCfstream);
            if (TestResult)
            {
                NbTests_Clip_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
                if((i == TEST_FEATURES_NB_STEPS) && (j == TEST_FEATURES_NB_STEPS))
                {
                    Err = ConvertBitmapToGamma(ClipInside_ResultingBitmap_FileName,&TargetBitmap,&Palette);
                    fflush(stdout);
                    if (Err != ST_NO_ERROR)
                    {
                        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                        return (TRUE);
                    }
                }
#endif
            }
        }
    }

    /* Test Clip positions X & Y with WriteInsideClipRectangle = FALSE  */
    STTBX_Print (("\n--- Test Clip positions X & Y with WriteInsideClipRectangle = FALSE\n"));
    for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesClip (i,j,100,100,FALSE,Handle,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_Clip_Run++],NbTests_Clip_Run, CRCfstream);
            if (TestResult)
                    NbTests_Clip_Passed++;
        }
    }

    /* Test Clip Small Width & Height with WriteInsideClipRectangle = FALSE  */
    STTBX_Print (("\n--- Test Clip Small Width & Height with WriteInsideClipRectangle = FALSE\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesClip (50,50,i,j,FALSE,Handle,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_Clip_Run++],NbTests_Clip_Run, CRCfstream);
            if (TestResult)
                    NbTests_Clip_Passed++;
        }
    }

    /* Test Clip Large Width & Height with WriteInsideClipRectangle = FALSE  */
    STTBX_Print (("\n--- Test Clip Large Width & Height with WriteInsideClipRectangle = FALSE\n"));
    for (i=1 ; i <= TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=1 ; j <= TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesClip (10,10,(150 + i),(150 + j),FALSE,Handle,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_Clip_Run++],NbTests_Clip_Run, CRCfstream);
            if (TestResult)
            {
                NbTests_Clip_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
                if((i == TEST_FEATURES_NB_STEPS) && (j == TEST_FEATURES_NB_STEPS))
                {
                    Err = ConvertBitmapToGamma(ClipOutside_ResultingBitmap_FileName,&TargetBitmap,&Palette);
                    fflush(stdout);
                    if (Err != ST_NO_ERROR)
                    {
                        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                        return (TRUE);
                    }
                }
#endif
            }
        }
    }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif

#ifndef TEST_FEATURES_NO_RESIZE
    /*=======================================================================
            Test9: Test Resize
      =======================================================================*/

    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test9: Resize\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_Resize_Run = 0;
    NbTests_Resize_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_RESIZE_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Set Context ------------ */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableClipRectangle     = FALSE;



    /* Test horizontal expension */
    STTBX_Print (("\n--- Test horizontal expansion\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS * 2); i++)
    {
        TestResult = BLIT_TestFeaturesResize (50,50,200,200,50,50,200+i,200, Handle, SourceBitmap, TargetBitmap,Context,CRCValuesTab[NbTests_Resize_Run++],NbTests_Resize_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Resize_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if (i == ((TEST_FEATURES_NB_STEPS * 2) - 1))
            {
                Err = ConvertBitmapToGamma(ResizeHExpansion_ResultingBitmap_FileName,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
        }
    }

    /* Test horizontal compression */
    STTBX_Print (("\n--- Test horizontal compression\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS * 2); i++)
    {
        TestResult = BLIT_TestFeaturesResize (50,50,200,200,50,50,200-i,200,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Resize_Run++],NbTests_Resize_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Resize_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if (i == ((TEST_FEATURES_NB_STEPS * 2) - 1))
            {
                Err = ConvertBitmapToGamma(ResizeHCompression_ResultingBitmap_FileName,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif

        }
    }

    /* Test vertical expension */
    STTBX_Print (("\n--- Test vertical expansion\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS * 2); i++)
    {
        TestResult = BLIT_TestFeaturesResize (50,50,200,200,50,50,200,200+i,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Resize_Run++],NbTests_Resize_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Resize_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if (i == ((TEST_FEATURES_NB_STEPS * 2) - 1))
            {
                Err = ConvertBitmapToGamma(ResizeVExpansion_ResultingBitmap_FileName,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
        }
    }

    /* Test vertical compression */
    STTBX_Print (("\n--- Test vertical compression\n"));
    for (i=0 ; i < (TEST_FEATURES_NB_STEPS * 2); i++)
    {
        TestResult = BLIT_TestFeaturesResize (50,50,200,200,50,50,200,200-i,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_Resize_Run++],NbTests_Resize_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Resize_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if (i == ((TEST_FEATURES_NB_STEPS * 2) - 1))
            {
                Err = ConvertBitmapToGamma(ResizeVCompression_ResultingBitmap_FileName,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
        }
    }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif

#if defined (ST_7109) || defined (ST_7200)
#ifndef TEST_FEATURES_NO_ALU
    /*=======================================================================
            Test11: Test Alu
      =======================================================================*/

    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test11: ALU\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_ALU_Run = 0;
    NbTests_ALU_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_ALU_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Set Context ------------ */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableClipRectangle     = FALSE;



    /* ------------ Set Src 1 ------------ */
    STTBX_Print (("-> Free source1\n"));
    GUTIL_Free(SourceBitmap.Data1_p);

    STTBX_Print (("-> Load source 1\n"));
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_LOGICAL_ZERO_FILE_NAME,SrcPartitionInfo,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Src 2 ------------ */
    STTBX_Print (("-> Free source2\n"));
    GUTIL_Free(SourceBitmap1.Data1_p);

    STTBX_Print (("-> Load source 1\n"));
    SourceBitmap1.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_LOGICAL_ONE_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Test AND ------------ */
    Context.AluMode                 = STBLIT_ALU_AND;

    STTBX_Print (("\n--- Test ALU : S1(0) AND S2(1)\n"));
    TestResult = BLIT_TestFeaturesALU (Handle,SourceBitmap,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_ALU_Run++],NbTests_ALU_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_ALU_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(AluAnd_ResultingBitmap_FileName,&TargetBitmap,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
    }
    /* ------------ Test AND_REV ------------ */
    Context.AluMode                 = STBLIT_ALU_AND_REV;

    STTBX_Print (("\n--- Test ALU : S1(0) AND_REV S2(1)\n"));
    TestResult = BLIT_TestFeaturesALU (Handle,SourceBitmap,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_ALU_Run++],NbTests_ALU_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_ALU_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(AluAndRev_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif

    }
    /* ------------ Test AND_INVERT ------------ */
    Context.AluMode                 = STBLIT_ALU_AND_INVERT;

    STTBX_Print (("\n--- Test ALU : S1(0) AND_INVERT S2(1)\n"));
    TestResult = BLIT_TestFeaturesALU (Handle,SourceBitmap,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_ALU_Run++],NbTests_ALU_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_ALU_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(AluAndInvert_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif
    }
    /* ------------ Test STBLIT_ALU_NOOP ------------ */
    Context.AluMode                 = STBLIT_ALU_NOOP;

    STTBX_Print (("\n--- Test ALU : S1(0) NOOP S2(1)\n"));
    TestResult = BLIT_TestFeaturesALU (Handle,SourceBitmap,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_ALU_Run++],NbTests_ALU_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_ALU_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(AluAndNoop_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif

    }
    /* ------------ Test STBLIT_ALU_XOR ------------ */
    Context.AluMode                 = STBLIT_ALU_XOR;

    STTBX_Print (("\n--- Test ALU : S1(0) XOR S2(1)\n"));
    TestResult = BLIT_TestFeaturesALU (Handle,SourceBitmap,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_ALU_Run++],NbTests_ALU_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_ALU_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(AluXor_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif
    }
    /* ------------ Test OR ------------ */
    Context.AluMode                 = STBLIT_ALU_OR;

    STTBX_Print (("\n--- Test ALU : S1(0) OR S2(1)\n"));
    TestResult = BLIT_TestFeaturesALU (Handle,SourceBitmap,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_ALU_Run++],NbTests_ALU_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_ALU_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(AluOr_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif
    }
    /* ------------ Test NOR ------------ */
    Context.AluMode                 = STBLIT_ALU_NOR;

    STTBX_Print (("\n--- Test ALU : S1(0) NOR S2(1)\n"));
    TestResult = BLIT_TestFeaturesALU (Handle,SourceBitmap,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_ALU_Run++],NbTests_ALU_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_ALU_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(AluNor_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif
    }
    /* ------------ Test OR_REVERSE ------------ */
    Context.AluMode                 = STBLIT_ALU_OR_REVERSE;

    STTBX_Print (("\n--- Test ALU : S1(0) OR_REVERSE S2(1)\n"));
    TestResult = BLIT_TestFeaturesALU (Handle,SourceBitmap,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_ALU_Run++],NbTests_ALU_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_ALU_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(AluOrReverse_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif

    }
    /* ------------ Test OR_INVERT ------------ */
    Context.AluMode                 = STBLIT_ALU_OR_INVERT;

    STTBX_Print (("\n--- Test ALU : S1(0) OR_INVERT S2(1)\n"));
    TestResult = BLIT_TestFeaturesALU (Handle,SourceBitmap,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_ALU_Run++],NbTests_ALU_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_ALU_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(AluOrInvert_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif
    }
    /* ------------ Test STBLIT_ALU_NAND ------------ */
    Context.AluMode                 = STBLIT_ALU_NAND;

    STTBX_Print (("\n--- Test ALU : S1(0) NAND S2(1)\n"));
    TestResult = BLIT_TestFeaturesALU (Handle,SourceBitmap,SourceBitmap1,TargetBitmap,Context,CRCValuesTab[NbTests_ALU_Run++],NbTests_ALU_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_ALU_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(AluNand_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif
    }
#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif
#endif /* 7109 */

#if defined (ST_7109) || defined (ST_7200) || defined (ST_7100)
#ifndef TEST_FEATURES_NO_MASK_WORD
    /*=======================================================================
            Test12: Test Mask word
      =======================================================================*/

    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test12: Mask word\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_Mask_Word_Run = 0;
    NbTests_Mask_Word_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_MASK_WORD_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Set Src ------------ */
    STTBX_Print (("-> Free source1\n"));
    GUTIL_Free(SourceBitmap1.Data1_p);

    STTBX_Print (("-> Load source 1\n"));
    SourceBitmap1.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE2_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    STTBX_Print (("-> Free source2\n"));
    GUTIL_Free(SourceBitmap.Data1_p);

    STTBX_Print (("-> Load source 2\n"));
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE1_FILE_NAME,SrcPartitionInfo,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Dst ------------ */
#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
    STTBX_Print (("-> Free target\n"));
    GUTIL_Free(TargetBitmap.Data1_p);

    /* ------------ Set Dst ------------ */
    STTBX_Print (("-> Load destination\n"));
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_TARGET1_FILE_NAME, DstPartitionInfo,&TargetBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#endif /* STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST */


    /* ------------ Set Context ------------ */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableClipRectangle     = FALSE;
    Context.EnableMaskWord          = TRUE;


    /* ------------ Test Red component ------------ */
    STTBX_Print (("\n--- Test Red component\n"));
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        MaskWordValue = 0xFF0000;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        MaskWordValue = 0xF800;
    }

    TestResult = BLIT_TestFeaturesMaskWord (Handle,SourceBitmap,TargetBitmap,MaskWordValue,Context,CRCValuesTab[NbTests_Mask_Word_Run++],NbTests_Mask_Word_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_Mask_Word_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(MaskWordRed_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif

    }


    /* ------------ Set Dst ------------ */
#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
    STTBX_Print (("-> Free target\n"));
    GUTIL_Free(TargetBitmap.Data1_p);

    /* ------------ Set Dst ------------ */
    STTBX_Print (("-> Load destination\n"));
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_TARGET1_FILE_NAME, DstPartitionInfo,&TargetBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#endif /* STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST */


    /* ------------ Test Green component ------------ */
    STTBX_Print (("\n--- Test Green component\n"));
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        MaskWordValue = 0xFF00;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        MaskWordValue = 0x7E0;
    }

    TestResult = BLIT_TestFeaturesMaskWord (Handle,SourceBitmap,TargetBitmap,MaskWordValue,Context,CRCValuesTab[NbTests_Mask_Word_Run++],NbTests_Mask_Word_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_Mask_Word_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(MaskWordGreen_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif

     }

    /* ------------ Set Dst ------------ */
#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
    STTBX_Print (("-> Free target\n"));
    GUTIL_Free(TargetBitmap.Data1_p);

    /* ------------ Set Dst ------------ */
    STTBX_Print (("-> Load destination\n"));
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_TARGET1_FILE_NAME, DstPartitionInfo,&TargetBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#endif /* STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST */

    /* ------------ Test Blue component ------------ */
    STTBX_Print (("\n--- Test Blue component\n"));
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        MaskWordValue = 0xFF;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        MaskWordValue = 0x001F;
    }

    TestResult = BLIT_TestFeaturesMaskWord (Handle,SourceBitmap,TargetBitmap,MaskWordValue,Context,CRCValuesTab[NbTests_Mask_Word_Run++],NbTests_Mask_Word_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_Mask_Word_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(MaskWordBlue_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif

    }


    /* ------------ Set Dst ------------ */
#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
    STTBX_Print (("-> Free target\n"));
    GUTIL_Free(TargetBitmap.Data1_p);

    /* ------------ Set Dst ------------ */
    STTBX_Print (("-> Load destination\n"));
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_TARGET1_FILE_NAME, DstPartitionInfo,&TargetBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#endif /* STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST */

    /* ------------ Test Remove Red component ------------ */
    STTBX_Print (("\n--- Test Remove Red component\n"));
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        MaskWordValue = 0x00FFFF;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        MaskWordValue = 0x7FF;
    }

    TestResult = BLIT_TestFeaturesMaskWord (Handle,SourceBitmap,TargetBitmap,MaskWordValue,Context,CRCValuesTab[NbTests_Mask_Word_Run++],NbTests_Mask_Word_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_Mask_Word_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(MaskWordRemoveRed_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif

    }

    /* ------------ Set Dst ------------ */
#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
    STTBX_Print (("-> Free target\n"));
    GUTIL_Free(TargetBitmap.Data1_p);

    /* ------------ Set Dst ------------ */
    STTBX_Print (("-> Load destination\n"));
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_TARGET1_FILE_NAME, DstPartitionInfo,&TargetBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#endif /* STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST */

    /* ------------ Test Remove Green component ------------ */
    STTBX_Print (("\n--- Test Remove Green component\n"));
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        MaskWordValue = 0xFF00FF;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        MaskWordValue = 0xF81F;
    }

    TestResult = BLIT_TestFeaturesMaskWord (Handle,SourceBitmap,TargetBitmap,MaskWordValue,Context,CRCValuesTab[NbTests_Mask_Word_Run++],NbTests_Mask_Word_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_Mask_Word_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(MaskWordRemoveGreen_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif

    }

    /* ------------ Set Dst ------------ */
#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
    STTBX_Print (("-> Free target\n"));
    GUTIL_Free(TargetBitmap.Data1_p);

    /* ------------ Set Dst ------------ */
    STTBX_Print (("-> Load destination\n"));
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_TARGET1_FILE_NAME, DstPartitionInfo,&TargetBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#endif /* STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST */

    /* ------------ Test Remove Blue component ------------ */
    STTBX_Print (("\n--- Test Remove Remove Red component\n"));
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        MaskWordValue = 0xFFFF00;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        MaskWordValue = 0xFFE0;
    }

    TestResult = BLIT_TestFeaturesMaskWord (Handle,SourceBitmap,TargetBitmap,MaskWordValue,Context,CRCValuesTab[NbTests_Mask_Word_Run++],NbTests_Mask_Word_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_Mask_Word_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(MaskWordRemoveBlue_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif
    }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif

#ifndef TEST_FEATURES_NO_COLOR_KEY
    /*=======================================================================
            Test13: Test Color key
      =======================================================================*/

    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test13: Color Key\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_Color_Key_Run = 0;
    NbTests_Color_Key_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_COLOR_KEY_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Set Src ------------ */
    STTBX_Print (("-> Free source1\n"));
    GUTIL_Free(SourceBitmap1.Data1_p);

    STTBX_Print (("-> Load source 1\n"));
    SourceBitmap1.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_COLOR_KEY_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    STTBX_Print (("-> Free source2\n"));
    GUTIL_Free(SourceBitmap.Data1_p);

    STTBX_Print (("-> Load source 2\n"));
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE1_FILE_NAME,SrcPartitionInfo,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }



    /* ------------ Set Context ------------ */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableClipRectangle     = FALSE;
    Context.EnableMaskWord          = FALSE;


    /* ------------ Set Context ------------ */
    Context.ColorKey.Type                 = STGXOBJ_COLOR_KEY_TYPE_RGB888;
    Context.ColorKey.Value.RGB888.RMin    = 0x0;
    Context.ColorKey.Value.RGB888.RMax    = 0xFF;
    Context.ColorKey.Value.RGB888.ROut    = 0;
    Context.ColorKey.Value.RGB888.REnable = 0;
    Context.ColorKey.Value.RGB888.GOut    = 1;
    Context.ColorKey.Value.RGB888.GEnable = 1;
    Context.ColorKey.Value.RGB888.BMin    = 0x0;
    Context.ColorKey.Value.RGB888.BMax    = 0xFF;
    Context.ColorKey.Value.RGB888.BOut    = 0;
    Context.ColorKey.Value.RGB888.BEnable = 0;



    /* ------------ Test Color Key ------------ */
    STTBX_Print (("\n--- Test Source mode\n"));
    for (i=0 ; i < 10 ; i++)
    {
        TestResult = BLIT_TestFeaturesColorKey (Handle,SourceBitmap,SourceBitmap1,TargetBitmap,(230 - 20*i),(240 - 20*i),
                                                  STBLIT_COLOR_KEY_MODE_SRC,Context,CRCValuesTab[NbTests_Color_Key_Run++],NbTests_Color_Key_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Key_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if( i == 0)
            {
                Err = ConvertBitmapToGamma(ColorKeySourceMode_ResultingBitmap_FileName,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
        }
    }

    STTBX_Print (("\n--- Test Destination mode\n"));
    for (i=0 ; i < 10 ; i++)
    {
        TestResult = BLIT_TestFeaturesColorKey (Handle,SourceBitmap,SourceBitmap1,TargetBitmap,(230 - 20*i),(240 - 20*i),
                                                  STBLIT_COLOR_KEY_MODE_DST,Context,CRCValuesTab[NbTests_Color_Key_Run++],NbTests_Color_Key_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_Color_Key_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if( i == 0)
            {
                Err = ConvertBitmapToGamma(ColorKeyDestinationMode_ResultingBitmap_FileName,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
        }
    }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif
#endif /*defined (ST_7109) || defined (ST_7200) || defined (ST_7100)*/



#if defined(ST_7109) || defined(ST_7200) || defined(ST_5100) || defined(ST_5105)
#ifndef TEST_FEATURES_NO_ANTIALIASING
    /*=======================================================================
            Test15: Test AntiAliasing
      =======================================================================*/

    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test15: AntiAliasing\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_AntiAliasing_Run    = 0;
    NbTests_AntiAliasing_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_ANTIALIASING_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Set Context ------------ */
    Context.AluMode                 = STBLIT_ALU_ALPHA_BLEND;
    Context.GlobalAlpha             = 100;
    Context.EnableClipRectangle     = FALSE;

    /* ------------ Set Dst ------------ */
#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
    STTBX_Print (("-> Free target\n"));
    GUTIL_Free(TargetBitmap.Data1_p);

    /* ------------ Set Dst ------------ */
    STTBX_Print (("-> Load destination\n"));
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_TARGET1_FILE_NAME, DstPartitionInfo,&TargetBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif /* STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST */


    /* Test AntiAliasing */
    STTBX_Print (("\n--- Test AntiAliasing\n"));

    TestResult = BLIT_TestFeaturesAntiAliasing (50,50,200,200,50,50,200,200,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_AntiAliasing_Run++],NbTests_AntiAliasing_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_AntiAliasing_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(AntiAliasing_ResultingBitmap_FileName,&TargetBitmap,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
        }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif

#ifndef TEST_FEATURES_NO_ANTIFLUTTER
    /*=======================================================================
            Test16: Test AntiFlutter
      =======================================================================*/

    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test16: AntiFlutter\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_AntiFlutter_Run    = 0;
    NbTests_AntiFlutter_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_ANTIFLUTTER_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Set Context ------------ */
    Context.AluMode                 = STBLIT_ALU_ALPHA_BLEND;
    Context.GlobalAlpha             = 100;
    Context.EnableClipRectangle     = FALSE;


#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
    STTBX_Print (("-> Free target\n"));
    GUTIL_Free(TargetBitmap.Data1_p);

    /* ------------ Set Dst ------------ */
    STTBX_Print (("-> Load destination\n"));
    TargetBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_TARGET1_FILE_NAME, DstPartitionInfo,&TargetBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif /* STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST */

    /* Test AntiFlutter */
    STTBX_Print (("\n--- Test AntiFlutter\n"));
        TestResult = BLIT_TestFeaturesAntiFlutter (50,50,200,200,50,50,200,200,Handle,SourceBitmap,TargetBitmap,Context,CRCValuesTab[NbTests_AntiFlutter_Run++],NbTests_AntiFlutter_Run, CRCfstream);
        if (TestResult)
        {
            NbTests_AntiFlutter_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            Err = ConvertBitmapToGamma(AntiFlutter_ResultingBitmap_FileName,&TargetBitmap,&Palette);
            fflush(stdout);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                return (TRUE);
            }
#endif
        }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif
#endif /* ST_7109 ST_7200 ST_5100 ST_5105*/

#ifndef TEST_FEATURES_NO_SETPIXEL
    /*=======================================================================
            Test17: Test Set Pixel
      =======================================================================*/
    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test17: Set Pixel\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_SetPixel_Run = 0;
    NbTests_SetPixel_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_SETPIXEL_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Context ------------ */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.GlobalAlpha             = 100;
    Context.EnableClipRectangle     = FALSE;

    /* Test Set Pixel */
    STTBX_Print (("\n--- Test Set Pixel\n"));
    TestResult = BLIT_TestFeaturesSetPixel (10,10,0xff,0,0,Handle,TargetBitmap,Context,CRCValuesTab[NbTests_SetPixel_Run++],NbTests_SetPixel_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_SetPixel_Passed++;

#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(SetPixel_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif
    }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif

#ifndef TEST_FEATURES_NO_DRAWLINE
    /*=======================================================================
            Test18: Test Draw Line
      =======================================================================*/
    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test18: Draw Line\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_DrawLine_Run = 0;
    NbTests_DrawLine_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_DRAWLINE_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Context ------------ */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.GlobalAlpha             = 100;
    Context.EnableClipRectangle     = FALSE;

    /* Test Horizontal Line */
    STTBX_Print (("\n--- Test Draw Horizontal Line\n"));
    TestResult = BLIT_TestFeaturesDrawHLine(20,20,100,0xff,0,0,Handle,TargetBitmap,Context,CRCValuesTab[NbTests_DrawLine_Run++],NbTests_DrawLine_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_DrawLine_Passed++;

#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(DrawHLine_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif
    }

    /* Test Vertical Line */
    STTBX_Print (("\n--- Test Draw Vertical Line\n"));
    TestResult = BLIT_TestFeaturesDrawVLine(10,10,100,0xff,0,0,Handle,TargetBitmap,Context,CRCValuesTab[NbTests_DrawLine_Run++],NbTests_DrawLine_Run, CRCfstream);
    if (TestResult)
    {
        NbTests_DrawLine_Passed ++;

#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
        Err = ConvertBitmapToGamma(DrawVLine_ResultingBitmap_FileName,&TargetBitmap,&Palette);
        fflush(stdout);
        if (Err != ST_NO_ERROR)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
            return (TRUE);
        }
#endif
    }


#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif

#ifndef TEST_FEATURES_NO_JOB
    /*=======================================================================
            Test14: Test job
      =======================================================================*/

    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test14: Job\n"));
    STTBX_Print (("===================================\n"));

    /* ------------ Init Test Counter ------------ */
    NbTests_Job_Run = 0;
    NbTests_Job_Passed = 0;

    /* ------------ Init Reference File ------------ */
    Err = BLIT_InitReferenceFile (TEST_FEATURES_JOB_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }



    /* ---- Set Job --------*/
    Err = STBLIT_CreateJob(Handle,&Job_Params,&Job1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Create job 1: %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Src ------------ */
#if !defined(ST_OSLINUX)
    STTBX_Print (("-> Free source1\n"));
    GUTIL_Free(SourceBitmap1.Data1_p);

    STTBX_Print (("-> Load source 1\n"));
    SourceBitmap1.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE2_FILE_NAME,SrcPartitionInfo,&SourceBitmap1,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif

    /* ------------ Init job test ------------ */
    STTBX_Print (("-> Init job test\n"));
    Err = BLIT_TestFeaturesJobTestInit (Handle, Job1, &JBHandle_1, &JBHandle_2, &JBHandle_3,
                                          SourceBitmap, Palette, TargetBitmap);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Job Test : %d\n",Err));
        return (TRUE);
    }

#if !defined(ST_7100)
    /* ------------ Test Dst positions X & Y  ------------ */
    STTBX_Print (("\n--- Test Dst positions X & Y\n"));
    for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesJobTestDestination(Handle, Job1, JBHandle_1, JBHandle_2, JBHandle_3,i,j,0,0,
                                                               SourceBitmap1,Palette,TargetBitmap,CRCValuesTab[NbTests_Job_Run++],NbTests_Job_Run, CRCfstream);
            if (TestResult)
            {
                NbTests_Job_Passed ++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if( (i == (TEST_FEATURES_NB_STEPS-1)) && (j == (TEST_FEATURES_NB_STEPS-1)) )
            {
                Err = ConvertBitmapToGamma(Job_ResultingBitmap_FileName1,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif
            }
        }
    }
#endif

#if 0  /* TBD */
    /* ------------ Test Dst width & height  ------------ */
    STTBX_Print (("\n--- Test Dst width & height\n"));
    for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
        {
            TestResult = BLIT_TestFeaturesJobTestDestination(Handle, Job1, JBHandle_1, JBHandle_2, JBHandle_3,0,0,i,j,
                                                               SourceBitmap1,Palette,TargetBitmap,CRCValuesTab[NbTests_Job_Run++],NbTests_Job_Run, CRCfstream);
            if (TestResult)
                    NbTests_Job_Passed++;
        }
    }
#endif


    /* ------------ Test color fill  ------------ */
    STTBX_Print (("\n--- Test color fill\n"));
    for (i=0 ; i < TEST_FEATURES_NB_STEPS ; i++)
    {
        for (j=0 ; j < TEST_FEATURES_NB_STEPS ; j++)
        {
            STGXOBJ_Bitmap_t TempBitmap;
            U32              R_Value, G_Value, B_Value, Temp;
            BOOL             WriteIntoClipRectangle;

            if ((j%2) == 0)
            {
                TempBitmap = SourceBitmap1;
                WriteIntoClipRectangle = TRUE;
            }
            else
            {
                TempBitmap = SourceBitmap;
                WriteIntoClipRectangle = FALSE;
            }

            Temp = (j%3);
            switch (Temp)
            {
                case 0:
                    R_Value = 0x20 + i + j;
                    G_Value = 0x70;
                    B_Value = 0x70;
                    break;

                case 1:
                    R_Value = 0x70;
                    G_Value = 0x20 + i + j;
                    B_Value = 0x70;
                    break;

                default:
                    R_Value = 0x70;
                    G_Value = 0x70;
                    B_Value = 0x20 + i + j;
                    break;
            }

            TestResult = BLIT_TestFeaturesJobTestContent(Handle, Job1, JBHandle_1, JBHandle_2, JBHandle_3,
                                                           i,j,WriteIntoClipRectangle,R_Value, G_Value, B_Value,
                                                           TempBitmap,Palette,TargetBitmap,CRCValuesTab[NbTests_Job_Run++],NbTests_Job_Run, CRCfstream);
            if (TestResult)
            {
                NbTests_Job_Passed++;
#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
            if( (i == (TEST_FEATURES_NB_STEPS-1)) && (j == (TEST_FEATURES_NB_STEPS-1)) )
            {
                Err = ConvertBitmapToGamma(Job_ResultingBitmap_FileName2,&TargetBitmap,&Palette);
                fflush(stdout);
                if (Err != ST_NO_ERROR)
                {
                    STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
                    return (TRUE);
                }
            }
#endif

            }
        }
    }

    Err = STBLIT_DeleteJob(Handle, Job1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print (("Err DeleteJob: %d\n",Err));
        return (TRUE);
    }

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

#endif

    /* report test results */
    STTBX_Print (("\n\n#########################################################\n"));
#ifndef TEST_FEATURES_NO_COPY
    STTBX_Print (("Copy : %d/%d\n",NbTests_Copy_Passed,NbTests_Copy_Run));
#endif
#ifndef TEST_FEATURES_NO_FILL
    STTBX_Print (("Fill : %d/%d\n",NbTests_Fill_Passed,NbTests_Fill_Run));
#endif
#if !defined (TEST_FEATURES_NO_OVERLAP_COPY)
    STTBX_Print (("Overlap Copy : %d/%d\n",NbTests_Overlap_Copy_Passed,NbTests_Overlap_Copy_Run));
#endif

#if !defined (TEST_FEATURES_NO_OVERLAP_BLIT)
    STTBX_Print (("Overlap Blit : %d/%d\n",NbTests_Overlap_Blit_Passed,NbTests_Overlap_Blit_Run));
#endif

#ifndef TEST_FEATURES_NO_BLEND_BITMAP
    STTBX_Print (("Bitmap Blend : %d/%d\n",NbTests_Bitmap_Blend_Passed,NbTests_Bitmap_Blend_Run));
#endif
#if !defined(TEST_FEATURES_NO_BLEND_COLOR)
    STTBX_Print (("Fill Blend : %d/%d\n",NbTests_Fill_Blend_Passed,NbTests_Fill_Blend_Run));
#endif
#ifndef TEST_FEATURES_NO_COPY2SRC
    STTBX_Print (("Copy 2 src : %d/%d\n",NbTests_Copy_2_src_Passed,NbTests_Copy_2_src_Run));
#endif
#ifndef TEST_FEATURES_NO_CLIP
    STTBX_Print (("Clip : %d/%d\n",NbTests_Clip_Passed,NbTests_Clip_Run));
#endif
#ifndef TEST_FEATURES_NO_RESIZE
    STTBX_Print (("Resize : %d/%d\n",NbTests_Resize_Passed,NbTests_Resize_Run));
#endif
#ifndef TEST_FEATURES_NO_COLOR_CONVERSION
    STTBX_Print (("Color Conversion : %d/%d\n",NbTests_Color_Conversion_Passed,NbTests_Color_Conversion_Run));
#endif
#if defined (ST_7109) || defined (ST_7200)
#ifndef TEST_FEATURES_NO_ALU
    STTBX_Print (("ALU : %d/%d\n",NbTests_ALU_Passed,NbTests_ALU_Run));
#endif
#ifndef TEST_FEATURES_NO_MASK_WORD
    STTBX_Print (("Mask Word : %d/%d\n",NbTests_Mask_Word_Passed,NbTests_Mask_Word_Run));
#endif
#ifndef TEST_FEATURES_NO_COLOR_KEY
    STTBX_Print (("Color Key : %d/%d\n",NbTests_Color_Key_Passed,NbTests_Color_Key_Run));
#endif
#endif /* 7109 */

#ifndef TEST_FEATURES_NO_JOB
    STTBX_Print (("Job : %d/%d\n",NbTests_Job_Passed,NbTests_Job_Run));
#endif

#if defined (ST_7109) || defined (ST_7200) || defined (ST_5100) || defined (ST_5105)
#ifndef TEST_FEATURES_NO_ANTIALIASING
    STTBX_Print (("AntiAliasing : %d/%d\n",NbTests_AntiAliasing_Passed,NbTests_AntiAliasing_Run));
#endif

#ifndef TEST_FEATURES_NO_ANTIFLUTTER
    STTBX_Print (("AntiFlutter : %d/%d\n",NbTests_AntiFlutter_Passed,NbTests_AntiFlutter_Run));
#endif
#endif

#ifndef TEST_FEATURES_NO_SETPIXEL
    STTBX_Print (("SetPixel : %d/%d\n",NbTests_SetPixel_Passed,NbTests_SetPixel_Run));
#endif

#ifndef TEST_FEATURES_NO_DRAWLINE
    STTBX_Print (("DrawLine : %d/%d\n",NbTests_DrawLine_Passed,NbTests_DrawLine_Run));
#endif

#if defined (ST_7109) || defined (ST_7200)
#ifndef TEST_FEATURES_NO_VC1_RANGE
    STTBX_Print (("VC1Range : %d/%d\n",NbTests_VC1_Range_Passed ,NbTests_VC1_Range_Run));
#endif
#endif

#if defined (ST_7109) || defined (ST_7200)
    STTBX_Print (("\nTOTAL : %d/%d\n",(NbTests_Copy_Passed + NbTests_Fill_Passed + NbTests_Overlap_Copy_Passed + NbTests_Bitmap_Blend_Passed + NbTests_Fill_Blend_Passed+NbTests_Copy_2_src_Passed+
                                       NbTests_Clip_Passed + NbTests_Resize_Passed + NbTests_Color_Conversion_Passed + NbTests_Job_Passed + NbTests_ALU_Passed + NbTests_Mask_Word_Passed+
                                       NbTests_Color_Key_Passed + NbTests_AntiAliasing_Passed + NbTests_AntiFlutter_Passed + NbTests_SetPixel_Passed + NbTests_DrawLine_Passed+ NbTests_VC1_Range_Passed ),
                                      (NbTests_Copy_Run+NbTests_Fill_Run+NbTests_Overlap_Copy_Run+NbTests_Bitmap_Blend_Run+NbTests_Fill_Blend_Run+NbTests_Copy_2_src_Run+
                                       NbTests_Clip_Run+NbTests_Resize_Run+NbTests_Color_Conversion_Run+NbTests_Job_Run+NbTests_ALU_Run+NbTests_Mask_Word_Run+NbTests_Color_Key_Run+
                                       NbTests_AntiAliasing_Run + NbTests_AntiFlutter_Run + NbTests_SetPixel_Run + NbTests_DrawLine_Run + NbTests_VC1_Range_Run)));
#elif defined (ST_5100) || defined (ST_5105)
    STTBX_Print (("\nTOTAL : %d/%d\n",(NbTests_Copy_Passed+NbTests_Fill_Passed+NbTests_Overlap_Copy_Passed+NbTests_Bitmap_Blend_Passed+NbTests_Fill_Blend_Passed+NbTests_Copy_2_src_Passed+
                                       NbTests_Clip_Passed+NbTests_Resize_Passed+NbTests_Color_Conversion_Passed+NbTests_Job_Passed+NbTests_AntiAliasing_Passed
                                       + NbTests_AntiFlutter_Passed + NbTests_SetPixel_Passed + NbTests_DrawLine_Passed ),
                                      (NbTests_Copy_Run+NbTests_Fill_Run+NbTests_Overlap_Copy_Run+NbTests_Bitmap_Blend_Run+NbTests_Fill_Blend_Run+NbTests_Copy_2_src_Run+
                                       NbTests_Clip_Run+NbTests_Resize_Run+NbTests_Color_Conversion_Run + NbTests_Job_Run + NbTests_AntiAliasing_Run + NbTests_AntiFlutter_Run +
                                       NbTests_SetPixel_Run+ NbTests_DrawLine_Run) ));

#else
    STTBX_Print (("\nTOTAL : %d/%d\n",(NbTests_Copy_Passed+NbTests_Fill_Passed+NbTests_Overlap_Copy_Passed+NbTests_Bitmap_Blend_Passed+NbTests_Fill_Blend_Passed+NbTests_Copy_2_src_Passed+
                                       NbTests_Clip_Passed+NbTests_Resize_Passed+NbTests_Color_Conversion_Passed+NbTests_Job_Passed+ NbTests_SetPixel_Passed + NbTests_DrawLine_Passed),
                                      (NbTests_Copy_Run+NbTests_Fill_Run+NbTests_Overlap_Copy_Run+NbTests_Bitmap_Blend_Run+NbTests_Fill_Blend_Run+NbTests_Copy_2_src_Run+
                                       NbTests_Clip_Run+NbTests_Resize_Run+NbTests_Color_Conversion_Run+NbTests_Job_Run + NbTests_SetPixel_Run + NbTests_DrawLine_Run) ));
#endif
STTBX_Print (("#########################################################\n"));



    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);
    GUTIL_Free(SourceBitmap1.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
#ifdef PTV
    Err = STBLIT_Close(Handle);
#else
    Err = STBLIT_Term(Name,&TermParams);
#endif /* PTV */
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

#if defined(STBLIT_DEBUG_GET_STATISTICS)
/*-----------------------------------------------------------------------------
 * Function : BLIT_ReportStatistics
 * Input    :
 * Output   :
 * Return   :
 * --------------------------------------------------------------------------*/
static void BLIT_ReportStatistics(STBLIT_Statistics_t sblitStatistics)
{
    /* report test results */
    STTBX_Print (("------------------------------------------------------\n"));
    STTBX_Print (("Submissions                 = %d\n", sblitStatistics.Submissions ));
    STTBX_Print (("BlitCompletionInterruptions = %d\n", sblitStatistics.BlitCompletionInterruptions ));
    STTBX_Print (("CorrectTimeStatisticsValues = %d\n\n", sblitStatistics.CorrectTimeStatisticsValues ));

#ifndef STBLIT_DEBUG_STATISTICS_USE_FIRST_AND_LAST_TIME_VALUE_ONLY
    STTBX_Print (("MinGenerationTime           = %d.%d ms\n", (sblitStatistics.MinGenerationTime/1000), (sblitStatistics.MinGenerationTime%1000) ));
#endif /* STBLIT_DEBUG_STATISTICS_USE_FIRST_AND_LAST_TIME_VALUE_ONLY */

    STTBX_Print (("AverageGenerationTime       = %d.%d ms\n", (sblitStatistics.AverageGenerationTime/1000), (sblitStatistics.AverageGenerationTime%1000) ));

#ifndef STBLIT_DEBUG_STATISTICS_USE_FIRST_AND_LAST_TIME_VALUE_ONLY
    STTBX_Print (("MaxGenerationTime           = %d.%d ms\n", (sblitStatistics.MaxGenerationTime/1000), (sblitStatistics.MaxGenerationTime%1000) ));
    STTBX_Print (("LatestBlitGenerationTime    = %d.%d ms\n\n", (sblitStatistics.LatestBlitGenerationTime/1000), (sblitStatistics.LatestBlitGenerationTime%1000) ));
    STTBX_Print (("MinExecutionTime            = %d.%d ms\n", (sblitStatistics.MinExecutionTime/1000), (sblitStatistics.MinExecutionTime%1000) ));
    STTBX_Print (("AverageExecutionTime        = %d.%d ms\n", (sblitStatistics.AverageExecutionTime/1000), (sblitStatistics.AverageExecutionTime%1000) ));
    STTBX_Print (("MaxExecutionTime            = %d.%d ms\n", (sblitStatistics.MaxExecutionTime/1000), (sblitStatistics.MaxExecutionTime%1000) ));
    STTBX_Print (("LatestBlitExecutionTime     = %d.%d ms\n\n", (sblitStatistics.LatestBlitExecutionTime/1000), (sblitStatistics.LatestBlitExecutionTime%1000) ));
    STTBX_Print (("MinProcessingTime           = %d.%d ms\n", (sblitStatistics.MinProcessingTime/1000), (sblitStatistics.MinProcessingTime%1000) ));
#endif /* STBLIT_DEBUG_STATISTICS_USE_FIRST_AND_LAST_TIME_VALUE_ONLY */

    STTBX_Print (("AverageProcessingTime       = %d.%d ms\n", (sblitStatistics.AverageProcessingTime/1000), (sblitStatistics.AverageProcessingTime%1000) ));

#ifndef STBLIT_DEBUG_STATISTICS_USE_FIRST_AND_LAST_TIME_VALUE_ONLY
    STTBX_Print (("MaxProcessingTime           = %d.%d ms\n", (sblitStatistics.MaxProcessingTime/1000), (sblitStatistics.MaxProcessingTime%1000) ));
    STTBX_Print (("LatestBlitProcessingTime    = %d.%d ms\n\n", (sblitStatistics.LatestBlitProcessingTime/1000), (sblitStatistics.LatestBlitProcessingTime%1000) ));
    STTBX_Print (("MinExecutionRate            = %d.%d MP/s\n", (sblitStatistics.MinExecutionRate/1000), (sblitStatistics.MinExecutionRate%1000) ));
#endif /* STBLIT_DEBUG_STATISTICS_USE_FIRST_AND_LAST_TIME_VALUE_ONLY */

    STTBX_Print (("AverageExecutionRate        = %d.%d MP/s\n", (sblitStatistics.AverageExecutionRate/1000), (sblitStatistics.AverageExecutionRate%1000) ));

#ifndef STBLIT_DEBUG_STATISTICS_USE_FIRST_AND_LAST_TIME_VALUE_ONLY
    STTBX_Print (("MaxExecutionRate            = %d.%d MP/s\n", (sblitStatistics.MaxExecutionRate/1000), (sblitStatistics.MaxExecutionRate%1000) ));
    STTBX_Print (("LatestBlitExecutionRate     = %d.%d MP/s\n", (sblitStatistics.LatestBlitExecutionRate/1000), (sblitStatistics.LatestBlitExecutionRate%1000) ));
#endif /* STBLIT_DEBUG_STATISTICS_USE_FIRST_AND_LAST_TIME_VALUE_ONLY */

    STTBX_Print (("------------------------------------------------------\n\n\n"));
}
#endif

#if defined(STBLIT_DEBUG_GET_STATISTICS) || defined(STBLIT_DEBUG_GET_STATISTICS_APPLICATION_LEVEL)
/*-----------------------------------------------------------------------------
 * Function : BLIT_TestPerformences;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestPerformences (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name="Blitter\0";

    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;
    STBLIT_Handle_t         Handle;
    STBLIT_BlitContext_t    Context;
    STGXOBJ_Bitmap_t        SourceBitmap, SourceBitmap2;
    STGXOBJ_Rectangle_t     Rectangle;
#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
    STGXOBJ_Bitmap_t        TempBitmap;
#endif
    STGXOBJ_Palette_t       Palette;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;
    STEVT_SubscriberID_t    SubscriberID;

#if defined (STBLIT_DEBUG_GET_STATISTICS)
    U32                     i,j;
    STBLIT_Statistics_t     sblitStatistics;
#endif
    STOS_Clock_t            time;
    STGXOBJ_Color_t         Color;

    STBLIT_Source_t         Src1,Src2;
    STBLIT_Destination_t    Dst;

#if defined (STBLIT_DEBUG_GET_STATISTICS_APPLICATION_LEVEL)

typedef struct _STBLIT_Op_size
{
    U32  Width;
	U32  Height;
} STBLIT_Op_Size;

#if defined(ST_OSLINUX)
    struct timeval StartTime;
    struct timeval EndTime;
#else
    STOS_Clock_t            TimeStart = 0;
    STOS_Clock_t            TimeStop  = 0;
#endif

    STBLIT_Op_Size BlitOpSizeTable[] = { {10, 10}, {20, 20}, {30, 30}, {40, 40}, {50, 50}, {100, 100}, {200, 200}, {300, 300}};

#define NUM_BLIT_OP_SIZE_TABLE_ELEM ((sizeof(BlitOpSizeTable))/(sizeof(STBLIT_Op_Size)))

#define NUM_BLIT_OP_LOOPS   100
    U32                     NumLoops;
    U32                     NumSizes;

#endif /*(STBLIT_DEBUG_GET_STATISTICS_APPLICATION_LEVEL)*/


    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                       = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                       = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 1;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188)|| defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 500;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 500;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber                     = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 6;

/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name,&InitParams);
        if (Err != ST_NO_ERROR  &&  Err != ST_ERROR_ALREADY_INITIALIZED)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }




    /* ------------ Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    BlitCompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);

    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = BlitCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&SubscriberID);

    /* ------------ Set Src ------------ */
    STTBX_Print (("-> Load source\n"));
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE1_FILE_NAME,SrcPartitionInfo,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    STTBX_Print (("-> Load source 2\n"));
    SourceBitmap2.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_SOURCE2_FILE_NAME,SrcPartitionInfo,&SourceBitmap2,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

#ifndef STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST
    /* ------------ Set Dst ------------ */
    STTBX_Print (("-> Load destination\n"));
    TempBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_FEATURES_TARGET1_FILE_NAME,DstPartitionInfo,&TempBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    TargetBitmap = TempBitmap;
#endif /* STBLIT_USE_EXTERN_TARGET_BITMAP_FOR_TEST */


    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = NULL;
    Context.EnableResizeFilter      = TRUE;
    Context.EventSubscriberID       = SubscriberID;

    /*=======================================================================
            Test1: Test copy
      =======================================================================*/
    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test1: Copy\n"));
    STTBX_Print (("===================================\n"));

#if defined (STBLIT_DEBUG_GET_STATISTICS_APPLICATION_LEVEL)
    /* ------------ Set Rectangle & Destinantion ------------ */
    Rectangle.PositionX = 10;
    Rectangle.PositionY = 10;

    printf ("STBLIT Operation      Op Size (WxH)          Time (usec) for %d blits \n", NUM_BLIT_OP_LOOPS);
    printf ("----------------------------------------------------------------------\n");
    for (NumSizes = 0; NumSizes < NUM_BLIT_OP_SIZE_TABLE_ELEM; NumSizes ++)
    {
        Rectangle.Width     = BlitOpSizeTable[NumSizes].Width ;
        Rectangle.Height    = BlitOpSizeTable[NumSizes].Height;

        printf ("COPY\t\t\t%dx%d\t\t\t", Rectangle.Width, Rectangle.Height);

#if defined(ST_OSLINUX)
        fflush(stdout);
        if (gettimeofday(&StartTime, NULL) < 0)
        {
            printf ("%s:.........gettimeofday() returned error \n", __FUNCTION__);
        }
#else
        TimeStart = STOS_time_now();
#endif

        for (NumLoops = 0; NumLoops < NUM_BLIT_OP_LOOPS; NumLoops ++)
        {
            /* ------------ Blit operation ------------ */
            Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&TargetBitmap,0,0,&Context);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Err Blit%d\n"));
                return (FALSE);
            }

            /* ------------ Wait complete event ------------ */
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond() * TEST_PERFORMENCES_DELAY);

            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Print(("Timeout%d\n"));
                return (FALSE);
            }

        }/*for NumLoops*/
#if defined(ST_OSLINUX)
        if (gettimeofday(&EndTime, NULL) < 0)
        {
            printf ("%s:.........gettimeofday() returned error \n", __FUNCTION__);
        }
#else
        TimeStop = STOS_time_now();
#endif

#if defined(ST_OSLINUX)
        printf ("%d \n",  EndTime.tv_usec - StartTime.tv_usec + ((EndTime.tv_sec - StartTime.tv_sec) * 1000000));
#elif defined(ST_OS21)
        printf ("%d \n", ((TimeStop - TimeStart)*1000000)/ST_GetClocksPerSecond());
#elif defined(ST_OS20)
        printf ("%d \n", ((TimeStop - TimeStart)*1000)/(ST_GetClocksPerSecond()/1000));
#endif

    }/* for NumSizes */
#endif /*(STBLIT_DEBUG_GET_STATISTICS_APPLICATION_LEVEL)*/


#if defined (STBLIT_DEBUG_GET_STATISTICS)
    /* Reset Statistics */
    Err = STBLIT_ResetStatistics(Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }
    /* Test small Width & Height */
    STTBX_Print (("Test small Width & Height\n"));

    /* ------------ Init test ------------ */
    Rectangle.Width  = 10;
    Rectangle.Height = 10;

    for (i=0 ; i < (SourceBitmap.Width - Rectangle.Width) ; i += TEST_PERFORMENCES_STEP)
    {
        for (j=0 ; j < (SourceBitmap.Height - Rectangle.Height) ; j += TEST_PERFORMENCES_STEP)
        {
            /* ------------ Set Rectangle & Destinantion ------------ */
            Rectangle.PositionX = i;
            Rectangle.PositionY = j;

            /* ------------ Blit operation ------------ */
            Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&TargetBitmap,0,0,&Context);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Err Blit%d\n"));
                return (FALSE);
            }

            /* ------------ Wait complete event ------------ */
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond() * TEST_PERFORMENCES_DELAY);

            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Print(("Timeout%d\n"));
                return (FALSE);
            }
        }
    }

    /* ------------ Get STBLIT Statistics ------------ */
    Err = STBLIT_GetStatistics(Handle,&sblitStatistics);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }
    else
    {
        /* report test results */
        BLIT_ReportStatistics(sblitStatistics);
    }

    /* Reset Statistics */
    Err = STBLIT_ResetStatistics(Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* Test medium Width & Height */
    STTBX_Print (("Test medium Width & Height\n"));

    /* ------------ Init test ------------ */
    Rectangle.Width  = 100;
    Rectangle.Height = 100;

    for (i=0 ; i < (SourceBitmap.Width - Rectangle.Width) ; i+=TEST_PERFORMENCES_STEP)
    {
        for (j=0 ; j < (SourceBitmap.Height - Rectangle.Height) ; j += TEST_PERFORMENCES_STEP)
        {
            /* ------------ Set Rectangle & Destinantion ------------ */
            Rectangle.PositionX = i;
            Rectangle.PositionY = j;

            /* ------------ Blit operation ------------ */
            Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&TargetBitmap,0,0,&Context);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Err Blit%d\n"));
                return (FALSE);
            }

            /* ------------ Wait complete event ------------ */
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond() * TEST_PERFORMENCES_DELAY);

            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Print(("Timeout%d\n"));
                return (FALSE);
            }
        }
    }

    /* ------------ Get STBLIT Statistics ------------ */
    Err = STBLIT_GetStatistics(Handle,&sblitStatistics);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }
    else
    {
        /* report test results */
        BLIT_ReportStatistics(sblitStatistics);
    }

    /* Reset Statistics */
    Err = STBLIT_ResetStatistics(Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* Test large Width & Height */
    STTBX_Print (("Test large Width & Height\n"));

    /* ------------ Init test ------------ */
    Rectangle.Width  = (SourceBitmap.Width / 2);
    Rectangle.Height = (SourceBitmap.Height / 2);

    for (i=0 ; i < (SourceBitmap.Width - Rectangle.Width) ; i+=TEST_PERFORMENCES_STEP)
    {
        for (j=0 ; j < (SourceBitmap.Height - Rectangle.Height) ; j+=TEST_PERFORMENCES_STEP)
        {
            /* ------------ Set Rectangle & Destinantion ------------ */
            Rectangle.PositionX = i;
            Rectangle.PositionY = j;

            /* ------------ Blit operation ------------ */
            Err = STBLIT_CopyRectangle(Handle,&SourceBitmap,&Rectangle,&TargetBitmap,0,0,&Context);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Err Blit%d\n"));
                return (FALSE);
            }

            /* ------------ Wait complete event ------------ */
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond() * TEST_PERFORMENCES_DELAY);

            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Print(("Timeout%d\n"));
                return (FALSE);
            }
        }
    }

    /* ------------ Get STBLIT Statistics ------------ */
    Err = STBLIT_GetStatistics(Handle,&sblitStatistics);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }
    else
    {
        /* report test results */
        BLIT_ReportStatistics(sblitStatistics);
    }

#endif /*(STBLIT_DEBUG_GET_STATISTICS)*/


    /*=======================================================================
            Test2: Test Fill
      =======================================================================*/
    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test2: Fill\n"));
    STTBX_Print (("===================================\n"));

    /* Set Color */
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_ARGB8888)
    {
        Color.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888 ;
        Color.Value.ARGB8888.Alpha = 0xFF; ;
        Color.Value.ARGB8888.R     = 0xFF; ;
        Color.Value.ARGB8888.G     = 0x00 ;
        Color.Value.ARGB8888.B     = 0x00 ;
    }
    if (TargetBitmap.ColorType == STGXOBJ_COLOR_TYPE_RGB565)
    {
        Color.Type               = STGXOBJ_COLOR_TYPE_RGB565 ;
        Color.Value.RGB565.R     = 0xff ;
        Color.Value.RGB565.G     = 0x00 ;
        Color.Value.RGB565.B     = 0x00 ;
    }

#if defined (STBLIT_DEBUG_GET_STATISTICS_APPLICATION_LEVEL)

    /* ------------ Set Rectangle & Destinantion ------------ */
    Rectangle.PositionX = 10;
    Rectangle.PositionY = 10;

    printf ("STBLIT Operation      Op Size (WxH)          Time (usec) for %d blits \n", NUM_BLIT_OP_LOOPS);
    printf ("----------------------------------------------------------------------\n");
    for (NumSizes = 0; NumSizes < NUM_BLIT_OP_SIZE_TABLE_ELEM; NumSizes ++)
    {
        Rectangle.Width     = BlitOpSizeTable[NumSizes].Width ;
        Rectangle.Height    = BlitOpSizeTable[NumSizes].Height;

        printf ("FILL\t\t\t%dx%d\t\t\t", Rectangle.Width, Rectangle.Height);

#if defined(ST_OSLINUX)
        fflush(stdout);
        if (gettimeofday(&StartTime, NULL) < 0)
        {
            printf ("%s:.........gettimeofday() returned error \n", __FUNCTION__);
        }
#else
        TimeStart = STOS_time_now();
#endif

        for (NumLoops = 0; NumLoops < NUM_BLIT_OP_LOOPS; NumLoops ++)
        {
            /* ------------ Blit operation ------------ */
            Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&Rectangle,&Color,&Context);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Err Blit%d\n"));
                return (FALSE);
            }

            /* ------------ Wait complete event ------------ */
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond() * TEST_PERFORMENCES_DELAY);

            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Print(("Timeout%d\n"));
                return (FALSE);
            }

        }/*for NumLoops*/
#if defined(ST_OSLINUX)
        if (gettimeofday(&EndTime, NULL) < 0)
        {
            printf ("%s:.........gettimeofday() returned error \n", __FUNCTION__);
        }
#else
        TimeStop = STOS_time_now();
#endif

#if defined(ST_OSLINUX)
        printf ("%d \n",  EndTime.tv_usec - StartTime.tv_usec + ((EndTime.tv_sec - StartTime.tv_sec) * 1000000));
#elif defined(ST_OS21)
        printf ("%d \n", ((TimeStop - TimeStart)*1000000)/ST_GetClocksPerSecond());
#elif defined(ST_OS20)
        printf ("%d \n", ((TimeStop - TimeStart)*1000)/(ST_GetClocksPerSecond()/1000));
#endif

    }/* for NumSizes */
#endif /*(STBLIT_DEBUG_GET_STATISTICS_APPLICATION_LEVEL)*/



#if defined (STBLIT_DEBUG_GET_STATISTICS)

    /* Reset Statistics */
    Err = STBLIT_ResetStatistics(Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* Test small Width & Height */
    STTBX_Print (("Test small Width & Height\n"));

    /* ------------ Init test ------------ */
    Rectangle.Width  = 10;
    Rectangle.Height = 10;

    for (i=0 ; i < (SourceBitmap.Width - Rectangle.Width) ; i+=TEST_PERFORMENCES_STEP)
    {
        for (j=0 ; j < (SourceBitmap.Height - Rectangle.Height) ; j+=TEST_PERFORMENCES_STEP)
        {
            /* ------------ Set Rectangle & Destinantion ------------ */
            Rectangle.PositionX = i;
            Rectangle.PositionY = j;

            /* ------------ Blit operation ------------ */
            Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&Rectangle,&Color,&Context);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Err Blit%d\n"));
                return (FALSE);
            }

            /* ------------ Wait complete event ------------ */
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond() * TEST_PERFORMENCES_DELAY);

            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Print(("Timeout%d\n"));
                return (FALSE);
            }
        }
    }

    /* ------------ Get STBLIT Statistics ------------ */
    Err = STBLIT_GetStatistics(Handle,&sblitStatistics);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }
    else
    {
        /* report test results */
        BLIT_ReportStatistics(sblitStatistics);
    }

    /* Reset Statistics */
    Err = STBLIT_ResetStatistics(Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* Test medium Width & Height */
    STTBX_Print (("Test medium Width & Height\n"));

    /* ------------ Init test ------------ */
    Rectangle.Width  = 100;
    Rectangle.Height = 100;

    for (i=0 ; i < (SourceBitmap.Width - Rectangle.Width) ; i+=TEST_PERFORMENCES_STEP)
    {
        for (j=0 ; j < (SourceBitmap.Height - Rectangle.Height) ; j+=TEST_PERFORMENCES_STEP)
        {
            /* ------------ Set Rectangle & Destinantion ------------ */
            Rectangle.PositionX = i;
            Rectangle.PositionY = j;

            /* ------------ Blit operation ------------ */
            Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&Rectangle,&Color,&Context);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Err Blit%d\n"));
                return (FALSE);
            }

            /* ------------ Wait complete event ------------ */
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond() * TEST_PERFORMENCES_DELAY);

            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Print(("Timeout%d\n"));
                return (FALSE);
            }
        }
    }

    /* ------------ Get STBLIT Statistics ------------ */
    Err = STBLIT_GetStatistics(Handle,&sblitStatistics);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }
    else
    {
        /* report test results */
        BLIT_ReportStatistics(sblitStatistics);
    }

    /* Reset Statistics */
    Err = STBLIT_ResetStatistics(Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* Test large Width & Height */
    STTBX_Print (("Test large Width & Height\n"));

    /* ------------ Init test ------------ */
    Rectangle.Width  = (SourceBitmap.Width / 2);
    Rectangle.Height = (SourceBitmap.Height / 2);

    for (i=0 ; i < (SourceBitmap.Width - Rectangle.Width) ; i+=TEST_PERFORMENCES_STEP)
    {
        for (j=0 ; j < (SourceBitmap.Height - Rectangle.Height) ; j+=TEST_PERFORMENCES_STEP)
        {
            /* ------------ Set Rectangle & Destinantion ------------ */
            Rectangle.PositionX = i;
            Rectangle.PositionY = j;

            /* ------------ Blit operation ------------ */
            Err = STBLIT_FillRectangle(Handle,&TargetBitmap,&Rectangle,&Color,&Context);
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Err Blit%d\n"));
                return (FALSE);
            }

            /* ------------ Wait complete event ------------ */
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond() * TEST_PERFORMENCES_DELAY);

            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Print(("Timeout%d\n"));
                return (FALSE);
            }
        }
    }


    /* ------------ Get STBLIT Statistics ------------ */
    Err = STBLIT_GetStatistics(Handle,&sblitStatistics);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }
    else
    {
        /* report test results */
        BLIT_ReportStatistics(sblitStatistics);
    }

#endif /*(STBLIT_DEBUG_GET_STATISTICS)*/

    /*=======================================================================
            Test3: Test Blit
      =======================================================================*/
    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test3: Blit\n"));
    STTBX_Print (("===================================\n"));



    /* ------------ Set Source1 and Source2 ------------ */
    Src1.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src1.Data.Bitmap_p        = &SourceBitmap;
    Src1.Palette_p            = NULL;

    Src2.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src2.Data.Bitmap_p        = &SourceBitmap2;
    Src2.Palette_p            = NULL;


    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &TargetBitmap;
    Dst.Palette_p             = NULL;

#if defined (STBLIT_DEBUG_GET_STATISTICS_APPLICATION_LEVEL)

    /* ------------ Blit operation ------------ */
    Src1.Rectangle.PositionX  = 10;
    Src1.Rectangle.PositionY  = 10;

    Src2.Rectangle.PositionX  = 10;
    Src2.Rectangle.PositionY  = 10;

    Dst.Rectangle.PositionX   = 10;
    Dst.Rectangle.PositionY   = 10;


    printf ("STBLIT Operation      Op Size (WxH)          Time (usec) for %d blits \n", NUM_BLIT_OP_LOOPS);
    printf ("----------------------------------------------------------------------\n");
    for (NumSizes = 0; NumSizes < NUM_BLIT_OP_SIZE_TABLE_ELEM; NumSizes ++)
    {
        Src1.Rectangle.Width      = BlitOpSizeTable[NumSizes].Width;
        Src1.Rectangle.Height     = BlitOpSizeTable[NumSizes].Height;

        Src2.Rectangle.Width      = BlitOpSizeTable[NumSizes].Width;
        Src2.Rectangle.Height     = BlitOpSizeTable[NumSizes].Height;

        Dst.Rectangle.Width       = BlitOpSizeTable[NumSizes].Width;
        Dst.Rectangle.Height      = BlitOpSizeTable[NumSizes].Height;

        printf ("COPY2SRC\t\t%dx%d\t\t\t", Src1.Rectangle.Width, Src1.Rectangle.Height);

#if defined(ST_OSLINUX)
        fflush(stdout);
        if (gettimeofday(&StartTime, NULL) < 0)
        {
            printf ("%s:.........gettimeofday() returned error \n", __FUNCTION__);
        }
#else
        TimeStart = STOS_time_now();
#endif

        for (NumLoops = 0; NumLoops < NUM_BLIT_OP_LOOPS; NumLoops ++)
        {

            /* ------------ Blit operation ------------ */
            Err = STBLIT_Blit(Handle,&Src1,&Src2,&Dst,&Context );
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Err Blit%d\n"));
                return (FALSE);
            }

            /* ------------ Wait complete event ------------ */
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond() * TEST_PERFORMENCES_DELAY);

            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Print(("Timeout%d\n"));
                return (FALSE);
            }

        }/*for NumLoops*/
#if defined(ST_OSLINUX)
        if (gettimeofday(&EndTime, NULL) < 0)
        {
            printf ("%s:.........gettimeofday() returned error \n", __FUNCTION__);
        }
#else
        TimeStop = STOS_time_now();
#endif

#if defined(ST_OSLINUX)
        printf ("%d \n",  EndTime.tv_usec - StartTime.tv_usec + ((EndTime.tv_sec - StartTime.tv_sec) * 1000000));
#elif defined(ST_OS21)
        printf ("%d \n", ((TimeStop - TimeStart)*1000000)/ST_GetClocksPerSecond());
#elif defined(ST_OS20)
        printf ("%d \n", ((TimeStop - TimeStart)*1000)/(ST_GetClocksPerSecond()/1000));
#endif

    }/* for NumSizes */
#endif /*(STBLIT_DEBUG_GET_STATISTICS_APPLICATION_LEVEL)*/


#if defined (STBLIT_DEBUG_GET_STATISTICS)
    /* Reset Statistics */
    Err = STBLIT_ResetStatistics(Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* Test small Width & Height */
    STTBX_Print (("Test small Width & Height\n"));

    /* ------------ Init test ------------ */
    Src1.Rectangle.Width      = 10;
    Src1.Rectangle.Height     = 10;
    Src2.Rectangle.Width      = 10;
    Src2.Rectangle.Height     = 10;
    Dst.Rectangle.Width       = 10;
    Dst.Rectangle.Height      = 10;

    for (i=0 ; i < (SourceBitmap.Width - Src1.Rectangle.Width) ; i+=TEST_PERFORMENCES_STEP)
    {
        for (j=0 ; j < (SourceBitmap.Height - Src1.Rectangle.Height) ; j+=TEST_PERFORMENCES_STEP)
        {
            /* ------------ Set Rectangle & Destinantion ------------ */
            Src1.Rectangle.PositionX  = i;
            Src1.Rectangle.PositionY  = j;
            Src2.Rectangle.PositionX  = i;
            Src2.Rectangle.PositionY  = j;
            Dst.Rectangle.PositionX   = i;
            Dst.Rectangle.PositionY   = j;


            /* ------------ Blit operation ------------ */
            Err = STBLIT_Blit(Handle,&Src1,&Src2,&Dst,&Context );
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Err Blit%d\n"));
                return (FALSE);
            }

            /* ------------ Wait complete event ------------ */
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond() * TEST_PERFORMENCES_DELAY);

            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Print(("Timeout%d\n"));
                return (FALSE);
            }
        }
    }

    /* ------------ Get STBLIT Statistics ------------ */
    Err = STBLIT_GetStatistics(Handle,&sblitStatistics);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }
    else
    {
        /* report test results */
        BLIT_ReportStatistics(sblitStatistics);
    }

    /* Reset Statistics */
    Err = STBLIT_ResetStatistics(Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* Test medium Width & Height */
    STTBX_Print (("Test medium Width & Height\n"));

    /* ------------ Init test ------------ */
    Src1.Rectangle.Width      = 100;
    Src1.Rectangle.Height     = 100;
    Src2.Rectangle.Width      = 100;
    Src2.Rectangle.Height     = 100;
    Dst.Rectangle.Width       = 100;
    Dst.Rectangle.Height      = 100;

    for (i=0 ; i < (SourceBitmap.Width - Src1.Rectangle.Width) ; i+=TEST_PERFORMENCES_STEP)
    {
        for (j=0 ; j < (SourceBitmap.Height - Src1.Rectangle.Height) ; j+=TEST_PERFORMENCES_STEP)
        {
            /* ------------ Set Rectangle & Destinantion ------------ */
            Src1.Rectangle.PositionX  = i;
            Src1.Rectangle.PositionY  = j;
            Src2.Rectangle.PositionX  = i;
            Src2.Rectangle.PositionY  = j;
            Dst.Rectangle.PositionX   = i;
            Dst.Rectangle.PositionY   = j;

            /* ------------ Blit operation ------------ */
            Err = STBLIT_Blit(Handle,&Src1,&Src2,&Dst,&Context );
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Err Blit%d\n"));
                return (FALSE);
            }

            /* ------------ Wait complete event ------------ */
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond() * TEST_PERFORMENCES_DELAY);

            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Print(("Timeout%d\n"));
                return (FALSE);
            }
        }
    }

    /* ------------ Get STBLIT Statistics ------------ */
    Err = STBLIT_GetStatistics(Handle,&sblitStatistics);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }
    else
    {
        /* report test results */
        BLIT_ReportStatistics(sblitStatistics);
    }

    /* Reset Statistics */
    Err = STBLIT_ResetStatistics(Handle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }

    /* Test large Width & Height */
    STTBX_Print (("Test large Width & Height\n"));

    /* ------------ Init test ------------ */
    Src1.Rectangle.Width      = (SourceBitmap.Width / 2);
    Src1.Rectangle.Height     = (SourceBitmap.Height / 2);
    Src2.Rectangle.Width      = (SourceBitmap.Width / 2);
    Src2.Rectangle.Height     = (SourceBitmap.Height / 2);
    Dst.Rectangle.Width       = (SourceBitmap.Width / 2);
    Dst.Rectangle.Height      = (SourceBitmap.Height / 2);

    for (i=0 ; i < (SourceBitmap.Width - Src1.Rectangle.Width) ; i+=TEST_PERFORMENCES_STEP)
    {
        for (j=0 ; j < (SourceBitmap.Height - Src1.Rectangle.Height) ; j+=TEST_PERFORMENCES_STEP)
        {
            /* ------------ Set Rectangle & Destinantion ------------ */
            Src1.Rectangle.PositionX  = i;
            Src1.Rectangle.PositionY  = j;
            Src2.Rectangle.PositionX  = i;
            Src2.Rectangle.PositionY  = j;
            Dst.Rectangle.PositionX   = i;
            Dst.Rectangle.PositionY   = j;

            /* ------------ Blit operation ------------ */
            Err = STBLIT_Blit(Handle,&Src1,&Src2,&Dst,&Context );
            if (Err != ST_NO_ERROR)
            {
                STTBX_Print(("Err Blit%d\n"));
                return (FALSE);
            }

            /* ------------ Wait complete event ------------ */
            time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond() * TEST_PERFORMENCES_DELAY);

            if(STOS_SemaphoreWaitTimeOut(BlitCompletionSemaphore_p,&time)!=0)
            {
                STTBX_Print(("Timeout%d\n"));
                return (FALSE);
            }
        }
    }

    /* ------------ Get STBLIT Statistics ------------ */
    Err = STBLIT_GetStatistics(Handle,&sblitStatistics);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Print(("Err Blit%d\n"));
        return (FALSE);
    }
    else
    {
        /* report test results */
        BLIT_ReportStatistics(sblitStatistics);
    }
#endif /*(STBLIT_DEBUG_GET_STATISTICS)*/



    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(SourceBitmap.Data1_p);
    GUTIL_Free(TargetBitmap.Data1_p);

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
#ifdef PTV
    Err = STBLIT_Close(Handle);
#else
    Err = STBLIT_Term(Name,&TermParams);
#endif /* PTV */
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}
#endif /* STBLIT_DEBUG_GET_STATISTICS && STBLIT_DEBUG_GET_STATISTICS_APPLICATION_LEVEL*/


/*-----------------------------------------------------------------------------
 * Function : BLIT_TestCRC
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestCRC (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    STGXOBJ_Bitmap_t        SourceBitmap;
    STGXOBJ_Palette_t       Palette;
    U32                     Time, Count2, CRCValue;
    char                    FileName[40];
    STOS_Clock_t            t1, t2;
    U32                     Tdiff, NbPic;


    /* ------------ Set Src ------------ */
    SourceBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;
    /*strcpy(FileName,"../../SidiBou.gam");*/
    strcpy(FileName,"../../ElJam.gam");

    Err = ConvertGammaToBitmap(FileName,SrcPartitionInfo,&SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    STTBX_Print(("-> Start init CRC table\n"));

    /* Init CRCValue */
    Blit_InitCrc32table();

    STTBX_Print(("-> Start CRC calc\n"));

    t1 = STOS_time_now();

#if 1
    for (Count2 = 0 ; Count2 < 200 ; Count2++)
    {
        /* Calculate  CRC Value */
        CRCValue = GetBitmapCRC(&SourceBitmap);
    }
#else
    memset(SourceBitmap.Data1_p , 0, SourceBitmap.Size1);
#endif

    t2 = STOS_time_now();

    STTBX_Print(("-> CRC=%x\n", CRCValue));

    Tdiff = ((U32)(STOS_time_minus(t2,t1))) * 1000;
    NbPic = (U32)(ST_GetClocksPerSecond());
    Time  = (U32)(Tdiff/NbPic);

    STTBX_Print(("-> T1 = %lu\n", (U32)t1));
    STTBX_Print(("-> T2 = %lu\n", (U32)t2));
    STTBX_Print(("-> Tdiff = %lu\n", Tdiff));
    STTBX_Print(("-> NbPic = %lu\n", NbPic));
    STTBX_Print(("-> Time = %lu\n", Time));

    return(FALSE);
}


/*-----------------------------------------------------------------------------
 * Function : BLIT_TestMultiInstances;
 * Input    : *pars_p, *result_sym_p
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * --------------------------------------------------------------------------*/
BOOL BLIT_TestMultiInstances (parse_t *pars_p, char *result_sym_p)
{
    ST_ErrorCode_t          Err;
    ST_DeviceName_t         Name = "Blitter\0";
    STBLIT_InitParams_t     InitParams;
    STBLIT_OpenParams_t     OpenParams;
    STBLIT_TermParams_t     TermParams;

    STGXOBJ_Palette_t       Palette;

    STEVT_OpenParams_t      EvtOpenParams;
    STEVT_Handle_t          EvtHandle;
    STEVT_DeviceSubscribeParams_t EvtSubscribeParams;

    FILE*                   CRCfstream;
    U32                     CRCValuesTab[TEST_FEATURES_NB_TESTS];
    U32                     CRCValue1, CRCValue2;

    /* ------------ Blit Device Init ------------ */
    InitParams.CPUPartition_p                       = SystemPartition_p;
    InitParams.DeviceType                           = CURRENT_DEVICE_TYPE;
    InitParams.BaseAddress_p                        = CURRENT_BASE_ADDRESS;
#ifdef ST_OSLINUX
    InitParams.AVMEMPartition                       = (STAVMEM_PartitionHandle_t)NULL;
#else
    InitParams.AVMEMPartition                       = AvmemPartitionHandle[AVMEM_PARTITION_NUM];
#endif
    InitParams.SharedMemoryBaseAddress_p            = (void*)TEST_SHARED_MEM_BASE_ADDRESS;
    InitParams.MaxHandles                           = 10;

    InitParams.SingleBlitNodeBufferUserAllocated    = FALSE;
#if defined (ST_5100) || defined (ST_5105) || defined (ST_5301) || defined(ST_7109) || defined (ST_5188)|| defined (ST_5525)\
 || defined (ST_5107) || defined (ST_7200)
    InitParams.SingleBlitNodeMaxNumber              = 1000;
#else
    InitParams.SingleBlitNodeMaxNumber              = 30;
#endif
/*    InitParams.SingleBlitNodeBuffer_p             = NULL;*/

    InitParams.JobBlitNodeBufferUserAllocated       = FALSE;
    InitParams.JobBlitNodeMaxNumber                 = 500;
/*    InitParams.JobBlitNodeBuffer_p                = NULL;*/

    InitParams.JobBlitBufferUserAllocated = FALSE;
    InitParams.JobBlitMaxNumber       = 30;
/*    InitParams.JobBlitBuffer_p        = NULL;*/

    InitParams.JobBufferUserAllocated               = FALSE;
    InitParams.JobMaxNumber                         = 6;

/*    InitParams.JobBuffer_p                        = NULL;*/

    InitParams.WorkBufferUserAllocated              = FALSE;
/*    InitParams.WorkBuffer_p                       = NULL;*/
    InitParams.WorkBufferSize             = WORK_BUFFER_SIZE;
    strcpy(InitParams.EventHandlerName,STEVT_DEVICE_NAME);

#if !defined(STBLIT_EMULATOR) && !defined(STBLIT_LINUX_FULL_USER_VERSION)
#if defined (ST_5528) || defined (ST_5100) || defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_5301)\
 || defined (ST_7109) || defined (ST_5188) || defined (ST_5525) || defined (ST_5107) || defined(ST_7200)
    InitParams.BlitInterruptLevel   = BLITTER_INT_LEVEL ;
    InitParams.BlitInterruptNumber  = BLITTER_INT_EVENT;
#else
    InitParams.BlitInterruptEvent = BLITTER_INT_EVENT;
    strcpy(InitParams.BlitInterruptEventName,STINTMR_DEVICE_NAME);
#endif
#endif

#ifdef ST_OSLINUX
    layer_init();
#endif

#ifdef PTV
    if (STBLITInitDone == FALSE)
#endif /* PTV */
    {
        Err = STBLIT_Init(Name, &InitParams);
        if (Err != ST_NO_ERROR  &&  Err != ST_ERROR_ALREADY_INITIALIZED)
        {
            STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init : %d\n",Err));
            return (TRUE);
        }
#ifdef PTV
      STBLITInitDone = TRUE;
#endif /* PTV */
    }

    /* ------------ Instances Blit Open ------------ */
    Err = STBLIT_Open(Name,&OpenParams,&STBlitHandle1);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open Instance 1 : %d\n",Err));
        return (TRUE);
    }

    Err = STBLIT_Open(Name,&OpenParams,&STBlitHandle2);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open Instance 2 : %d\n",Err));
        return (TRUE);
    }

    Err = STBLIT_Open(Name,&OpenParams,&STBlitHandle3);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open Instance 3 : %d\n",Err));
        return (TRUE);
    }

    Err = STBLIT_Open(Name,&OpenParams,&STBlitHandle4);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Open Instance 4 : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Initialize global semaphores ------------ */
    Task1CompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);
    Task2CompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);
    Task3CompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);
    Task4CompletionSemaphore_p = STOS_SemaphoreCreateFifoTimeOut(NULL,0);
    Task1StartSemaphore_p      = STOS_SemaphoreCreateFifo(NULL,0);
    Task1CompletedSemaphore_p  = STOS_SemaphoreCreateFifo(NULL,0);
    Task2StartSemaphore_p      = STOS_SemaphoreCreateFifo(NULL,0);
    Task2CompletedSemaphore_p  = STOS_SemaphoreCreateFifo(NULL,0);
    Task3StartSemaphore_p      = STOS_SemaphoreCreateFifo(NULL,0);
    Task3CompletedSemaphore_p  = STOS_SemaphoreCreateFifo(NULL,0);
    Task4StartSemaphore_p      = STOS_SemaphoreCreateFifo(NULL,0);
    Task4CompletedSemaphore_p  = STOS_SemaphoreCreateFifo(NULL,0);


    /* ------------ Open Event handler -------------- */
    Err = STEVT_Open(STEVT_DEVICE_NAME, &EvtOpenParams, &EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Open : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Subscribe to Blit Completed event ---------------- */
    EvtSubscribeParams.NotifyCallback     = TaskCompletedHandler;
    EvtSubscribeParams.SubscriberData_p   = NULL;
    Err = STEVT_SubscribeDeviceEvent(EvtHandle,Name,STBLIT_BLIT_COMPLETED_EVT,&EvtSubscribeParams);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Subscribe Blit completion : %d\n",Err));
        return (TRUE);
    }


    /* ------------ Get Subscriber ID ------------ */
    STEVT_GetSubscriberID(EvtHandle,&MultiInstancesTest_SubscriberID);



    /* ------------ Set Src ------------ */
    STTBX_Print (("-> Load source\n"));
    MultiInstancesTest_SourceBitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_MULTI_INSTANCES_SOURCE1_FILE_NAME,SrcPartitionInfo,&MultiInstancesTest_SourceBitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Dst ------------ */
    STTBX_Print (("-> Load destination 1\n"));
    MultiInstancesTest_Target1Bitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_MULTI_INSTANCES_TARGET1_FILE_NAME,DstPartitionInfo,&MultiInstancesTest_Target1Bitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Set Dst ------------ */
    STTBX_Print (("-> Load destination 2\n"));
    MultiInstancesTest_Target2Bitmap.Data1_p = NULL;
    Palette.Data_p       = NULL;

    Err = ConvertGammaToBitmap(TEST_MULTI_INSTANCES_TARGET2_FILE_NAME,DstPartitionInfo,&MultiInstancesTest_Target2Bitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Init CRCValue ------------ */
    STTBX_Print (("-> Init Crc32 table\n"));
    Blit_InitCrc32table();

    /* ------------ Init Reference File ------------ */
	STTBX_Print (("-> Init Reference File\n"));
	Err = BLIT_InitReferenceFile (TEST_MULTI_INSTANCES_REF_FILE_NAME, &CRCfstream, CRCValuesTab);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Init Reference File : %d\n",Err));
        return (TRUE);
    }


    /*------------ Create and run task1 ------------*/
    STTBX_Print (("-> Create task1\n"));
    Err = STOS_TaskCreate ((void (*) (void*))MultiInstancesTest_Task1, (void*)NULL, SystemPartition_p, SM_TASK_STACK_SIZE,
                          &Task1Stack, SystemPartition_p, &Task1_p, &Task1Desc,
                          SM_TASK_PRIORITY, "Task1", 0 /*task_flags_no_min_stack_size*/ );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err create task1 : %d\n",Err));
        return(ST_ERROR_BAD_PARAMETER);
    }

    /*------------ Create and run task2 ------------*/
    STTBX_Print (("-> Create task2\n"));
    Err = STOS_TaskCreate ((void (*) (void*))MultiInstancesTest_Task2, (void*)NULL, SystemPartition_p, SM_TASK_STACK_SIZE,
                          &Task2Stack, SystemPartition_p, &Task2_p, &Task2Desc,
                          SM_TASK_PRIORITY, "Task2", 0 /*task_flags_no_min_stack_size*/ );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err create task2 : %d\n",Err));
        return(ST_ERROR_BAD_PARAMETER);
    }

    /*------------ Create and run task3 ------------*/
    STTBX_Print (("-> Create task3\n"));
    Err = STOS_TaskCreate ((void (*) (void*))MultiInstancesTest_Task3, (void*)NULL, SystemPartition_p, SM_TASK_STACK_SIZE,
                          &Task3Stack, SystemPartition_p, &Task3_p, &Task3Desc,
                          SM_TASK_PRIORITY, "Task3", 0 /*task_flags_no_min_stack_size*/ );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err create task3 : %d\n",Err));
        return(ST_ERROR_BAD_PARAMETER);
    }

    /*------------ Create and run task4 ------------*/
    STTBX_Print (("-> Create task4\n"));
    Err = STOS_TaskCreate ((void (*) (void*))MultiInstancesTest_Task4, (void*)NULL, SystemPartition_p, SM_TASK_STACK_SIZE,
                          &Task4Stack, SystemPartition_p, &Task4_p, &Task4Desc,
                          SM_TASK_PRIORITY, "Task4", 0 /*task_flags_no_min_stack_size*/ );
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err create task4 : %d\n",Err));
        return(ST_ERROR_BAD_PARAMETER);
    }

#if 1
    /*=======================================================================
            Test1: Synchronous mode
    =======================================================================*/
    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test1: Synchronous mode\n"));
    STTBX_Print (("===================================\n"));

    /* Enable wait for blitter complete event */
    MultiInstancesTest_WaitForNotification = TRUE;

    /* Start Task1, Task2, Task3, Task4 */
    Task1PerformBlit = TRUE;
    Task2PerformBlit = TRUE;
    Task3PerformBlit = TRUE;
    Task4PerformBlit = TRUE;
    STOS_SemaphoreSignal(Task1StartSemaphore_p);
    STOS_SemaphoreSignal(Task2StartSemaphore_p);
    STOS_SemaphoreSignal(Task3StartSemaphore_p);
    STOS_SemaphoreSignal(Task4StartSemaphore_p);

    /* Wait for Task2, Task3, Task4 to terminate */
    STOS_SemaphoreWait(Task2CompletedSemaphore_p);
    STOS_SemaphoreWait(Task3CompletedSemaphore_p);
    STOS_SemaphoreWait(Task4CompletedSemaphore_p);

    /* Stop Task1 */
    Task1PerformBlit = FALSE;

    /* Wait for Task1 to terminate */
    STOS_SemaphoreWait(Task1CompletedSemaphore_p);

    /* Check error code */
    if ((Task1ErrCode != ST_NO_ERROR) || (Task2ErrCode != ST_NO_ERROR) ||
        (Task3ErrCode != ST_NO_ERROR) || (Task4ErrCode != ST_NO_ERROR))
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Synchronous mode test failed\n"));
        return (TRUE);
    }


#ifdef TEST_FEATURE_SAVE_OUTPUT_BITMAP
    /* Generate Bitmap */
    STTBX_Print (("-> Generate Bitmap1\n"));
    Err = ConvertBitmapToGamma(TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP1_FILE_NAME,&MultiInstancesTest_Target1Bitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    STTBX_Print (("-> Generate Bitmap2\n"));
    Err = ConvertBitmapToGamma(TEST_MULTI_INSTANCES_RESULT_SYNCH_MODE_BITMAP2_FILE_NAME,&MultiInstancesTest_Target2Bitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue1 = GetBitmapCRC(&MultiInstancesTest_Target1Bitmap);
    CRCValue2 = GetBitmapCRC(&MultiInstancesTest_Target2Bitmap);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES

    /* ------------ Store result ------------ */
    fwrite((void *)&CRCValue1, 1, 4, CRCfstream);
    fwrite((void *)&CRCValue2, 1, 4, CRCfstream);

    STTBX_Print (("Multi Instances Synchronous mode Test CRC1=%x\n", CRCValue1));
    STTBX_Print (("Multi Instances Synchronous mode Test CRC2=%x\n", CRCValue2));
#else
    /* ------------ Determine test result ------------ */
    STTBX_Print (("_________________________________________________________________\n"));
    if ((CRCValue1 == CRCValuesTab[0]) & (CRCValue2 == CRCValuesTab[1]))
    {
        STTBX_Print (("Multi Instances Synchronous mode Test: OK\n\n\n"));
    }
    else
    {
        STTBX_Print (("Multi Instances Synchronous mode Test: FAILED\n\n\n"));
        return (TRUE);
    }
#endif

#endif


#if 0
    /*=======================================================================
            Test2: Asynchronous mode
    =======================================================================*/
    STTBX_Print (("===================================\n"));
    STTBX_Print (("Test2: Asynchronous mode\n"));
    STTBX_Print (("===================================\n"));

    /* Enable wait for blitter complete event */
    MultiInstancesTest_WaitForNotification = FALSE;

    /* Start Task1, Task2, Task3, Task4 */
    Task1PerformBlit = TRUE;
    Task2PerformBlit = TRUE;
    Task3PerformBlit = TRUE;
    Task4PerformBlit = TRUE;
    STOS_SemaphoreSignal(Task1StartSemaphore_p);
    STOS_SemaphoreSignal(Task2StartSemaphore_p);
    STOS_SemaphoreSignal(Task3StartSemaphore_p);
    STOS_SemaphoreSignal(Task4StartSemaphore_p);

    /* Wait for Task2, Task3, Task4 to terminate */
    STOS_SemaphoreWait(Task2CompletedSemaphore_p);
    STOS_SemaphoreWait(Task3CompletedSemaphore_p);
    STOS_SemaphoreWait(Task4CompletedSemaphore_p);

    /* Stop Task1 */
    Task1PerformBlit = FALSE;

    /* Wait for Task1 to terminate */
    STOS_SemaphoreWait(Task1CompletedSemaphore_p);

    /* Check error code */
    if ((Task1ErrCode != ST_NO_ERROR) || (Task2ErrCode != ST_NO_ERROR) ||
        (Task3ErrCode != ST_NO_ERROR) || (Task4ErrCode != ST_NO_ERROR))
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Asynchronous mode test failed\n"));
        return (TRUE);
    }

#if 0
	/* Generate Bitmap */
    STTBX_Print (("-> Generate Bitmap1\n"));
    Err = ConvertBitmapToGamma(TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP1_FILE_NAME,&MultiInstancesTest_Target1Bitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }

    STTBX_Print (("-> Generate Bitmap2\n"));
    Err = ConvertBitmapToGamma(TEST_MULTI_INSTANCES_RESULT_ASYNCH_MODE_BITMAP2_FILE_NAME,&MultiInstancesTest_Target2Bitmap,&Palette);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Convert Gamma to Bitmap : %d\n",Err));
        return (TRUE);
    }
#endif

    /* ------------ Calculate  CRC Value ------------ */
    CRCValue1 = GetBitmapCRC(&MultiInstancesTest_Target1Bitmap);
    CRCValue2 = GetBitmapCRC(&MultiInstancesTest_Target2Bitmap);

    /* ------------ Determine test result ------------ */
    STTBX_Print (("_________________________________________________________________\n"));
    if ((CRCValue1 == CRCValuesTab[0]) & (CRCValue2 == CRCValuesTab[1]))
    {
        STTBX_Print (("Multi Instances Asynchronous mode Test: OK\n\n\n"));
    }
    else
    {
        STTBX_Print (("Multi Instances Asynchronous mode Test: FAILED\n\n\n"));
        return (TRUE);
    }
#endif

    /* --------------- Free Bitmap ------------ */
    GUTIL_Free(MultiInstancesTest_SourceBitmap.Data1_p);
    GUTIL_Free(MultiInstancesTest_Target1Bitmap.Data1_p);
    GUTIL_Free(MultiInstancesTest_Target2Bitmap.Data1_p);

#ifdef TEST_FEATURES_SAVE_CRC_VALUES
    /* ------------ Term Reference File ------------ */
    BLIT_TermReferenceFile(CRCfstream);
#endif

    /* --------------- Close Evt ----------*/
    Err = STEVT_Close(EvtHandle);
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Event Close : %d\n",Err));
        return (TRUE);
    }

    /* ------------ Blit Term ------------ */
    TermParams.ForceTerminate = TRUE;
#ifdef PTV
    Err = STBLIT_Close(Handle);
#else
    Err = STBLIT_Term(Name,&TermParams);
#endif /* PTV */
    if (Err != ST_NO_ERROR)
    {
        STTBX_Report ((STTBX_REPORT_LEVEL_INFO,"Err Term : %d\n",Err));
        return (TRUE);
    }

#ifdef ST_OSLINUX
    layer_term();
#endif

    return(FALSE);
}

/*-------------------------------------------------------------------------
 * Function   : MultiInstancesTest_Task1
 * Input      :
 * Output     :
 * Return     :
 * Description:
 * ----------------------------------------------------------------------*/
static void MultiInstancesTest_Task1(void *Param_p)
{
    STBLIT_BlitContext_t    Context;
    STBLIT_Source_t         Src;
    STBLIT_Destination_t    Dst;
    STOS_Clock_t            time;
	STOS_Clock_t            Timeout;
    U32                     Count;



    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = Task1Tag;
    Context.EnableResizeFilter      = TRUE;
    Context.EventSubscriberID       = MultiInstancesTest_SubscriberID;

    /* ------------ Set Source ------------ */
    Src.Type                 = STBLIT_SOURCE_TYPE_BITMAP;
    Src.Data.Bitmap_p        = &MultiInstancesTest_SourceBitmap;
    Src.Palette_p            = NULL;
    Src.Rectangle.PositionX  = 10;
    Src.Rectangle.PositionY  = 10;
    Src.Rectangle.Width      = MultiInstancesTest_SourceBitmap.Width - 2*Src.Rectangle.PositionX;
    Src.Rectangle.Height     = MultiInstancesTest_SourceBitmap.Height - 2*Src.Rectangle.PositionY;

    /* ------------ Set Destinantion ------------ */
    Dst.Bitmap_p              = &MultiInstancesTest_Target1Bitmap;
    Dst.Palette_p             = NULL;
    Dst.Rectangle.PositionX   = 0;
    Dst.Rectangle.PositionY   = 0;
    Dst.Rectangle.Width       = MultiInstancesTest_Target1Bitmap.Width;
    Dst.Rectangle.Height      = MultiInstancesTest_Target1Bitmap.Height;

    /* ------------ Init Count ------------ */
    Count = 0;

    /* ------------ Init Task ErrCode ------------ */
    Task1ErrCode = ST_NO_ERROR;

    /* ------------ Start execution ------------ */
    while (Task1IsRunning)
    {
        /* Wait for main notification */
        STOS_SemaphoreWait(Task1StartSemaphore_p);

        STTBX_Print (("-> Task1 started\n"));

        while ((Task1PerformBlit) && (Task1ErrCode == ST_NO_ERROR))
        {
            /* ------------ Report task1 status ------------ */
            if ((Count%100) == 0)
            {
                STTBX_Print (("--> Task1 %d blits processed\n",Count));
            }

            /* ------------ Blit operation ------------ */
            Task1ErrCode = STBLIT_Blit(STBlitHandle1,NULL,&Src,&Dst,&Context );
			if (MultiInstancesTest_WaitForNotification)
            {
				if (Task1ErrCode != ST_NO_ERROR)
				{
                	STTBX_Print(("[!] Task 1 Err Blit Count=%d \n",Count));
                }
				else
				{
                    /* ------------ Wait complete event ------------ */
            #ifndef STBLIT_EMULATOR
                #ifdef ST_OSLINUX
                    time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
                #else
                    time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
                #endif
                    if(STOS_SemaphoreWaitTimeOut(Task1CompletionSemaphore_p,&time)!=0)
                    {
                    	STTBX_Print(("[!] Task 1 Timeout Count=%d \n",Count));
                    	Task1ErrCode = ST_ERROR_TIMEOUT;
                    }
            #else
                    /*  Wait for Blit to be completed */
                    STOS_SemaphoreWaitTimeOut(Task1CompletionSemaphore_p, TIMEOUT_INFINITY);
            #endif
				}
            }
			else /* Asynchronous mode */
			{
                while (Task1ErrCode == STBLIT_ERROR_MAX_SINGLE_BLIT_NODE)
                {
					STTBX_Print(("[!] Task 1 No Free Handles Count=%d \n",Count));
                    Timeout = STOS_time_plus(STOS_time_now(), (U32)(NB_TICKS_PER_MILLI_SECOND * TEST_MULTI_INSTANCES_DELAY_VALUE));
            		STOS_TaskDelayUntil(Timeout);
					Task1ErrCode = STBLIT_Blit(STBlitHandle1,NULL,&Src,&Dst,&Context );
                }
				if (Task1ErrCode != ST_NO_ERROR)
				{
                	STTBX_Print(("[!] Task 1 Err Blit Count=%d \n",Count));
                }
			}


            /* ------------ Update count ------------ */
            Count++;
        }

        STTBX_Print (("-> Task1 completed\n"));

        /* Notify main for blit end */
        STOS_SemaphoreSignal(Task1CompletedSemaphore_p);
    }
}

/*-------------------------------------------------------------------------
 * Function   : MultiInstancesTest_Task2
 * Input      :
 * Output     :
 * Return     :
 * Description:
 * ----------------------------------------------------------------------*/
static void MultiInstancesTest_Task2(void *Param_p)
{
    STBLIT_BlitContext_t    Context;
    STGXOBJ_Color_t         Color;
    STGXOBJ_Rectangle_t     Rectangle;
    U32                     i,j;
    STOS_Clock_t            time;
	STOS_Clock_t            Timeout;



    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = Task2Tag;
    Context.EnableResizeFilter      = TRUE;
    Context.EventSubscriberID       = MultiInstancesTest_SubscriberID;

    /* ------------ Init test ------------ */
    Rectangle.Width  = TEST_MULTI_INSTANCES_STEP;
    Rectangle.Height = TEST_MULTI_INSTANCES_STEP;

    Color.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888 ;
    Color.Value.ARGB8888.Alpha = 0xFF;
    Color.Value.ARGB8888.R     = 0xFF;
    Color.Value.ARGB8888.G     = 0x0;
    Color.Value.ARGB8888.B     = 0x0;

    /* ------------ Init Task ErrCode ------------ */
    Task2ErrCode = ST_NO_ERROR;

    /* ------------ Start execution ------------ */
    while (Task2IsRunning)
    {
        /* Wait for main notification */
        STOS_SemaphoreWait(Task2StartSemaphore_p);

        STTBX_Print (("-> Task2 started\n"));

        /* Init vertical counter */
        j=0;

        while ((Task2PerformBlit) && (j < (MultiInstancesTest_SourceBitmap.Height - Rectangle.Height)) && (Task2ErrCode == ST_NO_ERROR))
        {
            STTBX_Print(("--> Task 2 : Process line %d\n", j));

            /* Init horiz counter */
            i=0;

            while ((Task2PerformBlit) && (i < (MultiInstancesTest_SourceBitmap.Width - Rectangle.Width)) && (Task2ErrCode == ST_NO_ERROR))
            {
                /* ------------ Set Rectangle & Destinantion ------------ */
                Rectangle.PositionX = i;
                Rectangle.PositionY = j;

                /* ------------ Set Color ------------ */
                Color.Value.ARGB8888.B++;
                if (Color.Value.ARGB8888.B == 255)
                {
                    Color.Value.ARGB8888.B = 0;
                    Color.Value.ARGB8888.G++;
                    if (Color.Value.ARGB8888.G == 255)
                    {
                        Color.Value.ARGB8888.G = 0;
                        Color.Value.ARGB8888.R--;
                        if (Color.Value.ARGB8888.R == 0)
                        {
                            Color.Value.ARGB8888.R = 0xFF;
                        }
                    }
                }

                /* ------------ Blit operation ------------ */
                Task2ErrCode = STBLIT_FillRectangle(STBlitHandle2,&MultiInstancesTest_Target2Bitmap,&Rectangle,&Color,&Context);
				if (MultiInstancesTest_WaitForNotification)
                {
					if (Task2ErrCode != ST_NO_ERROR)
					{
                    printf("Error ............................... \n");
                        STTBX_Print(("[!] Task 2 Err Blit i=%d j=%d\n",i,j));
                	}
					else
					{
                    	/* ------------ Wait complete event ------------ */
            	#ifndef STBLIT_EMULATOR
                	#ifdef ST_OSLINUX
                        time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
                	#else
                        time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
                	#endif
                    	if(STOS_SemaphoreWaitTimeOut(Task2CompletionSemaphore_p,&time)!=0)
                    	{
                            STTBX_Print(("[!] Task 2 Timeout i=%d j=%d\n",i,j));
                    	    Task2ErrCode = ST_ERROR_TIMEOUT;
                    	}
            	#else
                    	/*  Wait for Blit to be completed */
                    	STOS_SemaphoreWaitTimeOut(Task2CompletionSemaphore_p, TIMEOUT_INFINITY);
            	#endif
					}
                }
				else /* Asynchronous mode */
				{
                	while (Task2ErrCode == STBLIT_ERROR_MAX_SINGLE_BLIT_NODE)
                	{
						STTBX_Print(("[!] Task 2 No Free Handles i=%d j=%d\n",i,j));
                        Timeout = STOS_time_plus(STOS_time_now(), (U32)(NB_TICKS_PER_MILLI_SECOND * TEST_MULTI_INSTANCES_DELAY_VALUE));
            			STOS_TaskDelayUntil(Timeout);
						Task2ErrCode = STBLIT_FillRectangle(STBlitHandle2,&MultiInstancesTest_Target2Bitmap,&Rectangle,&Color,&Context);
                	}
					if (Task2ErrCode != ST_NO_ERROR)
					{
                	    STTBX_Print(("[!] Task 2 Err Blit i=%d j=%d\n",i,j));
                	}
				}


                /* Update counter */
                i+=TEST_MULTI_INSTANCES_STEP;
            }

            /* Update counter */
            j+=(3*TEST_MULTI_INSTANCES_STEP);
        }

        STTBX_Print (("-> Task2 completed\n"));

        /* Notify main for blit end */
        STOS_SemaphoreSignal(Task2CompletedSemaphore_p);
    }
}

/*-------------------------------------------------------------------------
 * Function   : MultiInstancesTest_Task3
 * Input      :
 * Output     :
 * Return     :
 * Description:
 * ----------------------------------------------------------------------*/
static void MultiInstancesTest_Task3(void *Param_p)
{
    STBLIT_BlitContext_t    Context;
    STGXOBJ_Color_t         Color;
    STGXOBJ_Rectangle_t     Rectangle;
    U32                     i,j;
    STOS_Clock_t            time;
	STOS_Clock_t            Timeout;


    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = Task3Tag;
    Context.EnableResizeFilter      = TRUE;
    Context.EventSubscriberID       = MultiInstancesTest_SubscriberID;

    /* ------------ Init test ------------ */
    Rectangle.Width  = TEST_MULTI_INSTANCES_STEP;
    Rectangle.Height = TEST_MULTI_INSTANCES_STEP;

    Color.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888 ;
    Color.Value.ARGB8888.Alpha = 0xFF; ;
    Color.Value.ARGB8888.R     = 0x0;
    Color.Value.ARGB8888.G     = 0xFF;
    Color.Value.ARGB8888.B     = 0x0;

    /* ------------ Init Task ErrCode ------------ */
    Task3ErrCode = ST_NO_ERROR;

    /* ------------ Start execution ------------ */
    while (Task3IsRunning)
    {
        /* Wait for main notification */
        STOS_SemaphoreWait(Task3StartSemaphore_p);

        STTBX_Print (("-> Task3 started\n"));

        /* Init vertical counter */
        j=TEST_MULTI_INSTANCES_STEP;

        while ((Task3PerformBlit) && (j < (MultiInstancesTest_SourceBitmap.Height - Rectangle.Height)) && (Task3ErrCode == ST_NO_ERROR))
        {
            STTBX_Print(("--> Task 3 : Process line %d\n", j));

            /* Init horiz counter */
            i=0;

            while ((Task3PerformBlit) && (i < (MultiInstancesTest_SourceBitmap.Width - Rectangle.Width)) && (Task3ErrCode == ST_NO_ERROR))
            {
                /* ------------ Set Rectangle & Destinantion ------------ */
                Rectangle.PositionX = i;
                Rectangle.PositionY = j;

                /* ------------ Set Color ------------ */
                Color.Value.ARGB8888.R++;
                if (Color.Value.ARGB8888.R == 255)
                {
                    Color.Value.ARGB8888.R = 0;
                    Color.Value.ARGB8888.B++;
                    if (Color.Value.ARGB8888.B == 255)
                    {
                        Color.Value.ARGB8888.B = 0;
                        Color.Value.ARGB8888.G--;
                        if (Color.Value.ARGB8888.G == 0)
                        {
                            Color.Value.ARGB8888.G = 0xFF;
                        }
                    }
                }

                /* ------------ Blit operation ------------ */
                Task3ErrCode = STBLIT_FillRectangle(STBlitHandle3,&MultiInstancesTest_Target2Bitmap,&Rectangle,&Color,&Context);
				if (MultiInstancesTest_WaitForNotification)
                {
					if (Task3ErrCode != ST_NO_ERROR)
					{
                        STTBX_Print(("[!] Task 3 Err Blit i=%d j=%d\n",i,j));
                	}
					else
					{
                    	/* ------------ Wait complete event ------------ */
            	#ifndef STBLIT_EMULATOR
                	#ifdef ST_OSLINUX
                        time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
                	#else
                        time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
                	#endif
                    	if(STOS_SemaphoreWaitTimeOut(Task3CompletionSemaphore_p,&time)!=0)
                    	{
                            STTBX_Print(("[!] Task 3 Timeout i=%d j=%d\n",i,j));
                    	    Task3ErrCode = ST_ERROR_TIMEOUT;
                    	}
            	#else
                    	/*  Wait for Blit to be completed */
                    	STOS_SemaphoreWaitTimeOut(Task3CompletionSemaphore_p, TIMEOUT_INFINITY);
            	#endif
					}
                }
				else /* Asynchronous mode */
				{
                	while (Task3ErrCode == STBLIT_ERROR_MAX_SINGLE_BLIT_NODE)
                	{
						STTBX_Print(("[!] Task 3 No Free Handles i=%d j=%d\n",i,j));
                        Timeout = STOS_time_plus(STOS_time_now(), (U32)(NB_TICKS_PER_MILLI_SECOND * TEST_MULTI_INSTANCES_DELAY_VALUE));
            			STOS_TaskDelayUntil(Timeout);
						Task3ErrCode = STBLIT_FillRectangle(STBlitHandle3,&MultiInstancesTest_Target2Bitmap,&Rectangle,&Color,&Context);
                	}
					if (Task3ErrCode != ST_NO_ERROR)
					{
                	    STTBX_Print(("[!] Task 3 Err Blit i=%d j=%d\n",i,j));
                	}
				}

                /* Update counter */
                i+=TEST_MULTI_INSTANCES_STEP;
            }

            /* Update counter */
            j+=(3*TEST_MULTI_INSTANCES_STEP);
        }

        STTBX_Print (("-> Task3 completed\n"));

        /* Notify main for blit end */
        STOS_SemaphoreSignal(Task3CompletedSemaphore_p);
    }
}

/*-------------------------------------------------------------------------
 * Function   : MultiInstancesTest_Task4
 * Input      :
 * Output     :
 * Return     :
 * Description:
 * ----------------------------------------------------------------------*/
static void MultiInstancesTest_Task4(void *Param_p)
{
    STBLIT_BlitContext_t    Context;
    STGXOBJ_Color_t         Color;
    STGXOBJ_Rectangle_t     Rectangle;
    U32                     i,j;
    STOS_Clock_t            time;
	STOS_Clock_t            Timeout;


    /* ------------ Set Context ------------ */
    Context.ColorKeyCopyMode        = STBLIT_COLOR_KEY_MODE_NONE;
    /* Context.ColorKey              = dummy; */
    Context.AluMode                 = STBLIT_ALU_COPY;
    Context.EnableMaskWord          = FALSE;
/*    Context.MaskWord                = 0xFFFFFF00;*/
    Context.EnableMaskBitmap        = FALSE;
/*    Context.MaskBitmap_p            = NULL;*/
    Context.EnableColorCorrection   = FALSE;
/*    Context.ColorCorrectionTable_p  = NULL;*/
    Context.Trigger.EnableTrigger   = FALSE;
    Context.GlobalAlpha             = 50;
    Context.EnableClipRectangle     = FALSE;
    Context.WriteInsideClipRectangle = TRUE;
    Context.EnableFlickerFilter     = FALSE;
    Context.JobHandle               = STBLIT_NO_JOB_HANDLE;
/*    Context.JobHandle               = JobHandle[0];*/
    Context.NotifyBlitCompletion    = TRUE;
    Context.UserTag_p               = Task4Tag;
    Context.EnableResizeFilter      = TRUE;
    Context.EventSubscriberID       = MultiInstancesTest_SubscriberID;

    /* ------------ Init test ------------ */
    Rectangle.Width  = TEST_MULTI_INSTANCES_STEP;
    Rectangle.Height = TEST_MULTI_INSTANCES_STEP;

    Color.Type                 = STGXOBJ_COLOR_TYPE_ARGB8888 ;
    Color.Value.ARGB8888.Alpha = 0xFF; ;
    Color.Value.ARGB8888.R     = 0x0;
    Color.Value.ARGB8888.G     = 0x0;
    Color.Value.ARGB8888.B     = 0xFF;

    /* ------------ Init Task ErrCode ------------ */
    Task4ErrCode = ST_NO_ERROR;

    /* ------------ Start execution ------------ */
    while (Task4IsRunning)
    {
        /* Wait for main notification */
        STOS_SemaphoreWait(Task4StartSemaphore_p);

        STTBX_Print (("-> Task4 started\n"));

        /* Init vertical counter */
        j=(2*TEST_MULTI_INSTANCES_STEP);

        while ((Task4PerformBlit) && (j < (MultiInstancesTest_SourceBitmap.Height - Rectangle.Height)) && (Task4ErrCode == ST_NO_ERROR))
        {
            STTBX_Print(("--> Task 4 : Process line %d\n", j));

            /* Init horiz counter */
            i=0;

            while ((Task4PerformBlit) && (i < (MultiInstancesTest_SourceBitmap.Width - Rectangle.Width)) && (Task4ErrCode == ST_NO_ERROR))
            {
                /* ------------ Set Rectangle & Destinantion ------------ */
                Rectangle.PositionX = i;
                Rectangle.PositionY = j;

                /* ------------ Set Color ------------ */
                Color.Value.ARGB8888.G++;
                if (Color.Value.ARGB8888.G == 255)
                {
                    Color.Value.ARGB8888.G = 0;
                    Color.Value.ARGB8888.R++;
                    if (Color.Value.ARGB8888.R == 255)
                    {
                        Color.Value.ARGB8888.R = 0;
                        Color.Value.ARGB8888.B--;
                        if (Color.Value.ARGB8888.B == 0)
                        {
                            Color.Value.ARGB8888.B = 0xFF;
                        }
                    }
                }

                /* ------------ Blit operation ------------ */
				Task4ErrCode = STBLIT_FillRectangle(STBlitHandle4,&MultiInstancesTest_Target2Bitmap,&Rectangle,&Color,&Context);
				if (MultiInstancesTest_WaitForNotification)
                {
					if (Task4ErrCode != ST_NO_ERROR)
					{
                        STTBX_Print(("[!] Task 4 Err Blit i=%d j=%d\n",i,j));
                	}
					else
					{
                    	/* ------------ Wait complete event ------------ */
            	#ifndef STBLIT_EMULATOR
                	#ifdef ST_OSLINUX
                        time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*TEST_DELAY);
                	#else
                        time = STOS_time_plus(STOS_time_now(), ST_GetClocksPerSecond()*5);
                	#endif
                    	if(STOS_SemaphoreWaitTimeOut(Task4CompletionSemaphore_p,&time)!=0)
                    	{
                            STTBX_Print(("[!] Task 4 Timeout i=%d j=%d\n",i,j));
                    	    Task4ErrCode = ST_ERROR_TIMEOUT;
                    	}
            	#else
                    	/*  Wait for Blit to be completed */
                    	STOS_SemaphoreWaitTimeOut(Task4CompletionSemaphore_p, TIMEOUT_INFINITY);
            	#endif
					}
                }
				else /* Asynchronous mode */
				{
                	while (Task4ErrCode == STBLIT_ERROR_MAX_SINGLE_BLIT_NODE)
                	{
						STTBX_Print(("[!] Task 4 No Free Handles i=%d j=%d\n",i,j));
                        Timeout = STOS_time_plus(STOS_time_now(), (U32)(NB_TICKS_PER_MILLI_SECOND * TEST_MULTI_INSTANCES_DELAY_VALUE));
            			STOS_TaskDelayUntil(Timeout);
						Task4ErrCode = STBLIT_FillRectangle(STBlitHandle4,&MultiInstancesTest_Target2Bitmap,&Rectangle,&Color,&Context);
                	}
					if (Task4ErrCode != ST_NO_ERROR)
					{
                	    STTBX_Print(("[!] Task 4 Err Blit i=%d j=%d\n",i,j));
                	}
				}




                /* Update counter */
                i+=TEST_MULTI_INSTANCES_STEP;
            }

            /* Update counter */
            j+=(3*TEST_MULTI_INSTANCES_STEP);
        }

        STTBX_Print (("-> Task4 completed\n"));

        /* Notify main for blit end */
        STOS_SemaphoreSignal(Task4CompletedSemaphore_p);
    }
}
#endif /* defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109)  || defined (ST_7200) */


/*-------------------------------------------------------------------------
 * Function : BLIT_InitCommand
 *            Definition of the macros
 * Input    :
 * Output   :
 * Return   : FALSE if error, TRUE if success
 * ----------------------------------------------------------------------*/
static BOOL BLIT_InitCommand(void)
{
    BOOL RetErr = FALSE;

    RetErr  = STTST_RegisterCommand( "BLIT_Init", BLIT_Init, "BLIT Init");
    RetErr |= STTST_RegisterCommand( "BLIT_Term", BLIT_Term, "BLIT Term");
#if defined (DVD_SECURED_CHIP)
    RetErr |= STTST_RegisterCommand( "Blit_MesInit", Blit_MesInit, "Initialize the MEP IP for secure chip");
#endif
	RetErr |= STTST_RegisterCommand( "BLIT_BlitDemoEvtSetup", BLIT_BlitDemoEvtSetup, "Test display with BLIT");
	RetErr |= STTST_RegisterCommand( "BLIT_Display_Test", BLIT_Display_Test, "Test display with BLIT");
	RetErr |= STTST_RegisterCommand( "BLIT_SetSnakeDemoTargetBitmap", BLIT_SetSnakeDemoTargetBitmap, "Set Snake Demo Target Bitmap");
	RetErr |= STTST_RegisterCommand( "BLIT_Snake_Start", BLIT_Snake_Start, "<Width><Height><Offsetx><Offsety><Complexity><Alpha><NbrObstacles>< 1: Wait For Blit Completetion, 0: If not> Test display with Snake Demo");
    RetErr |= STTST_RegisterCommand( "BLIT_Snake_End", BLIT_Snake_End, "End Snake Demo Test");
    RetErr |= STTST_RegisterCommand( "BLIT_SetSource", BLIT_SetSource, "Set Blit Source");
	RetErr |= STTST_RegisterCommand( "BLIT_SetBlitDemoTargetBitmap", BLIT_SetBlitDemoTargetBitmap, "Set Blit Demo Target Bitmap");
	RetErr |= STTST_RegisterCommand( "BLIT_BlitDemo_Start", BLIT_BlitDemo_Start, "<Width><Height><Offsetx><Offsety><Complexity><Alpha><NbrObstacles>< 1: Wait For Blit Completetion, 0: If not> Test display with Snake Demo");
	RetErr |= STTST_RegisterCommand( "BLIT_SetRandomFillDemoTargetBitmap", BLIT_SetRandomFillDemoTargetBitmap, "Set Random Fill Demo Target Bitmap");
	RetErr |= STTST_RegisterCommand( "BLIT_RandomFillDemo_Start", BLIT_RandomFillDemo_Start, "<Width><Height><Offsetx><Offsety><Complexity><Alpha><NbrObstacles>< 1: Wait For Blit Completetion, 0: If not> Test display with Snake Demo");
	RetErr |= STTST_RegisterCommand( "BLIT_SetRandomCopyDemoTargetBitmap", BLIT_SetRandomCopyDemoTargetBitmap, "Set Random Copy Demo Target Bitmap");
	RetErr |= STTST_RegisterCommand( "BLIT_RandomCopyDemo_Start", BLIT_RandomCopyDemo_Start, "<Width><Height><Offsetx><Offsety><Complexity><Alpha><NbrObstacles>< 1: Wait For Blit Completetion, 0: If not> Test display with Snake Demo");
#ifdef ENABLE_RECCURENCE_MODE_TEST
    RetErr |= STTST_RegisterCommand( "BLIT_SetVPHandle", BLIT_SetOSDVPHandle, "Set Vp Handle");
#endif
    RetErr |= STTST_RegisterCommand( "BLIT_SetTargetBitmap", BLIT_SetTargetBitmap, "Set Target Bitmap");
    RetErr |= register_command( "BLT_GET_REV",  BLIT_GetRevision,
        "Retrieves the driver revision");

#ifndef STBLIT_TEST_FRONTEND
    RetErr |= register_command( "BLT_TBCRASH",  BLIT_TestBlitterCrashCheck , "Test Blitter Crash Check feature");
    RetErr |= register_command( "BLT_TRESIZE",  BLIT_TestResize , "test resize feature, return a .gam file");
    RetErr |= register_command( "BLT_TANTIALIASING",  BLIT_TestAntiAliasing , "test AntiAliasing feature, return .gam files");
    RetErr |= register_command( "BLT_TANTIFLUTTER",  BLIT_TestAntiFlutter , "test AntiFlutter feature, return .gam files");
    RetErr |= register_command( "BLT_TFILL",  BLIT_TestFill , "test Fill function, return a .gam file");
    RetErr |= register_command( "BLT_TCOPY",  BLIT_TestCopy , "test Copy function, return a .gam file");
    RetErr |= register_command( "BLT_TOVERLAPCOPY",  BLIT_TestOverlapCopy , "test Copy function with overlap between source and destination, return .gam files");
    RetErr |= register_command( "BLT_TOVERLAPBLIT",  BLIT_TestOverlapBlit , "test Blit function with overlap between source and destination, return .gam files");
    RetErr |= register_command( "BLT_TDRAW",  BLIT_TestDraw , "test Draw function, return a .gam file");
    RetErr |= register_command( "BLT_TPIX",  BLIT_TestPix , "test SetPixel function, return a .gam file");
    RetErr |= register_command( "BLT_TCK",  BLIT_TestColorKey , "test ColorKey feature, return a .gam file");
    RetErr |= register_command( "BLT_TALU",  BLIT_TestALU , "test ALU feature, return a .gam file");
    RetErr |= register_command( "BLT_TMW",  BLIT_TestMaskWord , "test MaskWord feature, return a .gam file");
    RetErr |= register_command( "BLT_TMBITMAP",  BLIT_TestMaskBitmap , "test MaskBitmap feature, return a .gam file");
    RetErr |= register_command( "BLT_TMBITMAPFILL",  BLIT_TestMaskBitmapFill , "test MaskBitmap feature with a fill function, return a .gam file");
    RetErr |= register_command( "BLT_TCLIP",  BLIT_TestClip , "test Clipping feature, return a .gam file");
    RetErr |= register_command( "BLT_TCOLOR",  BLIT_TestColor , "test Color conversion, return .gam files");
    RetErr |= register_command( "BLT_SETSRCFORMAT",  SetSrcFileName , "Select source format");
    RetErr |= register_command( "BLT_SETDSTFORMAT",  SetDstFileName , "Select destination format");
    RetErr |= register_command( "BLT_TCOPY2src",  BLIT_TestCopy2Src , "test Copy 2 sources function, return a .gam file");
    RetErr |= register_command( "BLT_TCONCAT",  BLIT_TestConcat , "test Concat function, return a .gam file");
    RetErr |= register_command( "BLT_TBLEND",  BLIT_TestBlend , "test blend function, return a .gam file");
    RetErr |= register_command( "BLT_TFILTERFLICKER",  BLIT_TestFilterFlicker , "test filter flicker function, return a .gam file");
    RetErr |= register_command( "BLT_TVC1RANGE",  BLIT_TestVC1Range , "test VC1 Range Mapping/Reduction, return a .gam file");
    RetErr |= register_command( "BLT_TJOB",  BLIT_TestJob , "Test Job Feature, return .gam files");
    RetErr |= register_command( "BLT_TMBConversion",  BLIT_TestMBConversion , "Test Job Feature, return .gam files");
    RetErr |= register_command( "BLT_TRASTERTOPBOTTOM",  BLIT_TestTypeRasterTopBottom , "Test STGXOBJ_BITMAP_TYPE_RASTER_TOP_BOTTOM type, return .gam files");
    RetErr |= register_command( "BLT_TEXTRACTFIELDS",  BLIT_TestExtractFields , "Extract top and bottom fields from a gam file, return two .bin files");
#endif

#if defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109) || defined (ST_7200)
	RetErr |= STTST_RegisterCommand( "BLIT_TestFeatures", BLIT_TestFeatures, "Set Target Bitmap");
    RetErr |= STTST_RegisterCommand( "BLIT_TestMultiInstances", BLIT_TestMultiInstances, "Set Target Bitmap");
#if defined(STBLIT_DEBUG_GET_STATISTICS) || defined(STBLIT_DEBUG_GET_STATISTICS_APPLICATION_LEVEL)
    RetErr |= STTST_RegisterCommand( "BLIT_TestPerformences", BLIT_TestPerformences, "Set Target Bitmap");
#endif /* STBLIT_DEBUG_GET_STATISTICS */
#endif /* defined (ST_5105) || defined (ST_7710) || defined (ST_7100) || defined (ST_7109) || defined (ST_7200) */


    return(!RetErr);
}

/*#######################################################################*/
/*########################## GLOBAL FUNCTIONS ###########################*/
/*#######################################################################*/

BOOL BLIT_RegisterCmd(void)
{
    BOOL RetOk;

    RetOk = BLIT_InitCommand();
    if ( RetOk )
    {
        STTBX_Print(( "BLIT_RegisterCmd()     \t: ok           %s\n", STBLIT_GetRevision()));
    }
    else
    {
        STTBX_Print(( "BLIT_RegisterCmd()     \t: failed !\n" ));
    }
    return(RetOk);
}
