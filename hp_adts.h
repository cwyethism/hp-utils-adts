//
//  hp_adts.h
//  HuiMediaPlayer
//
//  Created by zhihui on 27.7.21.
//  Copyright © 2021 Wyeth. All rights reserved.
//

#ifndef hp_aac_adts_h
#define hp_aac_adts_h

#include <stdio.h>

#define HP_AAC_ADTS_HEADER_SIZE 7

typedef struct hp_aac_format {
    uint32_t    data_size; // frame_size = adts_header_size + data_size
    uint32_t    sample_rate;
    uint8_t     profile;
    uint8_t     channels;
} hp_aac_format;

#ifdef __cplusplus
extern "C" {
#endif

int sample_rate_for(uint32_t index);

int sample_index_for(uint32_t rate);

int hp_adts_header_parse(uint8_t *frm_buf, hp_aac_format *fmt);

int hp_adts_header_fill(hp_aac_format *fmt, uint8_t *adts_buf);

#ifdef __cplusplus
}
#endif

#endif /* hp_aac_adts_h */
